/**
   @date 2020/06/22
**/

#ifndef UnitDefinition_hpp
#define UnitDefinition_hpp

#include <string>

#include <G4UnitsTable.hh>
#include <CLHEP/Units/SystemOfUnits.h>

namespace clhep_py
{

    class UnitDefinition
    {
	
    private:
	
	std::string name;
	std::string symbol;
	std::string category;
	double      value;
	
    public:
	
	UnitDefinition
	(const std::string& name, const std::string& symbol, const std::string category, double value)
	    : name(name), symbol(symbol), category(category), value(value)
	{}

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

	
	std::string Name()     const { return name;     }
	std::string Symbol()   const { return symbol;   }
	std::string Category() const { return category; }
	double      Value()    const { return value;    }
	
    };

}

#endif
