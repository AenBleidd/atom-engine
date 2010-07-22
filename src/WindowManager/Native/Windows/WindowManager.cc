#include "WindowManager.h"

Window::Window( AtomLog* Log ) : OWindow( Log ) 
{
	
}

Window::~Window()
{
}

bool Window::Create( void )
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

	DWORD dwStyle = WS_CLIPCHILDREN | WS_CLIPSIBLINGS;
	
    /* Set the proper window styles */
    if( properties.visibility == visible )
        dwStyle |= WS_VISIBLE;
	if( properties.border == sizeable )
		dwStyle |= WS_BORDER | WS_SIZEBOX;
	if( properties.border == fixed )
		dwStyle |= WS_BORDER | WS_DLGFRAME;

    /* TODO:     
	 * Fix Noborder, Minimized and Maximized styles.
	 * Add functions:
	 *   SetWindowInfo()
	 *   GetWindowInfo()
	 *   SetCaption()
	 *   GetCaption()
	 *   SetPosition()
	 *   GetPosition()
	 *   SetDimentions()
	 *   GetDimentions()
	 *   Destroy()
	 */

    /* Set the window control button set */

    if( buttons.minimize )
        dwStyle |= WS_CAPTION | WS_MINIMIZEBOX;
    if( buttons.maximize )
        dwStyle |= WS_CAPTION | WS_MAXIMIZEBOX;	
    if( buttons.exit )
        dwStyle |= WS_CAPTION | WS_SYSMENU;
	
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
	
    snprintf( ( char* ) log->MsgBuf, sizeof( log->MsgBuf ), "Debug: CreateWindowEx() returned: %d", hWnd );
    log->DebugMessage( log->MsgBuf );	

    if( !hWnd )
    {
        log->SetLastErr( ERROR_ENGINE_WM, ERROR_WND_CR_FAIL );
        log->LogMessage( "Engine Fatal Error: Window cannot be created." );
		return false;
    }

    if( buttons.minimize )
	    ShowWindow( hWnd, SW_SHOWMINIMIZED );
    else if( buttons.maximize )
	    ShowWindow( hWnd, SW_SHOWMAXIMIZED );
    return true;
}

bool Window::Show( void )
{
    return ShowWindow( hWnd, SW_SHOW );
}

bool Window::Hide( void )
{
    return ShowWindow( hWnd, SW_HIDE );
}

bool Window::Minimize( void )
{
    return ShowWindow( hWnd, SW_MINIMIZE );
}

bool Window::Maximize( void )
{
    return ShowWindow( hWnd, SW_MAXIMIZE );
}

bool Window::Restore( void )
{
    return ShowWindow( hWnd, SW_RESTORE );
}

int Window::Run( void )
{
    MSG msg;
    while( GetMessage( &msg, NULL, 0, 0 ) )
    {
        TranslateMessage( &msg );
        DispatchMessage( &msg );
    }
	
	return msg.wParam;
}
LRESULT CALLBACK Window::WindowProcedure( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
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

bool Window::InitClass( void )
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
