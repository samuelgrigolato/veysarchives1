#ifndef SCREENS_WORLD_COMPONENTS_MOBILE_MOTION_CONTROLLER_H
#define SCREENS_WORLD_COMPONENTS_MOBILE_MOTION_CONTROLLER_H

#include "core/navigation.h"

void World_MobileMotionController_Init(struct Nav_Context *ctx);

void World_MobileMotionController_Render(struct Nav_Context *ctx);

void World_MobileMotionController_HandleFingerEvent(struct Nav_Context *ctx, struct Nav_FingerEvent *event);

void World_MobileMotionController_Destroy();

#endif
