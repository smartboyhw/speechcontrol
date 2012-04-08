cmake_minimum_required(VERSION 2.8)

configure_file("speechcontrol.pc.in"
               "${PROJECT_BINARY_DIR}/src/lib/speechcontrol.pc" @ONLY)

install(FILES "${PROJECT_SOURCE_DIR}/res/data/SpeechControl.desktop"
        DESTINATION "${CMAKE_INSTALL_PREFIX}/share/applications")

install(FILES "${PROJECT_BINARY_DIR}/src/lib/speechcontrol.pc"
        DESTINATION "${CMAKE_INSTALL_PREFIX}/share/pkgconfig")

add_subdirectory(contents)