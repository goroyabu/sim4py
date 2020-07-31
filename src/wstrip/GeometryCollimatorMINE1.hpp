/**
   @date 2020/07/30 adapt for sim4py by Goro Yabu
   @detail This file is inherited from GeometryCollimatorMINE1.hh in mine_app 
**/
#ifndef GeometryCollimatorMINE1_HH
#define GeometryCollimatorMINE1_HH

#include <P4GeometryConstruction.hpp>

#include "globals.hh"
#include "G4SystemOfUnits.hh"
#include "G4ThreeVector.hh"
// #include "GeometryCollimator.hh"

class G4LogicalVolume;
class G4VPhysicalVolume;

namespace mineapp {
    
    class GeometryCollimator : public P4GeometryConstruction
    {
	
    public:
	
	GeometryCollimator(const std::string& name);
	virtual ~GeometryCollimator() {};
	
	virtual void Construct(G4LogicalVolume*) = 0;
	
    private:
	
	void DefineMaterials() {};
	void SetupGeometry() {};
	
    };
    
    class GeometryCollimatorMINE1
	: public GeometryCollimator
    {
	
    public:
	
	GeometryCollimatorMINE1();
	virtual ~GeometryCollimatorMINE1() {};
	
	virtual void Construct(G4LogicalVolume* LocalWorld_Logical) override;
	
    private:
	
	void DefineMaterials() {};
	void SetupGeometry() {};
	
    };
    
}

#endif

