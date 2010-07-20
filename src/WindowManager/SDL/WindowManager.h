#ifndef _WINDOWMANAGER_H
#define _WINDOWMANAGER_H
#include "../OWindow.h"
#include <SDL.h>

class Window : public OWindow
{
public:
    Window( AtomLog* Log );
	~Window();
	bool MakeWindow( void );
	void ResizeWindow( int, int );
	void Run( void );
private:
	bool Init( void );
	bool Initialized;
};

#endif
