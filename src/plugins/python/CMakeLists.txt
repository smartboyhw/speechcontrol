project(SpeechControl-Python)
cmake_minimum_required(VERSION 2.8)

set(SPPY_PLUGIN_MINOR_VERSION 0)
set(SPPY_PLUGIN_MAJOR_VERSION 1)
set(SPPY_PLUGIN_VERSION "${SPPY_PLUGIN_MAJOR_VERSION}.${SPPY_PLUGIN_MINOR_VERSION}")
set(SPPY_PLUGIN_INCLUDE_DIRS ${SPCHCNTRL_APP_INCLUDE_DIRS})
set(SPPY_PLUGIN_LIBRARIES ${SPCHCNTRL_APP_LIBRARIES})

file(GLOB SPPY_SRCS "src/*.cpp")
file(GLOB SPPY_HDRS "src/*.hpp")

QT4_WRAP_CPP(SPPY_SRCS ${SPPY_HDRS})

add_library(speechcontrol-python SHARED ${SPPY_SRCS})
target_link_libraries(speechcontrol-python ${SPPY_PLUGIN_LIBRARIES})

add_dependencies(plugins speechcontrol-python)

install(TARGETS speechcontrol-python
        DESTINATION ${SPCHCNTRL_PLUGINS_LIB_INSTALL_DIR})

install(FILES "${PROJECT_SOURCE_DIR}/data/f2989a62-5de6-11e1-852d-d91646898ec2.spec"
        DESTINATION ${SPCHCNTRL_PLUGINS_SPEC_INSTALL_DIR})