# GameFS module
# Creates a GameFS static library
# Which is responsible for file, path and archive handling.

# GameFS module parameters

# Configure modules path
SET ( MODULE_PATH "${${PROJECT_NAME}_SOURCE_DIR}/../cmake_modules" )
INCLUDE ( "${MODULE_PATH}/modules.cmake" )
INCLUDE ( "${MODULE_PATH}/functions.cmake" )

IF ( WIN32 )
	SET ( GFS_PREFIX "Windows" )
ENDIF ( WIN32 )
IF ( UNIX )
	SET ( GFS_PREFIX "Unix" )
ENDIF ( UNIX )

INCLUDE_DIRECTORIES ( "${${PROJECT_NAME}_SOURCE_DIR}" )
IF ( GOOGLE_TEST AND GOOGLE_TEST_PATH)
  INCLUDE_DIRECTORIES ( "${${PROJECT_NAME}_SOURCE_DIR}/${GOOGLE_TEST_PATH}/include" )
ENDIF()

CreateMessagesFile("messages.msg" "GameFS" "CORE_FS")

# Create a static library which will be used by the engine
# As well as static library to be linked to fsman utility
ADD_LIBRARY ( ${GAMEFS_MODULE} STATIC gamefs crypt mount strings file ${GFS_PREFIX}/SysDepend_file )
ADD_LIBRARY ( ${GAMEFS_FSMAN_MODULE} STATIC gamefs crypt mount create ${GFS_PREFIX}/SysDepend_create navigate strings ${GFS_PREFIX}/SysDepend_strings file crc32 ${GFS_PREFIX}/SysDepend_file )
SET_TARGET_PROPERTIES ( ${GAMEFS_FSMAN_MODULE} PROPERTIES
	COMPILE_FLAGS "-D_FSMANAGER_"
)

# Installation instructions for the GameFS module
# Please specify the installation instructions specific
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

