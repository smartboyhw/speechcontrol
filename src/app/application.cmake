cmake_minimum_required(VERSION 2.8)

file(GLOB_RECURSE SPCHCNTRL_APP_SRCS "*.cpp" "*.cxx")
file(GLOB_RECURSE SPCHCNTRL_APP_HDRS "*.hpp" "*.hxx")

## Variables
set(SPCHCNTRL_QRESC "${CMAKE_SOURCE_DIR}/res/res.qrc")
file(GLOB SPCHCNTRL_UI "${CMAKE_SOURCE_DIR}/res/ui/*.ui")

## Do some handy dandy work.
QT4_ADD_RESOURCES(SPCHCNTRL_APP_SRCS ${SPCHCNTRL_QRESC})
QT4_WRAP_UI(SPCHCNTRL_APP_SRCS ${SPCHCNTRL_UI})
QT4_AUTOMOC(${SPCHCNTRL_APP_SRCS})

QT4_CREATE_TRANSLATION("speechcontrol.qm" "${SPCHCNTRL_APP_SRCS};${SPCHCNTRL_UI}" "speechcontrol.ts")
#QT4_ADD_TRANSLATION("speechcontrol.ts" "speechcontrol.qm")

include_directories(${SPCHCNTRL_APP_BUILD_INCLUDE_DIRS})
file(MAKE_DIRECTORY "${PROJECT_BINARY_DIR}/ui")
file(MAKE_DIRECTORY "${PROJECT_BINARY_DIR}/ui/wizards")
file(MAKE_DIRECTORY "${PROJECT_BINARY_DIR}/ui/wizards/adapt")
file(MAKE_DIRECTORY "${PROJECT_BINARY_DIR}/ui/wizards/contents")
file(MAKE_DIRECTORY "${PROJECT_BINARY_DIR}/ui/wizards/micsetup")
file(MAKE_DIRECTORY "${PROJECT_BINARY_DIR}/ui/wizards/quickstart")
file(MAKE_DIRECTORY "${PROJECT_BINARY_DIR}/ui/wizards/voxforge")
file(MAKE_DIRECTORY "${PROJECT_BINARY_DIR}/ui/settings")
file(MAKE_DIRECTORY "${PROJECT_BINARY_DIR}/services")
file(MAKE_DIRECTORY "${PROJECT_BINARY_DIR}/services/dictation")
file(MAKE_DIRECTORY "${PROJECT_BINARY_DIR}/services/desktopcontrol")
file(MAKE_DIRECTORY "${PROJECT_BINARY_DIR}/services/voxforge")
file(MAKE_DIRECTORY "${PROJECT_BINARY_DIR}/sessions")

## Define targets
add_executable(speechcontrol-app ${SPCHCNTRL_APP_SRCS}
    ${SPCHCNTRL_APP_SPRIVCPP})

set_target_properties(speechcontrol-app PROPERTIES
    COMPILE_FLAGS "${SPCHCNTRL_APP_COMPILE_FLAGS}"
    OUTPUT_NAME "speechcontrol"
    PROJECT_LABEL "SpeechControl")

add_dependencies(speechcontrol-app speechcontrol)
target_link_libraries(speechcontrol-app ${SPCHCNTRL_APP_LIBRARIES})

## Installs
install(TARGETS speechcontrol-app
        DESTINATION "${SPCHCNTRL_BIN_DIR}")
