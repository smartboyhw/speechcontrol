project(SpeechControlTests)

if(WITH_TESTS)
    message(STATUS "Building tests enabled.")
    set(SpeechControlLibrary_DIR "${CMAKE_BINARY_DIR}/src/lib/InstallFiles")
    set(SpeechControlApplication_DIR "${CMAKE_BINARY_DIR}/src/app/InstallFiles")

    add_custom_target(tests ALL
        COMMAND "ctest"
        WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/tests"
        COMMENT "Invoking CTest..")

    ## Variables
    set(TEST_CMAKE_DIR "cmake")
    include(CTest)
    enable_testing()

    file(REMOVE_RECURSE ${TEST_CMAKE_DIR})
    file(MAKE_DIRECTORY ${TEST_CMAKE_DIR})
    file(READ "template.build" TEST_BUILD_TEMPLATE)

    ## source files
    file(GLOB SPCHCNTRL_APP_TEST_SRCS "app/*.cpp")
    file(GLOB SPCHCNTRL_LIB_TEST_SRCS "lib/*.cpp")

    macro(buildtest _test_mode _test_src)
        set(TEST_MODE "${_test_mode}")
        set(TEST_SRC "${_test_src}")
        set(TEST_CONFIG_CODE "# Node provided.")
        string(REPLACE ".cpp" "" TEST_ID "${_test_src}")
        string(REPLACE "${PROJECT_SOURCE_DIR}/" "" TEST_ID ${TEST_ID})
        string(REPLACE "/" "_" TEST_ID ${TEST_ID})

        if (EXISTS "${CMAKE_SOURCE_DIR}/tests/config/${TEST_ID}.cmake.in")
            file(READ "${CMAKE_SOURCE_DIR}/tests/config/${TEST_ID}.cmake.in" TEST_CONFIG_CODE)
            string(CONFIGURE "${TEST_CONFIG_CODE}" TEST_CONFIG_CODE @ONLY)
        endif (EXISTS "${CMAKE_SOURCE_DIR}/tests/config/${TEST_ID}.cmake.in")

        string(CONFIGURE "${TEST_BUILD_TEMPLATE}" TEST_DATA @ONLY)

        set(TEST_BUILDFILE "${TEST_CMAKE_DIR}/${TEST_ID}/CMakeLists.txt")

        file(MAKE_DIRECTORY "${TEST_CMAKE_DIR}/${TEST_ID}")
        file(WRITE ${TEST_BUILDFILE} ${TEST_DATA})
        add_subdirectory("${TEST_CMAKE_DIR}/${TEST_ID}")
    endmacro(buildtest _test_mode _test_src)

    foreach(TEST_LIB_SRC ${SPCHCNTRL_LIB_TEST_SRCS})
        buildtest("LIB" "${TEST_LIB_SRC}")
    endforeach()

    foreach(TEST_APP_SRC ${SPCHCNTRL_APP_TEST_SRCS})
        buildtest("APP" "${TEST_APP_SRC}")
    endforeach()
else(WITH_TESTS)
    message(STATUS "Not generating tests.")
endif(WITH_TESTS)