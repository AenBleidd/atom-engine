#ifndef _OWINDOW_H
#define _OWINDOW_H
#include <AtomError.h>

#define OFF 0
#define ON 1

enum VISIBILITY
{
	visible,
	hidden
};

enum EFFECT
{
	normal,
	minimized,
	maximized,
	fullscreen
};

enum BORDER
{
	noborder,
	sizeable,
	fixed
};

struct WNDINFO;

class OWindow
{
public:
    OWindow( AtomLog* );
    struct properties
    {
        char* title;
        int x, y, width, height;
        VISIBILITY visibility;
        EFFECT effect;
        BORDER border;
    } properties;	
    struct buttons
    {
        int minimize : 1,
            maximize : 1,
            exit : 1,
            help : 1;
        } buttons;	
	virtual bool Create( void ) = 0;
    virtual bool Show( void ) = 0;
    virtual bool Hide( void ) = 0;
    virtual bool Minimize( void ) = 0;
    virtual bool Maximize( void ) = 0;
    virtual bool Restore( void ) = 0;
	virtual bool SetCaption( const char* ) = 0;
	virtual bool GetCaption( char const*, int ) = 0;
	virtual bool SetWindowRect( const RECT* ) = 0;
	virtual bool GetWindowRect( RECT const* ) = 0;
	virtual bool GetWindowInfo( WNDINFO* ) = 0;
	virtual bool SetWindowInfo( const WNDINFO* ) = 0;
	virtual bool GetWindowRect( RECT* ) = 0;
	virtual bool GetClientRect( RECT* ) = 0;
	virtual bool Close( void ) = 0;
	virtual bool Destroy( void ) = 0;
protected:
	AtomLog *log;
};

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

#endif
