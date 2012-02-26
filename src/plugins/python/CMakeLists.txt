project(SpeechControl-Python)
cmake_minimum_required(VERSION 2.8)

find_package(SpeechControlApplication REQUIRED)

set(SPPY_PLUGIN_MINOR_VERSION 0)
set(SPPY_PLUGIN_MAJOR_VERSION 1)
set(SPPY_PLUGIN_UUID "f2989a62-5de6-11e1-852d-d91646898ec2")
set(SPPY_PLUGIN_VERSION "${SPPY_PLUGIN_MAJOR_VERSION}.${SPPY_PLUGIN_MINOR_VERSION}")
set(SPPY_PLUGIN_INCLUDE_DIRS ${SPCHCNTRL_INCLUDE_DIRS})
set(SPPY_PLUGIN_LIBRARIES ${SPCHCNTRL_LIBRARIES})

file(GLOB SPPY_SRCS "src/*.cpp")
file(GLOB SPPY_HDRS "src/*.hpp")

QT4_AUTOMOC(${SPPY_SRCS})

include_directories(${SPPY_PLUGIN_INCLUDE_DIRS}
    "${PROJECT_SOURCE_DIR}/../../")

add_library(pyspchcntrl SHARED ${SPPY_SRCS})
target_link_libraries(pyspchcntrl ${SPPY_PLUGIN_LIBRARIES})

configure_file("${PROJECT_SOURCE_DIR}/config.hpp.in"
               "${PROJECT_SOURCE_DIR}/src/config.hpp" @ONLY)

add_dependencies(plugins pyspchcntrl)

install(TARGETS pyspchcntrl
        DESTINATION ${SPCHCNTRL_PLUGINS_LIB_INSTALL_DIR})

install(FILES "${PROJECT_SOURCE_DIR}/data/f2989a62-5de6-11e1-852d-d91646898ec2.spec"
        DESTINATION ${SPCHCNTRL_PLUGINS_SPEC_INSTALL_DIR})