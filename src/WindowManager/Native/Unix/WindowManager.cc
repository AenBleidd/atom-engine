#include "WindowManager.h"
#include <unistd.h>

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
    DisplayName = new char[0xFF];
    memset( &DisplayName[0], 0, sizeof( DisplayName ) );
}

AtomWindow::~AtomWindow()
{
    if( display )
        XCloseDisplay( display );
    delete[] DisplayName;
}

bool AtomWindow::XConnect( const char* DisplayName )
{
    /* Connect to X server */

    display = XOpenDisplay( DisplayName );
    
    if( !display )
    {
        log->SetLastErr( ERROR_ENGINE_WM, ERROR_X11_OPENDISPLAY );
        log->DebugMessage( "AtomWindow ctor : XOpenDisplay returned NULL" );
        return false;
    }
    else
        log->LogMessage( "Connection to X server opened" );
    return true;
}

bool AtomWindow::Create( const char* TitleText, int x, int y, int width, int height, FLAGS flags )
{
    if( !display )
    {
        /* SetLastErr */
        return false;
    }

    int screen_num = DefaultScreen( display );
        window = XCreateSimpleWindow( display, RootWindow( display, screen_num ), 
        x, y, width, height, 4, BlackPixel( display, screen_num ), WhitePixel( display, screen_num ) );

//    XSelectInput( display, window, ExposureMask | KeyPressMask );

    XMapWindow( display, window );
    XFlush( display );
    sleep(10);
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

bool AtomWindow::ShowMinimized( void )
{
    return true;
}

bool AtomWindow::ShowMaximized( void )
{
    return true;
}

bool AtomWindow::ShowNormal( void )
{
    return true;
}

bool AtomWindow::Update( void )
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

bool AtomWindow::GetCaption( const char* Caption, int nMaxCount )
{
    return true;
}

bool AtomWindow::SetWindowRect( const PATOMRECT Rect )
{
    return true;
}

bool AtomWindow::GetWindowRect( PATOMRECT const WindowRect )
{
    return true;
}

bool AtomWindow::GetClientRect( PATOMRECT const ClientRect )
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

bool AtomWindow::IsVisible( void )
{
	return true;
}

bool AtomWindow::IsMinimized( void )
{
	return true;
}

bool AtomWindow::IsMaximized( void )
{
	return true;
}

bool AtomWindow::IsFullScreen( void )
{
	return true;
}

bool AtomWindow::IsBorderSizeable( void )
{
	return true;
}

bool AtomWindow::IsBorderFixed( void )
{
	return true;
}

bool AtomWindow::IsTopMost( void )
{
	return true;
}

bool AtomWindow::HasMinimizeButton( void )
{
	return true;
}

bool AtomWindow::HasMaximizeButton( void )
{
	return true;
}

bool AtomWindow::HasExitButton( void )
{
	return true;
}

bool AtomWindow::HasHelpButton( void )
{
	return true;
}

void AtomWindow::OnCreate( void ){}
void AtomWindow::OnClose( void ){}
void AtomWindow::OnDestroy( void ){}
void AtomWindow::OnQuit( void ){}
