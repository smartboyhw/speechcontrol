#
# Handle our translation work.

find_package (Gettext REQUIRED)
include_directories(${GETTEXT_INCLUDE_DIR})

file(GLOB PO_FILES "${CMAKE_SOURCE_DIR}/po/*.po")
set(MO_FILES )

foreach( PO_FILE ${PO_FILES} )
    get_filename_component( _LANG ${PO_FILE} NAME_WE )
    set( MO_FILE "${CMAKE_BINARY_DIR}/po/${_LANG}.mo" )

    add_custom_command( OUTPUT ${MO_FILE}
        COMMAND ${GETTEXT_MSGFMT_EXECUTABLE} -o ${MO_FILE} ${PO_FILE}
        WORKING_DIRECTORY "${CMAKE_BINARY_DIR}"
        DEPENDS ${PO_FILE}
    )

    install ( FILES "${CMAKE_BINARY_DIR}/po/${_LANG}.mo"
        DESTINATION "${SPCHCNTRL_LOCALE_DIR}/${_LANG}/LC_MESSAGES/"
        RENAME speechcontrol.mo )

    list(APPEND MO_FILES ${MO_FILE})
endforeach(PO_FILE)
add_custom_target(pofiles ALL DEPENDS ${MO_FILES})