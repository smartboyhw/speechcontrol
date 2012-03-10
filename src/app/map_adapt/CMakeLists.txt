cmake_minimum_required(VERSION 2.8)

file(GLOB MAPADAPT_SRCS "*.c")
file(GLOB MAPADAPT_HDRS "*.h")

set(MAPADAPT_INCLUDE_DIRS ${SPHINXBASE_INCLUDE_DIRS})
set(MAPADAPT_LIBRARIES ${SPHINXBASE_LIBRARIES}
    common modinv io mllr s2io clust)

include_directories(${MAPADAPT_INCLUDE_DIRS})

add_executable(map_adapt ${MAPADAPT_SRCS})
target_link_libraries(map_adapt ${MAPADAPT_LIBRARIES})

