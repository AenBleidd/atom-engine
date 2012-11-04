# cmake configuration file for building ATOM package

# TODO: Add default settings for Embarcadero
# CMAKE_C_FLAGS             -tM
# CMAKE_CXX_FLAGS           -tM
# CMAKE_SHARED_LINKER_FLAGS -tCDR -lS:10000000 -lSc:10000000 

CMAKE_MINIMUM_REQUIRED ( VERSION  2.6 )

# Project information and versioning
PROJECT ( AtomEngine )

# CMake compatibility issues: don't modify this, please!
MARK_AS_ADVANCED ( CMAKE_BACKWARDS_COMPATIBILITY )
SET ( CMAKE_ALLOW_LOOSE_LOOP_CONSTRUCTS TRUE )
SET ( CMAKE_VERBOSE_MAKEFILE ON )

# Enable c++11 in gcc
IF ( CMAKE_COMPILER_IS_GNUCXX )
  EXECUTE_PROCESS ( COMMAND ${CMAKE_CXX_COMPILER} -dumpversion OUTPUT_VARIABLE GCC_VERSION )
  IF ( GCC_VERSION VERSION_GREATER 4.7 OR GCC_VERSION VERSION_EQUAL 4.7 )
    SET ( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11" )
  ELSEIF (GCC_VERSION VERSION_GREATER 4.6 OR GCC_VERSION VERSION_EQUAL 4.6 )
    SET ( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x" )
  ENDIF()
ENDIF()

# Project version
SET ( ${PROJECT_NAME}_MAJOR_VERSION "0" )
SET ( ${PROJECT_NAME}_MINOR_VERSION "1" )
SET ( ${PROJECT_NAME}_PATCH_LEVEL "0" )
SET ( ${PROJECT_NAME}_REVISION "0" )
SET ( ${PROJECT_NAME}_VERSION "${${PROJECT_NAME}_MAJOR_VERSION}.${${PROJECT_NAME}_MINOR_VERSION}.${${PROJECT_NAME}_PATCH_LEVEL}-${${PROJECT_NAME}_REVISION}" )
# Add definition of project version
ADD_DEFINITIONS ( -D_ATOM_VERSION_="${${PROJECT_NAME}_VERSION}" )

INCLUDE_DIRECTORIES ( "${${PROJECT_NAME}_SOURCE_DIR}" )
INCLUDE_DIRECTORIES ( "${${PROJECT_NAME}_SOURCE_DIR}/${OATOMENGINE}" )

# Command line options, their defaults and checks

# Project configuration types
SET ( CMAKE_CONFIGURATION_TYPES Release; Debug )

# set default cmake build type to Debug
IF ( NOT CMAKE_BUILD_TYPE )
    SET ( CMAKE_BUILD_TYPE Debug )
ENDIF()

# Check for appropriate build parameters
# If ok BUILD_OK is set to ON
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

# Nessessary definitions

IF ( UNIX )
  ADD_DEFINITIONS ( "-DUNIX" )
ELSEIF( WIN32 )
  ADD_DEFINITIONS ( "-DWINDOWS" )
ENDIF()

IF ( CMAKE_BUILD_TYPE STREQUAL "Debug" )
  ADD_DEFINITIONS ( "-DATOM_DEBUG" )
ENDIF()

# Recurse into subdirectory
ADD_SUBDIRECTORY ( WindowManager )
ADD_SUBDIRECTORY ( AtomError )
ADD_SUBDIRECTORY ( GameFS )
ADD_SUBDIRECTORY ( SysVars )
ADD_SUBDIRECTORY ( utils )
ADD_SUBDIRECTORY ( ResourceManager )
ADD_SUBDIRECTORY ( AtomEngine )
# Compile Google Test Framework
IF ( NOT GOOGLE_TEST )
  SET ( GOOGLE_TEST:BOOL OFF )
ENDIF()

IF ( GOOGLE_TEST AND GOOGLE_TEST_PATH)
#  ADD_SUBDIRECTORY ( ${GOOGLE_TEST_PATH} googletest )
  ADD_SUBDIRECTORY ( tests )
ENDIF()

# Cache some of the command-line parameters
SET ( CMAKE_BUILD_TYPE "${CMAKE_BUILD_TYPE}" CACHE STRING "The build type for the project" FORCE )
OPTION ( GOOGLE_TEST "Build tests with Google Test Framework" OFF )
SET ( GOOGLE_TEST_PATH "../googletest" CACHE STRING "Path to Google Test Framework" )
