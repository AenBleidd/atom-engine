#ifndef _WINDOWMANAGER_H_
#define _WINDOWMANAGER_H_
#include <OWindow.h>
#include <AtomError.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>
#include <X11/Xatom.h>
#include <stdlib.h>
#include <string.h>

class AtomWindow : public OWindow
{
public:
    AtomWindow( AtomLog* );
    ~AtomWindow();
    bool XConnect( const char* );
    bool Create( void );
    bool Show( void );
    bool Hide( void );
    bool Minimize( void );
    bool Maximize( void );
    bool Restore( void );
    bool SetCaption( const char* );
    bool GetCaption( char const*, int );
    bool SetWindowRect( const RECT* );
    bool GetWindowRect( RECT const* );
    bool GetWindowInfo( WNDINFO* );
    bool SetWindowInfo( const WNDINFO* );
    bool GetWindowRect( RECT* );
    bool GetClientRect( RECT* );
    bool Close( void );
    bool Destroy( void );	
    int Run( void );
private:
    char* DisplayName;
    Display *display;
    Window window;
};

#endif
