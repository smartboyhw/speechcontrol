cmake_minimum_required(VERSION 2.8)

include(SpeechControlDefaults)

file(GLOB SPCHCNTRL_CONTENT_FILES "*.spch")

install(FILES ${SPCHCNTRL_CONTENT_FILES}
        DESTINATION "${SPCHCNTRL_DATA_CONTENT_DIR}")