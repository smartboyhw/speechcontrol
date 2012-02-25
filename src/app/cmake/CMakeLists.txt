cmake_minimum_required(VERSION 2.8)

configure_file("SpeechControlApplicationConfig.cmake.in"
    "${PROJECT_BINARY_DIR}/InstallFiles/SpeechControlApplicationConfig.cmake" @ONLY)

configure_file("SpeechControlApplicationConfigVersion.cmake.in"
    "${PROJECT_BINARY_DIR}/InstallFiles/SpeechControlApplicationConfigVersion.cmake" @ONLY)

install(FILES "${PROJECT_BINARY_DIR}/InstallFiles/SpeechControlApplicationConfig.cmake"
    "${PROJECT_BINARY_DIR}/InstallFiles/SpeechControlApplicationConfigVersion.cmake"
    DESTINATION "${SPCHCNTRL_CMAKE_DIR}"
    COMPONENT "dev")
