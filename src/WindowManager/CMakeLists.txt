# WindowManager module
# Creates a WindowManager static library 
# Which is responsible for GUI windows creation and handling.

# WindowManager module parameters

# Configure modules path
SET ( MODULE_PATH "${${PROJECT_NAME}_SOURCE_DIR}/../cmake_modules" )
INCLUDE ( "${MODULE_PATH}/modules.cmake" )

# Command line options, their defaults and checks

# Set the GUI parameter possible values
SET ( GUI_TYPES SDL; Native )

# set Window Manager to Native by default
IF ( NOT GUI )
	SET ( GUI Native )
ENDIF()

# Add "OWindow.h" and "AtomError.h" directories to the include path
INCLUDE_DIRECTORIES ( "${${PROJECT_NAME}_SOURCE_DIR}/${WM_MODULE}" )
INCLUDE_DIRECTORIES ( "${${PROJECT_NAME}_SOURCE_DIR}/${ATOMERROR_MODULE}" )

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
		IF ( GUI STREQUAL "Native" )
			IF ( WIN32 )
				SET ( WM_PREFIX "${GUI}/Windows" )
			ENDIF ( WIN32 )
			IF ( UNIX )
				SET ( WM_PREFIX "${GUI}/Unix" )
			ENDIF ( UNIX )

			# ACHTUNG!!! General build instructions! Do not modify them, please!
			ADD_LIBRARY ( ${WM_MODULE} STATIC OWindow ${WM_PREFIX}/WindowManager )
			TARGET_LINK_LIBRARIES ( ${WM_MODULE} ${ATOMERROR_MODULE} )
		ENDIF (GUI STREQUAL "Native")
		IF ( GUI STREQUAL "SDL" )
			# Perform search on libSDL library
			FIND_PACKAGE ( SDL REQUIRED )
			IF ( SDL_FOUND )
				SET ( SDL_BUILDING_LIBRARY TRUE )
				INCLUDE_DIRECTORIES ( ${SDL_INCLUDE_DIR} "${${PROJECT_NAME}_SOURCE_DIR}/${ATOMERROR_MODULE}" )
				ADD_LIBRARY ( ${WM_MODULE} STATIC OWindow SDL/WindowManager )
				TARGET_LINK_LIBRARIES ( ${WM_MODULE} ${ATOMERROR_MODULE} ${SDL_LIBRARY} )
			ELSE()
				MESSAGE ( FATAL_ERROR "No libSDL library found." )
			ENDIF ( SDL_FOUND )
		ENDIF( GUI STREQUAL "SDL" )
		# TODO: ADD MORE LIBRARIES HERE
	ELSE()
		MESSAGE ( FATAL_ERROR "No suitable GUI parameter value supplied. Possible values are: ${GUI_VARIANTS}. Please choose one among them." )
	ENDIF()
ELSE()
	MESSAGE ( FATAL_ERROR "No GUI parameter value supplied. Possible values are: ${GUI_VARIANTS}. Please choose one among them." )
ENDIF()

# Send GUI to cache
SET ( GUI "${GUI}" CACHE STRING "The library responsible for GUI windows events. Possible values are: ${GUI_VARIANTS}" FORCE )

# Installation instructions for the WindowManager module
# Please specify the installation instructions sprcific 
# To the architecture the project is being built for.

IF ( UNIX )
	INSTALL (
		TARGETS ${WM_MODULE}
		RUNTIME DESTINATION bin/${ROOT_PROJECT_NAME}
		LIBRARY DESTINATION lib/${ROOT_PROJECT_NAME}
		ARCHIVE DESTINATION lib/static/${ROOT_PROJECT_NAME}
	)
ELSEIF ( WIN32 )
	INSTALL (
		TARGETS ${WM_MODULE}
		RUNTIME DESTINATION bin/
		LIBRARY DESTINATION lib/
		ARCHIVE DESTINATION lib/static/
	)
ENDIF()
