/**
   @date 2020/08/06
**/

#include "UserVolume.hpp"

#include <G4SystemOfUnits.hh>
#include <G4Box.hh>
#include <G4LogicalVolume.hh>
#include <G4VPhysicalVolume.hh>
#include <G4PVPlacement.hh>
#include <G4NistManager.hh>

UserVolume::UserVolume()
    : P4PVConstruct("UserVolume")
{
    DefineParameter<int>( "verbose_level", 0 );
    DefineParameter<double,sim4py::unit>( "thickness", 1.0, sim4py::mm );
    DefineParameter<std::string>( "material", "Al" );
}

void UserVolume::Construct(G4LogicalVolume* logical_world)
{
    auto [ verbose_level ] = GetParameter<int>( "verbose_level" );
    if ( verbose_level>0 )
        std::cout << "A method UserVolume::Construct is called." << std::endl;

    auto nist_manager = G4NistManager::Instance();
    if ( !nist_manager->FindOrBuildMaterial( "Si" ) )
        new G4Material( "Si", 14., 28.09*g/mole, 2.330*g/cm3 );
    if ( !nist_manager->FindOrBuildMaterial( "Al" ) )
        new G4Material( "Al", 13., 26.98*g/mole, 2.7*g/cm3 );

    auto [ matename ] = GetParameter<std::string>( "material" );
    auto [ thick, unit_of_thick ] = GetParameter<double,sim4py::unit>( "thickness" );

    auto halfx = thick * unit_of_thick;
    auto halfy = 10.0 * sim4py::mm;
    auto halfz = 10.0 * sim4py::mm;
    auto pos = G4ThreeVector( 20.0*sim4py::mm, 0, 0 );

    auto mate = nist_manager->FindOrBuildMaterial( matename );
    auto box = new G4Box( "box_usrvol", halfx, halfy, halfz );
    auto log = new G4LogicalVolume( box, mate, "logical_usrvol" );
    new G4PVPlacement( 0, pos, log, "physical_usrvol", logical_world, false, 0 );

    if ( verbose_level>0 )
        std::cout << "End of UserVolume::Construct." << std::endl;
}
