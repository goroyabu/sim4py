/**
   @date 2020/07/30 
**/
#ifndef P4GeometryConstruction_hpp
#define P4GeometryConstruction_hpp

#include <ParameterGene.hpp>

#include "globals.hh"
#include "G4SystemOfUnits.hh"
#include "G4ThreeVector.hh"

class G4LogicalVolume;
class G4VPhysicalVolume;

class P4GeometryConstruction
    : public sim4py::ParameterGene
{
    
public:
    
    P4GeometryConstruction(const std::string& name);
    virtual ~P4GeometryConstruction() {};    
    virtual void Construct(G4LogicalVolume*) = 0;    
    
};        

#endif

