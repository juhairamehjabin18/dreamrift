#ifndef UI_H
#define UI_H

#include "raylib.h"
#include "player.h"

void UIDrawHUD(const Player *p, int score, int highScore);
void UIDrawMenu(void);
void UIDrawPaused(void);
void UIDrawGameOver(int score, int highScore);

#endif /* UI_H */