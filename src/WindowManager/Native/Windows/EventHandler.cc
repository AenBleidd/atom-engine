#include "EventHandler.h"

EventHandler::EventHandler()
{
}

EventHandler::~EventHandler()
{
}

void EventHandler::Run( void )
{
    while( GetMessage( &msg, NULL, 0, 0 ) )
    {
        TranslateMessage( &msg );
        DispatchMessage( &msg );
    }
}

LRESULT CALLBACK EventHandler::WindowProcedure( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
    AtomWindow* receiver = ( AtomWindow* ) GetWindowLongPtr( hWnd, GWLP_USERDATA );
    
    switch( uMsg )
    {
        case WM_CREATE:
            receiver->OnCreate();
            break;
        case WM_SIZE:
            break;
        case WM_CLOSE:
            receiver->OnClose();
            break;
        case WM_DESTROY:
            receiver->OnDestroy();
            break;
        case WM_QUIT:
            receiver->OnQuit();
            break;
    }
    return DefWindowProc( hWnd, uMsg, wParam, lParam );
}