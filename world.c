// ============================================================
//  world.c  --  Obstacles and coins
//  OWNER: MEMBER 3
// ============================================================
//  NOTE: the parallax background used to live in this file. It
//  now lives in its own module, background.c, so this file only
//  covers obstacles and coins.
//
//  ART STYLE: salmon-pink brick platforms and purple crystal
//  spikes, capped with a dark zigzag edge (see ZigzagCap below).
//
//  Obstacles come in 3 shapes:
//    type 0 -- brick pillar
//    type 1 -- floating platform
//    type 2 -- purple crystal spike
//  None of them deal damage. They are pure navigation.
//
//  Every object here uses the OBJECT POOL pattern: a fixed-size
//  array plus an "alive" flag. No malloc, so no memory leaks.
// ============================================================

#include "game.h"

// ------------------------------------------------------------
//  Helper: the dark zigzag cap that sits on top of every pink
//  surface. This single shape is what gives the game its look.
// ------------------------------------------------------------
static void ZigzagCap(float x, float y, float w, bool flip) {
    const float tooth = 10.0f;
    int teeth = (int)(w / tooth);

    for (int i = 0; i < teeth; i++) {
        float tx = x + i * tooth;
        if (!flip) {
            DrawTriangle(
                (Vector2){ tx,             y     },
                (Vector2){ tx + tooth/2,   y + 7 },
                (Vector2){ tx + tooth,     y     },
                GND_EDGE);
        } else {
            DrawTriangle(
                (Vector2){ tx,             y     },
                (Vector2){ tx + tooth,     y     },
                (Vector2){ tx + tooth/2,   y - 7 },
                GND_EDGE);
        }
    }
}

// ------------------------------------------------------------
//  OBSTACLES
// ------------------------------------------------------------
void ObsSpawn(void) {
    for (int i = 0; i < MAX_OBS; i++) {
        if (obs[i].alive) continue;

        float w, h, y;
        int type = rand() % 3;

        if (type == 0) {           // Brick pillar
            w = RandF(46, 84);
            h = RandF(60, 150);
            y = GROUND_Y - h;
        } else if (type == 1) {    // Floating platform
            w = RandF(80, 120);
            h = 30;
            y = GROUND_Y - 190 - RandF(0, 90);
        } else {                   // Crystal spike
            w = 44;
            h = RandF(60, 105);
            y = GROUND_Y - h;
        }

        // Extra ceiling obstacle when gravity is flipped
        if (pl.gravFlip && rand() % 2 == 0) {
            y    = CEILING_Y;
            h    = RandF(50, 90);
            type = 0;
        }

        obs[i].rect  = (Rectangle){ (float)(SCREEN_W + 50), y, w, h };
        obs[i].type  = type;
        obs[i].alive = true;
        break;
    }
}

void ObsUpdate(float dt) {
    for (int i = 0; i < MAX_OBS; i++) {
        if (!obs[i].alive) continue;
        obs[i].rect.x -= scrollSpd * dt;
        if (obs[i].rect.x + obs[i].rect.width < -60) obs[i].alive = false;
    }
}

void ObsDraw(void) {
    for (int i = 0; i < MAX_OBS; i++) {
        if (!obs[i].alive) continue;

        Rectangle r = obs[i].rect;

        // ---------- TYPE 2: purple crystal spike ----------
        if (obs[i].type == 2) {
            float cx = r.x + r.width / 2;

            DrawTriangle(
                (Vector2){ cx,            r.y },
                (Vector2){ r.x,           r.y + r.height * 0.60f },
                (Vector2){ cx,            r.y + r.height },
                CRYS_MID);
            DrawTriangle(
                (Vector2){ cx,            r.y },
                (Vector2){ cx,            r.y + r.height },
                (Vector2){ r.x + r.width, r.y + r.height * 0.60f },
                CRYS_DARK);

            // Bright facet down the left edge
            DrawTriangle(
                (Vector2){ cx,                    r.y + 6 },
                (Vector2){ r.x + r.width * 0.22f, r.y + r.height * 0.58f },
                (Vector2){ cx,                    r.y + r.height * 0.72f },
                CRYS_LIGHT);

            // Small companion crystal
            float sx = r.x + r.width * 0.78f;
            DrawTriangle(
                (Vector2){ sx,      r.y + r.height * 0.45f },
                (Vector2){ sx - 11, r.y + r.height },
                (Vector2){ sx + 11, r.y + r.height },
                CRYS_MID);
            DrawTriangle(
                (Vector2){ sx,      r.y + r.height * 0.50f },
                (Vector2){ sx - 5,  r.y + r.height },
                (Vector2){ sx,      r.y + r.height },
                CRYS_LIGHT);
            continue;
        }

        // ---------- TYPE 0 and 1: pink brick ----------
        DrawRectangleRec(r, GND_LOW);
        DrawRectangle((int)r.x, (int)r.y,
                      (int)r.width, (int)(r.height * 0.62f), GND_BODY);
        DrawRectangle((int)r.x, (int)r.y, (int)r.width, 12, GND_TOP);

        // Brick seam lines
        for (int b = 1; b * 26 < (int)r.height; b++) {
            DrawLine((int)r.x,              (int)(r.y + b * 26),
                     (int)(r.x + r.width),  (int)(r.y + b * 26),
                     Fade(GND_EDGE, 0.22f));
        }

        ZigzagCap(r.x, r.y, r.width, false);
        DrawRectangleLinesEx(r, 2, Fade(GND_EDGE, 0.65f));
    }
}

// ------------------------------------------------------------
//  COINS
// ------------------------------------------------------------
void CoinSpawn(float x, float y) {
    for (int i = 0; i < MAX_COINS; i++) {
        if (coins[i].alive) continue;
        coins[i].pos   = (Vector2){ x, y };
        coins[i].alive = true;
        coins[i].phase = RandF(0, 6.28f);
        break;
    }
}

void CoinUpdate(float dt) {
    for (int i = 0; i < MAX_COINS; i++) {
        if (!coins[i].alive) continue;
        coins[i].pos.x -= scrollSpd * dt;
        if (coins[i].pos.x < -30) coins[i].alive = false;
    }
}

void CoinDraw(void) {
    float t = (float)GetTime();

    for (int i = 0; i < MAX_COINS; i++) {
        if (!coins[i].alive) continue;

        float cx = coins[i].pos.x;
        float cy = coins[i].pos.y + sinf(t * 3.0f + coins[i].phase) * 5.0f;

        // Soft glow
        DrawCircle((int)cx, (int)cy, 15, Fade(COIN_IN, 0.16f));
        // Dark rim, gold body, bright core
        DrawCircle((int)cx, (int)cy, 12, GND_EDGE);
        DrawCircle((int)cx, (int)cy, 11, COIN_OUT);
        DrawCircle((int)cx, (int)cy,  8, COIN_IN);
        // Diagonal shine streak
        DrawLine((int)(cx - 4), (int)(cy + 2),
                 (int)(cx + 1), (int)(cy - 4), COIN_SHINE);
        DrawLine((int)(cx - 2), (int)(cy + 4),
                 (int)(cx + 3), (int)(cy - 2), COIN_SHINE);
    }
}

// NOTE: BackgroundDraw() and its BubbleTree() helper have moved
// to background.c. BackgroundDraw() is still declared in game.h,
// so every caller (game.c, ui.c) works exactly as before.
