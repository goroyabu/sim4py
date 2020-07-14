/**
   @date 2020/06/25
**/

#include "P4ActionInitialization.hpp"
using sim4py:: operator<<;

#include <string>

#include <iostream>
using std::cout;
using std::cerr;
using std::endl;

P4ActionInitialization::P4ActionInitialization()
    : sim4py::ParameterGene("P4ActionInitialization"),
      G4VUserActionInitialization()
{
    primary_generator = nullptr;
    run_action        = new G4UserRunAction();
    event_action      = new G4UserEventAction();
    stacking_action   = new G4UserStackingAction();
    tracking_action   = new G4UserTrackingAction();
    stepping_action   = new G4UserSteppingAction();
    
    // SetParameter<std::string>("class_name", "P4ActionInitialization");
    // FixParameter<std::string>("class_name");
    
    verbose_level = 1;
    DefineParameter<int>("verbose_level", verbose_level);
}
P4ActionInitialization::~P4ActionInitialization()
{
    run_action = 0;
    event_action = 0;
    stacking_action = 0;
    tracking_action = 0;
    stepping_action = 0;
}

void P4ActionInitialization::Build() const
{
    if ( verbose_level>0 )
	cout << GetParameter<std::string>("class_name") << "::Build" << endl;   
    //cout << "P4ActionInitialization::Build" << endl;   
    
    this->SetUserAction( primary_generator );
    this->SetUserAction( run_action        );
    this->SetUserAction( event_action      );
    this->SetUserAction( stacking_action   );
    this->SetUserAction( tracking_action   );
    this->SetUserAction( stepping_action   );
}
void P4ActionInitialization::RegisterUserAction
(G4VUserPrimaryGeneratorAction* action)
{
    auto [ ver ] = GetParameter<int>("verbose_level");
    verbose_level = ver;
    if ( !action && verbose_level>0 )
	cerr << "***Error*** in " << GetParameter<std::string>("class_name") << " : nullptr cannot be registered. " << endl;
    else 
	primary_generator = action;
}
void P4ActionInitialization::RegisterUserAction
(G4UserRunAction*              action)
{
    auto [ ver ] = GetParameter<int>("verbose_level");
    verbose_level = ver;
    if ( !action && verbose_level>0 )
	cerr << "***Error*** in " << GetParameter<std::string>("class_name") << " : nullptr cannot be registered. " << endl;
    else 
	run_action = action;    
}
void P4ActionInitialization::RegisterUserAction
(G4UserEventAction*            action)
{
    auto [ ver ] = GetParameter<int>("verbose_level");
    verbose_level = ver;
    if ( !action && verbose_level>0 )
	cerr << "***Error*** in " << GetParameter<std::string>("class_name") << " : nullptr cannot be registered. " << endl;
    else 
	event_action = action;    
}
void P4ActionInitialization::RegisterUserAction
(G4UserStackingAction*         action)
{
    auto [ ver ] = GetParameter<int>("verbose_level");
    verbose_level = ver;
    if ( !action && verbose_level>0 )
	cerr << "***Error*** in " << GetParameter<std::string>("class_name") << " : nullptr cannot be registered. " << endl;
    else 
	stacking_action = action;
}
void P4ActionInitialization::RegisterUserAction
(G4UserTrackingAction*         action)
{
    auto [ ver ] = GetParameter<int>("verbose_level");
    verbose_level = ver;
    if ( !action && verbose_level>0 )
	cerr << "***Error*** in " << GetParameter<std::string>("class_name") << " : nullptr cannot be registered. " << endl;
    else 
	tracking_action = action;    
}
void P4ActionInitialization::RegisterUserAction
(G4UserSteppingAction*         action)
{
    auto [ ver ] = GetParameter<int>("verbose_level");
    verbose_level = ver;
    if ( !action && verbose_level>0 )
	cerr << "***Error*** in " << GetParameter<std::string>("class_name") << " : nullptr cannot be registered. " << endl;
    else 
	stepping_action = action;    
}


