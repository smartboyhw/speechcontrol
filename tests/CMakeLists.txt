project(SpeechControlTests)

if(WITH_TESTS)
    message(STATUS "Building tests enabled.")

    ## required packages
    list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/Modules")
    include(CTest)
    enable_testing()

    # Execinfo is needed for generating backtraces
    include_directories("../src")

    ## Variables
    set(TEST_CMAKE_DIR "cmake")

    ## Add the meta-target to build all tests.
    add_custom_target(tests ALL)
    set_target_properties(tests PROPERTIES
        EchoString "Building tests...")

    file(REMOVE_RECURSE ${TEST_CMAKE_DIR})
    file(MAKE_DIRECTORY ${TEST_CMAKE_DIR})
    file(READ "template.build" TEST_BUILD_TEMPLATE)
    ## source files
    file(GLOB SPCHCNTRL_APP_TEST_SRCS RELATIVE "${PROJECT_SOURCE_DIR}" "app/*.cpp" "lib/*.cpp")

    foreach(TEST_SRC ${SPCHCNTRL_APP_TEST_SRCS})
        string(REPLACE ".cpp" "" TEST_ID ${TEST_SRC})
        string(REPLACE "/" "_" TEST_ID ${TEST_ID})
        string(CONFIGURE "${TEST_BUILD_TEMPLATE}" TEST_DATA @ONLY)

        set(TEST_SRC "../${TEST_SRC}")
        set(TEST_BUILDFILE "${TEST_CMAKE_DIR}/${TEST_ID}.build")

        file(WRITE ${TEST_BUILDFILE} ${TEST_DATA})
        include("${TEST_BUILDFILE}")
        message(STATUS "Test ${TEST_ID} configured; awaiting build (i.e.: 'make ${TEST_ID}')")
    endforeach()
else(WITH_TESTS)
    message(STATUS "Not generating tests.")
endif(WITH_TESTS)