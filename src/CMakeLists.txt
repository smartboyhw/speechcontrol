cmake_minimum_required(VERSION 2.8)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

## Variables shared by app/ and plugins/
set(SPCHCNTRL_BUILDING ON)

add_subdirectory(lib)
add_subdirectory(app)
add_subdirectory(plugins)