cmake_minimum_required(VERSION 2.8)

file(GLOB MAPADAPT_SRCS "*.c")
file(GLOB MAPADAPT_HDRS "*.h")

set(MAPADAPT_INCLUDE_DIRS ${SPHINXBASE_INCLUDE_DIRS})
set(MAPADAPT_LIBRARIES ${SPHINXBASE_LIBRARIES}
    st_common st_modinv st_io st_mllr st_s2io st_clust)

include_directories(${MAPADAPT_INCLUDE_DIRS})

add_executable(st_map_adapt ${MAPADAPT_SRCS})
target_link_libraries(st_map_adapt ${MAPADAPT_LIBRARIES})

install(TARGETS st_map_adapt
        DESTINATION ${SPHINXTRAIN_BIN_DIR})