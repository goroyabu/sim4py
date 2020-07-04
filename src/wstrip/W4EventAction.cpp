/**
   @file    W4EventAction.cpp
   @author  Goro Yabu
   @date    2020/01/30
**/

#include "W4EventAction.hpp"

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
    : sim4py::ParameterGene<G4UserEventAction>()
{
    SetParameter<std::string>("class_name", "W4EventAction");
    
    save_nhit_min = 1;
    DefineParameter<int>("minimum_nhits_to_save", save_nhit_min);

    is_applied_parameters = false;
}

W4EventAction::~W4EventAction()
{
}

void W4EventAction::BeginOfEventAction(const G4Event* event)
{
    if ( is_applied_parameters==false ) {
	save_nhit_min = std::get<0>( GetParameter<int>("minimum_nhits_to_save") );
    }
    
    auto analysis_manager = P4RootAnalysisManager::Instance();
    analysis_manager->ClearNtupleVector();

    G4int eventID = event->GetEventID();
    analysis_manager->FillNtupleIColumnName( "eventID", eventID );
    
    if ( eventID%10000==0 ) {
	auto ima = std::chrono::system_clock::now();
	auto ima_time = std::chrono::system_clock::to_time_t(ima);
	cout << ">> Event " << std::setw(10) << eventID << " at " << std::ctime(&ima_time);
    }	    
    // std::cout << "W4EventAction::BeginOfW4EventAction()" << std::endl;
}
void W4EventAction::EndOfEventAction(const G4Event*)
{
    auto analysis_manager = P4RootAnalysisManager::Instance();
    
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
