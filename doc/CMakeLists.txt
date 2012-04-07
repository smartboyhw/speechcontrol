cmake_minimum_required(VERSION 2.8)
cmake_policy(SET CMP0017 OLD)

if(WITH_DOCS)
    add_subdirectory(doxygen)
    add_subdirectory(docbook)
endif(WITH_DOCS)