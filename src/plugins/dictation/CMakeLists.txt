project(SpeechControl-Dictation)
cmake_minimum_required(VERSION 2.8)

find_package(SpeechControlApplication REQUIRED QUIET)
include(SpeechControlMacros)

set(SPDCTN_PLUGIN_MINOR_VERSION 1)
set(SPDCTN_PLUGIN_MAJOR_VERSION 0)
set(SPDCTN_PLUGIN_ID "05333f7b")
set(SPDCTN_PLUGIN_VERSION "${SPDCTN_PLUGIN_MAJOR_VERSION}.${SPDCTN_PLUGIN_MINOR_VERSION}")
set(SPDCTN_PLUGIN_INCLUDE_DIRS ${SPCHCNTRL_INCLUDE_DIRS}
    ${XDO_INCLUDE_DIRS}
    ${HUNSPELL_INCLUDE_DIRS})
set(SPDCTN_PLUGIN_LIBRARIES ${SPCHCNTRL_LIBRARIES}
    ${XDO_LIBRARIES}
    ${HUNSPELL_LIBRARIES})

file(GLOB SPDCTN_UI "ui/*.ui")
file(GLOB SPDCTN_SRCS "src/*.cpp")
file(GLOB SPDCTN_HDRS "src/*.hpp")

find_package(Xdo REQUIRED)
find_package(Hunspell REQUIRED)


QT4_WRAP_UI(SPDCTN_SRCS ${SPDCTN_UI})
QT4_AUTOMOC(${SPDCTN_SRCS})
configure_file("${PROJECT_SOURCE_DIR}/src/global.hpp.in"
               "${PROJECT_SOURCE_DIR}/src/global.hpp" @ONLY)
include_directories(${SPDCTN_PLUGIN_INCLUDE_DIRS}
    "${PROJECT_SOURCE_DIR}/src"
    ${SPCHCNTRL_PLUGIN_BUILD_INCLUDE_DIRS})

add_library(spchcntrl-dctn SHARED ${SPDCTN_SRCS})
target_link_libraries(spchcntrl-dctn ${SPDCTN_PLUGIN_LIBRARIES})
set_target_properties(spchcntrl-dctn PROPERTIES
    DEFINE_SYMBOL "SPCH_EXPORT_SYMBOLS"
    VERSION "${SPDCTN_PLUGIN_VERSION}"
    SOVERSION "${SPDCTN_PLUGIN_VERSION}")
speechcontrol_install_plugin(spchcntrl-dctn ${SPDCTN_PLUGIN_ID})
add_dependencies(plugins spchcntrl-dctn)