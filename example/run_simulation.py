#!/usr/bin/env python

import argparse
import sim4py


def usage():

    parser = argparse.ArgumentParser(
        formatter_class=argparse.ArgumentDefaultsHelpFormatter
    )

    parser.add_argument(
        "-m", "--macro", type=str,
    )

    parser.add_argument(
        "-v", "--visualize", action="store_true",
    )

    parser.add_argument(
        "-e", "--energy", type=float, default=511.0,
    )

    parser.add_argument(
        "-n", "--nevents", type=int, default=10000,
    )

    return parser

def geometry():

    return """
##################################################
### Geometry
##################################################

/wstrip/geom/setLengthOf  worldSize      15    cm

/wstrip/geom/setLengthOf  detectorSize   2     mm
/wstrip/geom/setLengthOf  detectorGap    4     mm

/wstrip/geom/setPositionOf detectorCenter 0. 0. 0. mm
/wstrip/geom/setNumberOf xaxisNbins 100
/wstrip/geom/setNumberOf yaxisNbins 100

/wstrip/geom/addDetectorLayer Si 500 um
#/wstrip/geom/addDetectorLayer CdTe 2000 um
"""

def initialize():
    return """
##################################################
## Initialuze
##################################################

/run/initialize
"""

def particle_gun():
    return """
##################################################
### ParticleGun 
##################################################

/wstrip/prim/useParticleGun

/gun/particle  gamma
/gun/energy    511.0 keV
/gun/position  0. 0. 1. mm
/gun/direction 0. 0. -1.
"""

def point_source(args):
    return f"""
##################################################
### GeneralParticleSource
##################################################

/wstrip/prim/useGPS

### Point Source
/gps/particle gamma
#/gps/particle e-
/gps/pos/type Volume
/gps/pos/shape Sphere
/gps/pos/centre 0. 0. 0. mm
#/gps/pos/halfx 30 mm
#/gps/pos/halfy 30 mm
/gps/pos/radius 30 mm
#/gps/pos/radius 0.1 mm
/gps/ang/type iso
/gps/ang/type focused
#/gps/ang/mintheta 180 deg
#/gps/ang/maxtheta 180 deg
/gps/ang/focuspoint 0. 0. 0. mm
/gps/ene/type Mono
/gps/ene/mono {args.energy} keV
#/gps/ene/min 511.0 keV
#/gps/ene/max 511.0 keV
"""

def inside_of_sphere(args):
    return f"""
##################################################
### GeneralParticleSource
##################################################

/wstrip/prim/useGPS

/gps/particle gamma
/gps/pos/type Volume
/gps/pos/shape Sphere
/gps/pos/centre 0. 0. 0. mm
/gps/pos/radius 30 mm
/gps/ang/type focused
/gps/ang/focuspoint 0. 0. 0. mm
/gps/ene/type Mono
/gps/ene/mono {args.energy} keV
"""

def beam():
    return """
##################################################
### GeneralParticleSource
##################################################

/wstrip/prim/useGPS

### Beam
/gps/particle gamma
/gps/pos/type Plane
/gps/pos/shape Circle
/gps/pos/centre 0. 0. 50. mm
/gps/pos/radius       25 mm
#/gps/pos/halfx 25 mm
#/gps/pos/halfy 25 mm
/gps/ang/type iso
/gps/ang/maxtheta 0 deg
/gps/ene/type Mono
/gps/ene/mono 511.0 keV
"""

def visualize():
    return """
##################################################
### Visualize
##################################################

/vis/open OGL 600x600-0+0
/vis/viewer/set/autoRefresh false
/vis/verbose errors

/vis/drawVolume

/vis/viewer/set/viewpointThetaPhi 90. 0.
/vis/viewer/zoom 1.4

/vis/scene/add/axes 0 0 0 10 cm
/vis/scene/add/trajectories smooth
/vis/modeling/trajectories/create/drawByCharge
/vis/modeling/trajectories/drawByCharge-0/default/setDrawStepPts true
/vis/modeling/trajectories/drawByCharge-0/default/setStepPtsSize 2

/vis/scene/add/hits

/vis/filtering/trajectories/create/particleFilter
/vis/filtering/trajectories/particleFilter-0/add gamma
/vis/filtering/trajectories/particleFilter-0/add e-
/vis/filtering/trajectories/particleFilter-0/add ion
/vis/modeling/trajectories/create/drawByParticleID
/vis/modeling/trajectories/drawByParticleID-0/set e- blue
/vis/modeling/trajectories/drawByParticleID-0/set ion red

/vis/scene/endOfEventAction accumulate
/vis/viewer/set/autoRefresh true
/vis/verbose warnings
/vis/viewer/flush
"""

def run(args,cut_value=100,dr_over_range=0.01,final_range=0.1):
    return f"""
##################################################
### Analysis
##################################################

/analysis/setFileName test.root
/analysis/h1/set 0  1000 0. 1000. keV 

##################################################
### Run
##################################################

/run/setCut {cut_value} nm
/process/eLoss/StepFunction {dr_over_range} {final_range} nm
/run/beamOn {args.nevents}
"""

if __name__ == "__main__":

    parser = usage()
    args = parser.parse_args()

    command_list = []

    command_list.append( geometry() )
    command_list.append( initialize() )
    # command_list.append( point_source(args) )
    command_list.append( inside_of_sphere(args) )

    if args.visualize == True :
        command_list.append( visualize() )
    
    command_list.append( run(args) )

    sim4py.maincxx(
        command_list=command_list,
        use_gui=args.visualize
        )

    filename = "sim%Y%m%d_%H%M%S.root"

        # G4EmExtraParametersMessenger.cc
        # sannshou     
        # StepFuncCmd = new G4UIcommand("/process/eLoss/StepFunction",this);
#   StepFuncCmd->SetGuidance("Set the energy loss step limitation parameters for e+-.");
#   StepFuncCmd->SetGuidance("  dRoverR   : max Range variation per step");
#   StepFuncCmd->SetGuidance("  finalRange: range for final step");
#   StepFuncCmd->SetGuidance("  unit      : unit of finalRange");
#   StepFuncCmd->AvailableForStates(G4State_PreInit,G4State_Idle);   