#include "core/log.h"
#include "core/positioning.h"
#include "core/game.h"
#include "core/resources.h"
#include "core/audio.h"
#include "screens/battle/battle.h"
#include "screens/world/world.h"


static SDL_Texture *backgroundTexture;
static Aud_SoundID backgroundMusic;
static Aud_EntryID backgroundMusicEntry;

static Pos_AnchoredElement enemy1Pos;
static SDL_Rect enemy1Rect;
static SDL_Rect enemy1Pose;
static Pos_AnchoredElement enemy2Pos;
static SDL_Rect enemy2Rect;
static SDL_Rect enemy2Pose;
static SDL_Texture *enemyTexture;

static Pos_AnchoredElement playerPos;
static SDL_Rect playerRect;
static SDL_Texture *playerTexture;
static SDL_Rect playerPose;


void Battle_Init(Game_Context *ctx) {
  logInfo("Battle: initializing.");

  backgroundTexture = Res_LoadTexture(ctx, "battle-background.png");
  backgroundMusic = Aud_Load("battle-background.wav");
  backgroundMusicEntry = Aud_FadeInAndRepeat(backgroundMusic);

  enemy1Pos.anchors = POS_ANCHOR_TOP | POS_ANCHOR_CENTER_LEFT;
  enemy1Pos.width = 150;
  enemy1Pos.height = 150;
  enemy1Pos.anchorTop = 305;
  enemy1Pos.anchorCenterLeft = -300;
  enemy1Pose.w = 100;
  enemy1Pose.h = 100;
  enemy1Pose.x = 0;
  enemy1Pose.y = 0;
  enemy2Pos.anchors = POS_ANCHOR_TOP | POS_ANCHOR_CENTER_LEFT;
  enemy2Pos.width = 150;
  enemy2Pos.height = 150;
  enemy2Pos.anchorTop = 505;
  enemy2Pos.anchorCenterLeft = -350;
  enemy2Pose.w = 100;
  enemy2Pose.h = 100;
  enemy2Pose.x = 0;
  enemy2Pose.y = 0;
  enemyTexture = Res_LoadTexture(ctx, "goblin.png");

  playerPos.anchors = POS_ANCHOR_TOP | POS_ANCHOR_CENTER_LEFT;
  playerPos.width = 100;
  playerPos.height = 100;
  playerPos.anchorTop = 455;
  playerPos.anchorCenterLeft = 350;
  playerPose.w = 100;
  playerPose.h = 100;
  playerPose.x = 100;
  playerPose.y = 300;
  playerTexture = Res_LoadTexture(ctx, "main-character.png");

}


void Battle_UpdateModel(Uint64 elapsedTime) {
}


void Battle_Render(Game_Context *ctx) {

  SDL_Rect background = Pos_CalcCover();
  if (SDL_RenderCopy(ctx->renderer, backgroundTexture, NULL, &background) != 0) {
    logError("Battle: failed to render background: %s %s", SDL_GetError());
    exit(1);
  }

  enemy1Rect = Pos_CalcAnchored(&enemy1Pos);
  if (SDL_RenderCopy(ctx->renderer, enemyTexture, &enemy1Pose, &enemy1Rect) != 0) {
    logError("Battle: failed to render enemy1: %s %s", SDL_GetError());
    exit(1);
  }

  enemy2Rect = Pos_CalcAnchored(&enemy2Pos);
  if (SDL_RenderCopy(ctx->renderer, enemyTexture, &enemy2Pose, &enemy2Rect) != 0) {
    logError("Battle: failed to render enemy2: %s %s", SDL_GetError());
    exit(1);
  }

  playerRect = Pos_CalcAnchored(&playerPos);
  if (SDL_RenderCopy(ctx->renderer, playerTexture, &playerPose, &playerRect) != 0) {
    logError("Battle: failed to render player: %s %s", SDL_GetError());
    exit(1);
  }

}


void Battle_HandleClickTap(Game_Context *ctx, Input_ClickTap *pos) {
}


void Battle_HandleFingerEvent(Game_Context *ctx, Input_FingerEvent *event) {
  if (event->type == INPUT_FINGER_EVENT_TYPE_DOWN) {
    logInfo("Battle: player tapped.");
    Aud_FadeOutAndStop(backgroundMusicEntry);
    Game_GoTo(World_GetScreen());
  }
}


void Battle_HandleKeyboardEvent(Game_Context *ctx, Input_KeyboardEvent *event) {
  switch (event->type) {
  case INPUT_KEYBOARD_EVENT_TYPE_DOWN:
    switch (event->key) {
      case SDLK_SPACE:
        logInfo("Battle: player pressed space.");
        Aud_FadeOutAndStop(backgroundMusicEntry);
        Game_GoTo(World_GetScreen());
        break;
      default:
        break;
    }
    break;
  }
}


void Battle_Destroy() {
  logInfo("Battle: destroying.");
  Res_ReleaseTexture(backgroundTexture);
  Res_ReleaseTexture(enemyTexture);
  Res_ReleaseTexture(playerTexture);
  Aud_Unload(backgroundMusic);
}


Game_Screen* Battle_GetScreen() {
  static Game_Screen *instance = NULL;
  if (instance == NULL) {
    instance = malloc(sizeof(Game_Screen));
    instance->init = Battle_Init;
    instance->updateModel = Battle_UpdateModel;
    instance->render = Battle_Render;
    instance->handleClickTap = Battle_HandleClickTap;
    instance->handleFingerEvent = Battle_HandleFingerEvent;
    instance->handleKeyboardEvent = Battle_HandleKeyboardEvent;
    instance->destroy = Battle_Destroy;
  }
  return instance;
}
