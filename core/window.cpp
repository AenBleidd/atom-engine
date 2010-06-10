#include "window.h"

// Window Constructor
GameWnd::GameWnd(AtomLog *log) {
  atomlog = log;
}
// Window Destructor
GameWnd::~GameWnd() {
}
bool GameWnd::SDLInit(const GAMEWND* param) {
  int flags = SDL_HWSURFACE | SDL_OPENGL | SDL_DOUBLEBUF;
  if (param->mode == FULLSCREEN) flags |= SDL_FULLSCREEN;
  if (SDL_Init(SDL_INIT_VIDEO) < 0) return false;
/* Set the video mode */
  if (!(screen = SDL_SetVideoMode(param->width, param->height,
      param->bpp, flags))) {
     SDL_Quit();
     return false;
  }
/* Paint the background white */
  if (SDL_FillRect(screen, &screen->clip_rect,
      SDL_MapRGB(screen->format, 255, 255, 255))) {
    SDL_Quit();
    return false;
}
/* Apply the changes to the screen */
  if (SDL_Flip(screen)) {
    SDL_Quit();
    return false;
  }
/* Set window title */
  SDL_WM_SetCaption(param->title, param->title);
/* send a video resize event to paint the screen */
  SDL_Event resizeEvent;
  resizeEvent.type = SDL_VIDEORESIZE;
  resizeEvent.resize.w = param->width;
  resizeEvent.resize.h = param->height;
  SDL_PushEvent(&resizeEvent);

  return true;
}
bool GameWnd::GLInit() {
/* Initialize some OpenGL attributes */
  SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
  SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
  SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
  SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
  SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

  return true;
}
bool GameWnd::MakeWindow(const GAMEWND* param) {
// Initialize SDL
  if (!SDLInit(param)) return false;
// Initialize OpenGL
  if (!GLInit()) return false;
/* Process window message queue */
  while (true) {
    while (SDL_PollEvent(&event)) {
      switch (event.type) {
        case SDL_VIDEORESIZE:
          glViewport(0, 0, param->width, param->height);
          glClearColor(1.0f, 1.0f, 0.0f, 1.0f);
          glClear(GL_COLOR_BUFFER_BIT);
          glLoadIdentity();
          SDL_GL_SwapBuffers();
        break;

        case SDL_QUIT:
          SDL_Quit();
          return true;
        break;

        case SDL_KEYDOWN:
          if (event.key.keysym.sym == SDLK_ESCAPE)
            SDL_Quit();
            return true;
        break;

        default:
          break;
      }
    }  // while SQL_PollEvent
  }  // while (1)

  SDL_Quit();
  return true;
}
