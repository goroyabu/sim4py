# Project
cmake_minimum_required(VERSION 3.15)

# ----------

# ROOT
find_package(ROOT REQUIRED)

# Geant4
find_package(Geant4 REQUIRED ui_all vis_all)

# ----------

add_library(ActInit STATIC 
    ActionInitialization.cpp PrimaryGeneratorAction.cpp 
    RunAction.cpp EventAction.cpp SteppingAction.cpp AnalysisManager.cpp
)
target_include_directories(ActInit PRIVATE 
    ${ROOT_INCLUDE_DIRS}
    ${Geant4_INCLUDE_DIR}
    ../detector_construction ../physics_list 
    ../action_initialization ../user_interface
)

