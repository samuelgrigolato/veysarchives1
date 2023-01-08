#include "core/log.h"
#include "screens/main.h"
#include "screens/home.h"


void Main_Init() {
  logInfo("Main: initializing.");
}


void Main_Render(struct Nav_Context *ctx) {

  SDL_Rect player;
  player.w = ctx->windowWidth * 0.1;
  player.h = ctx->windowHeight * 0.1;
  player.x = ctx->windowWidth * 0.45;
  player.y = ctx->windowHeight * 0.45;

  SDL_SetRenderDrawColor(ctx->renderer, 0, 255, 255, SDL_ALPHA_OPAQUE);
  SDL_RenderFillRect(ctx->renderer, &player);

}


void Main_HandleClickTap(struct Nav_Context *ctx, struct Nav_ClickTap *pos) {
  Nav_GoTo(Home_GetScreen());
}


void Main_Destroy() {
  logInfo("Main: destroying.");
}


struct Nav_Screen* Main_GetScreen() {
  static struct Nav_Screen *instance = NULL;
  if (instance == NULL) {
    instance = malloc(sizeof(struct Nav_Screen));
    instance->init = Main_Init;
    instance->render = Main_Render;
    instance->handleClickTap = Main_HandleClickTap;
    instance->destroy = Main_Destroy;
  }
  return instance;
}
