cmake_minimum_required(VERSION 2.8)

include(UseQt4)

set(SPCHCNTRL_COMPILE_FLAGS "${QTGSTREAMER_FLAGS} --std=c++0x")
set(SPCHCNTRL_COMPILE_FLAGS_DEBUG "-Wall -Wextra -Wabi -Wctor-dtor-privacy -Wformat -Wunused -O0 -ggdb")
set(SPCHCNTRL_COMPILE_DEFINITIONS ${QT_DEFINITIONS} "-DBOOST_TT_HAS_OPERATOR_HPP_INCLUDED")
set(SPCHCNTRL_CMAKE_DIR "${CMAKE_INSTALL_PREFIX}/share/cmake/SpeechControl")
set(SPCHCNTRL_CMAKE_INSTALL_DIR "${CMAKE_MODULE_PATH}")
set(SPCHCNTRL_DATA_DIR "${CMAKE_INSTALL_PREFIX}/share/speechcontrol")
set(SPCHCNTRL_DATA_CONTENT_DIR "${SPCHCNTRL_DATA_DIR}/contents")
set(SPCHCNTRL_LIB_DIR "${CMAKE_INSTALL_PREFIX}/lib")
set(SPCHCNTRL_BIN_DIR "${CMAKE_INSTALL_PREFIX}/bin")
set(SPCHCNTRL_LOCALE_DIR "${CMAKE_INSTALL_PREFIX}/share/locale")
set(SPCHCNTRL_INCLUDE_DIR "${CMAKE_INSTALL_PREFIX}/include/speechcontrol")
set(SPCHCNTRL_PLUGINS_LIB_INSTALL_DIR "${SPCHCNTRL_LIB_DIR}/speechcontrol")
set(SPCHCNTRL_PLUGINS_SPEC_INSTALL_DIR "${CMAKE_INSTALL_PREFIX}/share/speechcontrol/plugins")
set(SPCHCNTRL_DOC_INSTALL_DIR "${CMAKE_INSTALL_PREFIX}/share/doc")

########### Add uninstall target ###############
if (NOT TARGET uninstall)
    CONFIGURE_FILE(
        "${CMAKE_CURRENT_SOURCE_DIR}/cmake/cmake_uninstall.cmake.in"
        "${CMAKE_CURRENT_BINARY_DIR}/cmake/cmake_uninstall.cmake"
    IMMEDIATE @ONLY)

    ADD_CUSTOM_TARGET(uninstall
                      "${CMAKE_COMMAND}" -P "${CMAKE_CURRENT_BINARY_DIR}/cmake/cmake_uninstall.cmake")
endif(NOT TARGET uninstall)

########### Some automatics ###################
set(CMAKE_USE_RELATIVE_PATHS ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)