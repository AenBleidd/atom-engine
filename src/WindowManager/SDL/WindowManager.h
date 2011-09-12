#ifndef _WINDOWMANAGER_H
#define _WINDOWMANAGER_H
#include "../OWindow.h"

class Window : public OWindow
{
public:
    Window( AtomLog* Log );
	~Window();
	bool Create( void );
	bool MakeWindow( void );
	void ResizeWindow( int, int );
	void Run( void );
private:
	bool Init( void );
	bool Initialized, Created;
//	SDL_Window* window;
};

#endif
