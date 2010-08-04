#ifndef EVENT_HANDLER
#define EVENT_HANDLER
#include <OEventHandler.h>
//#include <windows.h>
#include "WindowManager.h"

class EventHandler : public OEventHandler
{
public:
    EventHandler();
    ~EventHandler();
    void Run( void );
    static LRESULT CALLBACK WindowProcedure( HWND, UINT, WPARAM, LPARAM ); // callback event handler
private:
    MSG msg;
};

#endif
