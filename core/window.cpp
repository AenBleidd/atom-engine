#include <SDL/SDL.h>
#include "window.h"

bool MakeWindow(int width, int height, int bpp, const char *title,MODE mode)
{
	int flags = SDL_HWSURFACE | SDL_DOUBLEBUF;
	if(mode==FULLSCREEN) flags |= SDL_FULLSCREEN;
	
	SDL_Event event;
	SDL_Surface *screen;

	if(SDL_Init(SDL_INIT_VIDEO)<0) return false;
	if(!(screen = SDL_SetVideoMode(width, height, bpp, flags))) {SDL_Quit(); return false;}
	if(SDL_FillRect(screen, &screen->clip_rect, SDL_MapRGB(screen->format, 255, 255, 255))) {SDL_Quit(); return false;};
	if(SDL_Flip(screen)) {SDL_Quit(); return false;};
	SDL_WM_SetCaption(title, title);

	while(true) 
	{
		while(SDL_PollEvent(&event)) 
		{
			switch(event.type)
			{
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