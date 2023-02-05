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

SDL_bool mobileMotionControllerVisible = !DESKTOP;
struct Pos_AnchoredElement mobileMotionControllerPos;
SDL_Rect mobileMotionControllerRect;
SDL_Texture *mobileMotionControllerTexture;
SDL_Rect mobileMotionControllerPose;
SDL_bool mobileMotionControllerActive = SDL_FALSE;
#define MOBILE_MOTION_CONTROLLER_IDLE_THRESHOLD 75
#define MOBILE_MOTION_CONTROLLER_SIZE 400


void Main_Init(struct Nav_Context *ctx) {
  logInfo("Main: initializing.");

  mainCharacterPos.anchors = POS_ANCHOR_TOP | POS_ANCHOR_CENTER_LEFT;
  mainCharacterPos.width = 100;
  mainCharacterPos.height = 100;
  mainCharacterPos.anchorTop = 400;
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

}


void Main_Render(struct Nav_Context *ctx) {

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

  if (event->type == NAV_FINGER_EVENT_DOWN &&
      Pos_IsFingerEventInside(&mobileMotionControllerRect, event, ctx) &&
      mobileMotionControllerActive == SDL_FALSE) {

    mobileMotionControllerActive = SDL_TRUE;
    mobileMotionControllerPose.x = 0;
    mobileMotionControllerPose.y = 0;

  } else if (event->type == NAV_FINGER_EVENT_UP &&
             mobileMotionControllerActive == SDL_TRUE) {

    mobileMotionControllerActive = SDL_FALSE;
    mobileMotionControllerPose.x = 400;
    mobileMotionControllerPose.y = 0;

  } else if (mobileMotionControllerActive == SDL_TRUE) { // NAV_FINGER_EVENT_MOTION

    Sint32 eventX = ctx->windowWidth * event->nx;
    Sint32 eventY = ctx->windowHeight * event->ny;
    Sint32 relativeX = eventX - (mobileMotionControllerRect.x + (mobileMotionControllerRect.w / 2));
    Sint32 relativeY = eventY - (mobileMotionControllerRect.y + (mobileMotionControllerRect.h / 2));

    Sint32 poseX, poseY;
    if (relativeX > MOBILE_MOTION_CONTROLLER_IDLE_THRESHOLD) { // finger is to the right
      if (relativeY > MOBILE_MOTION_CONTROLLER_IDLE_THRESHOLD) { // finger is below
        // southeast
        poseX = 3 * MOBILE_MOTION_CONTROLLER_SIZE;
        poseY = 1 * MOBILE_MOTION_CONTROLLER_SIZE;
      } else if (relativeY < -MOBILE_MOTION_CONTROLLER_IDLE_THRESHOLD) { // finger is above
        // northeast
        poseX = 2 * MOBILE_MOTION_CONTROLLER_SIZE;
        poseY = 1 * MOBILE_MOTION_CONTROLLER_SIZE;
      } else { // finger is at vertical center
        // east
        poseX = 3 * MOBILE_MOTION_CONTROLLER_SIZE;
        poseY = 0 * MOBILE_MOTION_CONTROLLER_SIZE;
      }
    } else if (relativeX < -MOBILE_MOTION_CONTROLLER_IDLE_THRESHOLD) { // finger is to the left
      if (relativeY > 50) { // finger is below
        // southwest
        poseX = 0 * MOBILE_MOTION_CONTROLLER_SIZE;
        poseY = 2 * MOBILE_MOTION_CONTROLLER_SIZE;
      } else if (relativeY < -MOBILE_MOTION_CONTROLLER_IDLE_THRESHOLD) { // finger is above
        // northwest
        poseX = 1 * MOBILE_MOTION_CONTROLLER_SIZE;
        poseY = 2 * MOBILE_MOTION_CONTROLLER_SIZE;
      } else { // finger is at vertical center
        // west
        poseX = 1 * MOBILE_MOTION_CONTROLLER_SIZE;
        poseY = 1 * MOBILE_MOTION_CONTROLLER_SIZE;
      }
    } else { // finger is at horizontal center
      if (relativeY > MOBILE_MOTION_CONTROLLER_IDLE_THRESHOLD) { // finger is below
        // south
        poseX = 0 * MOBILE_MOTION_CONTROLLER_SIZE;
        poseY = 1 * MOBILE_MOTION_CONTROLLER_SIZE;
      } else if (relativeY < -MOBILE_MOTION_CONTROLLER_IDLE_THRESHOLD) { // finger is above
        // north
        poseX = 2 * MOBILE_MOTION_CONTROLLER_SIZE;
        poseY = 0 * MOBILE_MOTION_CONTROLLER_SIZE;
      } else { // finger is at vertical center as well
        // rest
        poseX = 0 * MOBILE_MOTION_CONTROLLER_SIZE;
        poseY = 0 * MOBILE_MOTION_CONTROLLER_SIZE;
      }
    }
    mobileMotionControllerPose.x = poseX;
    mobileMotionControllerPose.y = poseY;

  }
}


void Main_Destroy() {
  logInfo("Main: destroying.");
  Res_ReleaseTexture(optionsButtonTexture);
  Res_ReleaseTexture(mainCharacterTexture);
  if (mobileMotionControllerVisible) {
    Res_ReleaseTexture(mobileMotionControllerTexture);
  }
  Aud_Unload(optionsButtonPress);
}


struct Nav_Screen* Main_GetScreen() {
  static struct Nav_Screen *instance = NULL;
  if (instance == NULL) {
    instance = malloc(sizeof(struct Nav_Screen));
    instance->init = Main_Init;
    instance->render = Main_Render;
    instance->handleClickTap = Main_HandleClickTap;
    instance->handleFingerEvent = Main_HandleFingerEvent;
    instance->destroy = Main_Destroy;
  }
  return instance;
}
