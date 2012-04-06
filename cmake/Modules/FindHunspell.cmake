# - Find Hunspell on the development system.
# This module finds if Hunspell is installed and determines where the
# include files and libraries are. It also determines what the name of
# the library is. This code sets the following variables:
#
#  HUNSPELL_LIBRARIES           - path to the Hunspell library
#  HUNSPELL_INCLUDE_DIRS        - path to where hunspell.h is found
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
pkg_check_modules(_HS hunspell)

find_path(HUNSPELL_INCLUDE_DIRS "hunspell.hxx"
          HINTS ${_HS_INCLUDEDIR} ${_HS_INCLUDE_DIRS})
find_library(HUNSPELL_LIBRARIES "hunspell"
    HINTS ${_HS_LIBRARY_DIRS} ${_HS_LIBDIR})

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(Hunspell DEFAULT_MSG
    HUNSPELL_LIBRARIES HUNSPELL_INCLUDE_DIRS)

list(APPEND HUNSPELL_INCLUDE_DIRS ${_HS_INCLUDE_DIRS})

mark_as_advanced(HUNSPELL_INCLUDE_DIRS HUNSPELL_LIBRARIES)