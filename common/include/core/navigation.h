#ifndef CORE_NAVIGATION_h
#define CORE_NAVIGATION_h

#include "platform.h"
#include <SDL.h>


struct Nav_ClickTap {
  Sint32 x;
  Sint32 y;
};

struct Nav_Context {
  SDL_Renderer *renderer;
  SDL_Window *window;
  Sint32 windowWidth;
  Sint32 windowHeight;
};

struct Nav_Screen {
  void (*init)(struct Nav_Context *ctx);
  void (*handleClickTap)(struct Nav_Context *ctx, struct Nav_ClickTap *pos);
  void (*render)(struct Nav_Context *ctx);
  void (*destroy)();
};

void Nav_Init(SDL_Renderer *renderer, SDL_Window *window, struct Nav_Screen *initial);

void Nav_GoTo(struct Nav_Screen *next);

void Nav_HandleClickTap(struct Nav_ClickTap *pos);

void Nav_Render();


#endif
