#ifndef WORLD_H
#define WORLD_H

#include "raylib.h"
#include "textures.h"

typedef struct World {
    float scrollX;       /* accumulated scroll distance, used to offset tiles */
    float scrollSpeed;   /* current speed, changes with the Speed Potion */
} World;

World WorldInit(void);
void WorldUpdate(World *world, float dt, bool speedBoost);
void WorldDraw(const World *world, const GameTextures *tex);

#endif 
