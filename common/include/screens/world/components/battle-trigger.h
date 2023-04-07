#ifndef SCREENS_WORLD_COMPONENTS_BATTLE_TRIGGER_h
#define SCREENS_WORLD_COMPONENTS_BATTLE_TRIGGER_h

#include "core/game.h"

void World_BattleTrigger_Init(Game_Context *ctx);

void World_BattleTrigger_UpdateModel(Uint64 elapsedTime, Sint32 playerPosX, Sint32 playerPosY);

void World_BattleTrigger_Render(Game_Context *ctx);

void World_BattleTrigger_HandleClickTap(Game_Context *ctx, Input_ClickTap *pos);

void World_BattleTrigger_HandleKeyboardEvent(Game_Context *ctx, Input_KeyboardEvent *event);

void World_BattleTrigger_Destroy();

#endif
