configure_file("${SPCHCNTRL_CMAKE_SDIR}/SpeechControlApplicationConfig.cmake.in"
    "${PROJECT_BINARY_DIR}/InstallFiles/SpeechControlApplicationConfig.cmake" @ONLY)

configure_file("${SPCHCNTRL_CMAKE_SDIR}/SpeechControlApplicationConfigVersion.cmake.in"
    "${PROJECT_BINARY_DIR}/InstallFiles/SpeechControlApplicationConfigVersion.cmake" @ONLY)

configure_file("${SPCHCNTRL_CMAKE_SDIR}/SpeechControlLibraryConfig.cmake.in"
    "${PROJECT_BINARY_DIR}/InstallFiles/SpeechControlLibraryConfig.cmake" @ONLY)

configure_file("${SPCHCNTRL_CMAKE_SDIR}/SpeechControlLibraryConfigVersion.cmake.in"
    "${PROJECT_BINARY_DIR}/InstallFiles/SpeechControlLibraryConfigVersion.cmake" @ONLY)

install(FILES "${PROJECT_BINARY_DIR}/InstallFiles/SpeechControlApplicationConfig.cmake"
    "${PROJECT_BINARY_DIR}/InstallFiles/SpeechControlApplicationConfigVersion.cmake"
    "${PROJECT_BINARY_DIR}/InstallFiles/SpeechControlLibraryConfig.cmake"
    "${PROJECT_BINARY_DIR}/InstallFiles/SpeechControlLibraryConfigVersion.cmake"
    DESTINATION "${SPCHCNTRL_CMAKE_DIR}"
    COMPONENT "dev")
