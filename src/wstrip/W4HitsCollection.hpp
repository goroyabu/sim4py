/**
   @file    W4HitsCollection.hpp
   @author  Goro Yabu
   @date    2020/01/31
**/

#ifndef W4HitsCollection_hpp
#define W4HitsCollection_hpp

#include <G4VHit.hh>
#include <G4THitsCollection.hh>
#include <G4Allocator.hh>
#include <G4ThreeVector.hh>
#include <G4Threading.hh>

class W4DSDHit : public G4VHit
{

private:
    
    int detector_id;
    std::string process_name;
    double global_time;
    int stripid_x;
    int stripid_y;
    G4ThreeVector position;
    G4ThreeVector direction;
    double energy;

    double pixel_center_x;
    double pixel_center_y;
    
    int nmerged_hits;
    std::string merged_process_name;

    bool is_merge_same_pixel;
    bool is_merge_adjacent_pixel;
    
public:
    
    W4DSDHit()
    {
	detector_id = -1;
	process_name = "None";
	global_time = -1;
	stripid_x = -1;
	stripid_y = -1;
	position = G4ThreeVector(0,0,0);
	direction = G4ThreeVector(0,0,0);
	energy = 0;
	pixel_center_x = 0;
	pixel_center_y = 0;
	nmerged_hits = 1;
	merged_process_name = "None";
	is_merge_same_pixel = true;
	is_merge_adjacent_pixel = false;
    }
    W4DSDHit(const W4DSDHit& other)
    {
	detector_id = other.detector_id;
	process_name = other.process_name;
	global_time = other.global_time;
	stripid_x = other.stripid_x;
	stripid_y = other.stripid_y;
	position = other.position;
	direction = other.direction;
	energy = other.energy;
	pixel_center_x = other.pixel_center_x;
	pixel_center_y = other.pixel_center_y;
	nmerged_hits = other.nmerged_hits;
	merged_process_name = other.merged_process_name;
	is_merge_same_pixel = other.is_merge_same_pixel;
	is_merge_adjacent_pixel = other.is_merge_adjacent_pixel;
    }    
    virtual ~W4DSDHit()
    {
    }
    
    W4DSDHit& operator=(const W4DSDHit& other)
    {
	detector_id  = other.detector_id;
	process_name = other.process_name;
	global_time  = other.global_time;
	stripid_x    = other.stripid_x;
	stripid_y    = other.stripid_y;
	position     = other.position;
	direction    = other.direction;
	energy       = other.energy;
	pixel_center_x = other.pixel_center_x;
	pixel_center_y = other.pixel_center_y;	
	nmerged_hits = other.nmerged_hits;
	merged_process_name = other.merged_process_name;
	is_merge_same_pixel = other.is_merge_same_pixel;
	is_merge_adjacent_pixel = other.is_merge_adjacent_pixel;	
	return *this;
    }
    G4bool operator==(const W4DSDHit&) const;     
    
    inline void* operator new(size_t);
    inline void  operator delete(void*);
    
    // virtual void Draw() const override;
    // virtual void Print() const override;        
    
public:    

    inline W4DSDHit& SetDetectorID(int id)                   { detector_id = id; return *this; }
    inline W4DSDHit& SetTime(double t)                       { global_time = t; return *this; }
    inline W4DSDHit& SetStripID(int x, int y)                { stripid_x = x; stripid_y = y; return *this; }
    inline W4DSDHit& SetPosition(const G4ThreeVector& p)     { position = p; return *this;}
    inline W4DSDHit& SetDirection(const G4ThreeVector& v)    { direction = v; return *this; }
    inline W4DSDHit& SetEnergy(double e)                     { energy = e; return *this; }
    inline W4DSDHit& SetPixelCenter(double x, double y)
    {
	this->pixel_center_x = x;
	this->pixel_center_y = y;
	return *this;
    }
    inline W4DSDHit& SetProcessName(const std::string& name) { process_name = name; return *this; }
    inline W4DSDHit& SetMerge(bool is_merge_same, bool is_merge_adjacent)
    {
	this->is_merge_same_pixel = is_merge_same;
	this->is_merge_adjacent_pixel = is_merge_adjacent;
	return *this;
    }
    
    inline int DetectorID()          const { return detector_id; }
    inline double Time()             const { return global_time; }
    inline std::string ProcessName() const { return process_name; }
    inline int StripIDX()            const { return stripid_x; }
    inline int StripIDY()            const { return stripid_y; }
    inline double X()                const { return position.x(); }
    inline double Y()                const { return position.y(); }
    inline double Z()                const { return position.z(); }
    inline double DirX()             const { return direction.x(); }
    inline double DirY()             const { return direction.y(); }
    inline double DirZ()             const { return direction.z(); }
    inline double Energy()           const { return energy; }
    inline double PixelCenterX()     const { return pixel_center_x; }
    inline double PixelCenterY()     const { return pixel_center_y; }
    inline int Nmerged()             const { return nmerged_hits; }
    inline std::string MergedProcessName() const { return merged_process_name; }
    inline bool IsMergeSamePixel()     const { return is_merge_same_pixel; }
    inline bool IsMergeAdjacentPixel() const { return is_merge_adjacent_pixel; }

    static std::vector<W4DSDHit> MergeHits
    (std::vector<W4DSDHit*>* hits);
    //bool is_merge_same_pixel, bool is_merge_adjacent_pixel);

private:
    
    inline bool IsSamePixel(const W4DSDHit& other)
    {		
	return detector_id == other.detector_id
	    && stripid_x == other.stripid_x
	    && stripid_y == other.stripid_y;
    }
    inline bool IsAdjacentPixel(const W4DSDHit& other)
    {
	if ( detector_id != other.detector_id ) return false;
	if ( IsSamePixel( other ) == true ) return false;	
	auto dx = abs( stripid_x - other.stripid_x );
	auto dy = abs( stripid_y - other.stripid_y );	
	return ( dx==1 && dy==0 ) || ( dx==0 && dy==1 );
    }
    
    bool ExistSamePixelAmong(const std::vector<W4DSDHit>& hits);
    bool ExistAdjacentPixelAmong(const std::vector<W4DSDHit>& hits);    
    static W4DSDHit MergeThem
    (const std::vector<W4DSDHit>& hits,
     std::function<W4DSDHit(const W4DSDHit&, const W4DSDHit&)> merging_method);
    static W4DSDHit MergeOnEdepMax(const W4DSDHit& hit1, const W4DSDHit& hit2);
    static W4DSDHit MergeOnFirstHit(const W4DSDHit& hit1, const W4DSDHit& hit2);


};

using W4DSDHitsCollection = G4THitsCollection<W4DSDHit>;

extern G4ThreadLocal G4Allocator<W4DSDHit>* W4DSDHitAllocator;

inline void* W4DSDHit::operator new(size_t)
{
    if (!W4DSDHitAllocator) {
	W4DSDHitAllocator = new G4Allocator<W4DSDHit>;
    }
    void *hit;
    hit = (void *) W4DSDHitAllocator->MallocSingle();
    return hit;
}
inline void W4DSDHit::operator delete(void *hit)
{
    if (!W4DSDHitAllocator) {
	W4DSDHitAllocator = new G4Allocator<W4DSDHit>;
    }
    W4DSDHitAllocator->FreeSingle((W4DSDHit*) hit);
}

#endif
