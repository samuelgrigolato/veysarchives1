#include "core/log.h"
#include "core/resources.h"
#include "core/audio.h"
#include "core/positioning.h"
#include "screens/home.h"
#include "screens/world/components/options-button.h"


static Pos_AnchoredElement pos;
static SDL_Rect rect;
static SDL_Texture *texture;
static Aud_SoundID press;


void World_OptionsButton_Init(Nav_Context *ctx) {
  pos.anchors = POS_ANCHOR_BOTTOM | POS_ANCHOR_RIGHT;
  pos.width = 100;
  pos.height = 100;
  pos.anchorBottom = 0;
  pos.anchorRight = 0;
  texture = Res_LoadTexture(ctx, "options-button.png");
  press = Aud_Load("button-press.wav");
}


void World_OptionsButton_Render(Nav_Context *ctx) {
  rect = Pos_CalcAnchored(&pos);
  if (SDL_RenderCopy(ctx->renderer, texture, NULL, &rect) != 0) {
    logError("OptionsButton: failed to render: %s %s", SDL_GetError());
    exit(1);
  }
}


void World_OptionsButton_HandleClickTap(Nav_Context *ctx, Nav_ClickTap *pos) {
  if (Pos_IsInside(&rect, pos)) {
    Aud_PlayOnce(press);
    Nav_GoTo(Home_GetScreen());
  }
}


void World_OptionsButton_Destroy() {
  logInfo("OptionsButton: destroying.");
  Res_ReleaseTexture(texture);
  Aud_Unload(press);
}
