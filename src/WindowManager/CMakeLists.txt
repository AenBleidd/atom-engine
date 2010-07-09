# Window manager project
# Creates a WindowManager static library 
# Which is responsible for GUI windows creation and handling.

# CMake compatibility issues: don't modify this, please!
CMAKE_MINIMUM_REQUIRED ( VERSION  2.6 )

# Perform some general initialization
INCLUDE ( "${MODULE_PATH}/init.cmake" )

# Project information
PROJECT ( AtomWM )

# Project version
SET ( ${PROJECT_NAME}_MAJOR_VERSION "0" )
SET ( ${PROJECT_NAME}_MINOR_VERSION "1" )
SET ( ${PROJECT_NAME}_PATCH_LEVEL "0" )
SET ( ${PROJECT_NAME}_REVISION "0" )
SET ( ${PROJECT_NAME}_VERSION "${PROJECT_NAME}_MAJOR_VERSION.${PROJECT_NAME}_MINOR_VERSION.SET ( ${PROJECT_NAME}_PATCH_LEVEL Rev. ${PROJECT_NAME}_REVISION" )

INCLUDE ( "${MODULE_PATH}/WindowManager_checks.cmake" )

# Add uninstall target
ADD_CUSTOM_TARGET ( uninstall_WindowManager "${CMAKE_COMMAND}" -P "${PROJECT_BINARY_DIR}/cmake_uninstall.cmake" )
