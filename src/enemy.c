#include "enemy.h"


#define ENEMY_BAND_TOP    270.0f
#define ENEMY_BAND_BOTTOM 296.0f

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
    return e->baseY;
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
