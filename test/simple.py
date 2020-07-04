#!/usr/local/bin/python3

import sys
import sim4py.general as general

if __name__ == '__main__':

    run_manager = general.P4RunManager()
    run_manager.SetVerboseLevel( 3 )

    run_manager.Initialize()

    print("\nG4RunManager::BeamOn!!")
    run_manager.BeamOn( 10 )

    print("ENDDDDD")

    del run_manager

    print("DELEEEETTEEE")
