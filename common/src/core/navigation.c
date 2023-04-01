#include "core/navigation.h"
#include "core/positioning.h"


static Nav_Screen *current;
static Nav_Context ctx;


static void relayout() {
  SDL_GetWindowSize(ctx.window, &ctx.windowWidth, &ctx.windowHeight);
  Pos_Relayout(&ctx);
}


void Nav_Init(SDL_Renderer *renderer, SDL_Window *window, Nav_Screen *initial) {
  ctx.renderer = renderer;
  ctx.window = window;
  relayout();
  current = initial;
  current->init(&ctx);
}


void Nav_HandleClickTap(Nav_ClickTap *pos) {
  current->handleClickTap(&ctx, pos);
}


void Nav_HandleFingerEvent(Nav_FingerEvent *event) {
  current->handleFingerEvent(&ctx, event);
}


void Nav_HandleKeyboardEvent(Nav_KeyboardEvent *event) {
  current->handleKeyboardEvent(&ctx, event);
}


void Nav_GoTo(Nav_Screen *next) {
  if (current != NULL) {
    current->destroy();
  }
  current = next;
  current->init(&ctx);
}


void Nav_UpdateModel(Uint64 elapsedTime) {
  current->updateModel(elapsedTime);
}


void Nav_Render() {
  relayout();
  current->render(&ctx);
  Pos_RenderLetterBox(&ctx);
}
