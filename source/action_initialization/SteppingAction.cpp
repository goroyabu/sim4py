/**
   @file    SteppingAction.cpp
   @author  Goro Yabu
   @date    2020/01/30
**/

#include "SteppingAction.hpp"
#include "AnalysisManager.hpp"

#include <iostream>

#include <G4Step.hh>
#include <G4Track.hh>
#include <G4VProcess.hh>

SteppingAction::SteppingAction()
    : G4UserSteppingAction()
{
}

SteppingAction::~SteppingAction()
{
}

void SteppingAction::UserSteppingAction( const G4Step * step)
{
    G4Track* track = step->GetTrack();
    if ( track->GetTrackID()!=1 || track->GetCurrentStepNumber()!=1 ) return; 

    auto primary_point = step->GetPreStepPoint();
    auto first_reaction_point = step->GetPostStepPoint();
    auto process_name 
        = first_reaction_point->GetProcessDefinedStep()->GetProcessName();
    auto e = primary_point->GetTotalEnergy();

    if ( process_name=="RadioactiveDecay" ) {
        e = 0.0;
        
        auto secondaries
            = step->GetSecondaryInCurrentStep();

        if ( secondaries->empty()==false ) {
            auto particle = secondaries->at(0)->GetDefinition();	    
            if ( particle->GetParticleType()=="gamma" )	
                e = secondaries->at(0)->GetTotalEnergy();
        }
    }
    auto p = primary_point->GetPosition();
    
    auto x = p.x();
    auto y = p.y();
    auto z = p.z();

    auto analysis_manager = AnalysisManager::Instance();
    analysis_manager->FillNtupleDColumnName( "init_e", e/CLHEP::keV );
    analysis_manager->FillNtupleDColumnName( "init_x", x );
    analysis_manager->FillNtupleDColumnName( "init_y", y );
    analysis_manager->FillNtupleDColumnName( "init_z", z );
    // std::cout << "SteppingAction::UserSteppingAction()" << std::endl;
}
