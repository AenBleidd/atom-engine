#include "OWindow.h"

OWindow::OWindow( AtomLog& Log ) : log( Log )
{
    ERROR_ENGINE_WM = log.LoadStrings(ATOMERROR, wm_errors, wm_error_descr);
    printf( "Going to write log\n");
    printf( "Log has been written\n" );
}

FLAGS OWindow::SetWindowFlags( VISIBILITY_FLAGS vflags, BORDER_FLAGS brdflags, WND_BUTTON_FLAGS btnflags )
{
    return vflags | brdflags | btnflags;
}

/*
bool Create( const char*, int, int, int, int, FLAGS ){};
bool Show( void ){};
bool Hide( void ) = 0;
bool ShowMinimized( void ) = 0;
bool ShowMaximized( void ) = 0;
bool ShowNormal( void ) = 0;
bool Update( void ) = 0;
bool Minimize( void ) = 0;
bool Maximize( void ) = 0;
bool Restore( void ) = 0;
bool SetCaption( const char* ) = 0;
bool GetCaption( const char*, int ) = 0;
bool SetWindowRect( const PATOMRECT ) = 0;
bool GetWindowRect( PATOMRECT const ) = 0;
bool GetClientRect( PATOMRECT const ) = 0;
bool Close( void ) = 0;
bool Destroy( void ) = 0;
bool IsVisible( void ) = 0;
bool IsMinimized( void ) = 0;
bool IsMaximized( void ) = 0;
bool IsFullScreen( void ) = 0;
bool IsBorderSizeable( void ) = 0;
bool IsBorderFixed( void ) = 0;
bool IsTopMost( void ) = 0;
bool HasMinimizeButton( void ) = 0;
bool HasMaximizeButton( void ) = 0;
bool HasExitButton( void ) = 0;
bool HasHelpButton( void ) = 0;
*/

void OWindow::OnClose( void ){}
void OWindow::OnCreate( void ){}
void OWindow::OnDestroy( void ){}
void OWindow::OnQuit( void ){}
