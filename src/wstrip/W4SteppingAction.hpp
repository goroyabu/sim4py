/**
   @file    W4SteppingAction.hpp
   @author  Goro Yabu
   @date    2020/09/04
**/

#ifndef W4SteppingAction_hpp
#define W4SteppingAction_hpp

#include <ParameterGene.hpp>

#include <G4UserSteppingAction.hh>
#include <globals.hh>

class W4SteppingAction
    : public sim4py::ParameterGene, public G4UserSteppingAction
{

public:

    W4SteppingAction();
    ~W4SteppingAction();

    virtual void UserSteppingAction(const G4Step*);
    
};

#endif
