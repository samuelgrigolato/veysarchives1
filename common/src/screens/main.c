#include "core/log.h"
#include "core/positioning.h"
#include "core/resources.h"
#include "core/audio.h"
#include "screens/main.h"
#include "screens/home.h"


struct Pos_AnchoredElement optionsButtonPos;
struct Pos_AnchoredElement mainCharacterPos;

SDL_Rect optionsButtonRect;
SDL_Rect mainCharacterRect;

SDL_Texture *optionsButtonTexture;
SDL_Texture *mainCharacterTexture;

Aud_SoundID optionsButtonPress;


void Main_Init(struct Nav_Context *ctx) {
  logInfo("Main: initializing.");

  mainCharacterPos.anchors = POS_ANCHOR_TOP | POS_ANCHOR_CENTER_LEFT;
  mainCharacterPos.width = 100;
  mainCharacterPos.height = 100;
  mainCharacterPos.anchorTop = 400;
  mainCharacterPos.anchorCenterLeft = -50;

  optionsButtonPos.anchors = POS_ANCHOR_BOTTOM | POS_ANCHOR_RIGHT;
  optionsButtonPos.width = 100;
  optionsButtonPos.height = 100;
  optionsButtonPos.anchorBottom = 0;
  optionsButtonPos.anchorRight = 0;

  optionsButtonTexture = Res_LoadTexture(ctx, "options-button.png");
  mainCharacterTexture = Res_LoadTexture(ctx, "main-character.png");

  optionsButtonPress = Aud_Load("button-press.wav");
}


void Main_Render(struct Nav_Context *ctx) {

  SDL_Rect mainCharacterPose;
  mainCharacterPose.w = 99;
  mainCharacterPose.h = 99;
  mainCharacterPose.x = 1;
  mainCharacterPose.y = 201;

  mainCharacterRect = Pos_CalcAnchored(&mainCharacterPos);
  if (SDL_RenderCopy(ctx->renderer, mainCharacterTexture, &mainCharacterPose, &mainCharacterRect) != 0) {
    logError("Main: failed to render main character: %s %s", SDL_GetError());
    exit(1);
  }

  optionsButtonRect = Pos_CalcAnchored(&optionsButtonPos);
  if (SDL_RenderCopy(ctx->renderer, optionsButtonTexture, NULL, &optionsButtonRect) != 0) {
    logError("Main: failed to render options button: %s %s", SDL_GetError());
    exit(1);
  }
}


void Main_HandleClickTap(struct Nav_Context *ctx, struct Nav_ClickTap *pos) {
  if (Pos_IsInside(&optionsButtonRect, pos)) {
    Aud_PlayOnce(optionsButtonPress);
    Nav_GoTo(Home_GetScreen());
  }
}


void Main_Destroy() {
  logInfo("Main: destroying.");
  Res_ReleaseTexture(optionsButtonTexture);
  Res_ReleaseTexture(mainCharacterTexture);
  Aud_Unload(optionsButtonPress);
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
