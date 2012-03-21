# Set up Hunspell
# HUNSPELL_INCLUDE_DIR
# HUNSPELL_LIBRARIES

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