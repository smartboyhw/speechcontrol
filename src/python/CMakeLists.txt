INCLUDE_DIRECTORIES(
    ${PYTHON_INCLUDE_PATH}
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}
)

# Use this as a guide:
# https://github.com/OpenKinect/libfreenect/blob/master/wrappers/python/CMakeLists.txt

# How to Cython the .pyx file
add_custom_command(OUTPUT speechcontrol.cpp
  COMMAND
  ${CYTHON_EXECUTABLE} -o speechcontrol.cpp -f -I ${CMAKE_CURRENT_SOURCE_DIR} --cplus "${CMAKE_CURRENT_SOURCE_DIR}/speechcontrol.pyx")

list(APPEND ADDITIONAL_MAKE_CLEAN_FILES "speechcontrol.cpp")

# speechcontrol Python bindings
ADD_LIBRARY(speechcontrol_cython SHARED
    speechcontrol.cpp
)

TARGET_LINK_LIBRARIES(speechcontrol_cython
    speechcontrol
)

SET_TARGET_PROPERTIES(speechcontrol_cython PROPERTIES
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

