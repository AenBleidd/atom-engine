#ifndef _OWINDOW_H
#define _OWINDOW_H
#include <AtomError.h>

class OWindow
{
public:
	OWindow( AtomLog* );
	virtual bool MakeWindow( void ) = 0;
	virtual void ResizeWindow( int, int ) = 0;
protected:
	AtomLog *log;
};

#endif
