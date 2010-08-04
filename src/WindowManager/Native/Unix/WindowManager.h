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

    /* inherited functions */
    bool Create( const char*, int, int, int, int, FLAGS );
    bool Show( void );
    bool Hide( void );
    bool ShowMinimized( void );
    bool ShowMaximized( void );
    bool ShowNormal( void );
    bool Update( void );
    bool Minimize( void );
    bool Maximize( void );
    bool Restore( void );
    bool SetCaption( const char* );
    bool GetCaption( const char*, int );
    bool SetWindowRect( const PATOMRECT );
    bool GetWindowRect( PATOMRECT const );
    bool GetClientRect( PATOMRECT const );
    bool Close( void );
    bool Destroy( void );
    bool IsVisible( void );
    bool IsMinimized( void );
    bool IsMaximized( void );
    bool IsFullScreen( void );
    bool IsBorderSizeable( void );
    bool IsBorderFixed( void );
    bool IsTopMost( void );
    bool HasMinimizeButton( void );
    bool HasMaximizeButton( void );
    bool HasExitButton( void );
    bool HasHelpButton( void );

    /* inherited events */
    void OnCreate( void );
    void OnClose( void );
    void OnDestroy( void );
    void OnQuit( void );    
private:
    char* DisplayName;
    Display *display;
    Window window;
};

#endif
