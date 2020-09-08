/**
   @date 2020/07/02
**/

#include "P4ParticleGun.hpp"
using sim4py::unit;

#include <iostream>
using std::cout;
using std::endl;

#include <G4IonTable.hh>
#include <G4ParticleTable.hh>
#include <G4Geantino.hh>

P4ParticleGun::P4ParticleGun()
    : sim4py::ParameterGene("P4ParticleGun"), G4ParticleGun()
{
    is_applied_parameters = false;
    // SetParameter<std::string>("class_name", "P4ParticleGun");

    verbose_level = 1;
    DefineParameter<int>("verbose_level", verbose_level);

    DefineParameter<std::string>("particle", "gamma");
    DefineParameter<double,double,double>("direction", 1, 0, 0);
    DefineParameter<double, unit>("energy", 511.0, sim4py::keV);
    DefineParameter<double,double,double,unit>("position", 0, 0, 0, sim4py::cm);
    DefineParameter<double,double,double,unit>("momentum", 0, 0, 0, sim4py::MeV);
    DefineParameter<double,unit>("momentum_amp", -1, sim4py::MeV);
    // DefineParameter<double,unit>("time", 0, sim4py::nanosecond);
    DefineParameter<double,double,double>("polarization", 0, 0, 0);
    DefineParameter<int>("number", 1);
    DefineParameter<bool>("use_ion", false);
    DefineParameter<int>("ion_atomic_number", -1);
    DefineParameter<int>("ion_atomic_mass", -1);
    DefineParameter<int>("ion_charge", 0);
    DefineParameter<double>("ion_excitation", 0, sim4py::keV);
    DefineParameter<bool>("use_ionL", false);
    DefineParameter<int>("ionL_atomic_number", -1);
    DefineParameter<int>("ionL_atomic_mass", -1);
    DefineParameter<int>("ionL_charge", 0);
    DefineParameter<int>("ionL_level_number", 0);

    SetParticleDefinition( G4Geantino::Geantino() );
    SetParticleMomentumDirection( G4ThreeVector(1.0,0.0,0.0) );
    SetParticleEnergy( 1.0*sim4py::GeV );
    SetParticlePosition
	(G4ThreeVector(0.0*sim4py::cm, 0.0*sim4py::cm, 0.0*sim4py::cm));
    SetParticleTime( 0.0*CLHEP::ns );
}

P4ParticleGun::~P4ParticleGun()
{
}

void P4ParticleGun::GeneratePrimaryVertex(G4Event* event)
{
    if ( is_applied_parameters == false ) apply_parameters();
    G4ParticleGun::GeneratePrimaryVertex( event );
}

void P4ParticleGun::apply_parameters()
{
    verbose_level = std::get<0>( GetParameter<int>("verbose_level") );
    is_applied_parameters = true;
    
    auto [ particle ] = GetParameter<std::string>("particle");
    auto [ use_ion  ] = GetParameter<bool>("use_ion");
    auto [ use_ionL ] = GetParameter<bool>("use_ionL");
    
    if ( particle=="ion" && use_ion )
	ion_command();
    else if ( particle=="ion" && use_ionL )
	ion_level_command();    
    else {
	auto table = G4ParticleTable::GetParticleTable();
	auto particle_def = table->FindParticle( particle.c_str() );
	if ( particle_def!=nullptr )
	    SetParticleDefinition( particle_def );
    }
    
    auto [ dx, dy, dz ] = GetParameter<double,double,double>("direction");
    SetParticleMomentumDirection( G4ThreeVector(dx, dy, dz) );

    auto [ e, eu ] = GetParameter<double,unit>("energy"); 
    SetParticleEnergy( e*eu );
    
    if ( IsModifiedParameter<double,double,double,unit>("momentum") ) {
	auto [ mx, my, mz, mu ] = GetParameter<double,double,double,unit>("momentum");
	SetParticleMomentum( G4ThreeVector(mx*mu, my*mu, mz*mu) );
    }
    
    if ( IsModifiedParameter<double,unit>("momentum_amp") ) {
	auto [ ma, mau ] = GetParameter<double,unit>("momentum_amp");
	SetParticleMomentum( ma*mau );
    }
    
    auto [ px, py, pz, pu ] = GetParameter<double,double,double,unit>("position");
    SetParticlePosition( G4ThreeVector(px*pu, py*pu, pz*pu) );

    // auto [ t, tu ] = GetParameter<double,unit>("time");
    // SetParticleTime( t*tu );

    if ( IsModifiedParameter<double,double,double>("polarization") ) {
	auto [ pox, poy, poz ] = GetParameter<double,double,double>("polarization");
	SetParticlePolarization( G4ThreeVector(pox, poy, poz) );
    }

    if ( IsModifiedParameter<int>("number") ) {
	auto [ num ] = GetParameter<int>("number");
	SetNumberOfParticles( num );
    }
}

void P4ParticleGun::ion_command()
{
    auto [ number ] = GetParameter<int>("ion_atomic_number");
    auto [ mass ] = GetParameter<int>("ion_atomic_mass");
    auto [ charge ] = GetParameter<int>("ion_charge");
    auto [ energy, u ] = GetParameter<double,unit>("ion_excitation");
    
    G4ParticleDefinition* ion = 0;   
    ion =  G4IonTable::GetIonTable()->GetIon( number, mass, energy*u );
    if ( ion==nullptr ) {
	cout << "Ion with Z=" << number;
	cout << " A=" << mass << "is not defined" << endl;    
    }
    SetParticleDefinition( ion );
    SetParticleCharge( charge*CLHEP::eplus );
}

void P4ParticleGun::ion_level_command()
{
    auto [ number ] = GetParameter<int>("ionL_atomic_number");
    auto [ mass ] = GetParameter<int>("ionL_atomic_mass");
    auto [ charge ] = GetParameter<int>("ionL_charge");
    auto [ level ] = GetParameter<int>("ionL_level_number");
    
    G4ParticleDefinition* ion = 0;   
    ion =  G4IonTable::GetIonTable()->GetIon( number, mass, level );
    if ( ion==nullptr ) {
	cout << "Ion with Z=" << number << " A=" << mass;
	cout << ", I = " << level << "is not defined" << endl;    
    }
    SetParticleDefinition( ion );
    SetParticleCharge( charge*CLHEP::eplus );
}
