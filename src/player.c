#include "player.h"
#include <math.h>

Player PlayerInit(void)
{
    Player p = { 0 };
    PlayerReset(&p);
    return p;
}

void PlayerReset(Player *p)
{
    p->x = PLAYER_START_X;
    p->y = GROUND_Y - PLAYER_HEIGHT;
    p->velY = 0.0f;
    p->gravityDir = 1;
    p->onGround = true;
    p->facingLeft = false;
    p->anim = ANIM_FRONT;
    p->health = MAX_HEALTH;
    p->hitCooldown = 0.0f;
    p->activePotion = POTION_NONE;
    p->potionTimer = 0.0f;
}

static void HandleInput(Player *p, float dt)
{
    bool left  = IsKeyDown(KEY_LEFT)  || IsKeyDown(KEY_A);
    bool right = IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D);

    if (left && !right) {
        p->x -= PLAYER_SIDE_SPEED * dt;
        p->facingLeft = true;
        if (p->onGround) p->anim = ANIM_SIDE;
    } else if (right && !left) {
        p->x += PLAYER_SIDE_SPEED * dt;
        p->facingLeft = false;
        if (p->onGround) p->anim = ANIM_SIDE;
    } else if (p->onGround) {
        p->anim = ANIM_FRONT;
    }

    if (p->x < PLAYER_MIN_X) p->x = PLAYER_MIN_X;
    if (p->x > PLAYER_MAX_X) p->x = PLAYER_MAX_X;

    bool jumpPressed = IsKeyPressed(KEY_SPACE) || IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W);
    if (jumpPressed && p->onGround) {
        p->velY = -JUMP_SPEED * p->gravityDir;
        p->onGround = false;
    }
}

static void ApplyPhysics(Player *p, float dt)
{
    p->velY += GRAVITY_ACCEL * p->gravityDir * dt;
    p->y += p->velY * dt;

    if (p->gravityDir == 1) {
        /* normal gravity: floor is the ground strip */
        float floorY = GROUND_Y - PLAYER_HEIGHT;
        if (p->y >= floorY) {
            p->y = floorY;
            p->velY = 0.0f;
            p->onGround = true;
        } else {
            p->onGround = false;
        }
    } else {
        /* flipped gravity: floor is the ceiling */
        if (p->y <= CEILING_Y) {
            p->y = CEILING_Y;
            p->velY = 0.0f;
            p->onGround = true;
        } else {
            p->onGround = false;
        }
    }

    if (!p->onGround) p->anim = ANIM_JUMP;
}

static void UpdatePotionTimer(Player *p, float dt)
{
    if (p->activePotion == POTION_NONE) return;

    p->potionTimer -= dt;
    if (p->potionTimer <= 0.0f) {
        if (p->activePotion == POTION_GRAVITY) p->gravityDir = 1;
        p->activePotion = POTION_NONE;
        p->potionTimer = 0.0f;
    }
}

void PlayerUpdate(Player *p, float dt)
{
    HandleInput(p, dt);
    ApplyPhysics(p, dt);
    UpdatePotionTimer(p, dt);
    if (p->hitCooldown > 0.0f) p->hitCooldown -= dt;
}

void PlayerApplyPotion(Player *p, PotionType type)
{
    /* only one potion active at a time - a new one instantly replaces the old */
    if (p->activePotion == POTION_GRAVITY && type != POTION_GRAVITY) {
        p->gravityDir = 1;
    }

    p->activePotion = type;
    p->potionTimer = POTION_DURATION;

    if (type == POTION_GRAVITY) {
        p->gravityDir = -1;
        p->velY = 0.0f;
        p->onGround = false;
    }
}

void PlayerTakeDamage(Player *p, int amount)
{
    if (p->activePotion == POTION_SHIELD) return; /* invincible */

    p->health -= amount;
    if (p->health < 0) p->health = 0;
    p->hitCooldown = 1.0f;
}

bool PlayerCanTakeDamage(const Player *p)
{
    return p->hitCooldown <= 0.0f;
}

Rectangle PlayerGetRect(const Player *p)
{
    return (Rectangle){ p->x, p->y, (float)PLAYER_WIDTH, (float)PLAYER_HEIGHT };
}

void PlayerDraw(const Player *p, const GameTextures *tex)
{
    Texture2D sprite;
    switch (p->anim) {
        case ANIM_SIDE: sprite = tex->catSide; break;
        case ANIM_JUMP: sprite = tex->catJump; break;
        default:        sprite = tex->catFront; break;
    }

    float srcW = (float)sprite.width;
    float srcH = (float)sprite.height;
    if (p->facingLeft) srcW = -srcW;          /* horizontal flip */
    if (p->gravityDir == -1) srcH = -srcH;    /* vertical flip while on the ceiling */

    Rectangle src = { 0, 0, srcW, srcH };
    Rectangle dst = { p->x, p->y, (float)PLAYER_WIDTH, (float)PLAYER_HEIGHT };

    Color tint = WHITE;
    if (p->activePotion == POTION_SHIELD) {
        /* gentle pulsing tint so the player can *see* the shield is active */
        float pulse = (sinf((float)GetTime() * 8.0f) + 1.0f) * 0.5f;
        tint = (Color){ 255, 255, (unsigned char)(200 + pulse * 55), 255 };
    }

    DrawTexturePro(sprite, src, dst, (Vector2){ 0, 0 }, 0.0f, tint);
}
