# /*** @author Goro Yabu ***/
# /*** @date  2021/06/17 ***/

# Project 
project(sim4py CXX)
cmake_minimum_required(VERSION 3.15)

# Uninstallation
configure_file(cmake_uninstall.cmake.in cmake_uninstall.cmake IMMEDIATE @ONLY)
add_custom_target(uninstall COMMAND "${CMAKE_COMMAND}" -P cmake_uninstall.cmake)

# ----------

# C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_FLAGS "-std=c++1z -Wall")

# ----------

# Pybind11 
find_package(pybind11 REQUIRED)
set(PYBIND11_CPP_STANDARD "-std=c++1z")

# ----------

# ROOT
find_package(ROOT REQUIRED)
message(STATUS "ROOT_LIBRARIES=${ROOT_LIBRARIES}")
message(STATUS "ROOT_INCLUDE_DIRS=${ROOT_INCLUDE_DIRS}")
#target_include_directories(${ROOT_INCLUDE_DIRS})

# Geant4
# Find Geant4 package
option(WITH_GEANT4_UIVIS "Build example with Geant4 UI and Vis drivers" ON)
if(WITH_GEANT4_UIVIS)
  find_package(Geant4 REQUIRED ui_all vis_all)
else()
  find_package(Geant4 REQUIRED)
endif()
message(STATUS "WITH_GEANT4_UIVIS=${WITH_GEANT4_UIVIS}")

include(${Geant4_USE_FILE})
message(STATUS "Geant4_USE_FILE=${Geant4_USE_FILE}")
message(STATUS "Geant4_LIBRARIES=${Geant4_LIBRARIES}")
message(STATUS "Geant4_INCLUDE_DIR=${Geant4_INCLUDE_DIR}")
# include_directories(${Geant4_INCLUDE_DIR})

# ----------

pybind11_add_module(sim4py
  pymain.cpp
  )
target_link_libraries(sim4py PRIVATE 
    # ${ROOT_LIBRARIES}
    ${Geant4_LIBRARIES} 
    DetConst PhysList ActInit UsrInt
)
target_include_directories(sim4py PRIVATE
    ${ROOT_INCLUDE_DIRS}
    ${Geant4_INCLUDE_DIR} 
    ${CMAKE_CURRENT_SOURCE_DIR}/detector_construction 
    ${CMAKE_CURRENT_SOURCE_DIR}/physics_list 
    ${CMAKE_CURRENT_SOURCE_DIR}/action_initialization 
    ${CMAKE_CURRENT_SOURCE_DIR}/user_interface
)
install(TARGETS ${PROJECT_NAME} EXPORT ${PROJECT_NAME}-config DESTINATION lib/${PROJECT_NAME})
install(EXPORT ${PROJECT_NAME}-config DESTINATION lib/${PROJECT_NAME} EXPORT_LINK_INTERFACE_LIBRARIES)

# Build 
add_executable(simtestcxx main.cpp)
target_link_libraries(simtestcxx PRIVATE
    # ${ROOT_LIBRARIES} 
    ${Geant4_LIBRARIES} 
    DetConst PhysList ActInit UsrInt
)
target_include_directories(simtestcxx PRIVATE
    # ${ROOT_INCLUDE_DIRS}
    ${Geant4_INCLUDE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/detector_construction 
    ${CMAKE_CURRENT_SOURCE_DIR}/physics_list 
    ${CMAKE_CURRENT_SOURCE_DIR}/action_initialization 
    ${CMAKE_CURRENT_SOURCE_DIR}/user_interface
)
# install(TARGETS ${PROJECT_NAME} DESTINATION bin)

# set(common_include_dirs
#     ${ROOT_INCLUDE_DIRS}
#     ${Geant4_INCLUDE_DIR}
#     ${CMAKE_CURRENT_SOURCE_DIR}/detector_construction 
#     ${CMAKE_CURRENT_SOURCE_DIR}/physics_list 
#     ${CMAKE_CURRENT_SOURCE_DIR}/action_initialization 
#     ${CMAKE_CURRENT_SOURCE_DIR}/user_interface
# )

# ----------

set(detconst_source_paths)
set(detconst_source_files
    DetectorConstruction.cpp SensitiveDetector.cpp HitsCollection.cpp
)
foreach(filename IN LISTS detconst_source_files)
    list(APPEND detconst_source_paths
        ${CMAKE_CURRENT_SOURCE_DIR}/detector_construction/${filename})
endforeach()

add_library(DetConst SHARED
    ${detconst_source_paths}
)
target_link_libraries(DetConst PRIVATE
    ${ROOT_LIBRARIES} 
    ${Geant4_LIBRARIES}
    PhysList ActInit
)
target_include_directories(DetConst PRIVATE
    ${Geant4_INCLUDE_DIR}
    ${ROOT_INCLUDE_DIRS}
    ${CMAKE_CURRENT_SOURCE_DIR}/detector_construction
    ${CMAKE_CURRENT_SOURCE_DIR}/action_initialization
)

# ----------

set(physlist_source_paths)
set(physlist_source_files
    PhysicsList.cpp  PhysicsConstructor.cpp
)
foreach(filename IN LISTS physlist_source_files)
    list(APPEND physlist_source_paths
        ${CMAKE_CURRENT_SOURCE_DIR}/physics_list/${filename})
endforeach()

add_library(PhysList SHARED
    ${physlist_source_paths}
)
target_link_libraries(PhysList PRIVATE
    ${Geant4_LIBRARIES}
)
target_include_directories(PhysList PRIVATE
    ${Geant4_INCLUDE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/physics_list
)

# ----------

set(actinit_source_paths)
set(actinit_source_files
    ActionInitialization.cpp PrimaryGeneratorAction.cpp 
    RunAction.cpp EventAction.cpp SteppingAction.cpp AnalysisManager.cpp
)
foreach(filename IN LISTS actinit_source_files)
    list(APPEND actinit_source_paths
        ${CMAKE_CURRENT_SOURCE_DIR}/action_initialization/${filename})
endforeach()

add_library(ActInit SHARED 
    ${actinit_source_paths}
)
target_link_libraries(ActInit PRIVATE
    ${Geant4_LIBRARIES}
)
target_include_directories(ActInit PRIVATE 
    ${Geant4_INCLUDE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/action_initialization
    ${CMAKE_CURRENT_SOURCE_DIR}/detector_construction
)

# ----------

set(usrint_source_paths)
set(usrint_source_files
    UImessenger.cpp UIcmdWithAKeyAValueAndUnit.cpp 
    UIcmdWithCustomizableArguments.cpp
)
foreach(filename IN LISTS usrint_source_files)
    list(APPEND usrint_source_paths
        ${CMAKE_CURRENT_SOURCE_DIR}/user_interface/${filename})
endforeach()

add_library(UsrInt SHARED 
    ${usrint_source_paths}
)
target_link_libraries(UsrInt PRIVATE
    ${Geant4_LIBRARIES}
    DetConst PhysList ActInit
)
target_include_directories(UsrInt PRIVATE
    ${Geant4_INCLUDE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/detector_construction
    ${CMAKE_CURRENT_SOURCE_DIR}/physics_list
    ${CMAKE_CURRENT_SOURCE_DIR}/action_initialization
    ${CMAKE_CURRENT_SOURCE_DIR}/user_interface
)


