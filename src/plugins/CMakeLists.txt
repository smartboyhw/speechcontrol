cmake_minimum_required(VERSION 2.8)

if(WITH_PLUGINS)
    add_custom_target(plugins
                      COMMENT "Building all plugins...")

    ## directories
    add_subdirectory(indicator-applet)
    add_subdirectory(python)
endif(WITH_PLUGINS)