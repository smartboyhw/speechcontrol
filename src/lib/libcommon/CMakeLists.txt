cmake_minimum_required(VERSION 2.8)

file(GLOB libcommon_SRCS "*.c")
file(GLOB libcommon_HDRS "*.h")

set(LIBCOMMON_LIBRARIES clust)

add_library(common SHARED ${libcommon_SRCS})
target_link_libraries(common ${LIBCOMMON_LIBRARIES})