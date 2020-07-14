/**
   @date 2020/06/22
**/

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/operators.h>

#include <general-py.hpp>

#include <G4RunManager.hh>
#include <G4VUserDetectorConstruction.hh>
#include <G4VUserPhysicsList.hh>
#include <G4VUserActionInitialization.hh>

#include <G4VisExecutive.hh>
#include <G4UIExecutive.hh>
#include <G4UImanager.hh>

#include <G4PhysListFactory.hh>
#include <G4VModularPhysicsList.hh>
#include <FTFP_BERT.hh>
#include <G4EmStandardPhysics.hh>
#include <G4EmLivermorePhysics.hh>
#include <G4EmPenelopePhysics.hh>
#include <G4EmLowEPPhysics.hh>    

#include <P4RunManager.hpp>

#include <P4ActionInitialization.hpp>
#include <P4GeneralParticleSource.hpp>
#include <P4ParticleGun.hpp>
#include <P4PrimaryGeneratorAction.hpp>

#include <P4RootAnalysisManager.hpp>

#include <P4DetectorConstruction.hpp>

namespace general_py
{
    void define_general(pybind11::module& main)
    {
	using pybind11::detail::overload_cast_impl;
	
	pybind11::module sub = main.def_submodule("general","Sub-module including general Geant4 Classes.");

	using sim4py::ParameterGene;
        pybind11::class_<ParameterGene>(sub, "ParameterGene", pybind11::module_local());

	pybind11::class_<G4RunManager>(sub, "G4RunManager");
	    // .def(pybind11::init<>())
	    // .def("BeamOn", &G4RunManager::BeamOn, pybind11::arg("n_event"), pybind11::arg("macroFile")='\0', pybind11::arg("n_select")=-1)
	    // .def("Initialize", &G4RunManager::Initialize)
	    // .def("SetVerboseLevel", &G4RunManager::SetVerboseLevel)
	    // .def("GetVerboseLevel", &G4RunManager::GetVerboseLevel)
	    // .def("SetUserInitialization", [](G4RunManager& self, G4VUserDetectorConstruction* detector){ self.G4RunManager::SetUserInitialization(detector);})
	    // .def("SetUserInitialization", [](G4RunManager& self, G4VUserPhysicsList* physics){ self.G4RunManager::SetUserInitialization(physics);})
	    // .def("SetUserInitialization", [](G4RunManager& self, G4VUserActionInitialization* action){ self.G4RunManager::SetUserInitialization(action);});
	
	    // .def("SetUserInitialization", overload_cast_impl<G4VUserDetectorConstruction*>()(&G4RunManager::SetUserInitialization))
	    // .def("SetUserInitialization", overload_cast_impl<G4VUserPhysicsList*>()(&G4RunManager::SetUserInitialization))
	    // .def("SetUserInitialization", overload_cast_impl<G4VUserActionInitialization*>()(&G4RunManager::SetUserInitialization));
	
	auto p4run_manager = pybind11::class_<P4RunManager, ParameterGene, G4RunManager>(sub, "P4RunManager")
	    .def(pybind11::init<>())
	    .def("BeamOn", &P4RunManager::BeamOn, pybind11::arg("n_event"), pybind11::arg("macroFile")='\0', pybind11::arg("n_select")=-1)
	    .def("Initialize", &P4RunManager::Initialize)
	    .def("SetVerboseLevel", &P4RunManager::SetVerboseLevel)
	    .def("GetVerboseLevel", &P4RunManager::GetVerboseLevel)
	    // .def("SetUserInitialization", [](P4RunManager& self, G4VUserDetectorConstruction* detector){
	    // 				      auto det = std::move(detector);
	    // 				      detector = 0;
	    // 				      self.P4RunManager::SetUserInitialization(det);
	    // 				  })
	    .def("SetUserInitialization", [](P4RunManager& self, G4VUserDetectorConstruction* detector){ self.P4RunManager::SetUserInitialization(detector);})
	    .def("SetUserInitialization", [](P4RunManager& self, G4VUserPhysicsList* physics){ self.P4RunManager::SetUserInitialization(physics);})
	    .def("SetUserInitialization", [](P4RunManager& self, G4VUserActionInitialization* action){ self.P4RunManager::SetUserInitialization(action);})
	    .def("UseReferencePhysicsList", &P4RunManager::UseReferencePhysicsList, pybind11::arg("name")="FTFP_BERT")
	    .def("UseGenericPhysicsList", &P4RunManager::UseGenericPhysicsList);
	sim4py::define_common_method(p4run_manager);
	// sim4py::define_as_singleton(p4run_manager);

	// pybind11::class_<G4VisExecutive>(sub, "G4VisExecutive")
	//     .def_static("Instance", [](){
	// 				static G4VisExecutive * instance;
	// 				if ( !instance ) instance = new G4VisExecutive;
	// 				return instance;
	// 			    } )
	//     .def("Initialize", &G4VisExecutive::Initialize);
	
	// pybind11::class_<G4UIExecutive>(sub, "G4UIExecutive")
	//     .def_static("Instance", [](int argc, const std::vector<std::string>& argv){
	// 				std::vector<char*> cstrings;
	// 				cstrings.reserve( argv.size() );
	// 				for ( auto itr : argv )
	// 				    cstrings.emplace_back( const_cast<char*>( itr.c_str() ) );				     
	// 				return new G4UIExecutive(argc, &cstrings[0]);
	// 			    } )
	//     .def("SessionStart", &G4UIExecutive::SessionStart);

	// pybind11::class_<G4UImanager>(sub, "G4UImanager")
	//     .def_static("GetUIpointer", &G4UImanager::GetUIpointer)
	//     .def("ApplyCommand", [](G4UImanager& self, std::string command){ self.ApplyCommand( command.c_str() ); } );
	
	define_physics(sub);
	define_action(sub);
	define_analysis(sub);
	define_detector(sub);
    }
    
    void define_physics(pybind11::module& main)
    {	
	pybind11::module sub = main.def_submodule("physics","");

	/* Virtual Classes */
	pybind11::class_<G4VUserPhysicsList>
	    (sub, "G4VUserPhysicsList", pybind11::module_local());
	
	pybind11::class_<G4VModularPhysicsList, G4VUserPhysicsList>
	    (sub, "G4VModularPhysicsList", pybind11::module_local());

	pybind11::class_<G4VPhysicsConstructor>
	    (sub, "G4VPhysicsConstructor", pybind11::module_local());

	/* END OF Virtual Classes */
	
	auto factory = pybind11::class_<G4PhysListFactory>
	    (sub, "G4PhysListFactory", pybind11::module_local())
	    .def("GetReferencePhysList", [](G4PhysListFactory& self, std::string name){ return self.G4PhysListFactory::GetReferencePhysList( name.c_str() ); },
		 pybind11::arg("name"));
	sim4py::define_as_singleton(factory);

	auto ftfp_bert = pybind11::class_<FTFP_BERT, G4VModularPhysicsList>
	    (sub, "FTFP_BERT", pybind11::module_local())
	    .def("ReplacePhysics", &FTFP_BERT::ReplacePhysics)
	    .def("SetVerboseLevel", &FTFP_BERT::SetVerboseLevel);
	sim4py::define_as_singleton(ftfp_bert);

	auto emstan = pybind11::class_<G4EmStandardPhysics, G4VPhysicsConstructor>
	    (sub, "G4EmStandardPhysics", pybind11::module_local());
	sim4py::define_as_singleton(emstan);

	auto emliv = pybind11::class_<G4EmLivermorePhysics, G4VPhysicsConstructor>
	    (sub, "G4EmLivermorePhysics", pybind11::module_local());
	sim4py::define_as_singleton(emliv);

	auto empen = pybind11::class_<G4EmPenelopePhysics, G4VPhysicsConstructor>
	    (sub, "G4EmPenelopePhysics", pybind11::module_local());
	sim4py::define_as_singleton(empen);

	auto emlow = pybind11::class_<G4EmLowEPPhysics, G4VPhysicsConstructor>
	    (sub, "G4EmLowEPPhysics", pybind11::module_local());
	sim4py::define_as_singleton(emlow);
    }
    
    void define_action(pybind11::module& main)
    {
	using pybind11::detail::overload_cast_impl;
	
	pybind11::module sub = main.def_submodule("action","");

	/* Base Classes */
	pybind11::class_<G4UserRunAction>
	    (sub, "G4UserRunAction", pybind11::module_local());
	pybind11::class_<G4UserEventAction>
	    (sub, "G4UserEventAction", pybind11::module_local());
	pybind11::class_<G4UserStackingAction>
	    (sub, "G4UserStackingAction", pybind11::module_local());
	pybind11::class_<G4UserTrackingAction>
	    (sub, "G4UserTrackingAction", pybind11::module_local());
	pybind11::class_<G4UserSteppingAction>
	    (sub, "G4UserSteppingAction", pybind11::module_local());
	/* */

	/* Virtual Classes */
	pybind11::class_<G4VUserActionInitialization>(sub, "G4VUserActionInitialization", pybind11::module_local());
	pybind11::class_<G4VUserPrimaryGeneratorAction>
	    (sub, "G4VUserPrimaryGeneratorAction", pybind11::module_local());
	pybind11::class_<G4VPrimaryGenerator>
	    (sub, "G4VPrimaryGenerator", pybind11::module_local());
	/* */

	pybind11::class_<G4ParticleGun>(sub, "G4ParticleGun", pybind11::module_local());
	pybind11::class_<G4GeneralParticleSource>(sub, "G4GeneralParticleSource", pybind11::module_local());
	
	using sim4py::ParameterGene;
	// pybind11::class_<ParameterGene>(sub, "ParameterGene");
	
	auto act_init = pybind11::class_<P4ActionInitialization, ParameterGene, G4VUserActionInitialization>
	    (sub, "P4ActionInitialization", pybind11::module_local())
	    .def("RegisterUserAction", [](P4ActionInitialization& self, G4VUserPrimaryGeneratorAction*action)
				       { self.P4ActionInitialization::RegisterUserAction(action);} )
	    .def("RegisterUserAction", [](P4ActionInitialization& self, G4UserRunAction*action){ self.P4ActionInitialization::RegisterUserAction(action);} )
	    .def("RegisterUserAction", [](P4ActionInitialization& self, G4UserEventAction*action){ self.P4ActionInitialization::RegisterUserAction(action);} )
	    .def("RegisterUserAction", [](P4ActionInitialization& self, G4UserStackingAction*action){ self.P4ActionInitialization::RegisterUserAction(action);} )
	    .def("RegisterUserAction", [](P4ActionInitialization& self, G4UserTrackingAction*action){ self.P4ActionInitialization::RegisterUserAction(action);} )
	    .def("RegisterUserAction", [](P4ActionInitialization& self, G4UserSteppingAction*action){ self.P4ActionInitialization::RegisterUserAction(action);} );
	    
	    // .def("RegisterUserAction", overload_cast_impl<G4VUserPrimaryGeneratorAction*>()(&P4ActionInitialization::RegisterUserAction))
	    // .def("RegisterUserAction", overload_cast_impl<G4UserRunAction*>()(&P4ActionInitialization::RegisterUserAction))
	    // .def("RegisterUserAction", overload_cast_impl<G4UserEventAction*>()(&P4ActionInitialization::RegisterUserAction))
	    // .def("RegisterUserAction", overload_cast_impl<G4UserStackingAction*>()(&P4ActionInitialization::RegisterUserAction))
	    // .def("RegisterUserAction", overload_cast_impl<G4UserTrackingAction*>()(&P4ActionInitialization::RegisterUserAction))
	    // .def("RegisterUserAction", overload_cast_impl<G4UserSteppingAction*>()(&P4ActionInitialization::RegisterUserAction));
	//sim4py::define_common_method<P4ActionInitialization,G4VUserActionInitialization>(act_init);
	sim4py::define_common_method(act_init);
	sim4py::define_as_singleton(act_init);
	
	auto pg_action = pybind11::class_<P4PrimaryGeneratorAction, ParameterGene, G4VUserPrimaryGeneratorAction>
	    (sub, "P4PrimaryGeneratorAction", pybind11::module_local())
	    .def("SetPrimaryGenerator", &P4PrimaryGeneratorAction::SetPrimaryGenerator);
	//sim4py::define_common_method<P4PrimaryGeneratorAction,G4VUserPrimaryGeneratorAction>(pg_action);
	sim4py::define_common_method(pg_action);
	sim4py::define_as_singleton(pg_action);
	
	auto gps = pybind11::class_<P4GeneralParticleSource, ParameterGene, G4GeneralParticleSource, G4VPrimaryGenerator>
	    (sub, "P4GeneralParticleSource", pybind11::module_local());
	sim4py::define_common_method(gps);
	sim4py::define_as_singleton(gps);
	//sim4py::define_common_method<P4GeneralParticleSource,G4VPrimaryGenerator>(gps);

	auto gun = pybind11::class_<P4ParticleGun, ParameterGene, G4ParticleGun, G4VPrimaryGenerator>
	    (sub, "P4ParticleGun", pybind11::module_local());
	sim4py::define_common_method(gun);
	sim4py::define_as_singleton(gun);	
	
    }

    void define_analysis(pybind11::module& main)
    {
	pybind11::module sub = main.def_submodule("action","");
	auto ana = pybind11::class_<P4RootAnalysisManager>
	    (sub, "P4RootAnalysisManager", pybind11::module_local())
	    .def_static("Instance", &P4RootAnalysisManager::Instance);
	sim4py::define_common_method<P4RootAnalysisManager>(ana);
    }

    void define_detector(pybind11::module& main)
    {
	pybind11::module sub = main.def_submodule("detector","");
	auto det = pybind11::class_<P4DetectorConstruction>
	    (sub, "P4DetectorConstruction", pybind11::module_local());
	sim4py::define_as_singleton(det);
    }
    
}
