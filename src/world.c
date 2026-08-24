#include "world.h"
#include "config.h"
#include <math.h>

World WorldInit(void)
{
    World w = { 0 };
    w.scrollX = 0.0f;
    w.scrollSpeed = BASE_SCROLL_SPEED;
    return w;
}

void WorldUpdate(World *world, float dt, bool speedBoost)
{
    world->scrollSpeed = speedBoost ? BASE_SCROLL_SPEED * SPEED_POTION_MULTIPLIER
                                     : BASE_SCROLL_SPEED;
    world->scrollX += world->scrollSpeed * dt;
}

void WorldDraw(const World *world, const GameTextures *tex)
{
    /* --- background: static, never scrolls. Just tile it once across the
     * canvas width so there's no seam. Only the ground/obstacles/coins move,
     * which is what actually sells the "running" feeling and is much
     * simpler to reason about than a parallax sky. --- */
    
    float bgScale = (float)CANVAS_HEIGHT / (float)tex->background.height;
    float bgTileW = tex->background.width * bgScale;

    for (float x = 0; x < CANVAS_WIDTH; x += bgTileW) {
        DrawTextureEx(tex->background, (Vector2){ x, 0 }, 0.0f, bgScale, WHITE);
    }

    /* --- ground strip: tiled at native size along the bottom --- */
    float groundOffset = fmodf(world->scrollX, (float)tex->ground.width);
    for (float x = -groundOffset; x < CANVAS_WIDTH; x += tex->ground.width) {
        for (float y = GROUND_Y; y < CANVAS_HEIGHT; y += tex->ground.height) {
            DrawTextureV(tex->ground, (Vector2){ x, y }, WHITE);
        }
    }
}
