#include "core/log.h"
#include "core/positioning.h"
#include "core/resources.h"
#include "screens/world/components/battle-trigger.h"
#include "screens/battle/battle.h"

typedef enum {
  BATTLE_TRIGGER_STATE_IDLE,
  BATTLE_TRIGGER_STATE_LOW_LEVEL_ENEMIES,
  BATTLE_TRIGGER_STATE_SIMILAR_LEVEL_ENEMIES,
  BATTLE_TRIGGER_STATE_HIGH_LEVEL_ENEMIES
} TriggerState;

static TriggerState state = BATTLE_TRIGGER_STATE_IDLE;
static Uint64 timeSinceLastCheck = 0;
static Pos_AnchoredElement pos;
static SDL_Rect rect;
static SDL_Rect pose;
static SDL_Texture *texture;

#define ENEMY_AREA_X 500
#define ENEMY_AREA_Y 100
#define ENEMY_AREA_WIDTH 2000
#define ENEMY_AREA_HEIGHT 1000


static void startBattle() {
  logInfo("BattleTrigger: battle time!");
  Game_GoTo(Battle_GetScreen());
}


static void handleUserInteraction() {
  if (state == BATTLE_TRIGGER_STATE_LOW_LEVEL_ENEMIES) {
    logInfo("BattleTrigger: player accepted low level battle.");
    startBattle();
  } else if (state == BATTLE_TRIGGER_STATE_SIMILAR_LEVEL_ENEMIES) {
    logInfo("BattleTrigger: player suppressed similar level battle.");
    state = BATTLE_TRIGGER_STATE_IDLE;
    timeSinceLastCheck = 0;
  } else if (state == BATTLE_TRIGGER_STATE_HIGH_LEVEL_ENEMIES) {
    logInfo("BattleTrigger: player advanced the start of a high level battle.");
    startBattle();
  }
}


void World_BattleTrigger_Init(Game_Context *ctx) {
  logInfo("BattleTrigger: initializing.");

  state = BATTLE_TRIGGER_STATE_IDLE;
  timeSinceLastCheck = 0;

  pos.anchors = POS_ANCHOR_BOTTOM | POS_ANCHOR_CENTER_LEFT;
  pos.width = 100;
  pos.height = 100;
  pos.anchorBottom = 0;
  pos.anchorCenterLeft = -50;
  pose.x = 0;
  pose.y = 0;
  pose.w = 100;
  pose.h = 100;
  texture = Res_LoadTexture(ctx, "battle-trigger.png");
}

void World_BattleTrigger_UpdateModel(Uint64 elapsedTime, Sint32 playerPosX, Sint32 playerPosY) {
  timeSinceLastCheck += elapsedTime;

  // if the trigger is in IDLE state,
  // the elapsedTime since the last check is greater than a specific period
  // and the player is in the trigger area
  // then pick a random number between 0-99 and apply the following logic:
  // if the number is 80-90 then set the state to BATTLE_TRIGGER_STATE_LOW_LEVEL_ENEMIES
  // if the number is 91-95 then set the state to BATTLE_TRIGGER_STATE_SIMILAR_LEVEL_ENEMIES
  // if the number is 96-99 then set the state to BATTLE_TRIGGER_STATE_HIGH_LEVEL_ENEMIES
  // the next step for this logic is to consider the player's level and the area's level

  if (state == BATTLE_TRIGGER_STATE_IDLE &&
      timeSinceLastCheck > 2000 &&
      playerPosX >= ENEMY_AREA_X &&
      playerPosX <= ENEMY_AREA_X + ENEMY_AREA_WIDTH &&
      playerPosY >= ENEMY_AREA_Y &&
      playerPosY <= ENEMY_AREA_Y + ENEMY_AREA_HEIGHT) {
    logInfo("BattleTrigger: checking.");
    timeSinceLastCheck = 0;
    Uint8 random = rand() % 100;
    if (random > 80) {
      if (random <= 90) {
        logInfo("BattleTrigger: low level enemies.");
        state = BATTLE_TRIGGER_STATE_LOW_LEVEL_ENEMIES;
      } else if (random < 96) {
        logInfo("BattleTrigger: similar level enemies.");
        state = BATTLE_TRIGGER_STATE_SIMILAR_LEVEL_ENEMIES;
      } else {
        logInfo("BattleTrigger: high level enemies.");
        state = BATTLE_TRIGGER_STATE_HIGH_LEVEL_ENEMIES;
      }
    } else {
      logInfo("BattleTrigger: no enemies.");
    }
  }

  // if the state is similar or high level enemies
  // and the elapsedTime since the last check is greater than 3 seconds
  // then it is battle time!
  if ((state == BATTLE_TRIGGER_STATE_SIMILAR_LEVEL_ENEMIES || state == BATTLE_TRIGGER_STATE_HIGH_LEVEL_ENEMIES) &&
      timeSinceLastCheck > 3000) {
    startBattle();
  }

  // if the state is low level enemies
  // and the elapsedTime since the last check is greater than 3 seconds
  // then set the state to IDLE
  if (state == BATTLE_TRIGGER_STATE_LOW_LEVEL_ENEMIES && timeSinceLastCheck > 3000) {
    logInfo("BattleTrigger: player ignored low level battle.");
    timeSinceLastCheck = 0;
    state = BATTLE_TRIGGER_STATE_IDLE;
  }

}


void World_BattleTrigger_Render(Game_Context *ctx) {
  // if the state is not IDLE then render the battle trigger
  if (state != BATTLE_TRIGGER_STATE_IDLE) {
    rect = Pos_CalcAnchored(&pos);

    // set the pose based on the enemy level
    switch (state) {
    case BATTLE_TRIGGER_STATE_LOW_LEVEL_ENEMIES:
      pose.x = 0;
      break;
    case BATTLE_TRIGGER_STATE_SIMILAR_LEVEL_ENEMIES:
      pose.x = 100;
      break;
    case BATTLE_TRIGGER_STATE_HIGH_LEVEL_ENEMIES:
      pose.x = 200;
      break;
    default:
      break;
    }

    if (SDL_RenderCopy(ctx->renderer, texture, &pose, &rect) != 0) {
      logError("BattleTrigger: failed to render: %s %s", SDL_GetError());
      exit(1);
    }
  }
}


void World_BattleTrigger_HandleClickTap(Game_Context *ctx, Input_ClickTap *pos) {
  if (state != BATTLE_TRIGGER_STATE_IDLE && Pos_IsInside(&rect, pos)) {
    handleUserInteraction();
  }
}


void World_BattleTrigger_HandleKeyboardEvent(Game_Context *ctx, Input_KeyboardEvent *event) {
  switch (event->type) {
  case INPUT_KEYBOARD_EVENT_TYPE_DOWN:
    switch (event->key) {
      case SDLK_SPACE:
        handleUserInteraction();
        break;
      default:
        break;
    }
    break;
  }
}


void World_BattleTrigger_Destroy() {
  logInfo("BattleTrigger: destroying.");
  Res_ReleaseTexture(texture);
  timeSinceLastCheck = 0;
  state = BATTLE_TRIGGER_STATE_IDLE;
}
