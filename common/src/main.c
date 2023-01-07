
#ifndef __INTELLISENSE__
#include "platform.h"
#endif

#include <SDL.h>
#include <SDL_image.h>
#include "core/log.h"


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
    1366, 768,
    SDL_WINDOW_OPENGL | SDL_WINDOW_BORDERLESS | SDL_WINDOW_FULLSCREEN_DESKTOP);
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

  logInfo("Renderer created. Running!");

  int running = 1;
  SDL_Event event;

  SDL_Rect simpleSquare;
  simpleSquare.h = 50;
  simpleSquare.w = 50;
  simpleSquare.x = 50;
  simpleSquare.y = 50;

  while (running) {
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT) {
        running = 0;
      }
    }
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderFillRect(renderer, &simpleSquare);
    SDL_RenderPresent(renderer);
    SDL_Delay(100);
  }

  SDL_Quit();
  logInfo("Gracefully exited.");

  return 0;
}
