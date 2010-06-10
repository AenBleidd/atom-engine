#ifndef _CORE_WINDOW_H_
#define _CORE_WINDOW_H_

#include <SDL/SDL.h>
#include <GL/gl.h>
#include <GL/glu.h>

#include "error.h"

enum MODE {
  FULLSCREEN,
  WINDOWED
};
// Parameters of game window
struct GAMEWND {
  int width;
  int height;
  int bpp;
  const char *title;
  MODE mode;
};

class GameWnd {
 public:
  SDL_Event event;
  SDL_Surface *screen;
  GAMEWND param;  // Parameters of game window

  explicit GameWnd(AtomLog *log);
  ~GameWnd();
  // This Function Create Window
  bool MakeWindow(const GAMEWND* param);
 private:
  AtomLog *atomlog;
  bool SDLInit(const GAMEWND* param);
  bool GLInit(void);
};

#endif  // _CORE_WINDOW_H_
