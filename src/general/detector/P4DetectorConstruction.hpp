/**
   @file    P4DetectorConstruction.hpp
   @author  Goro Yabu
   @date    2019/12/22
**/

#ifndef P4DetectorConstruction_hpp
#define P4DetectorConstruction_hpp

#include <ParameterGene.hpp>
#include <P4PVConstruct.hpp>

#include <map>
#include <vector>

#include <G4VUserDetectorConstruction.hh>
#include <G4ThreeVector.hh>
#include <globals.hh>

class P4DetectorConstruction
    : public sim4py::ParameterGene, public G4VUserDetectorConstruction
{
    
public:
    
    P4DetectorConstruction();
    ~P4DetectorConstruction();
    
    virtual G4VPhysicalVolume* Construct() override;

    P4DetectorConstruction* AddVolume(P4PVConstruct* volume);

private:

    std::vector<P4PVConstruct*> vector_of_pvconstructs;
    
};
#endif

