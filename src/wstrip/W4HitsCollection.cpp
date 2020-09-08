/**
   @file    W4HitsCollection.cpp
   @author  Goro Yabu
   @date    2020/01/31
   @date    2020/08/25
**/

#include "W4HitsCollection.hpp"

G4ThreadLocal G4Allocator<W4DSDHit>* W4DSDHitAllocator = 0; 

W4DSDHit W4DSDHit::MergeOnEdepMax
(const W4DSDHit& hit1, const W4DSDHit& hit2)
{
    auto merged = hit1;    
    if ( hit1.energy < hit2.energy ) {
	merged.SetPosition( hit2.position );
	merged.SetPixelCenter
	    ( hit2.pixel_center_x, hit2.pixel_center_y, hit2.pixel_center_z );
    }
    else {
	merged.SetPosition( hit1.position );
	merged.SetPixelCenter
	    ( hit2.pixel_center_x, hit2.pixel_center_y, hit2.pixel_center_z );
    }
    merged.SetTime( hit1.global_time );
    merged.SetEnergy( hit1.energy + hit2.energy );
    merged.SetDirection( hit2.direction );
    merged.SetProcessName( hit1.process_name );
    merged.nmerged_hits = hit1.nmerged_hits + hit2.nmerged_hits;
    merged.merged_process_name
	= hit1.merged_process_name + "+" + hit2.merged_process_name;
    return merged;
}
W4DSDHit W4DSDHit::MergeOnFirstHit
(const W4DSDHit& hit1, const W4DSDHit& hit2)
{
    auto merged = hit1;
    merged.SetTime( hit1.global_time );
    merged.SetProcessName( hit1.process_name );
    merged.SetEnergy( hit1.energy + hit2.energy );
    merged.SetDirection( hit1.direction );    
    merged.nmerged_hits = hit1.nmerged_hits + hit2.nmerged_hits;
    merged.merged_process_name
	= hit1.merged_process_name + "+" + hit2.merged_process_name;
    return merged;
}
bool W4DSDHit::ExistSamePixelAmong
(const std::vector<W4DSDHit>& hits)
{
    for ( auto hit : hits )
        if ( this->IsSamePixel( hit )==true ) return true;
    return false;
}
bool W4DSDHit::ExistAdjacentPixelAmong
(const std::vector<W4DSDHit>& hits)
{
    for ( auto hit : hits )
        if ( this->IsAdjacentPixel( hit )==true ) return true;
    return false;
}
W4DSDHit W4DSDHit::MergeThem
(const std::vector<W4DSDHit>& hits,
 std::function<W4DSDHit(const W4DSDHit&, const W4DSDHit&)> merging_method)
{
    W4DSDHit merged;    
    if ( hits.empty() ) return merged;
    
    int nhits = (int)hits.size();
    merged = hits[ 0 ];
    
    for ( int ihit=1; ihit<nhits; ++ihit )
        merged = merging_method( hits[ihit], merged );
    
    return merged;
}
std::vector<W4DSDHit> W4DSDHit::MergeHits
(std::vector<W4DSDHit*>* hits)
//, bool is_merge_same_pixel, bool is_merge_adjacent_pixel)
{    
    std::vector<W4DSDHit> merged_hits;

    while ( hits->empty()==false ) {
	
	int nhits = static_cast<int>( hits->size() );
	std::vector<W4DSDHit> buffer;
	
	for ( int ihit=nhits-1; ihit>=0; --ihit ) {		
	    
	    if ( buffer.empty()==true ) {
		// buffer.emplace_back( hits[ihit] );
		// hits.erase( hits.begin()+ihit );
		buffer.emplace_back( *(hits->at(ihit)) );
		hits->erase( hits->begin()+ihit );
		continue;
	    }
	    
	    if ( hits->at(ihit)->ExistSamePixelAmong( buffer ) &&
		 hits->at(ihit)->IsMergeSamePixel()==true ) {
		
		// buffer.emplace_back( hits[ihit] );
		// hits.erase( hits.begin()+ihit );
		buffer.emplace_back( *(hits->at(ihit)) );
		hits->erase( hits->begin()+ihit );
		// cout << "same " << ihit << endl;
		continue;
		
	    }

	    if ( hits->at(ihit)->ExistAdjacentPixelAmong( buffer ) &&
		 hits->at(ihit)->IsMergeAdjacentPixel()==true ) {

		// buffer.emplace_back( hits[ihit] );
		// hits.erase( hits.begin()+ihit );
		buffer.emplace_back( *(hits->at(ihit)) );
		hits->erase( hits->begin()+ihit );
		// cout << "ad" << endl;
		continue;

	    }	    		
	}

	auto merged = W4DSDHit::MergeThem( buffer, &W4DSDHit::MergeOnFirstHit );
	merged_hits.emplace_back( merged );	    
	
    }
    return merged_hits;    
}
