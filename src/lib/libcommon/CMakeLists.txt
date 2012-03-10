cmake_minimum_required(VERSION 2.8)

file(GLOB libcommon_SRCS "*.c")
file(GLOB libcommon_HDRS "*.h")

set(LIBCOMMON_LIBRARIES st_clust)

add_library(st_common SHARED ${libcommon_SRCS})
target_link_libraries(st_common ${LIBCOMMON_LIBRARIES})

install(TARGETS st_common
        DESTINATION ${SPHINXTRAIN_LIB_DIR})

install(FILES ${libcommon_HDRS}
        DESTINATION "${SPHINXTRAIN_INCLUDE_DIR}/common")