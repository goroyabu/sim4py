/**
   @file    W4SensitiveDetector.hpp
   @author  Goro Yabu
   @date    2020/01/30
**/

#ifndef W4SensitiveDetector_hpp
#define W4SensitiveDetector_hpp

#include <ParameterGene.hpp>

// #include "W4HitsCollection.hpp"

#include <utility>

#include <G4VSensitiveDetector.hh>

// #include <TString.h>

class TObject;
class TH1D;

class W4SensitiveDetector
    : public G4VSensitiveDetector
{
    
public:
    
    W4SensitiveDetector( G4String name );
    ~W4SensitiveDetector();
    
    virtual void Initialize( G4HCofThisEvent* ) override;
    virtual G4bool ProcessHits( G4Step*, G4TouchableHistory* ) override;
    virtual void EndOfEvent( G4HCofThisEvent* ) override;
        
    G4int SetDetectorID( G4int id );
    G4int GetDetectorID();

    G4int SetGridXaxis( G4int nbins, G4double min, G4double max );
    G4int SetGridYaxis( G4int nbins, G4double min, G4double max );
    G4int SetGridZaxis( G4int nbins, G4double min, G4double max );

private:    

    std::pair<G4int, G4int> GetStripID( const G4ThreeVector& pos );
    std::pair<G4double, G4double> GetCenterOfPixel( G4int strip_x, G4int strip_y );
    
    class W4Hit
    {
	
    public:

	W4Hit();
	virtual ~W4Hit();

	W4Hit* SetDetectorID(int id);
	W4Hit* SetStripID(int x, int y);
	W4Hit* SetPosition(const G4ThreeVector& p);
	W4Hit* SetEnergy(double e);
	W4Hit* SetProcessName(const std::string& name);

	inline int DetectorID() { return detector_id; }
	inline std::string ProcessName() { return process_name; }
	inline int StripIDX() { return stripid_x; }
	inline int StripIDY() { return stripid_y; }
	inline double X() { return position.x(); }
	inline double Y() { return position.y(); }
	inline double Z() { return position.z(); }
	inline double Energy() { return energy; }

	bool IsSamePixel(const W4Hit& other);
	bool IsAdjacentPixel(const W4Hit& other);
	void MergeSamePixel(const W4Hit& other);
	void MergeAdjacentPixel(const W4Hit& other);
	

    private:
	
	int detector_id;
	std::string process_name;
	int stripid_x;
	int stripid_y;
	G4ThreeVector position;
	double energy;	       
	
    };

    
private:

    // W4HitsCollection * hits_collection;
    std::vector<W4Hit*> hits_collection;
    
    G4int detector_id;
    G4String grid_xaxis_name;
    G4String grid_yaxis_name;
    G4String grid_zaxis_name;
    TH1D * grid_xaxis;
    TH1D * grid_yaxis;
    TH1D * grid_zaxis;

    G4String GetUniqueObjectName(const G4String& name);
    void DeleteObject(TObject* obj);
    
};

#endif
