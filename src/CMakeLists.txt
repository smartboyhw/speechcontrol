cmake_minimum_required(VERSION 2.8)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

## Variables shared by app/ and plugins/
set(SPCHCNTRL_BUILDING ON)

add_subdirectory(lib)

if(WITH_FRONTEND)
    add_subdirectory(app)
endif()

if(WITH_PLUGINS)
    add_subdirectory(plugins)
endif()

if(WITH_PYTHON_BINDINGS)
    add_subdirectory(python)
endif()
