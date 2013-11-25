# AtomError module
# Creates an AtomError static library 
# Which is responsible for error handling and debug output.

# AtomError module parameters

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
INCLUDE_DIRECTORIES ( "${${PROJECT_NAME}_SOURCE_DIR}/${ATOMERROR_MODULE}" )
INCLUDE_DIRECTORIES ( "${${PROJECT_NAME}_SOURCE_DIR}/${ATOMERROR_MODULE}/${AE_PREFIX}" )
IF ( GOOGLE_TEST AND GOOGLE_TEST_PATH )
  INCLUDE_DIRECTORIES ( "${${PROJECT_NAME}_SOURCE_DIR}/${GOOGLE_TEST_PATH}/include" )
ENDIF()

# Command line options, their defaults and checks

# Set correct defines and include correct files
ADD_DEFINITIONS ( "-DATOMERROR" )
LIST ( APPEND ${ATOMERROR_MODULE}_SRC AtomError ${AE_PREFIX}/logpath )

# Create a static library
ADD_LIBRARY ( ${ATOMERROR_MODULE} STATIC ${${ATOMERROR_MODULE}_SRC})

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
