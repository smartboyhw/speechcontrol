cmake_minimum_required(VERSION 2.8)

file(GLOB libmllr_SRCS "*.c")
file(GLOB libmllr_HDRS "*.h")

add_library(mllr SHARED ${libmllr_SRCS})