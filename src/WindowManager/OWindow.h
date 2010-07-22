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
protected:
	AtomLog *log;
};

#endif
