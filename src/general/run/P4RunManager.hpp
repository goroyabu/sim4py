/**
   @date 2020/06/30
**/

#ifndef P4RunManager_hpp
#define P4RunManager_hpp

#include <ParameterGene.hpp>

#include <G4RunManager.hh>

class G4UIExecutive;
class G4VisExecutive;

class P4RunManager
    : public sim4py::ParameterGene, public G4RunManager
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

    virtual void UseReferencePhysicsList(const std::string& name="FTFP_BERT");
    virtual void UseGenericPhysicsList(const std::vector<std::string>& constructors);
    
private:

    /* parameters */
    int verbose_level;
    bool is_enabled_visualization;
    
    bool is_set_detector_construction;
    bool is_set_physics_list;
    bool is_set_action_initialization;
    bool is_already_setup_vis;

    G4UIExecutive * ui_executive;
    G4VisExecutive * vis_executive;

    G4VUserPhysicsList          * physics_list;
    G4VUserActionInitialization * action_initialization;
    G4VUserDetectorConstruction * detector_construction;
    
    void SetupVisualization();
    
};

#endif
