cmake_minimum_required(VERSION 2.8)

## Setting up KDE integration of SpeechControl; if detected, use.
find_package(KDE4 4.7.4)
if(WITH_KDE_SUPPORT AND KDE4_FOUND)
        ## Includes and new packages to add.
        list(APPEND CMAKE_MODULE_PATH ${KDE4_MODULE_DIR})
        include (KDE4Defaults)
        include (KDE4Macros)

        message(STATUS "KDE integration enabled.")

        file(GLOB SPCHCNTRL_KDE_APP_SRCS "*.cpp")

        option(WITH_KDE_PLASMOIDS "Build plasmoids for the KDE desktop." ON)

        ## Variables
        set(SPCHCNTRL_APP_KDE_UI ${SPCHCNTRL_UI})
        set(SPCHCNTRL_APP_KDE_COMPILE_FLAGS ${SPCHCNTRL_APP_COMPILE_FLAGS})
        set(SPCHCNTRL_APP_KDE_INCLUDE_DIRS ${SPCHCNTRL_APP_INCLUDE_DIRS}
            ${KDE4_INCLUDES})
        set(SPCHCNTRL_APP_KDE_LIBRARIES ${SPCHCNTRL_APP_LIBRARIES}
            ${KDE4_KDECORE_LIBS}
            ${KDE4_KDEUI_LIBRARY}
            ${KDE4_SOLID_LIBS})
        set(SPCHCNTRL_APP_KDE_DEFINTIONS "HAVE_KDE")
        set(SPCHCNTRL_KDE_APP_SCPP ${SPCHCNTRL_APP_SRCS}
            ${SPCHCNTRL_WINDOWS_SRCS}
            ${SPCHCNTRL_VOXFORGE_SRCS}
            ${SPCHCNTRL_DICTATION_SRCS}
            ${SPCHCNTRL_SESSIONS_SRCS}
            ${SPCHCNTRL_DESKTOPCONTROL_SRCS}
            ${SPCHCNTRL_KDE_APP_SRCS})
        set(SPCHCNTRL_APP_KDE_BUILD_INCLUDE_DIRS ${SPCHCNTRL_APP_BUILD_INCLUDE_DIRS}
            ${SPCHCNTRL_APP_KDE_INCLUDE_DIRS})

        ## User facing code
        kde4_add_ui_files(SPCHCNTRL_KDE_APP_SCPP ${SPCHCNTRL_APP_KDE_UI})

        ## Define the target.
        kde4_add_executable(speechcontrol-kde ${SPCHCNTRL_KDE_APP_SCPP})

        target_link_libraries(speechcontrol-kde ${SPCHCNTRL_APP_KDE_LIBRARIES})

        set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS})
        set_target_properties(speechcontrol-kde PROPERTIES
            COMPILE_DEFINITIONS "${SPCHCNTRL_APP_KDE_DEFINTIONS}"
            COMPILE_DEFINITIONS_DEBUG "__KDE_HAVE_GCC_VISIBILITY=ON;"
            DEFINE_SYMBOL "SPCHCNTRL_KDE_APP"
            COMPILE_FLAGS "${SPCHCNTRL_APP_KDE_COMPILE_FLAGS}"
            PROJECT_LABEL "SpeechControl-KDE")

        get_target_property(_srd speechcontrol-kde SOURCES)

        ## Install stuff. :)
        install(TARGETS speechcontrol-kde
                DESTINATION ${BIN_INSTALL_DIR})

        include_directories(${SPCHCNTRL_APP_KDE_BUILD_INCLUDE_DIRS})

        if (WITH_KDE_PLASMOIDS)
            message(STATUS "Building plasmoids for SpeechControl.")
            add_subdirectory(plasmoids)
        endif(WITH_KDE_PLASMOIDS)
else(WITH_KDE_SUPPORT AND KDE4_FOUND)
    message(STATUS "KDE integration disabled.")
endif(WITH_KDE_SUPPORT AND KDE4_FOUND)