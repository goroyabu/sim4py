/**
   @date 2020/06/25
**/

#include "P4SphericalSource.hpp"

#include <string>
#include <iostream>
using std::cout;
using std::cerr;
using std::endl;

#include <G4SingleParticleSource.hh>
#include <G4ParticleTable.hh>
#include <G4IonTable.hh>

using sim4py::unit;

P4SphericalSource::P4SphericalSource()
    : sim4py::ParameterGene( "P4SphericalSource" ), G4VUserPrimaryGeneratorAction()
{
    is_applied_parameters = false;

    verbose_level = 1;
    this->DefineParameter<int>( "verbose_level", verbose_level );

    this->DefineParameter<std::string>( "particle", "gamma" );
    this->DefineParameter<double,unit>( "energy", 511.0, sim4py::keV );
    this->DefineParameter<double,unit>( "time", 0.0, sim4py::ns );

    this->DefineParameter<double,double,double>( "axis_direction", 0.0, 0.0, 1.0 );
    this->DefineParameter<double,double,double>( "center_position", 0.0, 0.0, 0.0 );
    this->DefineParameter<double, unit>( "radius_min", 100.0, sim4py::mm  );
    this->DefineParameter<double, unit>( "radius_max", 100.0, sim4py::mm  );
    this->DefineParameter<double, unit>( "theta_min",    0.0, sim4py::deg );
    this->DefineParameter<double, unit>( "theta_max",  180.0, sim4py::deg );
    this->DefineParameter<double, unit>( "phi_min",      0.0, sim4py::deg );
    this->DefineParameter<double, unit>( "phi_max",    360.0, sim4py::deg );

}

P4SphericalSource::~P4SphericalSource()
{
}

void P4SphericalSource::GeneratePrimaries( G4Event* anEvent )
{
    if ( is_applied_parameters == false ) this->ApplyParameters();
    auto particle_tab = G4ParticleTable::GetParticleTable();

    auto particle_def = particle_tab->FindParticle( particle_name );
    auto prim_particle = new G4PrimaryParticle( particle_def );
    prim_particle->SetMomentumDirection( G4ThreeVector( px, py, pz ) );

    auto prim_vertex = new G4PrimaryVertex( x, y, z, t );
    prim_vertex->SetPrimary( prim_particle );
    anEvent->AddPrimaryVertex( prim_vertex );
}
