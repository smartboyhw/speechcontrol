# - Find PythonQt on the development system.
# This module finds if PythonQt is installed and determines where the
# include files and libraries are. It also determines what the name of
# the library is. This code sets the following variables:
#
#  PYTHONQT_LIBRARIES           - path to the PythonQt library
#  PYTHONQT_INCLUDE_DIRS        - path to where PythonQt.h is found
#
# It has one dependency on PythonLibs, which is pulled in automatically.
#
#=============================================================================
# Copyright (c) 2012 Jacky Alcine <jacky.alcine@thesii.org>
#
# This module is free software; you can redistribute it and/or
# modify it under the terms of the GNU Library General Public
# License as published by the Free Software Foundation; either
# version 2 of the License, or (at your option) any later version.
#
#=============================================================================
# (To distribute this file outside of CMake, substitute the full
#  License text for the above reference.)

find_package(PythonLibs 2.7 REQUIRED)
find_path(PYTHONQT_INCLUDE_DIRS "PythonQt/PythonQt.h"
                                PATH_SUFFIXES "PythonQt")
find_library(PYTHONQT_LIBRARIES "PythonQt")

include(FindPackageHandleStandardArgs)

find_package_handle_standard_args(PythonQt DEFAULT_MSG
    PYTHONQT_LIBRARIES PYTHONQT_INCLUDE_DIRS
    PYTHON_LIBRARIES PYTHON_INCLUDE_DIRS)

list(APPEND PYTHONQT_INCLUDE_DIRS ${PYTHON_INCLUDE_DIRS})
list(APPEND PYTHONQT_LIBRARIES ${PYTHON_LIBRARIES})

mark_as_advanced(PYTHONQT_INCLUDE_DIRS PYTHONQT_LIBRARIES)