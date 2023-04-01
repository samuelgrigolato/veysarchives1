#ifndef CORE_GAME_h
#define CORE_GAME_h

#include "platform.h"
#include <SDL.h>
#include "core/input.h"

typedef struct {
  SDL_Renderer *renderer;
  SDL_Window *window;
  Sint32 windowWidth;
  Sint32 windowHeight;
} Game_Context;

typedef struct {
  void (*init)(Game_Context *ctx);
  void (*handleClickTap)(Game_Context *ctx, Input_ClickTap *pos);
  void (*handleFingerEvent)(Game_Context *ctx, Input_FingerEvent *event);
  void (*handleKeyboardEvent)(Game_Context *ctx, Input_KeyboardEvent *event);
  void (*updateModel)(Uint64 elapsedTime);
  void (*render)(Game_Context *ctx);
  void (*destroy)();
} Game_Screen;

void Game_Init(SDL_Renderer *renderer, SDL_Window *window, Game_Screen *initial);

void Game_GoTo(Game_Screen *next);

void Game_HandleClickTap(Input_ClickTap *pos);

void Game_HandleFingerEvent(Input_FingerEvent *event);

void Game_HandleKeyboardEvent(Input_KeyboardEvent *event);

void Game_UpdateModel(Uint64 elapsedTime);

void Game_Render();

#endif
