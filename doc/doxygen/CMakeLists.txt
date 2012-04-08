cmake_minimum_required(VERSION 2.8)

find_package(Doxygen REQUIRED)
if (DOXYGEN_FOUND)
    set(SPCHCNTRL_DOC_SOURCES_DIR "${PROJECT_SOURCE_DIR}/src")
    set(SPCHCNTRL_DOC_BUILD_DIR "${PROJECT_BINARY_DIR}/doc/doxygen")
    set(SPCHCNTRL_APP_DOC_BUILD_DIR "${SPCHCNTRL_DOC_BUILD_DIR}/app")
    set(SPCHCNTRL_LIB_DOC_BUILD_DIR "${SPCHCNTRL_DOC_BUILD_DIR}/lib")

    configure_file("Doxyfile.app.in" "${PROJECT_BINARY_DIR}/doc/doxygen/Doxyfile.app" @ONLY)
    configure_file("Doxyfile.lib.in" "${PROJECT_BINARY_DIR}/doc/doxygen/Doxyfile.lib" @ONLY)

    ## The magical targets
    add_custom_target(docs-app
        COMMAND ${DOXYGEN_EXECUTABLE} ${PROJECT_BINARY_DIR}/doc/doxygen/Doxyfile.app
        SOURCES "${PROJECT_BINARY_DIR}/doc/doxygen/Doxyfile.app"
        COMMENT "Building documentation for SpeechControl application (API)...")
    add_custom_target(docs-lib
        COMMAND ${DOXYGEN_EXECUTABLE} ${PROJECT_BINARY_DIR}/doc/doxygen/Doxyfile.lib
        SOURCES "${PROJECT_BINARY_DIR}/doc/doxygen/Doxyfile.lib"
        COMMENT "Building documentation for SpeechControl library (API)...")
    add_custom_target(docs ALL
        COMMENT "Built documentation.")

    add_dependencies(docs docs-lib docs-app)
    add_dependencies(speechcontrol-app docs-app)
    add_dependencies(speechcontrol docs-lib)

    ## Installs
    install(DIRECTORY ${SPCHCNTRL_LIB_DOC_BUILD_DIR}/html/
            DESTINATION ${SPCHCNTRL_DOC_INSTALL_DIR}/libspeechcontrol)

    install(DIRECTORY ${SPCHCNTRL_APP_DOC_BUILD_DIR}/html/
            DESTINATION ${SPCHCNTRL_DOC_INSTALL_DIR}/speechcontrol/api)
endif(DOXYGEN_FOUND)
