cmake_minimum_required(VERSION 2.8)

file(GLOB libclust_SRCS "*.c")
file(GLOB libclust_HDRS "*.h")

add_library(clust SHARED ${libclust_SRCS})