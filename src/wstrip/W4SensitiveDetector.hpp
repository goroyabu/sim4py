/**
   @file    W4SensitiveDetector.hpp
   @author  Goro Yabu
   @date    2020/01/30
**/

#ifndef W4SensitiveDetector_hpp
#define W4SensitiveDetector_hpp

#include "W4HitsCollection.hpp"

#include <utility>
#include <tuple>

#include <G4VSensitiveDetector.hh>
#include <G4ThreeVector.hh>
#include <G4RotationMatrix.hh>

// #include <TString.h>
class TObject;
class TH1D;

#include <ParameterGene.hpp>

class W4SensitiveDetector
    : public sim4py::ParameterGene, public G4VSensitiveDetector
{
    
public:
    
    W4SensitiveDetector( G4String name );
    ~W4SensitiveDetector();
    
    virtual void Initialize( G4HCofThisEvent* ) override;
    virtual G4bool ProcessHits( G4Step*, G4TouchableHistory* ) override;
    virtual void EndOfEvent( G4HCofThisEvent* ) override;
        
    G4int SetDetectorID( G4int id );
    G4int GetDetectorID();

    G4int SetDetectorMaterial( int mate );
    G4int GetDetectorMaterial();

    G4int SetGridXaxis( G4int nbins, G4double min, G4double max );
    G4int SetGridYaxis( G4int nbins, G4double min, G4double max );
    G4int SetGridZaxis( G4int nbins, G4double min, G4double max );
    
    // G4int SetPositionRotation( const G4ThreeVector&, const G4ThreeVector& );
    G4int SetPositionRotation( const G4ThreeVector&, const G4RotationMatrix& );
    
private:    

    std::tuple<G4int, G4int, G4int> GetStripID
    ( const G4ThreeVector& pos );
    std::tuple<G4double, G4double, G4double> GetCenterOfPixel
    ( G4int strip_x, G4int strip_y, G4int strip_z );
    
    // class W4Hit
    // {
	
    // public:

    // 	W4Hit();
    // 	W4Hit(const W4Hit& other);
    // 	virtual ~W4Hit();

    // 	W4Hit& SetDetectorID(int id);
    // 	W4Hit& SetTime(double t);
    // 	W4Hit& SetStripID(int x, int y);
    // 	W4Hit& SetPosition(const G4ThreeVector& p);
    // 	W4Hit& SetDirection(const G4ThreeVector& v);
    // 	W4Hit& SetEnergy(double e);
    // 	W4Hit& SetProcessName(const std::string& name);

    // 	inline int DetectorID() const { return detector_id; }
    // 	inline double Time() const { return global_time; }
    // 	inline std::string ProcessName() const { return process_name; }
    // 	inline int StripIDX() const { return stripid_x; }
    // 	inline int StripIDY() const { return stripid_y; }
    // 	inline double X() const { return position.x(); }
    // 	inline double Y() const { return position.y(); }
    // 	inline double Z() const { return position.z(); }
    // 	inline double DirX() const { return direction.x(); }
    // 	inline double DirY() const { return direction.y(); }
    // 	inline double DirZ() const { return direction.z(); }
    // 	inline double Energy() const { return energy; }
    // 	inline int Nmerged() const { return nmerged_hits; }
    // 	inline std::string MergedProcessName() const { return merged_process_name; }
	
    // 	bool IsSamePixel(const W4Hit& other);
    // 	bool IsAdjacentPixel(const W4Hit& other);
    // 	// void MergeSamePixel(const W4Hit& other);
    // 	// void MergeAdjacentPixel(const W4Hit& other);

    // 	bool ExistSamePixelAmong(const std::vector<W4Hit>& hits);
    // 	bool ExistAdjacentPixelAmong(const std::vector<W4Hit>& hits);

    // 	static W4Hit MergeThem
    // 	(const std::vector<W4Hit>& hits,
    // 	 std::function<W4SensitiveDetector::W4Hit
    // 	 (const W4SensitiveDetector::W4Hit&, const W4SensitiveDetector::W4Hit&)>
    // 	 merging_method);
    // 	static W4Hit MergeOnEdepMax(const W4Hit& hit1, const W4Hit& hit2);
    // 	static W4Hit MergeOnFirstHit(const W4Hit& hit1, const W4Hit& hit2);
	
    // 	W4Hit& operator=(const W4Hit& other)
    // 	{
    // 	    detector_id = other.detector_id;
    // 	    process_name = other.process_name;
    // 	    global_time = other.global_time;
    // 	    stripid_x = other.stripid_x;
    // 	    stripid_y = other.stripid_y;
    // 	    position = other.position;
    // 	    direction = other.direction;
    // 	    energy = other.energy;
    // 	    nmerged_hits = other.nmerged_hits;
    // 	    merged_process_name = other.merged_process_name;
    // 	    return *this;
    // 	}

    // private:
	
    // 	int detector_id;
    // 	std::string process_name;
    // 	double global_time;
    // 	int stripid_x;
    // 	int stripid_y;
    // 	G4ThreeVector position;
    // 	G4ThreeVector direction;
    // 	double energy;

    // 	int nmerged_hits;
    // 	std::string merged_process_name;
	
    // };

    
private:

    // W4HitsCollection * hits_collection;
    //std::vector<W4Hit*> hits_collection;
    // std::vector<W4Hit> hits_collection;
    W4DSDHitsCollection * dsd_hits_collection;
    
    G4int detector_id;
    G4int detector_material;
    G4String grid_xaxis_name;
    G4String grid_yaxis_name;
    G4String grid_zaxis_name;
    TH1D * grid_xaxis;
    TH1D * grid_yaxis;
    TH1D * grid_zaxis;

    G4ThreeVector center_position;
    // G4ThreeVector rotation_vector;
    // G4ThreeVector inverse_rotation;
    G4RotationMatrix rotation;
    G4RotationMatrix inverse_rotation;

    G4String GetUniqueObjectName(const G4String& name);
    void DeleteObject(TObject* obj);
    void ApplyParameters();
    // std::vector<W4Hit> MergeHits(std::vector<W4Hit> hits);

    bool is_applied_parameters;
    bool is_merge_same_pixel;
    bool is_merge_adjacent_pixel;
    int  verbose_level;
};

#endif
