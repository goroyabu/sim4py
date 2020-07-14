/**
   @file    W4DetectorConstruction.hpp
   @author  Goro Yabu
   @date    2019/12/22
**/

#ifndef W4DetectorConstruction_hpp
#define W4DetectorConstruction_hpp

#include <ParameterGene.hpp>

#include <map>
#include <vector>

#include <G4VUserDetectorConstruction.hh>
#include <G4ThreeVector.hh>
#include <globals.hh>

class W4DetectorConstruction
    : public sim4py::ParameterGene, public G4VUserDetectorConstruction
{

public:
    W4DetectorConstruction();
    virtual ~W4DetectorConstruction();
    
    virtual G4VPhysicalVolume* Construct() override;
    static W4DetectorConstruction* Instance();

    int SetNumberOf(const G4String& key, G4int number);
    int SetLengthOf(const G4String& key, G4double value);
    int SetPositionOf(const G4String& key, const G4ThreeVector& vector);
    int SetNameOf(const G4String& key, const G4String& name);

    // int AddDetectorLayers(const G4String& material, G4double thickness);
    int AddDetectorLayer(const std::string& material, double thickness);
    
private:

    static W4DetectorConstruction * instance;
    std::map<G4String, G4String>       map_key2str;
    std::map<G4String, G4double>       map_key2val;
    std::map<G4String, G4ThreeVector>  map_key2vec;
    std::map<G4String, G4int>          map_key2int;
    std::vector<G4String>              vec_detmate;
    std::vector<G4double>              vec_detthic;
    
};
#endif

