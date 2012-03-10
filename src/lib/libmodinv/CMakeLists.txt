cmake_minimum_required(VERSION 2.8)

file(GLOB libmodinv_SRCS "*.c")
file(GLOB libmodinv_HDRS "*.h")

add_library(modinv SHARED ${libmodinv_SRCS})

install(TARGETS modinv
        DESTINATION ${SPHINXTRAIN_LIB_DIR})

install(FILES ${libmodinv_HDRS}
        DESTINATION "${SPHINXTRAIN_INCLUDE_DIR}/libmodinv")