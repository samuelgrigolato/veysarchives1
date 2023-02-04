#ifndef CORE_NAVIGATION_h
#define CORE_NAVIGATION_h

#include "platform.h"
#include <SDL.h>


struct Nav_ClickTap {
  Sint32 x;
  Sint32 y;
};

#define NAV_FINGER_EVENT_DOWN 1
#define NAV_FINGER_EVENT_MOTION 2
#define NAV_FINGER_EVENT_UP 3

struct Nav_FingerEvent {
  Uint8 type; // NAV_FINGER_EVENT_*
  float nx; // 0..1
  float ny; // 0..1
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
  void (*handleFingerEvent)(struct Nav_Context *ctx, struct Nav_FingerEvent *event);
  void (*render)(struct Nav_Context *ctx);
  void (*destroy)();
};

void Nav_Init(SDL_Renderer *renderer, SDL_Window *window, struct Nav_Screen *initial);

void Nav_GoTo(struct Nav_Screen *next);

void Nav_HandleClickTap(struct Nav_ClickTap *pos);

void Nav_HandleFingerEvent(struct Nav_FingerEvent *event);

void Nav_Render();


#endif
