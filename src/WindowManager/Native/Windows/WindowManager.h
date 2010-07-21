#ifndef _WINDOWMANAGER_H_
#define _WINDOWMANAGER_H_
#include <windows.h>
#include <windowsx.h>
#include <OWindow.h>
#include <AtomError.h>

class Window : public OWindow
{
public:
    Window( AtomLog* );
    ~Window();
    bool Create( void );
    void Run( void );
	static LRESULT CALLBACK WindowProcedure( HWND, UINT, WPARAM, LPARAM );
private:
	bool InitClass( void );
};

#endif
