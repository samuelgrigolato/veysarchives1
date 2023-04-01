#ifndef SCREENS_WORLD_COMPONENTS_MOBILE_MOTION_CONTROLLER_H
#define SCREENS_WORLD_COMPONENTS_MOBILE_MOTION_CONTROLLER_H

#include "core/game.h"

void World_MobileMotionController_Init(Game_Context *ctx);

void World_MobileMotionController_Render(Game_Context *ctx);

void World_MobileMotionController_HandleFingerEvent(Game_Context *ctx, Input_FingerEvent *event);

void World_MobileMotionController_Destroy();

#endif
