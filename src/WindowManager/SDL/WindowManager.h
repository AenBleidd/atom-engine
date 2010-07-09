/* hello.h */
#ifndef _WINDOWMANAGER_H
#define _WINDOWMANAGER_H
#include "../OWindow.h"
#include <SDL.h>

class Window : public OWindow
{
public:
    void Print();
	bool MakeWindow( void );
private:
	bool Init( void );
	SDL_Surface *screen;
	SDL_Event event;
};

#endif
