#include "core/log.h"
#include "core/positioning.h"


struct Pos_State {
  float zoom;
  float letterBoxTop;
  float letterBoxLeft;
};


struct Pos_State state;


void Pos_Relayout(struct Nav_Context *navCtx) {
  float windowRatio = (float)navCtx->windowWidth / navCtx->windowHeight;
  float windowRatioToBaseRatio = windowRatio / POS_DRAWABLE_SCREEN_RATIO;
  Sint32 usefulHeight;
  Sint32 usefulWidth;
  float zoom;
  if (windowRatioToBaseRatio > 1) {
    usefulHeight = navCtx->windowHeight;
    usefulWidth = POS_DRAWABLE_SCREEN_WIDTH * (navCtx->windowHeight / POS_DRAWABLE_SCREEN_HEIGHT);
  } else {
    usefulHeight = POS_DRAWABLE_SCREEN_HEIGHT * (navCtx->windowWidth / POS_DRAWABLE_SCREEN_WIDTH);
    usefulWidth = navCtx->windowWidth;
  }
  state.zoom = usefulHeight / POS_DRAWABLE_SCREEN_HEIGHT;
  state.letterBoxLeft = (navCtx->windowWidth - usefulWidth) / 2.0;
  state.letterBoxTop = (navCtx->windowHeight - usefulHeight) / 2.0;

  // useful for debugging
  //logInfo("Pos_Relayout: wrbr=%f z=%f lbl=%f lbt=%f uh=%d uw=%d", windowRatioToBaseRatio, state.zoom, state.letterBoxLeft, state.letterBoxTop, usefulHeight, usefulWidth);
}


void Pos_Apply(struct SDL_Rect *base) {
  base->h = base->h * state.zoom;
  base->w = base->w * state.zoom;
  base->y = base->y * state.zoom + state.letterBoxTop;
  base->x = base->x * state.zoom + state.letterBoxLeft;
}
