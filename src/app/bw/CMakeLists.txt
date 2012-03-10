project(SphinxTrainApp-Bw)
cmake_minimum_required(VERSION 2.8)

file(GLOB BW_SRCS "*.c")
file(GLOB BW_HDRS "*.h")

set(BW_INCLUDE_DIRS ${SPHINXBASE_INCLUDE_DIRS})
set(BW_LIBRARIES ${SPHINXBASE_LIBRARIES}
                 st_common st_modinv st_io st_mllr st_s2io st_clust)

include_directories(${BW_INCLUDE_DIRS})

add_executable(st_bw ${BW_SRCS})
target_link_libraries(st_bw ${BW_LIBRARIES})

install(TARGETS st_bw
        DESTINATION ${SPHINXTRAIN_BIN_DIR})