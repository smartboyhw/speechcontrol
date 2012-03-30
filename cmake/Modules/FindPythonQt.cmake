# Trys to find PythonQt
#
# PYTHONQT_LIBRARIES
# PYTHONQT_INCLUDE_DIRS

find_path(PYTHONQT_INCLUDE_DIRS "PythonQt.h"
          HINTS "${CMAKE_INSTALL_PREFIX}/include/PythonQt")

find_library(PYTHONQT_LIBRARIES "PythonQt")

include(FindPackageHandleStandardArgs)

find_package_handle_standard_args(PythonQt DEFAULT_MSG
    PYTHONQT_LIBRARIES PYTHONQT_INCLUDE_DIRS)

mark_as_advanced(PYTHONQT_INCLUDE_DIRS PYTHONQT_LIBRARIES)