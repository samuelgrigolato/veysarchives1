#include "core/log.h"
#include "core/resources.h"


SDL_Texture* Res_LoadTexture(struct Nav_Context *ctx, char *filePath) {
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
