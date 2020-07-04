/**
   @date 2020/06/22
**/

#ifndef geant4_py_hpp
#define geant4_py_hpp

#include <pybind11/pybind11.h>

namespace geant4_py
{
    void define_geant4(pybind11::module& main);
}

#endif
