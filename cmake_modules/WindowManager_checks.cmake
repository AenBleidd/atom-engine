# WindowManager ptoject parameters

# Set the GUI parameter possible values
SET ( GUI_TYPES SDL; Native )

# set Window Manager to SDL by default
IF ( NOT GUI )
	SET ( GUI SDL )
ENDIF()

# Check for appropriate GUI parameter
SET ( GUI_OK:BOOL OFF )
IF ( DEFINED GUI AND NOT GUI STREQUAL "" )
	FOREACH ( UI ${GUI_TYPES} )
		IF ( GUI STREQUAL UI )
			SET ( GUI_OK ON )
			BREAK()
		ENDIF()
	ENDFOREACH()
	IF( GUI_OK )
		IF ( GUI STREQUAL "SDL" )
			ADD_SUBDIRECTORY ( SDL )
		ENDIF()
		# TODO: ADD MORE LIBRARIES HERE
	ELSE()
		MESSAGE ( FATAL_ERROR "No suitable GUI parameter value supplied. Possible values are: ${GUI_VARIANTS}. Please choose one among them." )
	ENDIF()
ELSE()
MESSAGE ( FATAL_ERROR "No GUI parameter value supplied. Possible values are: ${GUI_VARIANTS}. Please choose one among them." )
ENDIF()

# Send GUI to cache
SET ( GUI "${GUI}" CACHE STRING "The library responsible for GUI windows events. Possible values are: ${GUI_VARIANTS}" FORCE )
