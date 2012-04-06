cmake_minimum_required(VERSION 2.8)
project(SpeechControlTests)

include(SpeechControlDefaults)
if(WITH_TESTS)
    message(STATUS "Building tests enabled.")
    if (TARGET speechcontrol)
        set(SpeechControlLibrary_DIR "${CMAKE_BINARY_DIR}/src/lib/InstallFiles")
    endif (TARGET speechcontrol)

    if (TARGET speechcontrol-frontend)
        set(SpeechControlApplication_DIR "${CMAKE_BINARY_DIR}/src/app/InstallFiles")
    endif (TARGET speechcontrol-frontend)

    set(TEST_INCLUDE_FILE "${CMAKE_SOURCE_DIR}/tests/CTestConfig.cmake")

    configure_file("${PROJECT_SOURCE_DIR}/CTestCustom.cmake.in"
                   "${PROJECT_BINARY_DIR}/CTestCustom.cmake" @ONLY)

    include(CTest)
    enable_testing()

    add_custom_target(tests)
    include(metapp.cmake)

    file(READ "template.build" TEST_BUILD_TEMPLATE)

    ## source files
    file(GLOB SPCHCNTRL_APP_TEST_SRCS "src/app/*.cpp")
    file(GLOB SPCHCNTRL_LIB_TEST_SRCS "src/lib/*.cpp")
    file(GLOB SPCHCNTRL_DBUS_TEST_SRCS "src/dbus/*.cpp")

    macro(buildtest _test_mode _test_src)
        set(TEST_MODE "${_test_mode}")
        set(TEST_SRC "${_test_src}")
        set(TEST_CONFIG_CODE "# None provided.")
        string(REPLACE ".cpp" "" TEST_ID "${_test_src}")
        string(REPLACE "${PROJECT_SOURCE_DIR}/src/" "" TEST_ID ${TEST_ID})
        string(REPLACE "/" "_" TEST_ID ${TEST_ID})

        ## Include test-specific data.
        if (EXISTS "${CMAKE_SOURCE_DIR}/tests/config/${TEST_ID}.cmake.in")
            file(READ "${CMAKE_SOURCE_DIR}/tests/config/${TEST_ID}.cmake.in" TEST_CONFIG_CODE)
            string(CONFIGURE "${TEST_CONFIG_CODE}" TEST_CONFIG_CODE @ONLY)
        endif (EXISTS "${CMAKE_SOURCE_DIR}/tests/config/${TEST_ID}.cmake.in")

        string(CONFIGURE "${TEST_BUILD_TEMPLATE}" TEST_DATA @ONLY)

        set(TEST_BUILDFILE "${PROJECT_BINARY_DIR}/${TEST_ID}.cmake")
        file(WRITE ${TEST_BUILDFILE} ${TEST_DATA})
        include(${TEST_BUILDFILE})
    endmacro(buildtest _test_mode _test_src)

    foreach(TEST_LIB_SRC ${SPCHCNTRL_LIB_TEST_SRCS})
        buildtest("LIB" "${TEST_LIB_SRC}")
    endforeach()

    foreach(TEST_APP_SRC ${SPCHCNTRL_APP_TEST_SRCS})
        buildtest("APP" "${TEST_APP_SRC}")
    endforeach()

    foreach(TEST_DBUS_SRC ${SPCHCNTRL_DBUS_TEST_SRCS})
        buildtest("DBUS" "${TEST_DBUS_SRC}")
    endforeach()

    if (WITH_PLUGINS)
        file(GLOB SPCHCNTRL_PLUGINS_TEST_SRCS "src/plugins/*.cpp")
        foreach(TEST_PLUGINS_SRC ${SPCHCNTRL_PLUGINS_TEST_SRCS})
            buildtest("PLUGIN" "${TEST_PLUGINS_SRC}")
        endforeach()
    endif(WITH_PLUGINS)

else(WITH_TESTS)
    message(STATUS "Not generating tests.")
endif(WITH_TESTS)