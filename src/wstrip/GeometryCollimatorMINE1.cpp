/**
   @date 2020/07/30 adapt for sim4py by Goro Yabu
   @detail This file is inherited from GeometryCollimatorMINE1.cc in mine_app
**/
#include <iostream>
using std::cout;
using std::cerr;
using std::endl;

#include <sstream>

#include "GeometryCollimatorMINE1.hpp"
using sim4py::unit;

// #include "AHG4MaterialManager.hh"

#include "G4Material.hh"
#include "G4NistManager.hh"
#include "G4PVPlacement.hh"
#include "G4PVParameterised.hh"
#include "G4LogicalVolume.hh"
#include "G4VPhysicalVolume.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4Cons.hh"
#include "G4Polyhedra.hh"
#include "G4SubtractionSolid.hh"
#include "G4RotationMatrix.hh"
#include "G4VisAttributes.hh"
#include "G4Colour.hh"

#include "G4UnionSolid.hh"
#include "G4IntersectionSolid.hh"


using namespace mineapp;

GeometryCollimator::GeometryCollimator(const std::string& name)
    : P4GeometryConstruction(name)
{
    DefineParameter<std::string>("preset_name", "MINE1");
    DefineParameter<bool>("use_preset_collimator", true);

    DefineParameter<std::string>("collimator_matrial", "Tungsten");
    DefineParameter("hole_radius", 0.50, sim4py::mm);
    DefineParameter("septa_thickness", 0.11, sim4py::mm);
    DefineParameter("collimator_thickness", 10.44*2.0, sim4py::mm);    
    DefineParameter("position", 0.0, 0.0, 0.0, sim4py::mm);    
    DefineParameter("n_of_hole_columns", 15);
    DefineParameter("n_of_hole_raws", 17);
}

GeometryCollimatorMINE1::GeometryCollimatorMINE1()
    : GeometryCollimator("GeometryCollimatorMINE")
{
}

void GeometryCollimatorMINE1::Construct(G4LogicalVolume* LocalWorld_Logical)
{
    cout << "GeometryCollimatorMINE1::Construct" << endl;
    
    auto [ use_preset ] = GetParameter<bool>("use_preset_collimator");

    if ( use_preset==true ) {
	auto [ preset_name ] = GetParameter<std::string>("preset_name");
	
	if ( preset_name=="MINE1" ) {
	    SetParameter<std::string>("collimator_matrial", "Tungsten"); 
	    SetParameter("hole_radius", 0.50, sim4py::mm);
	    SetParameter("septa_thickness", 0.11, sim4py::mm);
	    SetParameter("collimator_thickness", 10.44*2.0, sim4py::mm);
	    SetParameter("n_of_hole_columns", 15);
	    SetParameter("n_of_hole_raws", 17);
	    cout << "Use " << preset_name << " as collimator" << endl;
	}
	else if ( preset_name=="MINE2" ) {
	    SetParameter<std::string>("collimator_matrial", "Tungsten"); 
	    SetParameter("hole_radius", 0.136818181818, sim4py::mm);
	    SetParameter("septa_thickness", 0.0224063646098, sim4py::mm);
	    SetParameter("collimator_thickness", 2.53361552021*2.0, sim4py::mm);
	    SetParameter("n_of_hole_columns", 57);
	    SetParameter("n_of_hole_raws", 65);
	    cout << "Use " << preset_name << " as collimator" << endl;
	}
	else if ( preset_name=="TORAY1" ) {
	    SetParameter<std::string>("collimator_matrial", "Tungsten"); 
	    SetParameter("hole_radius", 0.4330127, sim4py::mm);
	    SetParameter("septa_thickness", 0.16, sim4py::mm);
	    SetParameter("collimator_thickness", 5.0*2.0, sim4py::mm);
	    SetParameter("n_of_hole_columns", 8);
	    SetParameter("n_of_hole_raws", 9);
	    cout << "Use " << preset_name << " as collimator" << endl;
	}
	else if ( preset_name=="LEHR" ) {
	    SetParameter<std::string>("collimator_matrial", "Lead"); 
	    SetParameter("hole_radius", 0.555, sim4py::mm);
	    SetParameter("septa_thickness", 0.16, sim4py::mm);
	    SetParameter("collimator_thickness", 12.025*2.0, sim4py::mm);
	    SetParameter("n_of_hole_columns", 13);
	    SetParameter("n_of_hole_raws", 15);
	    cout << "Use " << preset_name << " as collimator" << endl;
	}
	else {
	    cerr << "***Error*** in GeometryCollimator::Construct : ";
	    cerr << "Collimator preset named " << preset_name << " is not found." << endl;
	    return;
	}
    }

    
    const bool surfaceCheck = false;
    // G4double margin = 0.1*mm; 

    // AHG4MaterialManager* mmanager = AHG4MaterialManager::GetAHG4MaterialManager();
    // AHG4Material* air = mmanager->instantiate("Vacuum");	
    auto nist_manager = G4NistManager::Instance();
    // G4Material* air = mmanager->instantiate("Vacuum");	
    
    std::string matname;
    // AHG4MaterialProperty* p;
    // G4MaterialProperty* p;
    // matname = "AHG4SGD.Tungsten";
    // p = mmanager->RetrieveProperty("Tungsten");
    // mmanager->registerMaterial(matname, *p);
    // AHG4Material* tungsten = mmanager->instantiate(matname);
    // G4Material* tungsten = mmanager->instantiate(matname);
    G4Material* tungsten = nist_manager->FindOrBuildMaterial("G4_W");
    
    //    std::string matname;
    //    AHG4MaterialProperty* p;
    //    matname = "AHG4SGD.Lead";
    //    p = mmanager->RetrieveProperty("Lead");
    //    mmanager->registerMaterial(matname, *p);
    //   AHG4Material* lead = mmanager->instantiate(matname);
    
    
    // Collimator //////////////////////
        
    // double a0 = 0.50*mm; 
    // double d0 = 0.11*mm;
    const auto [ hol, holunit ] = GetParameter<double,unit>("hole_radius");
    const auto [ sep, sepunit ] = GetParameter<double,unit>("septa_thickness");
    const double a0 = hol * holunit;
    const double d0 = sep * sepunit;
    
    const int imax=3;
    const int jmax=3;
    
    double offset_x = -(2.0*a0+d0)*((double)imax-1.0)/2.0;
    double offset_y = -(a0+d0/2.0)/0.57735027*((double)jmax-1.0)/2.0;
    
    //G4double Collimator_px = 16.95*mm;
    G4double Collimator_px = -offset_x;
    //G4double Collimator_py = 16.95*mm;
    G4double Collimator_py = -offset_y;
    // //  G4double Collimator_pz = 10.44*mm;
    //G4double Collimator_pz = 10.44*mm;
    const auto [ thic, thicunit ] = GetParameter<double,unit>("collimator_thickness");
    double Collimator_pz = thic * thicunit * 0.5;     
    
    G4VSolid* Collimator_Solid
	= new G4Box("Collimator_Solid", Collimator_px, Collimator_py, Collimator_pz);

    const G4double zPlane[2] = {-(Collimator_pz+0.1*mm),Collimator_pz+0.1*mm};
    const G4double zInner[2] = {0.0*mm,0.0*mm};  
    const G4double zOuter[2] = {a0,a0};

    G4VSolid* hole_Solid
	= new G4Polyhedra("hole_Solid", 0.0*deg,360.0*deg, 6, 2, zPlane,zInner,zOuter);
    
    //  G4LogicalVolume* Collimator_Logical = new G4LogicalVolume(Collimator_Solid, tungsten, "Collimator_Logical");    
    
    G4RotationMatrix rot_p1 = G4RotationMatrix();
    rot_p1.rotateZ(30.0*deg);
    
    G4SubtractionSolid *a[imax][jmax];

    for ( int j=0; j<jmax; j++ ) {
	
	for ( int i=0; i<imax; i++ ) {
	    
	    double sx = offset_x;
	    double sy = offset_y;
	    
	    if ( j%2 ) {
		sx = sx + (a0+d0/2.0);
	    }
	    
	    G4ThreeVector p
		(sx+(2.0*a0+d0)*(double)i,sy+(a0+d0/2.0)/0.57735027*(double)j,0);
	    
	    G4String name = "a" + std::to_string(i)+ std::to_string(j);
	    std::cout << name << std::endl;
	    
	    if ( i==0 && j==0 ) {
		
		a[i][j] = new G4SubtractionSolid
		    ("a00",Collimator_Solid,hole_Solid,G4Transform3D(rot_p1, p));
		
	    }else{
		
		if ( i==0 ) {
		    a[i][j] = new G4SubtractionSolid
			(name, a[imax-1][j-1], hole_Solid, G4Transform3D(rot_p1, p));
		    
		}else{
		    
		    a[i][j] = new G4SubtractionSolid
			(name, a[i-1][j], hole_Solid, G4Transform3D(rot_p1, p));
		}
		
	    }
	    
	}
    }
    
    //  G4SubtractionSolid *aaa = new G4SubtractionSolid("a2918",Collimator_Solid,hole_Solid,G4Transform3D(rot_p1, p1));
    // G4LogicalVolume* Collimator_Logical = new G4LogicalVolume(a[imax-1][jmax-1], tungsten, "Collimator_Logical");
    G4LogicalVolume* Collimator_Logical
	= new G4LogicalVolume( a[imax-1][jmax-1], tungsten, "Collimator_Logical" );
    
    G4VisAttributes* Collimator_Attributes
	= new G4VisAttributes( G4Colour::Red() );
    
    Collimator_Attributes->SetForceSolid( true );  
    Collimator_Logical->SetVisAttributes( Collimator_Attributes ); 

    double pitch_x = -offset_x*2.0;
    double pitch_y = -offset_y*2.0;

    // double s_x = 0.0*mm-7.0*pitch_x;
    // double s_y = 0.0*mm-8.0*pitch_y;

    const auto [ n_columns ] = GetParameter<int>("n_of_hole_columns");
    const auto [ n_raws ]    = GetParameter<int>("n_of_hole_raws");
    
    auto [ centx, centy, centz, centunit ]
	= GetParameter<double,double,double,unit>("position");    
    double s_x = centx*centunit-(n_columns-1)/2*pitch_x;
    double s_y = centy*centunit-(n_raws-1)/2*pitch_y;
    double s_z = centz*centunit;
    
    G4ThreeVector pos;

    // for ( int i=0; i<15; i++ ) { //8
    for ( int i=0; i<n_columns; i++ ) { //8

	// for ( int j=0; j<17; j++ ) { //9 17
	for ( int j=0; j<n_raws; j++ ) { //9 17
	    
	    std::string name = "Collimator"+std::to_string(i)+std::to_string(j);
	    pos.setX(s_x+i*pitch_x);
	    pos.setY(s_y+j*pitch_y);
	    // pos.setZ(Collimator_pz+1.0*mm);
	    pos.setZ(s_z+Collimator_pz+1.0*mm);
	    
	    new G4PVPlacement(0, pos, Collimator_Logical, name,
			      LocalWorld_Logical, false, 0, surfaceCheck);
	    
	}
    }  
}
