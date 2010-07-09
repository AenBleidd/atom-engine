# cmake configuration file for building ATOM package

CMAKE_MINIMUM_REQUIRED ( VERSION  2.6 )

# Configure modules path
SET ( MODULE_PATH "${${PROJECT_NAME}_SOURCE_DIR}/../cmake_modules" )

# Perform some general initialization
INCLUDE ( "${MODULE_PATH}/init.cmake" )

ADD_SUBDIRECTORY ( GameFSManager )

# Add uninstall target
ADD_CUSTOM_TARGET ( uninstall_Utils "${CMAKE_COMMAND}" -P "${PROJECT_BINARY_DIR}/cmake_uninstall.cmake" )
