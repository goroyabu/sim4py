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
using std::endl;

P4DetectorConstruction::P4DetectorConstruction()
    : sim4py::ParameterGene("P4DetectorConstruction"), G4VUserDetectorConstruction()
{
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
    // G4SDManager   * sd_manager   = G4SDManager::GetSDMpointer();

    /* Definition of World */
    auto size_wld     = 5*cm;
    auto material_wld = nist_manager->FindOrBuildMaterial( "G4_AIR" );
    auto box_wld      = new G4Box( "BoxWorld", size_wld, size_wld, size_wld );
    auto logical_wld  = new G4LogicalVolume( box_wld, material_wld, "LogicalWorld" );
    auto physical_wld = new G4PVPlacement( 0, G4ThreeVector(0,0,0), logical_wld,
					   "PhysicalWorld", 0, false, 0 ); 
    
    return physical_wld;
}

