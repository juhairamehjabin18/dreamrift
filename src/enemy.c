#include "enemy.h"
#include <math.h>

/* baseY is the CENTER of the flap. The sine wave swings +/- ENEMY_FLAP_HEIGHT
 * around it, so picking this band (plus the clamp below) guarantees the bat
 * regularly dips all the way down to ground level, where the cat actually is. */
#define ENEMY_BAND_TOP    210.0f
#define ENEMY_BAND_BOTTOM 250.0f

EnemySystem EnemySystemInit(void)
{
    EnemySystem sys = { 0 };
    sys.spawnTimer = ENEMY_SPAWN_INTERVAL; /* first bat arrives after one full interval */
    return sys;
}

static void SpawnOne(EnemySystem *sys)
{
    for (int i = 0; i < MAX_ENEMIES; i++) {
        if (!sys->items[i].active) {
            sys->items[i].active = true;
            sys->items[i].x = CANVAS_WIDTH + 20.0f;
            sys->items[i].age = 0.0f;
            sys->items[i].baseY = (float)GetRandomValue((int)ENEMY_BAND_TOP, (int)ENEMY_BAND_BOTTOM);
            return;
        }
    }
}

void EnemySystemUpdate(EnemySystem *sys, float dt, float scrollDelta)
{
    sys->spawnTimer -= dt;
    if (sys->spawnTimer <= 0.0f) {
        SpawnOne(sys);
        sys->spawnTimer = ENEMY_SPAWN_INTERVAL;
    }

    for (int i = 0; i < MAX_ENEMIES; i++) {
        if (!sys->items[i].active) continue;
        sys->items[i].x -= scrollDelta;
        sys->items[i].age += dt;
        if (sys->items[i].x < -ENEMY_WIDTH) sys->items[i].active = false;
    }
}

static float EnemyCurrentY(const Enemy *e)
{
    /* "flaps up and down in place" - a simple sine wave around its base height */
    float y = e->baseY + sinf(e->age * ENEMY_FLAP_SPEED) * ENEMY_FLAP_HEIGHT;

    /* never let the bat's sprite sink visibly below the ground line */
    float lowestAllowed = GROUND_Y - ENEMY_HEIGHT;
    if (y > lowestAllowed) y = lowestAllowed;

    return y;
}

void EnemySystemCheckDamage(EnemySystem *sys, Player *p)
{
    if (!PlayerCanTakeDamage(p)) return;

    Rectangle pr = PlayerGetRect(p);
    for (int i = 0; i < MAX_ENEMIES; i++) {
        if (!sys->items[i].active) continue;

        Rectangle er = { sys->items[i].x, EnemyCurrentY(&sys->items[i]),
                          (float)ENEMY_WIDTH, (float)ENEMY_HEIGHT };

        if (CheckCollisionRecs(pr, er)) {
            PlayerTakeDamage(p, ENEMY_DAMAGE);
            break; /* one hit per frame is enough */
        }
    }
}

void EnemySystemDraw(const EnemySystem *sys, const GameTextures *tex)
{
    for (int i = 0; i < MAX_ENEMIES; i++) {
        if (!sys->items[i].active) continue;
        Vector2 pos = { sys->items[i].x, EnemyCurrentY(&sys->items[i]) };
        DrawTextureV(tex->dreadwing, pos, WHITE);
    }
}
