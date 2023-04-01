#ifndef SCREENS_WORLD_COMPONENTS_MAP_h
#define SCREENS_WORLD_COMPONENTS_MAP_h

#define MAP_ROWS 20
#define MAP_COLUMNS 60
#define MAP_CELL_SIZE 100

void World_Map_Init(Nav_Context *ctx);

SDL_bool World_Map_IsPassableTile(int row, int column);

void World_Map_Render(Nav_Context *ctx, Sint32 *playerPosX, Sint32 *playerPosY);

void World_Map_Destroy();

#endif
