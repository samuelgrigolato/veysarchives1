#include "core/navigation.h"
#include "core/positioning.h"


struct Nav_Screen *current;
struct Nav_Context ctx;


void relayout() {
  SDL_GetWindowSize(ctx.window, &ctx.windowWidth, &ctx.windowHeight);
  Pos_Relayout(&ctx);
}


void Nav_Init(SDL_Renderer *renderer, SDL_Window *window, struct Nav_Screen *initial) {
  ctx.renderer = renderer;
  ctx.window = window;
  relayout();
  current = initial;
  current->init(&ctx);
}


void Nav_HandleClickTap(struct Nav_ClickTap *pos) {
  current->handleClickTap(&ctx, pos);
}


void Nav_HandleFingerEvent(struct Nav_FingerEvent *event) {
  current->handleFingerEvent(&ctx, event);
}


void Nav_GoTo(struct Nav_Screen *next) {
  if (current != NULL) {
    current->destroy();
  }
  current = next;
  current->init(&ctx);
}


void Nav_Render() {
  relayout();
  current->render(&ctx);
}
