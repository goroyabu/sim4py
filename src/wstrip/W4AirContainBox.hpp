/**
   @file    W4AirContainBox.hpp
   @author  Goro Yabu
   @date    2020/09/15
**/

#ifndef W4AirContainBox_hpp
#define W4AirContainBox_hpp

#include <P4PVConstruct.hpp>

#include <vector>

#include <G4VUserDetectorConstruction.hh>
#include <G4ThreeVector.hh>
#include <globals.hh>

class W4AirContainBox
    : public P4PVConstruct
{

public:
    W4AirContainBox();
    virtual ~W4AirContainBox();

    virtual void Construct(G4LogicalVolume*) override;


private:

    // struct parameter_list
    // {
    // 	int verbose_level;
	
    //     std::string box_material;
    //     double box_size;
    // 	double box_thickness;
    //     G4ThreeVector box_center;

    // 	std::string content_material;
    // };
    
};

#endif
