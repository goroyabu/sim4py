#!/usr/local/bin/python3

import sys
import time
import argparse

import sim4py.general as general # 検出器によらない汎用的なクラスの入ったサブモジュール
import sim4py.CLHEP  as CLHEP  # 様々な定数があるサブモジュール
import sim4py.wstrip as wstrip # 両面ストリップ用クラスのサブモジュール

def define_detector():

    detector_construction = wstrip.W4DetectorConstruction()

    print("detector_construction is instantiated.")

    return detector_construction

def define_physics():

    factory = general.physics.G4PhysListFactory()
    physics_list = factory.GetReferencePhysList( "FTFP_BERT" )
    physics_list.SetVerboseLevel( 2 )

    #em_physics = general.physics.G4EmStandardPhysics()
    em_physics = general.physics.G4EmLivermorePhysics()
    # em_physics = general.physics.G4EmPenelopePhysics()
    # em_physics = general.physics.G4EmLowEPPhysics()

    # em_physics = wstrip.physics.W4CustomizedEmPhysics()
    #     .SetParameter( "basic_physics_constructor", "standard" )
    #     .switch_flag( "enable_doppler_broadening_effect", True )\

    physics_list.ReplacePhysics( em_physics )

    # physics_list = general.physics.P4PhysicsList()\
    #     .SetParameter( "default_cut_value", 100.0, CLHEP.nm )\
    #     .register_physics( em_constructor )
    #     .register_physics()

    print("physics_list is instantiated.")

    return physics_list

def define_action():

    #analysis = general.action.P4RootAnalysisManager.Instance()

    gps = general.action.P4GeneralParticleSource()
    gps.SetParameter( "particle", "gamma" )\
        .SetParameter( "position_type", "Plane" )\
        .SetParameter( "position_shape", "Sphere" )\
        .SetParameter( "position_center", 0.0, 0.0, 50.0, CLHEP.mm )\
        .SetParameter( "position_halfx", 0.5, CLHEP.mm )\
        .SetParameter( "position_halfy", 0.5, CLHEP.mm )\
        .SetParameter( "angle_type", "iso" )\
        .SetParameter( "angle_maxtheta", 90.0, CLHEP.deg )\
        .SetParameter( "energy_type", "Mono" )\
        .SetParameter( "mono_energy", 511.0, CLHEP.keV )

    gps.ShowParameters()

    # gun = geant4.G4ParticleGun()

    pg_action = general.action.P4PrimaryGeneratorAction()
    pg_action.SetPrimaryGenerator( gps )
        # .set_primary_generator( gun ) etc.
    pg_action.ShowParameters()

    run_action = wstrip.W4RunAction()

    action_initialization = general.action.P4ActionInitialization()
    action_initialization.RegisterUserAction( pg_action )
    action_initialization.RegisterUserAction( run_action )

    print("action_initialization is instantiated.")
    #analysis = general.action.P4RootAnalysisManager.Instance()

    return action_initialization

# def make_ana():
#
#     ana = general.action.P4RootAnalysisManager.Instance()
#     return ana

# def set(run_manager):
#     detector_construction = wstrip.W4DetectorConstruction()
#     print("set detector_construction")
#     run_manager.SetUserInitialization( detector_construction )
#     return run_manager


if __name__ == '__main__':

    #engine = CLHEP.RanecuEngine()
    #CLHEP.HepRandom.set_the_engine( engine )
    #CLHEP.HepRandom.set_the_seed( 100 );

    #run_manager = general.G4RunManager()
    run_manager = general.P4RunManager()
    run_manager.SetVerboseLevel( 2 )
    print( run_manager.GetVerboseLevel() )

    #general.action.P4RootAnalysisManager.Instance()

    #detector_construction = define_detector()
    #detector_construction = wstrip.W4DetectorConstruction.Instance()
    detector_construction = wstrip.W4DetectorConstruction.Instance()
    print("set detector_construction")
    ## -*- coding: utf-8 -*-
    #print( id(detector_construction) )
    #run_manager.SetUserInitialization( wstrip.W4DetectorConstruction())
    run_manager.SetUserInitialization( detector_construction )
    print("set ok")
    #detector_construction = 0
    #None
    #print( id(detector_construction) )
    print("none")
    #run_manager = set(run_manager)
    #physics_list = define_physics()
    #factory = general.physics.G4PhysListFactory()
    #physics_list = factory.GetReferencePhysList( "FTFP_BERT" )
    # physics_list = general.physics.FTFP_BERT()
    # physics_list.SetVerboseLevel(0)
    # em_physics = general.physics.G4EmLivermorePhysics()
    # #physics_list.ReplacePhysics( em_physics )
    # print("set physics_list")
    # run_manager.SetUserInitialization( physics_list )
    #
    # action_initialization = define_action()
    # print("set action_initialization")
    # #run_manager.SetUserInitialization( action_initialization )
    #
    # # run_manager.SetVerboseLevel( 0 )
    # print("G4RunManager::Initialize!!")
    run_manager.Initialize()
    #print("init 1")
    #run_manager.Initialize()
    #print("init 2")

    # run_manager = general.G4RunManager()\
    #     .SetUserInitialization( physics_list )\
    #     .SetUserInitialization( action_initialization )\
    #     .SetVerboseLevel( 1 )

    print("\nG4RunManager::BeamOn!!")
    run_manager.BeamOn( 10 )

    print("ENDDDDD")
    #
    print("srun")
    del run_manager

    #
    print("DELEEEETTEEE")

    # print( id(detector_construction) )
    # if detector_construction != None:
    #     print("delete")
    #     del detector_construction
    # else :
    #     print("none")
    #del physics_list
    print("sss")
    #
    # #del run_manager
    # print("end")
