#!/usr/local/bin/python3

import sys
import time
import argparse

import sim4py.general as general
import sim4py.CLHEP   as CLHEP
import sim4py.wstrip  as wstrip

import user_geometry1 as ug

def usage():

    parser = argparse.ArgumentParser()
    parser.add_argument("--visualize","-v",action='store_true')
    parser.add_argument("--nevents", "-n", type=int, default=100)
    return parser

def define_detector():

    detector_construction = general.detector.P4DetectorConstruction.Instance()

    user_volume = ug.UserVolume.Instance()
    
    detector_construction.AddVolume( user_volume )

    detector_construction.ShowParameters()

    print("DetectorConstruction is instantiated.")

    return detector_construction

def define_action():

    # gps = general.action.P4GeneralParticleSource.Instance()
    # gps.SetParameter( "verbose_level", 0 )\
    #     .SetParameter( "particle", "gamma" )\
    #     .SetParameter( "direction", 0, 0, -1 )\
    #     .SetParameter( "pos_type", "Volume" )\
    #     .SetParameter( "pos_shape", "Sphere" )\
    #     .SetParameter( "pos_centre", 0.0, 0.0, 41.35, CLHEP.mm )\
    #     .SetParameter( "pos_radius", 0.5, CLHEP.mm )\
    #     .SetParameter( "ang_type", "iso" )\
    #     .SetParameter( "ang_mintheta", 0.0, CLHEP.deg )\
    #     .SetParameter( "ang_maxtheta", 0.0, CLHEP.deg )\
    #     .SetParameter( "ene_type", "Mono" )\
    #     .SetParameter( "ene_mono", 123.4, CLHEP.keV )

    # gps.SetParameter( "particle", "ion")\
    #     .SwitchFlag( "use_ion", True )\
    #     .SetParameter( "ion_atomic_number", 49 )\
    #     .SetParameter( "ion_atomic_mass", 111 )\
    #     .SetParameter( "ion_charge", 49 )\
    #     .SetParameter( "ion_excitation", 0.0, CLHEP.MeV )

    # gps.ShowParameters()

    gun = general.action.P4ParticleGun.Instance()
    gun.SetParameter( "particle", "gamma" )\
        .SetParameter( "energy", 100.0, CLHEP.keV )\
        .SetParameter( "position", 0.0, 0.0, 0.0, CLHEP.mm )\
        .SetParameter( "direction", 0.0, 0.0, 1.0 )

    gun.ShowParameters()

    pg_action = general.action.P4PrimaryGeneratorAction.Instance()
    pg_action.SetParameter( "verbose_level", 0 )
    pg_action.SetPrimaryGenerator( gun )
    pg_action.ShowParameters()

    run_action = wstrip.W4RunAction.Instance()
    run_action.SetParameter( "file_name", "test.root" )
    run_action.ShowParameters()

    event_action = wstrip.W4EventAction.Instance()
    event_action.SetParameter( "minimum_nhits_to_save", 1 )\
        .SetParameter( "print_frequency", 100 )
    event_action.ShowParameters()

    action_initialization = general.action.P4ActionInitialization.Instance()
    action_initialization.RegisterUserAction( pg_action )
    action_initialization.RegisterUserAction( run_action )
    action_initialization.RegisterUserAction( event_action )
    action_initialization.ShowParameters()

    print("ActionInitialization is instantiated.")

    return action_initialization

if __name__ == '__main__':

    parser = usage()
    args = parser.parse_args()

    engine = CLHEP.RanecuEngine()
    CLHEP.HepRandom.setTheEngine( engine )
    CLHEP.HepRandom.setTheSeed( int(time.time()*1000) );

    run_manager = general.P4RunManager()
    run_manager.SetParameter( "verbose_level", 0 )\
        .SetParameter( "physlist_verbose_level", 0 )\
        .SetParameter( "cut_value", 100, CLHEP.um )\
        .SwitchFlag( "visualize", args.visualize )

    detector_construction = define_detector()
    #detector_construction = wstrip.W4DetectorConstruction.Instance()
    #print(detector_construction)

    #det2 = wstrip.W4DetectorConstruction.Instance()
    #print(det2)
    run_manager.SetUserInitialization( detector_construction )

    run_manager.UseReferencePhysicsList( "FTFP_BERT" )
    # my_physics_list = ["G4EmStandardPhysics", "G4EmExtraPhysics", "G4DecayPhysics", "G4HadronElasticPhysics",\
    # "G4HadronPhysicsFTFP_BERT", "G4StoppingPhysics", "G4IonPhysics", "G4NeutronTrackingCut"]
    #my_physics_list = ["W4EmStandardPhysicsWithDoppler", "G4EmExtraPhysics", "G4DecayPhysics", "G4HadronElasticPhysics",\
    #"G4HadronPhysicsFTFP_BERT", "G4StoppingPhysics", "G4IonPhysics", "G4NeutronTrackingCut"]
    #run_manager.UseGenericPhysicsList( my_physics_list )

    action_initialization = define_action()
    run_manager.SetUserInitialization( action_initialization )

    run_manager.ShowParameters()

    print("\nRunManager::BeamOn!!")

    if args.visualize == True :
        run_manager.BeamOn( 100 )
    else :
        run_manager.BeamOn( args.nevents )
