# GameFS module
# Creates a GameFS static library
# Which is responsible for file, path and archive handling.

# GameFS module parameters

# Configure modules path
SET ( MODULE_PATH "${${PROJECT_NAME}_SOURCE_DIR}/../cmake_modules" )
INCLUDE ( "${MODULE_PATH}/modules.cmake" )

# Create a static library which will be used by the engine
# As well as static library to be linked to fsman utility
ADD_LIBRARY ( ${GAMEFS_MODULE} STATIC gamefs crypt mount strings byteorderdetect file)
ADD_LIBRARY ( ${GAMEFS_FSMAN_MODULE} STATIC gamefs crypt mount create navigate strings byteorderdetect file crc32 )
SET_TARGET_PROPERTIES ( ${GAMEFS_FSMAN_MODULE} PROPERTIES
	COMPILE_FLAGS "-D_FSMANAGER_"
)

# Installation instructions for the GameFS module
# Please specify the installation instructions sprcific
# To the architecture the project is being built for.

# TODO: ${GAMEFS_FSMAN_MODULE} should not be installed,
# it is used only to build fsman utility

IF ( UNIX )
	INSTALL (
		TARGETS ${GAMEFS_MODULE}
		RUNTIME DESTINATION bin/${ROOT_PROJECT_NAME}
		LIBRARY DESTINATION lib/${ROOT_PROJECT_NAME}
		ARCHIVE DESTINATION lib/static/${ROOT_PROJECT_NAME}
	)
ELSEIF ( WIN32 )
	INSTALL (
		TARGETS ${GAMEFS_MODULE}
		RUNTIME DESTINATION bin/
		LIBRARY DESTINATION lib/
		ARCHIVE DESTINATION lib/static/
	)
ENDIF()

