# AtomLog module
# Creates an AtomLog static library 
# Which is responsible for error handling and debug output.

# AtomLog module parameters

# Configure modules path
SET ( MODULE_PATH "${${PROJECT_NAME}_SOURCE_DIR}/../cmake_modules" )
INCLUDE ( "${MODULE_PATH}/modules.cmake" )

IF ( WIN32 )
	SET ( AL_PREFIX "Windows" )
ENDIF ( WIN32 )
IF ( UNIX )
	SET ( AL_PREFIX "Unix" )
ENDIF ( UNIX )

INCLUDE_DIRECTORIES ( "${${PROJECT_NAME}_SOURCE_DIR}" )
INCLUDE_DIRECTORIES ( "${${PROJECT_NAME}_SOURCE_DIR}/${ATOMLOG_MODULE}" )
INCLUDE_DIRECTORIES ( "${${PROJECT_NAME}_SOURCE_DIR}/${ATOMLOG_MODULE}/${AL_PREFIX}" )
IF ( GOOGLE_TEST AND GOOGLE_TEST_PATH )
  INCLUDE_DIRECTORIES ( "${${PROJECT_NAME}_SOURCE_DIR}/${GOOGLE_TEST_PATH}/include" )
ENDIF()

# Command line options, their defaults and checks

# Set correct defines and include correct files
ADD_DEFINITIONS ( "-DATOMLOG" )
LIST ( APPEND ${ATOMLOG_MODULE}_SRC AtomLog ${AL_PREFIX}/logpath )

# Create a static library
ADD_LIBRARY ( ${ATOMLOG_MODULE} STATIC ${${ATOMLOG_MODULE}_SRC})

# Installation instructions for the AtomLog module
# Please specify the installation instructions sprcific 
# To the architecture the project is being built for.

IF ( UNIX )
	INSTALL (
		TARGETS ${ATOMLOG_MODULE}
		RUNTIME DESTINATION bin/${ROOT_PROJECT_NAME}
		LIBRARY DESTINATION lib/${ROOT_PROJECT_NAME}
		ARCHIVE DESTINATION lib/static/${ROOT_PROJECT_NAME}
	)
ELSEIF ( WIN32 )
	INSTALL (
		TARGETS ${ATOMLOG_MODULE}
		RUNTIME DESTINATION bin/
		LIBRARY DESTINATION lib/
		ARCHIVE DESTINATION lib/static/
	)
ENDIF()
