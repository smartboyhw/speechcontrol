cmake_minimum_required(VERSION 2.8)

if (WITH_PLUGINS)
    set(SpeechControlApplication_DIR "${PROJECT_BINARY_DIR}/InstallFiles")
    find_package(SpeechControlApplication)
    message(STATUS "Plug-ins are being built. Ensure that you've installed the SpeechControl application's headers first before attempting to build the plug-ins.")
    add_custom_target(plugins
                      COMMENT "Building all plug-ins...")

    ## directories
    add_subdirectory(python)
    add_subdirectory(dsktpcntrl-applications)

endif(WITH_PLUGINS)
