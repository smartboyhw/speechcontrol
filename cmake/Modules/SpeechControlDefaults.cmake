cmake_minimum_required(VERSION 2.8)

set(SPCHCNTRL_PLUGINS_LIB_INSTALL_DIR "@SPCHCNTRL_PLUGINS_LIB_INSTALL_DIR@")
set(SPCHCNTRL_PLUGINS_SPEC_INSTALL_DIR "@SPCHCNTRL_PLUGINS_SPEC_INSTALL_DIR@")
set(SPCHCNTRL_LIB_DIR "${CMAKE_INSTALL_PREFIX}/lib")
set(SPCHCNTRL_BIN_DIR "${CMAKE_INSTALL_PREFIX}/bin")
set(SPCHCNTRL_INCLUDE_DIR "${CMAKE_INSTALL_PREFIX}/include/speechcontrol")
set(SPCHCNTRL_PLUGINS_LIB_INSTALL_DIR "${SPCHCNTRL_LIB_DIR}/speechcontrol")
set(SPCHCNTRL_PLUGINS_SPEC_INSTALL_DIR "${CMAKE_INSTALL_PREFIX}/share/speechcontrol/plugins")
set(SPCHCNTRL_APP_DATA "${CMAKE_INSTALL_PREFIX}/share/speechcontrol")
set(SPCHCNTRL_DOC_INSTALL_DIR "${CMAKE_INSTALL_PREFIX}/share/doc")

########### Add uninstall target ###############
if (TARGET uninstall)
    message(STATUS "'uninstall' target handled.")
else(TARGET uninstall)
    CONFIGURE_FILE(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake/cmake_uninstall.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/cmake/cmake_uninstall.cmake"
    IMMEDIATE @ONLY)
    ADD_CUSTOM_TARGET(uninstall
    "${CMAKE_COMMAND}" -P "${CMAKE_CURRENT_BINARY_DIR}/cmake/cmake_uninstall.cmake")
endif(TARGET uninstall)