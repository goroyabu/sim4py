/**
   @date 2020/06/22
**/

#ifndef general_py_hpp
#define general_py_hpp

#include <pybind11/pybind11.h>

namespace general_py
{
    void define_general(pybind11::module& main);
    void define_physics(pybind11::module& main);
    void define_action(pybind11::module& main);
    void define_analysis(pybind11::module& main);
    void define_detector(pybind11::module& main);
}

#endif
