#include "platform.h"
#include <SDL.h>
#include <SDL_image.h>
#include "core/log.h"
#include "core/positioning.h"
#include "core/resources.h"
#include "core/audio.h"
#include "screens/home.h"
#include "screens/main.h"


struct Pos_AnchoredElement newGameButtonPos;
struct Pos_AnchoredElement continueButtonPos;

SDL_Rect newGameButtonRect;
SDL_Rect continueButtonRect;

SDL_Texture *backgroundTexture;
SDL_Texture *newGameButtonTexture;
SDL_Texture *continueButtonTexture;

Aud_SoundID backgroundMusic;
Aud_SoundID buttonPress;
Aud_EntryID backgroundMusicEntry;


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

  backgroundTexture = Res_LoadTexture(ctx, "home-background.png");
  newGameButtonTexture = Res_LoadTexture(ctx, "new-game-button.png");
  continueButtonTexture = Res_LoadTexture(ctx, "continue-button.png");

  backgroundMusic = Aud_Load("home-background.wav");
  backgroundMusicEntry = Aud_FadeInAndRepeat(backgroundMusic);

  buttonPress = Aud_Load("button-press.wav");
}


void Home_UpdateModel(Uint64 elapsedTime) {
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
    Aud_PlayOnce(buttonPress);
    Aud_FadeOutAndStop(backgroundMusicEntry);
    Nav_GoTo(Main_GetScreen());
  }
}


void Home_HandleFingerEvent(struct Nav_Context *ctx, struct Nav_FingerEvent *event) {
}


void Home_HandleKeyboardEvent(struct Nav_Context *ctx, struct Nav_KeyboardEvent *event) {
}


void Home_Destroy() {
  logInfo("Home: destroying.");
  Res_ReleaseTexture(backgroundTexture);
  Res_ReleaseTexture(newGameButtonTexture);
  Res_ReleaseTexture(continueButtonTexture);
  Aud_Unload(backgroundMusic);
  Aud_Unload(buttonPress);
}


struct Nav_Screen* Home_GetScreen() {
  static struct Nav_Screen *instance = NULL;
  if (instance == NULL) {
    instance = malloc(sizeof(struct Nav_Screen));
    instance->init = Home_Init;
    instance->updateModel = Home_UpdateModel;
    instance->render = Home_Render;
    instance->handleClickTap = Home_HandleClickTap;
    instance->handleFingerEvent = Home_HandleFingerEvent;
    instance->handleKeyboardEvent = Home_HandleKeyboardEvent;
    instance->destroy = Home_Destroy;
  }
  return instance;
}
