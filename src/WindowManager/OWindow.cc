#include "OWindow.h"

OWindow::OWindow( AtomLog* Log )
{
	log = Log;
	
	/* Initializing window properties with default values */
	memset( &properties.title[0], 0x0, sizeof( properties.title ) );
	properties.x = properties.y = 0; 
	properties.width = 150;
	properties.height = 150;
	properties.effect = normal;
	properties.border = noborder; 	
}
