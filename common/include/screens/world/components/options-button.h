#ifndef SCREENS_WORLD_COMPONENTS_OPTIONS_BUTTON_H
#define SCREENS_WORLD_COMPONENTS_OPTIONS_BUTTON_H

#include "core/game.h"

void World_OptionsButton_Init(Game_Context *ctx);

void World_OptionsButton_Render(Game_Context *ctx);

void World_OptionsButton_HandleClickTap(Game_Context *ctx, Input_ClickTap *pos);

void World_OptionsButton_Destroy();

#endif
