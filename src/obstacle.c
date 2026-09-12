#include "obstacle.h"

#define OBSTACLE_COIN_SCORE_VALUE 2
#define OBSTACLE_COIN_CHANCE 50   /* percent chance a given block carries a coin */

static float RandomGap(void)
{
    return OBSTACLE_MIN_GAP + (float)GetRandomValue(0, 1000) / 1000.0f *
           (OBSTACLE_MAX_GAP - OBSTACLE_MIN_GAP);
}

static bool RollHasCoin(void)
{
    return GetRandomValue(0, 100) < OBSTACLE_COIN_CHANCE;
}

ObstacleField ObstacleFieldInit(void)
{
    ObstacleField field = { 0 };
    float x = CANVAS_WIDTH + 100.0f;
    for (int i = 0; i < MAX_OBSTACLES; i++) {
        field.items[i].x = x;
        field.items[i].active = true;
        field.items[i].hasCoin = RollHasCoin();
        field.items[i].coinCollected = false;
        x += RandomGap();
    }
    return field;
}

void ObstacleFieldUpdate(ObstacleField *field, float scrollDelta)
{
    /* find the current right-most obstacle so a recycled one lines up after it */
    float rightMost = 0.0f;
    for (int i = 0; i < MAX_OBSTACLES; i++) {
        if (field->items[i].x > rightMost) rightMost = field->items[i].x;
    }

    for (int i = 0; i < MAX_OBSTACLES; i++) {
        field->items[i].x -= scrollDelta;
        if (field->items[i].x < -OBSTACLE_SIZE) {
            field->items[i].x = rightMost + RandomGap();
            field->items[i].hasCoin = RollHasCoin();
            field->items[i].coinCollected = false;
            rightMost = field->items[i].x;
        }
    }
}

void ObstacleFieldResolveLanding(const ObstacleField *field, Player *p)
{
    if (p->gravityDir != 1) return;   /* obstacles only sit on the normal ground */
    if (p->velY < 0.0f) return;       /* still moving upward, can't land yet */

    Rectangle pr = PlayerGetRect(p);
    float playerBottom = pr.y + pr.height;

    for (int i = 0; i < MAX_OBSTACLES; i++) {
        if (!field->items[i].active) continue;

        float obLeft  = field->items[i].x;
        float obRight = obLeft + OBSTACLE_SIZE;
        bool horizOverlap = (pr.x + pr.width > obLeft) && (pr.x < obRight);
        if (!horizOverlap) continue;

        /* land only when the cat's feet are right at the top of the block */
        if (playerBottom >= OBSTACLE_TOP_Y && playerBottom <= OBSTACLE_TOP_Y + 12.0f) {
            p->y = OBSTACLE_TOP_Y - PLAYER_HEIGHT;
            p->velY = 0.0f;
            p->onGround = true;
        }
    }
}

int ObstacleFieldCollectCoins(ObstacleField *field, const Player *p)
{
    int gained = 0;
    Rectangle pr = PlayerGetRect(p);

    for (int i = 0; i < MAX_OBSTACLES; i++) {
        if (!field->items[i].hasCoin || field->items[i].coinCollected) continue;

        Rectangle coinRect = { field->items[i].x, OBSTACLE_COIN_Y,
                                (float)COIN_SIZE, (float)COIN_SIZE };

        if (CheckCollisionRecs(pr, coinRect)) {
            field->items[i].coinCollected = true;
            gained += OBSTACLE_COIN_SCORE_VALUE;
        }
    }
    return gained;
}

void ObstacleFieldDraw(const ObstacleField *field, const GameTextures *tex)
{
    for (int i = 0; i < MAX_OBSTACLES; i++) {
        if (!field->items[i].active) continue;
        DrawTextureV(tex->block, (Vector2){ field->items[i].x, OBSTACLE_TOP_Y }, WHITE);

        if (field->items[i].hasCoin && !field->items[i].coinCollected) {
            DrawTextureV(tex->coin, (Vector2){ field->items[i].x, OBSTACLE_COIN_Y }, WHITE);
        }
    }
}
