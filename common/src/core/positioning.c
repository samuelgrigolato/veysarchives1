#include "core/log.h"
#include "core/positioning.h"


/**
 * If the display aspect ratio falls outside of the allowed interval
 * letter boxing will be applied. This rect is the useful area without
 * the black stripes.
 */
static SDL_Rect usefulArea;


/**
 * All the assets are drawn considering a preferred display with 900px
 * of height. This multiplier allows to correctly zoom both the height
 * and width of assets on devices with smaller or denser resolutions.
*/
static float zoom;

static Sint32 windowWidth;
static Sint32 windowHeight;


#define POS_MAX_SUPPORTED_ASPECT_RATIO (2100.0 / 900.0)
#define POS_MIN_SUPPORTED_ASPECT_RATIO (1600.0 / 900.0)


static float zoomed(Sint32 base) {
  return base * zoom;
}


void Pos_Relayout(Sint32 freshWindowWidth, Sint32 freshWindowHeight) {

  windowWidth = freshWindowWidth;
  windowHeight = freshWindowHeight;

  float displayAspectRatio = (float)windowWidth / windowHeight;

  usefulArea.x = 0;
  usefulArea.y = 0;
  usefulArea.h = windowHeight;
  usefulArea.w = windowWidth;

  float effectiveAspectRatio;
  if (displayAspectRatio < POS_MIN_SUPPORTED_ASPECT_RATIO) {

    // the display is way more "squarer" than the supported interval
    // so we will apply horizontal stripes at the top and bottom
    // to keep it within limits.

    effectiveAspectRatio = POS_MIN_SUPPORTED_ASPECT_RATIO;

    usefulArea.h = usefulArea.w / effectiveAspectRatio;
    Sint32 stripeHeight = (windowHeight - usefulArea.h) / 2.0;

    usefulArea.y = stripeHeight;

  } else if (displayAspectRatio > POS_MAX_SUPPORTED_ASPECT_RATIO) {

    // the display is way more "thinner" than the supported interval
    // so we will apply vertical stripes at the left and right
    // to keep it within limits.

    effectiveAspectRatio = POS_MAX_SUPPORTED_ASPECT_RATIO;

    usefulArea.w = usefulArea.h * effectiveAspectRatio;
    Sint32 stripeWidth = (windowWidth - usefulArea.w) / 2.0;

    usefulArea.x = stripeWidth;

  } else {
    effectiveAspectRatio = displayAspectRatio;
  }

  zoom = usefulArea.h / POS_BASE_DISPLAY_HEIGHT;

  // useful for debugging
  // logInfo("Pos_Relayout: usefulArea=%dh %dw %dx %dy zoom=%.2f r=%.2f",
  //   usefulArea.h, usefulArea.w, usefulArea.x, usefulArea.y, zoom, effectiveAspectRatio);
}


SDL_Rect Pos_CalcAnchored(Pos_AnchoredElement *el) {
  SDL_Rect result;
  result.w = SDL_ceilf(zoomed(el->width));
  result.h = SDL_ceilf(zoomed(el->height));

  if ((el->anchors & POS_ANCHOR_LEFT) == POS_ANCHOR_LEFT) {
    result.x = usefulArea.x + zoomed(el->anchorLeft);
  } else if ((el->anchors & POS_ANCHOR_RIGHT) == POS_ANCHOR_RIGHT) {
    result.x = usefulArea.x + usefulArea.w - zoomed(el->anchorRight) - result.w;
  } else if ((el->anchors & POS_ANCHOR_CENTER_LEFT) == POS_ANCHOR_CENTER_LEFT) {
    result.x = usefulArea.x + usefulArea.w / 2.0 + zoomed(el->anchorCenterLeft);
  } else if ((el->anchors & POS_ANCHOR_CENTER_RIGHT) == POS_ANCHOR_CENTER_RIGHT) {
    result.x = usefulArea.x + usefulArea.w / 2.0 - zoomed(el->anchorCenterRight) - result.w;
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


SDL_bool Pos_IsInside(SDL_Rect *calculatedRect, Input_ClickTap *pos) {
  return pos->x >= calculatedRect->x &&
    pos->x <= calculatedRect->x + calculatedRect->w &&
    pos->y >= calculatedRect->y &&
    pos->y <= calculatedRect->y + calculatedRect->h;
}


SDL_bool Pos_IsFingerEventInside(SDL_Rect *calculatedRect, Input_FingerEvent *event) {
  Sint32 x = windowWidth * event->nx;
  Sint32 y = windowHeight * event->ny;
  return x >= calculatedRect->x &&
    x <= calculatedRect->x + calculatedRect->w &&
    y >= calculatedRect->y &&
    y <= calculatedRect->y + calculatedRect->h;
}

void Pos_RenderLetterBox(SDL_Renderer *renderer) {
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
  if (usefulArea.x > 0) {
    SDL_Rect leftStripe = { 0, 0, usefulArea.x, windowHeight };
    SDL_RenderFillRect(renderer, &leftStripe);
    SDL_Rect rightStripe = { usefulArea.x + usefulArea.w, 0, windowWidth - usefulArea.x - usefulArea.w, windowHeight };
    SDL_RenderFillRect(renderer, &rightStripe);
  }
  if (usefulArea.y > 0) {
    SDL_Rect topStripe = { 0, 0, windowWidth, usefulArea.y };
    SDL_RenderFillRect(renderer, &topStripe);
    SDL_Rect bottomStripe = { 0, usefulArea.y + usefulArea.h, windowWidth, windowHeight - usefulArea.y - usefulArea.h };
    SDL_RenderFillRect(renderer, &bottomStripe);
  }
}
