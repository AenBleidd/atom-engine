#ifndef _WINDOW_H_
#define _WINDOW_H_

enum MODE {
	FULLSCREEN,
	WINDOWED
};

typedef struct ERR
{
      int code;
      int sub_code;
      int *func;
      const char *description;
}ERROR;

int GetLastErr (ERROR err);

bool MakeWindow(int width, int height, int bpp, const char *title, MODE mode);


#endif
