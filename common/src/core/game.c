#include "core/game.h"
#include "core/positioning.h"


static Game_Screen *current;
static Game_Context ctx;


static void relayout() {
  SDL_GetWindowSize(ctx.window, &ctx.windowWidth, &ctx.windowHeight);
  Pos_Relayout(ctx.windowWidth, ctx.windowHeight);
}


void Game_Init(SDL_Renderer *renderer, SDL_Window *window, Game_Screen *initial) {
  ctx.renderer = renderer;
  ctx.window = window;
  relayout();
  current = initial;
  current->init(&ctx);
}


void Game_HandleClickTap(Input_ClickTap *pos) {
  current->handleClickTap(&ctx, pos);
}


void Game_HandleFingerEvent(Input_FingerEvent *event) {
  current->handleFingerEvent(&ctx, event);
}


void Game_HandleKeyboardEvent(Input_KeyboardEvent *event) {
  current->handleKeyboardEvent(&ctx, event);
}


void Game_GoTo(Game_Screen *next) {
  if (current != NULL) {
    current->destroy();
  }
  current = next;
  current->init(&ctx);
}


void Game_UpdateModel(Uint64 elapsedTime) {
  current->updateModel(elapsedTime);
}


void Game_Render() {
  relayout();
  current->render(&ctx);
  Pos_RenderLetterBox(ctx.renderer);
}
