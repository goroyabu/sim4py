/**
   @date 2020/06/25
**/

#ifndef P4ActionInitialization_hpp
#define P4ActionInitialization_hpp

#include <ParameterGene.hpp>

#include <G4VUserActionInitialization.hh>
#include <G4VUserPrimaryGeneratorAction.hh>
#include <G4UserRunAction.hh>
#include <G4UserEventAction.hh>
#include <G4UserStackingAction.hh>
#include <G4UserTrackingAction.hh>
#include <G4UserSteppingAction.hh>

class P4ActionInitialization
    : public sim4py::ParameterGene<G4VUserActionInitialization>
{
    
public:

    P4ActionInitialization();
    virtual ~P4ActionInitialization();
    
    virtual void Build() const override;

    void RegisterUserAction(G4VUserPrimaryGeneratorAction* action);
    void RegisterUserAction(G4UserRunAction*               action);
    void RegisterUserAction(G4UserEventAction*             action);
    void RegisterUserAction(G4UserStackingAction*          action);
    void RegisterUserAction(G4UserTrackingAction*          action);
    void RegisterUserAction(G4UserSteppingAction*          action);
    
protected:

    int verbose_level;
    G4VUserPrimaryGeneratorAction*  primary_generator;
    G4UserRunAction*                run_action;
    G4UserEventAction*              event_action;
    G4UserStackingAction*           stacking_action;
    G4UserTrackingAction*           tracking_action;
    G4UserSteppingAction*           stepping_action;
};

#endif
