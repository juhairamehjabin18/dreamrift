// ============================================================
//  background.c  --  The parallax sky/cloud/tree/ground backdrop
//  OWNER: MEMBER 3  (split out of world.c for readability)
// ============================================================
//  ART STYLE: dark night sky, grey-blue cloud banks, rounded
//  mauve "bubble" treetops, and salmon-pink brick ground/ceiling
//  capped with a dark zigzag edge.
//
//  This file owns ONLY the background. Obstacles and coins still
//  live in world.c. The two files share the ZigzagCap() look, so
//  each keeps its own private (static) copy of it -- that keeps
//  this file fully self-contained and drop-in.
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

// One rounded "bubble" treetop made of overlapping circles
static void BubbleTree(float x, float y, float s, Color c) {
    DrawCircle((int)x,             (int)y,             (int)(30 * s), c);
    DrawCircle((int)(x - 24 * s),  (int)(y + 12 * s),  (int)(22 * s), c);
    DrawCircle((int)(x + 25 * s),  (int)(y + 10 * s),  (int)(24 * s), c);
    DrawCircle((int)(x -  8 * s),  (int)(y - 20 * s),  (int)(18 * s), c);
    DrawCircle((int)(x + 12 * s),  (int)(y - 18 * s),  (int)(16 * s), c);
}

// ------------------------------------------------------------
//  BACKGROUND -- parallax layers, drawn far to near
// ------------------------------------------------------------
void BackgroundDraw(void) {
    // ---------- Sky gradient ----------
    for (int i = 0; i < 10; i++) {
        float f = (float)i / 10.0f;
        Color c = {
            (unsigned char)(22 + (int)(f * 16)),
            (unsigned char)(18 + (int)(f * 12)),
            (unsigned char)(32 + (int)(f * 20)),
            255
        };
        DrawRectangle(0, i * (SCREEN_H / 10), SCREEN_W, SCREEN_H / 10 + 1, c);
    }

    // ---------- Stars (fixed seed so they don't flicker) ----------
    srand(5678);
    for (int i = 0; i < 60; i++) {
        int sx = rand() % SCREEN_W;
        int sy = rand() % (GROUND_Y - 260);
        float tw = 0.35f + 0.35f * sinf((float)GetTime() * 2.0f + i);
        DrawCircle(sx, sy, (i % 5 == 0) ? 2 : 1, Fade(WHITE, tw));
    }
    srand((unsigned int)time(NULL));

    // ---------- Far cloud bank (slowest parallax) ----------
    for (int i = 0; i < 9; i++) {
        float bx = fmodf(i * 210.0f - bgScroll * 0.06f,
                         SCREEN_W + 300.0f) - 150.0f;
        float by = 250.0f + (i % 3) * 42.0f;
        DrawCircle((int)bx,         (int)by,         62, CLOUD_FAR);
        DrawCircle((int)(bx + 55),  (int)(by + 16),  50, CLOUD_FAR);
        DrawCircle((int)(bx - 50),  (int)(by + 20),  44, CLOUD_FAR);
        DrawCircle((int)(bx + 20),  (int)(by - 30),  40, CLOUD_FAR);
    }

    // ---------- Near cloud bank ----------
    for (int i = 0; i < 7; i++) {
        float bx = fmodf(i * 265.0f - bgScroll * 0.13f,
                         SCREEN_W + 340.0f) - 170.0f;
        float by = 330.0f + (i % 2) * 50.0f;
        DrawCircle((int)bx,        (int)by,        54, CLOUD_NEAR);
        DrawCircle((int)(bx + 48), (int)(by + 14), 44, CLOUD_NEAR);
        DrawCircle((int)(bx - 44), (int)(by + 18), 38, CLOUD_NEAR);
    }

    // ---------- Far bubble trees ----------
    for (int i = 0; i < 11; i++) {
        float tx = fmodf(i * 150.0f - bgScroll * 0.22f,
                         SCREEN_W + 220.0f) - 110.0f;
        float ty = 400.0f + (i % 3) * 26.0f;
        DrawRectangle((int)(tx - 5), (int)(ty + 18), 10, 70, TRUNK_COL);
        BubbleTree(tx, ty, 0.72f, TREE_DARK);
    }

    // ---------- Mid bubble trees ----------
    for (int i = 0; i < 8; i++) {
        float tx = fmodf(i * 205.0f - bgScroll * 0.38f,
                         SCREEN_W + 260.0f) - 130.0f;
        float ty = 448.0f + (i % 2) * 30.0f;
        DrawRectangle((int)(tx - 6), (int)(ty + 22), 12, 90, TRUNK_COL);
        BubbleTree(tx, ty, 1.00f, TREE_MID);
    }

    // ---------- Near bubble trees (biggest, lightest) ----------
    for (int i = 0; i < 6; i++) {
        float tx = fmodf(i * 285.0f - bgScroll * 0.58f,
                         SCREEN_W + 320.0f) - 160.0f;
        float ty = 490.0f + (i % 2) * 22.0f;
        DrawRectangle((int)(tx - 8), (int)(ty + 26), 16, 100, TRUNK_COL);
        BubbleTree(tx, ty, 1.28f, TREE_LIGHT);
    }

    // ---------- The ground slab ----------
    DrawRectangle(0, GROUND_Y, SCREEN_W, SCREEN_H - GROUND_Y, GND_LOW);
    DrawRectangle(0, GROUND_Y, SCREEN_W, 62, GND_BODY);
    DrawRectangle(0, GROUND_Y, SCREEN_W, 14, GND_TOP);

    // Scrolling brick seams
    float seam = fmodf(bgScroll, 68.0f);
    for (int i = -1; i * 68 < SCREEN_W + 68; i++) {
        int bx = (int)(i * 68 - seam);
        DrawLine(bx, GROUND_Y + 14, bx, SCREEN_H, Fade(GND_EDGE, 0.28f));
    }
    DrawLine(0, GROUND_Y + 62, SCREEN_W, GROUND_Y + 62, Fade(GND_EDGE, 0.35f));

    ZigzagCap(0, (float)GROUND_Y, (float)SCREEN_W, false);

    // ---------- Ceiling strip (for gravity flip) ----------
    DrawRectangle(0, 0, SCREEN_W, CEILING_Y, GND_BODY);
    DrawRectangle(0, CEILING_Y - 14, SCREEN_W, 14, GND_TOP);
    ZigzagCap(0, (float)CEILING_Y, (float)SCREEN_W, true);
}
