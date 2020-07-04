#include <P4RunManager.hpp>
#include <P4DetectorConstruction.hpp>
#include <P4PrimaryGeneratorAction.hpp>
#include <P4GeneralParticleSource.hpp>
#include <P4ActionInitialization.hpp>
#include <P4RunAction.hpp>

#include <G4RunManager.hh>
#include <G4PhysListFactory.hh>

#include <iostream>

int main()
{
    using std::cout;
    using std::endl;

    auto run_manager = new P4RunManager();
    //auto run_manager = new G4RunManager();
    run_manager->SetVerboseLevel( 3 );

    // G4PhysListFactory factory;
    // auto phys = factory.GetReferencePhysList( "FTFP_BERT" );
    // run_manager->SetUserInitialization( phys );

    // auto det = new P4DetectorConstruction();
    // run_manager->SetUserInitialization( det );
    
    //auto pga = new P4PrimaryGeneratorAction();
    //pga->SetPrimaryGenerator(new P4GeneralParticleSource);
    //auto act = new P4ActionInitialization();
    //act->RegisterUserAction(pga);
    //run_manager->SetUserInitialization(act);

    // run_manager->SetUserAction( new P4PrimaryGeneratorAction );
    // run_manager->SetUserAction( new P4RunAction );
    
    cout << "Initialize" << endl;
    run_manager->Initialize();
    
    cout << "BeamOn" << endl;
    run_manager->BeamOn( 10 );
    
    cout << "delete" << endl;
    delete run_manager;

    cout << "end" << endl;
    return 0;
}
