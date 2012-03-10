cmake_minimum_required(VERSION 2.8)

file(GLOB libio_SRCS "*.c")
file(GLOB libio_HDRS "*.h")

set(LIBIO_LIBRARIES common)

add_library(io SHARED ${libio_SRCS})
target_link_libraries(io ${LIBIO_LIBRARIES})