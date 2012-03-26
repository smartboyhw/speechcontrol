cmake_minimum_required(VERSION 2.8)

add_subdirectory(voxforge)
add_subdirectory(dictation)
add_subdirectory(desktopcontrol)

install(FILES ${SPCHCNTRL_SERVICES_HDRS}
        DESTINATION "${SPCHCNTRL_APP_INCLUDE_DIR}/services"
        COMPONENT "dev")