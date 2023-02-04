#include "core/log.h"
#include "core/positioning.h"


/**
 * If the display aspect ratio falls outside of the allowed interval
 * letter boxing will be applied. This rect is the useful area without
 * the black stripes.
 */
SDL_Rect usefulArea;


/**
 * All the assets are drawn considering a preferred display with 900px
 * of height. This multiplier allows to correctly zoom both the height
 * and width of assets on devices with smaller or denser resolutions.
*/
float zoom;


#define POS_MAX_SUPPORTED_ASPECT_RATIO (2100.0 / 900.0)
#define POS_MIN_SUPPORTED_ASPECT_RATIO (1600.0 / 900.0)
#define POS_BASE_DISPLAY_HEIGHT 900.0
#define POS_BASE_DISPLAY_WIDTH 2100.0


Sint32 zoomed(Sint32 base) {
  return base * zoom;
}


void Pos_Relayout(struct Nav_Context *navCtx) {

  float displayAspectRatio = (float)navCtx->windowWidth / navCtx->windowHeight;

  usefulArea.x = 0;
  usefulArea.y = 0;
  usefulArea.h = navCtx->windowHeight;
  usefulArea.w = navCtx->windowWidth;

  float effectiveAspectRatio;
  if (displayAspectRatio < POS_MIN_SUPPORTED_ASPECT_RATIO) {

    // the display is way more "squarer" than the supported interval
    // so we will apply horizontal stripes at the top and bottom
    // to keep it within limits.

    effectiveAspectRatio = POS_MIN_SUPPORTED_ASPECT_RATIO;

    usefulArea.h = usefulArea.w / effectiveAspectRatio;
    Sint32 stripeHeight = (navCtx->windowHeight - usefulArea.h) / 2.0;

    usefulArea.y = stripeHeight;

  } else if (displayAspectRatio > POS_MAX_SUPPORTED_ASPECT_RATIO) {

    // the display is way more "thinner" than the supported interval
    // so we will apply vertical stripes at the left and right
    // to keep it within limits.

    effectiveAspectRatio = POS_MAX_SUPPORTED_ASPECT_RATIO;

    usefulArea.w = usefulArea.h * effectiveAspectRatio;
    Sint32 stripeWidth = (navCtx->windowWidth - usefulArea.w) / 2.0;

    usefulArea.x = stripeWidth;

  } else {
    effectiveAspectRatio = displayAspectRatio;
  }

  zoom = usefulArea.h / POS_BASE_DISPLAY_HEIGHT;

  // useful for debugging
  // logInfo("Pos_Relayout: usefulArea=%dh %dw %dx %dy zoom=%.2f r=%.2f",
  //   usefulArea.h, usefulArea.w, usefulArea.x, usefulArea.y, zoom, effectiveAspectRatio);
}


SDL_Rect Pos_CalcAnchored(struct Pos_AnchoredElement *el) {
  SDL_Rect result;
  result.w = zoomed(el->width);
  result.h = zoomed(el->height);

  if ((el->anchors & POS_ANCHOR_LEFT) == POS_ANCHOR_LEFT) {
    result.x = usefulArea.x + zoomed(el->anchorLeft);
  } else if ((el->anchors & POS_ANCHOR_RIGHT) == POS_ANCHOR_RIGHT) {
    result.x = usefulArea.x + usefulArea.w - zoomed(el->anchorRight) - result.w;
  } else if ((el->anchors & POS_ANCHOR_CENTER_LEFT) == POS_ANCHOR_CENTER_LEFT) {
    result.x = usefulArea.x + (usefulArea.w / 2.0) + zoomed(el->anchorCenterLeft);
  } else if ((el->anchors & POS_ANCHOR_CENTER_RIGHT) == POS_ANCHOR_CENTER_RIGHT) {
    result.x = usefulArea.x + (usefulArea.w / 2.0) - zoomed(el->anchorCenterRight) - result.w;
  }

  if ((el->anchors & POS_ANCHOR_TOP) == POS_ANCHOR_TOP) {
    result.y = usefulArea.y + zoomed(el->anchorTop);
  } else if ((el->anchors & POS_ANCHOR_BOTTOM) == POS_ANCHOR_BOTTOM) {
    result.y = usefulArea.y + usefulArea.h - zoomed(el->anchorBottom) - result.h;
  }

  // useful for debugging
  // logInfo("Pos_CalcAnchored: %dh %dw %dx %dy",
  //   result.h, result.w, result.x, result.y);
  return result;
}


SDL_Rect Pos_CalcCover() {
  SDL_Rect result;
  result.w = zoomed(POS_BASE_DISPLAY_WIDTH);
  result.h = usefulArea.h;
  result.x = usefulArea.x + (result.w - usefulArea.w) / -2;
  result.y = usefulArea.y;
  // useful for debugging
  // logInfo("Pos_CalcCover: %dh %dw %dx %dy",
  //   result.h, result.w, result.x, result.y);
  return result;
}


SDL_bool Pos_IsInside(SDL_Rect *calculatedRect, struct Nav_ClickTap *pos) {
  return pos->x >= calculatedRect->x &&
    pos->x <= calculatedRect->x + calculatedRect->w &&
    pos->y >= calculatedRect->y &&
    pos->y <= calculatedRect->y + calculatedRect->h;
}


SDL_bool Pos_IsFingerEventInside(SDL_Rect *calculatedRect, struct Nav_FingerEvent *event, struct Nav_Context *ctx) {
  Sint32 x = ctx->windowWidth * event->nx;
  Sint32 y = ctx->windowHeight * event->ny;
  return x >= calculatedRect->x &&
    x <= calculatedRect->x + calculatedRect->w &&
    y >= calculatedRect->y &&
    y <= calculatedRect->y + calculatedRect->h;
}
