/**
   @file    W4RunAction.hpp
   @author  Goro Yabu
   @date    2020/01/30
**/

#ifndef W4RunAction_hpp
#define W4RunAction_hpp

#include <ParameterGene.hpp>

#include <G4UserRunAction.hh>
#include <globals.hh>

#include <vector>

class W4RunAction
    : public sim4py::ParameterGene<G4UserRunAction>
{

public:

    W4RunAction();
    virtual ~W4RunAction();

    virtual G4Run* GenerateRun() override;
    virtual void BeginOfRunAction(const G4Run*) override;
    virtual void EndOfRunAction(const G4Run*) override;

    static W4RunAction* Instance();
    
private:

    static W4RunAction * W4RunActionInstance;
    int verbose_level;

};

#endif
