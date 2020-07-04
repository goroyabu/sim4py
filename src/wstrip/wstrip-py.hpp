/**
   @date 2020/06/22
**/

#ifndef wstrip_py_hpp
#define wstrip_py_hpp

#include <pybind11/pybind11.h>

namespace wstrip_py
{
    void define_wstrip(pybind11::module& main);
}

#endif
