cmake_minimum_required(VERSION 2.8)
cmake_policy(SET CMP0017 OLD)

if(WITH_DOCS)
    add_subdirectory(doxygen)
    add_subdirectory(docbook)
    if (WITH_KDE_SUPPORT)
        find_package(KDE4 QUIET)
        list(APPEND CMAKE_MODULE_PATH ${KDE4_MODULE_DIR})
        include (KDE4Defaults)
        include (KDE4Macros)
        kde4_create_handbook("docbook/index.docbook"
                             INSTALL_DESTINATION "${HTML_INSTALL_DIR}/en"
                             SUBDIR speechcontrol)
    endif (WITH_KDE_SUPPORT)
endif(WITH_DOCS)