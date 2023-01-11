#ifndef CORE_POSITIONING_h
#define CORE_POSITIONING_h

#include "platform.h"
#include <SDL.h>

#include "core/navigation.h"

#define POS_ANCHOR_TOP 1
#define POS_ANCHOR_BOTTOM 2
#define POS_ANCHOR_LEFT 4
#define POS_ANCHOR_RIGHT 8
#define POS_ANCHOR_CENTER_LEFT 16
#define POS_ANCHOR_CENTER_RIGHT 32

struct Pos_AnchoredElement {

  /** To be filled as a bitwise of POS_ANCHOR_XXXXX. */
  Uint8 anchors;

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

};

void Pos_Relayout(struct Nav_Context *navCtx);

SDL_Rect Pos_CalcAnchored(struct Pos_AnchoredElement *el);

/**
 * Calculates the correct render rectangle of an image drawn
 * in 2100x900 so it "covers" the entire display, keeping the
 * height and allowing horizontal borders to be hidden.
*/
SDL_Rect Pos_CalcCover();

SDL_bool Pos_IsInside(SDL_Rect *calculatedRect, struct Nav_ClickTap *pos);

#endif
