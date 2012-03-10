cmake_minimum_required(VERSION 2.8)

file(GLOB libio_SRCS "*.c")
file(GLOB libio_HDRS "*.h")

add_library(io SHARED ${libio_SRCS})