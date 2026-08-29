#ifndef COIN_H
#define COIN_H

#include "raylib.h"
#include "textures.h"
#include "config.h"
#include "player.h"

typedef struct Coin {
    float x;
    bool collected;
} Coin;

typedef struct CoinField {
    Coin items[MAX_COINS];
} CoinField;

CoinField CoinFieldInit(void);
void CoinFieldUpdate(CoinField *field, float scrollDelta);
int  CoinFieldCollect(CoinField *field, const Player *p);   /* returns score gained this frame */
void CoinFieldDraw(const CoinField *field, const GameTextures *tex, int gravityDir);

#endif /* COIN_H */
