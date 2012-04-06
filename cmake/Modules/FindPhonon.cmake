# - Find Phonon on the development system.
# This module finds if Phonon is installed and determines where the
# include files and libraries are. It also determines what the name of
# the library is. This code sets the following variables:
#
#  PHONON_LIBRARIES           - path to the Phonon library
#  PHONON_INCLUDE_DIRS        - path to where phonon.h is found
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


find_package(PkgConfig QUIET)
pkg_check_modules(_PHONON phonon)

find_path(PHONON_INCLUDE_DIRS phonon_export.h
    HINTS ${_PHONON_INCLUDEDIR} ${_PHONON_INCLUDE_DIRS}
    PATH_SUFFIXES phonon)
find_library(PHONON_LIBRARIES phonon
    HINTS ${_PHONON_LIBRARY_DIRS} ${_PHONON_LIBDIR})

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(Phonon DEFAULT_MSG
    PHONON_LIBRARIES PHONON_INCLUDE_DIRS)

list(APPEND PHONON_INCLUDE_DIRS ${_PHONON_INCLUDE_DIRS})

mark_as_advanced(PHONON_INCLUDE_DIRS PHONON_LIBRARIES)

set(PHONON_VERSION ${_PHONON_VERSION})