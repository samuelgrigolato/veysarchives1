#include "platform.h"
#include <SDL.h>
#include <SDL_image.h>
#include "core/log.h"
#include "core/positioning.h"
#include "screens/home.h"
#include "screens/main.h"


struct Pos_AnchoredElement newGameButtonPos;
struct Pos_AnchoredElement continueButtonPos;

SDL_Rect newGameButtonRect;
SDL_Rect continueButtonRect;

SDL_Texture *backgroundTexture;
SDL_Texture *newGameButtonTexture;
SDL_Texture *continueButtonTexture;


void Home_Init(struct Nav_Context *ctx) {
  logInfo("Home: initializing.");

  newGameButtonPos.anchors = POS_ANCHOR_BOTTOM | POS_ANCHOR_CENTER_RIGHT;
  newGameButtonPos.width = 400;
  newGameButtonPos.height = 100;
  newGameButtonPos.anchorBottom = 200;
  newGameButtonPos.anchorCenterRight = 100;

  continueButtonPos.anchors = POS_ANCHOR_BOTTOM | POS_ANCHOR_CENTER_LEFT;
  continueButtonPos.width = 400;
  continueButtonPos.height = 100;
  continueButtonPos.anchorBottom = 200;
  continueButtonPos.anchorCenterLeft = 100;

  char fullImagePath[200];

  sprintf(fullImagePath, "%s%s", "", "assets/home-background.png");
  backgroundTexture = IMG_LoadTexture(ctx->renderer, fullImagePath);
  if (backgroundTexture == NULL) {
    logError("Home: failed to load background texture: %s %s", SDL_GetError());
    exit(1);
  }

  sprintf(fullImagePath, "%s%s", "", "assets/new-game-button.png");
  newGameButtonTexture = IMG_LoadTexture(ctx->renderer, fullImagePath);
  if (newGameButtonTexture == NULL) {
    logError("Home: failed to load new game button texture: %s %s", SDL_GetError());
    exit(1);
  }

  sprintf(fullImagePath, "%s%s", "", "assets/continue-button.png");
  continueButtonTexture = IMG_LoadTexture(ctx->renderer, fullImagePath);
  if (continueButtonTexture == NULL) {
    logError("Home: failed to load continue button texture: %s %s", SDL_GetError());
    exit(1);
  }
}


void Home_Render(struct Nav_Context *ctx) {

  SDL_Rect background = Pos_CalcCover();
  if (SDL_RenderCopy(ctx->renderer, backgroundTexture, NULL, &background) != 0) {
    logError("Home: failed to render background: %s %s", SDL_GetError());
    exit(1);
  }

  newGameButtonRect = Pos_CalcAnchored(&newGameButtonPos);
  if (SDL_RenderCopy(ctx->renderer, newGameButtonTexture, NULL, &newGameButtonRect) != 0) {
    logError("Home: failed to render new game button: %s %s", SDL_GetError());
    exit(1);
  }

  continueButtonRect = Pos_CalcAnchored(&continueButtonPos);
  if (SDL_RenderCopy(ctx->renderer, continueButtonTexture, NULL, &continueButtonRect) != 0) {
    logError("Home: failed to render continue button: %s %s", SDL_GetError());
    exit(1);
  }

}


void Home_HandleClickTap(struct Nav_Context *ctx, struct Nav_ClickTap *pos) {
  if (Pos_IsInside(&newGameButtonRect, pos) || Pos_IsInside(&continueButtonRect, pos)) {
    Nav_GoTo(Main_GetScreen());
  }
}


void Home_Destroy() {
  logInfo("Home: destroying.");
  SDL_DestroyTexture(backgroundTexture);
  SDL_DestroyTexture(newGameButtonTexture);
  SDL_DestroyTexture(continueButtonTexture);
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
