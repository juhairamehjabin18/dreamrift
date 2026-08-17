#include "textures.h"

/* NOTE: paths are relative to the folder the game .exe/binary is RUN from.
 * Our build scripts (Makefile / run instructions) always run the game from
 * the project root, so "assets/xxx.png" always resolves correctly on
 * Windows, macOS and Linux. */

GameTextures LoadGameTextures(void)
{
    GameTextures tex = { 0 };

    tex.background    = LoadTexture("assets/background.png");
    tex.ground        = LoadTexture("assets/ground.png");
    tex.block         = LoadTexture("assets/block.png");
    tex.coin          = LoadTexture("assets/coin.png");
    tex.catFront      = LoadTexture("assets/cat_front.png");
    tex.catSide       = LoadTexture("assets/cat_side.png");
    tex.catJump       = LoadTexture("assets/cat_jump.png");
    tex.potionShield  = LoadTexture("assets/potion_shield.png");
    tex.potionSpeed   = LoadTexture("assets/potion_speed.png");
    tex.potionGravity = LoadTexture("assets/potion_gravity.png");
    tex.dreadwing     = LoadTexture("assets/dreadwing.png");

    /* Point (nearest-neighbour) filtering keeps pixel art crisp when scaled */
    SetTextureFilter(tex.background,    TEXTURE_FILTER_POINT);
    SetTextureFilter(tex.ground,        TEXTURE_FILTER_POINT);
    SetTextureFilter(tex.block,         TEXTURE_FILTER_POINT);
    SetTextureFilter(tex.coin,          TEXTURE_FILTER_POINT);
    SetTextureFilter(tex.catFront,      TEXTURE_FILTER_POINT);
    SetTextureFilter(tex.catSide,       TEXTURE_FILTER_POINT);
    SetTextureFilter(tex.catJump,       TEXTURE_FILTER_POINT);
    SetTextureFilter(tex.potionShield,  TEXTURE_FILTER_POINT);
    SetTextureFilter(tex.potionSpeed,   TEXTURE_FILTER_POINT);
    SetTextureFilter(tex.potionGravity, TEXTURE_FILTER_POINT);
    SetTextureFilter(tex.dreadwing,     TEXTURE_FILTER_POINT);

    return tex;
}

void UnloadGameTextures(GameTextures *tex)
{
    UnloadTexture(tex->background);
    UnloadTexture(tex->ground);
    UnloadTexture(tex->block);
    UnloadTexture(tex->coin);
    UnloadTexture(tex->catFront);
    UnloadTexture(tex->catSide);
    UnloadTexture(tex->catJump);
    UnloadTexture(tex->potionShield);
    UnloadTexture(tex->potionSpeed);
    UnloadTexture(tex->potionGravity);
    UnloadTexture(tex->dreadwing);
}
