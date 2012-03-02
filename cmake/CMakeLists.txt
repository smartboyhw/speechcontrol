cmake_minimum_required(VERSION 2.8)

file(GLOB CMAKE_FILES "*.cmake")

install(FILES ${CMAKE_FILES}
        DESTINATION ${CMAKE_INSTALL_PREFIX}/share/cmake-2.8/Modules)
install(FILES ${CMAKE_FIND_MODULES}
    DESTINATION
    "${CMAKE_INSTALL_PREFIX}/share/cmake-${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION}/Modules"
    COMPONENT dev)
