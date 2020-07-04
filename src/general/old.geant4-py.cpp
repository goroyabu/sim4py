/**
   @date 2020/06/22
**/

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/operators.h>

#include <geant4-py.hpp>

#include <G4RunManager.hh>
#include <G4VUserDetectorConstruction.hh>
#include <G4VUserPhysicsList.hh>
#include <G4VUserActionInitialization.hh>

#include <G4UIExecutive.hh>

namespace geant4_py
{
    void define_geant4(pybind11::module& main)
    {
	using pybind11::detail::overload_cast_impl;
	
	pybind11::module sub = main.def_submodule("Geant4","Sub-module including native Geant4 Classes.");
	
	pybind11::class_<G4RunManager>(sub, "G4RunManager")
	    .def(pybind11::init<>())
	    .def("beam_on", &G4RunManager::BeamOn, pybind11::arg("n_event"), pybind11::arg("macroFile")='\0', pybind11::arg("n_select")=-1)
	    .def("initialize", &G4RunManager::Initialize)
	    .def("set_verbose_level", &G4RunManager::SetVerboseLevel)
	    .def("get_verbose_level", &G4RunManager::GetVerboseLevel)
	    .def("set_user_initialization", overload_cast_impl<G4VUserDetectorConstruction*>()(&G4RunManager::SetUserInitialization))
	    .def("set_user_initialization", overload_cast_impl<G4VUserPhysicsList*>()(&G4RunManager::SetUserInitialization))
	    .def("set_user_initialization", overload_cast_impl<G4VUserActionInitialization*>()(&G4RunManager::SetUserInitialization));
	
	pybind11::class_<G4UIExecutive>(sub, "G4UIExecutive");
    }
}
