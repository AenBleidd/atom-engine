#ifndef _OWINDOW_H
#define _OWINDOW_H
#include <AtomError.h>

// To compile in UNIX
#ifndef UNIX
#include <WM_Types.h>
#endif  // UNIX
#ifdef UNIX
typedef struct _RECT {
  long left;
  long top;
  long right;
  long bottom;
} RECT, *PRECT, ATOMRECT, *PATOMRECT;
struct WNDINFO
{
        char const* title[0xFF];
        int x, y, width, height,
            client_x, client_y,
                client_width, client_height;
        int IsVisible : 1,
            IsMinimized : 1,
                IsMaximized : 1,
                IsFullScreen : 1,
            IsBorderSizeable : 1,
                IsBorderFixed : 1,
                IsTopMost : 1,
                HasMinimizeButton : 1,
                HasMaximizeButton : 1,
                HasExitButton : 1,
                HasHelpButton : 1;
};
#endif  // UNIX

typedef unsigned int FLAGS;

enum VISIBILITY_FLAGS
{
    ATOMWND_VISIBLE               = 0x00000001
};

enum BORDER_FLAGS
{
    ATOMWND_BORDER_SIZEABLE       = 0x00000002,
    ATOMWND_BORDER_FIXED          = 0x00000004
};

enum WND_BUTTON_FLAGS
{
    ATOMWND_BTN_MINIMIZE          = 0x00000008,
    ATOMWND_BTN_MAXIMIZE          = 0x00000010,
    ATOMWND_BTN_EXIT              = 0x00000020,
    ATOMWND_BTN_HELP              = 0x00000040
};

class OWindow
{
public:
    OWindow( AtomLog* );
    FLAGS SetWindowFlags( VISIBILITY_FLAGS, BORDER_FLAGS, WND_BUTTON_FLAGS );
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
protected:
	AtomLog *log;
};

#endif
