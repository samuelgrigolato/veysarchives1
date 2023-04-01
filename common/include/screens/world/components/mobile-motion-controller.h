#ifndef SCREENS_WORLD_COMPONENTS_MOBILE_MOTION_CONTROLLER_H
#define SCREENS_WORLD_COMPONENTS_MOBILE_MOTION_CONTROLLER_H

#include "core/navigation.h"

void World_MobileMotionController_Init(Nav_Context *ctx);

void World_MobileMotionController_Render(Nav_Context *ctx);

void World_MobileMotionController_HandleFingerEvent(Nav_Context *ctx, Nav_FingerEvent *event);

void World_MobileMotionController_Destroy();

#endif
