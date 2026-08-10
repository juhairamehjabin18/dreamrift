#ifndef GAME_H
#define GAME_H

#include "raylib.h"
#include "textures.h"
#include "world.h"
#include "player.h"
#include "obstacle.h"
#include "coin.h"
#include "potion.h"
#include "enemy.h"

typedef enum { SCREEN_MENU, SCREEN_PLAYING, SCREEN_PAUSED, SCREEN_GAMEOVER } GameScreen;

typedef struct GameState {
    GameScreen screen;

    World world;
    Player player;
    ObstacleField obstacles;
    CoinField coins;
    PotionSystem potions;
    EnemySystem enemies;

    int score;
    int highScore;
} GameState;

GameState GameInit(void);
void GameRestart(GameState *g);   /* keeps highScore, resets everything else */
void GameUpdate(GameState *g, float dt);
void GameDraw(const GameState *g, const GameTextures *tex);

#endif /* GAME_H */