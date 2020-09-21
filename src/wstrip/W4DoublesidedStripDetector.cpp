/**
   @file    W4DoublesidedStripDetector.cpp
   @author  Goro Yabu
   @date    2019/12/22
**/

#include "W4DoublesidedStripDetector.hpp"
#include "W4SensitiveDetector.hpp"

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

W4DoublesidedStripDetector::W4DoublesidedStripDetector()
    : P4PVConstruct("W4DoublesidedStripDetector")
{
    parameter.is_already_applied = false;

    DefineParameter<int>("verbose_level", 1);
    // DefineParameter<std::string>("world_material", "AIR");
    // DefineParameter<double,unit>("world_size", 10, sim4py::cm);
    // DefineParameter<double,double,double,unit>("world_center", 0, 0, 0, sim4py::mm);

    DefineParameter<double,unit>("detector_size", 32, sim4py::mm);
    DefineParameter<double,unit>("detector_gap", 4, sim4py::mm);
    
    DefineParameter<double,double,double,unit>
	( "detector_center", 0, 0, 0, sim4py::mm );
    DefineParameter<double,double,double>
	( "detector_normal", 0, 0, 1 );
    
    DefineParameter<bool>( "merge_same_pixel",     true );
    DefineParameter<bool>( "merge_adjacent_pixel", true );

    DefineParameter<bool>( "use_detector_board", true );
    DefineParameter<std::string>( "board_material", "SiO2" );
    DefineParameter<double,unit>( "board_size", 52, sim4py::mm );
    DefineParameter<double,unit>( "board_thickness", 1.0, sim4py::mm );    
}

W4DoublesidedStripDetector::~W4DoublesidedStripDetector()
{
}

void W4DoublesidedStripDetector::Construct
(G4LogicalVolume* logical_world)
{
    if ( parameter.verbose_level>0 ) {
	cout << endl;
	cout << "### Into DetectorConstruction::Construct() ###" << endl;
    }
    
    parameter.ApplyParameters(this);

    this->PrepareMaterials();    
    
    auto nist_manager = G4NistManager::Instance();
    auto sd_manager   = G4SDManager::GetSDMpointer();
    
    /* Definition of Detector */
    auto size_det    = parameter.detector_size*0.5;
    auto gap_det     = parameter.detector_gap;
    if ( parameter.detector_normal == G4ThreeVector(0,0,-1) ) {
	gap_det = -gap_det;
    }

    auto merge_same_pixel     = parameter.is_enabled_merge_same_pixel;
    auto merge_adjacent_pixel = parameter.is_enabled_merge_adjacent_pixel;
    
    auto ndet = (int)layers_info.size();
    if ( ndet>0 && parameter.verbose_level>0 )
	cout << "===== Construct Geometry =====" << endl;       

    auto board_material
	= nist_manager->FindOrBuildMaterial( parameter.board_material );
    auto board_size      = parameter.board_size*0.5;
    auto board_thickness = parameter.board_thickness*0.5;
    
    for ( auto layer : layers_info ) {

	auto detector_id = layer.detector_id;
	auto user_detector_id = layer.user_detector_id;
	auto idname   = std::to_string(user_detector_id);
	auto thick    = layer.thickness*0.5*sim4py::mm;
	auto matename = layer.material;
	
	auto mate = nist_manager->FindOrBuildMaterial( matename );

	if ( parameter.verbose_level>0 ) {

	    cout << "### Detector ";
	    cout << user_detector_id << std::right << std::setw(5) << "###";
	    // cout << "  DETID     : " << user_detector_id << std::right << std::setw(5);
	    cout << endl;
	    cout << "  Size      : " << size_det*2.0 << std::setw(5) << "mm\n";
	    cout << "  Material  : " << matename << std::setw(10) << endl;
	    cout << "  Thickness : " << thick << std::setw(5) << " mm " << endl;
	    
	    if ( parameter.is_enabled_board ) {
		cout << "  - Board Info" << endl;
		cout << "    Material  : " << parameter.board_material << endl;
		cout << "    Size      : " << parameter.board_size << " mm\n";
		cout << "    Thickness : " << parameter.board_thickness << "mm\n"; 
	    }
	    else
		cout << "  Without Board" << endl;

	    cout << endl;
	}
	
	auto box = new G4Box( "BoxDSD", size_det, size_det, thick );
	auto log = new G4LogicalVolume( box, mate, "LogicalDSD"+idname );
	auto pos = parameter.detector_center
	    + G4ThreeVector( 0, 0, -detector_id*gap_det );	
	new G4PVPlacement( 0, pos, log, "PhysicalDSD"+idname,
			   logical_world, false, 0 );

	if ( parameter.is_enabled_board ) {
	    auto solid_board = new G4Box( "solid_board", board_size, board_size,
					  board_thickness );
	    auto log_board = new G4LogicalVolume( solid_board, board_material,
						  "logical_board"+idname );
	    auto pos_board = pos + G4ThreeVector( 0, 0,
						  +1*(thick+board_thickness) );	
	    new G4PVPlacement( 0, pos_board, log_board, "phys_board"+idname,
			       logical_world, false, 0 );
	}
	
	auto sd = new W4SensitiveDetector( "mySensitiveDetector"+idname );
	sd->SetParameter<bool>("merge_same_pixel", merge_same_pixel);
	sd->SetParameter<bool>("merge_adjacent_pixel", merge_adjacent_pixel);
	sd->SetParameter<int>("verbose_level", parameter.verbose_level);
	sd->SetGridXaxis( 128, pos.x()-size_det, pos.x()+size_det );
	sd->SetGridYaxis( 128, pos.y()-size_det, pos.y()+size_det );
	sd->SetGridZaxis(   1, pos.z()-thick,    pos.z()+thick    );
	sd->SetDetectorID( user_detector_id );

	if ( matename=="Si" )
	    sd->SetDetectorMaterial(0);
	else if ( matename=="CdTe" )
	    sd->SetDetectorMaterial(1);
	else
	    sd->SetDetectorMaterial(-1);
	
	sd_manager->AddNewDetector( sd );
	log->SetSensitiveDetector( sd );

	// this->SetSensitiveDetector( log, sd );
	++detector_id;
    }
    
    if ( parameter.verbose_level>0 ) {
	cout << "### Exit from DetectorConstruction::Construct() ###" << endl;
	cout << endl;
    }
}

void W4DoublesidedStripDetector::PrepareMaterials()
{
    G4NistManager * nist_manager = G4NistManager::Instance();

    if ( !nist_manager->FindOrBuildMaterial( "Si" ) ) 
	new G4Material( "Si", 14., 28.09*g/mole, 2.330*g/cm3 );    
    if ( !nist_manager->FindOrBuildMaterial( "Cu" ) ) 
	new G4Material( "Cu", 29., 63.55*g/mole, 8.960*g/cm3 );    
    if ( !nist_manager->FindOrBuildMaterial( "Al" ) ) 
	new G4Material( "Al", 13., 26.98*g/mole, 2.7*g/cm3 );
    
    if ( !nist_manager->FindOrBuildMaterial( "CdTe" ) ) {
	auto Cd = new G4Element( "Cadmium",   "Cd", 48., 112.41*g/mole );
	auto Te = new G4Element( "Tellurium", "Te", 52., 127.60*g/mole );
	auto CdTe = new G4Material( "CdTe", 5.85*g/cm3, 2 );
	CdTe->AddElement( Cd, 1 );
	CdTe->AddElement( Te, 1 );
    }
    if ( !nist_manager->FindOrBuildMaterial( "SiO2" ) ) {
	G4Element* O  = new G4Element( "Oxygen",  "O",  8.0,  16.00*g/mole );
	G4Element* Si = new G4Element( "Silicon", "Si", 14.0, 28.09*g/mole );
	G4Material* SiO2 = new G4Material( "SiO2", 2.200*g/cm3, 2 );
	SiO2->AddElement( Si, 1 );
	SiO2->AddElement( O , 2 );
    }    

    if ( parameter.verbose_level>1 ) 
	cout << *(G4Material::GetMaterialTable()) << endl;
}

W4DoublesidedStripDetector* W4DoublesidedStripDetector::AddDetectorLayer(const std::string& material, double thickness)
{
    layer_info newdet;

    int id = layers_info.size();
    newdet.detector_id = id;
    newdet.user_detector_id = id;
    newdet.material = material;
    newdet.thickness = thickness;
    layers_info.emplace_back( newdet );
    
    return this;
}

W4DoublesidedStripDetector* W4DoublesidedStripDetector::AddDetectorLayer(int user_detector_id, const std::string& material, double thickness)
{
    layer_info newdet;

    newdet.detector_id = (int)layers_info.size();
    newdet.user_detector_id = user_detector_id;
    newdet.material = material;
    newdet.thickness = thickness;
    layers_info.emplace_back( newdet );
    
    return this;
}

void W4DoublesidedStripDetector::parameters_list::ApplyParameters
(sim4py::ParameterGene* module)
{
    if ( this->is_already_applied ) return;
    cout << "Applying Parameters of W4DoublesidedStripDetector Once" << endl;
    
    this->is_already_applied = true;

    auto [ ver ] = module->GetParameter<int>("verbose_level");
    this->verbose_level = ver;
    
    // auto [ wld_mate ] = module->GetParameter<std::string>("world_material");
    // this->world_material = wld_mate;
    // auto [ wsize, wsizeu ] = module->GetParameter<double,unit>("world_size");
    // this->world_size = wsize*wsizeu;
    // auto [ wx, wy, wz, wu ]
    // 	= module->GetParameter<double,double,double,unit>("world_center");
    // this->world_center = G4ThreeVector( wx*wu, wy*wu, wz*wu );

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

    auto [ use_board ] = module->GetParameter<bool>("use_detector_board");
    this->is_enabled_board = use_board;
    
    auto [ bo_mate ] = module->GetParameter<std::string>("board_material");
    if ( bo_mate=="Aluminum" ) bo_mate = "Al";
    else if ( bo_mate=="Copper" ) bo_mate = "Cu";
    this->board_material = bo_mate;
    
    auto [ bsize, u_bsize ] = module->GetParameter<double,unit>("board_size");
    this->board_size = bsize*u_bsize;
    
    auto [ bthic, u_bthic ]
	= module->GetParameter<double,unit>("board_thickness");
    this->board_thickness = bthic*u_bthic;
    
}
