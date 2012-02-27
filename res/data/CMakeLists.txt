cmake_minimum_required(VERSION 2.8)

if (KDE4_FOUND)
    install(FILES "${PROJECT_SOURCE_DIR}/res/data/SpeechControl-KDE.desktop"
            DESTINATION ${XDG_APPS_INSTALL_DIR})
endif (KDE4_FOUND)

install(FILES "${PROJECT_SOURCE_DIR}/res/data/SpeechControl-Qt.desktop"
        DESTINATION "${CMAKE_INSTALL_PREFIX}/share/applications")
