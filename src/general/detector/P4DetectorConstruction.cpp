/**
   @file    P4DetectorConstruction.cpp
   @author  Goro Yabu
   @date    2019/12/22
**/

#include "P4DetectorConstruction.hpp"

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
using std::cerr;
using std::endl;

using sim4py::unit;

P4DetectorConstruction::P4DetectorConstruction()
    : sim4py::ParameterGene("P4DetectorConstruction"), G4VUserDetectorConstruction()
{
    DefineParameter<int>("verbose_level", 1);
    DefineParameter<std::string>("world_material", "AIR");
    DefineParameter<double,unit>("world_size", 10, sim4py::cm);
    DefineParameter<double,double,double,unit>("world_center", 0, 0, 0, sim4py::mm);
}

P4DetectorConstruction::~P4DetectorConstruction()
{
}

G4VPhysicalVolume* P4DetectorConstruction::Construct()
{
    // cout << "DetectorConstruction::Construct()" << endl;

    G4GeometryManager::GetInstance()->OpenGeometry();
    G4PhysicalVolumeStore::GetInstance()->Clean();
    G4LogicalVolumeStore::GetInstance()->Clean();
    G4SolidStore::GetInstance()->Clean();
    
    G4NistManager * nist_manager = G4NistManager::Instance();

    /* Definition of World */
    // auto size_wld     = 5*cm;
    // auto material_wld = nist_manager->FindOrBuildMaterial( "G4_AIR" );

    auto [ world_size, size_unit ] = GetParameter<double,unit>("world_size");
    auto size_wld = world_size*size_unit*0.5;

    auto [ world_material ] = GetParameter<std::string>("world_material"); 
    auto material_wld = nist_manager->FindOrBuildMaterial( "G4_"+world_material );

    auto [ wx, wy, wz, wu ]
        = GetParameter<double,double,double,unit>("world_center");
    auto world_center = G4ThreeVector( wx*wu, wy*wu, wz*wu );
    
    auto box_wld      = new G4Box( "BoxWorld", size_wld, size_wld, size_wld );
    auto logical_wld  = new G4LogicalVolume( box_wld, material_wld, "LogicalWorld" );
    auto physical_wld = new G4PVPlacement( 0, world_center, logical_wld,
					   "PhysicalWorld", 0, false, 0 ); 

    cout << "Construct Volumes..." << endl;
    for ( auto volume : vector_of_pvconstructs ) {
	if ( volume==nullptr ) {
	    cerr << "***Error*** in P4DetectorConstruction::Construct";
	    cerr << " : A pointer of P4PVConstruct is already deleted." << endl;
	    continue;
	}
	volume->Construct( logical_wld );
    }
    
    return physical_wld;
}

P4DetectorConstruction* P4DetectorConstruction::AddVolume(P4PVConstruct* volume)
{
    if ( volume==nullptr ) {
	cerr << "***Error*** in P4DetectorConstruction::AddVolume";
	cerr << " : Attemp to set nullptr" << endl;
    }
    else
	vector_of_pvconstructs.emplace_back( volume );
    return this;
}
