/**
   @date 2020/06/30
**/

#include "P4RunManager.hpp"

#include <iostream>
using std::cout;
using std::cerr;
using std::endl;

#include <P4DetectorConstruction.hpp>
#include <P4PrimaryGeneratorAction.hpp>
#include <P4GeneralParticleSource.hpp>
#include <P4ActionInitialization.hpp>

#include <P4EventAction.hpp>
#include <P4RunAction.hpp>

#include <W4DetectorConstruction.hpp>
#include <W4RunAction.hpp>

#include <G4PhysListFactory.hh>

P4RunManager::P4RunManager()
    : sim4py::ParameterGene<G4RunManager>()
{
    SetParameter<std::string>("class_name", "P4RunManager");
    
    is_set_detector_construction = false;
    is_set_physics_list = false;
    is_set_action_initialization = false;

    verbose_level = 1;
    DefineParameter<int>("verbose_level", verbose_level);

    DefineParameter<int>("physlist_verbose_level", 0);
}

P4RunManager::~P4RunManager()
{
    if ( verbose_level>0 )
	cout << "P4RunManager::~P4RunManager" << endl;
    // G4RunManager::~G4RunManager();
}

void P4RunManager::BeamOn(G4int n_event, const char *macroFile, G4int n_select)
{
    Initialize();
    if ( verbose_level>0 )
	cout << "P4RunManager::BeamOn" << endl;
    G4RunManager::BeamOn( n_event, macroFile, n_select);
}

void P4RunManager::Initialize()
{
    verbose_level = std::get<0>( GetParameter<int>("verbose_level") );
    SetVerboseLevel( verbose_level );
    
    if ( verbose_level>0 )
	cout << "P4RunManager::Initialize" << endl;

    if ( is_set_physics_list==false ) {
	SetReferencePhysicsList();
	// G4PhysListFactory factory;
	// auto phys = factory.GetReferencePhysList( "FTFP_BERT" );
	// this->SetUserInitialization( phys );
    }
    
    // auto det = new P4DetectorConstruction();
    //auto det = W4DetectorConstruction::Instance();
    //this->SetUserInitialization( det );

    if ( is_set_action_initialization==false ) {
	auto pga = new P4PrimaryGeneratorAction();
	pga->SetPrimaryGenerator(new P4GeneralParticleSource);
	auto act = new P4ActionInitialization();
	act->RegisterUserAction(pga);
	//act->RegisterUserAction( new W4RunAction );
	this->SetUserInitialization(act);    
	
	this->SetUserAction( new P4RunAction );
	this->SetUserAction( new P4EventAction );
    }
    
    this->G4RunManager::Initialize();
}

void P4RunManager::SetUserInitialization
(G4VUserDetectorConstruction *userInit)
{
    if ( verbose_level>1 )
	cout << "P4RunManager::SetUserInitialization" << endl;	
    
    if ( userInit!=nullptr ) {
	G4RunManager::SetUserInitialization( userInit );
	is_set_detector_construction = true;
	return;
    }
    
    if ( verbose_level>0 ) {
	cerr << "***Error*** : in P4RunManager ";
	cerr << "Attempt to set nullptr as G4VUserDetectorConstruction" << endl;
    }	        
}

void P4RunManager::SetUserInitialization
(G4VUserPhysicsList *userInit)
{
    if ( verbose_level>1 )
	cout << "P4RunManager::SetUserInitialization" << endl;	

    if ( userInit!=nullptr ) {
	auto [ ver ] = GetParameter<int>("physlist_verbose_level");
	userInit->SetVerboseLevel( ver );
	if ( verbose_level>0 ) cout << "physlist_verbose_level = " << ver << endl;
	
	G4RunManager::SetUserInitialization( userInit );
	is_set_physics_list = true;
	
	return;
    }
    
    if ( verbose_level>0 ) {
	cerr << "***Error*** : in P4RunManager ";
	cerr << "Attempt to set nullptr as G4VUserPhysicsList" << endl;
    }	        
}

void P4RunManager::SetUserInitialization
(G4VUserActionInitialization *userInit)
{
    if ( verbose_level>1 )
	cout << "P4RunManager::SetUserInitialization" << endl;	
    
    if ( userInit!=nullptr ) {
	G4RunManager::SetUserInitialization( userInit );
	is_set_action_initialization = true;
	return;
    }


    
    if ( verbose_level>0 ) {
	cerr << "***Error*** : in P4RunManager ";
	cerr << "Attempt to set nullptr as G4VUserActionInitialization" << endl;
    }	        
}

void P4RunManager::SetReferencePhysicsList(const std::string& name)
{
    verbose_level = std::get<0>( GetParameter<int>("verbose_level") );
    SetVerboseLevel( verbose_level );

    G4PhysListFactory factory;
    auto phys = factory.GetReferencePhysList( name.c_str() );
    //phys->SetVerboseLevel( verbose_level );
    this->SetUserInitialization( phys );
}
