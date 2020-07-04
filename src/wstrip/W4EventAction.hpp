/**
   @file    W4EventAction.hpp
   @author  Goro Yabu
   @date    2020/01/30
**/

#ifndef W4EventAction_hpp
#define W4EventAction_hpp

#include <ParameterGene.hpp>

#include <G4UserEventAction.hh>
#include <globals.hh>

class W4EventAction
    : public sim4py::ParameterGene<G4UserEventAction>
{
    
public:

    W4EventAction();
    ~W4EventAction();

    virtual void BeginOfEventAction(const G4Event*) override;
    virtual void EndOfEventAction(const G4Event*) override;

    // void SetParameter(const G4String& key, int value);

private:

    bool is_applied_parameters;
    int save_nhit_min;
    
};

#endif
