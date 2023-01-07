
#ifndef __INTELLISENSE__
#include "platform.h"
#endif

#include <SDL.h>
#include <SDL_image.h>
#include "core/log.h"


int main(int argc, char* argv[]) {
  if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
    logError("Error: %s\n", SDL_GetError());
    return -1;
  }

  logInfo("SDL initialized.");

  SDL_Window *window = SDL_CreateWindow("Veys's Archives: My Evil Doings",
    SDL_WINDOWPOS_UNDEFINED,
    SDL_WINDOWPOS_UNDEFINED,
    640, 480,
    SDL_WINDOW_OPENGL);
  if (window == NULL) {
    logError("Error: %s\n", SDL_GetError());
    return -1;
  }

  logInfo("Window created.");

  SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);
  if (renderer == NULL) {
    logError("Error: %s\n", SDL_GetError());
    return -1;
  }

  logInfo("Renderer created. Running!");

  int running = 1;
  SDL_Event event;

  while (running) {
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT) {
        running = 0;
      }
    }
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);
    SDL_Delay(100);
  }

  SDL_Quit();
  logInfo("Gracefully exited.");

  return 0;
}
