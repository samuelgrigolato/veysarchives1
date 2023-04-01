#include "core/log.h"
#include "core/resources.h"


SDL_Texture* Res_LoadTexture(Nav_Context *ctx, char *filePath) {
  char fullImagePath[300];
  sprintf(fullImagePath, "%s%s%s", ASSETS_BASE_DIR, "assets/", filePath);
  SDL_Texture *texture = IMG_LoadTexture(ctx->renderer, fullImagePath);
  if (texture == NULL) {
    logError("Resources: failed to load texture '%s': %s", filePath, SDL_GetError());
    exit(1);
  }
  return texture;
}


void Res_ReleaseTexture(SDL_Texture *texture) {
  SDL_DestroyTexture(texture);
}


char* Res_ReadFully(char *filePath, Uint16 chunkSize) {
  char fullFilePath[300];
  sprintf(fullFilePath, "%s%s%s", ASSETS_BASE_DIR, "assets/", filePath);
  SDL_RWops* file = SDL_RWFromFile(fullFilePath, "r");
  if (file == NULL) {
    logError("Resources: failed to load file '%s': %s", filePath, SDL_GetError());
    exit(1);
  }
  char *result = NULL;
  Sint32 resultSize = 0;

  char *chunk = malloc(sizeof(char) * chunkSize);
  Sint32 chunkReadElements = 0;

  for (;;) {
    chunkReadElements = SDL_RWread(file, chunk, sizeof(char), chunkSize);
    if (chunkReadElements < 0) {
      logError("Resources: failed to load chunk from '%s': %s", SDL_GetError());
      exit(1);
    } else if (chunkReadElements == 0) {
      logInfo("Resources: EOF reached");
      break;
    } else {

      // allocate enough space for the previously loaded data plus the current chunk plus the NULL terminator
      char *largerResult = malloc(sizeof(char) * (resultSize + chunkReadElements + 1));

      // copy the previosuly loaded data and free the old buffer
      if (resultSize > 0) {
        SDL_memcpy(largerResult, result, sizeof(char) * resultSize);
        SDL_free(result);
      }

      // copy the current chunk
      SDL_memcpy(largerResult + (sizeof(char) * resultSize), chunk, sizeof(char) * chunkReadElements);

      // set the NULL terminator
      largerResult[sizeof(char) * (resultSize + chunkReadElements)] = 0;

      result = largerResult;
      resultSize += chunkReadElements;

      logInfo("Resources: chunk of %d elements, resultSize=%d", chunkReadElements, resultSize);
    }
  }

  return result;
}
