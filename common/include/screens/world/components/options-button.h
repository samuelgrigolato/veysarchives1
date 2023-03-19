#ifndef SCREENS_WORLD_COMPONENTS_OPTIONS_BUTTON_H
#define SCREENS_WORLD_COMPONENTS_OPTIONS_BUTTON_H

#include "core/navigation.h"

void World_OptionsButton_Init(struct Nav_Context *ctx);

void World_OptionsButton_Render(struct Nav_Context *ctx);

void World_OptionsButton_HandleClickTap(struct Nav_Context *ctx, struct Nav_ClickTap *pos);

void World_OptionsButton_Destroy();

#endif
