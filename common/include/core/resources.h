#ifndef CORE_RESOURCES_h
#define CORE_RESOURCES_h

#include "platform.h"
#include <SDL.h>
#include <SDL_image.h>
#include "core/game.h"

SDL_Texture* Res_LoadTexture(Game_Context *ctx, char *filePath);

void Res_ReleaseTexture(SDL_Texture *texture);

char* Res_ReadFully(char *filePath, Uint16 chunkSize);

#endif
