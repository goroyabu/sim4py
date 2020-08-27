/**
   @file    W4RunAction.cpp
   @author  Goro Yabu
   @date    2020/01/30
**/

#include "W4RunAction.hpp"

#include <P4RootAnalysisManager.hpp>

#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <iomanip>
#include <chrono>
#include <ctime>

#include <g4root.hh>

using std::cout;
using std::cerr;
using std::endl;

W4RunAction* W4RunAction::W4RunActionInstance = 0;

W4RunAction::W4RunAction()
    : sim4py::ParameterGene("W4RunAction"), G4UserRunAction()
{
    // SetParameter<std::string>("class_name", "W4RunAction");

    std::string file_name = "sim_%Y%m%d_%H%M%S.root";
    DefineParameter<std::string>("file_name", file_name);

    verbose_level = 1;
    DefineParameter<int>("verbose_level", verbose_level);
    
    auto analysis_manager = P4RootAnalysisManager::Instance();
    analysis_manager->SetActivation(true);
    analysis_manager->SetVerboseLevel(1);
    analysis_manager->SetFileName(file_name);

    G4int id = analysis_manager->CreateH1("spect","Deposited energy",
					  100, 0., 100.);
    analysis_manager->SetH1Activation(id, false);
    // if ( W4RunActionInstance==nullptr )
}

W4RunAction::~W4RunAction()
{
}

G4Run* W4RunAction::GenerateRun()
{
    verbose_level = std::get<0>( GetParameter<int>("verbose_level") );    

    if ( verbose_level>0 )
	cout << "W4RunAction is generating a run." << endl;    
    
    return G4UserRunAction::GenerateRun();
}
void W4RunAction::BeginOfRunAction(const G4Run*)
{
    auto analysis_manager = P4RootAnalysisManager::Instance();

    auto [ file_name ] = GetParameter<std::string>("file_name");
    auto ima = std::chrono::system_clock::now();
    auto ima_time = std::chrono::system_clock::to_time_t(ima);
    std::stringstream ss;
    ss << std::put_time( std::localtime(&ima_time), file_name.c_str() );
    file_name = ss.str();
    //ss << std::put_time( std::localtime(&ima_time), "%Y%m%d_%H%M%S" );
    //auto ima_str = ss.str();
    //auto outname = (G4String)analysis_manager->GetFileName()+"_"+ima_str+".root";
    
    analysis_manager->SetFileName( file_name );
    analysis_manager->OpenFile();

    analysis_manager->CreateNtuple( "g4tree", "g4tree" );
    analysis_manager->CreateNtupleIColumn( "eventID" );
    analysis_manager->CreateNtupleIColumn( "nhits" );
    analysis_manager->CreateNtupleIColumn( "hit_pattern" );
    analysis_manager->CreateNtupleDColumn( "etotal" );
    analysis_manager->CreateNtupleIColumnV( "detid",   128 );
    analysis_manager->CreateNtupleDColumnV( "global_time",   128 );
    analysis_manager->CreateNtupleIColumnV( "strip_x", 128 );
    analysis_manager->CreateNtupleIColumnV( "strip_y", 128 );    
    analysis_manager->CreateNtupleDColumnV( "edep",    128 );
    analysis_manager->CreateNtupleDColumnV( "pos_x",   128 );
    analysis_manager->CreateNtupleDColumnV( "pos_y",   128 );
    analysis_manager->CreateNtupleDColumnV( "pos_z",   128 );
    analysis_manager->CreateNtupleDColumnV( "dir_x", 128 );
    analysis_manager->CreateNtupleDColumnV( "dir_y", 128 );
    analysis_manager->CreateNtupleDColumnV( "dir_z", 128 );    
    analysis_manager->CreateNtupleDColumnV( "pixel_center_x", 128 );
    analysis_manager->CreateNtupleDColumnV( "pixel_center_y", 128 );
    analysis_manager->CreateNtupleDColumn( "init_e" );
    analysis_manager->CreateNtupleDColumn( "init_x" );
    analysis_manager->CreateNtupleDColumn( "init_y" );
    analysis_manager->CreateNtupleDColumn( "init_z" );
    analysis_manager->CreateNtupleSColumn( "proc_name" );
    analysis_manager->CreateNtupleIColumnV( "nmerged_raw_hits", 128 );
    analysis_manager->CreateNtupleSColumn( "raw_proc_name" );    
    
    analysis_manager->FinishNtuple();

    if ( verbose_level>0 )
	cout << "W4RunAction::BeginOfW4RunAction()" << endl;
}

void W4RunAction::EndOfRunAction(const G4Run*)
{
    auto analysis_manager = P4RootAnalysisManager::Instance();    
    analysis_manager->Write();
    analysis_manager->CloseFile();

    if ( verbose_level>0 )
	cout << "W4RunAction::EndOfRunAction()" << endl;
}

W4RunAction* W4RunAction::Instance()
{
    if ( W4RunActionInstance==nullptr )
	new W4RunAction;
    return W4RunActionInstance;
}
