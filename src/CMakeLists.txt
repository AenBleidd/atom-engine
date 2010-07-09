# cmake configuration file for building ATOM package

CMAKE_MINIMUM_REQUIRED ( VERSION  2.6 )

# Project information and versioning
PROJECT ( ATOM )

# Configure modules path
SET ( MODULE_PATH "${${PROJECT_NAME}_SOURCE_DIR}/../cmake_modules" )

# Perform some general initialization
INCLUDE ( "${MODULE_PATH}/init.cmake" )

# Project version
SET ( ${PROJECT_NAME}_MAJOR_VERSION "0" )
SET ( ${PROJECT_NAME}_MINOR_VERSION "1" )
SET ( ${PROJECT_NAME}_PATCH_LEVEL "0" )
SET ( ${PROJECT_NAME}_REVISION "0" )
SET ( ${PROJECT_NAME}_VERSION "${PROJECT_NAME}_MAJOR_VERSION.${PROJECT_NAME}_MINOR_VERSION.SET ( ${PROJECT_NAME}_PATCH_LEVEL Rev. ${PROJECT_NAME}_REVISION" )

# Configure this project as the root project
SET ( ROOT_PROJECT_NAME "${PROJECT_NAME}" )
SET ( ROOT_PROJECT_DIR "${PROJECT_SOURCE_DIR}/.." )
SET ( ROOT_PROJECT_SOURCE_DIR "${PROJECT_SOURCE_DIR}" )
SET ( ROOT_PROJECT_BUILD_DIR "${PROJECT_BINARY_DIR}/.." )
SET ( ROOT_PROJECT_BINARY_DIR "${PROJECT_BINARY_DIR}/bin" )
SET ( ROOT_PROJECT_LIBRARY_DIR "${PROJECT_BINARY_DIR}/lib" )
SET ( OUTPUT_DIR "${ROOT_PROJECT_DIR}/Out" )

# Project configuration types
SET ( CMAKE_CONFIGURATION_TYPES Release; Debug )

INCLUDE ( "${MODULE_PATH}/ATOM_checks.cmake" )

# Recurse into subdirectory
ADD_SUBDIRECTORY ( WindowManager )
ADD_SUBDIRECTORY ( AtomError )
ADD_SUBDIRECTORY ( utils )
ADD_SUBDIRECTORY ( GameFS )


# Cache some of the command-line parameters
SET ( CMAKE_BUILD_TYPE "${CMAKE_BUILD_TYPE}" CACHE STRING "The build type for the project" FORCE )

# Add uninstall target
ADD_CUSTOM_TARGET ( uninstall "${CMAKE_COMMAND}" -P "${PROJECT_BINARY_DIR}/cmake_uninstall.cmake" )
