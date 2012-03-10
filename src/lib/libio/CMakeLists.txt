cmake_minimum_required(VERSION 2.8)

file(GLOB libio_SRCS "*.c")
file(GLOB libio_HDRS "*.h")

set(LIBIO_LIBRARIES st_common)

add_library(st_io SHARED ${libio_SRCS})
target_link_libraries(st_io ${LIBIO_LIBRARIES})

install(TARGETS st_io
        DESTINATION ${SPHINXTRAIN_LIB_DIR})

install(FILES ${libio_HDRS}
        DESTINATION "${SPHINXTRAIN_INCLUDE_DIR}/io")