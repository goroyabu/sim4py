/**
   @date 2020/08/06
**/

#include "UserVolume.hpp"

#include <G4Box.hh>
#include <G4LogicalVolume.hh>
#include <G4VPhysicalVolume.hh>
#include <G4PVPlacement.hh>
#include <G4NistManager.hh>

UserVolume::UserVolume()
    : P4PVConstruct("UserVolume")
{
    verbose_level = 1;
    DefineParameter<int>("verbose_level", verbose_level);
}

void UserVolume::Construct(G4LogicalVolume* logical_world)
{
    auto nist_manager = G4NistManager::Instance();
    
    auto matename = "Al";    
    auto halfx = 1.0 * sim4py::mm;
    auto halfy = 10.0 * sim4py::mm;
    auto halfz = 10.0 * sim4py::mm;
    auto pos = G4ThreeVector(20*sim4py::mm,0,0);

    auto mate = nist_manager->FindOrBuildMaterial( matename );
    auto box = new G4Box( "box_usrvol", halfx, halfy, halfz );
    auto log = new G4LogicalVolume( box, mate, "logical_usrvol" );
    new G4PVPlacement( 0, pos, log, "physical_usrvol", logical_world, false, 0 );    
}
