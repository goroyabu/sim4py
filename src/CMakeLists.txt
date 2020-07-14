# ----- PROJECT NAME ----- #
project(sim4py CXX)

# ----- CMake ----- #
cmake_minimum_required(VERSION 3.5)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_FLAGS "-std=c++1z -Wall -g 3" "`python3 -m pybind11 --includes`")

# ----- Pybind11 ----- #
find_package(pybind11 REQUIRED)
set(PYBIND11_CPP_STANDARD "-std=c++1z")

# ----- Geant4 ----- #
find_package(Geant4 REQUIRED ui_all vis_all)
include(${Geant4_USE_FILE})
#include_directories(${Geant4_INCLUDE_DIR})

# ----- ROOT ----- #
find_package(ROOT)

#include_directories(clhep)
#add_subdirectory(clhep)

#set(SIM4_CXX_LIB_NAME sim4)

# ----- Sub Module ----- #
set(files_action general/action/P4PrimaryGeneratorAction.cpp general/action/P4ParticleGun.cpp general/action/P4GeneralParticleSource.cpp general/action/P4ActionInitialization.cpp general/action/P4EventAction.cpp general/action/P4RunAction.cpp)
set(files_analysis general/analysis/P4RootAnalysisManager.cpp)
set(files_wstrip wstrip/W4RunAction.cpp wstrip/W4EventAction.cpp wstrip/W4DetectorConstruction.cpp wstrip/W4SensitiveDetector.cpp)
set(files_detector general/detector/P4DetectorConstruction.cpp)
set(files_run general/run/P4RunManager.cpp)
#target_include_directories(action_py PRIVATE ${Geant4_INCLUDE_DIR} clhep general general/action python_interface)

# ----- Build Python Package ----- #
pybind11_add_module(${PROJECT_NAME} sim4-py.cpp clhep/clhep-py.cpp general/general-py.cpp wstrip/wstrip-py.cpp ${files_action} ${files_analysis} ${files_wstrip} ${files_detector} ${files_run})
target_link_libraries(${PROJECT_NAME} PRIVATE ${Geant4_LIBRARIES} ${ROOT_LIBRARIES})
target_include_directories(${PROJECT_NAME} PRIVATE ${Geant4_INCLUDE_DIR} ${ROOT_INCLUDE_DIRS} clhep general general/run general/action general/analysis general/detector python_interface wstrip)
install(TARGETS ${PROJECT_NAME} EXPORT ${PROJECT_NAME}-config DESTINATION lib/${PROJECT_NAME})
install(EXPORT ${PROJECT_NAME}-config DESTINATION lib/${PROJECT_NAME} EXPORT_LINK_INTERFACE_LIBRARIES)

# add_executable(sim4main main.cpp ${files_action} ${files_analysis} ${files_wstrip} ${files_detector} ${files_run})
# #add_executable(sim4main main.cpp)
# target_link_libraries(sim4main PRIVATE ${Geant4_LIBRARIES} ${ROOT_LIBRARIES} pybind11::embed)
# target_include_directories(sim4main PRIVATE ${Geant4_INCLUDE_DIR} ${ROOT_INCLUDE_DIRS} clhep general general/action general/analysis general/detector python_interface wstrip)

# ----- Uninstall ----- #
configure_file(cmake_uninstall.cmake.in cmake_uninstall.cmake IMMEDIATE @ONLY)
add_custom_target(uninstall COMMAND "${CMAKE_COMMAND}" -P cmake_uninstall.cmake)
