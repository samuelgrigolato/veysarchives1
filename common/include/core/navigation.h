#ifndef CORE_NAVIGATION_h
#define CORE_NAVIGATION_h

#include "platform.h"
#include <SDL.h>
#include "core/input.h"

typedef struct {
  SDL_Renderer *renderer;
  SDL_Window *window;
  Sint32 windowWidth;
  Sint32 windowHeight;
} Nav_Context;

typedef struct {
  void (*init)(Nav_Context *ctx);
  void (*handleClickTap)(Nav_Context *ctx, Input_ClickTap *pos);
  void (*handleFingerEvent)(Nav_Context *ctx, Input_FingerEvent *event);
  void (*handleKeyboardEvent)(Nav_Context *ctx, Input_KeyboardEvent *event);
  void (*updateModel)(Uint64 elapsedTime);
  void (*render)(Nav_Context *ctx);
  void (*destroy)();
} Nav_Screen;

void Nav_Init(SDL_Renderer *renderer, SDL_Window *window, Nav_Screen *initial);

void Nav_GoTo(Nav_Screen *next);

void Nav_HandleClickTap(Input_ClickTap *pos);

void Nav_HandleFingerEvent(Input_FingerEvent *event);

void Nav_HandleKeyboardEvent(Input_KeyboardEvent *event);

void Nav_UpdateModel(Uint64 elapsedTime);

void Nav_Render();

#endif
