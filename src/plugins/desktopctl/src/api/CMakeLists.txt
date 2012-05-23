cmake_minimum_required(VERSION 2.8)

find_package(SpeechControlApplication REQUIRED QUIET)
include(SpeechControlMacros)

set(SPDSKAPI_PLUGIN_MINOR_VERSION 1)
set(SPDSKAPI_PLUGIN_MAJOR_VERSION 0)
set(SPDSKAPI_PLUGIN_ID "d3a540f9")
set(SPDSKAPI_PLUGIN_VERSION "${SPDSKAPI_PLUGIN_MAJOR_VERSION}.${SPDSKAPI_PLUGIN_MINOR_VERSION}")
set(SPDSKAPI_PLUGIN_INCLUDE_DIRS ${SPCHCNTRL_INCLUDE_DIRS})
set(SPDSKAPI_PLUGIN_LIBRARIES ${SPCHCNTRL_LIBRARIES})

file(GLOB SPDSKAPI_UI "${PROJECT_SOURCE_DIR}/res/ui/*.ui")
file(GLOB SPDSKAPI_SRCS "*.cpp")
file(GLOB SPDSKAPI_HDRS "*.hpp")

QT4_AUTOMOC(${SPDSKAPI_SRCS})
QT4_WRAP_UI(SPDSKAPI_SRCS ${SPDSKAPI_UI})
configure_file("${PROJECT_SOURCE_DIR}/src/api/global.hpp.in"
               "${PROJECT_SOURCE_DIR}/src/api/global.hpp" @ONLY)
include_directories(${SPDSKAPI_PLUGIN_INCLUDE_DIRS}
    ${PROJECT_SOURCE_DIR}
    ${SPCHCNTRL_PLUGIN_BUILD_INCLUDE_DIRS})

add_library(spchcntrl-dskctlapi SHARED ${SPDSKAPI_SRCS})
target_link_libraries(spchcntrl-dskctlapi ${SPDSKAPI_PLUGIN_LIBRARIES})
set_target_properties(spchcntrl-dskctlapi PROPERTIES
    DEFINE_SYMBOL "SPCH_EXPORT_SYMBOLS"
    VERSION "${SPDSKAPI_PLUGIN_VERSION}"
    SOVERSION "${SPDSKAPI_PLUGIN_VERSION}")
speechcontrol_install_plugin(spchcntrl-dskctlapi ${SPDSKAPI_PLUGIN_ID})
add_dependencies(plugins spchcntrl-dskctlapi)