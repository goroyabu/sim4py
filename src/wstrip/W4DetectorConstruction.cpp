/**
   @file    W4DetectorConstruction.cpp
   @author  Goro Yabu
   @date    2019/12/22
**/

#include "W4DetectorConstruction.hpp"
#include "W4SensitiveDetector.hpp"

#include "GeometryCollimatorMINE1.hpp"

#include <iomanip>
#include <iostream>
#include <string>

#include <G4UnitsTable.hh>
#include <G4SystemOfUnits.hh>

#include <G4Element.hh>
#include <G4Material.hh>
#include <G4NistManager.hh>

#include <G4Box.hh>
#include <G4LogicalVolume.hh>
#include <G4VPhysicalVolume.hh>
#include <G4PVPlacement.hh>

#include <G4SDManager.hh>
#include <G4GeometryManager.hh>
#include <G4PhysicalVolumeStore.hh>
#include <G4LogicalVolumeStore.hh>
#include <G4SolidStore.hh>

#include <G4RunManager.hh>
#include <G4StateManager.hh>

using std::cout;
using std::endl;
using std::cerr;

using sim4py::unit;

// W4DetectorConstruction* W4DetectorConstruction::instance = 0;
W4DetectorConstruction::W4DetectorConstruction()
    : sim4py::ParameterGene("W4DetectorConstruction"),
      G4VUserDetectorConstruction()
{
    // cout << "W4DetectorConstruction::W4DetectorConstruction()" << endl;
    // SetParameter<std::string>("class_name", "W4DetectorConstruction");

    parameter.is_already_applied = false;

    DefineParameter<int>("verbose_level", 1);
    DefineParameter<std::string>("world_material", "AIR");
    DefineParameter<double,unit>("world_size", 10, sim4py::cm);
    DefineParameter<double,double,double,unit>("world_center", 0, 0, 0, sim4py::mm);

    DefineParameter<double,unit>("detector_size", 32, sim4py::mm);
    DefineParameter<double,unit>("detector_gap", 4, sim4py::mm);
    
    DefineParameter<double,double,double,unit>
	("detector_center", 0, 0, 0, sim4py::mm);
    DefineParameter<double,double,double>
	("detector_normal", 0, 0, 1);
    
    DefineParameter<bool>("merge_same_pixel", true);
    DefineParameter<bool>("merge_adjacent_pixel", true);       
    
    // map_key2str["worldMate"]        = "AIR";
    // map_key2val["worldSize"]        = 10*cm;
    // map_key2vec["worldCenter"]      = G4ThreeVector(0,0,0);

    // map_key2str["detectorMate"]     = "CdTe";
    // map_key2val["detectorSize"]     = 32*mm;
    // map_key2val["detectorGap"]      = 4*mm;
    // map_key2vec["detectorCenter"]   = G4ThreeVector(0,0,0);

    // G4NistManager * nist_manager = G4NistManager::Instance();
    // if ( !nist_manager->FindOrBuildMaterial("CdTe") ) {
    // 	auto Cd = new G4Element("Cadmium",   "Cd", 48., 112.41*g/mole);
    // 	auto Te = new G4Element("Tellurium", "Te", 52., 127.60*g/mole);
    // 	auto CdTe = new G4Material("CdTe", 5.85*g/cm3, 2);
    // 	CdTe->AddElement(Cd, 1);
    // 	CdTe->AddElement(Te, 1);
    // }
    // if ( !nist_manager->FindOrBuildMaterial("Si") ) {
    // 	new G4Material("Si", 14., 28.09*g/mole, 2.330*g/cm3);
    // }
    
    // G4cout << *(G4Material::GetMaterialTable()) << G4endl;

    // instance = this;
}

W4DetectorConstruction::~W4DetectorConstruction()
{
    // instance = 0;
}

G4VPhysicalVolume* W4DetectorConstruction::Construct()
{
    //if ( parameter.is_already_applied==false )
    parameter.ApplyParameters(this);

    if ( parameter.verbose_level>0 )
	cout << "DetectorConstruction::Construct()" << endl;

    this->PrepareMaterials();    
    G4GeometryManager::GetInstance()->OpenGeometry();
    G4PhysicalVolumeStore::GetInstance()->Clean();
    G4LogicalVolumeStore::GetInstance()->Clean();
    G4SolidStore::GetInstance()->Clean();
    
    G4NistManager * nist_manager = G4NistManager::Instance();
    G4SDManager   * sd_manager   = G4SDManager::GetSDMpointer();

    /* Definition of World */
    // auto size_wld     = this->map_key2val["worldSize"]*0.5;    
    //auto matestr_wld  = "G4_"+this->map_key2str["worldMate"];
    auto size_wld    = parameter.world_size*0.5;
    auto matestr_wld = "G4_"+parameter.world_material;
    
    auto material_wld = nist_manager->FindOrBuildMaterial( matestr_wld );
    auto box_wld      = new G4Box( "BoxWorld", size_wld, size_wld, size_wld );
    auto logical_wld  = new G4LogicalVolume( box_wld, material_wld, "LogicalWorld" );
    auto physical_wld = new G4PVPlacement( 0, G4ThreeVector(0,0,0), logical_wld, "PhysicalWorld", 0, false, 0 ); 
    
    /* Definition of Detector */
    // auto size_det     = this->map_key2val["detectorSize"]*0.5;
    // auto gap_det      = this->map_key2val["detectorGap"];
    auto size_det    = parameter.detector_size*0.5;
    auto gap_det     = parameter.detector_gap;
    if ( parameter.detector_normal == G4ThreeVector(0,0,-1) ) {
	gap_det = -gap_det;
    }

    // auto [ merge_same_pixel ] = GetParameter<bool>("merge_same_pixel");
    // auto [ merge_adjacent_pixel ] = GetParameter<bool>("merge_adjacent_pixel"); 
    auto merge_same_pixel     = parameter.is_enabled_merge_same_pixel;
    auto merge_adjacent_pixel = parameter.is_enabled_merge_adjacent_pixel;
    
    // auto ndet = (int)vec_detthic.size();
    auto ndet = (int)layers_info.size();
    if ( ndet>0 && parameter.verbose_level>0 )
	cout << "***** Construct Geometry *****" << endl;       

    int detector_id = 0;
    for ( auto layer : layers_info ) {

	auto idname   = std::to_string(detector_id);
	auto thick    = layer.thickness*0.5*sim4py::mm;
	auto matename = layer.material;
	
	auto mate = nist_manager->FindOrBuildMaterial( matename );

	if ( parameter.verbose_level>0 ) {
	    cout << "  DETID     : " << detector_id << std::right << std::setw(5) << endl;
	    cout << "  Material  : " << matename << std::setw(10) << endl;
	    cout << "  Thickness : " << thick << std::setw(5) << " mm " << endl;
	    cout << endl;
	}
	
	auto box = new G4Box( "BoxDetector", size_det, size_det, thick );
	auto log = new G4LogicalVolume( box, mate, "LogicalDetector"+idname );
	auto pos = parameter.detector_center
	    + G4ThreeVector( 0, 0, -detector_id*gap_det );
	
	new G4PVPlacement( 0, pos, log, "PhysicalDetector"+idname,
			   logical_wld, false, 0 );
	
	auto sd = new W4SensitiveDetector( "mySensitiveDetector"+idname );
	sd->SetParameter<bool>("merge_same_pixel", merge_same_pixel);
	sd->SetParameter<bool>("merge_adjacent_pixel", merge_adjacent_pixel);
	sd->SetGridXaxis( 128, -size_det,  size_det  );
	sd->SetGridYaxis( 128, -size_det,  size_det  );
	sd->SetGridZaxis(   1,         0,  thick );
	sd->SetDetectorID( detector_id );
	sd_manager->AddNewDetector( sd );

	this->SetSensitiveDetector( log, sd );
	++detector_id;
    }
    
    // for ( int i=0; i<ndet; ++i) {
    // 	auto idstr = std::to_string(i);
    // 	auto thick_det = vec_detthic[i]*0.5;
    // 	auto matestr_det = vec_detmate[i];
    // 	auto material_det = nist_manager->FindOrBuildMaterial( matestr_det );
	
    // 	cout << "  DETID     : " << i << std::right << std::setw(5) << endl;
    // 	cout << "  Material  : " << matestr_det << std::setw(10) << endl;
    // 	cout << "  Thickness : " << thick_det << std::setw(5) << " mm " << endl;
    // 	cout << endl;
	
    // 	auto box_det_itr            = new G4Box( "BoxDetector", size_det, size_det, thick_det );
    // 	auto logical_det_itr        = new G4LogicalVolume( box_det_itr, material_det, "LogicalDetector"+idstr );

    // 	auto det_pos = parameter.detector_center + G4ThreeVector( 0, 0, -i*gap_det );
	
    // 	new G4PVPlacement( 0, G4ThreeVector( 0, 0, -i*gap_det ), logical_det_itr, "PhysicalDetector"+idstr, logical_wld, false, 0 );
	
    // 	auto sensitive_detector_itr = new W4SensitiveDetector( "mySensitiveDetector"+idstr );
    // 	sensitive_detector_itr->SetParameter<bool>("merge_same_pixel", merge_same_pixel);
    // 	sensitive_detector_itr->SetParameter<bool>("merge_adjacent_pixel", merge_adjacent_pixel);
    // 	sensitive_detector_itr->SetGridXaxis( 128, -size_det,  size_det  );
    // 	sensitive_detector_itr->SetGridYaxis( 128, -size_det,  size_det  );
    // 	sensitive_detector_itr->SetGridZaxis(   1,         0,  thick_det );
    // 	sensitive_detector_itr->SetDetectorID(i);
    // 	sd_manager->AddNewDetector( sensitive_detector_itr );
    // 	this->SetSensitiveDetector( logical_det_itr, sensitive_detector_itr );	
    // }    

    // auto use_collimetor = true;
    // if ( use_collimetor ) {
    // 	auto collimator = new mineapp::GeometryCollimatorMINE1();
    // 	collimator->Construct( logical_wld );
    // }

    if ( vector_of_subgeom.empty()==false )
	cout << "Sub-Geometry Construcntion..." << endl;
    else
	cout << "Detector Construction has no sub-geometries" << endl;
    
    for ( auto geom : vector_of_subgeom ) 
    	geom->Construct( logical_wld );    
    
    if ( parameter.verbose_level>1 )
	cout << "DetectorConstruction::Construct() End" << endl;
    return physical_wld;
}

void W4DetectorConstruction::PrepareMaterials()
{
    G4NistManager * nist_manager = G4NistManager::Instance();
    if ( !nist_manager->FindOrBuildMaterial("CdTe") ) {
	auto Cd = new G4Element("Cadmium",   "Cd", 48., 112.41*g/mole);
	auto Te = new G4Element("Tellurium", "Te", 52., 127.60*g/mole);
	auto CdTe = new G4Material("CdTe", 5.85*g/cm3, 2);
	CdTe->AddElement(Cd, 1);
	CdTe->AddElement(Te, 1);
    }
    if ( !nist_manager->FindOrBuildMaterial("Si") ) {
	new G4Material("Si", 14., 28.09*g/mole, 2.330*g/cm3);
    }

    if ( parameter.verbose_level>1 ) 
	cout << *(G4Material::GetMaterialTable()) << endl;
}

W4DetectorConstruction* W4DetectorConstruction::AddGeometry
(P4GeometryConstruction* geom)
{
    if ( geom==nullptr ) {
	cerr << "***Error*** in W4DetectorConstruction::AddGeometry : ";
	cerr << "Attemp to set a nullptr" << endl;
	return this;
    }
    
    this->vector_of_subgeom.emplace_back( geom );
    return this;
}

// W4DetectorConstruction* W4DetectorConstruction::Instance()
// {
//     if ( instance==nullptr ) {
// 	new W4DetectorConstruction();
//     }
//     // else {
//     // }
//     return instance;	
// }

// int W4DetectorConstruction::SetNumberOf(const G4String& key, G4int number)
// {
//     if ( this->map_key2int.find(key) == this->map_key2int.end() ){
// 	G4cerr << "*Warning* in W4DetectorConstruction::SetNumberOf : "
// 	       << "There is no command key named " << key << G4endl;
// 	return -1;
//     }
//     this->map_key2int[key] = number;
    
//     if ( G4StateManager::GetStateManager()->GetCurrentState() != G4State_PreInit )
// 	G4RunManager::GetRunManager()->ReinitializeGeometry();
//     return 0;    
// }
// int W4DetectorConstruction::SetLengthOf(const G4String& key, G4double value)
// {
//     if ( this->map_key2val.find(key) == this->map_key2val.end() ){
// 	G4cerr << "*Warning* in W4DetectorConstruction::SetLengthOf : "
// 	       << "There is no command key named " << key << G4endl;
// 	return -1;
//     }
//     this->map_key2val[key] = value;
    
//     if ( G4StateManager::GetStateManager()->GetCurrentState() != G4State_PreInit )
// 	G4RunManager::GetRunManager()->ReinitializeGeometry();
//     return 0;
// }

int W4DetectorConstruction::AddDetectorLayer(const std::string& material, double thickness)
{
    // vec_detmate.emplace_back(material.c_str());
    // vec_detthic.emplace_back(thickness);

    layer_info newdet;

    newdet.material = material;
    newdet.thickness = thickness;
    layers_info.emplace_back( newdet );
    
    return 0;
}

void W4DetectorConstruction::parameters_list::ApplyParameters
(sim4py::ParameterGene* module)
{
    if ( this->is_already_applied ) return;
    this->is_already_applied = true;

    auto [ ver ] = module->GetParameter<int>("verbose_level");
    this->verbose_level = ver;
    
    auto [ wld_mate ] = module->GetParameter<std::string>("world_material");
    this->world_material = wld_mate;

    auto [ wsize, wsizeu ] = module->GetParameter<double,unit>("world_size");
    this->world_size = wsize*wsizeu;

    auto [ wx, wy, wz, wu ]
	= module->GetParameter<double,double,double,unit>("world_center");
    this->world_center = G4ThreeVector( wx*wu, wy*wu, wz*wu );

    auto [ dsize, dunit ] = module->GetParameter<double,unit>("detector_size");
    this->detector_size = dsize*dunit;

    auto [ dgap, dgunit ] = module->GetParameter<double,unit>("detector_gap");
    this->detector_gap = dgap*dgunit;
    
    auto [ px, py, pz, pu ]
	= module->GetParameter<double,double,double,unit>("detector_center");
    this->detector_center = G4ThreeVector( px*pu, py*pu, pz*pu );

    auto [ nx, ny, nz ]
	= module->GetParameter<double,double,double>("detector_normal");
    this->detector_normal = G4ThreeVector( nx, ny, nz );
    
    auto [ same_pix ] = module->GetParameter<bool>("merge_same_pixel");
    this->is_enabled_merge_same_pixel = same_pix;
    
    auto [ adja_pix ] = module->GetParameter<bool>("merge_adjacent_pixel");
    this->is_enabled_merge_adjacent_pixel = adja_pix;
}
