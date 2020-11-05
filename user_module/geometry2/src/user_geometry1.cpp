/**
   @date 2020/08/06
**/

#include <pybind11/pybind11.h>

#include <ParameterGene.hpp>

#include "UserVolume.hpp"
// #include "UserDetectorWithSensitive.hpp"
// #include "UserMultipleVolumes.hpp"

PYBIND11_MODULE(user_geometry1, module) {

    module.attr("__name__") = "user_geometry1";
    module.doc() = R"pbdoc(Example1 of User's geometry for sim4py)pbdoc";    
    
    pybind11::class_<sim4py::ParameterGene>( module, "ParameterGene", pybind11::module_local() );
    
    auto sample = pybind11::class_<UserVolume, P4PVConstruct, sim4py::ParameterGene>( module, "UserVolume" )
    	.def(pybind11::init<>());
    sim4py::define_common_method(sample);
    sim4py::define_as_singleton(sample);
    
#ifdef VERSION_INFO
    module.attr("__version__") = VERSION_INFO;
#else
    module.attr("__version__") = "dev";
#endif
    
}
