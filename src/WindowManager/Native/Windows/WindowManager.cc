#include "WindowManager.h"
#include "EventHandler.h"

    /* TODO:     
     * Fix Noborder and FullScreen styles.
    */

void TranslateRect( LPRECT SystemRect, PATOMRECT AtomRect )
{
    AtomRect->x = SystemRect->left;
    AtomRect->y = SystemRect->top;
    AtomRect->width = SystemRect->right - SystemRect->left;
    AtomRect->height = SystemRect->bottom - SystemRect->top;
}    
    
AtomWindow::AtomWindow( AtomLog& Log ) : OWindow( Log ) 
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
    wcl.lpfnWndProc = EventHandler::WindowProcedure;
    wcl.hInstance = hInstance = GetModuleHandle( NULL );
    wcl.hIcon = LoadIcon( NULL, IDI_APPLICATION );
    wcl.hIconSm = LoadIcon( NULL, IDI_APPLICATION );
    wcl.hCursor = LoadCursor( NULL, IDC_ARROW );
    wcl.hbrBackground = GetStockBrush( WHITE_BRUSH );
    wcl.lpszClassName = clsname;

    /* Try to register class in the system */
    if( !RegisterClassEx( &wcl ) )
        throw ATOMWM_INIT_EXCEPTION;
}

AtomWindow::~AtomWindow()
{
    if( !UnregisterClass( clsname, hInstance ) )
        log.SetLastWrn( ERROR_ENGINE_WM, ERROR_CLSUNREG_FAIL );
}

bool AtomWindow::Create( const char* TitleText, int x, int y, int width, int height, FLAGS flags )
{
    DWORD dwStyle = WS_CLIPCHILDREN | WS_CLIPSIBLINGS,
    dwExStyle = 0;

    if( flags & ATOMWND_VISIBLE )
        dwStyle |= WS_VISIBLE;
    if( flags & ATOMWND_BORDER_SIZEABLE )
        dwStyle |= WS_BORDER | WS_SIZEBOX;
    if( flags & ATOMWND_BORDER_FIXED )
        dwStyle |= WS_BORDER | WS_DLGFRAME;
    if( flags & ATOMWND_BTN_MINIMIZE )
        dwStyle |= WS_MINIMIZEBOX;
    if( flags & ATOMWND_BTN_MAXIMIZE )
        dwStyle |= WS_MAXIMIZEBOX;
    if( flags & ATOMWND_BTN_EXIT )
        dwStyle |= WS_SYSMENU;
    if( flags & ATOMWND_BTN_HELP )
        dwExStyle |= WS_EX_CONTEXTHELP;

    /* Try to create a native window */
    hWnd = CreateWindowEx (
        dwExStyle,
        ( LPCTSTR ) clsname,
        ( LPCTSTR ) TitleText,
        dwStyle,
        x, 
        y, 
        width, 
        height,
        NULL,
        NULL,
        hInstance,
        NULL
    );
	
    if( !hWnd )
    {
        log.SetLastErr( ERROR_ENGINE_WM, ERROR_WND_CR_FAIL );
        log.LogMessage( "Engine Fatal Error: Window cannot be created." );
        return false;
    }
    
    SetWindowLong( hWnd, GWLP_USERDATA, ( LONG_PTR ) this );
    OnCreate();
    
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

bool AtomWindow::ShowMinimized( void )
{
    return ShowWindow( hWnd, SW_SHOWMINIMIZED );
}

bool AtomWindow::ShowMaximized( void )
{
    return ShowWindow( hWnd, SW_SHOWMAXIMIZED );
}

bool AtomWindow::ShowNormal( void )
{
    return ShowWindow( hWnd, SW_SHOWNORMAL );
}

bool AtomWindow::Update( void )
{
    return UpdateWindow( hWnd );
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

bool AtomWindow::GetCaption( const char* Caption, int nMaxCount )
{
    return GetWindowText( hWnd, ( char* ) Caption, nMaxCount );
}

bool AtomWindow::SetWindowRect( const PATOMRECT Rect )
{
    return MoveWindow( hWnd, Rect->x, Rect->y, 
        Rect->width, Rect->height, true );
}

bool AtomWindow::GetWindowRect( PATOMRECT const WindowRect )
{
    RECT rect;
    if( !::GetWindowRect( hWnd, ( LPRECT ) &rect ) )
        return false;

    ::TranslateRect( &rect, WindowRect );
    return true;
}

bool AtomWindow::GetClientRect( PATOMRECT const ClientRect )
{
    RECT rect;
    if( !::GetClientRect( hWnd, ( LPRECT ) &rect ) )
        return false;

    ::TranslateRect( &rect, ClientRect );
    return true;
}

bool AtomWindow::Close( void )
{
    return CloseWindow( hWnd );
}

bool AtomWindow::Destroy( void )
{
    return DestroyWindow( hWnd );
}

bool AtomWindow::IsVisible( void )
{
    WINDOWINFO wi;
    if( !GetWindowInfo( hWnd, &wi ) )
    {
        log.SetLastErr( ERROR_ENGINE_WM, ERROR_WINDOWINFO_QUERY_FAIL );
        log.DebugMessage( "Error: AtomWindow::IsVisible() : Call to GetWindowInfo() has failed" );
        return false;
    }

    if( wi.dwStyle & WS_VISIBLE )
        return true;
        
    return false;
}

bool AtomWindow::IsMinimized( void )
{
    return ::IsIconic( hWnd );
}

bool AtomWindow::IsMaximized( void )
{
    return ::IsZoomed( hWnd );
}

bool AtomWindow::IsFullScreen( void )
{
	return false;
}

bool AtomWindow::IsBorderSizeable( void )
{
    WINDOWINFO wi;
    if( !GetWindowInfo( hWnd, &wi ) )
    {
        log.SetLastErr( ERROR_ENGINE_WM, ERROR_WINDOWINFO_QUERY_FAIL );
        log.DebugMessage( "Error: AtomWindow::IsBorderSizeable() : Call to GetWindowInfo() has failed" );
        return false;
    }

    if( wi.dwStyle & WS_SIZEBOX )
        return true;
        
    return false;
}

bool AtomWindow::IsBorderFixed( void )
{
    if( IsBorderSizeable() )
        return false;
    return true;
}

bool AtomWindow::IsTopMost( void )
{
    WINDOWINFO wi;
    if( !GetWindowInfo( hWnd, &wi ) )
    {
        log.SetLastErr( ERROR_ENGINE_WM, ERROR_WINDOWINFO_QUERY_FAIL );
        log.DebugMessage( "Error: AtomWindow::IsTopMost() : Call to GetWindowInfo() has failed" );
        return false;
    }

    if( wi.dwExStyle & WS_EX_TOPMOST )
        return true;
        
    return false;
}

bool AtomWindow::HasMinimizeButton( void )
{
    WINDOWINFO wi;
    if( !GetWindowInfo( hWnd, &wi ) )
    {
        log.SetLastErr( ERROR_ENGINE_WM, ERROR_WINDOWINFO_QUERY_FAIL );
        log.DebugMessage( "Error: AtomWindow::IsMinimizeButton() : Call to GetWindowInfo() has failed" );
        return false;
    }

    if( wi.dwStyle & WS_MINIMIZEBOX )
        return true;
        
    return false;
}

bool AtomWindow::HasMaximizeButton( void )
{
    WINDOWINFO wi;
    if( !GetWindowInfo( hWnd, &wi ) )
    {
        log.SetLastErr( ERROR_ENGINE_WM, ERROR_WINDOWINFO_QUERY_FAIL );
        log.DebugMessage( "Error: AtomWindow::IsMaximizeButton() : Call to GetWindowInfo() has failed" );
        return false;
    }

    if( wi.dwStyle & WS_MAXIMIZEBOX )
        return true;
        
    return false;
}

bool AtomWindow::HasExitButton( void )
{
    WINDOWINFO wi;
    if( !GetWindowInfo( hWnd, &wi ) )
    {
        log.SetLastErr( ERROR_ENGINE_WM, ERROR_WINDOWINFO_QUERY_FAIL );
        log.DebugMessage( "Error: AtomWindow::HasExitButton() : Call to GetWindowInfo() has failed" );
        return false;
    }

    if( wi.dwStyle & WS_SYSMENU )
        return true;
        
    return false;
}

bool AtomWindow::HasHelpButton( void )
{
    WINDOWINFO wi;
    if( !GetWindowInfo( hWnd, &wi ) )
    {
        log.SetLastErr( ERROR_ENGINE_WM, ERROR_WINDOWINFO_QUERY_FAIL );
        log.DebugMessage( "Error: AtomWindow::HasHelpButton() : Call to GetWindowInfo() has failed" );
        return false;
    }

    if( wi.dwExStyle & WS_EX_CONTEXTHELP )
        return true;
        
    return false;
}
   
void AtomWindow::OnCreate( void ){}
void AtomWindow::OnClose( void )
{
    PostQuitMessage( EXIT_SUCCESS );
}
void AtomWindow::OnDestroy( void ){}
void AtomWindow::OnQuit( void ){}
