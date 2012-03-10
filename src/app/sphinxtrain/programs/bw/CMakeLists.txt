cmake_minimum_required(VERSION 2.8)

file(GLOB BW_SRCS "*.c")
file(GLOB BW_HDRS "*.h")

set(BW_INCLUDE_DIRS ${SPHINXBASE_INCLUDE_DIRS})
set(BW_LIBRARIES ${SPHINXBASE_LIBRARIES}
                 common modinv io mllr s2io)

include_directories(${BW_INCLUDE_DIRS})

add_executable(bw ${BW_SRCS})
target_link_libraries(bw ${BW_LIBRARIES})
