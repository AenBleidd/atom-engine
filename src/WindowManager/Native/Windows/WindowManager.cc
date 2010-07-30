#include "WindowManager.h"

    /* TODO:          * Fix Noborder and FullScreen styles.
    */

AtomWindow::AtomWindow( AtomLog* Log ) : OWindow( Log ) {}

AtomWindow::~AtomWindow()
{
    if( !UnregisterClass( clsname, hInstance ) )
    {
        log->SetLastWrn( ERROR_ENGINE_WM, ERROR_CLSUNREG_FAIL );
        snprintf( ( char* ) log->MsgBuf, sizeof( log->MsgBuf ), 
            "Warning: System was unable to unregister the window class %s", clsname );
        log->LogMessage( log->MsgBuf );
    }
}

bool AtomWindow::Create( void )
{
    if( !InitClass() )
    {
        log->SetLastErr( ERROR_ENGINE_WM, ERROR_CLSREG_FAIL );
        log->LogMessage( "Engine Fatal Error: The windowing subsystem cannot be initialized." );
        return false;
    }

    /* Some input parameters checks */
    if( 
        properties.x < 0 ||
        properties.y < 0 ||
        properties.width < 0 ||
        properties.height < 0
    )
    {
        log->SetLastErr( ERROR_ENGINE_WM, ERROR_INVALID_INPUT_PARAMETER );
        log->LogMessage( "Error: Window position or dimentions cannot be a negative value." );
        return false;
    }

    /* Set the proper window styles */
    DWORD dwStyle = WS_CLIPCHILDREN | WS_CLIPSIBLINGS,
    dwExStyle = 0;
    if( properties.visibility == visible )
        dwStyle |= WS_VISIBLE;
    if( properties.effect == maximized )
        dwStyle |= WS_MAXIMIZE;
    if( properties.effect == minimized )
        dwStyle |= WS_MINIMIZE;
    if( properties.border == sizeable )
        dwStyle |= WS_BORDER | WS_SIZEBOX;
    if( properties.border == fixed )
        dwStyle |= WS_BORDER | WS_DLGFRAME;

    /* Set the window control button set */

    if( buttons.minimize )
        dwStyle |= WS_CAPTION | WS_MINIMIZEBOX;
    if( buttons.maximize )
        dwStyle |= WS_CAPTION | WS_MAXIMIZEBOX;	
    if( buttons.exit )
        dwStyle |= WS_CAPTION | WS_SYSMENU;
    if( buttons.help )
        dwExStyle |= WS_EX_CONTEXTHELP;
		
    /* Try to create a native window */
    hWnd = CreateWindowEx (
        0,
        ( LPCTSTR ) clsname,
        ( LPCTSTR ) properties.title,
        dwStyle,
        properties.x, 
        properties.y, 
        properties.width, 
        properties.height,
        NULL,
        NULL,
        hInstance,
        NULL
    );
	
    snprintf( ( char* ) log->MsgBuf, sizeof( log->MsgBuf ), "Debug: CreateWindowEx() [API] returned: %d", hWnd );
    log->DebugMessage( log->MsgBuf );	

    if( !hWnd )
    {
        log->SetLastErr( ERROR_ENGINE_WM, ERROR_WND_CR_FAIL );
        log->LogMessage( "Engine Fatal Error: Window cannot be created." );
        return false;
    }
    
    if( properties.effect == maximized )
        ShowWindow( hWnd, SW_SHOWMAXIMIZED );
    if( properties.effect == minimized )
        ShowWindow( hWnd, SW_SHOWMINIMIZED );

    return true;
}

bool AtomWindow::Show( void )
{
    return ShowWindow( hWnd, SW_SHOW );
}

bool AtomWindow::Hide( void )
{
    return ShowWindow( hWnd, SW_HIDE );
}

bool AtomWindow::Minimize( void )
{
    return ShowWindow( hWnd, SW_MINIMIZE );
}

bool AtomWindow::Maximize( void )
{
    return ShowWindow( hWnd, SW_MAXIMIZE );
}

bool AtomWindow::Restore( void )
{
    return ShowWindow( hWnd, SW_RESTORE );
}

bool AtomWindow::SetCaption( const char* Caption )
{
    return SetWindowText( hWnd, Caption );
}

bool AtomWindow::GetCaption( char const* Caption, int nMaxCount )
{
    return GetWindowText( hWnd, ( char* ) Caption, nMaxCount );
}

bool AtomWindow::SetWindowRect( const RECT* Rect )
{
    return MoveWindow( hWnd, Rect->left, Rect->top, 
        Rect->right - Rect->left, Rect->bottom - Rect->top, true );
}

bool AtomWindow::GetWindowRect( RECT const* Rect )
{
    return ::GetWindowRect( hWnd, ( LPRECT ) Rect );
}

bool AtomWindow::GetWindowInfo( WNDINFO* Info )
{
    /* Window Title */
    if( !GetWindowText( hWnd, ( char* ) Info->title, sizeof( Info->title ) ) )
    {
        log->SetLastErr( ERROR_ENGINE_WM, ERROR_TITLE );
        log->DebugMessage( "Error: GetWindowInfo() : Call to GetWindowText() has failed" );
        return false;
    }

    WINDOWINFO wi;
    if( !::GetWindowInfo( hWnd, &wi ) )
    {
        log->SetLastErr( ERROR_ENGINE_WM, ERROR_WINDOWINFO_QUERY_FAIL );
        log->DebugMessage( "Error: GetWindowInfo() [API] : Call to GetWindowInfo() has failed" );
        return false;
    }
	
    /* Window size and position */
    Info->x = wi.rcWindow.left;
    Info->y = wi.rcWindow.top;
    Info->width = wi.rcWindow.right - wi.rcWindow.left;
    Info->height = wi.rcWindow.bottom - wi.rcWindow.top;

    /* Window client area size and position */
    Info->client_x = wi.rcClient.left;
    Info->client_y = wi.rcClient.top;
    Info->client_width = wi.rcClient.right - wi.rcClient.left;
    Info->client_height = wi.rcClient.bottom - wi.rcClient.top;	
	
    /* IsMinimized */
    if( IsIconic( hWnd ) )
        Info->IsMinimized = ON;
    else Info->IsMinimized = OFF;

    /* IsMaximized */
    if( IsZoomed( hWnd ) )
        Info->IsMaximized = ON;
    else Info->IsMaximized = OFF;

    /* IsFullScreen */
    /* TODO: DoSomething(); */

    /* IsBorderSizeable */
    if( wi.dwStyle & WS_SIZEBOX )
        Info->IsBorderSizeable = ON;
    else Info->IsBorderSizeable = OFF;

    /* IsBorderFixed */
    if( wi.dwStyle & WS_DLGFRAME )
        Info->IsBorderFixed = ON;
    else Info->IsBorderFixed = OFF;

    /* IsTopMost */
    if( wi.dwExStyle & WS_EX_TOPMOST )
        Info->IsTopMost = ON;
    else Info->IsTopMost = OFF;

    /* HasMinimizeButton */
    if( wi.dwStyle & WS_MINIMIZEBOX )
        Info->HasMinimizeButton = ON;
    else Info->HasMinimizeButton = OFF;

    /* HasMaximizeButton */
    if( wi.dwStyle & WS_MINIMIZEBOX )
        Info->HasMaximizeButton = ON;
    else Info->HasMaximizeButton = OFF;

    /* HasExitButton */
    if( wi.dwStyle & WS_SYSMENU )
        Info->HasExitButton = ON;
    else Info->HasExitButton = OFF;	

    /* HasHelpButton */
    if( wi.dwExStyle & WS_EX_CONTEXTHELP )
        Info->HasHelpButton = ON;
    else Info->HasHelpButton = OFF;

    return true;
}

bool AtomWindow::SetWindowInfo( const WNDINFO* Info )
{
    /* Window Title */
    if( !SetWindowText( hWnd, ( char* ) Info->title ) )
    {
        log->SetLastErr( ERROR_ENGINE_WM, ERROR_WINDOWINFO_SETTEXT );
        log->DebugMessage( "Error: SetWindowInfo() : Call to SetWindowText() has failed" );
        return false;
    }

    /* Window size and position */
    if( !MoveWindow( hWnd, Info->x, Info->y, Info->width, Info->height, true ) )
    {
        log->SetLastErr( ERROR_ENGINE_WM, ERROR_MVWIN_FAIL );
        log->DebugMessage( "Error: SetWindowInfo() : Call to MoveWindow() has failed" );
        return false;
    }

    WINDOWINFO wi;
	
    /* Window client area size and position */
    if( Info->client_x && Info->client_y &&
        Info->client_width && Info->client_height )
    {
        if( !::GetWindowInfo( hWnd, &wi ) )
        {
            log->SetLastErr( ERROR_ENGINE_WM, ERROR_WINDOWINFO_QUERY_FAIL );
            log->DebugMessage( "Error: SetWindowInfo() : Call to GetWindowInfo() has failed" );
            return false;
        }		

        RECT WndRect;
        WndRect.left = Info->client_x;
        WndRect.top = Info->client_y;
        WndRect.right = Info->client_x + Info->client_width;
        WndRect.bottom = Info->client_y + Info->client_height;

        if( !AdjustWindowRectEx( &WndRect, wi.dwStyle, false, wi.dwExStyle ) )
        {
            log->SetLastErr( ERROR_ENGINE_WM, ERROR_ADJUSTRECT_FAIL );
            log->DebugMessage( "Error: SetWindowInfo() : Call to AdjustWindowRectEx() has failed" );
            return false;
        }		

        if( !MoveWindow( hWnd, WndRect.left, WndRect.top, 
        WndRect.right - WndRect.left, WndRect.bottom - WndRect.top, true ) )
        {
            log->SetLastErr( ERROR_ENGINE_WM, ERROR_MVWIN_FAIL );
            log->DebugMessage( "Error: SetWindowInfo() : Call to MoveWindow() has failed" );
            return false;
        }
    }
	

    if( Info->IsMinimized && !IsIconic( hWnd ) )
    {
        if( !::GetWindowInfo( hWnd, &wi ) )
        {
            log->SetLastErr( ERROR_ENGINE_WM, ERROR_WINDOWINFO_QUERY_FAIL );
            log->DebugMessage( "Error: SetWindowInfo() : Call to GetWindowInfo() has failed" );
            return false;
        }		        SetClassLong( hWnd, GWL_STYLE, wi.dwStyle |= WS_MINIMIZE );
        ShowWindow( hWnd, SW_SHOWMINIMIZED );
    }


    if( Info->IsMaximized && !IsZoomed( hWnd ) )
    {
        if( !::GetWindowInfo( hWnd, &wi ) )
        {
            log->SetLastErr( ERROR_ENGINE_WM, ERROR_WINDOWINFO_QUERY_FAIL );
            log->DebugMessage( "Error: SetWindowInfo() : Call to GetWindowInfo() has failed" );
            return false;
        }		
        SetClassLong( hWnd, GWL_STYLE, wi.dwStyle |= WS_MAXIMIZE );
        ShowWindow( hWnd, SW_SHOWMAXIMIZED );		
    }

    /* IsFullScreen */
    /* TODO: DoSomething(); */

    if( Info->IsBorderSizeable )
    {
        if( !::GetWindowInfo( hWnd, &wi ) )
        {
            log->SetLastErr( ERROR_ENGINE_WM, ERROR_WINDOWINFO_QUERY_FAIL );
            log->DebugMessage( "Error: SetWindowInfo() : Call to GetWindowInfo() has failed" );
            return false;
        }		
        SetClassLong( hWnd, GWL_STYLE, wi.dwStyle |= WS_SIZEBOX );
    }

    if( Info->IsBorderFixed )
    {
        if( !::GetWindowInfo( hWnd, &wi ) )
        {
            log->SetLastErr( ERROR_ENGINE_WM, ERROR_WINDOWINFO_QUERY_FAIL );
            log->DebugMessage( "Error: SetWindowInfo() : Call to GetWindowInfo() has failed" );
            return false;
        }
        SetClassLong( hWnd, GWL_STYLE, wi.dwStyle |= WS_DLGFRAME );
    }

    if( Info->IsTopMost )
    {
        if( !BringWindowToTop( hWnd ) )
        {
            log->SetLastErr( ERROR_ENGINE_WM, ERROR_BRING_TO_TOP );
            log->DebugMessage( "Error: SetWindowInfo() : Call to BringWindowToTop() has failed" );
            return false;
        }		
    }

    if( Info->HasMinimizeButton )
    {
        if( !::GetWindowInfo( hWnd, &wi ) )
        {
            log->SetLastErr( ERROR_ENGINE_WM, ERROR_WINDOWINFO_QUERY_FAIL );
            log->DebugMessage( "Error: SetWindowInfo() : Call to GetWindowInfo() has failed" );
            return false;
        }
        SetClassLong( hWnd, GWL_STYLE, wi.dwStyle |= WS_MINIMIZE | WS_MINIMIZEBOX );
    }


    if( Info->HasMaximizeButton )
    {
        if( !::GetWindowInfo( hWnd, &wi ) )
        {
            log->SetLastErr( ERROR_ENGINE_WM, ERROR_WINDOWINFO_QUERY_FAIL );
            log->DebugMessage( "Error: SetWindowInfo() : Call to GetWindowInfo() has failed" );
            return false;
        }
        SetClassLong( hWnd, GWL_STYLE, wi.dwStyle |= WS_MAXIMIZE | WS_MAXIMIZEBOX );
    }

    if( Info->HasExitButton )
    {
        if( !::GetWindowInfo( hWnd, &wi ) )
        {
            log->SetLastErr( ERROR_ENGINE_WM, ERROR_WINDOWINFO_QUERY_FAIL );
            log->DebugMessage( "Error: SetWindowInfo() : Call to GetWindowInfo() has failed" );
            return false;
        }        SetClassLong( hWnd, GWL_STYLE, wi.dwStyle |= WS_SYSMENU );
    }

    if( Info->HasHelpButton )
    {
        if( !::GetWindowInfo( hWnd, &wi ) )
        {
            log->SetLastErr( ERROR_ENGINE_WM, ERROR_WINDOWINFO_QUERY_FAIL );
            log->DebugMessage( "Error: SetWindowInfo() : Call to GetWindowInfo() has failed" );
            return false;
        }
        SetClassLong( hWnd, GWL_EXSTYLE, wi.dwStyle |= WS_EX_CONTEXTHELP );
    }

    return true;
}

bool AtomWindow::GetWindowRect( RECT* WindowRect )
{
    return ::GetWindowRect( hWnd, WindowRect );
}

bool AtomWindow::GetClientRect( RECT* ClientRect )
{
    return ::GetClientRect( hWnd, ClientRect );
}

bool AtomWindow::Close( void )
{
    return CloseWindow( hWnd );
}

bool AtomWindow::Destroy( void )
{
    return DestroyWindow( hWnd );
}
	
int AtomWindow::Run( void )
{
    MSG msg;
    while( GetMessage( &msg, NULL, 0, 0 ) )
    {
        TranslateMessage( &msg );
        DispatchMessage( &msg );
    }
    return msg.wParam;
}

LRESULT CALLBACK AtomWindow::WindowProcedure( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
    switch( uMsg )
    {
        case WM_CLOSE:
        case WM_DESTROY:
            PostQuitMessage( EXIT_SUCCESS );
            break;
    }
    return DefWindowProc( hWnd, uMsg, wParam, lParam );
}

bool AtomWindow::InitClass( void )
{
    /* Produce a suitable class name */
    static int CN = 0;
    memset( &clsname[0], 0, sizeof( clsname ) );
    snprintf( &clsname[0], sizeof( clsname ), "ATOM_%d", CN++ );

    /* Create a class for the window */
    WNDCLASSEX wcl;
    memset( &wcl, 0, sizeof( wcl ) );
    wcl.cbSize = sizeof ( wcl );
    wcl.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
    wcl.lpfnWndProc = Window::WindowProcedure;
    wcl.hInstance = hInstance = GetModuleHandle( NULL );
    wcl.hIcon = LoadIcon( NULL, IDI_APPLICATION );
    wcl.hIconSm = LoadIcon( NULL, IDI_APPLICATION );
    wcl.hCursor = LoadCursor( NULL, IDC_ARROW );
    wcl.hbrBackground = GetStockBrush( WHITE_BRUSH );
    wcl.lpszClassName = clsname;

    /* Try to register class in the system */
    retval = RegisterClassEx( &wcl );
    snprintf( ( char* ) log->MsgBuf, sizeof( log->MsgBuf ), "Debug: RegisterClassEx() returned: %d", retval );
    log->DebugMessage( log->MsgBuf );
    if( !retval )
        return false;
    return true;
}
