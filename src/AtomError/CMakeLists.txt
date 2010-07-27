# AtomError module
# Creates an AtomError static library 
# Which is responsible for error handling and debug output.

# AtomError module parameters

# Configure modules path
SET ( MODULE_PATH "${${PROJECT_NAME}_SOURCE_DIR}/../cmake_modules" )
INCLUDE ( "${MODULE_PATH}/modules.cmake" )

# Some nessessary definitions
# Is it really nessessary ?
# ADD_DEFINITIONS ( "-D_FSMANAGER_" )

# Create a static library
ADD_LIBRARY ( ${ATOMERROR_MODULE} STATIC AtomError )

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
