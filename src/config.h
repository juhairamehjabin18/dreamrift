#ifndef CONFIG_H
#define CONFIG_H

/* ---------- Canvas  ---------- */
#define CANVAS_WIDTH   640
#define CANVAS_HEIGHT  360

/* ---------- World layout ---------- */
#define GROUND_Y        310.0f   /* top of the ground strip = the player's normal floor  */
#define CEILING_Y        40.0f   /* the player's "floor" while the Gravity Potion is active */

/* ---------- Player ---------- */
#define PLAYER_WIDTH     18
#define PLAYER_HEIGHT    18
#define PLAYER_START_X  120.0f
#define PLAYER_MIN_X     20.0f
#define PLAYER_MAX_X    280.0f   /* the cat is allowed to roam left/right inside this band */
#define PLAYER_SIDE_SPEED 110.0f
#define GRAVITY_ACCEL   900.0f
#define JUMP_SPEED      370.0f
#define MAX_HEALTH      100

/* ---------- World scrolling ---------- */
#define BASE_SCROLL_SPEED 140.0f
#define SPEED_POTION_MULTIPLIER 1.7f

/* ---------- Potions ---------- */
#define POTION_DURATION   10.0f
#define MAX_POTIONS_ON_SCREEN 2
#define POTION_SPAWN_MIN  4.0f
#define POTION_SPAWN_MAX  8.0f

typedef enum {
    POTION_NONE = 0,
    POTION_SHIELD,
    POTION_SPEED,
    POTION_GRAVITY
} PotionType;

/* ---------- Enemy: Dreadwing ---------- */
#define ENEMY_SPAWN_INTERVAL 2.0f
#define ENEMY_DAMAGE          5
#define MAX_ENEMIES           3
#define ENEMY_WIDTH          28
#define ENEMY_HEIGHT         24


/* ---------- Obstacles (blocks) ---------- */
#define MAX_OBSTACLES     6
#define OBSTACLE_SIZE    16
#define OBSTACLE_MIN_GAP 160.0f
#define OBSTACLE_MAX_GAP 260.0f

/* ---------- Coins ---------- */
#define MAX_COINS        30
#define COIN_SIZE         8
#define COIN_MIN_GAP     70.0f
#define COIN_MAX_GAP    140.0f

#endif /* CONFIG_H */
