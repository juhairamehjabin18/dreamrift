#include "game.h"
#include "ui.h"

GameState GameInit(void)
{
    GameState g = { 0 };
    g.screen = SCREEN_MENU;
    g.world = WorldInit();
    g.player = PlayerInit();
    g.obstacles = ObstacleFieldInit();
    g.coins = CoinFieldInit();
    g.potions = PotionSystemInit();
    g.enemies = EnemySystemInit();
    g.score = 0;
    g.highScore = 0;
    return g;
}

void GameRestart(GameState *g)
{
    int keepHigh = g->highScore;
    g->world = WorldInit();
    PlayerReset(&g->player);
    g->obstacles = ObstacleFieldInit();
    g->coins = CoinFieldInit();
    g->potions = PotionSystemInit();
    g->enemies = EnemySystemInit();
    g->score = 0;
    g->highScore = keepHigh;
}

static void HandleScreenInput(GameState *g)
{
    switch (g->screen) {
        case SCREEN_MENU:
            if (IsKeyPressed(KEY_ENTER)) {
                GameRestart(g);
                g->screen = SCREEN_PLAYING;
            }
            break;
        case SCREEN_PLAYING:
            if (IsKeyPressed(KEY_P)) g->screen = SCREEN_PAUSED;
            break;
        case SCREEN_PAUSED:
            if (IsKeyPressed(KEY_P)) g->screen = SCREEN_PLAYING;
            break;
        case SCREEN_GAMEOVER:
            if (IsKeyPressed(KEY_R)) {
                GameRestart(g);
                g->screen = SCREEN_PLAYING;
            }
            break;
    }
}

static void UpdatePlaying(GameState *g, float dt)
{
    bool speedBoost = (g->player.activePotion == POTION_SPEED);
    WorldUpdate(&g->world, dt, speedBoost);
    float scrollDelta = g->world.scrollSpeed * dt;

    PlayerUpdate(&g->player, dt);
    ObstacleFieldResolveLanding(&g->obstacles, &g->player);
    ObstacleFieldUpdate(&g->obstacles, scrollDelta);
    CoinFieldUpdate(&g->coins, scrollDelta);
    PotionSystemUpdate(&g->potions, dt, scrollDelta);
    EnemySystemUpdate(&g->enemies, dt, scrollDelta);

    g->score += CoinFieldCollect(&g->coins, &g->player);
    g->score += ObstacleFieldCollectCoins(&g->obstacles, &g->player);
    PotionSystemCollect(&g->potions, &g->player);
    EnemySystemCheckDamage(&g->enemies, &g->player);

    if (g->player.health <= 0) {
        if (g->score > g->highScore) g->highScore = g->score;
        g->screen = SCREEN_GAMEOVER;
    }
}

void GameUpdate(GameState *g, float dt)
{
    HandleScreenInput(g);
    if (g->screen == SCREEN_PLAYING) UpdatePlaying(g, dt);
}

void GameDraw(const GameState *g, const GameTextures *tex)
{
    WorldDraw(&g->world, tex);
    ObstacleFieldDraw(&g->obstacles, tex);
    CoinFieldDraw(&g->coins, tex, g->player.gravityDir);
    PotionSystemDraw(&g->potions, tex, g->player.gravityDir);
    EnemySystemDraw(&g->enemies, tex);
    PlayerDraw(&g->player, tex);

    switch (g->screen) {
        case SCREEN_MENU:
            UIDrawMenu();
            break;
        case SCREEN_PLAYING:
            UIDrawHUD(&g->player, g->score, g->highScore);
            break;
        case SCREEN_PAUSED:
            UIDrawHUD(&g->player, g->score, g->highScore);
            UIDrawPaused();
            break;
        case SCREEN_GAMEOVER:
            UIDrawGameOver(g->score, g->highScore);
            break;
    }
}
