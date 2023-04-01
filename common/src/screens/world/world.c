#include "core/log.h"
#include "core/navigation.h"
#include "core/positioning.h"
#include "core/resources.h"
#include "core/audio.h"
#include "screens/world/components/map.h"
#include "screens/world/components/main-character.h"
#include "screens/world/components/options-button.h"
#include "screens/world/components/mobile-motion-controller.h"
#include "screens/world/world.h"
#include "screens/home.h"


static Sint32 playerPosX = 100;
static Sint32 playerPosY = 100;


void World_Init(Nav_Context *ctx) {
  logInfo("World: initializing.");
  World_MainCharacter_Init(ctx);
  World_OptionsButton_Init(ctx);
  World_MobileMotionController_Init(ctx);
  World_Map_Init(ctx);
}


void World_UpdateModel(Uint64 elapsedTime) {
  World_MainCharacter_UpdateModel(elapsedTime, &playerPosX, &playerPosY);
}


void World_Render(Nav_Context *ctx) {
  World_Map_Render(ctx, &playerPosX, &playerPosY);
  World_MainCharacter_Render(ctx);
  World_OptionsButton_Render(ctx);
  World_MobileMotionController_Render(ctx);
}


void World_HandleClickTap(Nav_Context *ctx, Input_ClickTap *pos) {
  World_OptionsButton_HandleClickTap(ctx, pos);
}


void World_HandleFingerEvent(Nav_Context *ctx, Input_FingerEvent *event) {
  World_MobileMotionController_HandleFingerEvent(ctx, event);
}


void World_HandleKeyboardEvent(Nav_Context *ctx, Input_KeyboardEvent *event) {
  World_MainCharacter_HandleKeyboardEvent(ctx, event);
}


void World_Destroy() {
  logInfo("World: destroying.");
  World_MainCharacter_Destroy();
  World_Map_Destroy();
  World_OptionsButton_Destroy();
  World_MobileMotionController_Destroy();
}


Nav_Screen* World_GetScreen() {
  static Nav_Screen *instance = NULL;
  if (instance == NULL) {
    instance = malloc(sizeof(Nav_Screen));
    instance->init = World_Init;
    instance->updateModel = World_UpdateModel;
    instance->render = World_Render;
    instance->handleClickTap = World_HandleClickTap;
    instance->handleFingerEvent = World_HandleFingerEvent;
    instance->handleKeyboardEvent = World_HandleKeyboardEvent;
    instance->destroy = World_Destroy;
  }
  return instance;
}
