#include "core/navigation.h"


struct Nav_Screen *current;
struct Nav_Context ctx;


void Nav_Init(SDL_Renderer *renderer, SDL_Window *window, struct Nav_Screen *initial) {
  ctx.renderer = renderer;
  ctx.window = window;
  current = initial;
  current->init();
}


void Nav_HandleClickTap(struct Nav_ClickTap *pos) {
  current->handleClickTap(&ctx, pos);
}


void Nav_GoTo(struct Nav_Screen *next) {
  if (current != NULL) {
    current->destroy();
  }
  current = next;
  current->init();
}


void Nav_Render() {
  SDL_GetWindowSize(ctx.window, &ctx.windowWidth, &ctx.windowHeight);
  current->render(&ctx);
}
