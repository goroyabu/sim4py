/**
   @date 2020/06/22
**/

#ifndef clhep_py_hpp
#define clhep_py_hpp

#include <pybind11/pybind11.h>

namespace clhep_py
{
    void define_clhep(pybind11::module& main);
    void define_units(pybind11::module& sub);
}

#endif
