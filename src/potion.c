#include "potion.h"

#define POTION_SIZE 12

static float RandomInterval(void)
{
    return POTION_SPAWN_MIN + (float)GetRandomValue(0, 1000) / 1000.0f *
           (POTION_SPAWN_MAX - POTION_SPAWN_MIN);
}

static PotionType RandomPotionType(void)
{
    int r = GetRandomValue(0, 2);
    if (r == 0) return POTION_SHIELD;
    if (r == 1) return POTION_SPEED;
    return POTION_GRAVITY;
}

static float PotionGetY(int gravityDir)
{
    return (gravityDir == 1) ? (GROUND_Y - 70.0f) : (CEILING_Y + 70.0f);
}

PotionSystem PotionSystemInit(void)
{
    PotionSystem sys = { 0 };
    sys.spawnTimer = RandomInterval();
    return sys;
}

static bool SpawnOne(PotionSystem *sys)
{
    for (int i = 0; i < MAX_POTIONS_ON_SCREEN; i++) {
        if (!sys->items[i].active) {
            sys->items[i].active = true;
            sys->items[i].type = RandomPotionType();
            sys->items[i].x = CANVAS_WIDTH + 20.0f;
            return true;
        }
    }
    return false;
}

void PotionSystemUpdate(PotionSystem *sys, float dt, float scrollDelta)
{
    sys->spawnTimer -= dt;
    if (sys->spawnTimer <= 0.0f) {
        SpawnOne(sys);
        sys->spawnTimer = RandomInterval();
    }

    for (int i = 0; i < MAX_POTIONS_ON_SCREEN; i++) {
        if (!sys->items[i].active) continue;
        sys->items[i].x -= scrollDelta;
        if (sys->items[i].x < -POTION_SIZE) sys->items[i].active = false;
    }
}

void PotionSystemCollect(PotionSystem *sys, Player *p)
{
    Rectangle pr = PlayerGetRect(p);
    for (int i = 0; i < MAX_POTIONS_ON_SCREEN; i++) {
        if (!sys->items[i].active) continue;

        float y = PotionGetY(p->gravityDir);
        Rectangle prect = { sys->items[i].x, y, (float)POTION_SIZE, (float)POTION_SIZE };

        if (CheckCollisionRecs(pr, prect)) {
            PlayerApplyPotion(p, sys->items[i].type);
            sys->items[i].active = false;
        }
    }
}

void PotionSystemDraw(const PotionSystem *sys, const GameTextures *tex, int gravityDir)
{
    for (int i = 0; i < MAX_POTIONS_ON_SCREEN; i++) {
        if (!sys->items[i].active) continue;

        Texture2D sprite;
        switch (sys->items[i].type) {
            case POTION_SHIELD:  sprite = tex->potionShield;  break;
            case POTION_SPEED:   sprite = tex->potionSpeed;   break;
            default:              sprite = tex->potionGravity; break;
        }

        float y = PotionGetY(gravityDir);
        DrawTextureV(sprite, (Vector2){ sys->items[i].x, y }, WHITE);
    }
}
