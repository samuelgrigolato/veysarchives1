#include "core/log.h"
#include "core/positioning.h"
#include "core/resources.h"
#include "core/audio.h"
#include "screens/main.h"
#include "screens/home.h"


struct Pos_AnchoredElement optionsButtonPos;
SDL_Rect optionsButtonRect;
SDL_Texture *optionsButtonTexture;
Aud_SoundID optionsButtonPress;

struct Pos_AnchoredElement mainCharacterPos;
SDL_Rect mainCharacterRect;
SDL_Texture *mainCharacterTexture;
SDL_Rect mainCharacterPose;
SDL_bool mainCharacterWalkingNorth = SDL_FALSE;
SDL_bool mainCharacterWalkingSouth = SDL_FALSE;
SDL_bool mainCharacterWalkingWest = SDL_FALSE;
SDL_bool mainCharacterWalkingEast = SDL_FALSE;
Sint32 mainCharacterTimeWalking = 0;
#define MAIN_CHARACTER_SIZE 100

SDL_bool mobileMotionControllerVisible = !DESKTOP;
struct Pos_AnchoredElement mobileMotionControllerPos;
SDL_Rect mobileMotionControllerRect;
SDL_Texture *mobileMotionControllerTexture;
SDL_Rect mobileMotionControllerPose;
SDL_bool mobileMotionControllerActive = SDL_FALSE;
#define MOBILE_MOTION_CONTROLLER_IDLE_THRESHOLD 75
#define MOBILE_MOTION_CONTROLLER_SIZE 400

#define MAP_ROWS 20
#define MAP_COLUMNS 60
char *map;
SDL_Texture *mapCellTextures;
SDL_Texture *mobileMotionControllerTexture;
#define MAP_CELL_SIZE 100
#define MAP_MAX_VISIBLE_ROWS ((POS_BASE_DISPLAY_HEIGHT / MAP_CELL_SIZE) + 1)
#define MAP_MAX_VISIBLE_COLUMNS ((POS_BASE_DISPLAY_WIDTH / MAP_CELL_SIZE) + 1)
Sint32 playerPosX = 100;
Sint32 playerPosY = 100;

struct Pos_AnchoredElement minimapPos;
SDL_Rect minimapRect;
SDL_Texture *minimapTexture;
Aud_SoundID minimapPress;
#define MINIMAP_CELL_SIZE 5
#define MINIMAP_ROWS (200 / MINIMAP_CELL_SIZE)
#define MINIMAP_COLUMNS (200 / MINIMAP_CELL_SIZE)


void Main_Init(struct Nav_Context *ctx) {
  logInfo("Main: initializing.");

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

  optionsButtonPos.anchors = POS_ANCHOR_BOTTOM | POS_ANCHOR_RIGHT;
  optionsButtonPos.width = 100;
  optionsButtonPos.height = 100;
  optionsButtonPos.anchorBottom = 0;
  optionsButtonPos.anchorRight = 0;
  optionsButtonTexture = Res_LoadTexture(ctx, "options-button.png");
  optionsButtonPress = Aud_Load("button-press.wav");

  if (mobileMotionControllerVisible) {
    mobileMotionControllerPos.anchors = POS_ANCHOR_BOTTOM | POS_ANCHOR_LEFT;
    mobileMotionControllerPos.width = MOBILE_MOTION_CONTROLLER_SIZE;
    mobileMotionControllerPos.height = MOBILE_MOTION_CONTROLLER_SIZE;
    mobileMotionControllerPos.anchorBottom = 50;
    mobileMotionControllerPos.anchorLeft = 50;
    mobileMotionControllerPose.w = MOBILE_MOTION_CONTROLLER_SIZE;
    mobileMotionControllerPose.h = MOBILE_MOTION_CONTROLLER_SIZE;
    mobileMotionControllerPose.x = MOBILE_MOTION_CONTROLLER_SIZE;
    mobileMotionControllerPose.y = 0;
    mobileMotionControllerTexture = Res_LoadTexture(ctx, "mobile-motion-controller.png");
  }

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


void Main_UpdateModel(Uint64 elapsedTime) {

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

  if (dx != 0 && playerPosX + dx + (dx < 0 ? -MAIN_CHARACTER_SIZE : 0) >= 0) {
    Sint32 futureCellX = (playerPosX + dx + (dx < 0 ? -MAIN_CHARACTER_SIZE : 0)) / MAP_CELL_SIZE;
    if (futureCellX >= 0 && futureCellX < MAP_COLUMNS) {
      char futureCellTile = map[((playerPosY / MAP_CELL_SIZE) * (MAP_COLUMNS + 1)) + futureCellX];
      if (futureCellTile == '0') {
        // passable tile
        playerPosX += dx;
      }
    }
  }
  if (dy != 0 && playerPosY + dy >= 0) {
    Sint32 futureCellY = (playerPosY + dy) / MAP_CELL_SIZE;
    if (futureCellY >= 0 && futureCellY < MAP_ROWS) {
      char futureCellBottomRightTile = map[(futureCellY * (MAP_COLUMNS + 1)) + (playerPosX / MAP_CELL_SIZE)];
      char futureCellBottomLeftTile = map[(futureCellY * (MAP_COLUMNS + 1)) + ((playerPosX - MAIN_CHARACTER_SIZE) / MAP_CELL_SIZE)];
      if (futureCellBottomRightTile == '0' && futureCellBottomLeftTile == '0') {
        playerPosY += dy;
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


void Main_Render(struct Nav_Context *ctx) {

  // render map
  Sint32 playerCellX = playerPosX / MAP_CELL_SIZE;
  Sint32 playerCellY = playerPosY / MAP_CELL_SIZE;
  Sint32 playerCellRelativePosX = playerPosX % MAP_CELL_SIZE;
  Sint32 playerCellRelativePosY = playerPosY % MAP_CELL_SIZE;
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

      struct Pos_AnchoredElement mapCell;
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
    logError("Main: failed to render minimap: %s %s", SDL_GetError());
    exit(1);
  }

  // render minimap squares
  Sint32 leftMostMinimapColumn = playerCellX - (MINIMAP_COLUMNS / 2);
  Sint32 highestMinimapRow = playerCellY - (MINIMAP_ROWS / 2);
  for (uint8_t row = 0; row < MINIMAP_ROWS; row++) {
    for (uint8_t column = 0; column < MINIMAP_COLUMNS; column++) {
      SDL_Rect minimapCell;
      minimapCell.w = MINIMAP_CELL_SIZE;
      minimapCell.h = MINIMAP_CELL_SIZE;
      minimapCell.x = minimapRect.x + 20 + column * MINIMAP_CELL_SIZE;
      minimapCell.y = minimapRect.y + 20 + row * MINIMAP_CELL_SIZE;

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

      SDL_RenderFillRect(ctx->renderer, &minimapCell);
    }
  }

  mainCharacterRect = Pos_CalcAnchored(&mainCharacterPos);
  if (SDL_RenderCopy(ctx->renderer, mainCharacterTexture, &mainCharacterPose, &mainCharacterRect) != 0) {
    logError("Main: failed to render main character: %s %s", SDL_GetError());
    exit(1);
  }

  optionsButtonRect = Pos_CalcAnchored(&optionsButtonPos);
  if (SDL_RenderCopy(ctx->renderer, optionsButtonTexture, NULL, &optionsButtonRect) != 0) {
    logError("Main: failed to render options button: %s %s", SDL_GetError());
    exit(1);
  }

  if (mobileMotionControllerVisible) {
    mobileMotionControllerRect = Pos_CalcAnchored(&mobileMotionControllerPos);
    if (SDL_RenderCopy(ctx->renderer, mobileMotionControllerTexture, &mobileMotionControllerPose, &mobileMotionControllerRect) != 0) {
      logError("Main: failed to render mobile motion controller: %s %s", SDL_GetError());
      exit(1);
    }
  }

}


void Main_HandleClickTap(struct Nav_Context *ctx, struct Nav_ClickTap *pos) {
  if (Pos_IsInside(&optionsButtonRect, pos)) {
    Aud_PlayOnce(optionsButtonPress);
    Nav_GoTo(Home_GetScreen());
  }
}


void Main_HandleFingerEvent(struct Nav_Context *ctx, struct Nav_FingerEvent *event) {
  if (!mobileMotionControllerVisible) return;

  if (event->type == NAV_FINGER_EVENT_TYPE_DOWN &&
      Pos_IsFingerEventInside(&mobileMotionControllerRect, event, ctx) &&
      mobileMotionControllerActive == SDL_FALSE) {

    mobileMotionControllerActive = SDL_TRUE;
    mobileMotionControllerPose.x = 0;
    mobileMotionControllerPose.y = 0;

  } else if (event->type == NAV_FINGER_EVENT_TYPE_UP &&
             mobileMotionControllerActive == SDL_TRUE) {

    mobileMotionControllerActive = SDL_FALSE;
    mobileMotionControllerPose.x = 400;
    mobileMotionControllerPose.y = 0;
    mainCharacterWalkingNorth = SDL_FALSE;
    mainCharacterWalkingSouth = SDL_FALSE;
    mainCharacterWalkingWest = SDL_FALSE;
    mainCharacterWalkingEast = SDL_FALSE;

  } else if (mobileMotionControllerActive == SDL_TRUE) { // NAV_FINGER_EVENT_MOTION

    Sint32 eventX = ctx->windowWidth * event->nx;
    Sint32 eventY = ctx->windowHeight * event->ny;
    Sint32 relativeX = eventX - (mobileMotionControllerRect.x + (mobileMotionControllerRect.w / 2));
    Sint32 relativeY = eventY - (mobileMotionControllerRect.y + (mobileMotionControllerRect.h / 2));

    Sint32 poseX, poseY;
    if (relativeX > MOBILE_MOTION_CONTROLLER_IDLE_THRESHOLD) { // finger is to the right
      mainCharacterWalkingWest = SDL_FALSE;
      mainCharacterWalkingEast = SDL_TRUE;
      if (relativeY > MOBILE_MOTION_CONTROLLER_IDLE_THRESHOLD) { // finger is below
        // southeast
        poseX = 3 * MOBILE_MOTION_CONTROLLER_SIZE;
        poseY = 1 * MOBILE_MOTION_CONTROLLER_SIZE;
        mainCharacterWalkingNorth = SDL_FALSE;
        mainCharacterWalkingSouth = SDL_TRUE;
      } else if (relativeY < -MOBILE_MOTION_CONTROLLER_IDLE_THRESHOLD) { // finger is above
        // northeast
        poseX = 2 * MOBILE_MOTION_CONTROLLER_SIZE;
        poseY = 1 * MOBILE_MOTION_CONTROLLER_SIZE;
        mainCharacterWalkingNorth = SDL_TRUE;
        mainCharacterWalkingSouth = SDL_FALSE;
      } else { // finger is at vertical center
        // east
        poseX = 3 * MOBILE_MOTION_CONTROLLER_SIZE;
        poseY = 0 * MOBILE_MOTION_CONTROLLER_SIZE;
        mainCharacterWalkingNorth = SDL_FALSE;
        mainCharacterWalkingSouth = SDL_FALSE;
      }
    } else if (relativeX < -MOBILE_MOTION_CONTROLLER_IDLE_THRESHOLD) { // finger is to the left
      mainCharacterWalkingWest = SDL_TRUE;
      mainCharacterWalkingEast = SDL_FALSE;
      if (relativeY > 50) { // finger is below
        // southwest
        poseX = 0 * MOBILE_MOTION_CONTROLLER_SIZE;
        poseY = 2 * MOBILE_MOTION_CONTROLLER_SIZE;
        mainCharacterWalkingNorth = SDL_FALSE;
        mainCharacterWalkingSouth = SDL_TRUE;
      } else if (relativeY < -MOBILE_MOTION_CONTROLLER_IDLE_THRESHOLD) { // finger is above
        // northwest
        poseX = 1 * MOBILE_MOTION_CONTROLLER_SIZE;
        poseY = 2 * MOBILE_MOTION_CONTROLLER_SIZE;
        mainCharacterWalkingNorth = SDL_TRUE;
        mainCharacterWalkingSouth = SDL_FALSE;
      } else { // finger is at vertical center
        // west
        poseX = 1 * MOBILE_MOTION_CONTROLLER_SIZE;
        poseY = 1 * MOBILE_MOTION_CONTROLLER_SIZE;
        mainCharacterWalkingNorth = SDL_FALSE;
        mainCharacterWalkingSouth = SDL_FALSE;
      }
    } else { // finger is at horizontal center
      mainCharacterWalkingWest = SDL_FALSE;
      mainCharacterWalkingEast = SDL_FALSE;
      if (relativeY > MOBILE_MOTION_CONTROLLER_IDLE_THRESHOLD) { // finger is below
        // south
        poseX = 0 * MOBILE_MOTION_CONTROLLER_SIZE;
        poseY = 1 * MOBILE_MOTION_CONTROLLER_SIZE;
        mainCharacterWalkingNorth = SDL_FALSE;
        mainCharacterWalkingSouth = SDL_TRUE;
      } else if (relativeY < -MOBILE_MOTION_CONTROLLER_IDLE_THRESHOLD) { // finger is above
        // north
        poseX = 2 * MOBILE_MOTION_CONTROLLER_SIZE;
        poseY = 0 * MOBILE_MOTION_CONTROLLER_SIZE;
        mainCharacterWalkingNorth = SDL_TRUE;
        mainCharacterWalkingSouth = SDL_FALSE;
      } else { // finger is at vertical center as well
        // rest
        poseX = 0 * MOBILE_MOTION_CONTROLLER_SIZE;
        poseY = 0 * MOBILE_MOTION_CONTROLLER_SIZE;
        mainCharacterWalkingNorth = SDL_FALSE;
        mainCharacterWalkingSouth = SDL_FALSE;
      }
    }
    mobileMotionControllerPose.x = poseX;
    mobileMotionControllerPose.y = poseY;

  }
}


void Main_HandleKeyboardEvent(struct Nav_Context *ctx, struct Nav_KeyboardEvent *event) {
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


void Main_Destroy() {
  logInfo("Main: destroying.");
  Res_ReleaseTexture(optionsButtonTexture);
  Res_ReleaseTexture(mainCharacterTexture);
  if (mobileMotionControllerVisible) {
    Res_ReleaseTexture(mobileMotionControllerTexture);
  }
  Res_ReleaseTexture(mapCellTextures);
  Res_ReleaseTexture(minimapTexture);
  SDL_free(map);
  Aud_Unload(optionsButtonPress);
}


struct Nav_Screen* Main_GetScreen() {
  static struct Nav_Screen *instance = NULL;
  if (instance == NULL) {
    instance = malloc(sizeof(struct Nav_Screen));
    instance->init = Main_Init;
    instance->updateModel = Main_UpdateModel;
    instance->render = Main_Render;
    instance->handleClickTap = Main_HandleClickTap;
    instance->handleFingerEvent = Main_HandleFingerEvent;
    instance->handleKeyboardEvent = Main_HandleKeyboardEvent;
    instance->destroy = Main_Destroy;
  }
  return instance;
}
