#include "ui.h"
#include "config.h"
#include <stdio.h>

static const char *PotionName(PotionType t)
{
    switch (t) {
        case POTION_SHIELD:  return "SHIELD";
        case POTION_SPEED:   return "SPEED";
        case POTION_GRAVITY: return "GRAVITY";
        default:             return "";
    }
}

void UIDrawHUD(const Player *p, int score, int highScore)
{
    /* --- health bar, top-left --- */
    int barX = 10, barY = 10, barW = 120, barH = 14;
    DrawRectangle(barX, barY, barW, barH, (Color){ 60, 20, 20, 255 });
    float pct = (float)p->health / (float)MAX_HEALTH;
    DrawRectangle(barX, barY, (int)(barW * pct), barH, (Color){ 210, 40, 60, 255 });
    DrawRectangleLines(barX, barY, barW, barH, RAYWHITE);

    char hpText[16];
    snprintf(hpText, sizeof(hpText), "%d/%d", p->health, MAX_HEALTH);
    DrawText(hpText, barX + barW + 8, barY, 10, RAYWHITE);

    /* --- score, top-right --- */
    char scoreText[32];
    snprintf(scoreText, sizeof(scoreText), "SCORE %04d", score);
    int scoreW = MeasureText(scoreText, 14);
    DrawText(scoreText, CANVAS_WIDTH - scoreW - 10, 10, 14, RAYWHITE);

    char hiText[32];
    snprintf(hiText, sizeof(hiText), "HIGH  %04d", highScore);
    int hiW = MeasureText(hiText, 10);
    DrawText(hiText, CANVAS_WIDTH - hiW - 10, 28, 10, (Color){ 255, 215, 90, 255 });

    /* --- active potion indicator --- */
    if (p->activePotion != POTION_NONE) {
        char potText[32];
        snprintf(potText, sizeof(potText), "%s %.1fs", PotionName(p->activePotion), p->potionTimer);
        DrawText(potText, 10, 30, 12, (Color){ 130, 220, 255, 255 });
    }
}