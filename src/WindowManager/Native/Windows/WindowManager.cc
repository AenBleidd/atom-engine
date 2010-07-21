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
        log->SetLastErr( ERROR_ENGINE_WM, ERROR_CLSREG_FAILED );
		
    }
}

LRESULT CALLBACK Window::WindowProcedure( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
}

bool Window::InitClass( void )
{
    /* Produce a suitable class name */
    static int CN = 0;
    char clsname[0xFF];
    memset( &clsname[0], 0, sizeof( clsname ) );
    snprintf( &clsname[0], sizeof( clsname ), "ATOM_%d", CN++ );

    /* Create a class for the window */
    WNDCLASSEX wcl;
    memset( &wcl, 0, sizeof( wcl ) );
    wcl.cbSize = sizeof ( wcl );
    wcl.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
    wcl.lpfnWndProc = Window::WindowProcedure;
    wcl.hInstance = GetModuleHandle( NULL );
    wcl.hIcon = LoadIcon( NULL, IDI_APPLICATION );
    wcl.hIconSm = LoadIcon( NULL, IDI_APPLICATION );
    wcl.hCursor = LoadCursor( NULL, IDC_ARROW );
    wcl.hbrBackground = GetStockBrush( WHITE_BRUSH );
    wcl.lpszClassName = clsname;

    /* Try to register class in the system */
    if( !RegisterClassEx( &wcl ) )
        return false;
    return true;
}
