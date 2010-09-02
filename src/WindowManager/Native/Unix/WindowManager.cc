#include "WindowManager.h"
#include <unistd.h>

/* SYNOPSYS
 * This is the template for implementation of the OWindow interface
 * in Unix and Unix-like OSes. Please realize those methods provided below
 * using *NATIVE* platform methods. In Unix-like system it, most likely, would be XLib.
*/

/* TODO:
 * Optimize Xlib error handling using those functions:
 * XErrorHandler, XIOErrorHandler
 * XSetErrorHandler, XSetIOErrorHandler
 * XGetErrorText, XGetErrorDatabaseText
 *
*/

AtomWindow::AtomWindow( AtomLog& Log, const char* Hostname, short Display_Number, short Screen_Number ) : OWindow( Log )
{
    /* Make some space to store the display string */
    char DisplayString[0xFF];

    /* create the display string in format "hostname : display_number . screen_number" */
    snprintf( &DisplayString[0], sizeof( DisplayString ), "%s:%d.%d", Hostname, Display_Number, Screen_Number );

    /* Tell where we are going to connect to */
    snprintf( ( char * ) log.MsgBuf, sizeof( log.MsgBuf ), "WM: Connecting to X server on %s", &DisplayString[0] );
    log.LogMessage( log.MsgBuf );

    /* Connect to X server */

    display = XOpenDisplay( NULL );

    if( !display )
    {
        log.SetLastErr( ERROR_ENGINE_WM, ERROR_X11_CONNECTION );
        log.DebugMessage( "ERROR: AtomWindow ctor : XOpenDisplay returned NULL" );
    }
    else
        log.LogMessage( "WM: Connection to X server opened." );
}


AtomWindow::~AtomWindow()
{
    if( display )
    {
        XCloseDisplay( display );
        log.LogMessage( "WM: Connection to X server terminated." );
    }
}

bool AtomWindow::Create( const char* TitleText, int x, int y, int width, int height, FLAGS flags )
{
    /* Check for connection to X server */
    if( !display )
    {
        log.SetLastErr( ERROR_ENGINE_WM, ERROR_X11_CONNECTION );
        log.DebugMessage( "ERROR: AtomWindow::Create(): No display available" );
        return false;
    }

    int screen_num = DefaultScreen( display );
    int border = 4;
    XTextProperty caption;

    if( !XStringListToTextProperty( ( char** ) &TitleText, 1, &caption ))
    {
        log.SetLastErr( ERROR_ENGINE_WM, ERROR_X11_STRING_CONVERSION );
        log.DebugMessage( "ERROR: AtomWindow::SetCaption() : XStringListToTextProperty() call returned NULL" );
        return false;
    }

    /* Set the desired window dimentions and position */
    XSizeHints sh;
    sh.flags = PPosition | PSize;
    sh.x = x;
    sh.y = y;
    sh.width = width;
    sh.height = height;

    /* case we want to get border non-sizeable */
    if( flags & ATOMWND_BORDER_FIXED )
    {
        sh.flags |= PBaseSize | PMinSize | PMaxSize;
        sh.base_width = width;
        sh.base_height = height;
        sh.min_width = sh.max_width = width;
        sh.min_height = sh.max_height = height;
    }

    /* Set the window manager hints */
    XWMHints wmh;
    wmh.flags = InputHint | StateHint;
    wmh.input = true;
    wmh.initial_state = NormalState;

    /* Create a window */
    window = XCreateSimpleWindow( display, RootWindow( display, screen_num ),
        x, y, width, height, 1, BlackPixel( display, screen_num ), WhitePixel( display, screen_num ) );

    /* Apply size hints */
    XSetWMNormalHints( display, window, &sh );
    XSetWMHints( display, window, &wmh );

    /* Set window caption */
    XSetWMName( display, window, &caption );

//    XSelectInput( display, window, ExposureMask | KeyPressMask );

    return true;
}

bool AtomWindow::Show( void )
{
    XMapWindow( display, window );
    XFlush( display );
    return true;
}

bool AtomWindow::Hide( void )
{
    // TODO: Fix this. Not working properly.
    XUnmapWindow( display, window );
    XFlush( display );
    return true;
}

bool AtomWindow::ShowMinimized( void )
{
//    return XIconifyWindow( display, window, DefaultScreen( display ) );
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
    XFlush( display );
    return true;
}

bool AtomWindow::Minimize( void )
{
    // TODO: Fix this. Not working properly.
    return XIconifyWindow( display, window, DefaultScreen( display ) );
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
    XTextProperty caption;

    if( !XStringListToTextProperty( ( char** ) &Caption, 1, &caption ))
    {
        log.SetLastErr( ERROR_ENGINE_WM, ERROR_X11_STRING_CONVERSION );
        log.DebugMessage( "ERROR: AtomWindow::SetCaption() : XStringListToTextProperty() call returned NULL" );
        return false;
    }

    XSetWMName( display, window, &caption );
    return true;
}

bool AtomWindow::GetCaption( const char* Caption, int nMaxCount )
{
    XTextProperty caption;
    if( !XGetWMName( display, window, &caption ) )
        return false;
    Caption = ( const char* ) caption.value;
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
    XDestroyWindow( display, window );
	return true;
}

bool AtomWindow::Destroy( void )
{
    XDestroySubwindows( display, window );
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
    XSizeHints sh;
    long supplied_return;
    if( ! XGetWMNormalHints( display, window, &sh, &supplied_return ) )
    {
        log.SetLastErr( ERROR_ENGINE_WM, ERROR_X11_WM_HINTS );
        log.DebugMessage( "ERROR: AtomWindow::IsBorderSizeable() : XGetNormalHints() call returned NULL" );
        return false;
    }
    if( sh.min_width != sh.max_width &&
        sh.min_height != sh.max_height &&
        sh.min_width != 0 && sh.max_width != 0 &&
        sh.min_height != 0 && sh.max_height != 0 )
	return true;
    return false;
}

bool AtomWindow::IsBorderFixed( void )
{
    XSizeHints sh;
    long supplied_return;
    if( ! XGetWMNormalHints( display, window, &sh, &supplied_return ) )
    {
        log.SetLastErr( ERROR_ENGINE_WM, ERROR_X11_WM_HINTS );
        log.DebugMessage( "ERROR: AtomWindow::IsBorderFixed() : XGetNormalHints() call returned NULL" );
        return false;
    }
    if( sh.min_width == sh.max_width &&
        sh.min_height == sh.max_height &&
        sh.min_width != 0 && sh.max_width != 0 &&
        sh.min_height != 0 && sh.max_height != 0 )
	return true;
    return false;
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
