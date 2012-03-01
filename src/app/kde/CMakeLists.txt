cmake_minimum_required(VERSION 2.8)

## Setting up KDE integration of SpeechControl; if detected, use.
find_package(KDE4 4.7.4)
if(WITH_KDE_SUPPORT AND KDE4_FOUND)
    message(STATUS "KDE integration enabled.")
    list(APPEND CMAKE_MODULE_PATH ${KDE4_MODULE_DIR})

    option(WITH_KDE_PLASMOIDS "Build plasmoids for the KDE desktop." ON)

    if (WITH_KDE_PLASMOIDS)
        message(STATUS "Building plasmoids for SpeechControl.")
        add_subdirectory(plasmoids)
    endif(WITH_KDE_PLASMOIDS)
else(WITH_KDE_SUPPORT AND KDE4_FOUND)
    message(STATUS "KDE integration disabled.")
endif(WITH_KDE_SUPPORT AND KDE4_FOUND)