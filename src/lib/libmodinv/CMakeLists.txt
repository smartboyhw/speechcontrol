cmake_minimum_required(VERSION 2.8)

file(GLOB libmodinv_SRCS "*.c")
file(GLOB libmodinv_HDRS "*.h")

add_library(st_modinv SHARED ${libmodinv_SRCS})

install(TARGETS st_modinv
        DESTINATION ${SPHINXTRAIN_LIB_DIR})

install(FILES ${libmodinv_HDRS}
        DESTINATION "${SPHINXTRAIN_INCLUDE_DIR}/modinv")