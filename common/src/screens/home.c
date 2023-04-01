#include "platform.h"
#include <SDL.h>
#include <SDL_image.h>
#include "core/log.h"
#include "core/navigation.h"
#include "core/positioning.h"
#include "core/resources.h"
#include "core/audio.h"
#include "screens/home.h"
#include "screens/world/world.h"


static Pos_AnchoredElement newGameButtonPos;
static SDL_Rect newGameButtonRect;
static SDL_Texture *newGameButtonTexture;

static Pos_AnchoredElement continueButtonPos;
static SDL_Rect continueButtonRect;
static SDL_Texture *continueButtonTexture;

static SDL_Texture *backgroundTexture;
static Aud_SoundID backgroundMusic;
static Aud_EntryID backgroundMusicEntry;

static Aud_SoundID buttonPress;


void Home_Init(Nav_Context *ctx) {
  logInfo("Home: initializing.");

  newGameButtonPos.anchors = POS_ANCHOR_BOTTOM | POS_ANCHOR_CENTER_RIGHT;
  newGameButtonPos.width = 400;
  newGameButtonPos.height = 100;
  newGameButtonPos.anchorBottom = 90;
  newGameButtonPos.anchorCenterRight = 110;
  newGameButtonTexture = Res_LoadTexture(ctx, "new-game-button.png");

  continueButtonPos.anchors = POS_ANCHOR_BOTTOM | POS_ANCHOR_CENTER_LEFT;
  continueButtonPos.width = 400;
  continueButtonPos.height = 100;
  continueButtonPos.anchorBottom = 90;
  continueButtonPos.anchorCenterLeft = 110;
  continueButtonTexture = Res_LoadTexture(ctx, "continue-button.png");

  backgroundTexture = Res_LoadTexture(ctx, "home-background.png");

  backgroundMusic = Aud_Load("home-background.wav");
  backgroundMusicEntry = Aud_FadeInAndRepeat(backgroundMusic);

  buttonPress = Aud_Load("button-press.wav");
}


void Home_UpdateModel(Uint64 elapsedTime) {
}


void Home_Render(Nav_Context *ctx) {

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


void Home_HandleClickTap(Nav_Context *ctx, Input_ClickTap *pos) {
  if (Pos_IsInside(&newGameButtonRect, pos) || Pos_IsInside(&continueButtonRect, pos)) {
    Aud_PlayOnce(buttonPress);
    Aud_FadeOutAndStop(backgroundMusicEntry);
    Nav_GoTo(World_GetScreen());
  }
}


void Home_HandleFingerEvent(Nav_Context *ctx, Input_FingerEvent *event) {
}


void Home_HandleKeyboardEvent(Nav_Context *ctx, Input_KeyboardEvent *event) {
}


void Home_Destroy() {
  logInfo("Home: destroying.");
  Res_ReleaseTexture(backgroundTexture);
  Res_ReleaseTexture(newGameButtonTexture);
  Res_ReleaseTexture(continueButtonTexture);
  Aud_Unload(backgroundMusic);
  Aud_Unload(buttonPress);
}


Nav_Screen* Home_GetScreen() {
  static Nav_Screen *instance = NULL;
  if (instance == NULL) {
    instance = malloc(sizeof(Nav_Screen));
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
