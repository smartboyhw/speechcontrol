project(SpeechControlApplication)
cmake_minimum_required(VERSION 2.8)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

## Extra packages for the application
find_package(Xdo REQUIRED)
find_package(Hunspell REQUIRED)

## Required files
file(GLOB SPCHCNTRL_APP_SRCS "*.cpp")
file(GLOB SPCHCNTRL_APP_HDRS "*.hpp")
file(GLOB SPCHCNTRL_VOXFORGE_HDRS "voxforge/*.hpp")
file(GLOB SPCHCNTRL_VOXFORGE_SRCS "voxforge/*.cpp")
file(GLOB SPCHCNTRL_DICTATION_HDRS "dictation/*.hpp")
file(GLOB SPCHCNTRL_DICTATION_SRCS "dictation/*.cpp")
file(GLOB SPCHCNTRL_SESSIONS_HDRS "sessions/*.hpp")
file(GLOB SPCHCNTRL_SESSIONS_SRCS "sessions/*.cpp")
file(GLOB SPCHCNTRL_DESKTOPCONTROL_HDRS "desktopcontrol/*.hpp")
file(GLOB SPCHCNTRL_DESKTOPCONTROL_SRCS "desktopcontrol/*.cpp")
file(GLOB_RECURSE SPCHCNTRL_WINDOWS_HDRS "windows/*.hpp")
file(GLOB_RECURSE SPCHCNTRL_WINDOWS_SRCS "windows/*.cpp")

## Needed variables for the SpeechControl application.
set(SPCHCNTRL_APP_COMPILE_FLAGS ${SPCHCNTRL_COMPILE_FLAGS})
set(SPCHCNTRL_APP_COMPILE_FLAGS_DEBUG ${SPCHCNTRL_COMPILE_FLAGS_DEBUG})
set(SPCHCNTRL_APP_COMPILE_DEFINITIONS ${SPCHCNTRL_COMPILE_DEFINITIONS})
set(SPCHCNTRL_APP_INCLUDE_DIR "${SPCHCNTRL_INCLUDE_DIR}/app")
set(SPCHCNTRL_APP_INCLUDE_DIRS "${SPCHCNTRL_INCLUDE_DIR}"
    ${SPCHCNTRL_LIB_INCLUDE_DIR}
    ${SPCHCNTRL_APP_INCLUDE_DIR}
    ${SPCHCNTRL_INCLUDE_DIRS}
    ${XDO_INCLUDE_DIRS}
    ${HUNSPELL_INCLUDE_DIRS}
    ${SPCHCNTRL_LIB_INCLUDE_DIRS})
set(SPCHCNTRL_APP_LIBRARIES ${SPCHCNTRL_LIBRARIES}
    ${SPCHCNTRL_LIB_LIBRARIES}
    ${QT_QTGUI_LIBRARY}
    ${QT_QTDBUS_LIBRARY}
    ${QT_QTNETWORK_LIBRARY}
    ${XDO_LIBRARIES}
    ${HUNSPELL_LIBRARIES}
    speechcontrol)
set(SPCHCNTRL_APP_BUILD_INCLUDE_DIRS ${SPCHCNTRL_APP_INCLUDE_DIRS}
    "${PROJECT_BINARY_DIR}"
    "${PROJECT_BINARY_DIR}/frontend"
    "${PROJECT_SOURCE_DIR}/../lib"
    "${PROJECT_SOURCE_DIR}/app/windows"
    "${PROJECT_SOURCE_DIR}/../")

list(REMOVE_ITEM SPCHCNTRL_APP_BUILD_INCLUDE_DIRS ${SPCHCNTRL_APP_INCLUDE_DIR})

## Needed flags for SphinxTrain apps.
set(SPHINXTRAIN_INCLUDE_DIRS "${SPHINXBASE_INCLUDE_DIRS}"
        "${SPHINXTRAIN_INCLUDE_DIR}")
set(SPHINXTRAIN_BUILD_INCLUDE_DIRS "${SPHINXTRAIN_INCLUDE_DIRS}"
    "${PROJECT_SOURCE_DIR}/../include")

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(SPCHCNTRL_APP_COMPILE_FLAGS "${SPCHCNTRL_APP_COMPILE_FLAGS} ${SPCHCNTRL_APP_COMPILE_FLAGS_DEBUG}")
endif (CMAKE_BUILD_TYPE STREQUAL "Debug")

include_directories(${SPCHCNTRL_APP_BUILD_INCLUDE_DIRS}
    ${SPHINXTRAIN_BUILD_INCLUDE_DIRS})

# Some important definitions.
add_definitions(${SPCHCNTRL_APP_COMPILE_DEFINITIONS})

## Add sub-directories.
add_subdirectory(cmake)
add_subdirectory(desktopcontrol)
add_subdirectory(dictation)
add_subdirectory(sessions)
add_subdirectory(voxforge)
add_subdirectory(windows)

configure_file("${PROJECT_SOURCE_DIR}/config.hpp.in"
               "${PROJECT_SOURCE_DIR}/config.hpp" @ONLY)

# Add build information for front-end application, daemon and KDE features
include(application.cmake)
add_subdirectory(daemon)
add_subdirectory(kde)

## Installs
install(FILES ${SPCHCNTRL_APP_HDRS}
    DESTINATION "${SPCHCNTRL_APP_INCLUDE_DIR}"
    COMPONENT "dev")