#ifndef _OWINDOW_H
#define _OWINDOW_H
#include <AtomError.h>

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
		unsigned char title[0xFF];
		int x, y, width, height;
		VISIBILITY visibility;
		EFFECT effect;
		BORDER border;
	} properties;	
	virtual bool Create( void ) = 0;
protected:
	AtomLog *log;
};

#endif
