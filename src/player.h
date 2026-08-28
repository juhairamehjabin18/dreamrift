#ifndef PLAYER_H
#define PLAYER_H

#include "raylib.h"
#include "textures.h"
#include "config.h"

typedef enum { ANIM_FRONT, ANIM_SIDE, ANIM_JUMP } PlayerAnim;

typedef struct Player {
    float x, y;        /* top-left position, in canvas pixels */
    float velY;
    int   gravityDir;  /* +1 = normal (falls down), -1 = flipped (falls up) */
    bool  onGround;
    bool  facingLeft;
    PlayerAnim anim;

    int   health;
    float hitCooldown;   /* brief invulnerability window after taking damage */

    PotionType activePotion;
    float potionTimer;   /* seconds remaining */
} Player;

Player PlayerInit(void);
void PlayerReset(Player *p);
void PlayerUpdate(Player *p, float dt);
void PlayerApplyPotion(Player *p, PotionType type);
void PlayerTakeDamage(Player *p, int amount);
bool PlayerCanTakeDamage(const Player *p);
Rectangle PlayerGetRect(const Player *p);
void PlayerDraw(const Player *p, const GameTextures *tex);

#endif
