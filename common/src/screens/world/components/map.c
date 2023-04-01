#include "core/positioning.h"
#include "core/audio.h"
#include "core/log.h"
#include "core/resources.h"
#include "screens/world/components/map.h"


char *map;
SDL_Texture *mapCellTextures;
#define MAP_MAX_VISIBLE_ROWS ((POS_BASE_DISPLAY_HEIGHT / MAP_CELL_SIZE) + 1)
#define MAP_MAX_VISIBLE_COLUMNS ((POS_BASE_DISPLAY_WIDTH / MAP_CELL_SIZE) + 1)


Pos_AnchoredElement minimapPos;
SDL_Rect minimapRect;
SDL_Texture *minimapTexture;
Aud_SoundID minimapPress;
#define MINIMAP_CELL_SIZE 5
#define MINIMAP_ROWS (200 / MINIMAP_CELL_SIZE)
#define MINIMAP_COLUMNS (200 / MINIMAP_CELL_SIZE)


void World_Map_Init(Nav_Context *ctx) {
  logInfo("Map: initializing.");

  map = Res_ReadFully("map.txt", 128);

  // apply terrain seam pass
  for (uint8_t row = 0; row < MAP_ROWS; row++) {
    for (uint8_t column = 0; column < MAP_COLUMNS; column++) {
      if (column > 0) {
        char *tile = map + ((row * (MAP_COLUMNS + 1)) + column);
        char *leftTile = tile - 1;
        if (*tile == '1' && *leftTile == '0') {
          // things to improve:
          // separate model from model with seams applied
          *tile = '2';
        }
      }
    }
  }

  mapCellTextures = Res_LoadTexture(ctx, "map-tiles.png");

  minimapPos.anchors = POS_ANCHOR_TOP | POS_ANCHOR_RIGHT;
  minimapPos.width = 240;
  minimapPos.height = 240;
  minimapPos.anchorTop = 20;
  minimapPos.anchorRight = 20;
  minimapTexture = Res_LoadTexture(ctx, "minimap.png");

}


SDL_bool World_Map_IsPassableTile(int row, int column) {
  char *tile = map + ((row * (MAP_COLUMNS + 1)) + column);
  return *tile == '0';
}


void World_Map_Render(Nav_Context *ctx, Sint32 *playerPosX, Sint32 *playerPosY) {

  Sint32 playerCellX = *playerPosX / MAP_CELL_SIZE;
  Sint32 playerCellY = *playerPosY / MAP_CELL_SIZE;
  Sint32 playerCellRelativePosX = *playerPosX % MAP_CELL_SIZE;
  Sint32 playerCellRelativePosY = *playerPosY % MAP_CELL_SIZE;
  Sint32 leftMostRenderedColumn = playerCellX - (MAP_MAX_VISIBLE_COLUMNS / 2);
  Sint32 highestRenderedRow = playerCellY - (MAP_MAX_VISIBLE_ROWS / 2);
  for (uint8_t row = 0; row < MAP_MAX_VISIBLE_ROWS; row++) {
    for (uint8_t column = 0; column < MAP_MAX_VISIBLE_COLUMNS; column++) {

      Sint32 renderedCellY = highestRenderedRow + row;
      Sint32 renderedCellX = leftMostRenderedColumn + column;
      if (renderedCellX < 0 || renderedCellY < 0 ||
          renderedCellY >= MAP_ROWS ||
          renderedCellX >= MAP_COLUMNS) {
        // cell is out of bounds
        continue;
      }

      Pos_AnchoredElement mapCell;
      mapCell.anchors = POS_ANCHOR_TOP | POS_ANCHOR_CENTER_LEFT;
      mapCell.height = MAP_CELL_SIZE;
      mapCell.width = MAP_CELL_SIZE;
      mapCell.anchorTop = (row * MAP_CELL_SIZE) - playerCellRelativePosY;
      mapCell.anchorCenterLeft = -(POS_BASE_DISPLAY_WIDTH / 2) + (column * MAP_CELL_SIZE) - playerCellRelativePosX;

      SDL_Rect mapCellPos = Pos_CalcAnchored(&mapCell);
      SDL_Rect mapCellTile;
      mapCellTile.w = MAP_CELL_SIZE;
      mapCellTile.h = MAP_CELL_SIZE;
      mapCellTile.x = MAP_CELL_SIZE * (map[(renderedCellY * (MAP_COLUMNS + 1)) + renderedCellX] - '0');
      mapCellTile.y = 0;
      SDL_RenderCopy(ctx->renderer, mapCellTextures, &mapCellTile, &mapCellPos);

    }
  }

  minimapRect = Pos_CalcAnchored(&minimapPos);
  if (SDL_RenderCopy(ctx->renderer, minimapTexture, NULL, &minimapRect) != 0) {
    logError("Map: failed to render minimap: %s %s", SDL_GetError());
    exit(1);
  }

  // render minimap squares
  Sint32 leftMostMinimapColumn = playerCellX - (MINIMAP_COLUMNS / 2);
  Sint32 highestMinimapRow = playerCellY - (MINIMAP_ROWS / 2);
  for (uint8_t row = 0; row < MINIMAP_ROWS; row++) {
    for (uint8_t column = 0; column < MINIMAP_COLUMNS; column++) {

      Pos_AnchoredElement minimapCellPos;
      minimapCellPos.anchors = POS_ANCHOR_TOP | POS_ANCHOR_RIGHT;
      minimapCellPos.width = MINIMAP_CELL_SIZE;
      minimapCellPos.height = MINIMAP_CELL_SIZE;
      minimapCellPos.anchorTop = 40 + row * MINIMAP_CELL_SIZE;
      minimapCellPos.anchorRight = 40 + (MINIMAP_COLUMNS - column - 1) * MINIMAP_CELL_SIZE;

      Uint8 cellR = 0;
      Uint8 cellG = 0;
      Uint8 cellB = 0;
      if (row == MINIMAP_ROWS / 2 && column == MINIMAP_COLUMNS / 2) {
        cellR = cellG = cellB = 255;
      } else {
        Sint32 minimapCellY = highestMinimapRow + row;
        Sint32 minimapCellX = leftMostMinimapColumn + column;
        if (minimapCellX < 0 || minimapCellY < 0 ||
            minimapCellY >= MAP_ROWS ||
            minimapCellX >= MAP_COLUMNS) {
          // cell is out of bounds, black cell is fine
        } else {
          char *minimapCellTile = map + ((minimapCellY * (MAP_COLUMNS + 1)) + minimapCellX);
          if (*minimapCellTile == '0') {
            cellG = 200;
          } else {
            cellB = 200;
          }
        }
      }
      SDL_SetRenderDrawColor(ctx->renderer, cellR, cellG, cellB, 255);

      SDL_Rect minimapCell = Pos_CalcAnchored(&minimapCellPos);
      SDL_RenderFillRect(ctx->renderer, &minimapCell);
    }
  }

}


void World_Map_Destroy() {
  logInfo("Map: destroying.");
  Res_ReleaseTexture(mapCellTextures);
  Res_ReleaseTexture(minimapTexture);
  SDL_free(map);
}
