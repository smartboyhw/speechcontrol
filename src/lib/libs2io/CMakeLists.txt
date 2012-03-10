cmake_minimum_required(VERSION 2.8)

file(GLOB libs2io_SRCS "*.c")
file(GLOB libs2io_HDRS "*.h")

add_library(st_s2io SHARED ${libs2io_SRCS})

install(TARGETS st_s2io
        DESTINATION ${SPHINXTRAIN_LIB_DIR})

install(FILES ${libs2io_HDRS}
        DESTINATION "${SPHINXTRAIN_INCLUDE_DIR}/s2io")