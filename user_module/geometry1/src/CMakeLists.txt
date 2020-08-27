# ----- PROJECT NAME ----- #
project(user_geometry1 CXX)

# ----- CMake ----- #
cmake_minimum_required(VERSION 3.5)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_FLAGS "-std=c++1z -Wall")

# ----- Pybind11 ----- #
find_package(pybind11 REQUIRED)
set(PYBIND11_CPP_STANDARD "-std=c++1z")

# ----- Geant4 ----- #
find_package(Geant4 REQUIRED ui_all vis_all)
include(${Geant4_USE_FILE})

# ----- ROOT ----- #
find_package(ROOT)

# ----- sim4py ----- #
find_package(sim4pycxx)

# ----- Build Python Package ----- #
pybind11_add_module( ${PROJECT_NAME}
  user_geometry1.cpp
  UserVolume.cpp
  )
target_link_libraries( ${PROJECT_NAME} PRIVATE ${Geant4_LIBRARIES} ${ROOT_LIBRARIES} sim4pycxx)
target_include_directories( ${PROJECT_NAME} PRIVATE ${Geant4_INCLUDE_DIR} ${ROOT_INCLUDE_DIRS} src/ )
install(TARGETS ${PROJECT_NAME} EXPORT ${PROJECT_NAME}-config DESTINATION lib/${PROJECT_NAME})
install(EXPORT ${PROJECT_NAME}-config DESTINATION lib/${PROJECT_NAME} EXPORT_LINK_INTERFACE_LIBRARIES)

# ----- Uninstall ----- #
configure_file(cmake_uninstall.cmake.in cmake_uninstall.cmake IMMEDIATE @ONLY)
add_custom_target(uninstall COMMAND "${CMAKE_COMMAND}" -P cmake_uninstall.cmake)
