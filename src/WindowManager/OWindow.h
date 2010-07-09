#ifndef _OWINDOW_H
#define _OWINDOW_H

class OWindow
{
public:
	virtual ~OWindow();
	virtual bool MakeWindow( void ) = 0;
};

#endif
