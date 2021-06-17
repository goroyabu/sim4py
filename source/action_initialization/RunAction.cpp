/**
   @file    RunAction.cpp
   @author  Goro Yabu
   @date    2020/01/30
**/

#include "RunAction.hpp"
#include "AnalysisManager.hpp"

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

RunAction::RunAction()
    : G4UserRunAction()
{
    auto analysis_manager = AnalysisManager::Instance();
    analysis_manager->SetActivation(true);
    analysis_manager->SetVerboseLevel(1);
    analysis_manager->SetFileName("test_lv3");

    G4int id = analysis_manager->CreateH1("spect","Deposited energy",
					  100, 0., 100.);
    analysis_manager->SetH1Activation(id, false);
}

RunAction::~RunAction()
{
}

G4Run* RunAction::GenerateRun()
{
    std::cout << "RunAction::GenerateRun()" << std::endl;
    return G4UserRunAction::GenerateRun();
}
void RunAction::BeginOfRunAction(const G4Run*)
{
    auto analysis_manager = AnalysisManager::Instance();

    auto file_name = (G4String)analysis_manager->GetFileName();
    auto ima = std::chrono::system_clock::now();
    auto ima_time = std::chrono::system_clock::to_time_t(ima);
    std::stringstream ss;
    ss << std::put_time( std::localtime(&ima_time), file_name.c_str() );
    file_name = ss.str();

    // auto ima = std::chrono::system_clock::now();
    // auto ima_time = std::chrono::system_clock::to_time_t(ima);
    // std::stringstream ss;
    // ss << std::put_time( std::localtime(&ima_time), "%Y%m%d_%H%M%S" );
    // auto ima_str = ss.str();
    // auto outname = (G4String)analysis_manager->GetFileName()+"_"+ima_str+".root";
    
    // analysis_manager->SetFileName( outname );
    analysis_manager->SetFileName( file_name );
    analysis_manager->OpenFile();

    // analysis_manager->CreateNtuple( "g4tree", "g4tree" );
    // analysis_manager->CreateNtupleIColumn( "eventID" );
    // analysis_manager->CreateNtupleIColumn( "nhits" );
    // analysis_manager->CreateNtupleIColumn( "hit_pattern" );
    // analysis_manager->CreateNtupleDColumn( "etotal" );
    // analysis_manager->CreateNtupleIColumnV( "detid",   128 );
    // analysis_manager->CreateNtupleIColumnV( "strip_x", 128 );
    // analysis_manager->CreateNtupleIColumnV( "strip_y", 128 );    
    // analysis_manager->CreateNtupleDColumnV( "edep",    128 );
    // analysis_manager->CreateNtupleDColumnV( "pos_x",   128 );
    // analysis_manager->CreateNtupleDColumnV( "pos_y",   128 );
    // analysis_manager->CreateNtupleDColumnV( "pos_z",   128 );
    // analysis_manager->CreateNtupleDColumnV( "pixel_center_x", 128 );
    // analysis_manager->CreateNtupleDColumnV( "pixel_center_y", 128 );
    // analysis_manager->CreateNtupleSColumn( "proc_name" );
    // analysis_manager->FinishNtuple();

    const static int array_size = 1024; //128
    analysis_manager->CreateNtuple( "g4tree", "g4tree" );
    analysis_manager->CreateNtupleIColumn( "eventID" );
    analysis_manager->CreateNtupleIColumn( "nhits" );
    analysis_manager->CreateNtupleIColumn( "hit_pattern" );
    analysis_manager->CreateNtupleDColumn( "etotal" );
    analysis_manager->CreateNtupleIColumnV( "detid",   array_size );
    analysis_manager->CreateNtupleIColumnV( "material",   array_size );
    analysis_manager->CreateNtupleDColumnV( "global_time",   array_size );
    analysis_manager->CreateNtupleIColumnV( "strip_x", array_size );
    analysis_manager->CreateNtupleIColumnV( "strip_y", array_size );    
    analysis_manager->CreateNtupleDColumnV( "edep",    array_size );
    analysis_manager->CreateNtupleDColumnV( "ekin",    array_size );
    analysis_manager->CreateNtupleDColumnV( "pos_x",   array_size );
    analysis_manager->CreateNtupleDColumnV( "pos_y",   array_size );
    analysis_manager->CreateNtupleDColumnV( "pos_z",   array_size );
    analysis_manager->CreateNtupleDColumnV( "dir_x",   array_size );
    analysis_manager->CreateNtupleDColumnV( "dir_y",   array_size );
    analysis_manager->CreateNtupleDColumnV( "dir_z",   array_size );
    analysis_manager->CreateNtupleDColumnV( "bgn_x",   array_size );
    analysis_manager->CreateNtupleDColumnV( "bgn_y",   array_size );
    analysis_manager->CreateNtupleDColumnV( "bgn_z",   array_size );
    analysis_manager->CreateNtupleDColumnV( "end_x",  array_size );
    analysis_manager->CreateNtupleDColumnV( "end_y",  array_size );
    analysis_manager->CreateNtupleDColumnV( "end_z",  array_size );
    analysis_manager->CreateNtupleDColumnV( "in_x",    array_size );
    analysis_manager->CreateNtupleDColumnV( "in_y",    array_size );
    analysis_manager->CreateNtupleDColumnV( "in_z",    array_size );    
    analysis_manager->CreateNtupleDColumnV( "out_x",   array_size );    
    analysis_manager->CreateNtupleDColumnV( "out_y",   array_size );
    analysis_manager->CreateNtupleDColumnV( "out_z",   array_size );
    analysis_manager->CreateNtupleDColumnV( "step_length",    array_size );
    analysis_manager->CreateNtupleDColumnV( "pixel_center_x", array_size );
    analysis_manager->CreateNtupleDColumnV( "pixel_center_y", array_size );
    analysis_manager->CreateNtupleDColumnV( "pixel_center_z", array_size );
    analysis_manager->CreateNtupleDColumn( "init_e" );
    analysis_manager->CreateNtupleDColumn( "init_x" );
    analysis_manager->CreateNtupleDColumn( "init_y" );
    analysis_manager->CreateNtupleDColumn( "init_z" );
    analysis_manager->CreateNtupleSColumn( "proc_name" );
    analysis_manager->CreateNtupleSColumn( "part_name" );
    // analysis_manager->CreateNtupleIColumnV( "nmerged_raw_hits", array_size );    
    analysis_manager->FinishNtuple();
    
    std::cout << "RunAction::BeginOfRunAction()" << std::endl;
}
void RunAction::EndOfRunAction(const G4Run*)
{
    auto analysis_manager = AnalysisManager::Instance();    
    analysis_manager->Write();
    analysis_manager->CloseFile();
    std::cout << "RunAction::EndOfRunAction()" << std::endl;
}
