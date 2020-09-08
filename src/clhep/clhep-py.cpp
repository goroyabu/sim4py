/**
   @date 2020/06/22
**/

#include "clhep-py.hpp"
#include <UnitDefinition.hpp>
// #include "ConstantDefinition.hpp"

#include <pybind11/stl.h>
#include <pybind11/operators.h>

#include <CLHEP/Random/Random.h>
#include <CLHEP/Random/RanecuEngine.h>
#include <CLHEP/Units/SystemOfUnits.h>

namespace clhep_py
{

    void define_clhep(pybind11::module& main)
    {    
	pybind11::module sub = main.def_submodule("CLHEP","");
	
	/* Random */
	pybind11::class_<CLHEP::HepRandomEngine>(sub, "HepRandomEngine");
	
	pybind11::class_<CLHEP::RanecuEngine, CLHEP::HepRandomEngine>(sub, "RanecuEngine")
	    .def(pybind11::init<>());
	
	pybind11::class_<CLHEP::HepRandom>(sub, "HepRandom")
	    .def_static("setTheSeed", &CLHEP::HepRandom::setTheSeed, pybind11::arg("seed"), pybind11::arg("lux")=3)
	    .def_static("setTheEngine", &CLHEP::HepRandom::setTheEngine);

	/* Unit */
	using namespace sim4py;
	
	pybind11::class_<UnitDefinition>(sub, "UnitDefinition")
	    .def("name", &UnitDefinition::Name)
	    .def("symbol", &UnitDefinition::Symbol)
	    .def("value", &UnitDefinition::Value)
	    .def("category", &UnitDefinition::Category)
	    .def(pybind11::self == pybind11::self)
	    .def(pybind11::self * float())
	    .def(float() * pybind11::self)
	    .def(pybind11::self * pybind11::self)
	    .def(pybind11::self / pybind11::self);

	define_units(sub);
	
    }


    void define_units(pybind11::module& sub)
    {
	


	using namespace sim4py;
	
	// static auto um = new UnitDefinition("micrometer","um"      ,"Length",CLHEP::micrometer);
	// static auto keV = new UnitDefinition("kiloelectronvolt","keV","Energy",CLHEP::kiloelectronvolt);
	// sub.attr("um") = pybind11::cast(um);
	// sub.attr("keV") = pybind11::cast(keV);

	sub.attr("pc")       = pybind11::cast( pc );     
	sub.attr("km")       = pybind11::cast( km );     
	sub.attr("m")        = pybind11::cast( m );      
	sub.attr("cm")       = pybind11::cast( cm );     
	sub.attr("mm")       = pybind11::cast( mm );     
	sub.attr("um")       = pybind11::cast( um );     
	sub.attr("nm")       = pybind11::cast( nm );     
	sub.attr("Ang")      = pybind11::cast( Ang );    
	sub.attr("fm")       = pybind11::cast( fm );     
	sub.attr("rad")      = pybind11::cast( rad );    
	sub.attr("mrad")     = pybind11::cast( mrad );   
	sub.attr("deg")      = pybind11::cast( deg );
	sub.attr("ns")       = pybind11::cast( ns );
	sub.attr("s")       = pybind11::cast( s );
	sub.attr("ms")       = pybind11::cast( ms );
	sub.attr("us")       = pybind11::cast( us );
	sub.attr("ps")       = pybind11::cast( ps );    
	sub.attr("eV")       = pybind11::cast( eV );     
	sub.attr("keV")      = pybind11::cast( keV );    
	sub.attr("MeV")      = pybind11::cast( MeV );    
	sub.attr("GeV")      = pybind11::cast( GeV );    
	sub.attr("TeV")      = pybind11::cast( TeV );    
	sub.attr("PeV")      = pybind11::cast( PeV );    
	sub.attr("J")        = pybind11::cast( J );      
	sub.attr("mg")       = pybind11::cast( mg );     
	sub.attr("g")        = pybind11::cast( g );      
	sub.attr("kg")       = pybind11::cast( kg );     
	sub.attr("Pa")       = pybind11::cast( Pa );     
	sub.attr("bar")      = pybind11::cast( bar );    
	sub.attr("atm")      = pybind11::cast( atm );    
	sub.attr("Bq")       = pybind11::cast( Bq );     
	sub.attr("Ci")       = pybind11::cast( Ci );
	
	// static auto pc       = new UnitDefinition( "parsec",                "pc",     "Length",           CLHEP::parsec );              
	// sub.attr("pc")       = pybind11::cast( pc );     
	
	// static auto km       = new UnitDefinition( "kilometer",             "km",     "Length",           CLHEP::kilometer );           
	// sub.attr("km")       = pybind11::cast( km );     
	
	// static auto m        = new UnitDefinition( "meter",                 "m",      "Length",           CLHEP::meter );               
	// sub.attr("m")        = pybind11::cast( m );      
	
	// static auto cm       = new UnitDefinition( "centimeter",            "cm",     "Length",           CLHEP::centimeter );          
	// sub.attr("cm")       = pybind11::cast( cm );     
	
	// static auto mm       = new UnitDefinition( "millimeter",            "mm",     "Length",           CLHEP::millimeter );          
	// sub.attr("mm")       = pybind11::cast( mm );     
	
	// static auto um       = new UnitDefinition( "micrometer",            "um",     "Length",           CLHEP::micrometer );          
	// sub.attr("um")       = pybind11::cast( um );     
	
	// static auto nm       = new UnitDefinition( "nanometer",             "nm",     "Length",           CLHEP::nanometer );           
	// sub.attr("nm")       = pybind11::cast( nm );     
	
	// static auto Ang      = new UnitDefinition( "angstrom",              "Ang",    "Length",           CLHEP::angstrom );            
	// sub.attr("Ang")      = pybind11::cast( Ang );    
	
	// static auto fm       = new UnitDefinition( "fermi",                 "fm",     "Length",           CLHEP::fermi );               
	// sub.attr("fm")       = pybind11::cast( fm );     
	
	// static auto rad      = new UnitDefinition( "radian",                "rad",    "Angle",            CLHEP::radian );              
	// sub.attr("rad")      = pybind11::cast( rad );    
	
	// static auto mrad     = new UnitDefinition( "milliradian",           "mrad",   "Angle",            CLHEP::milliradian );         
	// sub.attr("mrad")     = pybind11::cast( mrad );   
	
	// static auto deg      = new UnitDefinition( "degree",                "deg",    "Angle",            CLHEP::degree );              
	// sub.attr("deg")      = pybind11::cast( deg );    
	
	// static auto eV       = new UnitDefinition( "electronvolt",          "eV",     "Energy",           CLHEP::electronvolt );        
	// sub.attr("eV")       = pybind11::cast( eV );     
	
	// static auto keV      = new UnitDefinition( "kiloelectronvolt",      "keV",    "Energy",           CLHEP::kiloelectronvolt );    
	// sub.attr("keV")      = pybind11::cast( keV );    
	
	// static auto MeV      = new UnitDefinition( "megaelectronvolt",      "MeV",    "Energy",           CLHEP::megaelectronvolt );    
	// sub.attr("MeV")      = pybind11::cast( MeV );    
	
	// static auto GeV      = new UnitDefinition( "gigaelectronvolt",      "GeV",    "Energy",           CLHEP::gigaelectronvolt );    
	// sub.attr("GeV")      = pybind11::cast( GeV );    
	
	// static auto TeV      = new UnitDefinition( "teraelectronvolt",      "TeV",    "Energy",           CLHEP::teraelectronvolt );    
	// sub.attr("TeV")      = pybind11::cast( TeV );    
	
	// static auto PeV      = new UnitDefinition( "petaelectronvolt",      "PeV",    "Energy",           CLHEP::petaelectronvolt );    
	// sub.attr("PeV")      = pybind11::cast( PeV );    
	
	// static auto J        = new UnitDefinition( "joule",                 "J",      "Energy",           CLHEP::joule );               
	// sub.attr("J")        = pybind11::cast( J );      
	
	// static auto mg       = new UnitDefinition( "milligram",             "mg",     "Mass",             CLHEP::milligram );           
	// sub.attr("mg")       = pybind11::cast( mg );     
	
	// static auto g        = new UnitDefinition( "gram",                  "g",      "Mass",             CLHEP::gram );                
	// sub.attr("g")        = pybind11::cast( g );      
	
	// static auto kg       = new UnitDefinition( "kilogram",              "kg",     "Mass",             CLHEP::kilogram );            
	// sub.attr("kg")       = pybind11::cast( kg );     
	
	// static auto Pa       = new UnitDefinition( "pascal",                "Pa",     "Pressure",         CLHEP::pascal );              
	// sub.attr("Pa")       = pybind11::cast( Pa );     
	
	// static auto bar      = new UnitDefinition( "bar",                   "bar",    "Pressure",         CLHEP::bar );                 
	// sub.attr("bar")      = pybind11::cast( bar );    
	
	// static auto atm      = new UnitDefinition( "atmosphere",            "atm",    "Pressure",         CLHEP::atmosphere );          
	// sub.attr("atm")      = pybind11::cast( atm );    
	
	// static auto Bq       = new UnitDefinition( "becquerel",             "Bq",     "Activity",         CLHEP::becquerel );           
	// sub.attr("Bq")       = pybind11::cast( Bq );     
	
	// static auto Ci       = new UnitDefinition( "curie",                 "Ci",     "Activity",         CLHEP::curie );               
	// sub.attr("Ci")       = pybind11::cast( Ci );     
	
    }
    
}
