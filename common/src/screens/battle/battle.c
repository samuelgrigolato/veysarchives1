#include "core/log.h"
#include "core/game.h"
#include "screens/battle/battle.h"
#include "screens/world/world.h"


void Battle_Init(Game_Context *ctx) {
  logInfo("Battle: initializing.");
}


void Battle_UpdateModel(Uint64 elapsedTime) {
}


void Battle_Render(Game_Context *ctx) {
}


void Battle_HandleClickTap(Game_Context *ctx, Input_ClickTap *pos) {
}


void Battle_HandleFingerEvent(Game_Context *ctx, Input_FingerEvent *event) {
}


void Battle_HandleKeyboardEvent(Game_Context *ctx, Input_KeyboardEvent *event) {
  switch (event->type) {
  case INPUT_KEYBOARD_EVENT_TYPE_DOWN:
    switch (event->key) {
      case SDLK_SPACE:
        logInfo("Battle: player pressed space.");
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
