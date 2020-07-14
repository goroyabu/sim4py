/**
   @file    P4EventAction.cpp
   @author  Goro Yabu
   @date    2020/01/30
**/

#include "P4EventAction.hpp"

#include <iostream>
using std::cout;
using std::endl;

#include <iomanip>

#include <chrono>
#include <ctime>

#include <G4Event.hh>

P4EventAction::P4EventAction()
    : sim4py::ParameterGene("P4EventAction"), G4UserEventAction()
{
    verbose_level = 1;
    print_frequency = 1000;
    DefineParameter<int>("verbose_level", verbose_level);
    DefineParameter<int>("print_frequency", print_frequency);
    is_set_parameters = false;
}

P4EventAction::~P4EventAction()
{
}

void P4EventAction::BeginOfEventAction(const G4Event* event)
{
    if ( is_set_parameters == false ) {
	auto [ ver ]  = GetParameter<int>("verbose_level");
	auto [ freq ] = GetParameter<int>("print_frequency");
	verbose_level = ver;
	print_frequency = freq;
	is_set_parameters = true;
    }
    
    G4int eventID = event->GetEventID();

    auto ima = std::chrono::system_clock::now();
    auto ima_time = std::chrono::system_clock::to_time_t(ima);
    
    if ( eventID%10000==0 && verbose_level>0 )
	cout << ">> Event " << std::setw(10) << eventID << " at " << std::ctime(&ima_time);

    if ( verbose_level>2 )  
	cout << "EventAction::BeginOfEventAction()" << endl;
}
void P4EventAction::EndOfEventAction(const G4Event*)
{
    if ( verbose_level>2 )
	cout << "P4EventAction::EndOfEventAction()" << endl;
}
