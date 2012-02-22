project(SpeechControlApplication)
cmake_minimum_required(VERSION 2.8)

file(GLOB_RECURSE SPCHCNTRL_APP_SCPP "*.cpp")
file(GLOB_RECURSE SPCHCNTRL_APP_SHPP "*.hpp")
file(GLOB_RECURSE SPCHCNTRL_UI "../../ui/*.ui")
file(GLOB SPCHCNTRL_QRESC "../../ui/*.qrc")

set(SPCHCNTRL_APP_INCLUDE_DIR "${SPCHCNTRL_INCLUDE_DIR}/app")
set(SPCHCNTRL_APP_INCLUDE_DIRS "${SPCHCNTRL_LIB_INCLUDE_DIR}"
    ${SPCHCNTRL_INCLUDE_DIRS})
set(SPCHCNTRL_APP_LIBRARIES ${SPCHCNTRL_LIBRARIES} spchcntrl)
set(SPCHCNTRL_APP_DATA "${CMAKE_INSTALL_PREFIX}/share/speechcontrol")
set(SPCHCNTRL_APP_SOURCE_DIRECTORIES "desktopcontrol"
    "dictation" "plugins" "sessions" "windows" "wizards")

include_directories("${PROJECT_SOURCE_DIR}/../lib")

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

configure_file("${SPCHCNTRL_CMAKE_SDIR}/SpeechControlApplicationConfig.cmake.in"
    "${PROJECT_BINARY_DIR}/InstallFiles/SpeechControlApplicationConfig.cmake" @ONLY)

configure_file("${SPCHCNTRL_CMAKE_SDIR}/SpeechControlApplicationConfigVersion.cmake.in"
    "${PROJECT_BINARY_DIR}/InstallFiles/SpeechControlApplicationConfigVersion.cmake" @ONLY)

#message("${SPCHCNTRL_APP_SCPP}")

add_executable(speechcontrol ${SPCHCNTRL_APP_SCPP})

set_target_properties(spchcntrl PROPERTIES
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

install(FILES "${PROJECT_BINARY_DIR}/InstallFiles/SpeechControlApplicationConfig.cmake"
    "${PROJECT_BINARY_DIR}/InstallFiles/SpeechControlApplicationConfigVersion.cmake"
    DESTINATION "${SPCHCNTRL_CMAKE_DIR}"
    COMPONENT "dev")

install(FILES "${PROJECT_SOURCE_DIR}/SpeechControl.desktop"
    DESTINATION "${CMAKE_INSTALL_PREFIX}/share/applications")