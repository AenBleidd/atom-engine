#include "OWindow.h"

OWindow::OWindow( AtomLog* Log )
{
    log = Log;
}

FLAGS OWindow::SetWindowFlags( VISIBILITY_FLAGS vflags, BORDER_FLAGS brdflags, WND_BUTTON_FLAGS btnflags )
{
    return vflags | brdflags | btnflags;
}
