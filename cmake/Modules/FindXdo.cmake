# Set up XDO
# XDO_INCLUDE_DIR
# XDO_LIBRARIES

find_package(PkgConfig QUIET)
pkg_check_modules(_XDO xdo QUIET)

find_path(XDO_INCLUDE_DIRS "xdo.h"
          HINTS ${_XDO_INCLUDEDIR} ${_XDO_INCLUDE_DIRS})
find_library(XDO_LIBRARIES "xdo"
    HINTS ${_XDO_LIBRARY_DIRS} ${_XDO_LIBDIR})

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(XDO DEFAULT_MSG
    XDO_LIBRARIES XDO_INCLUDE_DIRS)

list(APPEND XDO_INCLUDE_DIRS ${_XDO_INCLUDE_DIRS})

mark_as_advanced(XDO_INCLUDE_DIRS XDO_LIBRARIES)