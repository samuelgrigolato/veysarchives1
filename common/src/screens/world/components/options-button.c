#include "core/log.h"
#include "core/resources.h"
#include "core/audio.h"
#include "core/positioning.h"
#include "screens/home.h"
#include "screens/world/components/options-button.h"


static struct Pos_AnchoredElement optionsButtonPos;
static SDL_Rect optionsButtonRect;
static SDL_Texture *optionsButtonTexture;
static Aud_SoundID optionsButtonPress;


void World_OptionsButton_Init(struct Nav_Context *ctx) {
  optionsButtonPos.anchors = POS_ANCHOR_BOTTOM | POS_ANCHOR_RIGHT;
  optionsButtonPos.width = 100;
  optionsButtonPos.height = 100;
  optionsButtonPos.anchorBottom = 0;
  optionsButtonPos.anchorRight = 0;
  optionsButtonTexture = Res_LoadTexture(ctx, "options-button.png");
  optionsButtonPress = Aud_Load("button-press.wav");
}


void World_OptionsButton_Render(struct Nav_Context *ctx) {
  optionsButtonRect = Pos_CalcAnchored(&optionsButtonPos);
  if (SDL_RenderCopy(ctx->renderer, optionsButtonTexture, NULL, &optionsButtonRect) != 0) {
    logError("OptionsButton: failed to render: %s %s", SDL_GetError());
    exit(1);
  }
}


void World_OptionsButton_HandleClickTap(struct Nav_Context *ctx, struct Nav_ClickTap *pos) {
  if (Pos_IsInside(&optionsButtonRect, pos)) {
    Aud_PlayOnce(optionsButtonPress);
    Nav_GoTo(Home_GetScreen());
  }
}


void World_OptionsButton_Destroy() {
  logInfo("OptionsButton: destroying.");
  Res_ReleaseTexture(optionsButtonTexture);
  Aud_Unload(optionsButtonPress);
}
