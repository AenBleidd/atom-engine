/* hello.cc */
#include "WindowManager.h"
#include <iostream>

#define WIDTH 800
#define HEIGHT 600
#define BPP 32

using namespace std;

void Window::Print()
{
	char a;
    cout<<"Hello, World!"<<endl;
	cin>>&a;
}

bool Window::Init( void )
{
	int SDL_INIT_FLAGS = SDL_INIT_VIDEO;
	int SDL_FLAGS = SDL_HWSURFACE | SDL_DOUBLEBUF;
	
	if( SDL_Init ( SDL_INIT_FLAGS ) < 0 )
		return false;
		
	/* Set the video mode */
	screen = SDL_SetVideoMode( WIDTH, HEIGHT, BPP, SDL_FLAGS);
	if ( !screen ) 
	{
		SDL_Quit();
		return false;	
	}
	
	/* Paint the background white */
	if( SDL_FillRect( screen, &screen->clip_rect, SDL_MapRGB( screen->format, 255, 255, 255) ) ) 
	{
		SDL_Quit();
		return false;
	}	
	
	/* Set window title */
	SDL_WM_SetCaption( "SDL_Window", "SDL_Window" );
	
	/* send a video resize event to paint the screen */
	SDL_Event resizeEvent;
	resizeEvent.type = SDL_VIDEORESIZE;
	resizeEvent.resize.w = WIDTH;
	resizeEvent.resize.h = HEIGHT;
	SDL_PushEvent(&resizeEvent);	
	
	return true;
}

bool Window::MakeWindow ( void )
{
	// Initialize SDL
	if( !Init() ) 
		return false;	
		
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
					return true;
				break;

				case SDL_KEYDOWN:
					if( event.key.keysym.sym == SDLK_ESCAPE )
						SDL_Quit();
					return true;
				break;

				default:
				break;
			} /* switch( event.type ) */
    }  /* while SQL_PollEvent */
  }  /* while( 1 ) */

  SDL_Quit();
  return true;		
}