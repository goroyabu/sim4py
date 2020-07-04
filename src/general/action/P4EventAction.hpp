/**
   @file    P4EventAction.hpp
   @author  Goro Yabu
   @date    2020/01/30
**/

#ifndef P4EventAction_hpp
#define P4EventAction_hpp

#include <ParameterGene.hpp>

#include <G4UserEventAction.hh>
// #include <globals.hh>

class P4EventAction
    : public sim4py::ParameterGene<G4UserEventAction>
{
    
public:

    P4EventAction();
    ~P4EventAction();

    virtual void BeginOfEventAction(const G4Event*) override;
    virtual void EndOfEventAction(const G4Event*) override;

private:

    int verbose_level;
    int print_frequency;
    bool is_set_parameters;
    
};

#endif
