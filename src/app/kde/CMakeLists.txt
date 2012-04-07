cmake_minimum_required(VERSION 2.8)

## Setting up KDE integration of SpeechControl; if detected, use.
if(WITH_KDE_SUPPORT)
    find_package(KDE4 4.7.4)
    if (KDE4_FOUND)
        message(STATUS "KDE integration enabled.")
        list(APPEND CMAKE_MODULE_PATH ${KDE4_MODULE_DIR})
        add_subdirectory(plasmoids)
    else(KDE4_FOUND)
        set(WITH_KDE_SUPPORT OFF)
        message(STATUS "KDE integration disabled.")
    endif (KDE4-NOTFOUND)
endif(WITH_KDE_SUPPORT)