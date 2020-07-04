/**
   @file    P4RunAction.hpp
   @author  Goro Yabu
   @date    2020/01/30
**/

#ifndef P4RunAction_hpp
#define P4RunAction_hpp

#include <ParameterGene.hpp>

#include <G4UserRunAction.hh>
// #include <globals.hh>

// #include <vector>

class P4RunAction
    : public sim4py::ParameterGene<G4UserRunAction>
{

public:

    P4RunAction();
    ~P4RunAction();

    G4Run* GenerateRun() override;
    virtual void BeginOfRunAction(const G4Run*) override;
    virtual void EndOfRunAction(const G4Run*) override;   

private:

    int verbose_level;
    int print_frequency;
    bool is_set_parameters;
    
};

#endif
