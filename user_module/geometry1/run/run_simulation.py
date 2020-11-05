#!/usr/local/bin/python3

import os
import sys
import time
import argparse

import sim4py.general as general # 汎用的なクラスがあるサブモジュール
import sim4py.CLHEP   as CLHEP   # 単位や乱数エンジン
import sim4py.wstrip  as wstrip  # 両面ストリップ用のサブモジュール

import user_geometry1 as ug

def usage():

    parser = argparse.ArgumentParser( description="Monte-Carlo simulator of e-track on Si",\
        formatter_class=argparse.ArgumentDefaultsHelpFormatter )

    parser.add_argument( "-e", "--incident-energy", metavar='E', type=float,\
        help="energy of incident gamma-rays [keV]", default=511.0 )
    parser.add_argument( "-n", "--nevents", metavar='N', type=int,\
        help="number of events", default=1000 )

    parser.add_argument( '--source-shape', choices=['Sphere','Point'], default='Sphere',\
        help='shape of source')
    parser.add_argument( '--source-halfwidth', type=float, default=1.0,\
        help='radius or halfwidth of point source [mm]' )
    # parser.add_argument( '--source-distance', metavar='DISTANCE', type=float, default=41.35,\
    #     help='distance from source to the first detector plane [mm]')

    parser.add_argument( "--ion", action="store_true", help='use radioactive nuclide as a source' )
    parser.add_argument( "--source", choices=['22Na', '111In', '131I', '57Co', '241Am', '133Ba', '137Cs'],\
        help='nuclide name', default='111In' )

    parser.add_argument( '--board-material', choices=['SiO2','Al','Cu','None'],\
        help='material of detector board', default='None' )

    parser.add_argument( '--merging-mode', choices=['any-merge-disabled','only-same-pixels','include-adjacent-pixels'],\
        default='any-merge-disabled', help='switch merging mode' )
    parser.add_argument( '--set-cut', metavar='CUT', type=float, default=100.0,\
        help='cut value[um]')
    parser.add_argument( '--dr-over-range', metavar='F1', type=float, default=0.2,\
        help='no dimension')
    parser.add_argument( '--final-range', metavar='F2', type=float, default=1000000,\
        help='[nm]' )

    parser.add_argument( '-g', '--graphical-interface', action='store_true' )
    parser.add_argument( '-v', '--verbose', action='count', default=0 )
    parser.add_argument( '--dry-run', action='store_true' )

    return parser

def define_detector(args):

    dsd = wstrip.W4DoublesidedStripDetector.Instance()\
        .SetParameter( "verbose_level", args.verbose )\
        .SetParameter( "detector_size", 3.2, CLHEP.cm )\
        .SetParameter( 'npixels_xside', 128 )\
        .SetParameter( 'npixels_yside', 128 )\
        .SetParameter( "detector_center", 0, 0, 0.0, CLHEP.mm )\
        .SetParameter( "detector_normal", 0, 0, -1.0 )
        # .SetParameter( "detector_gap", args.gap, CLHEP.mm )\
        # .SetParameter( 'gap_of_abso_and_scat', args.gap_of_stoa, CLHEP.mm )\
        # .SetParameter( 'detid_max_of_scatterer', args.nscats-1 )

    dsd.AddDetectorLayer( "CdTe", 750*CLHEP.um )\
        .SwitchFlag( "use_detector_board", False )

    if args.merging_mode == 'only-same-pixels' :
        dsd.SwitchFlag( "merge_same_pixel",     True  )
        dsd.SwitchFlag( "merge_adjacent_pixel", False )
    elif args.merging_mode == 'include-adjacent-pixels' :
        dsd.SwitchFlag( "merge_same_pixel",     True  )
        dsd.SwitchFlag( "merge_adjacent_pixel", True  )
    else :
        dsd.SwitchFlag( "merge_same_pixel",     False )
        dsd.SwitchFlag( "merge_adjacent_pixel", False )

    if args.board_material!='None' :
        dsd.SwitchFlag( "use_detector_board", True )\
            .SetParameter( "board_material", args.board_material )\
            .SetParameter( 'board_size', 52, CLHEP.mm )

    dsd.ShowParameters()

    # box = wstrip.W4AirContainBox.Instance()\
    #     .SetParameter( "box_material", "Aluminum" )\
    #     .SetParameter( "box_center", 0, 0, 119, CLHEP.mm)\
    #     .SetParameter( "box_length_x",    310,  CLHEP.mm )\
    #     .SetParameter( "box_length_y",    200,  CLHEP.mm )\
    #     .SetParameter( "box_length_z",    190,  CLHEP.mm )\
    #     .SetParameter( "box_thickness_x", 1,    CLHEP.mm )\
    #     .SetParameter( "box_thickness_y", 6,    CLHEP.mm )\
    #     .SetParameter( "box_thickness_z", 1,    CLHEP.mm )
    # box.ShowParameters()

    user_volume = ug.UserVolume.Instance()
    user_volume.ShowParameters()

    detector_construction = general.detector.P4DetectorConstruction.Instance()\
        .SetParameter( "world_size", 300, CLHEP.cm )

    detector_construction.AddVolume( dsd )
    detector_construction.AddVolume( user_volume )
    # detector_construction.AddVolume( box )

    detector_construction.ShowParameters()

    return detector_construction

def define_action( file_name, args ):

    gps = general.action.P4GeneralParticleSource.Instance()

    gps.SetParameter( "verbose_level", args.verbose )\
        .SetParameter( "direction", 0.0, 0.0, 1.0 )\
        .SetParameter( "pos_centre", 0.0, 0.0, -100.0, CLHEP.mm )\
        .SetParameter( "ang_type", "iso" )\
        .SetParameter( "ang_mintheta", 150.0, CLHEP.deg )\
        .SetParameter( "ang_maxtheta", 180.0, CLHEP.deg )\
        .SetParameter( "particle", "gamma")

    if args.source_shape == 'Sphere' :
        gps.SetParameter( "pos_type", "Volume" )\
            .SetParameter( "pos_shape", "Sphere" )\
            .SetParameter( "pos_radius", args.source_halfwidth, CLHEP.mm )
    elif args.source_shape == 'Point' :
        gps.SetParameter( "pos_type", "Point" )\
            .SetParameter( "pos_shape", "Sphere" )\
            .SetParameter( "pos_radius", 0.001, CLHEP.mm )
    elif args.source_shape == 'Square' :
        gps.SetParameter( "pos_type", "Plane" )\
            .SetParameter( "pos_shape", "Square" )\
            .SetParameter( "pos_halfx", args.source_halfwidth, CLHEP.mm )\
            .SetParameter( "pos_halfy", args.source_halfwidth, CLHEP.mm )

    if args.ion == True:

        ion_atomic_number = 49
        ion_atomic_mass = 111
        if args.source == '22Na':
            ion_atomic_number = 11
            ion_atomic_mass = 22
        elif args.source == '111In':
            ion_atomic_number = 49
            ion_atomic_mass = 111
        elif args.source == '131I':
            ion_atomic_number = 53
            ion_atomic_mass = 131
        elif args.source == '57Co':
            ion_atomic_number = 27
            ion_atomic_mass = 57
        elif args.source == '241Am':
            ion_atomic_number = 95
            ion_atomic_mass = 241
        elif args.source == '133Ba':
            ion_atomic_number = 56
            ion_atomic_mass = 133
        elif args.source == '137Cs' :
            ion_atomic_number = 55
            ion_atomic_mass = 137

        gps.SetParameter( "particle", "ion")\
            .SwitchFlag( "use_ion", True )\
            .SetParameter( "ion_atomic_number", ion_atomic_number )\
            .SetParameter( "ion_atomic_mass", ion_atomic_mass )\
            .SetParameter( "ion_charge", 0 )\
            .SetParameter( "ion_excitation", 0.0, CLHEP.MeV )\
            .SetParameter( "time", 0.0, CLHEP.ns )
    else :
        gps.SetParameter( "ene_type", "Mono" )\
            .SetParameter( "ene_mono", args.incident_energy, CLHEP.keV )

    gps.ShowParameters()

    # gun = general.action.P4ParticleGun.Instance()
    # gun.SetParameter( "particle", "gamma" )\
    #     .SetParameter( "energy", args.incident_energy, CLHEP.keV )\
    #     .SetParameter( "position", 0.0, 0.0, -50.0, CLHEP.mm )\
    #     .SetParameter( "direction", 0.0, 0.0, 1.0 )

    # gun.SetParameter( "particle", "ion" )\
    #     .SetParameter( "position", 0.0, 0.0, -50.0, CLHEP.mm )\
    #     .SetParameter( "direction", 0.0, 0.0, 1.0 )\
    #     .SwitchFlag( "use_ion", True )\
    #     .SetParameter( "ion_atomic_number", ion_atomic_number )\
    #     .SetParameter( "ion_atomic_mass", ion_atomic_mass )\
    #     .SetParameter( "ion_charge", 0 )\
    #     .SetParameter( "ion_excitation", 0.0, CLHEP.MeV )\
    #     .SetParameter( "number", 100 )

    # gun.ShowParameters()

    pg_action = general.action.P4PrimaryGeneratorAction.Instance()
    pg_action.SetParameter( "verbose_level", args.verbose )
    pg_action.SetPrimaryGenerator( gps )
    # pg_action.SetPrimaryGenerator( gun )
    pg_action.ShowParameters()

    run_action = wstrip.W4RunAction.Instance()
    run_action.SetParameter( "file_name", file_name )
    run_action.ShowParameters()

    event_action = wstrip.W4EventAction.Instance()
    event_action.SetParameter( "minimum_nhits_to_save", 2 )\
        .SetParameter( "print_frequency", 10000 )
    event_action.ShowParameters()

    stepping_action = wstrip.W4SteppingAction.Instance()
    stepping_action.ShowParameters()

    action_initialization = general.action.P4ActionInitialization.Instance()
    action_initialization.RegisterUserAction( pg_action )
    action_initialization.RegisterUserAction( run_action )
    action_initialization.RegisterUserAction( event_action )
    action_initialization.RegisterUserAction( stepping_action )
    action_initialization.ShowParameters()

    print("ActionInitialization is instantiated.")

    return action_initialization

if __name__ == '__main__':

    #outdir="g4raw/"
    #os.makedirs( outdir, exist_ok=True )

    parser = usage()
    args = parser.parse_args()

    file_name = 'test.root'
    # if args.ion == False :
    #     file_name = outdir+'sim_'+'{:.0f}keV'.format( args.incident_energy )+'_%y%m%d_%H%M%S.root'
    # else :
    #     file_name = outdir+'sim_'+args.source+'_%y%m%d_%H%M%S.root'

    if args.merging_mode == 'only-same-pixels' :
        file_name = file_name.replace( '.root', '_merge1.root' )
    elif args.merging_mode == 'include-adjacent-pixels' :
        file_name = file_name.replace( '.root', '_merge2.root' )
    else :
        file_name = file_name.replace( '.root', '_merge0.root' )

    engine = CLHEP.RanecuEngine()
    CLHEP.HepRandom.setTheEngine( engine )
    CLHEP.HepRandom.setTheSeed( int(time.time()*1000) );

    run_manager = general.P4RunManager()
    run_manager.SetParameter( "verbose_level", args.verbose )\
        .SetParameter( "physlist_verbose_level", args.verbose )\
        .SetParameter( "cut_value", args.set_cut, CLHEP.nm )\
        .SetParameter( 'dr_over_range', args.dr_over_range )\
        .SetParameter( 'final_range', args.final_range, CLHEP.nm )\
        .SwitchFlag( "visualize", args.graphical_interface )

    # Define Geometry
    detector_construction = define_detector(args)
    run_manager.SetUserInitialization( detector_construction )
    #

    # Define Physics
    if args.graphical_interface :
        run_manager.UseReferencePhysicsList( "FTFP_BERT" )
    else:
        my_physics_list = ["G4EmLivermorePhysics", "G4EmExtraPhysics", "G4DecayPhysics",\
            "G4RadioactiveDecayPhysics", "G4HadronElasticPhysics", "G4HadronPhysicsFTFP_BERT",\
            "G4StoppingPhysics", "G4IonPhysics", "G4NeutronTrackingCut" ]
        # run_manager.UseReferencePhysicsList( "FTFP_BERT_LIV" )
        run_manager.UseGenericPhysicsList( my_physics_list )
    #

    # Define Action
    action_initialization = define_action( file_name, args )
    run_manager.SetUserInitialization( action_initialization )
    #

    run_manager.ShowParameters()

    if args.dry_run == False:
        print("\nRunManager::BeamOn!!")
        start = time.time()
        run_manager.BeamOn( args.nevents )
        end = time.time()
        print(end-start,"[sec]")
