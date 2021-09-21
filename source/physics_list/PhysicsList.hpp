/**
   @file PhysicsList.hpp
   @author  Goro Yabu
   @date    2019/12/22
**/

#ifndef PhysicsList_hpp
#define PhysicsList_hpp

#include <G4VModularPhysicsList.hh>
#include <globals.hh>

class PhysicsList : public G4VModularPhysicsList
{
    
public:
    
    PhysicsList();
    ~PhysicsList();

    void ConstructParticle() override;
    void ConstructProcess() override;
    void SetCuts() override;    
    void AddPhysics(const G4String& name);
    void SetComptonModel(const G4String& name);
    
private:

    G4String compton_model;
    bool is_enabled_livermore_compton;

    G4VPhysicsConstructor * constructor;
    // G4VPhysicsConstructor * constructor_knmodel_standard;
    // G4VPhysicsConstructor * constructor_kncompton_standard;
    // G4VPhysicsConstructor * constructor_knmodel_livermore;
    // G4VPhysicsConstructor * constructor_kncompton_livermore;
    
};

#endif
