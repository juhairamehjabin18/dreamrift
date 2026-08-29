#ifndef POTION_H
#define POTION_H

#include "raylib.h"
#include "textures.h"
#include "config.h"
#include "player.h"

typedef struct PotionPickup {
    float x;
    PotionType type;   /* SHIELD, SPEED or GRAVITY - never NONE for a spawned pickup */
    bool active;
} PotionPickup;

typedef struct PotionSystem {
    PotionPickup items[MAX_POTIONS_ON_SCREEN];
    float spawnTimer;
} PotionSystem;

PotionSystem PotionSystemInit(void);
void PotionSystemUpdate(PotionSystem *sys, float dt, float scrollDelta);
void PotionSystemCollect(PotionSystem *sys, Player *p);   /* applies effect on contact */
void PotionSystemDraw(const PotionSystem *sys, const GameTextures *tex, int gravityDir);

#endif /* POTION_H */
