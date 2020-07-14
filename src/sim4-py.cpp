/**
   @date 2020/06/22
**/

#include <pybind11/pybind11.h>
// #include <pybind11/stl.h>
// #include <pybind11/operators.h>

#include <clhep-py.hpp>
#include <general-py.hpp>
#include <wstrip-py.hpp>

//#include <VANLmodule.hpp>
#include <ParameterGene.hpp>

// #include <G4RunManager.hh>
// #include <G4VUserDetectorConstruction.hh>
// #include <G4VUserPhysicsList.hh>
// #include <G4VUserActionInitialization.hh>
// void define_geant4(pybind11::module& main)
// {
//     using pybind11::detail::overload_cast_impl;
    
//     pybind11::module sub = main.def_submodule("Geant4","General Class");
//     pybind11::class_<G4RunManager>(sub, "G4RunManager")
// 	.def(pybind11::init<>())
// 	.def("beam_on", &G4RunManager::BeamOn, pybind11::arg("n_event"), pybind11::arg("macroFile")='\0', pybind11::arg("n_select")=-1)
// 	.def("initialize", &G4RunManager::Initialize)
// 	.def("set_verbose_level", &G4RunManager::SetVerboseLevel)
// 	.def("get_verbose_level", &G4RunManager::GetVerboseLevel)
// 	.def("set_user_initialization", overload_cast_impl<G4VUserDetectorConstruction*>()(&G4RunManager::SetUserInitialization))
// 	.def("set_user_initialization", overload_cast_impl<G4VUserPhysicsList*>()(&G4RunManager::SetUserInitialization))
// 	.def("set_user_initialization", overload_cast_impl<G4VUserActionInitialization*>()(&G4RunManager::SetUserInitialization));
// }

// class test_class
// {
// public:
//     std::string class_name;
//     double number;
//     std::string key;

//     test_class()
// 	: class_name("test_class"), number(777), key("")
//     {}
//     test_class(std::string key)
// 	: class_name("test_class"), number(88.0), key(key)
//     {}
// };

// class test_w_params : public sim4py::ParameterGene<test_class>
// {

// public:
//     test_w_params()
// 	: sim4py::ParameterGene<test_class>(test_class())
//     {
// 	DefineParameter<int>("id", 101);
// 	SetParameter<int>("id", 108);
// 	GetParameter<int>("id");
//     }

//     std::string get_name() { return test_class::class_name; } 
//     double get_number() { return test_class::number; }
//     std::string get_key() { return test_class::key; }
// };

// // template<class... Parents>
// void declare_module(pybind11::module& m, const std::string pyclassname)
// {
//     using mod = anl::ParameterGene<Parents...>;
//     pybind11::class_<mod>(m, pyclassname.c_str())
// 	.def(pybind11::init<>())
// 	.def("set_parameter", &mod::template set_parameter<int>)
// 	.def("get_parameter", &mod::template get_parameter<int>);
// }

// template<class T>
// void declare_common_method(pybind11::class_<T>& c)
// {
//     c.def(pybind11::init<>())
// 	.def("set_parameter", &T::template set_parameter<int>)
// 	.def("get_parameter", &T::template get_parameter<int>)
// 	.def("show_parameters", &T::show_parameters);
// }

PYBIND11_MODULE(sim4py, m) {
    
    m.attr("__name__") = "sim4py";
    m.doc() = R"pbdoc(
    )pbdoc";

    // pybind11::class_<test_class>(m, "test_class");
    // auto c = pybind11::class_<test_w_params>(m, "test_w_params")
    // 	.def("name", &test_w_params::get_name)
    // 	.def("number", &test_w_params::get_number)
    // 	.def("key", &test_w_params::get_key);
    // // sim4py::ParameterGene<test_class>::define_common_method(c);
    // sim4py::define_common_method<test_w_params>(c);
    // //define_common_method<test_w_params>(c);
    
    // //declare_common_method(c);
    // //declare_module<test_class>(m, "test_class_p");
    
    clhep_py::define_clhep(m);

    general_py::define_general(m);
    
    wstrip_py::define_wstrip(m);
    
    
#ifdef VERSION_INFO
    m.attr("__version__") = VERSION_INFO;
#else
    m.attr("__version__") = "dev";
#endif
}
