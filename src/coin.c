#include "coin.h"

#define COIN_SCORE_VALUE 2

/* Fixed, regular spacing - this is what makes the coin trail look "even"
 * instead of random clumps and gaps. */
#define COIN_GAP 60.0f

/* Coins reposition themselves depending on the cat's current gravity so they
 * stay reachable whether the cat is running on the ground or the ceiling. */
static float CoinGetY(int gravityDir)
{
    return (gravityDir == 1) ? (GROUND_Y - 50.0f) : (CEILING_Y + 50.0f);
}

CoinField CoinFieldInit(void)
{
    CoinField field = { 0 };
    float x = CANVAS_WIDTH + 60.0f;
    for (int i = 0; i < MAX_COINS; i++) {
        field.items[i].x = x;
        field.items[i].collected = false;
        x += COIN_GAP;
    }
    return field;
}

void CoinFieldUpdate(CoinField *field, float scrollDelta)
{
    float rightMost = 0.0f;
    for (int i = 0; i < MAX_COINS; i++) {
        if (field->items[i].x > rightMost) rightMost = field->items[i].x;
    }

    for (int i = 0; i < MAX_COINS; i++) {
        field->items[i].x -= scrollDelta;
        if (field->items[i].x < -COIN_SIZE) {
            field->items[i].x = rightMost + COIN_GAP;
            field->items[i].collected = false;
            rightMost = field->items[i].x;
        }
    }
}

int CoinFieldCollect(CoinField *field, const Player *p)
{
    int gained = 0;
    Rectangle pr = PlayerGetRect(p);

    for (int i = 0; i < MAX_COINS; i++) {
        if (field->items[i].collected) continue;

        float y = CoinGetY(p->gravityDir);
        Rectangle cr = { field->items[i].x, y, (float)COIN_SIZE, (float)COIN_SIZE };

        if (CheckCollisionRecs(pr, cr)) {
            field->items[i].collected = true;
            gained += COIN_SCORE_VALUE;
        }
    }
    return gained;
}

void CoinFieldDraw(const CoinField *field, const GameTextures *tex, int gravityDir)
{
    float y = CoinGetY(gravityDir);
    for (int i = 0; i < MAX_COINS; i++) {
        if (field->items[i].collected) continue;
        DrawTextureV(tex->coin, (Vector2){ field->items[i].x, y }, WHITE);
    }
}
