project(SpeechControlApplication)
cmake_minimum_required(VERSION 2.8)

## Required files
file(GLOB_RECURSE SPCHCNTRL_APP_SCPP "*.cpp")
file(GLOB_RECURSE SPCHCNTRL_APP_SHPP "*.hpp")
file(GLOB_RECURSE SPCHCNTRL_UI "../../ui/*.ui")
file(GLOB SPCHCNTRL_QRESC "../../ui/*.qrc")

## Needed variables
set(SPCHCNTRL_APP_COMPILE_FLAGS ${SPCHCNTRL_COMPILE_FLAGS})
set(SPCHCNTRL_APP_COMPILE_DEFINITIONS ${SPCHCNTRL_COMPILE_DEFINITIONS})
set(SPCHCNTRL_APP_INCLUDE_DIR "${SPCHCNTRL_INCLUDE_DIR}/app")
set(SPCHCNTRL_APP_INCLUDE_DIRS ${SPCHCNTRL_LIB_INCLUDE_DIR}
    ${SPCHCNTRL_INCLUDE_DIRS})
set(SPCHCNTRL_APP_LIBRARIES ${SPCHCNTRL_LIBRARIES}
    ${QT_QTGUI_LIBRARY}
    spchcntrl)
set(SPCHCNTRL_APP_DATA "${CMAKE_INSTALL_PREFIX}/share/speechcontrol")
set(SPCHCNTRL_APP_SOURCE_DIRECTORIES "plugins" "sessions" "windows" "wizards")

## Setting up KDE integration
if(WITH_KDE)
    find_package(KDE4)
    if (KDE4_FOUND)
        include (KDE4Defaults)
        message(STATUS "KDE integration enabled.")
        list(APPEND SPCHCNTRL_APP_INCLUDE_DIRS ${KDE4_INCLUDES})
        list(APPEND SPCHCNTRL_APP_COMPILE_DEFINITIONS -DHAVE_KDE)
        #string(REPLACE " -D_" ";-D" SPCHCNTRL_APP_COMPILE_DEFINITIONS ${KDE4_DEFINITIONS})
        message("${SPCHCNTRL_APP_COMPILE_DEFINITIONS}")
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
        message(STATUS "Support for D-Bus enabled; QtDBus found.")
        list(APPEND SPCHCNTRL_APP_COMPILE_DEFINITIONS HAVE_DBUS)
    else(QT_QTDBUS_FOUND)
        message(STATUS "Support for D-Bus disabled since QtDBus couldn't be not found.")
        set(WITH_DBUS OFF)
    endif(QT_QTDBUS_FOUND)
else(WITH_DBUS)
    message(STATUS "Support for D-Bus disabled.")
endif(WITH_DBUS)

## Do some Qt-specific stuff (and check if we're playing with KDE).
if (KDE4_FOUND)
    kde4_add_ui_files(SPCHCNTRL_APP_SCPP ${SPCHCNTRL_UI})
else (KDE4_FOUND)
    QT4_WRAP_UI(SPCHCNTRL_APP_SHPP ${SPCHCNTRL_UI})
    QT4_WRAP_CPP(SPCHCNTRL_APP_SCPP ${SPCHCNTRL_APP_SHPP} OPTIONS -nw)
endif (KDE4_FOUND)

QT4_ADD_RESOURCES(SPCHCNTRL_APP_SCPP ${SPCHCNTRL_QRESC})

## Add sub-directories.
foreach(SPCHCNTRL_APP_SOURCE_DIRECTORY ${SPCHCNTRL_APP_SOURCE_DIRECTORIES})
    add_subdirectory("${SPCHCNTRL_APP_SOURCE_DIRECTORY}")
endforeach(SPCHCNTRL_APP_SOURCE_DIRECTORY)

if(WITH_DBUS)
    add_subdirectory(dbus)
endif(WITH_DBUS)

## Define the exectuable target (check for KDE again!)
if(KDE4_FOUND)
    kde4_add_executable(speechcontrol ${SPCHCNTRL_APP_SCPP})
else(KDE4_FOUND)
    add_executable(speechcontrol ${SPCHCNTRL_APP_SCPP})
endif(KDE4_FOUND)

set_target_properties(spchcntrl PROPERTIES
    DEFINE_SYMBOL "SPCHCNTRL_APP"
    COMPILE_FLAGS "${SPCHCNTRL_APP_COMPILE_FLAGS}"
    COMPILE_DEFINITIONS "${SPCHCNTRL_APP_COMPILE_DEFINITIONS}"
    PROJECT_LABEL "SpeechControl")

target_link_libraries(speechcontrol ${SPCHCNTRL_APP_LIBRARIES})

## Installs
if (KDE4_FOUND)
    install(TARGETS speechcontrol DESTINATION ${INSTALL_TARGETS_DEFAULT_ARGS})
    install(FILES "${PROJECT_SOURCE_DIR}/SpeechControl.desktop" DESTINATION ${XDG_APPS_INSTALL_DIR})
else (KDE4_FOUND)
    install(TARGETS speechcontrol DESTINATION "${SPCHCNTRL_BIN_DIR}")
    install(FILES "${PROJECT_SOURCE_DIR}/SpeechControl.desktop" DESTINATION "${SPCHCNTRL_APP_DATA}/SpeechControl.desktop")
endif (KDE4_FOUND)

install(FILES ${SPCHCNTRL_APP_SHPP}
    DESTINATION "${SPCHCNTRL_APP_INCLUDE_DIR}"
    COMPONENT "dev")

install(FILES ${SPCHCNTRL_UI} ${SPCHCNTRL_QRESC}
    DESTINATION "${SPCHCNTRL_APP_DATA}"
    COMPONENT "dev")

## Finally include directories.
include_directories(${SPCHCNTRL_APP_INCLUDE_DIRS}
    "${PROJECT_SOURCE_DIR}/../lib")