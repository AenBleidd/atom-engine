#include "OWindow.h"

OWindow::OWindow( AtomLog* Log )
{
    log = Log;

    /* Initializing window properties with default values */
    properties.x = properties.y = 0; 
    properties.width = 150;
    properties.height = 150;
    properties.effect = normal;
    properties.border = noborder;
	buttons.minimize = OFF;
	buttons.maximize = OFF;
	buttons.exit = OFF;
	buttons.help = OFF;
}
