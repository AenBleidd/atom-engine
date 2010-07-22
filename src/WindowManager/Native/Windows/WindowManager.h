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
	bool Show( void );
    bool Hide( void );
    bool Minimize( void );
    bool Maximize( void );
    bool Restore( void );
    int Run( void );
    static LRESULT CALLBACK WindowProcedure( HWND, UINT, WPARAM, LPARAM ); // callback event handler
private:
    char clsname[0xFF]; // window classname
    int retval;         // value returned by the function - use for debug purposes only.
    bool InitClass( void );
    HWND hWnd;
    HINSTANCE hInstance;
};

#endif
