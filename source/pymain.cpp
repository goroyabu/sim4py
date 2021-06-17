/**
   @date 2020/06/22
**/

#include <pybind11/pybind11.h>

/** C++ Standard Library **/
#include <iostream>
#include <chrono>
#include <iomanip>
#include <sstream>

/** Geant4 Header **/
#include <G4RunManager.hh>
#include <G4UIExecutive.hh>
#include <G4UImanager.hh>
#include <G4VisExecutive.hh>
#include <Randomize.hh>

/** User's customized files **/
#include "DetectorConstruction.hpp"
#include "PhysicsList.hpp"
#include "ActionInitialization.hpp"
#include "UImessenger.hpp"

using std::cout;
using std::endl;


#include <pybind11/stl.h>
// #include <pybind11/operators.h>

// #include <clhep-py.hpp>
// #include <general-py.hpp>
// #include <wstrip-py.hpp>

//#include <VANLmodule.hpp>
// #include <ParameterGene.hpp>

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

std::vector<std::string> split(const std::string& str)
{
    std::vector<std::string> v;
    std::stringstream ss(str);
    std::string buf;
    while ( std::getline(ss, buf) )
        if ( buf!="" ) v.push_back(buf);
    return v;
}

// int maincxx(int argc, char** argv)
int maincxx(
    const std::string& macro, 
    const std::vector<std::string>& command_list, 
    const bool use_gui
    )
    // const std::vector<std::string>& args)
    // , 
    // const std::vector<std::string>& command_list)
{
    // int argc = 0;
    // char * argv[] = ;

    // int argc = args.size() + 1; 
    // char ** argv = new char*[ argc ];
    
    // auto progname = std::string("simtestcxx");
    // argv[0] = new char[ progname.size() ];
    // strcpy( argv[0], progname.c_str() );

    // for ( auto i=0; i<(int)args.size(); ++i ){
    //     argv[i+1] = new char[ args[i].size() ];
    //     strcpy( argv[i+1], args[i].c_str() );
    // }

    /* Setting of Random */
    G4Random::setTheEngine( new CLHEP::RanecuEngine );
    G4Random::setTheSeed  ( time(NULL) );
    /* Random */

    /* Setting of Run Manager */
    auto run_manager  = new G4RunManager;
    
    auto detector = new DetectorConstruction;
    run_manager->SetUserInitialization( detector );
    
    auto physics  = new PhysicsList;
    run_manager->SetUserInitialization( physics  );

    auto action   = new ActionInitialization;       
    run_manager->SetUserInitialization( action   );

    // run_manager->Initialize();
    /* Run Manager */

    
    /* Setting of UI Messenger */
    auto ui_messenger = new UImessenger;

    ui_messenger->SetDetectorConstruction( detector );

    auto primary_generator = action->GetPrimaryGeneratorAction();
    ui_messenger->SetPrimaryGeneratorAction( primary_generator );

    auto event_action = action->GetEventAction();
    ui_messenger->SetEventAction( event_action );
    
    ui_messenger->SetPhysicsList( physics );
   /* UI Messenger */
    
    auto progname = std::string("simtestcxx");
    auto c_argc = 1;
    if ( macro!="" ) c_argc = 2;
    
    auto c_argv = new char*[ c_argc ];
    c_argv[0] = new char[ progname.size() ];
    strcpy( c_argv[0], progname.c_str() );
    if ( c_argc == 2 ) {
        c_argv[1] = new char[ macro.size() ];
        strcpy( c_argv[1], macro.c_str() );
    }

    auto ui_executive = new G4UIExecutive(c_argc,c_argv);
    auto ui_manager   = G4UImanager::GetUIpointer();
    
    auto vis_executive = new G4VisExecutive;
    vis_executive->Initialize();
    
    // // if ( argc<1 ) {
    // if ( argc<=1 ) {
    //     ui_executive->SessionStart();
    // }
    // else {
	//     G4String command = "/control/execute ";
    // 	G4String fileName = argv[1];
    //     // G4String fileName = argv[0];
    // 	ui_manager->ApplyCommand(command+fileName);
    //     ui_executive->SessionStart();
    // }

    if ( macro!="" ) {
        G4String command = "/control/execute ";
    	G4String fileName = macro.c_str();
    	ui_manager->ApplyCommand(command+fileName);
    }
    for ( auto command_string : command_list ) {
        auto command_1lines = split( command_string );
        for ( auto command : command_1lines ) {
            ui_manager->ApplyCommand( command );
        }
    }

    if ( use_gui == true )
        ui_executive->SessionStart();

    delete run_manager;
    delete ui_executive;
    delete vis_executive;
    delete ui_messenger;
    
    return 0;    
}

PYBIND11_MODULE(sim4py, m) {
    
    m.attr("__name__") = "sim4py";
    m.doc() = R"pbdoc(
        Description of sim4py
    )pbdoc";

    m.def("maincxx", &maincxx, "A main function of C++ program",
        pybind11::arg("macro")=std::string(""),
        pybind11::arg("command_list")=std::vector<std::string>({}),
        pybind11::arg("use_gui")=false
        );

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
    
    
#ifdef VERSION_INFO
    m.attr("__version__") = VERSION_INFO;
#else
    m.attr("__version__") = "dev";
#endif
}
