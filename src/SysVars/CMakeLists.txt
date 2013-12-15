# SysVars module
# Creates a SysVars static library
# Needed to identify some system variables

# GameFS module parameters

# Configure modules path
SET ( MODULE_PATH "${${PROJECT_NAME}_SOURCE_DIR}/../cmake_modules" )
INCLUDE ( "${MODULE_PATH}/modules.cmake" )

IF ( WIN32 )
	SET ( SV_PREFIX "Windows" )
ENDIF ( WIN32 )
IF ( UNIX )
	SET ( SV_PREFIX "Unix" )
ENDIF ( UNIX )

INCLUDE_DIRECTORIES ( "${${PROJECT_NAME}_SOURCE_DIR}" )

# Create a static library which will be used by the engine
ADD_LIBRARY ( ${SYSVARS_MODULE} STATIC byteorderdetect ${SV_PREFIX}/guid )
IF ( UNIX )
TARGET_LINK_LIBRARIES ( ${SYSVARS_MODULE} uuid )
ELSEIF ( WIN32 )
TARGET_LINK_LIBRARIES ( ${SYSVARS_MODULE} rpcrt4 )
ENDIF()

# Installation instructions for the SysVars module
# Please specify the installation instructions sprcific
# To the architecture the project is being built for.

IF ( UNIX )
	INSTALL (
		TARGETS ${SYSVARS_MODULE}
		RUNTIME DESTINATION bin/${ROOT_PROJECT_NAME}
		LIBRARY DESTINATION lib/${ROOT_PROJECT_NAME}
		ARCHIVE DESTINATION lib/static/${ROOT_PROJECT_NAME}
	)
ELSEIF ( WIN32 )
	INSTALL (
		TARGETS ${SYSVARS_MODULE}
		RUNTIME DESTINATION bin/
		LIBRARY DESTINATION lib/
		ARCHIVE DESTINATION lib/static/
	)
ENDIF()

