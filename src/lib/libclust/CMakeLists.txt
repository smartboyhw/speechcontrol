cmake_minimum_required(VERSION 2.8)

file(GLOB libclust_SRCS "*.c")
file(GLOB libclust_HDRS "*.h")

add_library(st_clust SHARED ${libclust_SRCS})

install(TARGETS st_clust
        DESTINATION ${SPHINXTRAIN_LIB_DIR})

install(FILES ${libclust_HDRS}
        DESTINATION "${SPHINXTRAIN_INCLUDE_DIR}/clust")