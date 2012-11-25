# ResourceManager module
# Creates a ResourceManager static library
# Which is responsible for memory managing.

# ResourceManager module parameters

# Configure modules path
SET ( MODULE_PATH "${${PROJECT_NAME}_SOURCE_DIR}/../cmake_modules" )
INCLUDE ( "${MODULE_PATH}/modules.cmake" )

INCLUDE_DIRECTORIES ( "${${PROJECT_NAME}_SOURCE_DIR}" )
INCLUDE_DIRECTORIES ( "${${PROJECT_NAME}_SOURCE_DIR}/${ATOMENGINE_MODULE}" )
IF ( GOOGLE_TEST AND GOOGLE_TEST_PATH)
  INCLUDE_DIRECTORIES ( "${${PROJECT_NAME}_SOURCE_DIR}/${GOOGLE_TEST_PATH}/include" )
ENDIF()

# Create a static library which will be used by the engine
ADD_LIBRARY ( ${RESOURCEMANAGER_MODULE} STATIC ResourceManager )

# Installation instructions for the ResourceManager module
# Please specify the installation instructions specific
# To the architecture the project is being built for.

IF ( UNIX )
	INSTALL (
		TARGETS ${RESOURCEMANAGER_MODULE}
		RUNTIME DESTINATION bin/${ROOT_PROJECT_NAME}
		LIBRARY DESTINATION lib/${ROOT_PROJECT_NAME}
		ARCHIVE DESTINATION lib/static/${ROOT_PROJECT_NAME}
	)
ELSEIF ( WIN32 )
	INSTALL (
		TARGETS ${RESOURCEMANAGER_MODULE}
		RUNTIME DESTINATION bin/
		LIBRARY DESTINATION lib/
		ARCHIVE DESTINATION lib/static/
	)
ENDIF()

