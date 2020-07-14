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
#include <G4GenericPhysicsList.hh>

#include <G4UIExecutive.hh>
#include <G4UImanager.hh>
#include <G4VisExecutive.hh>

P4RunManager::P4RunManager()
    : sim4py::ParameterGene("P4RunManager"), G4RunManager()
{
    // SetParameter<std::string>("class_name", "P4RunManager");
    
    is_set_detector_construction = false;
    is_set_physics_list = false;
    is_set_action_initialization = false;
    is_already_setup_vis = false;

    verbose_level = 1;
    DefineParameter<int>("verbose_level", verbose_level);

    DefineParameter<int>("physlist_verbose_level", 0);

    DefineParameter<bool>("visualize", false);

    DefineParameter<double,sim4py::unit>("cut_value", 1.0, sim4py::mm);
    
    ui_executive = nullptr;
    vis_executive = nullptr;
}

P4RunManager::~P4RunManager()
{
    if ( verbose_level>0 )
	cout << "P4RunManager::~P4RunManager" << endl;

    if ( ui_executive!=nullptr ) {
	delete ui_executive;
	ui_executive = 0;
    }

    if ( vis_executive!=nullptr ) {
	delete vis_executive;
	vis_executive = 0;
    }
    
    // G4RunManager::~G4RunManager();
}

void P4RunManager::BeamOn(G4int n_event, const char *macroFile, G4int n_select)
{
    Initialize();
   
    is_enabled_visualization = std::get<0>( GetParameter<bool>("visualize") );
    if ( is_enabled_visualization )  SetupVisualization();
    
    if ( verbose_level>0 )
	cout << "P4RunManager::BeamOn" << endl;
    G4RunManager::BeamOn( n_event, macroFile, n_select);

    if ( is_enabled_visualization ) ui_executive->SessionStart();
}

void P4RunManager::Initialize()
{
    verbose_level = std::get<0>( GetParameter<int>("verbose_level") );
    SetVerboseLevel( verbose_level );    
    
    if ( verbose_level>0 )
	cout << "P4RunManager::Initialize" << endl;

    if ( is_set_physics_list==false ) {
	UseReferencePhysicsList();
	// G4PhysListFactory factory;
	// auto phys = factory.GetReferencePhysList( "FTFP_BERT" );
	// this->SetUserInitialization( phys );
    }

    if ( is_set_detector_construction==false ) {
    
	auto det = new P4DetectorConstruction();
	// auto det = W4DetectorConstruction::Instance();
	this->SetUserInitialization( det );
	
    }

    if ( is_set_action_initialization==false ) {

	auto gps = new P4GeneralParticleSource();
	gps->SetParameter<int>("verbose_level", verbose_level);

	auto pga = new P4PrimaryGeneratorAction();
	pga->SetPrimaryGenerator( gps );
	pga->SetParameter<int>("verbose_level", verbose_level);
	
	auto act = new P4ActionInitialization();
	act->RegisterUserAction(pga);
	act->SetParameter<int>("verbose_level", verbose_level);
	this->SetUserInitialization(act);    
	
	this->SetUserAction( new P4RunAction );
	this->SetUserAction( new P4EventAction );
    }

    if ( physics_list ) cout << "Phys ok" << endl;
    if ( action_initialization ) cout << "Act ok" << endl;
    if ( detector_construction ) cout << "Det ok" << endl;	
    
    this->G4RunManager::Initialize();

    if ( verbose_level>0 )
	cout << "P4RunManager::Initialize done" << endl;
}

void P4RunManager::SetUserInitialization
(G4VUserDetectorConstruction *userInit)
{
    if ( verbose_level>1 )
	cout << "P4RunManager::SetUserInitialization" << endl;	
    
    if ( userInit!=nullptr ) {
	detector_construction = userInit;
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
    if ( verbose_level>0 )
	cout << "P4RunManager::SetUserInitialization" << endl;	

    if ( userInit!=nullptr ) {
	auto [ ver ] = GetParameter<int>("physlist_verbose_level");
	userInit->SetVerboseLevel( ver );
	if ( verbose_level>0 ) cout << "physlist_verbose_level = " << ver << endl;

	auto [ cut, cu ] = GetParameter<double,sim4py::unit>("cut_value");
	userInit->SetDefaultCutValue(cut*cu);
	userInit->SetCuts();
	userInit->SetCutValue(cut*cu,"e-");
	if ( verbose_level>0 ) cout << "cut_value = " << cut << " " << cu << endl;

	physics_list = userInit;
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
	action_initialization = userInit;
	G4RunManager::SetUserInitialization( userInit );
	is_set_action_initialization = true;
	return;
    }
    
    if ( verbose_level>0 ) {
	cerr << "***Error*** : in P4RunManager ";
	cerr << "Attempt to set nullptr as G4VUserActionInitialization" << endl;
    }	        
}

void P4RunManager::UseReferencePhysicsList(const std::string& name)
{
    verbose_level = std::get<0>( GetParameter<int>("verbose_level") );
    SetVerboseLevel( verbose_level );

    G4PhysListFactory factory;
    auto phys = factory.GetReferencePhysList( name.c_str() );
    //phys->SetVerboseLevel( verbose_level );
    this->SetUserInitialization( phys );
}

void P4RunManager::UseGenericPhysicsList(const std::vector<std::string>& constructors)
{
    verbose_level = std::get<0>( GetParameter<int>("verbose_level") );
    SetVerboseLevel( verbose_level );
    
    auto list_of_constructors  = new std::vector<G4String>;
    for ( auto itr : constructors ) {
	list_of_constructors->emplace_back( itr.c_str() );
    }    
    auto phys = new G4GenericPhysicsList( list_of_constructors );
    this->SetUserInitialization( phys );    
}

void P4RunManager::SetupVisualization()
{
    if ( is_already_setup_vis ) return;
    is_already_setup_vis = true;

    // auto arg = std::string("python");
    std::vector<char*> argv = { const_cast<char*>("python") };
    ui_executive = new G4UIExecutive( argv.size(), &argv[0] );
    vis_executive = new G4VisExecutive;
    vis_executive->Initialize();
    
    auto ui_manager = G4UImanager::GetUIpointer();
    // ui_manager->ApplyCommand("/control/verbose 2");
    // ui_manager->ApplyCommand("/run/verbose 2");
    // ui_manager->ApplyCommand("/control/execute config.mac");
    ui_manager->ApplyCommand("/vis/open OGL 600x600-0+0");
    ui_manager->ApplyCommand("/vis/viewer/set/autoRefresh false");
    ui_manager->ApplyCommand("/vis/verbose errors");
    ui_manager->ApplyCommand("/vis/drawVolume");
    ui_manager->ApplyCommand("/vis/viewer/set/viewpointThetaPhi 90. 0.");
    ui_manager->ApplyCommand("/vis/viewer/zoom 1.4");
    ui_manager->ApplyCommand("/vis/scene/add/axes 0 0 0 10 cm");
    ui_manager->ApplyCommand("/vis/scene/add/trajectories smooth");
    ui_manager->ApplyCommand("/vis/modeling/trajectories/create/drawByCharge");
    ui_manager->ApplyCommand("/vis/modeling/trajectories/drawByCharge-0/default/setDrawStepPts true");
    ui_manager->ApplyCommand("/vis/modeling/trajectories/drawByCharge-0/default/setStepPtsSize 2");
    ui_manager->ApplyCommand("/vis/scene/add/hits");
    ui_manager->ApplyCommand("/vis/filtering/trajectories/create/particleFilter");
    ui_manager->ApplyCommand("/vis/filtering/trajectories/particleFilter-0/add gamma");
    ui_manager->ApplyCommand("/vis/modeling/trajectories/create/drawByParticleID");
    ui_manager->ApplyCommand("/vis/modeling/trajectories/drawByParticleID-0/set e- blue");
    ui_manager->ApplyCommand("/vis/scene/endOfEventAction accumulate");
    ui_manager->ApplyCommand("/vis/viewer/set/autoRefresh true");
    ui_manager->ApplyCommand("/vis/verbose warnings");
    ui_manager->ApplyCommand("/vis/viewer/flush");

    // ui_executive->SessionStart();
}
