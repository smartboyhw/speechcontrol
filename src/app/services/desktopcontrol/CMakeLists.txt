cmake_minimum_required(VERSION 2.8)

install(FILES ${SPCHCNTRL_DESKTOPCONTROL_HDRS}
        DESTINATION "${SPCHCNTRL_APP_INCLUDE_DIR}/services/desktopcontrol"
        COMPONENT "dev")
