cmake_minimum_required(VERSION 2.8)

file(GLOB SPCHCNTRL_PLASMOID_SRCS "*.cpp")

## Variables
set(SPCHCNTRL_PLASMOID_INCLUDE_DIRS ${KDE4_INCLUDES})
set(SPCHCNTRL_PLASMOID_LIBRARIES ${KDE4_PLASMA_LIBS}
    ${KDE4_KDEUI_LIBS})
set(SPCHCNTRL_PLASMOID_DEFINITONS ${QT_DEFINITIONS}
    ${KDE4_DEFINITIONS})

include_directories(${SPCHCNTRL_PLASMOID_INCLUDE_DIRS})

kde4_add_plugin(plasmoid-speechcontrol ${SPCHCNTRL_PLASMOID_SRCS})
target_link_libraries(plasmoid-speechcontrol ${SPCHCNTRL_PLASMOID_LIBRARIES})
set_target_properties(plasmoid-speechcontrol PROPERTIES
    COMPILE_DEFINITIONS SPCHCNTRL_PLASMOID_DEFINITONS)

install(TARGETS plasmoid-speechcontrol
        DESTINATION ${PLUGIN_INSTALL_DIR})
