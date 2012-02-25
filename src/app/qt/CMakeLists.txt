cmake_minimum_required(VERSION 2.8)

file(GLOB SPCHCNTRL_QT_APP_SRCS "*.cpp")
file(GLOB SPCHCNTRL_QT_APP_HDRS "*.hpp")

## Variables
set(SPCHCNTRL_APP_SCPP ${SPCHCNTRL_APP_SRCS}
    ${SPCHCNTRL_WINDOWS_SRCS}
    ${SPCHCNTRL_VOXFORGE_SRCS}
    ${SPCHCNTRL_SESSIONS_SRCS}
    ${SPCHCNTRL_DESKTOPCONTROL_SRCS}
    ${SPCHCNTRL_QT_APP_SRCS})
set(SPCHCNTRL_APP_SHPP ${SPCHCNTRL_APP_HDRS})

## Do some handy dandy work.
QT4_ADD_RESOURCES(SPCHCNTRL_APP_SCPP ${SPCHCNTRL_QRESC})
QT4_WRAP_UI(SPCHCNTRL_APP_SCPP ${SPCHCNTRL_UI})
QT4_AUTOMOC(${SPCHCNTRL_APP_SCPP})

include_directories(${SPCHCNTRL_APP_INCLUDE_DIRS})

## Define targets
add_executable(speechcontrol ${SPCHCNTRL_APP_SCPP})
set_target_properties(speechcontrol PROPERTIES
    DEFINE_SYMBOL "SPCHCNTRL_APP"
    COMPILE_DEFINITIONS "${SPCHCNTRL_APP_COMPILE_DEFINITIONS}"
    COMPILE_FLAGS "${SPCHCNTRL_APP_COMPILE_FLAGS}"
    PROJECT_LABEL "SpeechControl")
add_dependencies(speechcontrol spchcntrl)
target_link_libraries(speechcontrol ${SPCHCNTRL_APP_LIBRARIES})

## Installs
install(TARGETS speechcontrol
        DESTINATION "${SPCHCNTRL_BIN_DIR}")

include_directories(${SPCHCNTRL_APP_INCLUDE_DIRS})

add_test(testapp COMMAND $<TARGET_FILE:speechcontrol>)