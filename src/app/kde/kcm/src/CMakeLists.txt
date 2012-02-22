cmake_minimum_required(VERSION 2.8)

file(GLOB SPCHCNTRL_KCM_SRCS "*.cpp")

kde4_add_plugin (kcm-speechcontrol ${SPCHCNTRL_KCM_SRCS})

target_link_libraries (kcm-speechcontrol ${SPCHCNTRL_KCM_LIBRARIES})

install (TARGETS kcm-speechcontrol
         DESTINATION ${PLUGIN_INSTALL_DIR})
