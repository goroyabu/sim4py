/**
   @file PhysicsList.cpp
   @author  Goro Yabu
   @date    2019/12/22
**/

#include "PhysicsList.hpp"

#include <iostream>

#include <G4UnitsTable.hh>
#include <G4SystemOfUnits.hh>

#include <G4EmParameters.hh>
//#include <G4EmLivermorePhysics.hh>

// particles
#include <G4Gamma.hh>
#include <G4Electron.hh>
#include <G4Positron.hh>
#include <G4MuonPlus.hh>
#include <G4MuonMinus.hh>
#include <G4PionPlus.hh>
#include <G4PionMinus.hh>
#include <G4KaonPlus.hh>
#include <G4KaonMinus.hh>
#include <G4Proton.hh>
#include <G4AntiProton.hh>
#include <G4Deuteron.hh>
#include <G4Triton.hh>
#include <G4He3.hh>
#include <G4Alpha.hh>
#include <G4GenericIon.hh>

//#include "CustomizedLivermorePhysics.hpp"
#include "PhysicsConstructor.hpp"


PhysicsList::PhysicsList()
    :G4VModularPhysicsList()
{
    // this->RegisterPhysics( new G4EmLivermorePhysics );
    // this->RegisterPhysics( new CustomizedLivermorePhysics );    
    // this->RegisterPhysics( new PhysicsConstructor );

    compton_model = "G4KleinNishinaCompton";
    is_enabled_livermore_compton = false;
    constructor
	= new PhysicsConstructor(1, compton_model, is_enabled_livermore_compton);

    // constructor_knmodel_standard
    // 	= new PhysicsConstructor(1, "G4KleinNishinaModel", false);
    // constructor_kncompton_standard
    // 	= new PhysicsConstructor(1, "G4KleinNishinaCompton", false);
    // constructor_knmodel_livermore
    // 	= new PhysicsConstructor(1, "G4KleinNishinaModel", true);    
    // constructor_kncompton_livermore
    // 	= new PhysicsConstructor(1, "G4KleinNishinaCompton", true);

    // this->RegisterPhysics( constructor );
    this->SetVerboseLevel(2);
    std::cout << "PhysicsList::PhysicsList()" << std::endl;
}
PhysicsList::~PhysicsList()
{
    // delete constructor;
}

void PhysicsList::ConstructParticle()
{
    // gamma
    G4Gamma::Gamma();

    // leptons
    G4Electron::Electron();
    G4Positron::Positron();
    G4MuonPlus::MuonPlus();
    G4MuonMinus::MuonMinus();

    // mesons
    G4PionPlus::PionPlusDefinition();
    G4PionMinus::PionMinusDefinition();
    G4KaonPlus::KaonPlusDefinition();
    G4KaonMinus::KaonMinusDefinition();

    // baryons
    G4Proton::Proton();
    G4AntiProton::AntiProton();

    // ions
    G4Deuteron::Deuteron();
    G4Triton::Triton();
    G4He3::He3();
    G4Alpha::Alpha();
    G4GenericIon::GenericIonDefinition();
}
void PhysicsList::ConstructProcess()
{
    std::cout << "PhysicsList::ConstructProcess() " << compton_model << " " << is_enabled_livermore_compton << std::endl;
    AddTransportation();
    constructor->ConstructProcess();
    
    // if ( compton_model=="G4KleinNishinaModel" && is_enabled_livermore_compton )	    
    // 	constructor_knmodel_livermore->ConstructProcess();
    // else if ( compton_model=="G4KleinNishinaModel" && !is_enabled_livermore_compton)
    // 	constructor_knmodel_standard->ConstructProcess();
    // else if ( compton_model=="G4KleinNishinaCompton" && is_enabled_livermore_compton )
    // 	constructor_kncompton_livermore->ConstructProcess();
    // else if ( compton_model=="G4KleinNishinaCompton" && !is_enabled_livermore_compton )
    // 	constructor_kncompton_standard->ConstructProcess();
    // else
    // 	constructor_kncompton_standard->ConstructProcess();
    
    
    //G4VModularPhysicsList::ConstructProcess();
    G4EmParameters* param = G4EmParameters::Instance();
    param->SetIntegral(false);
}
void PhysicsList::SetCuts()
{
    this->SetDefaultCutValue(0.1*mm);
    std::cout << "PhysicsList::SetCuts()" << std::endl;
}

void PhysicsList::AddPhysics(const G4String& name)
{
    std::cout << "PhysicsList::AddPhysics" << std::endl;
    if ( name=="livermore" ) {
    	is_enabled_livermore_compton = true;
    }
    else if ( name=="standard" ) {
    	is_enabled_livermore_compton = false;
    }
    // this->RemovePhysics( constructor );
    // this->RemovePhysics( constructor->GetPhysicsName() );
    delete constructor;	
    constructor
	= new PhysicsConstructor(1, compton_model, is_enabled_livermore_compton);
    // this->RegisterPhysics( constructor );
    //this->ReplacePhysics( constructor );
    // this->ConstructProcess();
    std::cout << "Recreate PhysicsList " <<  compton_model << " " << is_enabled_livermore_compton << std::endl;
}
void PhysicsList::SetComptonModel(const G4String& name)
{
    std::cout << "PhysicsList::SetComptonModel" << std::endl;
    if ( name=="G4KleinNishinaModel" ) {
    	compton_model = name;
    }
    else if ( name=="G4KleinNishinaCompton" ) {
    	compton_model = name;
    }
    // this->RemovePhysics( constructor );
    delete constructor;
    // // this->RemovePhysics( constructor->GetPhysicsName() );
    constructor
	= new PhysicsConstructor(1, compton_model, is_enabled_livermore_compton);    
    // this->RemovePhysics( 0 );
    // this->RegisterPhysics( constructor );
    // this->ReplacePhysics( constructor );
    // this->ConstructProcess();
    std::cout << "Recreate PhysicsList " << compton_model << " " << is_enabled_livermore_compton << std::endl;
}
