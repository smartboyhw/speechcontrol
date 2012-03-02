# Searches for the XDO library.
# If found, provides the following:
#
# - XDO_INCLUDE_DIRS : The include directories (typically just one) to find the headers for XDO.
# - XDO_LIBRARIES : The libraries needed to link to XDO.

find_path(XDO_INCLUDE_DIRS "xdo.h")

if(XDO_INCLUDE_DIRS STREQUAL "XDO_INCLUDE_DIRS-NOTFOUND")
    set(XDO_INCLUDE_DIRS "")
else(XDO_INCLUDE_DIRS STREQUAL "XDO_INCLUDE_DIRS-NOTFOUND")

    find_library(XDO_LIBRARIES "xdo")
    if(XDO_LIBRARIES STREQUAL "XDO_LIBRARIES-NOTFOUND")
        set(XDO_LIBRARIES "")
        message(WARNING "Xdo library not found.")
    else(XDO_LIBRARIES STREQUAL "XDO_LIBRARIES-NOTFOUND")
        message(STATUS "Found Xdo: ${XDO_LIBRARIES}")
    endif(XDO_LIBRARIES STREQUAL "XDO_LIBRARIES-NOTFOUND")
endif(XDO_INCLUDE_DIRS STREQUAL "XDO_INCLUDE_DIRS-NOTFOUND")

if (NOT XDO-NOTFOUND)
    set(XDO_LIBRARIES ${XDO})
    set(XDO_INCLUDE_DIRS "${CMAKE_INSTALL_PREFIX}/include")
endif(NOT XDO-NOTFOUND)
