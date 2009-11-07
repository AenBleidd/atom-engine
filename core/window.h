#ifndef _WINDOW_H_
#define _WINDOW_H_

#include <SDL/SDL.h>
#include <GL/gl.h>
#include <GL/glu.h>

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

// Parameters of game window
struct GAMEWND {
  int width;
  int height;
  int bpp;
  const char *title;
  MODE mode;
};

int GetLastErr (ERROR err);

class GameWnd {
  public:
  SDL_Event event;
  SDL_Surface *screen;
  GAMEWND param; // Parameters of game window

  GameWnd();
  ~GameWnd();
  // This Function Create Window
  bool MakeWindow(const GAMEWND* param);
  private:
  bool SDLInit(const GAMEWND* param);
  bool GLInit (void);
};

#endif
