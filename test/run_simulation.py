#!/usr/local/bin/python3

import sys
import time
import argparse

import sim4py.general as general # 汎用的なクラスがあるサブモジュール
import sim4py.CLHEP   as CLHEP   # 単位や乱数エンジン
import sim4py.wstrip  as wstrip  # 両面ストリップ用のサブモジュール

def define_detector():

    detector_construction = wstrip.W4DetectorConstruction.Instance()
    detector_construction.AddDetectorLayer( "CdTe", 750*CLHEP.um )
    detector_construction.ShowParameters()

    print("DetectorConstruction is instantiated.")

    return detector_construction

def define_action():

    gps = general.action.P4GeneralParticleSource.Instance()
    gps.SetParameter( "verbose_level", 0 )\
        .SetParameter( "particle", "gamma" )\
        .SetParameter( "direction", 0, 0, -1 )\
        .SetParameter( "pos_type", "Volume" )\
        .SetParameter( "pos_shape", "Sphere" )\
        .SetParameter( "pos_centre", 0.0, 0.0, 50.0, CLHEP.mm )\
        .SetParameter( "pos_radius", 0.5, CLHEP.mm )\
        .SetParameter( "ang_type", "iso" )\
        .SetParameter( "ang_mintheta", 0.0, CLHEP.deg )\
        .SetParameter( "ang_maxtheta", 45.0, CLHEP.deg )\
        .SetParameter( "ene_type", "Mono" )\
        .SetParameter( "ene_mono", 123.4, CLHEP.keV )
        # .SwitchFlag( "use_ion", False )\
        # .SetParameter( "ion_atomic_number", 49 )\
        # .SetParameter( "ion_atomic_mass", 111 )\
        # .SetParameter( "ion_charge", 49 )\
        # .SetParameter( "ion_excitation", 0.0, CLHEP.MeV )\
    gps.ShowParameters()
    #
    # gun = general.action.P4ParticleGun.Instance()
    # gun.SetParameter( "particle", "ion" )\
    #     .SetParameter( "energy", 0.0, CLHEP.keV )\
    #     .SetParameter( "position", 0.0, 0.0, 0.0, CLHEP.mm )\
    #     .SetParameter( "direction", -1.0, 0.0, 0.0, CLHEP.mm )\
    #     .SwitchFlag( "use_ion", True )\
    #     .SetParameter( "ion_atomic_number", 49 )\
    #     .SetParameter( "ion_atomic_mass", 111 )\
    #     .SetParameter( "ion_charge", 49 )\
    #     .SetParameter( "ion_excitation", 0.0, CLHEP.MeV )
    # gun.ShowParameters()

    pg_action = general.action.P4PrimaryGeneratorAction.Instance()
    pg_action.SetParameter( "verbose_level", 1 )
    pg_action.SetPrimaryGenerator( gps )
    pg_action.ShowParameters()

    run_action = wstrip.W4RunAction.Instance()
    run_action.SetParameter("file_name", "test.root")
    run_action.ShowParameters()

    event_action = wstrip.W4EventAction.Instance()
    event_action.ShowParameters()

    action_initialization = general.action.P4ActionInitialization.Instance()
    action_initialization.RegisterUserAction( pg_action )
    action_initialization.RegisterUserAction( run_action )
    action_initialization.RegisterUserAction( event_action )
    action_initialization.ShowParameters()

    print("ActionInitialization is instantiated.")

    return action_initialization

if __name__ == '__main__':

    # engine = CLHEP.RanecuEngine()
    # CLHEP.HepRandom.setTheEngine( engine )
    # CLHEP.HepRandom.setTheSeed( 100 );

    run_manager = general.P4RunManager()
    run_manager.SetParameter("verbose_level", 1)\
        .SetParameter("physlist_verbose_level", 0)

    detector_construction = define_detector()
    run_manager.SetUserInitialization( detector_construction )

    run_manager.SetReferencePhysicsList( "FTFP_BERT" )

    # my_physics_list = ["G4EmStandardPhysics", "G4EmExtraPhysics", "G4DecayPhysics", "G4HadronElasticPhysics",\
    # "G4HadronPhysicsFTFP_BERT", "G4StoppingPhysics", "G4IonPhysics", "G4NeutronTrackingCut"]
    # run_manager.SetGenericPhysicsList( my_physics_list )

    action_initialization = define_action()
    run_manager.SetUserInitialization( action_initialization )

    run_manager.ShowParameters()

    print("\nRunManager::BeamOn!!")
    run_manager.BeamOn( 100000 )
