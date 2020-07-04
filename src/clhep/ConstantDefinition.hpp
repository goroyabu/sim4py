/**
   @date 2020/06/23
**/

#ifndef ConstantDefinition_hpp
#define ConstantDefinition_hpp

#include "UnitDefinition.hpp"

#include <string>

#include <CLHEP/Units/PhysicalConstants.h>

namespace clhep_py
{

    class ConstantDefinition
    {

    private:
	
	std::string name;
	double      value;
	UnitDefinition unit;
	std::string desc;

    public:

	ConstantDefinition
	(const std::string& name, double value, const UnitDefinition& unit, const std::string& desc="")
	    : name(name), unit(unit), desc(desc)
	{
	    this->value = value * this->unit.Value();
	}

	double operator()() const { return value; }

	std::string Name()  const { return name;          }
	double      Value() const { return value;         }
	std::string Unit()  const { return unit.Symbol(); }
	
    };
}

#endif
