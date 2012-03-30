cmake_minimum_required(VERSION 2.8)

if (WITH_PLUGINS)
    set(SpeechControlApplication_DIR "${PROJECT_BINARY_DIR}/src/app/InstallFiles")
    set(SpeechControlLibrary_DIR "${PROJECT_BINARY_DIR}/src/lib/InstallFiles")
    set(SPCHCNTRL_PLUGIN_BUILD_INCLUDE_DIRS "${CMAKE_SOURCE_DIR}/src"
        "${CMAKE_SOURCE_DIR}/src/lib"
        "${CMAKE_SOURCE_DIR}/src/app")

    find_package(SpeechControlApplication QUIET)
    find_package(SpeechControlLibrary QUIET)
    message(STATUS "Plug-ins are being built.")

    add_custom_target(plugins ALL
        DEPENDS speechcontrol-frontend
        COMMENT "Built plug-ins.")

    ## directories
    #add_subdirectory(python)
    add_subdirectory(transcriber)
    add_subdirectory(desktopctl)

endif(WITH_PLUGINS)
