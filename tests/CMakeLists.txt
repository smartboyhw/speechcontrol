project(SpeechControlTests)

if(WITH_TESTS)
    message(STATUS "Building tests enabled.")

    ## required packages
    list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/Modules")
    include(CTest)
    enable_testing()

    ## Required packages
    pkg_search_module(CPPTEST libcpptest)
    find_package(ExecInfo)
    if(EXECINFO_FOUND)
        list(APPEND TEST_COMPILE_DEFINITIONS "HAVE_EXECINFO")
        if (EXECINFO_LIBRARIES)
            list(APPEND TEST_LIBRARIES ${EXECINFO_LIBRARIES})
        endif(EXECINFO_LIBRARIES)

        list(APPEND TEST_INCLUDE_DIRS ${EXECINFO_INCLUDES})
    endif(EXECINFO_FOUND)
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
    file(GLOB SPCHCNTRL_APP_TEST_SRCS "lib/*.cpp" "app/*.cpp")

    foreach(TEST_SRC ${SPCHCNTRL_APP_TEST_SRCS})
        string(REPLACE ".cpp" "" TEST_ID ${TEST_SRC})
        string(REPLACE "${PROJECT_SOURCE_DIR}/" "" TEST_ID ${TEST_ID})
        string(REPLACE "/" "_" TEST_ID ${TEST_ID})
        string(CONFIGURE "${TEST_BUILD_TEMPLATE}" TEST_DATA @ONLY)

        set(TEST_SRC "../${TEST_SRC}")
        set(TEST_BUILDFILE "${TEST_CMAKE_DIR}/${TEST_ID}/CMakeLists.txt")

        file(MAKE_DIRECTORY "${TEST_CMAKE_DIR}/${TEST_ID}")
        file(WRITE ${TEST_BUILDFILE} ${TEST_DATA})
        add_subdirectory("${TEST_CMAKE_DIR}/${TEST_ID}")
    endforeach()
else(WITH_TESTS)
    message(STATUS "Not generating tests.")
endif(WITH_TESTS)