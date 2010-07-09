# Locate SDL library
# This module defines
# SDL_LIBRARY, the name of the library to link against
# SDL_FOUND, if false, do not try to link to SDL
# SDL_INCLUDE_DIR, where to find SDL.h
#
# This module responds to the the flag:
# SDL_BUILDING_LIBRARY
# If this is defined, then no SDL_main will be linked in because 
# only applications need main().
# Otherwise, it is assumed you are building an application and this
# module will attempt to locate and set the the proper link flags
# as part of the returned SDL_LIBRARY variable.
#
#
# Additional Note: If you see an empty SDL_LIBRARY_TEMP in your configuration
# and no SDL_LIBRARY, it means CMake did not find your SDL library 
# (SDL.dll, libsdl.so, SDL.framework, etc). 
# Set SDL_LIBRARY_TEMP to point to your SDL library, and configure again. 
# Similarly, if you see an empty SDLMAIN_LIBRARY, you should set this value
# as appropriate. These values are used to generate the final SDL_LIBRARY
# variable, but when these values are unset, SDL_LIBRARY does not get created.
#
#
# Uncomment this string to look only for static libraries
#SET ( CMAKE_FIND_LIBRARY_SUFFIXES .a; .la )

#FIND_LIBRARY (
#	LIBSDL                                # Name of the variable
#	NAMES SDLmain SDL                     # Possible library names of the variable
#	PATHS ENV PATH                        # Paths to look into
#	DOC "The Simple DirectMedia library"  # comment
#	PATH_SUFFIXES SDL SDL/lib sdl/lib lib # Subdirectories to look in path
#)


# Modified by Alessandro aka -=SATAN=- 
# Added code to assist with automated building by using environmental variables
# and providing a more controlled/consistent search behavior in different platforms.
# Added a search for SDLmain which is needed by some platforms.
# Added a search for threads which is needed by some platforms.
# Added needed compile switches for MinGW.
#

FIND_PATH (
	SDL_INCLUDE_DIR SDL.h
	HINTS
	$ENV{SDLDIR}
	PATH_SUFFIXES include/SDL include
	PATHS
		ENV PATH
		~/Library/Frameworks
		/Library/Frameworks
		/usr/local/include/SDL12
		/usr/local/include/SDL11 # FreeBSD ports
		/usr/include/SDL12
		/usr/include/SDL11
		/sw # Fink
		/opt/local # DarwinPorts
		/opt/csw # Blastwave
		/opt
)
MESSAGE("SDL_INCLUDE_DIR is ${SDL_INCLUDE_DIR}")

# SDL-1.1 is the name used by FreeBSD ports...
# don't confuse it for the version number.
FIND_LIBRARY(
	SDL_LIBRARY_TEMP 
	NAMES SDL SDL-1.1
	HINTS
	$ENV{SDLDIR}
	PATH_SUFFIXES lib64 lib
	PATHS
		ENV PATH
		/sw
		/opt/local
		/opt/csw
		/opt
)

MESSAGE("SDL_LIBRARY_TEMP is ${SDL_LIBRARY_TEMP}")

IF ( NOT SDL_BUILDING_LIBRARY )
  IF ( NOT ${SDL_INCLUDE_DIR} MATCHES ".framework" )
    FIND_LIBRARY ( 
		SDLMAIN_LIBRARY 
		NAMES SDLmain SDLmain-1.1
		HINTS
		$ENV{SDLDIR}
		PATH_SUFFIXES lib64 lib
		PATHS
			ENV PATH
			/sw
			/opt/local
			/opt/csw
			/opt
    )
  ENDIF ( NOT ${SDL_INCLUDE_DIR} MATCHES ".framework" )
ENDIF ( NOT SDL_BUILDING_LIBRARY )

IF ( NOT APPLE )
  FIND_PACKAGE ( Threads )
ENDIF ( NOT APPLE )

# MinGW needs an additional library, mwindows
# It's total link flags should look like -lmingw32 -lSDLmain -lSDL -lmwindows
# (Actually on second look, I think it only needs one of the m* libraries.)
IF ( MINGW )
  SET ( MINGW32_LIBRARY mingw32 CACHE STRING "mwindows for MinGW" )
ENDIF ( MINGW )

SET ( SDL_FOUND "NO" )
IF ( SDL_LIBRARY_TEMP )
  # For SDLmain
  IF ( NOT SDL_BUILDING_LIBRARY )
    IF ( SDLMAIN_LIBRARY )
      SET ( SDL_LIBRARY_TEMP ${SDLMAIN_LIBRARY} ${SDL_LIBRARY_TEMP} )
    ENDIF ( SDLMAIN_LIBRARY )
  ENDIF ( NOT SDL_BUILDING_LIBRARY )

  # For OS X, SDL uses Cocoa as a backend so it must link to Cocoa.
  # CMake doesn't display the -framework Cocoa string in the UI even 
  # though it actually is there if I modify a pre-used variable.
  # I think it has something to do with the CACHE STRING.
  # So I use a temporary variable until the end so I can set the 
  # "real" variable in one-shot.
  IF ( APPLE )
    SET ( SDL_LIBRARY_TEMP ${SDL_LIBRARY_TEMP} "-framework Cocoa" )
  ENDIF( APPLE )
    
  # For threads, as mentioned Apple doesn't need this.
  # In fact, there seems to be a problem if I used the Threads package
  # and try using this line, so I'm just skipping it entirely for OS X.
  IF( NOT APPLE )
    SET ( SDL_LIBRARY_TEMP ${SDL_LIBRARY_TEMP} ${CMAKE_THREAD_LIBS_INIT} )
  ENDIF( NOT APPLE )

  # For MinGW library
  IF( MINGW )
    SET ( SDL_LIBRARY_TEMP ${MINGW32_LIBRARY} ${SDL_LIBRARY_TEMP} )
  ENDIF ( MINGW )

  # Set the final string here so the GUI reflects the final state.
  SET ( SDL_LIBRARY ${SDL_LIBRARY_TEMP} CACHE STRING "Where the SDL Library can be found" )
  # Set the temp variable to INTERNAL so it is not seen in the CMake GUI
  SET ( SDL_LIBRARY_TEMP "${SDL_LIBRARY_TEMP}" CACHE INTERNAL "" )

  SET ( SDL_FOUND "YES" )
ENDIF ( SDL_LIBRARY_TEMP )

MESSAGE("SDL_LIBRARY is ${SDL_LIBRARY}")
