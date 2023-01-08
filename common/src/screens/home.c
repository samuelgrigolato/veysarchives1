#include "platform.h"
#include <SDL.h>
#include "core/log.h"
#include "screens/home.h"
#include "screens/main.h"


void Home_Init() {
  logInfo("Home: initializing.");
}


void Home_Render(struct Nav_Context *ctx) {

  SDL_Rect logo;
  logo.w = ctx->windowWidth * 0.8;
  logo.h = ctx->windowHeight * 0.5;
  logo.x = ctx->windowWidth * 0.1;
  logo.y = ctx->windowHeight * 0.15;

  SDL_SetRenderDrawColor(ctx->renderer, 0, 0, 255, SDL_ALPHA_OPAQUE);
  SDL_RenderFillRect(ctx->renderer, &logo);

  SDL_Rect newGameButton;
  newGameButton.w = ctx->windowWidth * 0.25;
  newGameButton.h = ctx->windowHeight * 0.1;
  newGameButton.x = ctx->windowWidth * 0.2;
  newGameButton.y = ctx->windowHeight * 0.75;

  SDL_SetRenderDrawColor(ctx->renderer, 0, 255, 0, SDL_ALPHA_OPAQUE);
  SDL_RenderFillRect(ctx->renderer, &newGameButton);

  SDL_Rect continueButton;
  continueButton.w = ctx->windowWidth * 0.25;
  continueButton.h = ctx->windowHeight * 0.1;
  continueButton.x = ctx->windowWidth * 0.55;
  continueButton.y = ctx->windowHeight * 0.75;

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
