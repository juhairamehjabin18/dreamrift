// ============================================================
//  main.c  --  DREAMRIFT entry point
//  Ties together: background.c, world.c (obstacles+coins),
//  game.c (player + globals)
//
//  COMPILE (Linux):
//    gcc -o dreamrift main.c game.c world.c background.c -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
// ============================================================
#include "game.h"

static int   score        = 0;
static bool  gameOver      = false;
static float obsTimer      = 0.0f;
static float coinTimer     = 0.0f;

static void ResetGame(void) {
    for (int i = 0; i < MAX_OBS; i++)   obs[i].alive   = false;
    for (int i = 0; i < MAX_COINS; i++) coins[i].alive = false;
    PlayerInit();
    score      = 0;
    gameOver   = false;
    obsTimer   = 0.0f;
    coinTimer  = 0.0f;
    bgScroll   = 0.0f;
}

static bool CheckCollision(void) {
    Rectangle prect = PlayerRect();
    for (int i = 0; i < MAX_OBS; i++) {
        if (obs[i].alive && CheckCollisionRecs(prect, obs[i].rect)) return true;
    }
    return false;
}

static void CheckCoins(void) {
    Rectangle prect = PlayerRect();
    for (int i = 0; i < MAX_COINS; i++) {
        if (!coins[i].alive) continue;
        Rectangle crect = { coins[i].pos.x - 12, coins[i].pos.y - 12, 24, 24 };
        if (CheckCollisionRecs(prect, crect)) {
            coins[i].alive = false;
            score++;
        }
    }
}

int main(void) {
    InitWindow(SCREEN_W, SCREEN_H, "DREAMRIFT");
    SetTargetFPS(60);
    srand((unsigned int)time(NULL));

    ResetGame();

    while (!WindowShouldClose()) {
        float dt = GetFrameTime();

        if (!gameOver) {
            // ---------- Update ----------
            bgScroll += scrollSpd * dt;

            PlayerUpdate(dt);
            ObsUpdate(dt);
            CoinUpdate(dt);

            obsTimer += dt;
            if (obsTimer > 1.4f) { obsTimer = 0.0f; ObsSpawn(); }

            coinTimer += dt;
            if (coinTimer > 0.9f) {
                coinTimer = 0.0f;
                CoinSpawn(SCREEN_W + 40, RandF(GROUND_Y - 220, GROUND_Y - 60));
            }

            CheckCoins();
            if (CheckCollision()) gameOver = true;
        } else {
            if (IsKeyPressed(KEY_ENTER)) ResetGame();
        }

        // ---------- Draw ----------
        BeginDrawing();
        ClearBackground(BLACK);

        BackgroundDraw();
        ObsDraw();
        CoinDraw();
        PlayerDraw();

        DrawText(TextFormat("SCORE: %d", score), 30, 20, 26, WHITE);
        DrawText("A/D: move   SHIFT: run   SPACE: jump   G: flip gravity",
                 30, 54, 16, Fade(WHITE, 0.6f));

        if (gameOver) {
            DrawRectangle(0, 0, SCREEN_W, SCREEN_H, Fade(BLACK, 0.55f));
            DrawText("GAME OVER", SCREEN_W/2 - 110, SCREEN_H/2 - 30, 40, WHITE);
            DrawText("Press ENTER to restart", SCREEN_W/2 - 110, SCREEN_H/2 + 20, 20, Fade(WHITE, 0.8f));
        }

        EndDrawing();
    }

    CloseWindow();
    return 0;
}