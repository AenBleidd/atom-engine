# GameFS Manager project
# Creates an GameFS Manager program 
# Which is responsible for managing the GameFS.

# CMake compatibility issues: don't modify this, please!
CMAKE_MINIMUM_REQUIRED ( VERSION  2.6 )

# Perform some general initialization
INCLUDE ( "${MODULE_PATH}/init.cmake" )

# Project information
PROJECT ( fsman )

# Project version
SET ( ${PROJECT_NAME}_MAJOR_VERSION "0" )
SET ( ${PROJECT_NAME}_MINOR_VERSION "1" )
SET ( ${PROJECT_NAME}_PATCH_LEVEL "0" )
SET ( ${PROJECT_NAME}_REVISION "0" )
SET ( ${PROJECT_NAME}_VERSION "${PROJECT_NAME}_MAJOR_VERSION.${PROJECT_NAME}_MINOR_VERSION.SET ( ${PROJECT_NAME}_PATCH_LEVEL Rev. ${PROJECT_NAME}_REVISION" )

#ADD_DEFINITIONS ( "-D_FSMAN_VERSION_" )
ADD_DEFINITIONS ( "-D_FSMANAGER_" )

# Ensure that 'AtomError' and 'GameFS' targets are built before current one
SET ( ${PROJECT_NAME}_DEPENDS "AtomError GameFS" )

# Make a binary 
ADD_EXECUTABLE ( ${PROJECT_NAME} fsman crc32 )

# Link against libAtomError and libGameFS
TARGET_LINK_LIBRARIES ( ${PROJECT_NAME} AtomError GameFS )

# Add apptopriate library seatch paths to linker
SET_DIRECTORY_PROPERTIES ( PROPERTIES 
	LINK_DIRECTORIES "${AtomError_BINARY_DIR} ${GameFS_BINARY_DIR}"
	#LINK_DIRECTORIES "${GameFS_BINARY_DIR}" 
	#INCLUDE_DIRECTORIES "${AtomError_BINARY_DIR} ${GameFS_BINARY_DIR}" 
)
 
# Installation instructions for the GameFS manager
INSTALL (
	TARGETS ${PROJECT_NAME}
	RUNTIME DESTINATION bin/${ROOT_PROJECT_NAME}
	LIBRARY DESTINATION lib/${ROOT_PROJECT_NAME}
	ARCHIVE DESTINATION lib/static/${ROOT_PROJECT_NAME}
)

# Add uninstall target
ADD_CUSTOM_TARGET ( uninstall_FSManager "${CMAKE_COMMAND}" -P "${PROJECT_BINARY_DIR}/cmake_uninstall.cmake" )
