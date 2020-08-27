/**
   @date 2020/07/30 
**/
#ifndef P4PVConstruct_hpp
#define P4PVConstruct_hpp

#include <ParameterGene.hpp>

// #include "globals.hh"
// #include "G4SystemOfUnits.hh"
// #include "G4ThreeVector.hh"

class G4LogicalVolume;
class G4VPhysicalVolume;

class P4PVConstruct
    : public sim4py::ParameterGene
{
    
public:
    
    P4PVConstruct(const std::string& name);
    virtual ~P4PVConstruct() {};    
    virtual void Construct(G4LogicalVolume*) = 0;    
    
};        

#endif

