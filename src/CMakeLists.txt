cmake_minimum_required(VERSION 2.8)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

## Variables shared by app/ and plugins/
set(SPCHCNTRL_BUILDING ON)
set(SPCHCNTRL_PLUGINS_LIB_INSTALL_DIR "${CMAKE_INSTALL_PREFIX}/lib/spchcntrl")
set(SPCHCNTRL_PLUGINS_SPEC_INSTALL_DIR "${CMAKE_INSTALL_PREFIX}/share/speechcontrol/plugins")

add_subdirectory(lib)
add_subdirectory(app)
add_subdirectory(plugins)