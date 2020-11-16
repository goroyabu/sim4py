/**
   @file    W4SensitiveDetector.cpp
   @author  Goro Yabu
   @date    2020/01/30
**/

#include "W4SensitiveDetector.hpp"
#include "W4HitsCollection.hpp"

#include <P4RootAnalysisManager.hpp>

#include <iostream>
#include <vector>
#include <algorithm>

#include <G4ParticleDefinition.hh>
#include <G4UnitsTable.hh>

#include <G4Track.hh>
#include <G4Step.hh>
#include <G4HCofThisEvent.hh>
#include <G4TouchableHistory.hh>
#include <G4VProcess.hh>

#include <G4SDManager.hh>

#include <TROOT.h>
#include <TString.h>
#include <TH1D.h>

using std::cout;
using std::endl;

W4SensitiveDetector::W4SensitiveDetector( G4String name )
    : sim4py::ParameterGene("W4SensitiveDetector"),
      G4VSensitiveDetector(name)
{
    collectionName.insert( name+"_hc" );
    
    dsd_hits_collection = nullptr;

    detector_id = 0;
    detector_material = 0;
    grid_xaxis = nullptr;
    grid_yaxis = nullptr;
    grid_zaxis = nullptr;
    
    grid_xaxis_name = GetUniqueObjectName("grid_xaxis");
    grid_yaxis_name = GetUniqueObjectName("grid_yaxis");
    grid_zaxis_name = GetUniqueObjectName("grid_zaxis");    
    
    grid_xaxis = new TH1D( grid_xaxis_name, ";X", 100, -5, 5);
    grid_yaxis = new TH1D( grid_yaxis_name, ";Y", 100, -5, 5);
    grid_zaxis = new TH1D( grid_zaxis_name, ";Z",   1, -5, 5);

    is_merge_same_pixel = true;
    is_merge_adjacent_pixel = true;
    DefineParameter<bool>("merge_same_pixel", is_merge_same_pixel);
    DefineParameter<bool>("merge_adjacent_pixel", is_merge_adjacent_pixel);

    verbose_level = 0;
    DefineParameter<int>("verbose_level", verbose_level);
    
    is_applied_parameters = false;

    center_position = G4ThreeVector( 0, 0, 0 );
    // rotation_vector = G4ThreeVector( 0, 0, 0 );
    // inverse_rotation = -1 * rotation_vector;

    // rotation.set( 0, 0, 0 );
    // inverse_rotation = rotation.inverse();
}

W4SensitiveDetector::~W4SensitiveDetector()
{
    DeleteObject( grid_xaxis );
    DeleteObject( grid_yaxis );
    DeleteObject( grid_zaxis );
}

void W4SensitiveDetector::Initialize( G4HCofThisEvent* hce )
{
    if ( is_applied_parameters==false )
	ApplyParameters();

    // if ( hce->GetCapacity()==0 ) {

    // dsd_hits_collection
    // 	= new W4DSDHitsCollection( "W4SensitiveDetector", "W4HitsCollection" );
    dsd_hits_collection
        = new W4DSDHitsCollection( G4VSensitiveDetector::SensitiveDetectorName,
				   G4VSensitiveDetector::collectionName[0] );
    auto hcID
	= G4SDManager::GetSDMpointer()->GetCollectionID( G4VSensitiveDetector::collectionName[0] );
    hce->AddHitsCollection( hcID, dsd_hits_collection );
    
    // }
    // else
    // 	dsd_hits_collection = dynamic_cast<W4DSDHitsCollection*>( hce->GetHC( 0 ) );
    
    // hits_collection.clear();
}

G4bool W4SensitiveDetector::ProcessHits( G4Step* step, G4TouchableHistory* )
{    
    G4StepPoint* pre = step->GetPreStepPoint();
    G4StepPoint* post = step->GetPostStepPoint();

    auto process = post->GetProcessDefinedStep(); // korede OK 
    //auto process = pre->GetProcessDefinedStep();
    G4String process_name = "None";
    if ( process!=nullptr ) {
	process_name = process->GetProcessName();
    }
    auto global_time = post->GetGlobalTime();
    
    G4double edep           = step->GetTotalEnergyDeposit();
    G4double length         = step->GetStepLength();
    G4String particle_name  = step->GetTrack()->GetParticleDefinition()->GetParticleType();

    // G4ThreeVector pos = pre->GetPosition();
    // G4ThreeVector dir = pre->GetMomentumDirection();
    G4ThreeVector pre_pos   = pre->GetPosition();    
    G4ThreeVector pre_dir   = pre->GetMomentumDirection();
    G4ThreeVector post_pos  = post->GetPosition();
    G4ThreeVector post_dir  = post->GetMomentumDirection();
    G4double kinetic_energy = post->GetKineticEnergy();    

    // auto touchable = step->GetPreStepPoint()->GetTouchable();
    // auto cell_id   = touchable->GetReplicaNumber(1);

    if ( edep <= 0.0 || length <= 0.0 ) return true;
    
    auto [ strip_x, strip_y, strip_z ] = GetStripID( pre_pos );
    
    // W4Hit hit;
    // hit.SetDetectorID( GetDetectorID() );
    // hit.SetProcessName( process_name );
    // hit.SetTime( global_time/CLHEP::ns );
    // hit.SetStripID( strip_x, strip_y );
    // hit.SetPosition( pos/CLHEP::mm );
    // hit.SetDirection( dir/CLHEP::mm );
    // hit.SetEnergy( edep/CLHEP::keV );
    // hits_collection.emplace_back( hit );
    
    auto hit = new W4DSDHit();
    hit->SetDetectorID( GetDetectorID() );
    hit->SetDetectorMaterial( GetDetectorMaterial() );
    hit->SetProcessName( process_name );
    hit->SetParticleName( particle_name );
    hit->SetTime( global_time/CLHEP::ns );
    hit->SetStripID( strip_x, strip_y );
    hit->SetPosition( pre_pos/CLHEP::mm );
    hit->SetDirection( pre_dir/CLHEP::mm );
    hit->SetPrePosition( pre_pos/CLHEP::mm );
    hit->SetPreDirection( pre_dir/CLHEP::mm );
    hit->SetPostPosition( post_pos/CLHEP::mm );
    hit->SetPostDirection( post_dir/CLHEP::mm );
    hit->SetStepLength( length/CLHEP::mm );

    hit->SetEnergy( edep/CLHEP::keV );
    hit->SetKineticEnergy( kinetic_energy/CLHEP::keV );
    hit->SetMerge( is_merge_same_pixel, is_merge_adjacent_pixel );
    // auto [ x, y, z ] = GetCenterOfPixel( hit->StripIDX(), hit->StripIDY() );
    auto [ x, y, z ] = GetCenterOfPixel( strip_x, strip_y, strip_z );
    hit->SetPixelCenter( x, y, z );
    dsd_hits_collection->insert( hit );

    if ( verbose_level>2 ) {
	cout << "W4SensitiveDetector::ProcessHits : ";
	cout << "detid=" << GetDetectorID() << ", material=" << GetDetectorMaterial();
	cout << ", proc=" << process_name << ", strip=" << strip_x << "," << strip_y;
	cout << ", edep=" << edep/CLHEP::keV << endl;	
    }
    
    return true;
}

void W4SensitiveDetector::EndOfEvent( G4HCofThisEvent* )
{
    // std::cout << "W4SensitiveDetector::EndOfEvent()" << std::endl;
    //std::vector<DsdHit*> merged_hits;
    //auto nhits = hits_collection->entries();

    // auto hits  = dsd_hits_collection->GetVector();
    
    // auto analysis_manager = P4RootAnalysisManager::Instance();
        
    //std::vector<W4Hit> merged_hits;
    //int nhits = hits_collection.size();
        
    // auto merged_hits = MergeHits( hits_collection );
    // auto merged_hits = W4DSDHit::MergeHits( hits, is_merge_same_pixel, is_merge_adjacent_pixel );

    // auto merged_hits = W4DSDHit::MergeHits( hits ); 
    // std::sort( merged_hits.begin(), merged_hits.end(),
    // 	       [](const W4DSDHit& former, const W4DSDHit& latter)
    // 	       {
    // 		   return former.Time() < latter.Time();
    // 	       });
 
    // auto nhits = (int)merged_hits.size();
    // analysis_manager->AddNtupleIColumnName( "nhits", nhits );
    // if ( nhits>0 )
    // 	analysis_manager->AddNtupleIColumnName( "hit_pattern", (int)pow(10, detector_id) );

    // for ( auto hit : merged_hits ) {	
    // 	analysis_manager->FillNtupleDColumnVName( "edep", hit.Energy() );
    // 	analysis_manager->FillNtupleIColumnVName( "detid",    hit.DetectorID() );
    // 	analysis_manager->FillNtupleDColumnVName( "global_time", hit.Time() );
    // 	analysis_manager->FillNtupleIColumnVName( "strip_x",  hit.StripIDX()  );
    // 	analysis_manager->FillNtupleIColumnVName( "strip_y",  hit.StripIDY()   );
    // 	analysis_manager->FillNtupleDColumnVName( "pos_x",  hit.X() );
    // 	analysis_manager->FillNtupleDColumnVName( "pos_y",  hit.Y() );
    // 	analysis_manager->FillNtupleDColumnVName( "pos_z",  hit.Z() );
    // 	analysis_manager->FillNtupleDColumnVName( "dir_x",  hit.DirX() );
    // 	analysis_manager->FillNtupleDColumnVName( "dir_y",  hit.DirY() );
    // 	analysis_manager->FillNtupleDColumnVName( "dir_z",  hit.DirZ() );
    // 	analysis_manager->AddNtupleDColumnName( "etotal", hit.Energy() );
    // 	analysis_manager->AddNtupleSColumnName( "proc_name", hit.ProcessName() );
    // 	auto [ x, y ] = GetCenterOfPixel( hit.StripIDX(), hit.StripIDY() );
    // 	analysis_manager->FillNtupleDColumnVName( "pixel_center_x", x );
    // 	analysis_manager->FillNtupleDColumnVName( "pixel_center_y", y );
	
    // 	analysis_manager->FillNtupleIColumnVName( "nmerged_raw_hits", hit.Nmerged() );
    // 	analysis_manager->AddNtupleSColumnName( "raw_proc_name", hit.MergedProcessName() );
    // }
}

G4int W4SensitiveDetector::GetDetectorID()
{
    return detector_id;
}

G4int W4SensitiveDetector::SetDetectorID( G4int id )
{
    detector_id = id; return 0;
}

G4int W4SensitiveDetector::GetDetectorMaterial()
{
    return detector_material;
}
G4int W4SensitiveDetector::SetDetectorMaterial( G4int mate )
{
    detector_material = mate; return 0;
}

G4int W4SensitiveDetector::SetGridXaxis( G4int nbins, G4double min, G4double max )
{
    if ( !grid_xaxis ) return -1;
    grid_xaxis->SetBins( nbins, min, max );    
    return 0;
}

G4int W4SensitiveDetector::SetGridYaxis( G4int nbins, G4double min, G4double max )
{
    if ( !grid_yaxis ) return -1;
    grid_yaxis->SetBins( nbins, min, max );	
    return 0;
}

G4int W4SensitiveDetector::SetGridZaxis( G4int nbins, G4double min, G4double max )
{
    if ( !grid_zaxis ) return -1;
    grid_zaxis->SetBins( nbins, min, max );	
    return 0;
}

// G4int W4SensitiveDetector::SetPositionRotation( const G4ThreeVector& pos, const G4ThreeVector& rot)
G4int W4SensitiveDetector::SetPositionRotation( const G4ThreeVector& pos, const G4RotationMatrix& rot)
{
    this->center_position  = pos;
    // this->rotation_vector  = rot;
    // this->inverse_rotation = -1 * rot;
    this->rotation = rot;
    this->inverse_rotation = rot.inverse();
    return 0;
}

std::tuple<G4int, G4int, G4int> W4SensitiveDetector::GetStripID
( const G4ThreeVector& pos )
{
    auto local_pos = pos - this->center_position;
    local_pos.transform( this->inverse_rotation );
    // local_pos += this->center_position;
    // local_pos.rotateX( this->inverse_rotation.getX() );
    // local_pos.rotateY( this->inverse_rotation.getY() );
    // local_pos.rotateZ( this->inverse_rotation.getZ() );
    
    auto x = grid_xaxis->FindBin( local_pos.x() ) - 1;
    // auto y = grid_yaxis->FindBin( local_pos.y() ) - 1;
    auto y = grid_yaxis->FindBin( local_pos.y() ) - 1 + grid_xaxis->GetXaxis()->GetNbins();
    auto z = grid_zaxis->FindBin( local_pos.z() ) - 1;
    return std::make_tuple(x, y, z);
}

std::tuple<G4double, G4double, G4double> W4SensitiveDetector::GetCenterOfPixel
( G4int strip_x, G4int strip_y, G4int strip_z )
{
    auto x = grid_xaxis->GetBinCenter( strip_x+1 );
    auto y = grid_yaxis->GetBinCenter( strip_y+1-grid_xaxis->GetXaxis()->GetNbins() );
    auto z = grid_zaxis->GetBinCenter( strip_z+1 );

    auto local_pos = G4ThreeVector( x, y, z );
    local_pos.transform( this->rotation );
    local_pos += this->center_position;

    x = local_pos.x();
    y = local_pos.y();
    z = local_pos.z();
    
    return std::make_tuple(x, y, z);
}

G4String W4SensitiveDetector::GetUniqueObjectName(const G4String& name)
{
    TObject* obj = gROOT->FindObject((TString)name);
    if ( !obj ) return G4String(name);

    TString newname; int i = 1;
    while ( i<100 ) {
	newname = name+Form( "_%d", i);
	TObject* tmp = gROOT->FindObject(newname);
	if ( !tmp ) return G4String(newname);
	++i;
    }
    
    return "";
}

void W4SensitiveDetector::DeleteObject(TObject* obj)
{
    gROOT->Remove(obj);
    if ( !!obj ) obj->Delete();
}

void W4SensitiveDetector::ApplyParameters()
{
    auto [ same ] = GetParameter<bool>("merge_same_pixel");
    is_merge_same_pixel = same;

    auto [ adja ] = GetParameter<bool>("merge_adjacent_pixel");
    is_merge_adjacent_pixel = adja;

    auto [ ver ] = GetParameter<int>("verbose_level");
    verbose_level = ver;
    
    is_applied_parameters = true;
}

// std::vector<W4SensitiveDetector::W4Hit> W4SensitiveDetector::MergeHits(std::vector<W4SensitiveDetector::W4Hit> hits)
// {    
//     std::vector<W4Hit> merged_hits;

//     while ( hits.empty()==false ) {
	
// 	int nhits = static_cast<int>( hits.size() );
// 	std::vector<W4Hit> buffer;
	
// 	for ( int ihit=nhits-1; ihit>=0; --ihit ) {		
	    
// 	    if ( buffer.empty()==true ) {
// 		buffer.emplace_back( hits[ihit] );
// 		hits.erase( hits.begin()+ihit );
// 		continue;
// 	    }
	    
// 	    if ( hits[ihit].ExistSamePixelAmong( buffer ) &&
// 		 is_merge_same_pixel==true ) {
		
// 		buffer.emplace_back( hits[ihit] );
// 		hits.erase( hits.begin()+ihit );
// 		// cout << "same " << ihit << endl;
// 		continue;
		
// 	    }

// 	    if ( hits[ihit].ExistAdjacentPixelAmong( buffer ) &&
// 		 is_merge_adjacent_pixel==true ) {

// 		buffer.emplace_back( hits[ihit] );
// 		hits.erase( hits.begin()+ihit );
// 		// cout << "ad" << endl;
// 		continue;

// 	    }	    		
// 	}

// 	auto merged = W4Hit::MergeThem( buffer, &W4Hit::MergeOnFirstHit );
// 	merged_hits.emplace_back( merged );	    
	
//     }

//     return merged_hits;    
// }

// W4SensitiveDetector::W4Hit::W4Hit()    
// {
//     detector_id = -1;
//     process_name = "None";
//     global_time = -1;
//     stripid_x = -1;
//     stripid_y = -1;
//     position = G4ThreeVector(0,0,0);
//     direction = G4ThreeVector(0,0,0);
//     energy = 0;
//     nmerged_hits = 1;
//     merged_process_name = "None";
// }
// W4SensitiveDetector::W4Hit::W4Hit(const W4Hit& other)
// {
//     detector_id = other.detector_id;
//     process_name = other.process_name;
//     global_time = other.global_time;
//     stripid_x = other.stripid_x;
//     stripid_y = other.stripid_y;
//     position = other.position;
//     direction = other.direction;
//     energy = other.energy;
//     nmerged_hits = other.nmerged_hits;
//     merged_process_name = other.merged_process_name;
// }

// W4SensitiveDetector::W4Hit::~W4Hit()
// {
// }

// W4SensitiveDetector::W4Hit& W4SensitiveDetector::W4Hit::SetDetectorID(int id)
// {
//     this->detector_id = id;
//     return *this;
// }
// W4SensitiveDetector::W4Hit& W4SensitiveDetector::W4Hit::SetTime(double t)
// {
//     this->global_time = t;
//     return *this;
// }
// W4SensitiveDetector::W4Hit& W4SensitiveDetector::W4Hit::SetStripID(int x, int y)
// {
//     this->stripid_x = x; this->stripid_y = y;
//     return *this;
// }
// W4SensitiveDetector::W4Hit& W4SensitiveDetector::W4Hit::SetPosition
// (const G4ThreeVector& p)
// {
//     this->position = p;
//     return *this;
// }
// W4SensitiveDetector::W4Hit& W4SensitiveDetector::W4Hit::SetDirection
// (const G4ThreeVector& v)
// {
//     this->direction = v;
//     return *this;
// }
// W4SensitiveDetector::W4Hit& W4SensitiveDetector::W4Hit::SetEnergy
// (double e)
// {
//     this->energy = e;
//     return *this;
// }
// W4SensitiveDetector::W4Hit& W4SensitiveDetector::W4Hit::SetProcessName
// (const std::string& name)
// {
//     this->process_name = name;
//     this->merged_process_name = name;
//     return *this;
// }
// bool W4SensitiveDetector::W4Hit::IsSamePixel
// (const W4SensitiveDetector::W4Hit& other)
// {
//     // std::cout << detector_id << " " << stripid_x << " " << stripid_y << " == " << other.detector_id << " " << other.stripid_x << " " << other.stripid_y << std::endl;
//     // auto is1 = detector_id == other.detector_id;
//     // auto is2 = stripid_x == other.stripid_x;
//     // auto is3 = stripid_y == other.stripid_y;

//     // if ( is1 ) cout << "same detid!!!" << endl;
//     // else cout << "not same detid!!!" << endl;

//     // if ( is2 ) cout << "same x!!!" << endl;
//     // else cout << "not same x!!!"<< endl;

//     // if ( is3 ) cout << "same y!!!" << endl;
//     // else cout << "not same y!!!"<< endl;
    
//     // return is1 && is2 && is3;
    
//     return detector_id == other.detector_id
//         && stripid_x == other.stripid_x
//         && stripid_y == other.stripid_y;
// }
// bool W4SensitiveDetector::W4Hit::IsAdjacentPixel
// (const W4SensitiveDetector::W4Hit& other)
// {
//     if ( detector_id != other.detector_id ) return false;
//     if ( IsSamePixel( other ) == true ) return false;
     
//     auto dx = abs( stripid_x - other.stripid_x );
//     auto dy = abs( stripid_y - other.stripid_y );
    
//     return ( dx==1 && dy==0 ) || ( dx==0 && dy==1 );
// }
// // void W4SensitiveDetector::W4Hit::MergeSamePixel
// // (const W4SensitiveDetector::W4Hit& other)
// // {
// //     // if ( IsSamePixel(other)==false ) return;

// //     G4ThreeVector newpos(0,0,0);
// //     if ( this->energy > other.energy ) newpos = this->position;
// //     else newpos = other.position;

// //     SetPosition( newpos );
// //     SetEnergy( this->energy + other.energy );
// // }
// // void W4SensitiveDetector::W4Hit::MergeAdjacentPixel
// // (const W4SensitiveDetector::W4Hit& other)
// // {
// //     // if ( IsAdjacentPixel(other)==false ) return;
    
// //     G4ThreeVector newpos(0,0,0);

// //     //    if ( mode==0 ) {
// //     if ( this->energy > other.energy ) newpos = this->position;
// //     else newpos = other.position;
// //     //}

// //     SetPosition( newpos );
// //     SetEnergy( this->energy + other.energy );
// // }

// W4SensitiveDetector::W4Hit W4SensitiveDetector::W4Hit::MergeOnEdepMax
// (const W4SensitiveDetector::W4Hit& hit1, const W4SensitiveDetector::W4Hit& hit2)
// {
//     auto merged = hit1;

//     merged.SetTime( hit1.global_time );
    
//     if ( hit1.energy < hit2.energy ) merged.SetPosition( hit2.position );
//     else merged.SetPosition( hit1.position );
    
//     merged.SetEnergy( hit1.energy + hit2.energy );
//     merged.SetDirection( hit2.direction );
//     merged.SetProcessName( hit1.process_name );
//     // merged.SetProcessName( hit1.process_name+"+"+hit2.process_name );
//     // cout << "merge" << endl;

//     merged.nmerged_hits = hit1.nmerged_hits + hit2.nmerged_hits;
//     merged.merged_process_name = hit1.merged_process_name + "+" + hit2.merged_process_name;
    
//     return merged;
// }

// W4SensitiveDetector::W4Hit W4SensitiveDetector::W4Hit::MergeOnFirstHit
// (const W4SensitiveDetector::W4Hit& hit1, const W4SensitiveDetector::W4Hit& hit2)
// {
//     auto merged = hit1;    

//     merged.SetTime( hit1.global_time );
    
//     merged.SetEnergy( hit1.energy + hit2.energy );    
//     merged.SetDirection( hit1.direction );
//     merged.SetProcessName( hit1.process_name );

//     merged.nmerged_hits = hit1.nmerged_hits + hit2.nmerged_hits;
//     merged.merged_process_name = hit1.merged_process_name + "+" + hit2.merged_process_name;    
    
//     return merged;
// }

// bool W4SensitiveDetector::W4Hit::ExistSamePixelAmong
// (const std::vector<W4SensitiveDetector::W4Hit>& hits)
// {
//     for ( auto hit : hits ) 
// 	if ( this->IsSamePixel( hit )==true ) return true;    
//     return false;
// }

// bool W4SensitiveDetector::W4Hit::ExistAdjacentPixelAmong
// (const std::vector<W4SensitiveDetector::W4Hit>& hits)
// {
//     for ( auto hit : hits )
// 	if ( this->IsAdjacentPixel( hit )==true ) return true;
//     return false;
// }

// W4SensitiveDetector::W4Hit W4SensitiveDetector::W4Hit::MergeThem
// (const std::vector<W4SensitiveDetector::W4Hit>& hits,
//  std::function<W4SensitiveDetector::W4Hit(const W4SensitiveDetector::W4Hit&, const W4SensitiveDetector::W4Hit&)> merging_method)
// {
//     W4Hit merged;
    
//     if ( hits.empty() ) return merged;        

//     int nhits = static_cast<int>( hits.size() );    
//     merged = hits[ 0 ];
    
//     for ( int ihit=1; ihit<nhits; ++ihit )
// 	merged = merging_method( hits[ihit], merged );	
    
//     return merged;
// }
