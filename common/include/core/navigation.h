#ifndef CORE_NAVIGATION_h
#define CORE_NAVIGATION_h

#include "platform.h"
#include <SDL.h>


struct Nav_ClickTap {
  Sint32 x;
  Sint32 y;
};

typedef enum {
  NAV_FINGER_EVENT_TYPE_DOWN = 1,
  NAV_FINGER_EVENT_TYPE_MOTION = 2,
  NAV_FINGER_EVENT_TYPE_UP = 3
} Nav_FingerEventType;

struct Nav_FingerEvent {
  Nav_FingerEventType type;
  float nx; // 0..1
  float ny; // 0..1
};

typedef enum {
  NAV_KEYBOARD_EVENT_TYPE_DOWN = 1,
  NAV_KEYBOARD_EVENT_TYPE_UP = 2
} Nav_KeyboardEventType;

struct Nav_KeyboardEvent {
  Nav_KeyboardEventType type;
  SDL_Keycode key;
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
  void (*handleKeyboardEvent)(struct Nav_Context *ctx, struct Nav_KeyboardEvent *event);
  void (*updateModel)(Uint64 elapsedTime);
  void (*render)(struct Nav_Context *ctx);
  void (*destroy)();
};

void Nav_Init(SDL_Renderer *renderer, SDL_Window *window, struct Nav_Screen *initial);

void Nav_GoTo(struct Nav_Screen *next);

void Nav_HandleClickTap(struct Nav_ClickTap *pos);

void Nav_HandleFingerEvent(struct Nav_FingerEvent *event);

void Nav_HandleKeyboardEvent(struct Nav_KeyboardEvent *event);

void Nav_UpdateModel(Uint64 elapsedTime);

void Nav_Render();


#endif
