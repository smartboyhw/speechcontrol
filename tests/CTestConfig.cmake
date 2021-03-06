## This file should be placed in the root directory of your project.
## Then modify the CMakeLists.txt file in the root directory of your
## project to incorporate the testing dashboard.
## # The following are required to uses Dart and the Cdash dashboard
##   ENABLE_TESTING()
##   INCLUDE(CTest)

set(CTEST_PROJECT_NAME "SpeechControl")
set(CTEST_NIGHTLY_START_TIME "01:00:00 UTC")

set(CTEST_DROP_METHOD "http")
set(CTEST_DROP_SITE "thesii.org")
set(CTEST_DROP_LOCATION "/Cdash/submit.php?project=SpeechControl")
set(CTEST_DROP_SITE_CDASH TRUE)

set(CTEST_DROP_SITE_CDASH TRUE)
set(CTEST_OUTPUT_ON_FAILURE ON)
set(CTEST_UPDATE_COMMAND "git")
set(CTEST_SOURCE_DIRECTORY "${CMAKE_SOURCE_DIR}")
set(CTEST_BINARY_DIRECTORY "${CMAKE_BINARY_DIR}")