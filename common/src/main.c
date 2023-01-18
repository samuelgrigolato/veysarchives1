#include "platform.h"
#include <SDL.h>
#include <SDL_image.h>
#include "core/log.h"
#include "core/navigation.h"
#include "core/positioning.h"
#include "core/audio.h"
#include "screens/home.h"


void logDisplayInfo(SDL_Window *window) {
  int h, w;
  SDL_GL_GetDrawableSize(window, &w, &h);
  logInfo("DrawableSize: %dx%d", w, h);
  SDL_GetWindowSize(window, &w, &h);
  logInfo("WindowSize: %dx%d", w, h);
  SDL_GetWindowSizeInPixels(window, &w, &h);
  logInfo("WindowSizeInPixels: %dx%d", w, h);
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

  Nav_Init(renderer, window, Home_GetScreen());

  while (running) {
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT) {
        running = 0;
      } else if (event.type == SDL_MOUSEBUTTONUP) {
        if (event.button.button == SDL_BUTTON_LEFT) {
          struct Nav_ClickTap pos;
          pos.x = event.button.x;
          pos.y = event.button.y;
          Nav_HandleClickTap(&pos);
        }
      }
    }
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(renderer);
    Nav_Render();
    SDL_RenderPresent(renderer);
    SDL_Delay(100);
  }

  SDL_Quit();
  logInfo("Gracefully exited.");

  return 0;
}
