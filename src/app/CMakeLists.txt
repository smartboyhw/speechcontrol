project(SpeechControlApplication)
cmake_minimum_required(VERSION 2.8)

## Required files
file(GLOB_RECURSE SPCHCNTRL_APP_SCPP "*.cpp")
file(GLOB_RECURSE SPCHCNTRL_APP_SHPP "*.hpp")
file(GLOB_RECURSE SPCHCNTRL_UI "../../ui/*.ui")
file(GLOB SPCHCNTRL_QRESC "../../ui/*.qrc")

## Needed variables
set(SPCHCNTRL_APP_COMPILE_DEFINITIONS "${SPCHCNTRL_COMPILE_DEFINITIONS}")
set(SPCHCNTRL_APP_COMPILE_FLAGS "${SPCHCNTRL_COMPILE_FLAGS}")
set(SPCHCNTRL_APP_INCLUDE_DIR "${SPCHCNTRL_INCLUDE_DIR}/app")
set(SPCHCNTRL_APP_INCLUDE_DIRS "${SPCHCNTRL_LIB_INCLUDE_DIR}"
    ${SPCHCNTRL_INCLUDE_DIRS})
set(SPCHCNTRL_APP_LIBRARIES ${SPCHCNTRL_LIBRARIES} spchcntrl)
set(SPCHCNTRL_APP_DATA "${CMAKE_INSTALL_PREFIX}/share/speechcontrol")
set(SPCHCNTRL_APP_SOURCE_DIRECTORIES "desktopcontrol"
    "dictation" "plugins" "sessions" "windows" "wizards")

## Setting up KDE integration
if(WITH_KDE)
    find_package(KDE4)
    if (KDE4_FOUND)
        message(STATUS "KDE integration enabled.")
        list(APPEND SPCHCNTRL_APP_INCLUDE_DIRS ${KDE4_INCLUDES})
        list(APPEND SPCHCNTRL_APP_COMPILE_DEFINITIONS HAVE_KDE ${KDE4_DEFINITIONS})
        string(REPLACE "-D" ";" SPCHCNTRL_APP_COMPILE_DEFINITIONS ${SPCHCNTRL_APP_COMPILE_DEFINITIONS})
        list(APPEND SPCHCNTRL_APP_LIBRARIES ${KDE4_KDECORE_LIBS} ${KDE4_KDEUI_LIBRARY} ${KDE4_SOLID_LIBS} knotifyconfig)
    else(KDE4_FOUND)
        message(STATUS "KDE integration disabled; required development files not found.")
    endif(KDE4_FOUND)
else(WITH_KDE)
    message(STATUS "KDE integration disabled.")
endif(WITH_KDE)

## Setting up D-Bus support.
if(WITH_DBUS)
    if(QT_QTDBUS_FOUND)
        message(STATUS "Support for D-Bus enabled; Qt-DBus found.")
        list(APPEND SPCHCNTRL_APP_COMPILE_DEFINITIONS HAVE_DBUS)
    else(QT_QTDBUS_FOUND)
        message(STATUS "Support for D-Bus disabled since Qt-DBus couldn't be not found.")
    endif(QT_QTDBUS_FOUND)
else(WITH_DBUS)
    message(STATUS "Support for D-Bus disabled.")
endif(WITH_DBUS)

include_directories("${SPCHCNTRL_APP_INCLUDE_DIRS}"
    "${PROJECT_SOURCE_DIR}/../lib")

foreach(SPCHCNTRL_APP_SOURCE_DIRECTORY ${SPCHCNTRL_APP_SOURCE_DIRECTORIES})
    add_subdirectory("${SPCHCNTRL_APP_SOURCE_DIRECTORY}")
endforeach(SPCHCNTRL_APP_SOURCE_DIRECTORY)

QT4_WRAP_UI(SPCHCNTRL_APP_SHPP ${SPCHCNTRL_UI})
QT4_WRAP_CPP(SPCHCNTRL_APP_SCPP ${SPCHCNTRL_APP_SHPP} OPTIONS
    -DBOOST_TT_HAS_OPERATOR_HPP_INCLUDED)
QT4_ADD_RESOURCES(SPCHCNTRL_APP_SCPP ${SPCHCNTRL_QRESC})

## configures
configure_file("${PROJECT_SOURCE_DIR}/config.hpp.in"
    "${PROJECT_SOURCE_DIR}/config.hpp" @ONLY)

    #message("${SPCHCNTRL_APP_SCPP}")

add_executable(speechcontrol ${SPCHCNTRL_APP_SCPP})

set_target_properties(spchcntrl PROPERTIES
    COMPILE_FLAGS ${SPCHCNTRL_APP_COMPILE_FLAGS}
    COMPILE_DEFINITIONS "${SPCHCNTRL_APP_COMPILE_DEFINITIONS}"
    PROJECT_LABEL "SpeechControl")

target_link_libraries(speechcontrol ${SPCHCNTRL_APP_LIBRARIES})

## Installs
install(TARGETS speechcontrol
    DESTINATION "${SPCHCNTRL_BIN_DIR}")

install(FILES ${SPCHCNTRL_APP_SHPP}
    DESTINATION "${SPCHCNTRL_APP_INCLUDE_DIR}"
    COMPONENT "dev")

install(FILES ${SPCHCNTRL_UI} ${SPCHCNTRL_QRESC}
    DESTINATION "${SPCHCNTRL_APP_DATA}"
    COMPONENT "dev")

install(FILES "${PROJECT_SOURCE_DIR}/SpeechControl.desktop"
    DESTINATION "${SPCHCNTRL_APP_DATA}/SpeechControl.desktop")