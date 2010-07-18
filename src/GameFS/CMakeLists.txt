# GameFS project
# Creates a GameFS static library 
# Which is responsible for file and path handling.

# CMake compatibility issues: don't modify this, please!
CMAKE_MINIMUM_REQUIRED ( VERSION  2.6 )

# Perform some general initialization
INCLUDE ( "${MODULE_PATH}/init.cmake" )

# Project information
PROJECT ( GameFS )

# Project version
SET ( ${PROJECT_NAME}_MAJOR_VERSION "0" )
SET ( ${PROJECT_NAME}_MINOR_VERSION "1" )
SET ( ${PROJECT_NAME}_PATCH_LEVEL "0" )
SET ( ${PROJECT_NAME}_REVISION "0" )
SET ( ${PROJECT_NAME}_VERSION "${PROJECT_NAME}_MAJOR_VERSION.${PROJECT_NAME}_MINOR_VERSION.SET ( ${PROJECT_NAME}_PATCH_LEVEL Rev. ${PROJECT_NAME}_REVISION" )

# Make a static library which will be used by the engine
ADD_LIBRARY ( ${PROJECT_NAME} STATIC gamefs )

# Make a static library which will be used by fsman utility
ADD_LIBRARY ( ${PROJECT_NAME}_FSMan STATIC gamefs )
SET_TARGET_PROPERTIES ( ${PROJECT_NAME}_FSMan
	PROPERTIES COMPILE_FLAGS "-D_FSMANAGER_"
)

# Installation instructions for the window manager
INSTALL (
	TARGETS ${PROJECT_NAME} ${PROJECT_NAME}_FSMan
	RUNTIME DESTINATION bin/${ROOT_PROJECT_NAME}
	LIBRARY DESTINATION lib/${ROOT_PROJECT_NAME}
	ARCHIVE DESTINATION lib/static/${ROOT_PROJECT_NAME}
)

# Add uninstall target
ADD_CUSTOM_TARGET ( uninstall_GameFS "${CMAKE_COMMAND}" -P "${PROJECT_BINARY_DIR}/cmake_uninstall.cmake" )
