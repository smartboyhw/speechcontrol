project(SpeechControl-Python)
cmake_minimum_required(VERSION 2.8)

find_package(SpeechControlApplication REQUIRED QUIET)
include(SpeechControlMacros)

set(SPPY_PLUGIN_MINOR_VERSION 0)
set(SPPY_PLUGIN_MAJOR_VERSION 1)
set(SPPY_PLUGIN_UUID "f2989a62-5de6-11e1-852d-d91646898ec2")
set(SPPY_PLUGIN_VERSION "${SPPY_PLUGIN_MAJOR_VERSION}.${SPPY_PLUGIN_MINOR_VERSION}")
set(SPPY_PLUGIN_INCLUDE_DIRS ${SPCHCNTRL_INCLUDE_DIRS})
set(SPPY_PLUGIN_LIBRARIES ${SPCHCNTRL_LIBRARIES})

file(GLOB SPPY_SRCS "src/*.cpp")
file(GLOB SPPY_HDRS "src/*.hpp")

QT4_AUTOMOC(${SPPY_SRCS})

configure_file("${PROJECT_SOURCE_DIR}/config.hpp.in"
               "${PROJECT_SOURCE_DIR}/src/config.hpp" @ONLY)

include_directories(${SPPY_PLUGIN_INCLUDE_DIRS}
    "${PROJECT_SOURCE_DIR}/../../")

add_library(spchcntrl-python SHARED ${SPPY_SRCS})
target_link_libraries(spchcntrl-python ${SPPY_PLUGIN_LIBRARIES})
speechcontrol_install_plugin(spchcntrl-python)
add_dependencies(plugins spchcntrl-python)

install(FILES "${PROJECT_SOURCE_DIR}/data/f2989a62-5de6-11e1-852d-d91646898ec2.spec"
        DESTINATION ${SPCHCNTRL_PLUGINS_SPEC_INSTALL_DIR})