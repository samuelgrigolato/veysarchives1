#include "core/game.h"
#include "core/positioning.h"
#include "core/log.h"
#include "core/resources.h"
#include "screens/world/components/main-character.h"
#include "screens/world/components/map.h"


static Pos_AnchoredElement pos;
static SDL_Rect rect;
static SDL_Texture *texture;
static SDL_Rect pose;
static SDL_bool walkingNorth = SDL_FALSE;
static SDL_bool walkingSouth = SDL_FALSE;
static SDL_bool walkingWest = SDL_FALSE;
static SDL_bool walkingEast = SDL_FALSE;
static Sint32 timeWalking = 0;
#define MAIN_CHARACTER_SIZE 100


void World_MainCharacter_Init(Game_Context *ctx) {
  logInfo("MainCharacter: initializing.");

  walkingNorth = SDL_FALSE;
  walkingSouth = SDL_FALSE;
  walkingWest = SDL_FALSE;
  walkingEast = SDL_FALSE;
  timeWalking = 0;

  pos.anchors = POS_ANCHOR_TOP | POS_ANCHOR_CENTER_LEFT;
  pos.width = 100;
  pos.height = 100;
  pos.anchorTop = 405;
  pos.anchorCenterLeft = -50;
  pose.w = 99;
  pose.h = 99;
  pose.x = 1;
  pose.y = 201;
  texture = Res_LoadTexture(ctx, "main-character.png");

}


void World_MainCharacter_UpdateModel(Uint64 elapsedTime, Sint32 *playerPosX, Sint32 *playerPosY) {

  Sint32 dx = 0;
  Sint32 dy = 0;

  if (walkingNorth) {
    dy -= elapsedTime / 2;
  }
  if (walkingSouth) {
    dy += elapsedTime / 2;
  }
  if (walkingWest) {
    dx -= elapsedTime / 2;
  }
  if (walkingEast) {
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
    timeWalking += elapsedTime;
    pose.x = ((timeWalking / 100) % 3) * MAIN_CHARACTER_SIZE;
  } else {
    timeWalking = 0;
    pose.x = 1 * MAIN_CHARACTER_SIZE;
  }

  if (dx > 0) { // east
    pose.y = 1 * MAIN_CHARACTER_SIZE;
  } else if (dx < 0) { // west
    pose.y = 3 * MAIN_CHARACTER_SIZE;
  } else {
    if (dy > 0) { // south
      pose.y = 2 * MAIN_CHARACTER_SIZE;
    } else if (dy < 0) { // north
      pose.y = 0 * MAIN_CHARACTER_SIZE;
    }
  }

}


void World_MainCharacter_Render(Game_Context *ctx) {

  rect = Pos_CalcAnchored(&pos);
  if (SDL_RenderCopy(ctx->renderer, texture, &pose, &rect) != 0) {
    logError("MainCharacter: failed to render: %s %s", SDL_GetError());
    exit(1);
  }

}

void World_MainCharacter_SetWalkingDirections(MaybeBool north, MaybeBool south, MaybeBool west, MaybeBool east) {
  if (north != MaybeBool_EMPTY) {
    walkingNorth = north == MaybeBool_TRUE;
  }
  if (south != MaybeBool_EMPTY) {
    walkingSouth = south == MaybeBool_TRUE;
  }
  if (west != MaybeBool_EMPTY) {
    walkingWest = west == MaybeBool_TRUE;
  }
  if (east != MaybeBool_EMPTY) {
    walkingEast = east == MaybeBool_TRUE;
  }
}


void World_MainCharacter_HandleKeyboardEvent(Game_Context *ctx, Input_KeyboardEvent *event) {
  switch (event->type) {
    case INPUT_KEYBOARD_EVENT_TYPE_DOWN:
      switch (event->key) {
        case SDLK_UP:
          walkingNorth = SDL_TRUE;
          break;
        case SDLK_DOWN:
          walkingSouth = SDL_TRUE;
          break;
        case SDLK_LEFT:
          walkingWest = SDL_TRUE;
          break;
        case SDLK_RIGHT:
          walkingEast = SDL_TRUE;
          break;
        default:
          break;
      }
      break;
    case INPUT_KEYBOARD_EVENT_TYPE_UP:
    default:
      switch (event->key) {
        case SDLK_UP:
          walkingNorth = SDL_FALSE;
          break;
        case SDLK_DOWN:
          walkingSouth = SDL_FALSE;
          break;
        case SDLK_LEFT:
          walkingWest = SDL_FALSE;
          break;
        case SDLK_RIGHT:
          walkingEast = SDL_FALSE;
          break;
        default:
          break;
      }
      break;
  }
}


void World_MainCharacter_Destroy() {
  logInfo("MainCharacter: destroying.");
  Res_ReleaseTexture(texture);
}
