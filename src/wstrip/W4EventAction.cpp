/**
   @file    W4EventAction.cpp
   @author  Goro Yabu
   @date    2020/01/30
**/

#include "W4EventAction.hpp"
#include "W4HitsCollection.hpp"

#include <iostream>
using std::cout;
using std::endl;
#include <iomanip>
#include <string>
#include <chrono>
#include <ctime>

#include <G4Event.hh>

#include <P4RootAnalysisManager.hpp>

W4EventAction::W4EventAction()
    : sim4py::ParameterGene("W4EventAction"), G4UserEventAction()
{
    // SetParameter<std::string>("class_name", "W4EventAction");
    
    save_nhit_min = 1;
    DefineParameter<int>("minimum_nhits_to_save", save_nhit_min);

    print_frequency = 1000;
    DefineParameter<int>("print_frequency", print_frequency);
    
    is_applied_parameters = false;
}

W4EventAction::~W4EventAction()
{
}

void W4EventAction::BeginOfEventAction(const G4Event* event)
{
    if ( is_applied_parameters==false ) {
	save_nhit_min = std::get<0>( GetParameter<int>("minimum_nhits_to_save") );
	print_frequency
	    = std::get<0>( GetParameter<int>("print_frequency") );
	is_applied_parameters = true;
    }
    
    auto analysis_manager = P4RootAnalysisManager::Instance();
    analysis_manager->ClearNtupleVector();

    G4int eventID = event->GetEventID();
    analysis_manager->FillNtupleIColumnName( "eventID", eventID );

    // auto primary = event->GetPrimaryVertex();
    // auto e = primary->GetPrimary()->GetTotalEnergy();
    // auto x = primary->GetX0();
    // auto y = primary->GetY0();
    // auto z = primary->GetZ0();

    // analysis_manager->FillNtupleDColumnName( "init_e", e/CLHEP::keV );
    // analysis_manager->FillNtupleDColumnName( "init_x", x );
    // analysis_manager->FillNtupleDColumnName( "init_y", y );
    // analysis_manager->FillNtupleDColumnName( "init_z", z );
    
    if ( eventID%print_frequency==0 ) {
	auto ima = std::chrono::system_clock::now();
	auto ima_time = std::chrono::system_clock::to_time_t(ima);
	cout << ">> Event " << std::setw(10) << eventID << " at " << std::ctime(&ima_time);
    }	    
    // std::cout << "W4EventAction::BeginOfW4EventAction()" << std::endl;
}
void W4EventAction::EndOfEventAction(const G4Event*event)
{
    auto analysis_manager = P4RootAnalysisManager::Instance();

    auto hce = event->GetHCofThisEvent();
    auto nofhc = hce->GetCapacity();

    auto hits_overall = new std::vector<W4DSDHit*>;
    
    for ( int ihc=0; ihc<nofhc; ++ihc ) {
	auto hc = dynamic_cast<W4DSDHitsCollection*>( hce->GetHC( ihc ) );
	auto hits = hc->GetVector();
	std::copy( hits->begin(), hits->end(), std::back_inserter(*hits_overall) );

	if ( hits->empty()==false )
	    analysis_manager->AddNtupleIColumnName( "hit_pattern", (int)pow(10, hits->at(0)->DetectorID()) );
    }

    auto merged_hits = W4DSDHit::MergeHits( hits_overall );
    std::sort( merged_hits.begin(), merged_hits.end(),
               [](const W4DSDHit& former, const W4DSDHit& latter)
               {
                   return former.Time() < latter.Time();
               });

    auto nhits = (int)merged_hits.size();
    analysis_manager->AddNtupleIColumnName( "nhits", nhits );

    for ( auto hit : merged_hits ) {
        analysis_manager->FillNtupleDColumnVName( "edep",              hit.Energy() );
        analysis_manager->FillNtupleIColumnVName( "detid",             hit.DetectorID() );
	analysis_manager->FillNtupleIColumnVName( "material",          hit.Material() );
        analysis_manager->FillNtupleDColumnVName( "global_time",       hit.Time() );
        analysis_manager->FillNtupleIColumnVName( "strip_x",           hit.StripIDX()  );
        analysis_manager->FillNtupleIColumnVName( "strip_y",           hit.StripIDY()   );
        analysis_manager->FillNtupleDColumnVName( "pos_x",             hit.X() );
        analysis_manager->FillNtupleDColumnVName( "pos_y",             hit.Y() );
        analysis_manager->FillNtupleDColumnVName( "pos_z",             hit.Z() );
        analysis_manager->FillNtupleDColumnVName( "dir_x",             hit.DirX() );
        analysis_manager->FillNtupleDColumnVName( "dir_y",             hit.DirY() );
        analysis_manager->FillNtupleDColumnVName( "dir_z",             hit.DirZ() );
        analysis_manager->AddNtupleDColumnName  ( "etotal",            hit.Energy() );
        analysis_manager->AddNtupleSColumnName  ( "proc_name",         hit.ProcessName() );
        // auto [ x, y ] = GetCenterOfPixel( hit.StripIDX(), hit.StripIDY() );
        analysis_manager->FillNtupleDColumnVName( "pixel_center_x",    hit.PixelCenterX() );
        analysis_manager->FillNtupleDColumnVName( "pixel_center_y",    hit.PixelCenterY() );
	analysis_manager->FillNtupleDColumnVName( "pixel_center_z",    hit.PixelCenterZ() );
        analysis_manager->FillNtupleIColumnVName( "nmerged_raw_hits",  hit.Nmerged() );
        // analysis_manager->AddNtupleSColumnName  ( "raw_proc_name",     hit.MergedProcessName() );
    }    
    
    if ( analysis_manager->GetNtupleIColumn("nhits")>=save_nhit_min ) {
	analysis_manager->AddNtupleRow();
    }
    // std::cout << "W4EventAction::EndOfW4EventAction()" << std::endl;
}
// void W4EventAction::SetParameter(const G4String& key, int value)
// {
//     if ( key == "SaveMinNhits" )
// 	save_nhit_min = value;    
// }
