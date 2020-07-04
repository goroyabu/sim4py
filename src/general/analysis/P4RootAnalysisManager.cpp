/**
   @file    P4RootAnalysisManager.cpp
   @author  Goro Yabu
   @date 2020/02/04
**/

#include "P4RootAnalysisManager.hpp"

#include <algorithm>
#include <string>

using std::cout;
using std::endl;

P4RootAnalysisManager * P4RootAnalysisManager::P4RootAnalysisManagerInstance = 0;

P4RootAnalysisManager * P4RootAnalysisManager::Instance()
{
    if ( P4RootAnalysisManagerInstance == 0 ) {
       P4RootAnalysisManagerInstance = new P4RootAnalysisManager();
    }
    // else
    // 	cout << "P4RootAnalysisManager is already active." << endl;
    return P4RootAnalysisManagerInstance;
} 
P4RootAnalysisManager::P4RootAnalysisManager()
    : sim4py::ParameterGene<G4RootAnalysisManager>()
{
    SetParameter<std::string>("class_name","P4RootAnalysisManager");
    P4RootAnalysisManagerInstance = this;
    cout << "P4RootAnalysisManager is activated." << endl;
}
P4RootAnalysisManager::~P4RootAnalysisManager()
{
    P4RootAnalysisManagerInstance = 0;
}

P4RootAnalysisManager::column_index P4RootAnalysisManager::GetColumnIndex(const G4String& name)
{
    auto itr = ColumnIndex.find(name);
    if ( itr == ColumnIndex.end() ) return column_index();
    return itr->second;
}
G4int P4RootAnalysisManager::AddColumnIndex(const G4String& name, const column_index& column)
{
    if ( this->GetColumnIndex(name).ntupleId != -1 ) return -1;
    ColumnIndex[name] = column; return 0;
}
G4bool P4RootAnalysisManager::ClearNtupleVector()
{
    for ( auto&& column : IColumn  ) column = 0;
    for ( auto&& column : DColumn  ) column = 0.0;
    for ( auto&& column : SColumn  ) column = "";
    for ( auto&& column : IColumnV ) column->clear();
    for ( auto&& column : DColumnV ) column->clear();
    return true;
}

G4int P4RootAnalysisManager::CreateNtupleIColumn(const G4String& name)
{
    auto index = column_index( 0, ColumnIndex.size(), 1,
			       this->int_number, IColumn.size() );
    if ( this->AddColumnIndex( name, index ) < 0 ) return -1;
    IColumn.emplace_back(0);
    return G4RootAnalysisManager::CreateNtupleIColumn(name);
}
G4int P4RootAnalysisManager::CreateNtupleDColumn(const G4String& name)
{
    auto index = column_index( 0, ColumnIndex.size(), 1,
			       this->double_number, DColumn.size() );
    if ( this->AddColumnIndex( name, index ) < 0 ) return -1;
    DColumn.emplace_back( 0.0 );
    return G4RootAnalysisManager::CreateNtupleDColumn(name);
}
G4int P4RootAnalysisManager::CreateNtupleSColumn(const G4String& name)
{
    auto index = column_index( 0, ColumnIndex.size(), 1,
			       this->string_number, SColumn.size() );
    if ( this->AddColumnIndex( name, index ) < 0 ) return -1;
    SColumn.emplace_back( "" );
    return G4RootAnalysisManager::CreateNtupleSColumn(name);    
}
G4int P4RootAnalysisManager::CreateNtupleIColumnV(const G4String& name, G4int maxSize)
{
    auto index = column_index( 0, ColumnIndex.size(), maxSize,
			       this->int_number, IColumnV.size() );
    if ( this->AddColumnIndex( name, index ) < 0 ) return -1;
    
    auto column = new std::vector<G4int>; column->reserve(maxSize);
    IColumnV.emplace_back( column );
    return this->G4RootAnalysisManager::CreateNtupleIColumn(name, *column);
}
G4int P4RootAnalysisManager::CreateNtupleDColumnV(const G4String& name, G4int maxSize)
{
    auto index = column_index( 0, ColumnIndex.size(), maxSize,
			       this->double_number, DColumnV.size() );
    if ( this->AddColumnIndex( name, index ) < 0 ) return -1;

    auto column = new std::vector<G4double>; column->reserve(maxSize);
    DColumnV.emplace_back( column );
    return this->G4RootAnalysisManager::CreateNtupleDColumn(name, *column);
}

G4bool P4RootAnalysisManager::FillNtupleIColumnName(const G4String& name, G4int value)
{
    auto index = this->GetColumnIndex( name );
    if ( index.typeNumber != this->int_number ) return false;
    if ( index.isVector!=false || index.isVariable!=false ) return false;
    return G4RootAnalysisManager::FillNtupleIColumn(index.columnId, value);
}
G4bool P4RootAnalysisManager::FillNtupleDColumnName(const G4String& name, G4double value)
{
    auto index = this->GetColumnIndex( name );
    if ( index.typeNumber != this->double_number ) return false;
    if ( index.isVector!=false || index.isVariable!=false ) return false;
    DColumn[ index.indexInType ] = value;
    return G4RootAnalysisManager::FillNtupleDColumn(index.columnId, value);
}
G4bool P4RootAnalysisManager::FillNtupleSColumnName(const G4String& name, const G4String& value)
{
    auto index = this->GetColumnIndex( name );
    if ( index.typeNumber != this->string_number ) return false;
    if ( index.isVector!=false || index.isVariable!=false ) return false;
    SColumn[ index.indexInType ] = value;
    return G4RootAnalysisManager::FillNtupleSColumn(index.columnId, value);
}
G4bool P4RootAnalysisManager::FillNtupleIColumnVName(const G4String& name, G4int value)
{
    auto index = this->GetColumnIndex( name );
    if ( index.typeNumber != this->int_number ) return false;
    if ( index.maxSize <= (int)IColumnV[ index.indexInType ]->size() ) return false;
    IColumnV[ index.indexInType ]->emplace_back( value );
    return true;
}
G4bool P4RootAnalysisManager::FillNtupleDColumnVName(const G4String& name, G4double value)
{
    auto index = this->GetColumnIndex( name );
    if ( index.typeNumber != this->double_number ) return false;
    if ( index.maxSize <= (int)DColumnV[ index.indexInType ]->size() ) return false;
    DColumnV[ index.indexInType ]->emplace_back( value );
    return true;
}

G4bool P4RootAnalysisManager::FillNtupleIColumnV(G4int columnId, G4int elementId, G4int value)
{
    if ( (int)IColumnV.size()<=columnId ) return false;
    if ( (int)IColumnV[columnId]->size()<=elementId ) return false;
    IColumnV[columnId]->at(elementId) = value;
    return true;
}

G4bool P4RootAnalysisManager::FillNtupleDColumnV(G4int columnId, G4int elementId, G4double value)
{
    if ( (int)DColumnV.size()<=columnId ) return false;
    if ( (int)DColumnV[columnId]->size()<=elementId ) return false;
    DColumnV[columnId]->at(elementId) = value;
    return true;
}

G4bool P4RootAnalysisManager::AddNtupleIColumnName(const G4String& name, G4int value)
{
    auto index = this->GetColumnIndex( name );
    if ( index.typeNumber != this->int_number ) return false;
    if ( index.isVector!=false || index.isVariable!=false ) return false;
    IColumn[ index.indexInType ] += value;
    return G4RootAnalysisManager::FillNtupleIColumn(index.columnId,
						    IColumn[index.indexInType] );
}
G4bool P4RootAnalysisManager::AddNtupleDColumnName(const G4String& name, G4double value)
{
    auto index = this->GetColumnIndex( name );
    if ( index.typeNumber != this->double_number ) return false;
    if ( index.isVector!=false || index.isVariable!=false ) return false;
    DColumn[ index.indexInType ] += value;
    return G4RootAnalysisManager::FillNtupleDColumn(index.columnId,
						    DColumn[index.indexInType] );
}
G4bool P4RootAnalysisManager::AddNtupleSColumnName(const G4String& name, const G4String& value)
{
    auto index = this->GetColumnIndex( name );
    if ( index.typeNumber != this->string_number ) return false;
    if ( index.isVector!=false || index.isVariable!=false ) return false;
    // SColumn[ index.indexInType ] += value+":";
    // auto str = std::string(value);
    SColumn[ index.indexInType ] =  SColumn[ index.indexInType ]+":"+value;
    // cout << SColumn[ index.indexInType ] << endl;
    // G4RootAnalysisManager::FillNtupleSColumn( index.columnId, "" );
    return G4RootAnalysisManager::FillNtupleSColumn
	( index.columnId, (G4String)SColumn[index.indexInType] );
}

G4int P4RootAnalysisManager::GetNtupleIColumn(const G4String& name)
{
    auto index = this->GetColumnIndex( name );
    if ( index.typeNumber != this->int_number ) return -1;
    if ( index.isVector!=false || index.isVariable!=false ) return -1;
    return IColumn[ index.indexInType ];
}
