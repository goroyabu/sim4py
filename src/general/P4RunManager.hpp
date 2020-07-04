/**
   @date 2020/06/30
**/

#ifndef P4RunManager_hpp
#define P4RunManager_hpp

#include <ParameterGene.hpp>

#include <G4RunManager.hh>

class P4RunManager
    : public sim4py::ParameterGene<G4RunManager>
{
    
public:

    P4RunManager();
    virtual ~P4RunManager();

    virtual void BeamOn
    (G4int n_event, const char *macroFile=0, G4int n_select=-1) override;
    virtual void Initialize() override;
    virtual void SetUserInitialization
    (G4VUserDetectorConstruction *userInit) override;
    virtual void SetUserInitialization
    (G4VUserPhysicsList *userInit) override;
    virtual void SetUserInitialization
    (G4VUserActionInitialization *userInit) override;

    virtual void SetReferencePhysicsList(const std::string& name="FTFP_BERT");
    
private:

    bool is_set_detector_construction;
    bool is_set_physics_list;
    bool is_set_action_initialization;
    int verbose_level;
    
};

#endif
