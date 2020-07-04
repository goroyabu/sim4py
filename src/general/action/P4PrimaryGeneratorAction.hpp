/**
   @date 2020/06/25
**/

#ifndef P4PrimaryGeneratorAction_hpp
#define P4PrimaryGeneratorAction_hpp

#include <G4VUserPrimaryGeneratorAction.hh>
#include <ParameterGene.hpp>

//class G4VUserPrimaryGeneratorAction;
class G4VPrimaryGenerator;

class P4PrimaryGeneratorAction
    : public sim4py::ParameterGene<G4VUserPrimaryGeneratorAction>
{
    
public:
    
    P4PrimaryGeneratorAction();
    ~P4PrimaryGeneratorAction();

    virtual void GeneratePrimaries(G4Event* event) override;

    void SetPrimaryGenerator(G4VPrimaryGenerator* gen);

private:

    bool is_applied_parameters;
    int verbose_level;
    
    G4VPrimaryGenerator* primary_generator;    
    
};

#endif
