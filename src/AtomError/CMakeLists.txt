# AtomError project
# Creates an AtomError static library 
# Which is responsible for error handling and debug output.

# CMake compatibility issues: don't modify this, please!
CMAKE_MINIMUM_REQUIRED ( VERSION  2.6 )

# Perform some general initialization
INCLUDE ( "${MODULE_PATH}/init.cmake" )

# Project information
PROJECT ( AtomError )

# Project version
SET ( ${PROJECT_NAME}_MAJOR_VERSION "0" )
SET ( ${PROJECT_NAME}_MINOR_VERSION "1" )
SET ( ${PROJECT_NAME}_PATCH_LEVEL "0" )
SET ( ${PROJECT_NAME}_REVISION "0" )
SET ( ${PROJECT_NAME}_VERSION "${PROJECT_NAME}_MAJOR_VERSION.${PROJECT_NAME}_MINOR_VERSION.SET ( ${PROJECT_NAME}_PATCH_LEVEL Rev. ${PROJECT_NAME}_REVISION" )

ADD_DEFINITIONS ( "-D_FSMANAGER_" )

ADD_LIBRARY ( ${PROJECT_NAME} STATIC AtomError )


# Installation instructions for the error handler
INSTALL (
	TARGETS ${PROJECT_NAME}
	RUNTIME DESTINATION bin/${ROOT_PROJECT_NAME}
	LIBRARY DESTINATION lib/${ROOT_PROJECT_NAME}
	ARCHIVE DESTINATION lib/static/${ROOT_PROJECT_NAME}
)

# Add uninstall target
ADD_CUSTOM_TARGET ( uninstall_AtomError "${CMAKE_COMMAND}" -P "${PROJECT_BINARY_DIR}/cmake_uninstall.cmake" )
