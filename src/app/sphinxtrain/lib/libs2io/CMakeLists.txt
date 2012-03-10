cmake_minimum_required(VERSION 2.8)

file(GLOB libs2io_SRCS "*.c")
file(GLOB libs2io_HDRS "*.h")

add_library(s2io SHARED ${libs2io_SRCS})