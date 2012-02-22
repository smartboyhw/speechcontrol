cmake_minimum_required(VERSION 2.8)

## Setting up KDE integration of SpeechControl; if detected, use.
find_package(KDE4)
if(WITH_KDE_SUPPORT OR KDE4_FOUND)
    set(WITH_KDE_SUPPORT ON)
    if (KDE4_FOUND)
        message(STATUS "KDE integration enabled.")

        option(WITH_KDE_PLASMOIDS       "Build plasmoids for the KDE desktop." ON)
        option(WITH_KCM_SUPPORT         "Build a KControl module."             ON)

        ## Includes and new packages to add.
        list(APPEND CMAKE_MODULE_PATH ${KDE4_MODULE_DIR})
        include (KDE4Defaults)

        ## Variables
        set(SPCHCNTRL_APP_KDE_UI ${SPCHCNTRL_UI})
        set(SPCHCNTRL_APP_KDE_COMPILE_FLAGS ${SPCHCNTRL_APP_COMPILE_FLAGS})
        set(SPCHCNTRL_APP_KDE_INCLUDE_DIRS ${SPCHCNTRL_APP_INCLUDE_DIRS} ${KDE4_INCLUDES})
        set(SPCHCNTRL_APP_KDE_LIBRARIES ${SPCHCNTRL_LIBRARIES}
            ${KDE4_KDECORE_LIBS}
            ${KDE4_KDEUI_LIBRARY}
            ${KDE4_SOLID_LIBS}
            ${QT_QTDBUS_LIBRRARY}
            spchcntrl)
        set(SPCHCNTRL_APP_KDE_DEFINTIONS "HAVE_KDE")
        set(SPCHCNTRL_APP_KDE_SCPP ${SPCHCNTRL_APP_SRCS}
            ${SPCHCNTRL_WINDOWS_SRCS})

        ## User facing code
        kde4_add_ui_files(SPCHCNTRL_APP_KDE_SCPP ${SPCHCNTRL_APP_KDE_UI})
        QT4_ADD_RESOURCES(SPCHCNTRL_APP_KDE_SCPP ${SPCHCNTRL_QRESC})

        ## Define the target.
        kde4_add_executable(speechcontrol-kde ${SPCHCNTRL_APP_KDE_SCPP})

        target_link_libraries(speechcontrol-kde ${SPCHCNTRL_APP_KDE_LIBRARIES})

        set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS})
        set_target_properties(speechcontrol-kde PROPERTIES
            COMPILE_DEFINITIONS "${SPCHCNTRL_APP_KDE_DEFINTIONS}"
            COMPILE_DEFINITIONS_DEBUG "__KDE_HAVE_GCC_VISIBILITY=ON;"
            DEFINE_SYMBOL "SPCHCNTRL_KDE_APP"
            COMPILE_FLAGS "${SPCHCNTRL_APP_KDE_COMPILE_FLAGS}"
            PROJECT_LABEL "SpeechControl-KDE")

        ## Install stuff. :)
        install(TARGETS speechcontrol-kde
                DESTINATION ${BIN_INSTALL_DIR})

        include_directories(${SPCHCNTRL_APP_KDE_INCLUDE_DIRS})

        if (WITH_KCM_SUPPORT)
            message(STATUS "Building KControl module for SpeechControl.")
            add_subdirectory(kcm)
        endif(WITH_KCM_SUPPORT)

        if (WITH_KDE_PLASMOIDS)
            message(STATUS "Building plasmoids for SpeechControl.")
            add_subdirectory(plasmoids)
        endif(WITH_KDE_PLASMOIDS)

    else(KDE4_FOUND)
        message(STATUS "KDE integration disabled; required development files not found.")
    endif(KDE4_FOUND)
else(WITH_KDE_SUPPORT OR KDE4_FOUND)
    message(STATUS "KDE integration disabled.")
endif(WITH_KDE_SUPPORT OR KDE4_FOUND)
