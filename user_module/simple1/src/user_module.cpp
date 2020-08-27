/**
   @date 2020/08/06
**/

#include <pybind11/pybind11.h>
#include <ParameterGene.hpp>

namespace user
{
    class SampleClass
	: public sim4py::ParameterGene
    {
	
    public:
	SampleClass()
	    : sim4py::ParameterGene("SampleClass")
	{
	    DefineParameter<std::string>("title_of_examination", "build_test");
	}	
    };   
}

PYBIND11_MODULE(user_module, module) {

    module.attr("__name__") = "user_module";
    module.doc() = R"pbdoc(Example of User's module for sim4py)pbdoc";    

    pybind11::class_<sim4py::ParameterGene>( module, "ParameterGene" );
    auto sample = pybind11::class_<user::SampleClass, sim4py::ParameterGene>( module, "SampleClass" )
	.def(pybind11::init<>());
    sim4py::define_common_method(sample);    
    
#ifdef VERSION_INFO
    module.attr("__version__") = VERSION_INFO;
#else
    module.attr("__version__") = "dev";
#endif
    
}
