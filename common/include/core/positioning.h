#ifndef CORE_POSITIONING_h
#define CORE_POSITIONING_h

#include "platform.h"
#include <SDL.h>

#include "core/navigation.h"

#define POS_DRAWABLE_SCREEN_WIDTH 1600.0
#define POS_DRAWABLE_SCREEN_HEIGHT 800.0
#define POS_DRAWABLE_SCREEN_RATIO (POS_DRAWABLE_SCREEN_WIDTH / POS_DRAWABLE_SCREEN_HEIGHT)

void Pos_Relayout(struct Nav_Context *navCtx);

void Pos_Apply(struct SDL_Rect *base);

#endif
