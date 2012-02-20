project(SpeechControlTests)

## required packages
include(CTest)
pkg_search_module(CPPTEST libcppunit)

## variables
set(SPCHCNTRL_TEST_INCLUDE_DIRS "${CPPTEST_INCLUDES}"
	${SPCHCNTRL_LIB_INCLUDE_DIRS}
    ${SPCHCNTRL_APP_INCLUDE_DIRS})
set(SPCHCNTRL_TEST_LIBRARIES "${CPPTEST_LIBRARIES}"
	${SPCHCNTRL_LIB_LIBRARIES}
	${SPCHCNTRL_APP_LIBRARIES})
set(TEST_CMAKE_DIR "cmake")

## Add CDash information.
set(CTEST_PROJECT_NAME "SpeechControl")
set(CTEST_NIGHTLY_START_TIME "01:00:00 UTC")
set(CTEST_DROP_METHOD "http")
set(CTEST_DROP_SITE "dev.thesii.org/cdash")
set(CTEST_DROP_LOCATION "/submit.php?project=spchcntrl")
set(CTEST_DROP_SITE_CDASH TRUE)

file(REMOVE_RECURSE ${TEST_CMAKE_DIR})
file(MAKE_DIRECTORY ${TEST_CMAKE_DIR})
file(READ "template.build" TEST_BUILD_TEMPLATE)
## source files
file(GLOB SPCHCNTRL_APP_TEST_SRCS RELATIVE "${PROJECT_SOURCE_DIR}" "app/*.cpp"
                                                                   "lib/*.cpp")

foreach(TEST_SRC ${SPCHCNTRL_APP_TEST_SRCS})
    string(REPLACE ".cpp" "" TEST_ID ${TEST_SRC})
    string(REPLACE "/" "_" TEST_ID ${TEST_ID})
    string(CONFIGURE "${TEST_BUILD_TEMPLATE}" TEST_DATA @ONLY)
    set(TEST_SRC "../${TEST_SRC}")
    set(TEST_BUILDFILE "${TEST_CMAKE_DIR}/${TEST_ID}.build")
    file(WRITE ${TEST_BUILDFILE} ${TEST_DATA})
    include(${TEST_BUILDFILE})
endforeach()
