#ifndef _WINDOW_H_
#define _WINDOW_H_

enum MODE {
	FULLSCREEN,
	WINDOWED
};

bool MakeWindow(int width, int height, int bpp, const char *title, MODE mode);

#endif
