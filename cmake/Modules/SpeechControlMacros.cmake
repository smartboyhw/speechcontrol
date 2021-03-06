include(SpeechControlDefaults)

MACRO(PKGCONFIG_GETVAR _package _var _output_variable)
  SET(${_output_variable})

  # if pkg-config has been found
  IF(PKG_CONFIG_FOUND)

    EXEC_PROGRAM(${PKG_CONFIG_EXECUTABLE} ARGS ${_package} --exists RETURN_VALUE _return_VALUE OUTPUT_VARIABLE _pkgconfigDevNull )

    # and if the package of interest also exists for pkg-config, then get the information
    IF(NOT _return_VALUE)

      EXEC_PROGRAM(${PKG_CONFIG_EXECUTABLE} ARGS ${_package} --variable ${_var} OUTPUT_VARIABLE ${_output_variable} )

    ELSE(NOT _return_VALUE)
      MESSAGE(WARNING "${_package} not found.")
    ENDIF(NOT _return_VALUE)

  ELSE(PKG_CONFIG_FOUND)
    MESSAGE(ERROR "PkgConfig not found.")
  ENDIF(PKG_CONFIG_FOUND)

ENDMACRO(PKGCONFIG_GETVAR _package _var _output_variable)

macro(dbus_add_activation_service _sources)
    PKGCONFIG_GETVAR(dbus-1 session_bus_services_dir _install_dir)
    foreach (_i ${_sources})
        get_filename_component(_service_file ${_i} ABSOLUTE)
        string(REGEX REPLACE "\\.service.*$" ".service" _output_file ${_i})
        set(_target ${CMAKE_CURRENT_BINARY_DIR}/${_output_file})
        configure_file(${_service_file} ${_target})
        install(FILES ${_target} DESTINATION ${DBUS_SERVICES_INSTALL_DIR} )
    endforeach (_i ${ARGN})
endmacro(dbus_add_activation_service _sources)

macro(speechcontrol_install_plugin _plugin_target _plugin_id)
    install(TARGETS ${_plugin_target}
            DESTINATION ${SPCHCNTRL_PLUGINS_LIB_INSTALL_DIR})

    install(FILES "${PROJECT_SOURCE_DIR}/data/${_plugin_id}.spec"
            DESTINATION ${SPCHCNTRL_PLUGINS_SPEC_INSTALL_DIR})
    if (KDE4_FOUND)
        install(TARGETS ${_plugin_target}
                DESTINATION "${PLUGIN_INSTALL_DIR}/plugins/speechcontrol")

        list(APPEND SPCHCNTRL_PLUGINS_LIB_INSTALL_DIR "${PLUGIN_INSTALL_DIR}/plugins/speechcontrol")
        list(REMOVE_DUPLICATES SPCHCNTRL_PLUGINS_LIB_INSTALL_DIR)
    endif (KDE4_FOUND)
endmacro(speechcontrol_install_plugin _plugin_target)