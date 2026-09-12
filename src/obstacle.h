#ifndef OBSTACLE_H
#define OBSTACLE_H

#include "raylib.h"
#include "textures.h"
#include "config.h"
#include "player.h"

typedef struct Obstacle {
    float x;
    bool active;
    bool hasCoin;       /* this block currently carries a coin on top of it */
    bool coinCollected; /* that coin has already been picked up */
} Obstacle;

typedef struct ObstacleField {
    Obstacle items[MAX_OBSTACLES];
} ObstacleField;

/* returns the y (top) of the obstacle - useful for coins that sit on top of one */
#define OBSTACLE_TOP_Y (GROUND_Y - OBSTACLE_SIZE)
#define OBSTACLE_COIN_Y (OBSTACLE_TOP_Y - COIN_SIZE - 4)

ObstacleField ObstacleFieldInit(void);
void ObstacleFieldUpdate(ObstacleField *field, float scrollDelta);
void ObstacleFieldResolveLanding(const ObstacleField *field, Player *p);
int  ObstacleFieldCollectCoins(ObstacleField *field, const Player *p); /* score gained this frame */
void ObstacleFieldDraw(const ObstacleField *field, const GameTextures *tex);

#endif /* OBSTACLE_H */
