#ifndef CORE_NAVIGATION_h
#define CORE_NAVIGATION_h

#include "platform.h"
#include <SDL.h>


typedef struct {
  Sint32 x;
  Sint32 y;
} Nav_ClickTap;

typedef enum {
  NAV_FINGER_EVENT_TYPE_DOWN = 1,
  NAV_FINGER_EVENT_TYPE_MOTION = 2,
  NAV_FINGER_EVENT_TYPE_UP = 3
} Nav_FingerEventType;

typedef struct {
  Nav_FingerEventType type;
  float nx; // 0..1
  float ny; // 0..1
} Nav_FingerEvent;

typedef enum {
  NAV_KEYBOARD_EVENT_TYPE_DOWN = 1,
  NAV_KEYBOARD_EVENT_TYPE_UP = 2
} Nav_KeyboardEventType;

typedef struct {
  Nav_KeyboardEventType type;
  SDL_Keycode key;
} Nav_KeyboardEvent;

typedef struct {
  SDL_Renderer *renderer;
  SDL_Window *window;
  Sint32 windowWidth;
  Sint32 windowHeight;
} Nav_Context;

typedef struct {
  void (*init)(Nav_Context *ctx);
  void (*handleClickTap)(Nav_Context *ctx, Nav_ClickTap *pos);
  void (*handleFingerEvent)(Nav_Context *ctx, Nav_FingerEvent *event);
  void (*handleKeyboardEvent)(Nav_Context *ctx, Nav_KeyboardEvent *event);
  void (*updateModel)(Uint64 elapsedTime);
  void (*render)(Nav_Context *ctx);
  void (*destroy)();
} Nav_Screen;

void Nav_Init(SDL_Renderer *renderer, SDL_Window *window, Nav_Screen *initial);

void Nav_GoTo(Nav_Screen *next);

void Nav_HandleClickTap(Nav_ClickTap *pos);

void Nav_HandleFingerEvent(Nav_FingerEvent *event);

void Nav_HandleKeyboardEvent(Nav_KeyboardEvent *event);

void Nav_UpdateModel(Uint64 elapsedTime);

void Nav_Render();


#endif
