/**
   @date 2020/06/22
**/

#include "wstrip-py.hpp"

#include <iostream>

#include <pybind11/stl.h>
#include <pybind11/operators.h>

// #include "W4ActionInitialization.hpp"

#include "P4ActionInitialization.hpp"
#include "W4RunAction.hpp"
#include "W4EventAction.hpp"
#include "W4SteppingAction.hpp"
#include "W4DetectorConstruction.hpp"
#include "W4EmStandardPhysicsSwitchingDoppler.hpp"
#include "GeometryCollimatorMINE1.hpp"

#include "W4DoublesidedStripDetector.hpp"
#include "W4AirContainBox.hpp"

namespace wstrip_py
{    
    void define_wstrip(pybind11::module& main)
    {
	using std::cerr;
	using std::endl;
	
	pybind11::module sub = main.def_submodule("wstrip","");	
	
	pybind11::class_<G4VUserActionInitialization>(sub, "G4VUserActionInitialization");
	pybind11::class_<G4UserRunAction>(sub, "G4UserRunAction");	
	pybind11::class_<G4UserEventAction>(sub, "G4UserEventAction");
	pybind11::class_<G4UserSteppingAction>(sub, "G4UserSteppingAction");
	pybind11::class_<G4VUserDetectorConstruction>(sub, "G4VUserDetectorConstruction");

	using sim4py::ParameterGene;
	pybind11::class_<ParameterGene>(sub, "ParameterGene");

	auto geom = pybind11::class_<P4GeometryConstruction, ParameterGene>( sub, "P4GeometryConstruction", pybind11::module_local() );
	pybind11::class_<P4PVConstruct, ParameterGene>( sub, "P4PVConstruct", pybind11::module_local() );
	// sim4py::define_common_method(geom);	
	
	// pybind11::class_<P4ActionInitialization,G4VUserActionInitialization>(sub, "P4ActionInitialization");
	
	// auto init = pybind11::class_<W4ActionInitialization,P4ActionInitialization>(sub, "W4ActionInitialization", pybind11::module_local());
	// sim4py::define_common_method(init);
	// sim4py::define_as_singleton(init);
	
	auto run = pybind11::class_<W4RunAction, ParameterGene, G4UserRunAction>(sub, "W4RunAction", pybind11::module_local());
	sim4py::define_common_method(run);
	sim4py::define_as_singleton(run);

	auto event = pybind11::class_<W4EventAction, ParameterGene, G4UserEventAction>(sub, "W4EventAction", pybind11::module_local());
	sim4py::define_common_method(event);
	sim4py::define_as_singleton(event);
	
	auto stepping = pybind11::class_<W4SteppingAction, ParameterGene, G4UserSteppingAction>(sub, "W4SteppingAction", pybind11::module_local());
	sim4py::define_common_method(stepping);
	sim4py::define_as_singleton(stepping);
	
	auto d = pybind11::class_<W4DetectorConstruction, ParameterGene, G4VUserDetectorConstruction>
	    (sub, "W4DetectorConstruction", pybind11::module_local())
	    .def("AddDetectorLayer", &W4DetectorConstruction::AddDetectorLayer)
	    .def("AddGeometry", &W4DetectorConstruction::AddGeometry);
	sim4py::define_common_method(d);
	//d.def(pybind11::init<>());
	sim4py::define_as_singleton(d);

	pybind11::class_<mineapp::GeometryCollimator>(sub, "GeometryCollimator", pybind11::module_local());
	auto col = pybind11::class_<mineapp::GeometryCollimatorMINE1, mineapp::GeometryCollimator, P4GeometryConstruction, ParameterGene>
	    (sub, "W4ParallelCollimator", pybind11::module_local());	    
	sim4py::define_common_method(col);
	sim4py::define_as_singleton(col);

	auto dsd = pybind11::class_<W4DoublesidedStripDetector, P4PVConstruct, ParameterGene>
	    ( sub, "W4DoublesidedStripDetector", pybind11::module_local() )
	    .def("AddDetectorLayer", pybind11::overload_cast<const std::string&, double>(&W4DoublesidedStripDetector::AddDetectorLayer) )
	    .def("SetCurrentLayerSize", &W4DoublesidedStripDetector::SetCurrentLayerSize)
	    .def("SetCurrentLayerPixels", &W4DoublesidedStripDetector::SetCurrentLayerPixels)
	    .def("SetCurrentLayerPosition", &W4DoublesidedStripDetector::SetCurrentLayerPosition)
	    .def("SetCurrentLayerRotation", &W4DoublesidedStripDetector::SetCurrentLayerRotation);
	    //.def("SetCurrentLayerNormal", &W4DoublesidedStripDetector::SetCurrentLayerNormal);
	sim4py::define_common_method( dsd );
	sim4py::define_as_singleton( dsd );

	auto air_box = pybind11::class_<W4AirContainBox, P4PVConstruct, ParameterGene>
	    ( sub, "W4AirContainBox", pybind11::module_local() );
	sim4py::define_common_method( air_box );
	sim4py::define_as_singleton( air_box );
	
	// .def(pybind11::init( [](){ return nullptr; }))
	// .def_static("Instance", &W4RunAction::Instance, pybind11::return_value_policy::reference);	
	// auto d = pybind11::class_<W4DetectorConstruction, G4VUserDetectorConstruction>
	//     (sub, "W4DetectorConstruction")
	//     .def(pybind11::init<>(), pybind11::return_value_policy::reference_internal)
	//     .def_static("Instance", &W4DetectorConstruction::Instance, pybind11::return_value_policy::reference);
	// .def_static("Instance", &W4DetectorConstruction::Instance, pybind11::return_value_policy::reference)	    
	// .def( pybind11::init([](){ return nullptr; } ) );
	//.def( pybind11::init(&W4DetectorConstruction::Instance), pybind11::return_value_policy::reference);
	//.def( pybind11::init([](){ return W4DetectorConstruction::Instance(); } ), pybind11::return_value_policy::reference )
	
    }
}
