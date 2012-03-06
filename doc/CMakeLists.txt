cmake_minimum_required(VERSION 2.8)
cmake_policy(SET CMP0017 OLD)

if(WITH_DOCS)
    find_package(Doxygen)
    if (DOXYGEN_FOUND)
        set(SPCHCNTRL_DOC_SOURCES_DIR "${PROJECT_SOURCE_DIR}/src")
        set(SPCHCNTRL_DOC_BUILD_DIR "${PROJECT_BINARY_DIR}/doc")
        set(SPCHCNTRL_APP_DOC_BUILD_DIR "${SPCHCNTRL_DOC_BUILD_DIR}/app")
        set(SPCHCNTRL_LIB_DOC_BUILD_DIR "${SPCHCNTRL_DOC_BUILD_DIR}/lib")

        configure_file("Doxyfile.app.in" "${PROJECT_BINARY_DIR}/Doxyfile.app" @ONLY)
        configure_file("Doxyfile.lib.in" "${PROJECT_BINARY_DIR}/Doxyfile.lib" @ONLY)

        ## The magical targets
        add_custom_target(docs-app
            COMMAND ${DOXYGEN_EXECUTABLE} ${PROJECT_BINARY_DIR}/Doxyfile.app
            SOURCES "Doxyfile.app.in"
            COMMENT "Built documentation for SpeechControl application (API).")
        add_custom_target(docs-lib
            COMMAND ${DOXYGEN_EXECUTABLE} ${PROJECT_BINARY_DIR}/Doxyfile.lib
            SOURCES "Doxyfile.lib.in"
            COMMENT "Built documentation for SpeechControl library (API).")
        add_custom_target(docs
            COMMENT "Built documentation.")

        add_dependencies(docs docs-lib docs-app)
        add_dependencies(speechcontrol-frontend docs-app)
        add_dependencies(speechcontrol docs-lib)

        ## Installs
        install(DIRECTORY ${SPCHCNTRL_LIB_DOC_BUILD_DIR}/html/
                DESTINATION ${SPCHCNTRL_DOC_INSTALL_DIR}/libspeechcontrol)

        install(DIRECTORY ${SPCHCNTRL_APP_DOC_BUILD_DIR}/html/
                DESTINATION ${SPCHCNTRL_DOC_INSTALL_DIR}/speechcontrol/api)

        if (WITH_KDE_SUPPORT)
            find_package(KDE4 QUIET)
            list(APPEND CMAKE_MODULE_PATH ${KDE4_MODULE_DIR})
            include (KDE4Defaults)
            include (KDE4Macros)
            kde4_create_handbook("speechcontrol.docbook"
                INSTALL_DESTINATION ${HTML_INSTALL_DIR})
        endif (WITH_KDE_SUPPORT)
    endif(DOXYGEN_FOUND)
endif(WITH_DOCS)