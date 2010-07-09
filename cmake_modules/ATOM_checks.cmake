# CMake project build configuraion check script

# set default cmake build type to Release
IF ( NOT CMAKE_BUILD_TYPE )
    SET ( CMAKE_BUILD_TYPE Release )
ENDIF()

# Check for appropriate build parameters
SET ( BUILD_OK:BOOL OFF )
IF ( DEFINED CMAKE_BUILD_TYPE )
	FOREACH ( CFG ${CMAKE_CONFIGURATION_TYPES} )
		IF ( CMAKE_BUILD_TYPE STREQUAL CFG )
			SET ( BUILD_OK ON )
			BREAK()
		ENDIF()
	ENDFOREACH()
	IF ( NOT BUILD_OK )
		MESSAGE ( FATAL_ERROR "Wrong build type parameter. Possible values for build type are: ${CMAKE_CONFIGURATION_TYPES}. Please choose one of those" )
	ENDIF()
ELSE()
	MESSAGE ( FATAL_ERROR "Build type is not defined. Possible values are: ${CMAKE_CONFIGURATION_TYPES}. Please specify one" )
ENDIF()

IF ( UNIX )
	ADD_DEFINITIONS ( "-DUNIX" )
ENDIF()

IF ( CMAKE_BUILD_TYPE STREQUAL "Debug" )
	ADD_DEFINITIONS ( "-DATOM_DEBUG" )
ENDIF()


