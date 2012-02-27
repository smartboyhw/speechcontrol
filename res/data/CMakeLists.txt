cmake_minimum_required(VERSION 2.8)

if (WITH_KDE_SUPPORT)
    find_package(KDE4 QUIET)
    list(APPEND CMAKE_MODULE_PATH ${KDE4_MODULE_DIR})
    include (KDE4Defaults)
    include (KDE4Macros)
    install(FILES "${PROJECT_SOURCE_DIR}/res/data/SpeechControl-KDE.desktop"
            DESTINATION ${XDG_APPS_INSTALL_DIR})
endif (WITH_KDE_SUPPORT)

install(FILES "${PROJECT_SOURCE_DIR}/res/data/SpeechControl-Qt.desktop"
        DESTINATION "${CMAKE_INSTALL_PREFIX}/share/applications")
