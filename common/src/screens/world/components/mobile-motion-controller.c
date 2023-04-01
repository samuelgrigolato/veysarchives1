#include "core/log.h"
#include "core/positioning.h"
#include "core/resources.h"
#include "screens/world/components/main-character.h"
#include "screens/world/components/mobile-motion-controller.h"


static SDL_bool visible = !DESKTOP;
static Pos_AnchoredElement pos;
static SDL_Rect rect;
static SDL_Texture *texture;
static SDL_Rect pose;
static SDL_bool active = SDL_FALSE;
#define MOBILE_MOTION_CONTROLLER_IDLE_THRESHOLD 75
#define MOBILE_MOTION_CONTROLLER_SIZE 400


void World_MobileMotionController_Init(Nav_Context *ctx) {
  logInfo("MobileMotionController: initializing.");
  if (visible) {
    pos.anchors = POS_ANCHOR_BOTTOM | POS_ANCHOR_LEFT;
    pos.width = MOBILE_MOTION_CONTROLLER_SIZE;
    pos.height = MOBILE_MOTION_CONTROLLER_SIZE;
    pos.anchorBottom = 50;
    pos.anchorLeft = 50;
    pose.w = MOBILE_MOTION_CONTROLLER_SIZE;
    pose.h = MOBILE_MOTION_CONTROLLER_SIZE;
    pose.x = MOBILE_MOTION_CONTROLLER_SIZE;
    pose.y = 0;
    texture = Res_LoadTexture(ctx, "mobile-motion-controller.png");
  }
}


void World_MobileMotionController_Render(Nav_Context *ctx) {
  if (visible) {
    rect = Pos_CalcAnchored(&pos);
    if (SDL_RenderCopy(ctx->renderer, texture, &pose, &rect) != 0) {
      logError("MobileMotionController: failed to render: %s %s", SDL_GetError());
      exit(1);
    }
  }
}


void World_MobileMotionController_HandleFingerEvent(Nav_Context *ctx, Nav_FingerEvent *event) {
  if (!visible) return;

  MaybeBool mainCharacterWalkingNorth = MaybeBool_EMPTY,
    mainCharacterWalkingSouth = MaybeBool_EMPTY,
    mainCharacterWalkingWest = MaybeBool_EMPTY,
    mainCharacterWalkingEast = MaybeBool_EMPTY;

  if (event->type == NAV_FINGER_EVENT_TYPE_DOWN &&
      Pos_IsFingerEventInside(&rect, event, ctx) &&
      active == SDL_FALSE) {

    active = SDL_TRUE;
    pose.x = 0;
    pose.y = 0;

  } else if (event->type == NAV_FINGER_EVENT_TYPE_UP &&
             active == SDL_TRUE) {

    active = SDL_FALSE;
    pose.x = 400;
    pose.y = 0;
    mainCharacterWalkingNorth = MaybeBool_FALSE;
    mainCharacterWalkingSouth = MaybeBool_FALSE;
    mainCharacterWalkingWest = MaybeBool_FALSE;
    mainCharacterWalkingEast = MaybeBool_FALSE;

  } else if (active == SDL_TRUE) { // NAV_FINGER_EVENT_MOTION

    Sint32 eventX = ctx->windowWidth * event->nx;
    Sint32 eventY = ctx->windowHeight * event->ny;
    Sint32 relativeX = eventX - (rect.x + (rect.w / 2));
    Sint32 relativeY = eventY - (rect.y + (rect.h / 2));

    Sint32 poseX, poseY;
    if (relativeX > MOBILE_MOTION_CONTROLLER_IDLE_THRESHOLD) { // finger is to the right
      mainCharacterWalkingWest = MaybeBool_FALSE;
      mainCharacterWalkingEast = MaybeBool_TRUE;
      if (relativeY > MOBILE_MOTION_CONTROLLER_IDLE_THRESHOLD) { // finger is below
        // southeast
        poseX = 3 * MOBILE_MOTION_CONTROLLER_SIZE;
        poseY = 1 * MOBILE_MOTION_CONTROLLER_SIZE;
        mainCharacterWalkingNorth = MaybeBool_FALSE;
        mainCharacterWalkingSouth = MaybeBool_TRUE;
      } else if (relativeY < -MOBILE_MOTION_CONTROLLER_IDLE_THRESHOLD) { // finger is above
        // northeast
        poseX = 2 * MOBILE_MOTION_CONTROLLER_SIZE;
        poseY = 1 * MOBILE_MOTION_CONTROLLER_SIZE;
        mainCharacterWalkingNorth = MaybeBool_TRUE;
        mainCharacterWalkingSouth = MaybeBool_FALSE;
      } else { // finger is at vertical center
        // east
        poseX = 3 * MOBILE_MOTION_CONTROLLER_SIZE;
        poseY = 0 * MOBILE_MOTION_CONTROLLER_SIZE;
        mainCharacterWalkingNorth = MaybeBool_FALSE;
        mainCharacterWalkingSouth = MaybeBool_FALSE;
      }
    } else if (relativeX < -MOBILE_MOTION_CONTROLLER_IDLE_THRESHOLD) { // finger is to the left
      mainCharacterWalkingWest = MaybeBool_TRUE;
      mainCharacterWalkingEast = MaybeBool_FALSE;
      if (relativeY > 50) { // finger is below
        // southwest
        poseX = 0 * MOBILE_MOTION_CONTROLLER_SIZE;
        poseY = 2 * MOBILE_MOTION_CONTROLLER_SIZE;
        mainCharacterWalkingNorth = MaybeBool_FALSE;
        mainCharacterWalkingSouth = MaybeBool_TRUE;
      } else if (relativeY < -MOBILE_MOTION_CONTROLLER_IDLE_THRESHOLD) { // finger is above
        // northwest
        poseX = 1 * MOBILE_MOTION_CONTROLLER_SIZE;
        poseY = 2 * MOBILE_MOTION_CONTROLLER_SIZE;
        mainCharacterWalkingNorth = MaybeBool_TRUE;
        mainCharacterWalkingSouth = MaybeBool_FALSE;
      } else { // finger is at vertical center
        // west
        poseX = 1 * MOBILE_MOTION_CONTROLLER_SIZE;
        poseY = 1 * MOBILE_MOTION_CONTROLLER_SIZE;
        mainCharacterWalkingNorth = MaybeBool_FALSE;
        mainCharacterWalkingSouth = MaybeBool_FALSE;
      }
    } else { // finger is at horizontal center
      mainCharacterWalkingWest = MaybeBool_FALSE;
      mainCharacterWalkingEast = MaybeBool_FALSE;
      if (relativeY > MOBILE_MOTION_CONTROLLER_IDLE_THRESHOLD) { // finger is below
        // south
        poseX = 0 * MOBILE_MOTION_CONTROLLER_SIZE;
        poseY = 1 * MOBILE_MOTION_CONTROLLER_SIZE;
        mainCharacterWalkingNorth = MaybeBool_FALSE;
        mainCharacterWalkingSouth = MaybeBool_TRUE;
      } else if (relativeY < -MOBILE_MOTION_CONTROLLER_IDLE_THRESHOLD) { // finger is above
        // north
        poseX = 2 * MOBILE_MOTION_CONTROLLER_SIZE;
        poseY = 0 * MOBILE_MOTION_CONTROLLER_SIZE;
        mainCharacterWalkingNorth = MaybeBool_TRUE;
        mainCharacterWalkingSouth = MaybeBool_FALSE;
      } else { // finger is at vertical center as well
        // rest
        poseX = 0 * MOBILE_MOTION_CONTROLLER_SIZE;
        poseY = 0 * MOBILE_MOTION_CONTROLLER_SIZE;
        mainCharacterWalkingNorth = MaybeBool_FALSE;
        mainCharacterWalkingSouth = MaybeBool_FALSE;
      }
    }
    pose.x = poseX;
    pose.y = poseY;

  }

  World_MainCharacter_SetWalkingDirections(mainCharacterWalkingNorth, mainCharacterWalkingSouth, mainCharacterWalkingWest, mainCharacterWalkingEast);

}


void World_MobileMotionController_Destroy() {
  if (visible) {
    Res_ReleaseTexture(texture);
  }
}
