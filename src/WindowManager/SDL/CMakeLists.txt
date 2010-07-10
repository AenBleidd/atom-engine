# A CMake build configuration script
# to build a WindowManager module
# using libSDL

# Perform search on libSDL library
FIND_PACKAGE ( SDL REQUIRED )
IF ( SDL_FOUND )
	INCLUDE_DIRECTORIES ( ${SDL_INCLUDE_DIR} )
	ADD_LIBRARY ( ${PROJECT_NAME} STATIC WindowManager )
	TARGET_LINK_LIBRARIES ( ${PROJECT_NAME} ${SDL_LIBRARY} )
ENDIF ()

# Installation instructions for the window manager
INSTALL (
	TARGETS ${PROJECT_NAME}
	RUNTIME DESTINATION bin/${ROOT_PROJECT_NAME}
	LIBRARY DESTINATION lib/${ROOT_PROJECT_NAME}
	ARCHIVE DESTINATION lib/static/${ROOT_PROJECT_NAME}
)

# Add uninstall target
ADD_CUSTOM_TARGET ( uninstall_WindowManager "${CMAKE_COMMAND}" -P "${PROJECT_BINARY_DIR}/cmake_uninstall.cmake" )
