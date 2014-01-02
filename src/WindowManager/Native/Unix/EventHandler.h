#ifndef EVENT_HANDLER
#define EVENT_HANDLER
#include <OEventHandler.h>
#include "WindowManager.h"

class EventHandler : public OEventHandler
{
public:
    EventHandler();
    ~EventHandler();
    void Run( void );
};

#endif
