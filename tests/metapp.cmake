## Make the meta-library for the application to prevent HUGE builds.
file(GLOB_RECURSE TEST_APP_SRCS
    "${CMAKE_SOURCE_DIR}/src/app/ui/*.c*"
    "${CMAKE_SOURCE_DIR}/src/app/services/*.c*"
    "${CMAKE_SOURCE_DIR}/src/app/sessions/*.c*")
file (GLOB TEST_APP_BASE_SRCS
    "${CMAKE_SOURCE_DIR}/src/app/*.c*")

list(APPEND TEST_APP_SRCS ${TEST_APP_BASE_SRCS})
list(REMOVE_ITEM TEST_APP_SRCS "${CMAKE_SOURCE_DIR}/src/app/main.cpp")

set(TEST_COMPILE_DEFINITIONS ${SPCHCNTRL_COMPILE_DEFINITIONS} BUILDING_TEST)
set(TEST_COMPILE_FLAGS ${SPCHCNTRL_COMPILE_FLAGS})
set(TEST_LIBRARIES ${SPCHCNTRL_APP_LIBRARIES}
                    speechcontrol
                    ${QT_QTWEBKIT_LIBRARY}
                    ${QT_QTDECLARATIVE_LIBRARY}
                    ${QT_QTTEST_LIBRARY})
set(TEST_INCLUDE_DIRS ${SPCHCNTRL_INCLUDE_DIRS}
    ${SPCHCNTRL_LIB_INCLUDE_DIRS}
    ${QT_INCLUDE_DIRS}
    "${PROJECT_BINARY_DIR}"
    "${CMAKE_BINARY_DIR}/src/lib"
    "${CMAKE_SOURCE_DIR}/src"
    "${CMAKE_SOURCE_DIR}/src/lib")

include_directories("${CMAKE_BINARY_DIR}/src/app"
                    "${CMAKE_BINARY_DIR}/src/lib")

add_library(test-appcore SHARED ${TEST_APP_SRCS})
target_link_libraries(test-appcore ${TEST_LIBRARIES} ${SPCHCNTRL_APP_LIBRARIES})
set_target_properties(test-appcore PROPERTIES
    COMPILE_DEFINITIONS "${SPCHCNTRL_APP_COMPILE_DEFINITIONS}"
    COMPILE_FLAGS "${SPCHCNTRL_APP_COMPILE_FLAGS}"
    EchoString "Built core application library for test-suite."
    EXCLUDE_FROM_ALL ON)

add_dependencies(test-appcore speechcontrol-frontend
                              speechcontrol)
