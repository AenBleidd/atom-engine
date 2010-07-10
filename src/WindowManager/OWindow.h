#ifndef _OWINDOW_H
#define _OWINDOW_H

class OWindow
{
public:
	OWindow(){};
	virtual ~OWindow(){};
	virtual bool MakeWindow( void ) = 0;
};

bool MakeWindow( void ){}

#endif
