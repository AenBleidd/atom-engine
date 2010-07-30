#include "WindowManager.h"

/* SYNOPSYS 
 * This is the template for implementation of the OWindow interface
 * in Unix and Unix-like OSes. Please realize those methods provided below
 * using *NATIVE* platform methods. In Unix-like system it, most likely, would be XLib.
*/

/* TODO:
 * Change AtomWindow ctor AtomWindow( AtomLog* Log, const char* DisplayString to 
 * AtomWindow( AtomLog* Log, const char* Protocol, const char* Hostname, 
 * unsigned int DisplayNumber, unsigned int ScreenNumber );
 * DisplayString should be presented as: Protocol/Hostname:DisplayNumber.ScreenNumber
 * Check for format string conformancy to the standards.
 *
 *
 * ACHTUNG!!! A segfault in destructor. Needs urgent debugging and fixing.
 *
*/

AtomWindow::AtomWindow( AtomLog* Log ) : OWindow( Log )
{
    AtomWindow( Log, NULL );
}

AtomWindow::AtomWindow( AtomLog* Log, const char* DisplayString ) : OWindow( Log )
{

    DisplayName = new char[0xFF];
    memset( &DisplayName[0], 0, sizeof( DisplayName ) );
    if( DisplayString )
        strncpy( &DisplayName[0], ( char* ) DisplayString, sizeof( DisplayName ) );


    /* Connect to X server */
    
    if( ( display = XOpenDisplay( DisplayName ) ) == NULL )
    {
        log->SetLastErr( ERROR_ENGINE_WM, ERROR_X11_OPENDISPLAY );
        log->DebugMessage( "AtomWindow ctor : XOpenDisplay returned NULL" );
    }
    else
        log->LogMessage( "Connection to X server opened" );
}

AtomWindow::~AtomWindow()
{
    if( display )
        XCloseDisplay( display );
    delete[] DisplayName;
}

bool AtomWindow::Create( void )
{
    XWindowAttributes windowattr;
    int screen_num = DefaultScreen( display );
    if( XGetWindowAttributes( display, RootWindow(display, screen_num), &windowattr ) )
    {
        window = XCreateSimpleWindow( display, RootWindow( display, screen_num ), 0, 0, windowattr.width/3, windowattr.height/4, 
             4, BlackPixel( display, screen_num ), WhitePixel( display, screen_num ) );
    }
    return true;
}

bool AtomWindow::Show( void )
{
    return XMapWindow( display, window );
}

bool AtomWindow::Hide( void )
{
    return true;
}

bool AtomWindow::Minimize( void )
{
    return true;
}

bool AtomWindow::Maximize( void )
{
    return true;
}

bool AtomWindow::Restore( void )
{
    return true;
}

bool AtomWindow::SetCaption( const char* Caption )
{
    return true;
}

bool AtomWindow::GetCaption( char const* Caption, int nMaxCount )
{
    return true;
}

bool AtomWindow::SetWindowRect( const RECT* Rect )
{
    return true;
}

bool AtomWindow::GetWindowRect( RECT const* Rect )
{
    return true;
}

bool AtomWindow::GetWindowInfo( WNDINFO* Info )
{
	return true;
}

bool AtomWindow::SetWindowInfo( const WNDINFO* Info )
{
	return true;
}

bool AtomWindow::GetWindowRect( RECT* WindowRect )
{
	return true;
}

bool AtomWindow::GetClientRect( RECT* ClientRect )
{
	return true;
}

bool AtomWindow::Close( void )
{
	return true;
}

bool AtomWindow::Destroy( void )
{
	return true;
}
	
int AtomWindow::Run( void )
{
	return 0;
}
