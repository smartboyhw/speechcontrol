project(SpeechControl-Python)
cmake_minimum_required(VERSION 2.8)

find_package(SpeechControlApplication REQUIRED)
find_package(PythonQt)
include(SpeechControlMacros)

set(SPPY_PLUGIN_MINOR_VERSION 1)
set(SPPY_PLUGIN_MAJOR_VERSION 0)
set(SPPY_PLUGIN_ID "f2989a62")
set(SPPY_PLUGIN_VERSION "${SPPY_PLUGIN_MAJOR_VERSION}.${SPPY_PLUGIN_MINOR_VERSION}")
set(SPPY_PLUGIN_INCLUDE_DIRS ${SPCHCNTRL_APP_INCLUDE_DIRS}
    ${PYTHONQT_INCLUDE_DIRS})
set(SPPY_PLUGIN_LIBRARIES ${SPCHCNTRL_APP_LIBRARIES}
    ${PYTHONQT_LIBRARIES})

file(GLOB SPPY_SRCS "src/*.cpp")
file(GLOB SPPY_HDRS "src/*.hpp")

QT4_AUTOMOC(${SPPY_SRCS})

configure_file("${PROJECT_SOURCE_DIR}/config.hpp.in"
               "${PROJECT_SOURCE_DIR}/src/config.hpp" @ONLY)

include_directories(${SPPY_PLUGIN_INCLUDE_DIRS}
    "${PROJECT_SOURCE_DIR}/src"
    ${SPCHCNTRL_PLUGIN_BUILD_INCLUDE_DIRS})

add_library(spchcntrl-python SHARED ${SPPY_SRCS})
target_link_libraries(spchcntrl-python ${SPPY_PLUGIN_LIBRARIES})
set_target_properties(spchcntrl-python PROPERTIES
    DEFINE_SYMBOL "SPCH_EXPORT_SYMBOLS"
    VERSION "${SPPY_PLUGIN_VERSION}"
    SOVERSION "${SPPY_PLUGIN_VERSION}")

speechcontrol_install_plugin(spchcntrl-python "${SPPY_PLUGIN_ID}")
add_dependencies(plugins spchcntrl-python)
