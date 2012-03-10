cmake_minimum_required(VERSION 2.8)

file(GLOB libcommon_SRCS "*.c")
file(GLOB libcommon_HDRS "*.h")

add_library(common SHARED ${libcommon_SRCS})