cmake_minimum_required(VERSION 2.8)

file(GLOB libmllr_SRCS "*.c")
file(GLOB libmllr_HDRS "*.h")

add_library(mllr SHARED ${libmllr_SRCS})

install(TARGETS mllr
        DESTINATION ${SPHINXTRAIN_LIB_DIR})

install(FILES ${libmllr_HDRS}
        DESTINATION "${SPHINXTRAIN_INCLUDE_DIR}/libmllr")