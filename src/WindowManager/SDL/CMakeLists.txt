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
