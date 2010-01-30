#ifndef UNIX
#include <windows.h>
#endif

#include "../preproc.h"

#include "window.h"
#include "error.h"

AtomLog atomlog;

#ifndef UNIX
int WINAPI WinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
    LPSTR lpCmdLine,
    int nCmdShow
)
#else
int main(int argc, char **argv)
#endif
{
// Set window parameters
  GameWnd wnd;
  wnd.param.width = 800;
  wnd.param.height = 600;
  wnd.param.bpp = 32;
  wnd.param.title = "Atom Engine";
  wnd.param.mode = WINDOWED;
// Create window
  wnd.MakeWindow(&wnd.param);
  return 0;
}
