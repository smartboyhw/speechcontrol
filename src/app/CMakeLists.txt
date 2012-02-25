project(SpeechControlApplication)
cmake_minimum_required(VERSION 2.8)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

## Required application packages
pkg_search_module(INDICATOR indicate-qt REQUIRED)

## Required files
file(GLOB SPCHCNTRL_APP_SRCS "*.cpp")
file(GLOB SPCHCNTRL_APP_HDRS "*.hpp")
file(GLOB SPCHCNTRL_VOXFORGE_HDRS "voxforge/*.hpp")
file(GLOB SPCHCNTRL_VOXFORGE_SRCS "voxforge/*.cpp")
file(GLOB SPCHCNTRL_SESSIONS_HDRS "sessions/*.hpp")
file(GLOB SPCHCNTRL_SESSIONS_SRCS "sessions/*.cpp")
file(GLOB SPCHCNTRL_DESKTOPCONTROL_HDRS "desktopcontrol/*.hpp")
file(GLOB SPCHCNTRL_DESKTOPCONTROL_SRCS "desktopcontrol/*.cpp")
file(GLOB_RECURSE SPCHCNTRL_WINDOWS_HDRS "windows/*.hpp")
file(GLOB_RECURSE SPCHCNTRL_WINDOWS_SRCS "windows/*.cpp")

## Needed variables
set(SPCHCNTRL_APP_COMPILE_FLAGS ${SPCHCNTRL_COMPILE_FLAGS})
set(SPCHCNTRL_APP_COMPILE_DEFINITIONS ${SPCHCNTRL_COMPILE_DEFINITIONS})
set(SPCHCNTRL_APP_INCLUDE_DIR "${SPCHCNTRL_INCLUDE_DIR}/app")
set(SPCHCNTRL_APP_INCLUDE_DIRS "${SPCHCNTRL_INCLUDE_DIR}"
    ${SPCHCNTRL_LIB_INCLUDE_DIR}
    ${SPCHCNTRL_APP_INCLUDE_DIR}
    ${SPCHCNTRL_LIB_INCLUDE_DIRS}
    ${SPCHCNTRL_INCLUDE_DIRS}
    ${INDICATOR_INCLUDE_DIRS})
set(SPCHCNTRL_APP_LIBRARIES ${SPCHCNTRL_LIBRARIES}
    ${SPCHCNTRL_LIB_LIBRARIES}
    ${QT_QTGUI_LIBRRARY}
    ${QT_QTDBUS_LIBRRARY}
    ${INDICATOR_LIBRARIES}
    spchcntrl)
set(SPCHCNTRL_APP_BUILD_INCLUDE_DIRS "${PROJECT_SOURCE_DIR}/../lib"
    "${PROJECT_BINARY_DIR}")
set(SPCHCNTRL_APP_DATA "${CMAKE_INSTALL_PREFIX}/share/speechcontrol")

include_directories("${PROJECT_SOURCE_DIR}/../lib"
    "${PROJECT_SOURCE_DIR}"
    "${PROJECT_SOURCE_DIR}/windows")

# Some important definitions.
add_definitions(${SPCHCNTRL_APP_COMPILE_DEFINITIONS})

## Add sub-directories.
add_subdirectory(desktopcontrol)
add_subdirectory(sessions)
add_subdirectory(voxforge)
add_subdirectory(windows)

configure_file("${PROJECT_SOURCE_DIR}/config.hpp.in"
               "${PROJECT_SOURCE_DIR}/config.hpp" @ONLY)

# Add build information for pure Qt app and quasi-KDE app.
add_subdirectory(qt)
add_subdirectory(kde)

## Installs
install(FILES ${SPCHCNTRL_APP_HDRS}
    DESTINATION "${SPCHCNTRL_APP_INCLUDE_DIR}"
    COMPONENT "dev")

add_subdirectory(cmake)