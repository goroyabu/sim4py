/**
   @file    P4RunAction.cpp
   @author  Goro Yabu
   @date    2020/01/30
**/

#include "P4RunAction.hpp"

// #include <P4RootAnalysisManager.hpp>

#include <iostream>
using std::cout;
using std::endl;

#include <sstream>
#include <string>
// #include <vector>
#include <iomanip>
#include <chrono>
#include <ctime>

// #include <g4root.hh>

P4RunAction::P4RunAction()
    : sim4py::ParameterGene<G4UserRunAction>()
{
    SetParameter<std::string>("class_name", "P4RunAction");

    verbose_level = 1;
    print_frequency = 1000;
    DefineParameter<int>("verbose_level", verbose_level);
    DefineParameter<int>("print_frequency", print_frequency);
    is_set_parameters = false;
}

P4RunAction::~P4RunAction()
{
}

G4Run* P4RunAction::GenerateRun()
{
    if ( is_set_parameters == false ) {
	auto [ ver ]  = GetParameter<int>("verbose_level");
	auto [ freq ] = GetParameter<int>("print_frequency");
	verbose_level = ver;
	print_frequency = freq;
	is_set_parameters = true;
    }

    if ( verbose_level>0 )
	cout << "P4RunAction::GenerateRun()" << endl;

    return G4UserRunAction::GenerateRun();
}
void P4RunAction::BeginOfRunAction(const G4Run*)
{
    auto ima = std::chrono::system_clock::now();
    auto ima_time = std::chrono::system_clock::to_time_t(ima);
    std::stringstream ss;
    ss << std::put_time( std::localtime(&ima_time), "%Y%m%d_%H%M%S" );
    auto ima_str = ss.str();
    // auto outname = analysis_manager->GetFileName()+"_"+ima_str+".root";

    if ( verbose_level>0 )
	cout << ima_str << endl;

    if ( verbose_level>0 )
	cout << "P4RunAction::BeginOfP4RunAction()" << endl;
}
void P4RunAction::EndOfRunAction(const G4Run*)
{
    if ( verbose_level>0 )
	cout << "P4RunAction::EndOfP4RunAction()" << endl;
}
