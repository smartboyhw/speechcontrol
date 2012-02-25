cmake_minimum_required(VERSION 2.8)

configure_file("SpeechControlLibraryConfig.cmake.in"
    "${PROJECT_BINARY_DIR}/InstallFiles/SpeechControlLibraryConfig.cmake" @ONLY)

configure_file("SpeechControlLibraryConfigVersion.cmake.in"
    "${PROJECT_BINARY_DIR}/InstallFiles/SpeechControlLibraryConfigVersion.cmake" @ONLY)

install(FILES "${PROJECT_BINARY_DIR}/InstallFiles/SpeechControlLibraryConfig.cmake"
    "${PROJECT_BINARY_DIR}/InstallFiles/SpeechControlLibraryConfigVersion.cmake"
    DESTINATION "${SPCHCNTRL_CMAKE_DIR}"
    COMPONENT "dev")
