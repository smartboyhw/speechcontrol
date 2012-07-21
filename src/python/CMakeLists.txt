include_directories(
    ${PYTHON_INCLUDE_PATH}
    ${CMAKE_CURRENT_SOURCE_DIR}/..
    ${CMAKE_CURRENT_BINARY_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}/../lib
    ${SPCHCNTRL_INCLUDE_DIRS}
    ${CMAKE_CURRENT_SOURCE_DIR}/../lib
)

find_program(CYTHON_EXECUTABLE cython)

# Use this as a guide:
# https://github.com/OpenKinect/libfreenect/blob/master/wrappers/python/CMakeLists.txt

# How to Cython the .pyx file
add_custom_command(OUTPUT speechcontrol.cpp
  COMMAND
  ${CYTHON_EXECUTABLE} -o speechcontrol.cpp -f -I ${CMAKE_CURRENT_SOURCE_DIR} --cplus "${CMAKE_CURRENT_SOURCE_DIR}/speechcontrol.pyx")

list(APPEND ADDITIONAL_MAKE_CLEAN_FILES "speechcontrol.cpp")

# speechcontrol Python bindings
add_library(speechcontrol_cython SHARED
    speechcontrol.cpp
)

target_link_libraries(speechcontrol_cython
    speechcontrol
)

set_target_properties(speechcontrol_cython PROPERTIES
    PREFIX ""
    OUTPUT_NAME speechcontrol)

# direct file and path creation for stuff that doesn't need to be compiled
file(MAKE_DIRECTORY python)
# module init
file(COPY __init__.py DESTINATION python)
##

#INSTALL(FILES
    #__init__.py
    #    DESTINATION "${DATADIR}/python/speechcontrol")

