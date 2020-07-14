/**
   @date 2020/06/25
**/

#include "P4PrimaryGeneratorAction.hpp"

#include <iostream>
#include <string>

#include <G4VPrimaryGenerator.hh>

P4PrimaryGeneratorAction::P4PrimaryGeneratorAction()
    : sim4py::ParameterGene("P4PrimaryGeneratorAction"),
      G4VUserPrimaryGeneratorAction()   
{
    primary_generator = nullptr;
    
    // SetParameter<std::string>("class_name", "P4PrimaryGeneratorAction");

    is_applied_parameters = false;
    
    verbose_level = 1;
    DefineParameter<int>("verbose_level", verbose_level);
}

P4PrimaryGeneratorAction::~P4PrimaryGeneratorAction()
{
}

void P4PrimaryGeneratorAction::GeneratePrimaries(G4Event* event)
{
    if ( is_applied_parameters==false ) {
	auto [ verbose ] = GetParameter<int>("verbose_level");
	verbose_level = verbose;
	is_applied_parameters = true;
    }
    
    if ( verbose_level>2 )
	std::cout << "P4PrimaryGeneratorAction::GeneratePrimaries" << std::endl;

    if ( !primary_generator ) {
	if ( verbose_level>0 )
	std::cerr << "***Error*** in P4PrimaryGeneratorAction : PrimaryGenerator is required to be set via SetPrimaryGenerator." << std::endl;
    } else	
	primary_generator->GeneratePrimaryVertex( event );
}

void P4PrimaryGeneratorAction::SetPrimaryGenerator(G4VPrimaryGenerator* gen)
{
    if ( !gen && verbose_level>0 )
	std::cerr << "***Error*** in P4PrimaryGeneratorAction : nullptr cannot be set." << std::endl;
    else 
	primary_generator = gen;
}
