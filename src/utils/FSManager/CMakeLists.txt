# FSManager utility
# Creates an FS Manager program
# Which is responsible for managing the GameFS.

# FSManager module parameters

# Configure modules path
SET ( MODULE_PATH "${${PROJECT_NAME}_SOURCE_DIR}/../cmake_modules" )
INCLUDE ( "${MODULE_PATH}/modules.cmake" )

INCLUDE_DIRECTORIES ( "${${PROJECT_NAME}_SOURCE_DIR}" )
INCLUDE_DIRECTORIES ( "${${PROJECT_NAME}_SOURCE_DIR}/${OATOMENGINE}" )

# Some nessessary definitions
ADD_DEFINITIONS ( "-D_FSMANAGER_" )

# Module dependencies: libAtomError and libGameFS_FSMan
# Ensure that those targets are built before current one
SET ( ${FSMAN_MODULE}_DEPENDS "AtomError GameFS_FSMan SysVars" )

# Make fsman utility
ADD_EXECUTABLE ( ${FSMAN_MODULE} fsman )

# Link against libAtomError and libGameFS_FSMan
# TODO: Library names should be taken from variables
TARGET_LINK_LIBRARIES ( ${FSMAN_MODULE} ${ATOMERROR_MODULE} ${GAMEFS_FSMAN_MODULE} ${SYSVARS_MODULE})

# Add apptopriate library search paths to linker
#SET_DIRECTORY_PROPERTIES ( PROPERTIES
#	LINK_DIRECTORIES "${AtomError_BINARY_DIR} ${GameFS_BINARY_DIR}"
#)

# Installation instructions for the FSMan module
# Please specify the installation instructions sprcific
# To the architecture the project is being built for.

IF ( UNIX )
	INSTALL (
		TARGETS ${FSMAN_MODULE}
		RUNTIME DESTINATION bin/${ROOT_PROJECT_NAME}
		LIBRARY DESTINATION lib/${ROOT_PROJECT_NAME}
		ARCHIVE DESTINATION lib/static/${ROOT_PROJECT_NAME}
	)
ELSEIF ( WIN32 )
	INSTALL (
		TARGETS ${FSMAN_MODULE}
		RUNTIME DESTINATION bin/
		LIBRARY DESTINATION lib/
		ARCHIVE DESTINATION lib/static/
	)
ENDIF()

