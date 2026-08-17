#ifndef TEXTURES_H
#define TEXTURES_H

#include "raylib.h"

/* Every sprite the game uses, loaded once at startup and unloaded once at
 * shutdown. Passing this struct around (instead of loading textures inside
 * random functions) means there is exactly ONE place that touches the disk. */

typedef struct GameTextures {
    Texture2D background;
    Texture2D ground;
    Texture2D block;
    Texture2D coin;
    Texture2D catFront;
    Texture2D catSide;
    Texture2D catJump;
    Texture2D potionShield;
    Texture2D potionSpeed;
    Texture2D potionGravity;
    Texture2D dreadwing;
} GameTextures;

GameTextures LoadGameTextures(void);
void UnloadGameTextures(GameTextures *tex);

#endif 
