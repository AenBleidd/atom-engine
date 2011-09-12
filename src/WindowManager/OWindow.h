#ifndef _OWINDOW_H
#define _OWINDOW_H
#include <AtomError.h>
#include <WM_Types.h>

typedef unsigned int FLAGS;

enum VISIBILITY_FLAGS
{
    ATOMWND_VISIBLE             = 0x00000001
};

enum BORDER_FLAGS
{
    ATOMWND_BORDER_SIZEABLE     = 0x00000002,
    ATOMWND_BORDER_FIXED        = 0x00000004,
    ATOMWND_NOBORDER            = 0x00000008
};

enum WND_BUTTON_FLAGS
{
    ATOMWND_BTN_MINIMIZE        = 0x00000010,
    ATOMWND_BTN_MAXIMIZE        = 0x00000020,
    ATOMWND_BTN_EXIT            = 0x00000040,
    ATOMWND_BTN_HELP            = 0x00000080
};
// Error Codes
static int32_t ERROR_ENGINE_WM = -1;
static char *wm_error_descr = "Engine Error. WindowManager Error.";
/* WindowManager Error Codes */
#define NO_ERROR                                                     0x00000000
#define ERROR_CLSREG_FAIL                                            0x00000001
#define ERROR_WND_CR_FAIL                                            0x00000002
#define ERROR_INVALID_INPUT_PARAMETER                                0x00000003
#define ERROR_CLSUNREG_FAIL                                          0x00000004
#define ERROR_TITLE                                                  0x00000005
#define ERROR_WINDOWINFO_QUERY_FAIL                                  0x00000006
#define ERROR_WINDOWINFO_SETTEXT                                     0x00000007
#define ERROR_MVWIN_FAIL                                             0x00000008
#define ERROR_ADJUSTRECT_FAIL                                        0x00000009
#define ERROR_BRING_TO_TOP                                           0x0000000A
#define ERROR_X11_CONNECTION                                         0x0000000B
#define ERROR_X11_WM_HINTS                                           0x0000000C
#define ERROR_X11_STRING_CONVERSION                                  0x0000000D
static char *wm_errors[] = {
"No Error",
"Fatal Error: Error registering the window class",
"Fatal Errpr: Couldn't create a window",
"Invalid input parameter",
"Error unregistering the window class",
"Error getting window title",
"Error getting the window info block",
"Error setting the window text while applying the window info block",
"Error trying to change window size and dimentions",
"Error adjusting the client area for a window",
"Error trying to make window topmost",
"No connection to X server established",
"Error getting WM hints: window size and position",
"No memory for XLib string conversion"
};

class OWindow
{
public:
    FLAGS SetWindowFlags( VISIBILITY_FLAGS, BORDER_FLAGS, WND_BUTTON_FLAGS );

    /* functions that affect window */
    virtual bool Create( const char*, int, int, int, int, FLAGS ) = 0;
    virtual bool Show( void ) = 0;
    virtual bool Hide( void ) = 0;
    virtual bool ShowMinimized( void ) = 0;
    virtual bool ShowMaximized( void ) = 0;
    virtual bool ShowNormal( void ) = 0;
    virtual bool Update( void ) = 0;
    virtual bool Minimize( void ) = 0;
    virtual bool Maximize( void ) = 0;
    virtual bool Restore( void ) = 0;
    virtual bool SetCaption( const char* ) = 0;
    virtual bool GetCaption( const char*, int ) = 0;
    virtual bool SetWindowRect( const PATOMRECT ) = 0;
    virtual bool GetWindowRect( PATOMRECT const ) = 0;
    virtual bool GetClientRect( PATOMRECT const ) = 0;
    virtual bool Close( void ) = 0;
    virtual bool Destroy( void ) = 0;
    virtual bool IsVisible( void ) = 0;
    virtual bool IsMinimized( void ) = 0;
    virtual bool IsMaximized( void ) = 0;
    virtual bool IsFullScreen( void ) = 0;
    virtual bool IsBorderSizeable( void ) = 0;
    virtual bool IsBorderFixed( void ) = 0;
    virtual bool IsTopMost( void ) = 0;
    virtual bool HasMinimizeButton( void ) = 0;
    virtual bool HasMaximizeButton( void ) = 0;
    virtual bool HasExitButton( void ) = 0;
    virtual bool HasHelpButton( void ) = 0;

    /* events handled by the window */
    virtual void OnClose( void );
    virtual void OnCreate( void );
    virtual void OnDestroy( void );
    virtual void OnQuit( void );

protected:
    OWindow( AtomLog& );
	AtomLog& log;
};

#endif
