#include <AtomError.h>
#include <SDL.h>
//#include <SDL_Video.h>
#include "WindowManager.h"

#define WIDTH 50
#define HEIGHT 50
#define BPP 32

Window::Window( AtomLog* Log ) : OWindow( Log )
{
	/* Perform libSDL initialization */
	
	Initialized = Created = false;
	
	if( Init() )
		Initialized = true;
/*	else
		log->SetLastErr( ERROR_ENGINE_WM, ERROR_WM_SDL_INIT_FAILED );	*/
}

Window::~Window()
{
	if( Initialized )
		SDL_Quit();
}

bool Window::Init( void )
{
	int SDL_INIT_FLAGS = SDL_INIT_VIDEO;

	if( SDL_Init ( SDL_INIT_FLAGS ) < 0 )
		return false;
	return true;
}

bool Window::Create( void )
{
	return true;
}


bool Window::MakeWindow ( void )
{
	if( !Initialized ) 
	{
//		log->SetLastErr( ERROR_ENGINE_WM, ERROR_WM_SDL_INIT_FAILED );	
		return false;   
	}
		
	int SDL_FLAGS = SDL_HWSURFACE | SDL_DOUBLEBUF;
	SDL_Surface *screen;		
	
	screen = SDL_SetVideoMode( WIDTH, HEIGHT, BPP, SDL_FLAGS);
	if ( !screen ) 
	{
//		log->SetLastErr( ERROR_ENGINE_WM, ERROR_WM_SDL_WINDOW_CREATION_FAILED );	
		SDL_Quit();
		return false;   
	}
	
	/* Set a window caption */
	SDL_WM_SetCaption( "SDL_Window", "SDL_Window" );
	
	/* send a video resize event to paint the screen */
	SDL_Event resizeEvent;
	resizeEvent.type = SDL_VIDEORESIZE;
	resizeEvent.resize.w = WIDTH;
	resizeEvent.resize.h = HEIGHT;
	SDL_PushEvent(&resizeEvent);	
	
	return true; 
}

void Window::ResizeWindow( int width, int height )
{
	/* send a video resize event */
	SDL_Event resizeEvent;
	resizeEvent.type = SDL_VIDEORESIZE;
	resizeEvent.resize.w = width;
	resizeEvent.resize.h = height;
	SDL_PushEvent(&resizeEvent);		
}

void Window::Run( void )
{
	SDL_Event event;
	
	/* Process window message queue */
	while( true ) {
		while( SDL_PollEvent( &event ) ) 
		{
			switch( event.type ) 
			{
				case SDL_VIDEORESIZE:
					break;

				case SDL_QUIT:
					SDL_Quit();
					return;
					break;

				case SDL_KEYDOWN:
					if( event.key.keysym.sym == SDLK_ESCAPE )
						SDL_Quit();
						return;
					break;

				default:
					break;
			} /* switch( event.type ) */
		}  /* while SQL_PollEvent */
	}  /* while( 1 ) */
}
