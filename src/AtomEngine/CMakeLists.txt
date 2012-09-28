# AtomEngine module
# Creates an AtomEngine static library 
# Which is the main module.

# AtomEngine module parameters

# Configure modules path
SET ( MODULE_PATH "${${PROJECT_NAME}_SOURCE_DIR}/../cmake_modules" )
INCLUDE ( "${MODULE_PATH}/modules.cmake" )

IF ( WIN32 )
	SET ( AE_PREFIX "Windows" )
ENDIF ( WIN32 )
IF ( UNIX )
	SET ( AE_PREFIX "Unix" )
ENDIF ( UNIX )

INCLUDE_DIRECTORIES ( "${${PROJECT_NAME}_SOURCE_DIR}" )
INCLUDE_DIRECTORIES ( "${${PROJECT_NAME}_SOURCE_DIR}/${ATOMENGINE_MODULE}" )
INCLUDE_DIRECTORIES ( "${${PROJECT_NAME}_SOURCE_DIR}/${ATOMERROR_MODULE}" )
INCLUDE_DIRECTORIES ( "${${PROJECT_NAME}_SOURCE_DIR}/${ATOMERROR_MODULE}/${AE_PREFIX}" )

# Command line options, their defaults and checks

# Create a static library
        ADD_LIBRARY ( ${ATOMENGINE_MODULE} STATIC OAtomEngine AtomEngine )

# Installation instructions for the AtomEengine module
# Please specify the installation instructions specific 
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
		TARGETS ${ATOMENGINE_MODULE}
		RUNTIME DESTINATION bin/
		LIBRARY DESTINATION lib/
		ARCHIVE DESTINATION lib/static/
	)
ENDIF()

ADD_SUBDIRECTORY ( tests )
