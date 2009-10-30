#ifndef UNIX
#define UNIX
#endif
//#undef UNIX
#ifndef UNIX
#include <windows.h>
#endif

#include "window.h"

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
	MakeWindow(800,600,32,"Atom Engine",WINDOWED);
	return 0;
}
int GetLastErr (ERROR err) {
  return err.code;
}


