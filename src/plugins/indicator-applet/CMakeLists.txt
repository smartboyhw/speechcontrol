project(Indicator-SpeechControl)
cmake_minimum_required(VERSION 2.8)

pkg_search_module(INDICATOR indicate-qt REQUIRED)

set(SPIA_PLUGIN_MINOR_VERSION 0)
set(SPIA_PLUGIN_MAJOR_VERSION 1)
set(SPIA_PLUGIN_VERSION "${SPIA_PLUGIN_MAJOR_VERSION}.${SPIA_PLUGIN_MINOR_VERSION}")
set(SPIA_PLUGIN_INCLUDE_DIRS ${SPCHCNTRL_APP_INCLUDE_DIRS}
    ${INDICATOR_INCLUDE_DIRS})
set(SPIA_PLUGIN_LIBRARIES ${SPCHCNTRL_APP_LIBRARIES}
    ${INDICATOR_LIBRARIES})

file(GLOB SPIA_SRCS "src/*.cpp")
file(GLOB SPIA_HDRS "src/*.hpp")

QT4_WRAP_CPP(SPIA_SRCS ${SPIA_HDRS})

add_library(speechcontrol-indicatorapplet SHARED ${SPIA_SRCS})
target_link_libraries(speechcontrol-indicatorapplet ${SPIA_PLUGIN_LIBRARIES})

add_dependencies(plugins speechcontrol-indicatorapplet)

install(TARGETS speechcontrol-indicatorapplet
        DESTINATION ${SPCHCNTRL_PLUGINS_LIB_INSTALL_DIR})

install(FILES "${PROJECT_SOURCE_DIR}/data/cac40936-5de6-11e1-852d-431a288c9099.spec"
        DESTINATION ${SPCHCNTRL_PLUGINS_SPEC_INSTALL_DIR})