/**
   @date 2020/06/22
**/

#ifndef UnitDefinition_hpp
#define UnitDefinition_hpp

#include <string>
#include <iostream>

#include <CLHEP/Units/SystemOfUnits.h>

namespace sim4py
{

    class UnitDefinition
    {
	
    private:
	
	std::string name;
	std::string symbol;
	std::string category;
	double      value;
	
    public:
	
	UnitDefinition()
	    : name("None"), symbol(""), category("None"), value(0.0)
	{}
	
	UnitDefinition
	(const std::string& name, const std::string& symbol, const std::string category, double value)
	    : name(name), symbol(symbol), category(category), value(value)
	{}

	UnitDefinition operator=(const UnitDefinition& other)
	{
	    if ( this == &other ) return *this;
	    if ( this->category != other.category ) {
		std::cerr << "*Warning* in UnitDefinition : Assignment of units with different categories is not allowed." << std::endl;
		return *this;
	    }	    
	    this->name = other.name;
	    this->symbol = other.symbol;
	    this->value = other.value;
	    return *this;
	}
	
	bool operator==(const UnitDefinition& other) const
	{
	    return this->name == other.name && this->symbol == other.symbol
		&& this->value == other.value && this->category == other.category;
	}
	
	bool operator!=(const UnitDefinition& other) const
	{
	    return this->name != other.name || this->symbol != other.symbol
		|| this->value != other.value || this->category != other.category;
	}
	
	double operator()() const { return this->Value(); }
	double operator*(double a) const { return this->Value()*a; }
	friend double operator*(double a, const UnitDefinition& u) { return a*u.Value(); }
	
	UnitDefinition operator*(const UnitDefinition& other) const 
	{
	    auto newnam = (std::string)this->Name() + "*" + other.Name();
	    auto newval = this->Value() * other.Value();
	    auto newsym = (std::string)this->Symbol() + "*" + other.Symbol();
	    auto newcat = (std::string)this->Category() + "*" + other.Category();
	    return UnitDefinition(newnam, newsym, newcat, newval);
	}
	
	UnitDefinition operator/(const UnitDefinition& other) const
	{
	    auto newnam = (std::string)this->Name() + "/" + other.Name();
	    auto newval = this->Value() / other.Value();
	    auto newsym = (std::string)this->Symbol() + "/" + other.Symbol();
	    auto newcat = (std::string)this->Category() + "/" + other.Category();
	    return UnitDefinition(newnam, newsym, newcat, newval);
	}
	
	std::ostream& operator<< (std::ostream& os) const 
	{
	    os << this->Symbol(); return os;
	}
	
	std::string Name()     const { return name;     }
	std::string Symbol()   const { return symbol;   }
	std::string Category() const { return category; }
	double      Value()    const { return value;    }
	
    };

    // template<class Char, class Traits>
    // std::basic_ostream<Char, Traits>& operator<<(std::basic_ostream<char, Traits>& os, const UnitDefinition& u)  
    // {
    // 	os << u.Symbol(); return os;
    // }    
    inline std::ostream& operator<<(std::ostream& os, const UnitDefinition& u)
    {
	os << u.Symbol(); return os;
    }
    
    // static UnitDefinition test("test","ts","Weight",0.1);

    static const UnitDefinition pc        ( "parsec",              "pc",     "Length",           CLHEP::parsec );              
    static const UnitDefinition km        ( "kilometer",           "km",     "Length",           CLHEP::kilometer );           
    static const UnitDefinition m         ( "meter",               "m",      "Length",           CLHEP::meter );               
    static const UnitDefinition cm        ( "centimeter",          "cm",     "Length",           CLHEP::centimeter );          
    static const UnitDefinition mm        ( "millimeter",          "mm",     "Length",           CLHEP::millimeter );          
    static const UnitDefinition um        ( "micrometer",          "um",     "Length",           CLHEP::micrometer );          
    static const UnitDefinition nm        ( "nanometer",           "nm",     "Length",           CLHEP::nanometer );           
    static const UnitDefinition Ang       ( "angstrom",            "Ang",    "Length",           CLHEP::angstrom );            
    static const UnitDefinition fm        ( "fermi",               "fm",     "Length",           CLHEP::fermi );               
    static const UnitDefinition rad       ( "radian",              "rad",    "Angle",            CLHEP::radian );              
    static const UnitDefinition mrad      ( "milliradian",         "mrad",   "Angle",            CLHEP::milliradian );         
    static const UnitDefinition deg       ( "degree",              "deg",    "Angle",            CLHEP::degree );
    static const UnitDefinition ns        ( "nanosecond",          "ns",     "Time",             CLHEP::nanosecond );
    static const UnitDefinition s         ( "second",              "s",      "Time",             CLHEP::second );
    static const UnitDefinition ms        ( "millisecond",         "ms",     "Time",             CLHEP::millisecond );
    static const UnitDefinition us        ( "microsecond",         "us",     "Time",             CLHEP::microsecond );
    static const UnitDefinition ps        ( "picosecond",          "ps",     "Time",             CLHEP::picosecond );
    static const UnitDefinition eV        ( "electronvolt",        "eV",     "Energy",           CLHEP::electronvolt );        
    static const UnitDefinition keV       ( "kiloelectronvolt",    "keV",    "Energy",           CLHEP::kiloelectronvolt );    
    static const UnitDefinition MeV       ( "megaelectronvolt",    "MeV",    "Energy",           CLHEP::megaelectronvolt );    
    static const UnitDefinition GeV       ( "gigaelectronvolt",    "GeV",    "Energy",           CLHEP::gigaelectronvolt );    
    static const UnitDefinition TeV       ( "teraelectronvolt",    "TeV",    "Energy",           CLHEP::teraelectronvolt );    
    static const UnitDefinition PeV       ( "petaelectronvolt",    "PeV",    "Energy",           CLHEP::petaelectronvolt );    
    static const UnitDefinition J         ( "joule",               "J",      "Energy",           CLHEP::joule );               
    static const UnitDefinition mg        ( "milligram",           "mg",     "Mass",             CLHEP::milligram );           
    static const UnitDefinition g         ( "gram",                "g",      "Mass",             CLHEP::gram );                
    static const UnitDefinition kg        ( "kilogram",            "kg",     "Mass",             CLHEP::kilogram );            
    static const UnitDefinition Pa        ( "pascal",              "Pa",     "Pressure",         CLHEP::pascal );              
    static const UnitDefinition bar       ( "bar",                 "bar",    "Pressure",         CLHEP::bar );                 
    static const UnitDefinition atm       ( "atmosphere",          "atm",    "Pressure",         CLHEP::atmosphere );          
    static const UnitDefinition Bq        ( "becquerel",           "Bq",     "Activity",         CLHEP::becquerel );           
    static const UnitDefinition Ci        ( "curie",               "Ci",     "Activity",         CLHEP::curie );   
    
}

#endif
