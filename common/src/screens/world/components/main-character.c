#include "core/positioning.h"
#include "core/log.h"
#include "core/resources.h"
#include "screens/world/components/main-character.h"
#include "screens/world/components/map.h"


Pos_AnchoredElement mainCharacterPos;
SDL_Rect mainCharacterRect;
SDL_Texture *mainCharacterTexture;
SDL_Rect mainCharacterPose;
SDL_bool mainCharacterWalkingNorth = SDL_FALSE;
SDL_bool mainCharacterWalkingSouth = SDL_FALSE;
SDL_bool mainCharacterWalkingWest = SDL_FALSE;
SDL_bool mainCharacterWalkingEast = SDL_FALSE;
Sint32 mainCharacterTimeWalking = 0;
#define MAIN_CHARACTER_SIZE 100


void World_MainCharacter_Init(Nav_Context *ctx) {
  logInfo("MainCharacter: initializing.");

  mainCharacterPos.anchors = POS_ANCHOR_TOP | POS_ANCHOR_CENTER_LEFT;
  mainCharacterPos.width = 100;
  mainCharacterPos.height = 100;
  mainCharacterPos.anchorTop = 405;
  mainCharacterPos.anchorCenterLeft = -50;
  mainCharacterPose.w = 99;
  mainCharacterPose.h = 99;
  mainCharacterPose.x = 1;
  mainCharacterPose.y = 201;
  mainCharacterTexture = Res_LoadTexture(ctx, "main-character.png");

}


void World_MainCharacter_UpdateModel(Uint64 elapsedTime, Sint32 *playerPosX, Sint32 *playerPosY) {

  Sint32 dx = 0;
  Sint32 dy = 0;

  if (mainCharacterWalkingNorth) {
    dy -= elapsedTime / 2;
  }
  if (mainCharacterWalkingSouth) {
    dy += elapsedTime / 2;
  }
  if (mainCharacterWalkingWest) {
    dx -= elapsedTime / 2;
  }
  if (mainCharacterWalkingEast) {
    dx += elapsedTime / 2;
  }

  if (dx != 0 && *playerPosX + dx + (dx < 0 ? -MAIN_CHARACTER_SIZE : 0) >= 0) {
    Sint32 futureCellX = (*playerPosX + dx + (dx < 0 ? -MAIN_CHARACTER_SIZE : 0)) / MAP_CELL_SIZE;
    if (futureCellX >= 0 && futureCellX < MAP_COLUMNS) {
      if (World_Map_IsPassableTile(*playerPosY / MAP_CELL_SIZE, futureCellX)) {
        *playerPosX += dx;
      }
    }
  }
  if (dy != 0 && *playerPosY + dy >= 0) {
    Sint32 futureCellY = (*playerPosY + dy) / MAP_CELL_SIZE;
    if (futureCellY >= 0 && futureCellY < MAP_ROWS) {
      SDL_bool futureBottomRightPassable = World_Map_IsPassableTile(futureCellY, *playerPosX / MAP_CELL_SIZE);
      SDL_bool futureBottomLeftPassable = World_Map_IsPassableTile(futureCellY, (*playerPosX - MAIN_CHARACTER_SIZE) / MAP_CELL_SIZE);
      if (futureBottomRightPassable && futureBottomLeftPassable) {
        *playerPosY += dy;
      }
    }
  }

  if (dx != 0 || dy != 0) {
    mainCharacterTimeWalking += elapsedTime;
    mainCharacterPose.x = ((mainCharacterTimeWalking / 100) % 3) * MAIN_CHARACTER_SIZE;
  } else {
    mainCharacterTimeWalking = 0;
    mainCharacterPose.x = 1 * MAIN_CHARACTER_SIZE;
  }

  if (dx > 0) { // east
    mainCharacterPose.y = 1 * MAIN_CHARACTER_SIZE;
  } else if (dx < 0) { // west
    mainCharacterPose.y = 3 * MAIN_CHARACTER_SIZE;
  } else {
    if (dy > 0) { // south
      mainCharacterPose.y = 2 * MAIN_CHARACTER_SIZE;
    } else if (dy < 0) { // north
      mainCharacterPose.y = 0 * MAIN_CHARACTER_SIZE;
    }
  }

}


void World_MainCharacter_Render(Nav_Context *ctx) {

  mainCharacterRect = Pos_CalcAnchored(&mainCharacterPos);
  if (SDL_RenderCopy(ctx->renderer, mainCharacterTexture, &mainCharacterPose, &mainCharacterRect) != 0) {
    logError("MainCharacter: failed to render: %s %s", SDL_GetError());
    exit(1);
  }

}

void World_MainCharacter_SetWalkingDirections(MaybeBool north, MaybeBool south, MaybeBool west, MaybeBool east) {
  if (north != MaybeBool_EMPTY) {
    mainCharacterWalkingNorth = north == MaybeBool_TRUE;
  }
  if (south != MaybeBool_EMPTY) {
    mainCharacterWalkingSouth = south == MaybeBool_TRUE;
  }
  if (west != MaybeBool_EMPTY) {
    mainCharacterWalkingWest = west == MaybeBool_TRUE;
  }
  if (east != MaybeBool_EMPTY) {
    mainCharacterWalkingEast = east == MaybeBool_TRUE;
  }
}


void World_MainCharacter_HandleKeyboardEvent(Nav_Context *ctx, Nav_KeyboardEvent *event) {
  switch (event->type) {
    case NAV_KEYBOARD_EVENT_TYPE_DOWN:
      switch (event->key) {
        case SDLK_UP:
          mainCharacterWalkingNorth = SDL_TRUE;
          break;
        case SDLK_DOWN:
          mainCharacterWalkingSouth = SDL_TRUE;
          break;
        case SDLK_LEFT:
          mainCharacterWalkingWest = SDL_TRUE;
          break;
        case SDLK_RIGHT:
          mainCharacterWalkingEast = SDL_TRUE;
          break;
        default:
          break;
      }
      break;
    case NAV_KEYBOARD_EVENT_TYPE_UP:
    default:
      switch (event->key) {
        case SDLK_UP:
          mainCharacterWalkingNorth = SDL_FALSE;
          break;
        case SDLK_DOWN:
          mainCharacterWalkingSouth = SDL_FALSE;
          break;
        case SDLK_LEFT:
          mainCharacterWalkingWest = SDL_FALSE;
          break;
        case SDLK_RIGHT:
          mainCharacterWalkingEast = SDL_FALSE;
          break;
        default:
          break;
      }
      break;
  }
}


void World_MainCharacter_Destroy() {
  logInfo("MainCharacter: destroying.");
  Res_ReleaseTexture(mainCharacterTexture);
}
