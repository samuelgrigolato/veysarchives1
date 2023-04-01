#ifndef CORE_POSITIONING_h
#define CORE_POSITIONING_h

#include "platform.h"
#include <SDL.h>
#include "core/input.h"

#define POS_BASE_DISPLAY_HEIGHT 900.0
#define POS_BASE_DISPLAY_WIDTH 2100.0

typedef enum {
  POS_ANCHOR_TOP = 1,
  POS_ANCHOR_BOTTOM = 2,
  POS_ANCHOR_LEFT = 4,
  POS_ANCHOR_RIGHT = 8,
  POS_ANCHOR_CENTER_LEFT = 16,
  POS_ANCHOR_CENTER_RIGHT = 32
} Pos_Anchor;

typedef struct {

  /** To be filled as a bitwise of POS_ANCHOR_XXXXX. */
  Pos_Anchor anchors;

  Sint32 anchorTop;
  Sint32 anchorBottom;
  Sint32 anchorLeft;
  Sint32 anchorRight;
  Sint32 anchorCenterLeft;
  Sint32 anchorCenterRight;

  /** As in a base display, i.e. 900px height. */
  Sint32 width;

  /** As in a base display, i.e. 900px height. */
  Sint32 height;

} Pos_AnchoredElement;

void Pos_Relayout(Sint32 windowWidth, Sint32 windowHeight);

SDL_Rect Pos_CalcAnchored(Pos_AnchoredElement *el);

/**
 * Calculates the correct render rectangle of an image drawn
 * in 2100x900 so it "covers" the entire display, keeping the
 * height and allowing horizontal borders to be hidden.
*/
SDL_Rect Pos_CalcCover();

SDL_bool Pos_IsInside(SDL_Rect *calculatedRect, Input_ClickTap *pos);

SDL_bool Pos_IsFingerEventInside(SDL_Rect *calculatedRect, Input_FingerEvent *event);

void Pos_RenderLetterBox(SDL_Renderer *renderer);

#endif
