# AtomError module
# Creates an AtomError static library 
# Which is responsible for error handling and debug output.

# AtomError module parameters

# Configure modules path
SET ( MODULE_PATH "${${PROJECT_NAME}_SOURCE_DIR}/../cmake_modules" )
INCLUDE ( "${MODULE_PATH}/modules.cmake" )

# Command line options, their defaults and checks

# set System Journal parameter by default
IF ( NOT SYSTEM_JOURNAL )
	SET ( SYSTEM_JOURNAL "ON" )
ENDIF()

# Set correct defines and include correct files
IF ( SYSTEM_JOURNAL STREQUAL "ON" )
	IF ( WIN32 )
		SET ( AE_PREFIX "Windows" )
	ENDIF ( WIN32 )
	IF ( UNIX )
		SET ( AE_PREFIX "Unix" )
	ENDIF ( UNIX )
	ADD_DEFINITIONS ( "-DSYSJOURNAL" )
# Create a static library
        ADD_LIBRARY ( ${ATOMERROR_MODULE} STATIC OAtomError AtomError ${AE_PREFIX}/logpath ${AE_PREFIX}/SystemJournal )
ELSE ()
        ADD_LIBRARY ( ${ATOMERROR_MODULE} STATIC OAtomError AtomError ${AE_PREFIX}/logpath)
ENDIF ( SYSTEM_JOURNAL STREQUAL "ON" )

# Send SYSTEM_JOURNAL to cache
SET ( SYSTEM_JOURNAL "${SYSTEM_JOURNAL}" CACHE STRING "The library responsible for system loggong events. Possible values are: ON; OFF" FORCE )

# Installation instructions for the AtomError module
# Please specify the installation instructions sprcific 
# To the architecture the project is being built for.

IF ( UNIX )
	INSTALL (
		TARGETS ${ATOMERROR_MODULE}
		RUNTIME DESTINATION bin/${ROOT_PROJECT_NAME}
		LIBRARY DESTINATION lib/${ROOT_PROJECT_NAME}
		ARCHIVE DESTINATION lib/static/${ROOT_PROJECT_NAME}
	)
ELSEIF ( WIN32 )
	INSTALL (
		TARGETS ${ATOMERROR_MODULE}
		RUNTIME DESTINATION bin/
		LIBRARY DESTINATION lib/
		ARCHIVE DESTINATION lib/static/
	)
ENDIF()
