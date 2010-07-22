#include "WindowManager.h"

/* SYNOPSYS 
 * This is the template for implementation of the OWindow interface
 * in Unix and Unix-like OSes. Please realize those methods provided below
 * using *NATIVE* platform methods. In Unix-like system it, most likely, would be XLib.
*/

Window::Window( AtomLog* Log ) : OWindow( Log ) {}

Window::~Window()
{
}

bool Window::Create( void )
{
    return true;
}

bool Window::Show( void )
{
    return true;
}

bool Window::Hide( void )
{
    return true;
}

bool Window::Minimize( void )
{
    return true;
}

bool Window::Maximize( void )
{
    return true;
}

bool Window::Restore( void )
{
    return true;
}

bool Window::SetCaption( const char* Caption )
{
    return true;
}

bool Window::GetCaption( char const* Caption, int nMaxCount )
{
    return true;
}

bool Window::SetWindowRect( const RECT* Rect )
{
    return true;
}

bool Window::GetWindowRect( RECT const* Rect )
{
    return true;
}

bool Window::GetWindowInfo( WNDINFO* Info )
{
	return true;
}

bool Window::SetWindowInfo( const WNDINFO* Info )
{
	return true;
}

bool Window::GetWindowRect( RECT* WindowRect )
{
	return true;
}

bool Window::GetClientRect( RECT* ClientRect )
{
	return true;
}

bool Window::Close( void )
{
	return true;
}

bool Window::Destroy( void )
{
	return true;
}
	
int Window::Run( void )
{
	return 0;
}
