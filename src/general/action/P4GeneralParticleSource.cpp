/**
   @date 2020/06/25
**/

#include "P4GeneralParticleSource.hpp"

#include <string>
#include <iostream>
using std::cout;
using std::cerr;
using std::endl;

#include <G4SingleParticleSource.hh>
#include <G4ParticleTable.hh>
#include <G4IonTable.hh>

using sim4py::unit;

P4GeneralParticleSource::P4GeneralParticleSource()
    : sim4py::ParameterGene("P4GeneralParticleSource"), G4GeneralParticleSource()
{
    is_applied_parameters = false;    
    // SetParameter<std::string>("class_name", "P4GeneralParticleSource");

    verbose_level = 1;
    DefineParameter<int>("verbose_level", verbose_level);
    
    DefineParameter<std::string>("particle", "gamma");
    DefineParameter<double,double,double>("direction", 1.0, 0.0, 0.0);
    DefineParameter<double,unit>("energy", 511.0, sim4py::keV);
    DefineParameter<double,double,double,unit>("position", 0.0, 0.0, 0.0, sim4py::mm);
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

    // DefineParameter<double,unit>("time", 0, sim4py::nanosecond);
    // DefineParameter<double,double,double>("polarization", 0, 0, 0);
    
    DefineParameter<std::string>("pos_type", "Volume");
    // Candidates of pos_type : Point Beam Plane Surface Volume

    DefineParameter<std::string>("pos_shape", "Sphere");
    // Candidates of pos_shape : Circle Annulus Ellipse Square Rectangle Sphere Ellipsoid Cylinder Para
    
    DefineParameter<double,double,double,unit>("pos_centre", 0.0, 0.0, 0.0, sim4py::mm);
    DefineParameter<double,double,double>("pos_rotation1", 0.0, 0.0, 0.0);
    DefineParameter<double,double,double>("pos_rotation2", 0.0, 0.0, 0.0);    
    DefineParameter<double, unit>("pos_halfx", 0.5, sim4py::mm);
    DefineParameter<double, unit>("pos_halfy", 0.5, sim4py::mm);
    DefineParameter<double, unit>("pos_halfz", 0.5, sim4py::mm);
    DefineParameter<double, unit>("pos_radius", 0.5, sim4py::mm);
    DefineParameter<double, unit>("pos_inner_radius", 0.5, sim4py::mm);
    DefineParameter<double, unit>("pos_sigma_r", 0.5, sim4py::mm);
    DefineParameter<double, unit>("pos_sigma_x", 0.5, sim4py::mm);
    DefineParameter<double, unit>("pos_sigma_y", 0.5, sim4py::mm);
    // DefineParameter<double, unit>("pos_paralp", 0.0, sim4py::rad);
    // DefineParameter<double, unit>("pos_parthe", 0.0, sim4py::rad);
    // DefineParameter<double, unit>("pos_parphi", 0.0, sim4py::rad);
    DefineParameter<std::string>("pos_confine", "NULL");
    
    DefineParameter<std::string>("ang_type", "iso");
    DefineParameter<double,double,double>("ang_rotation1", 0.0, 0.0, 0.0);
    DefineParameter<double,double,double>("ang_rotation2", 0.0, 0.0, 0.0);
    DefineParameter<double, unit>("ang_mintheta", 0.0, sim4py::deg);
    DefineParameter<double, unit>("ang_maxtheta", 90.0, sim4py::deg);
    DefineParameter<double, unit>("ang_minphi", 0.0, sim4py::deg);
    DefineParameter<double, unit>("ang_maxphi", 360.0, sim4py::deg);
    DefineParameter<double, unit>("ang_sigma_r", 0.0, sim4py::rad);
    DefineParameter<double, unit>("ang_sigma_x", 0.0, sim4py::rad);
    DefineParameter<double, unit>("ang_sigma_y", 0.0, sim4py::rad);
    DefineParameter<double,double,double,unit>
	("ang_focuspoint", 0.0, 0.0, 0.0, sim4py::cm);
    DefineParameter<bool>("ang_user_coordinate", false);
    DefineParameter<bool>("ang_use_surface_normals", false);
    
    DefineParameter<std::string>("ene_type", "Mono");
    DefineParameter<double,unit>("ene_min", 0.0, sim4py::MeV);
    DefineParameter<double,unit>("ene_max", 1.0, sim4py::MeV);
    DefineParameter<double,unit>("ene_mono", 511.0, sim4py::keV);
    DefineParameter<double,unit>("ene_sigma", 0.0, sim4py::keV);
    DefineParameter<double>("ene_alpha", 0.0);
    // DefineParameter<double>("ene_temp", 0.0);
    // DefineParameter<double,unit>("ene_ezero", 0.0, sim4py::MeV);
    // DefineParameter<double>("ene_gradient_per_mev", 0.0);
    // DefineParameter<double,unit>("ene_intercept", 0.0, sim4py::MeV);
    // DefineParameter<double>("ene_bias_alpha", 0.0);
}

P4GeneralParticleSource::~P4GeneralParticleSource()
{
}

void P4GeneralParticleSource::GeneratePrimaryVertex(G4Event* event)
{
    if ( is_applied_parameters == false ) apply_parameters();
    G4GeneralParticleSource::GeneratePrimaryVertex( event );
}

void P4GeneralParticleSource::apply_parameters()
{
    // this->fix_parameters();
    verbose_level = std::get<0>( GetParameter<int>("verbose_level") );
    SetVerbosity( verbose_level );
    is_applied_parameters = true;
    
    auto single_source = G4GeneralParticleSource::GetCurrentSource();

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
	if ( particle_def!=nullptr ) {
	    SetParticleDefinition( particle_def );
	    if ( verbose_level>1 ) cout << particle << " is set"  << endl;  
	}
	// else {
	//     auto geantino = table->FindParticle( "geantino" );
	//     if ( geantino!=nullptr ) SetParticleDefinition( geantino );
	// }
    }

    if ( IsModifiedParameter<double,double,double>("direction") ) {
	auto [ dx, dy, dz ] = GetParameter<double,double,double>("direction");
	single_source->GetAngDist()->SetAngDistType("planar");
	single_source->GetAngDist()->SetParticleMomentumDirection( G4ThreeVector(dx, dy, dz) );
    }

    // auto [ e, eu ] = GetParameter<double,unit>("energy");
    // single_source->GetEneDist()->SetEnergyDisType("Mono");
    // single_source->>GetEneDist()->SetMonoEnergy( e*eu );

    // auto [ px, py, pz, pu ] = GetParameter<double,double,double,unit>("position");
    // single_source->GetPosDist()->SetPosDisType("Point")
    // single_source->SetParticlePosition( G4ThreeVector(px*pu, py*pu, pz*pu) );
    
    auto [ pos_type ] = GetParameter<std::string>("pos_type");
    std::vector<std::string> pos_type_cand
	= { "Point", "Beam", "Plane", "Surface", "Volume" };
    if ( match_candidates( "pos_type", pos_type, pos_type_cand ) ) 
	single_source->GetPosDist()->SetPosDisType( pos_type.c_str() );	

    auto [ pos_shape ] = GetParameter<std::string>("pos_shape");
    std::vector<std::string> pos_shape_cand
	= { "Circle", "Annulus", "Ellipse", "Square",
	    "Rectangle", "Sphere", "Ellipsoid", "Cylinder", "Para" };
    if ( match_candidates( "pos_shape", pos_shape, pos_shape_cand ) )    
	single_source->GetPosDist()->SetPosDisShape( pos_shape.c_str() );
    
    auto [ x, y, z, u ]
	= GetParameter<double, double, double, unit>("pos_centre");
    single_source->GetPosDist()->SetCentreCoords( G4ThreeVector(x*u, y*u, z*u));

    // auto [ r1x, r1y, r1z ] = GetParameter<double,double,double>("pos_rotation1");
    // single_source->GetPosDist()->SetPosRot1( G4ThreeVector(r1x,r1y,r1z) );
    // auto [ r2x, r2y, r2z ] = GetParameter<double,double,double>("pos_rotation2");
    // single_source->GetPosDist()->SetPosRot2( G4ThreeVector(r2x,r2y,r2z) );
    
    auto [ halfx, ux ] = GetParameter<double, unit>("pos_halfx");
    single_source->GetPosDist()->SetHalfX( halfx*ux );
    auto [ halfy, uy ] = GetParameter<double, unit>("pos_halfy");
    single_source->GetPosDist()->SetHalfY( halfy*uy );
    auto [ halfz, uz ] = GetParameter<double, unit>("pos_halfz");
    single_source->GetPosDist()->SetHalfZ( halfz*uz );

    auto [ radius, ur ] = GetParameter<double,unit>("pos_radius");
    single_source->GetPosDist()->SetRadius( radius*ur );
    // auto [ radius0, ur0 ] = GetParameter<double,unit>("pos_inner_radius");
    // single_source->GetPosDist()->SetRadius0( radius0*ur0 );

    // auto [ sigr, usigr ] = GetParameter<double,unit>("pos_sigma_r");
    // single_source->GetPosDist()->SetBeamSigmaInR( sigr*usigr );

    // auto [ sigx, usigx ] = GetParameter<double,unit>("pos_sigma_x");
    // single_source->GetPosDist()->SetBeamSigmaInX( sigx*usigx );

    // auto [ sigy, usigy ] = GetParameter<double,unit>("pos_sigma_y");
    // single_source->GetPosDist()->SetBeamSigmaInX( sigy*usigy );
    
    auto [ angle_type ]= GetParameter<std::string>("ang_type");
    single_source->GetAngDist()->SetAngDistType( angle_type.c_str() );
    auto [ angle_mintheta, umint ] = GetParameter<double, unit>("ang_mintheta");
    single_source->GetAngDist()->SetMinTheta( angle_mintheta*umint );
    auto [ angle_maxtheta, umaxt ] = GetParameter<double, unit>("ang_maxtheta");
    single_source->GetAngDist()->SetMaxTheta( angle_maxtheta*umaxt );

    auto [ energy_type ] = GetParameter<std::string>("ene_type");
    single_source->GetEneDist()->SetEnergyDisType( energy_type.c_str() );
    auto [ mono_energy, umono ] = GetParameter<double,unit>("ene_mono");
    single_source->GetEneDist()->SetMonoEnergy( mono_energy*umono );   
}
void P4GeneralParticleSource::ion_command()
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

void P4GeneralParticleSource::ion_level_command()
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

bool P4GeneralParticleSource::match_candidates
(const std::string& key, const std::string& input, std::vector<std::string>& cand_list)
{
    for ( auto cand : cand_list ) {
	if ( input==cand ) return true;
    }

    cerr << "***Error*** in P4GeneralParticleSource : ";
    cerr << "Input value " << input << " is not matched candidates." << endl;
    cerr << "Candidates of " << key << " = [ ";
    for ( auto cand : cand_list ) {
	cerr << cand << " ";
    }
    cerr << "]" << endl;    
    
    return false;
}
