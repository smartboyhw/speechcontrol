cmake_minimum_required(VERSION 2.8)

install(FILES "speechcontrol.png"
        DESTINATION "${CMAKE_INSTALL_PREFIX}/share/icons")

install(FILES "speechcontrol.svg"
        DESTINATION "${CMAKE_INSTALL_PREFIX}/share/icons")

if (WITH_KDE_SUPPORT)
    find_package(KDE4 QUIET)
    if (KDE4_FOUND)
        list(APPEND CMAKE_MODULE_PATH ${KDE4_MODULE_DIR})
        include (KDE4Defaults)
        include (KDE4Macros)
        kde4_install_icons("${PROJECT_SOURCE_DIR}/res/img")
    endif(KDE4_FOUND)
endif(WITH_KDE_SUPPORT)