#include <SDL/SDL.h>
#include <windows.h>
#include <gl/gl.h>
#include <gl/glu.h>
#include "window.h"

bool MakeWindow(int width, int height, int bpp, const char *title,MODE mode)
{
	int flags = SDL_HWSURFACE | SDL_OPENGL | SDL_DOUBLEBUF;
	if(mode==FULLSCREEN) flags |= SDL_FULLSCREEN;
	
	SDL_Event event;
	SDL_Surface *screen;

	if(SDL_Init(SDL_INIT_VIDEO)<0) return false;

	/* Initialize some OpenGL attributes */
	SDL_GL_SetAttribute( SDL_GL_RED_SIZE, 8 );
	SDL_GL_SetAttribute( SDL_GL_GREEN_SIZE, 8 );
	SDL_GL_SetAttribute( SDL_GL_BLUE_SIZE, 8 );
	SDL_GL_SetAttribute( SDL_GL_ALPHA_SIZE, 8 );
	SDL_GL_SetAttribute( SDL_GL_DEPTH_SIZE, 16 );
	SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );

	/* Set the video mode */
	if(!(screen = SDL_SetVideoMode(width, height, bpp, flags))) {SDL_Quit(); return false;}

	/* Paint the background white */
	if(SDL_FillRect(screen, &screen->clip_rect, SDL_MapRGB(screen->format, 255, 255, 255))) {SDL_Quit(); return false;}

	/* Apply the changes to the screen */
	if(SDL_Flip(screen)) {SDL_Quit(); return false;};

	/* Set window title */
	SDL_WM_SetCaption(title, title);

	/* send a video resize event to paint the screen */
	SDL_Event resizeEvent;
	resizeEvent.type=SDL_VIDEORESIZE;
	resizeEvent.resize.w=width;
	resizeEvent.resize.h=height;
	SDL_PushEvent(&resizeEvent);

	/* Process window message queue */
	while(true) 
	{
		while(SDL_PollEvent(&event)) 
		{
			switch(event.type)
			{
				case SDL_VIDEORESIZE:
						glViewport(0,0,width,height);
						glClearColor(1.0f, 1.0f, 0.0f, 1.0f);
						glClear(GL_COLOR_BUFFER_BIT);	
						glLoadIdentity();
						SDL_GL_SwapBuffers();
						break;

				case SDL_QUIT:
					goto FIN;

				case SDL_KEYDOWN:
					if(event.key.keysym.sym == SDLK_ESCAPE)
						goto FIN;

				default:
					break;
			}
		}// while SQL_PollEvent
	}// while (1)
	
	FIN: 
	SDL_Quit();
	return true;
}