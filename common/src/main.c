#include "platform.h"
#include <SDL.h>
#include <SDL_image.h>
#include "core/log.h"
#include "core/navigation.h"
#include "core/positioning.h"
#include "core/audio.h"
#include "core/input.h"
#include "screens/home.h"


static void logDisplayInfo(SDL_Window *window) {
  int h, w;
  SDL_GL_GetDrawableSize(window, &w, &h);
  logInfo("DrawableSize: %dx%d", w, h);
  SDL_GetWindowSize(window, &w, &h);
  logInfo("WindowSize: %dx%d", w, h);
  SDL_GetWindowSizeInPixels(window, &w, &h);
  logInfo("WindowSizeInPixels: %dx%d", w, h);
}


static void handleFingerEvent(SDL_Event *event) {
  Input_FingerEvent fingerEvent;
  switch (event->type) {
    case SDL_FINGERDOWN:
      fingerEvent.type = INPUT_FINGER_EVENT_TYPE_DOWN;
      break;
    case SDL_FINGERMOTION:
      fingerEvent.type = INPUT_FINGER_EVENT_TYPE_MOTION;
      break;
    case SDL_FINGERUP:
    default:
      fingerEvent.type = INPUT_FINGER_EVENT_TYPE_UP;
      break;
  }
  fingerEvent.nx = event->tfinger.x;
  fingerEvent.ny = event->tfinger.y;
  Nav_HandleFingerEvent(&fingerEvent);
}


static void handleKeyboardEvent(SDL_Event *event) {
  Input_KeyboardEvent keyboardEvent;
  switch (event->type) {
    case SDL_KEYDOWN:
      keyboardEvent.type = INPUT_KEYBOARD_EVENT_TYPE_DOWN;
      break;
    case SDL_FINGERUP:
    default:
      keyboardEvent.type = INPUT_KEYBOARD_EVENT_TYPE_UP;
      break;
  }
  keyboardEvent.key = event->key.keysym.sym;
  Nav_HandleKeyboardEvent(&keyboardEvent);
}


int main(int argc, char* argv[]) {
  if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
    logError("Error: %s\n", SDL_GetError());
    return -1;
  }

  logInfo("SDL initialized.");

  SDL_Window *window = SDL_CreateWindow("Veys's Archives: My Evil Doings",
    SDL_WINDOWPOS_UNDEFINED,
    SDL_WINDOWPOS_UNDEFINED,
    1600, 900,
    PLATFORM_SDL_FLAGS);
  if (window == NULL) {
    logError("Error: %s\n", SDL_GetError());
    return -1;
  }

  logDisplayInfo(window);

  logInfo("Window created.");

  SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);
  if (renderer == NULL) {
    logError("Error: %s\n", SDL_GetError());
    return -1;
  }

  logInfo("Renderer created.");

  Aud_Init();

  logInfo("Audio started. Running!");

  int running = 1;
  SDL_Event event;
  Uint64 lastFrameStartTime = SDL_GetTicks64();

  Nav_Init(renderer, window, Home_GetScreen());

  while (running) {
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT) {
        running = 0;
      } else if (event.type == SDL_MOUSEBUTTONUP) {
        if (event.button.button == SDL_BUTTON_LEFT) {
          Input_ClickTap pos;
          pos.x = event.button.x;
          pos.y = event.button.y;
          Nav_HandleClickTap(&pos);
        }
      } else if (event.type == SDL_FINGERDOWN ||
                 event.type == SDL_FINGERUP ||
                 event.type == SDL_FINGERMOTION) {
        handleFingerEvent(&event);
      } else if (event.type == SDL_KEYDOWN ||
                 event.type == SDL_KEYUP) {
        handleKeyboardEvent(&event);
      }
    }

    Uint64 thisFrameStartTime = SDL_GetTicks64();
    Uint64 elapsedTime = thisFrameStartTime - lastFrameStartTime;
    Nav_UpdateModel(elapsedTime);
    lastFrameStartTime= thisFrameStartTime;

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(renderer);
    Nav_Render();
    SDL_RenderPresent(renderer);

    Uint64 thisFrameEndTime = SDL_GetTicks64();
    Uint64 thisFrameDuration = thisFrameEndTime - thisFrameStartTime;
    if (thisFrameDuration < 25) {
      // make the minimum frame duration to be 25ms
      SDL_Delay(25 - thisFrameDuration);
    }
  }

  SDL_Quit();
  logInfo("Gracefully exited.");

  return 0;
}
