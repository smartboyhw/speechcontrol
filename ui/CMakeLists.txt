project(SpeechControlUserInterface)
cmake_minimum_required(VERSION 2.8)

install(FILES "img/sc.png"
        DESTINATION "${CMAKE_INSTALL_PREFIX}/share/icons"
        RENAME "speechcontrol.png")

install(FILES "img/sc.xpm"
        DESTINATION "${CMAKE_INSTALL_PREFIX}/share/icons"
        RENAME "speechcontrol.xpm")