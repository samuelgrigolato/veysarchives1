#include "platform.h"
#include <SDL.h>
#include "core/log.h"
#include "core/positioning.h"
#include "screens/home.h"
#include "screens/main.h"


void Home_Init() {
  logInfo("Home: initializing.");
}


void Home_Render(struct Nav_Context *ctx) {

  SDL_Rect background;
  background.w = 1600;
  background.h = 800;
  background.x = 0;
  background.y = 0;
  Pos_Apply(&background);

  SDL_SetRenderDrawColor(ctx->renderer, 0, 0, 255, SDL_ALPHA_OPAQUE);
  SDL_RenderFillRect(ctx->renderer, &background);

  SDL_Rect newGameButton;
  newGameButton.w = 350;
  newGameButton.h = 100;
  newGameButton.x = 400;
  newGameButton.y = 500;
  Pos_Apply(&newGameButton);

  SDL_SetRenderDrawColor(ctx->renderer, 0, 255, 0, SDL_ALPHA_OPAQUE);
  SDL_RenderFillRect(ctx->renderer, &newGameButton);

  SDL_Rect continueButton;
  continueButton.w = 350;
  continueButton.h = 100;
  continueButton.x = 850;
  continueButton.y = 500;
  Pos_Apply(&continueButton);

  SDL_SetRenderDrawColor(ctx->renderer, 255, 0, 0, SDL_ALPHA_OPAQUE);
  SDL_RenderFillRect(ctx->renderer, &continueButton);

}


void Home_HandleClickTap(struct Nav_Context *ctx, struct Nav_ClickTap *pos) {
  float xRatio = pos->x / (float)ctx->windowWidth;
  float yRatio = pos->y / (float)ctx->windowHeight;
  logInfo("xRatio=%f yRatio=%f", xRatio, yRatio);
  if (xRatio <= 0.45 && xRatio >= 0.2 && yRatio <= 0.85 && yRatio >= 0.75) {
    Nav_GoTo(Main_GetScreen());
  }
}


void Home_Destroy() {
  logInfo("Home: destroying.");
}


struct Nav_Screen* Home_GetScreen() {
  static struct Nav_Screen *instance = NULL;
  if (instance == NULL) {
    instance = malloc(sizeof(struct Nav_Screen));
    instance->init = Home_Init;
    instance->render = Home_Render;
    instance->handleClickTap = Home_HandleClickTap;
    instance->destroy = Home_Destroy;
  }
  return instance;
}
