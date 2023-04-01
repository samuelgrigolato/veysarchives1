#ifndef SCREENS_WORLD_COMPONENTS_MAIN_CHARACTER_h
#define SCREENS_WORLD_COMPONENTS_MAIN_CHARACTER_h

#include "core/game.h"

typedef enum {
  MaybeBool_FALSE,
  MaybeBool_TRUE,
  MaybeBool_EMPTY
} MaybeBool;

void World_MainCharacter_Init(Game_Context *ctx);

void World_MainCharacter_UpdateModel(Uint64 elapsedTime, Sint32 *playerPosX, Sint32 *playerPosY);

void World_MainCharacter_Render(Game_Context *ctx);

void World_MainCharacter_SetWalkingDirections(MaybeBool north, MaybeBool south, MaybeBool west, MaybeBool east);

void World_MainCharacter_HandleKeyboardEvent();

void World_MainCharacter_Destroy();

#endif
