/**
   @file    EventAction.cpp
   @author  Goro Yabu
   @date    2020/01/30
**/

#include "EventAction.hpp"
#include "AnalysisManager.hpp"
#include "HitsCollection.hpp"

#include <iostream>
#include <string>
#include <iomanip>
#include <chrono>
#include <ctime>

#include <G4Event.hh>

using std::cout;
using std::endl;

EventAction::EventAction()
    : G4UserEventAction()
{
    save_nhit_min = 1;
}

EventAction::~EventAction()
{
}

void EventAction::BeginOfEventAction(const G4Event* event)
{
    auto analysis_manager = AnalysisManager::Instance();
    analysis_manager->ClearNtupleVector();

    G4int eventID = event->GetEventID();
    analysis_manager->FillNtupleIColumnName( "eventID", eventID );
    
    if ( eventID%10000==0 ) {
        auto ima = std::chrono::system_clock::now();
        auto ima_time = std::chrono::system_clock::to_time_t(ima);
        //auto ima_str = std::put_time( std::localtime(&ima_time), "%Y%m%d_%H%M%S" ); 
        cout << ">> Event " << std::setw(10) << eventID << " at " << std::ctime(&ima_time);
    }
    // std::cout << "EventAction::BeginOfEventAction()" << std::endl;
}
void EventAction::EndOfEventAction(const G4Event* event)
{
    auto analysis_manager = AnalysisManager::Instance();
    
    auto hce = event->GetHCofThisEvent();
    auto nofhc = hce->GetCapacity();
    // cout << nofhc << endl;

    auto hits_overall = new std::vector<DSDHit*>;
    
    for ( int ihc=0; ihc<nofhc; ++ihc ) {
        auto hc = dynamic_cast<DSDHitsCollection*>( hce->GetHC( ihc ) );
        auto hits = hc->GetVector();
        std::copy( hits->begin(), hits->end(), std::back_inserter(*hits_overall) );

        if ( hits->empty()==false )
            analysis_manager->AddNtupleIColumnName
                ( "hit_pattern", (int)pow(10, hits->at(0)->DetectorID()) );
    }

    // auto merged_hits = W4DSDHit::MergeHits( hits_overall );
    // std::sort( merged_hits.begin(), merged_hits.end(),
    //            [](const W4DSDHit& former, const W4DSDHit& latter)
    //            {
    //                return former.Time() < latter.Time();
    //            });

    // auto nhits = (int)merged_hits.size();
    auto nhits = (int)hits_overall->size();
    analysis_manager->AddNtupleIColumnName( "nhits", nhits );

    for ( auto hit : *hits_overall ) {
        analysis_manager->FillNtupleDColumnVName( "edep",              hit->Energy() );
        analysis_manager->FillNtupleDColumnVName( "ekin",              hit->KineticEnergy() );
        analysis_manager->FillNtupleIColumnVName( "detid",             hit->DetectorID() );
        analysis_manager->FillNtupleIColumnVName( "material",          hit->Material() );
        analysis_manager->FillNtupleDColumnVName( "global_time",       hit->Time() );
        analysis_manager->FillNtupleIColumnVName( "strip_x",           hit->StripIDX()  );
        analysis_manager->FillNtupleIColumnVName( "strip_y",           hit->StripIDY()   );
        analysis_manager->FillNtupleDColumnVName( "pos_x",             hit->X() );
        analysis_manager->FillNtupleDColumnVName( "pos_y",             hit->Y() );
        analysis_manager->FillNtupleDColumnVName( "pos_z",             hit->Z() );
        analysis_manager->FillNtupleDColumnVName( "dir_x",             hit->DirX() );
        analysis_manager->FillNtupleDColumnVName( "dir_y",             hit->DirY() );
        analysis_manager->FillNtupleDColumnVName( "dir_z",             hit->DirZ() );

        analysis_manager->FillNtupleDColumnVName( "bgn_x",             hit->PosPreX()  );
        analysis_manager->FillNtupleDColumnVName( "bgn_y",             hit->PosPreY()  );
        analysis_manager->FillNtupleDColumnVName( "bgn_z",             hit->PosPreZ()  );
        analysis_manager->FillNtupleDColumnVName( "end_x",             hit->PosPostX() );
        analysis_manager->FillNtupleDColumnVName( "end_y",             hit->PosPostY() );
        analysis_manager->FillNtupleDColumnVName( "end_z",             hit->PosPostZ() );
        analysis_manager->FillNtupleDColumnVName( "in_x",              hit->DirPreX()  );
        analysis_manager->FillNtupleDColumnVName( "in_y",              hit->DirPreY()  );
        analysis_manager->FillNtupleDColumnVName( "in_z",              hit->DirPreZ()  );
        analysis_manager->FillNtupleDColumnVName( "out_x",             hit->DirPostX() );
        analysis_manager->FillNtupleDColumnVName( "out_y",             hit->DirPostY() );
        analysis_manager->FillNtupleDColumnVName( "out_z",             hit->DirPostZ() );
        analysis_manager->FillNtupleDColumnVName( "step_length",       hit->StepLength());
	
        analysis_manager->AddNtupleDColumnName  ( "etotal",            hit->Energy() );
        analysis_manager->AddNtupleSColumnName  ( "proc_name",         hit->ProcessName() );
        analysis_manager->AddNtupleSColumnName  ( "part_name",         hit->ParticleName() );
        analysis_manager->FillNtupleDColumnVName( "pixel_center_x",    hit->PixelCenterX() );
        analysis_manager->FillNtupleDColumnVName( "pixel_center_y",    hit->PixelCenterY() );
        analysis_manager->FillNtupleDColumnVName( "pixel_center_z",    hit->PixelCenterZ() );
        // analysis_manager->FillNtupleIColumnVName( "nmerged_raw_hits",  hit->Nmerged() );
    }    
    
    // analysis_manager->AddNtupleRow();
    
    // cout << nhits << endl;
    if ( analysis_manager->GetNtupleIColumn("nhits")>=save_nhit_min ) {
        analysis_manager->AddNtupleRow();
    }
    // std::cout << "EventAction::EndOfEventAction()" << std::endl;
}
void EventAction::SetParameter(const G4String& key, int value)
{
    if ( key == "SaveMinNhits" )
        save_nhit_min = value;    
}
