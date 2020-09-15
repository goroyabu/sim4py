/**
   @file   W4AirContainBox.cpp
   @author Goro Yabu
   @date   2020/09/15
**/

#include "W4AirContainBox.hpp"
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

#include <G4SubtractionSolid.hh>

#include <G4RunManager.hh>
#include <G4StateManager.hh>

using std::cout;
using std::endl;
using std::cerr;

using sim4py::unit;

W4AirContainBox::W4AirContainBox()
    : P4PVConstruct("W4AirContainBox")
{
    // parameter.is_already_applied = false;

    DefineParameter<int>("verbose_level", 1);
    DefineParameter<std::string>("box_material", "Al");
    DefineParameter<std::string>("content_material", "AIR");

    DefineParameter<double,double,double,unit>
	("box_center", 0, 0, 24+95, sim4py::mm);
    DefineParameter<double,unit>("box_thickness_x", 1.0, sim4py::mm);
    DefineParameter<double,unit>("box_thickness_y", 6.0, sim4py::mm);
    DefineParameter<double,unit>("box_thickness_z", 1.0, sim4py::mm);
    DefineParameter<double,unit>("box_length_x", 310, sim4py::mm);
    DefineParameter<double,unit>("box_length_y", 200, sim4py::mm);
    DefineParameter<double,unit>("box_length_z", 190, sim4py::mm);
    // DefineParameter<double,unit>("box_thickness", 1.0, sim4py::mm);
    // DefineParameter<double,unit>("box_size", 10, sim4py::cm);       
}

W4AirContainBox::~W4AirContainBox()
{
}

void W4AirContainBox::Construct
(G4LogicalVolume* logical_world)
{
    auto [ verbose_level ] = GetParameter<int>("verbose_level");
    
    if ( verbose_level>0 ) {
        cout << endl;
        cout << "### Into W4AirContainBox::Construct() ###" << endl;
    }

    // auto nist_manager = G4NistManager::Instance();
    // auto sd_manager   = G4SDManager::GetSDMpointer();

    // auto [ box_material_name ] = GetParameter<std::string>("box_material");

    double z, a, density;
    auto box_material =
	new G4Material("Aluminum", z= 13., a= 26.98*g/mole, density= 2.7*g/cm3);
    // auto box_material =
    // 	nist_manager->FindOrBuildMaterial( box_material_name );
    
    // auto [ content_material_name ] =
    //  GetParameter<std::string>("content_material");
    // auto content_material =
    // 	nist_manager->FindOrBuildMaterial( "G4_"+content_material_name );
    
    auto [ lx, u_lx ] = GetParameter<double,unit>("box_length_x");
    auto [ ly, u_ly ] = GetParameter<double,unit>("box_length_y");
    auto [ lz, u_lz ] = GetParameter<double,unit>("box_length_z");
    auto solid_box = new G4Box( "solid_box",
				lx*u_lx*0.5,
				ly*u_ly*0.5,
				lz*u_lz*0.5 );

    auto [ tx, u_tx ] = GetParameter<double,unit>("box_thickness_x");
    auto [ ty, u_ty ] = GetParameter<double,unit>("box_thickness_y");
    auto [ tz, u_tz ] = GetParameter<double,unit>("box_thickness_z");    
    auto solid_content = new G4Box( "solid_content",
				    (lx*u_lx-2.0*tx*u_tx)*0.5,
				    (ly*u_ly-2.0*ty*u_ty)*0.5,
				    (lz*u_lz-2.0*tz*u_tz)*0.5 );
   
    auto solid_outer_box = new G4SubtractionSolid( "solid_outer_box",
						   solid_box,
						   solid_content     );
    
    auto logical_outer_box = new G4LogicalVolume( solid_outer_box,
						  box_material,
						  "logical_outer_box" );       


    // auto logical_content = new G4LogicalVolume( solid_content,
    // 						content_material,
    // 						"logical_content" );
    
    auto [ px, py, pz, pu ] =
	GetParameter<double,double,double,unit>("box_center");
    auto center_outer_box = G4ThreeVector( px*pu, py*pu, pz*pu );
    auto rotation_outer_box = G4RotationMatrix();
    auto transform3d_outer_box
	= G4Transform3D( rotation_outer_box, center_outer_box );
    
    // new G4PVPlacement( transform3d_outer_box, logical_outer_box,
    // 		       "W4AirContainBox_Box", logical_content,
    // 		       false, 0, true );		       
    
    new G4PVPlacement( 0, center_outer_box,
		       logical_outer_box, "W4AirContainBox_Outer",
		       logical_world, false, 0 );    
    
    if ( verbose_level>0 ) {
	cout << "### Exit from W4AirContainBox::Construct() ###" << endl;
        cout << endl;
    }
}

