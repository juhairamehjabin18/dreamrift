#ifndef ENEMY_H
#define ENEMY_H

#include "raylib.h"
#include "textures.h"
#include "config.h"
#include "player.h"

typedef struct Enemy {
    float x;
    float baseY;
    float age;   
    bool active;
} Enemy;

typedef struct EnemySystem {
    Enemy items[MAX_ENEMIES];
    float spawnTimer;
} EnemySystem;

EnemySystem EnemySystemInit(void);
void EnemySystemUpdate(EnemySystem *sys, float dt, float scrollDelta);
void EnemySystemCheckDamage(EnemySystem *sys, Player *p);
void EnemySystemDraw(const EnemySystem *sys, const GameTextures *tex);

#endif /* ENEMY_H */
