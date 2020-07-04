/**
   @file    W4SensitiveDetector.cpp
   @author  Goro Yabu
   @date    2020/01/30
**/

#include "W4SensitiveDetector.hpp"

#include <P4RootAnalysisManager.hpp>

#include <iostream>
#include <vector>

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
    : G4VSensitiveDetector(name)
{
    // hits_collection = nullptr;
    detector_id = 0;
    grid_xaxis = nullptr;
    grid_yaxis = nullptr;
    grid_zaxis = nullptr;
    
    grid_xaxis_name = GetUniqueObjectName("grid_xaxis");
    grid_yaxis_name = GetUniqueObjectName("grid_yaxis");
    grid_zaxis_name = GetUniqueObjectName("grid_zaxis");    
    
    grid_xaxis = new TH1D( grid_xaxis_name, ";X", 100, -5, 5);
    grid_yaxis = new TH1D( grid_yaxis_name, ";Y", 100, -5, 5);
    grid_zaxis = new TH1D( grid_zaxis_name, ";Z",   1, -5, 5);
}

W4SensitiveDetector::~W4SensitiveDetector()
{
    DeleteObject( grid_xaxis );
    DeleteObject( grid_yaxis );
    DeleteObject( grid_zaxis );
}

void W4SensitiveDetector::Initialize( G4HCofThisEvent* )
{    
    // //this->hits_collection = new HitsCollection("detector", "collection");
    // cout << "SD::Initialize" << endl;
    // this->hits_collection = new W4HitsCollection( GetName(), GetName()+"_hc" );

    // cout << "Make HC" << endl;
    // auto index_of_hits_collection
    // 	= G4SDManager::GetSDMpointer()->GetCollectionID( GetName()+"_hc" );
    // 	//= G4SDManager::GetSDMpointer()->GetCollectionID( "collection" );
    // cout << "Get Index" << endl;
    // hce->AddHitsCollection( index_of_hits_collection, this->hits_collection );
    // cout << "SD::Initialize End" << endl;
    hits_collection.clear();
}

void W4SensitiveDetector::EndOfEvent( G4HCofThisEvent* )
{
    // std::cout << "W4SensitiveDetector::EndOfEvent()" << std::endl;
    
    auto analysis_manager = P4RootAnalysisManager::Instance();
        
    //std::vector<DsdHit*> merged_hits;
    std::vector<W4Hit*> merged_hits;
    //auto nhits = hits_collection->entries();
    int nhits = hits_collection.size();
    //auto hits  = hits_collection->GetVector();
    
    for ( int i=0; i<nhits; ++i ) {
       	
	if ( i == 0 ) {
	    // merged_hits.emplace_back( (*hits)[i] );
	    merged_hits.emplace_back( hits_collection[0] ); 
	    continue;
	}
	
	//auto current  = (*hits)[i];
	auto current = hits_collection[i];
	int mode = 1;

	int nhits_merged = (int)merged_hits.size();
	for ( int j=0; j<nhits_merged; ++j ) {
	    if ( mode == 0 ) {
		if ( merged_hits[j]->IsSamePixel( *current ) ) {
		    merged_hits[j]->MergeSamePixel( *current );
		    j = nhits_merged++;		    
		}
	    }
	    else {
		if ( merged_hits[j]->IsAdjacentPixel( *current ) ) {
		    merged_hits[j]->MergeAdjacentPixel( *current );
		    j = nhits_merged++;		    
		}
	    }		
	}
	if ( nhits_merged == (int)merged_hits.size() )
	    merged_hits.emplace_back( current );	

    }

    //analysis_manager->FillNtupleIColumnName( "nhits", (int)merged_hits.size() );
    nhits = (int)merged_hits.size();
    analysis_manager->AddNtupleIColumnName( "nhits", nhits );
    if ( nhits>0 )
	analysis_manager->AddNtupleIColumnName( "hit_pattern", (int)pow(10, detector_id) );

    // cout << nhits << " ";
    for ( auto hit : merged_hits ) {	
	analysis_manager->FillNtupleDColumnVName( "edep", hit->Energy() );
	analysis_manager->FillNtupleIColumnVName( "detid",    hit->DetectorID() );
	analysis_manager->FillNtupleIColumnVName( "strip_x",  hit->StripIDX()  );
	analysis_manager->FillNtupleIColumnVName( "strip_y",  hit->StripIDY()   );
	analysis_manager->FillNtupleDColumnVName( "pos_x",  hit->X() );
	analysis_manager->FillNtupleDColumnVName( "pos_y",  hit->Y() );
	analysis_manager->FillNtupleDColumnVName( "pos_z",  hit->Z() );
	analysis_manager->AddNtupleDColumnName( "etotal", hit->Energy() );
	analysis_manager->AddNtupleSColumnName( "proc_name", hit->ProcessName() );
	// cout << hit->Energy() << " ";
	auto [ x, y ] = GetCenterOfPixel( hit->StripIDX(), hit->StripIDY() );
	analysis_manager->FillNtupleDColumnVName( "pixel_center_x", x );
	analysis_manager->FillNtupleDColumnVName( "pixel_center_y", y );
    }
    // cout << endl;
    //if ( (int)merged_hits.size()!=0 )
    //analysis_manager->FillNtupleDColumnName( "etotal", merged_hits[0]->total_energy );
    // analysis_manager->FillH1( 0, total_energy );
}

G4bool W4SensitiveDetector::ProcessHits( G4Step* step, G4TouchableHistory* )
{
    // std::cout << "W4SensitiveDetector::ProcessHits()" << std::endl;

    // cout << "detid=" << GetDetectorID() << endl;
    // auto analysis_manager = AnalysisManager::Instance();
    
    G4StepPoint* pre = step->GetPreStepPoint();
    G4StepPoint* post = step->GetPostStepPoint();

    // auto process = pre->GetProcessDefinedStep();
    auto process = post->GetProcessDefinedStep();
    G4String process_name = "";
    if ( process!=nullptr ) {
	process_name = process->GetProcessName();
	// if ( process_name!="msc" && process_name!="Transportation" )
	//     cout << process_name << endl;
    }
    
    G4double edep = step->GetTotalEnergyDeposit();
    G4double length = step->GetStepLength();

    G4ThreeVector pos = pre->GetPosition();

    // auto touchable = step->GetPreStepPoint()->GetTouchable();
    // auto cell_id   = touchable->GetReplicaNumber(1);

    if ( edep <= 0.0 || length <= 0.0 ) return true;

    //auto hit = new DsdHit;
    auto hit = new W4Hit();
    auto [ strip_x , strip_y ] = GetStripID( pos );
    hit->SetDetectorID( GetDetectorID() )
	->SetProcessName( process_name )
	->SetStripID( strip_x, strip_y )
	->SetPosition( pos/CLHEP::mm )
	->SetEnergy( edep/CLHEP::keV );

    hits_collection.emplace_back( hit ); 
    
    //hits_collection->insert( hit );
    //(*hits_collection)[0]->AddTotalEnergy( edep/CLHEP::keV );

    //hit->Add(edep, length);
    // if ( ! hit ) {
    // 	G4ExceptionDescription msg;
    // 	msg << "Cannot access hit " << cell_id;
    // 	G4Exception("B4cCalorimeterSD::ProcessHits()",
    // 		    "MyCode0004", FatalException, msg);
    // }

    return true;
}

G4int W4SensitiveDetector::GetDetectorID()
{
    return detector_id;
}

G4int W4SensitiveDetector::SetDetectorID( G4int id )
{
    detector_id = id; return 0;
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

std::pair<G4int, G4int> W4SensitiveDetector::GetStripID( const G4ThreeVector& pos )
{
    auto x = grid_xaxis->FindBin( pos.x() );
    auto y = grid_yaxis->FindBin( pos.y() );
    return std::make_pair(x, y);
}

std::pair<G4double, G4double> W4SensitiveDetector::GetCenterOfPixel( G4int strip_x, G4int strip_y )
{
    auto x = grid_xaxis->GetBinCenter( strip_x );
    auto y = grid_xaxis->GetBinCenter( strip_y );
    return std::make_pair(x, y);
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

W4SensitiveDetector::W4Hit::W4Hit()    
{
    detector_id = -1;
    process_name = "";
    stripid_x = -1;
    stripid_y = -1;
    position = G4ThreeVector(0,0,0);
    energy = 0;
}

W4SensitiveDetector::W4Hit::~W4Hit()
{
}

W4SensitiveDetector::W4Hit* W4SensitiveDetector::W4Hit::SetDetectorID(int id)
{
    this->detector_id = id;
    return this;
}
W4SensitiveDetector::W4Hit* W4SensitiveDetector::W4Hit::SetStripID(int x, int y)
{
    this->stripid_x = x; this->stripid_y = y;
    return this;
}
W4SensitiveDetector::W4Hit* W4SensitiveDetector::W4Hit::SetPosition
(const G4ThreeVector& p)
{
    this->position = p;
    return this;
}
W4SensitiveDetector::W4Hit* W4SensitiveDetector::W4Hit::SetEnergy
(double e)
{
    this->energy = e;
    return this;
}
W4SensitiveDetector::W4Hit* W4SensitiveDetector::W4Hit::SetProcessName
(const std::string& name)
{
    this->process_name = name;
    return this;
}
bool W4SensitiveDetector::W4Hit::IsSamePixel
(const W4SensitiveDetector::W4Hit& other)
{
    return detector_id   == other.detector_id
        && stripid_x == other.stripid_x
        && stripid_x == other.stripid_y;
}
bool W4SensitiveDetector::W4Hit::IsAdjacentPixel
(const W4SensitiveDetector::W4Hit& other)
{
    if ( detector_id != other.detector_id ) return false;
    if ( IsSamePixel( other ) == true ) return false;
     
    auto dx = abs( stripid_x - other.stripid_x );
    auto dy = abs( stripid_y - other.stripid_y );
    
    return ( dx==1 && dy==0 ) || ( dx==0 && dy==1 );
}
void W4SensitiveDetector::W4Hit::MergeSamePixel
(const W4SensitiveDetector::W4Hit& other)
{
    if ( IsSamePixel(other)==false ) return;

    G4ThreeVector newpos(0,0,0);
    if ( this->energy > other.energy ) newpos = this->position;
    else newpos = other.position;

    SetPosition( newpos );
    SetEnergy( this->energy + other.energy );
}
void W4SensitiveDetector::W4Hit::MergeAdjacentPixel
(const W4SensitiveDetector::W4Hit& other)
{
    if ( IsAdjacentPixel(other)==false ) return;

    G4ThreeVector newpos(0,0,0);

    //    if ( mode==0 ) {
    if ( this->energy > other.energy ) newpos = this->position;
    else newpos = other.position;
    //}

    SetPosition( newpos );
    SetEnergy( this->energy + other.energy );
}
