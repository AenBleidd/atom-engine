# Tests

# Configure modules path
SET ( MODULE_PATH "${${PROJECT_NAME}_SOURCE_DIR}/../cmake_modules" )
INCLUDE ( "${MODULE_PATH}/modules.cmake" )

INCLUDE_DIRECTORIES ( "${${PROJECT_NAME}_SOURCE_DIR}" )
INCLUDE_DIRECTORIES ( "${${PROJECT_NAME}_SOURCE_DIR}/${GOOGLE_TEST_PATH}" )
INCLUDE_DIRECTORIES ( "${${PROJECT_NAME}_SOURCE_DIR}/${GOOGLE_TEST_PATH}/include" )
INCLUDE_DIRECTORIES ( "${${PROJECT_NAME}_SOURCE_DIR}/${ATOMLOG_MODULE}" )
INCLUDE_DIRECTORIES ( "${${PROJECT_NAME}_SOURCE_DIR}/${ATOMFS_MODULE}" )

OPTION ( DISABLE_PTHREADS "Do not use pthreads" OFF )

# Module dependencies
# Ensure that those targets are built before current one
SET ( ${TESTS_MODULE}_DEPENDS "GoogleTestFramework" "AtomError")

IF ( NOT DISABLE_PTHREADS )
# Defines CMAKE_USE_PTHREADS_INIT and CMAKE_THREAD_LIBS_INIT.
  FIND_PACKAGE ( Threads )
endif()

IF ( CMAKE_USE_PTHREADS_INIT )  # The pthreads library is available and allowed.
  ADD_DEFINITIONS ( -DGTEST_HAS_PTHREAD=1 )
else()
  ADD_DEFINITIONS ( -DGTEST_HAS_PTHREAD=0 )
endif()

LIST ( APPEND ${TESTS_MODULE}_SRC ${${PROJECT_NAME}_SOURCE_DIR}/${GOOGLE_TEST_PATH}/src/gtest-all test_exec )

# AtomLog tests

LIST ( APPEND ${TESTS_MODULE}_SRC  AtomLog/AtomLog_GetLogPath_Test )

# SysVars tests

LIST ( APPEND ${TESTS_MODULE}_SRC  SysVars/SysVars_common_tests )

# AtomFS tests
LIST ( APPEND ${TESTS_MODULE}_SRC  AtomFS/AtomFS_Common_Tests )
LIST ( APPEND ${TESTS_MODULE}_SRC  AtomFS/AtomFS_Create_Test )

ADD_EXECUTABLE ( ${TESTS_MODULE}  ${${TESTS_MODULE}_SRC} )

# TODO: Library names should be taken from variables
TARGET_LINK_LIBRARIES ( ${TESTS_MODULE} ${ATOMENGINE_MODULE} ${ATOMLOG_MODULE} ${SYSVARS_MODULE} ${ATOMFS_MODULE} )

# Installation instructions for the FSMan module
# Please specify the installation instructions sprcific
# To the architecture the project is being built for.

IF ( UNIX )
	INSTALL (
		TARGETS ${TESTS_MODULE}
		RUNTIME DESTINATION bin/${ROOT_PROJECT_NAME}
		LIBRARY DESTINATION lib/${ROOT_PROJECT_NAME}
		ARCHIVE DESTINATION lib/static/${ROOT_PROJECT_NAME}
	)
ELSEIF ( WIN32 )
	INSTALL (
		TARGETS ${TESTS_MODULE}
		RUNTIME DESTINATION bin/
		LIBRARY DESTINATION lib/
		ARCHIVE DESTINATION lib/static/
	)
ENDIF()
