project(SpeechControl-DesktopControl-SystemControl)
cmake_minimum_required(VERSION 2.8)

find_package(SpeechControlApplication REQUIRED QUIET)
include(SpeechControlMacros)

set(SPDSKSYSCNTRL_PLUGIN_MINOR_VERSION 1)
set(SPDSKSYSCNTRL_PLUGIN_MAJOR_VERSION 0)
set(SPDSKSYSCNTRL_PLUGIN_ID "53df2954")
set(SPDSKSYSCNTRL_PLUGIN_VERSION "${SPDSKSYSCNTRL_PLUGIN_MAJOR_VERSION}.${SPDSKSYSCNTRL_PLUGIN_MINOR_VERSION}")
set(SPDSKSYSCNTRL_PLUGIN_INCLUDE_DIRS ${SPCHCNTRL_APP_INCLUDE_DIRS})
set(SPDSKSYSCNTRL_PLUGIN_LIBRARIES ${SPCHCNTRL_APP_LIBRARIES} spchcntrl-dskctlapi)

file(GLOB SPDSKSYSCNTRL_SRCS "src/*.cpp")
file(GLOB SPDSKSYSCNTRL_HDRS "src/*.hpp")

QT4_AUTOMOC(${SPDSKSYSCNTRL_SRCS})
configure_file("${PROJECT_SOURCE_DIR}/config.hpp.in"
               "${PROJECT_SOURCE_DIR}/src/config.hpp" @ONLY)
include_directories(${SPDSKSYSCNTRL_PLUGIN_INCLUDE_DIRS}
    "${PROJECT_SOURCE_DIR}/src"
    ${SPCHCNTRL_PLUGIN_BUILD_INCLUDE_DIRS})

add_library(spchcntrl-syscntrl SHARED ${SPDSKSYSCNTRL_SRCS})
target_link_libraries(spchcntrl-syscntrl ${SPDSKSYSCNTRL_PLUGIN_LIBRARIES})
set_target_properties(spchcntrl-syscntrl PROPERTIES
    DEFINE_SYMBOL "SPCH_EXPORT_SYMBOLS"
    VERSION "${SPDSKSYSCNTRL_PLUGIN_VERSION}"
    SOVERSION "${SPDSKSYSCNTRL_PLUGIN_VERSION}")
speechcontrol_install_plugin(spchcntrl-syscntrl ${SPDSKSYSCNTRL_PLUGIN_ID})
add_dependencies(plugins spchcntrl-syscntrl)