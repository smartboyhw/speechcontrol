cmake_minimum_required(VERSION 2.8)

install(FILES "${PROJECT_SOURCE_DIR}/res/data/SpeechControl.desktop"
        DESTINATION "${CMAKE_INSTALL_PREFIX}/share/applications")

install(DIRECTORY "${PROJECT_SOURCE_DIR}/res/data/contents"
        DESTINATION "${SPCHCNTRL_DATA_DIR}")