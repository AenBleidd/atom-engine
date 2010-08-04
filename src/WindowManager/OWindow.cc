#include "OWindow.h"

OWindow::OWindow( AtomLog* Log )
{
    log = Log;
}

FLAGS OWindow::SetWindowFlags( VISIBILITY_FLAGS vflags, BORDER_FLAGS brdflags, WND_BUTTON_FLAGS btnflags )
{
    return vflags | brdflags | btnflags;
}

void __fastcall OWindow::OnClose( void ){}
void __fastcall OWindow::OnCreate( void ){}
void __fastcall OWindow::OnDestroy( void ){}
void __fastcall OWindow::OnQuit( void ){}