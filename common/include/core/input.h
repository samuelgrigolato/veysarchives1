#ifndef CORE_INPUT_h
#define CORE_INPUT_h

#include <SDL.h>

typedef struct {
  Sint32 x;
  Sint32 y;
} Input_ClickTap;

typedef enum {
  INPUT_FINGER_EVENT_TYPE_DOWN = 1,
  INPUT_FINGER_EVENT_TYPE_MOTION = 2,
  INPUT_FINGER_EVENT_TYPE_UP = 3
} Input_FingerEventType;

typedef struct {
  Input_FingerEventType type;
  float nx; // 0..1
  float ny; // 0..1
} Input_FingerEvent;

typedef enum {
  INPUT_KEYBOARD_EVENT_TYPE_DOWN = 1,
  INPUT_KEYBOARD_EVENT_TYPE_UP = 2
} Input_KeyboardEventType;

typedef struct {
  Input_KeyboardEventType type;
  SDL_Keycode key;
} Input_KeyboardEvent;

#endif
