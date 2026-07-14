// ============================================================
//  cat_movement.c  --  DREAMRIFT: Player Movement Demo
// ============================================================
//  A single-file demo of the player character.
//  Shows three things and nothing else:
//
//      1. WALKING   -- 4-frame animation cycle
//      2. RUNNING   -- faster movement, faster animation
//      3. JUMPING   -- variable height (tap = hop, hold = full)
//
//  There are NO image files. The cat is built entirely from
//  circles, rectangles, and triangles.
//
//  COMPILE
//  -------
//  Windows (MSYS2 UCRT64):
//    gcc -o cat_movement.exe cat_movement.c -lraylib -lopengl32 -lgdi32 -lwinmm
//
//  macOS:
//    gcc -o cat_movement cat_movement.c -lraylib -framework OpenGL -framework Cocoa -framework IOKit -framework CoreAudio -framework CoreVideo
//
//  Linux:
//    gcc -o cat_movement cat_movement.c -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
//
//  CONTROLS
//  --------
//    A / Left Arrow   -- walk left
//    D / Right Arrow  -- walk right
//    Hold SHIFT       -- run (faster)
//    W / Up / Space   -- jump   (tap = small hop, hold = full jump)
// ============================================================

#include "raylib.h"
#include <math.h>

// ------------------------------------------------------------
//  CONSTANTS
// ------------------------------------------------------------
#define SCREEN_W      1000
#define SCREEN_H       560
#define GROUND_Y       440      // where the floor is
#define PLAYER_W        48
#define PLAYER_H        60

#define GRAVITY       1380.0f   // downward pull, pixels/second^2
#define JUMP_FORCE    -700.0f   // upward kick (negative = up)
#define WALK_SPEED     220.0f
#define RUN_SPEED      420.0f

// ------------------------------------------------------------
//  COLOURS
// ------------------------------------------------------------
#define CAT_DARK    (Color){ 62,  58,  74, 255}   // coat
#define CAT_DARK2   (Color){ 44,  40,  56, 255}   // coat shading
#define CAT_CREAM   (Color){245, 240, 236, 255}   // face + chest
#define CAT_NOSE    (Color){232, 154, 168, 255}   // nose + toe beans
#define CAT_EYE     (Color){ 42,  36,  56, 255}   // pupils

#define BG_SKY      (Color){ 30,  24,  44, 255}
#define GND_TOP     (Color){196, 112, 126, 255}
#define GND_BODY    (Color){168,  90, 107, 255}
#define GND_EDGE    (Color){ 46,  30,  46, 255}

// ------------------------------------------------------------
//  THE PLAYER
// ------------------------------------------------------------
typedef struct {
    Vector2 pos;        // top-left corner
    Vector2 vel;        // velocity in pixels/second
    bool    onGround;

    int     frame;      // walk cycle: 0 -> 1 -> 2 -> 3 -> 0
    float   frameT;     // timer that advances the frame
    bool    facingLeft; // which way the cat is looking
} Player;

static Player pl = {0};
static float  groundScroll = 0.0f;   // makes the floor look like it moves

// ============================================================
//  UPDATE  --  input and physics, once per frame
// ============================================================
//  dt = "delta time" = seconds since the last frame (~0.016 at
//  60 FPS). Multiplying every movement by dt means the cat moves
//  at the same real-world speed on any computer, fast or slow.
// ============================================================
void PlayerUpdate(float dt) {

    // ---------- 1. HORIZONTAL MOVEMENT (walk / run) ----------
    bool running = IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_RIGHT_SHIFT);
    float speed  = running ? RUN_SPEED : WALK_SPEED;

    pl.vel.x = 0;
    if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT)) {
        pl.vel.x   =  speed;
        pl.facingLeft = false;
    }
    if (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT)) {
        pl.vel.x   = -speed;
        pl.facingLeft = true;
    }

    // ---------- 2. JUMPING ----------
    bool jumpPressed = IsKeyPressed(KEY_W) || IsKeyPressed(KEY_UP)
                    || IsKeyPressed(KEY_SPACE);
    bool jumpHeld    = IsKeyDown(KEY_W)    || IsKeyDown(KEY_UP)
                    || IsKeyDown(KEY_SPACE);

    // Can only jump when standing on the ground
    if (jumpPressed && pl.onGround) {
        pl.vel.y    = JUMP_FORCE;
        pl.onGround = false;
    }

    // VARIABLE JUMP HEIGHT:
    // If the player lets go of the button while still rising,
    // we cut the upward speed short. Tapping = a small hop,
    // holding = the full jump. This is the classic platformer feel.
    if (!jumpHeld && !pl.onGround && pl.vel.y < -180.0f) {
        pl.vel.y = -180.0f;
    }

    // ---------- 3. GRAVITY ----------
    pl.vel.y += GRAVITY * dt;          // pull downward every frame
    if (pl.vel.y > 1200) pl.vel.y = 1200;   // terminal velocity

    // ---------- 4. APPLY THE VELOCITY ----------
    pl.pos.x += pl.vel.x * dt;
    pl.pos.y += pl.vel.y * dt;

    // ---------- 5. KEEP THE CAT ON SCREEN ----------
    if (pl.pos.x < 20)                        pl.pos.x = 20;
    if (pl.pos.x > SCREEN_W - PLAYER_W - 20)  pl.pos.x = SCREEN_W - PLAYER_W - 20;

    // ---------- 6. LANDING ON THE GROUND ----------
    if (pl.pos.y + PLAYER_H >= GROUND_Y) {
        pl.pos.y    = GROUND_Y - PLAYER_H;   // snap exactly to the floor
        pl.vel.y    = 0;
        pl.onGround = true;
    }

    // ---------- 7. WALK ANIMATION ----------
    // The cat only animates while it is actually moving.
    // Running cycles the frames faster than walking.
    if (pl.vel.x != 0 && pl.onGround) {
        float frameTime = running ? 0.07f : 0.12f;
        pl.frameT += dt;
        if (pl.frameT >= frameTime) {
            pl.frameT = 0;
            pl.frame  = (pl.frame + 1) % 4;   // 0,1,2,3,0,1,2,3...
        }
    } else if (pl.onGround) {
        pl.frame  = 0;      // standing still -> reset to the idle pose
        pl.frameT = 0;
    }

    // Scroll the floor pattern so movement is easy to see
    groundScroll += pl.vel.x * dt;
}

// ============================================================
//  DRAW  --  the tuxedo cat, built from basic shapes
// ============================================================
void PlayerDraw(void) {
    float px = pl.pos.x, py = pl.pos.y;
    float cx = px + PLAYER_W / 2.0f;   // horizontal centre

    // Vertical anchors for each body part
    float headY = py + 17;
    float bodyY = py + 40;
    float feetY = py + 50;
    float tailY = py + 38;

    // ---------- ANIMATION OFFSETS ----------
    // The body bobs up slightly on frames 1 and 3.
    float bob = (pl.frame == 1 || pl.frame == 3) ? 1.5f : 0.0f;

    // The legs swing forward on frame 0 and backward on frame 2.
    float legSwing = (pl.frame == 0) ?  4.0f
                   : (pl.frame == 2) ? -4.0f : 0.0f;

    // When airborne, tuck the legs up instead of animating them.
    if (!pl.onGround) { legSwing = 0.0f; bob = 0.0f; feetY -= 4; }

    // ---------- TAIL (drawn first, so it sits behind) ----------
    float wag = sinf((float)GetTime() * 7.0f) * 5.0f;
    float tdir = pl.facingLeft ? 1.0f : -1.0f;   // tail goes behind the cat
    DrawCircle((int)(cx + 20 * tdir), (int)(tailY + bob),        7, CAT_DARK);
    DrawCircle((int)(cx + 27 * tdir), (int)(tailY -  7 + wag),   6, CAT_DARK);
    DrawCircle((int)(cx + 30 * tdir), (int)(tailY - 16 + wag),   5, CAT_DARK);
    DrawCircle((int)(cx + 31 * tdir), (int)(tailY - 22 + wag),   4, CAT_CREAM);

    // ---------- BACK LEGS ----------
    DrawCircle((int)(cx - 13), (int)(bodyY + 8), 6, CAT_DARK2);
    DrawCircle((int)(cx + 13), (int)(bodyY + 8), 6, CAT_DARK2);

    // ---------- BODY ----------
    DrawEllipse((int)cx, (int)(bodyY + bob),     16, 13, CAT_DARK);   // dark back
    DrawEllipse((int)cx, (int)(bodyY + 4 + bob), 11,  9, CAT_CREAM);  // cream chest

    // ---------- FRONT PAWS (these do the walking) ----------
    float pawL = feetY + legSwing;    // one paw forward...
    float pawR = feetY - legSwing;    // ...while the other goes back

    DrawCircle((int)(cx - 9), (int)(pawL + 5), 6, CAT_CREAM);
    DrawRectangle((int)(cx - 14), (int)pawL, 11, 5, CAT_CREAM);
    DrawCircle((int)(cx - 9), (int)(pawL + 7), 2, CAT_NOSE);   // toe bean

    DrawCircle((int)(cx + 9), (int)(pawR + 5), 6, CAT_CREAM);
    DrawRectangle((int)(cx + 3), (int)pawR, 11, 5, CAT_CREAM);
    DrawCircle((int)(cx + 9), (int)(pawR + 7), 2, CAT_NOSE);   // toe bean

    // ---------- EARS ----------
    DrawTriangle((Vector2){cx - 16, headY + 6},
                 (Vector2){cx - 13, headY - 16},
                 (Vector2){cx -  4, headY + 6}, CAT_DARK);
    DrawTriangle((Vector2){cx +  4, headY + 6},
                 (Vector2){cx + 13, headY - 16},
                 (Vector2){cx + 16, headY + 6}, CAT_DARK);
    DrawCircle((int)(cx - 11), (int)(headY - 6), 3, CAT_NOSE);  // inner ear
    DrawCircle((int)(cx + 11), (int)(headY - 6), 3, CAT_NOSE);  // inner ear

    // ---------- HEAD ----------
    DrawCircle ((int)cx, (int)(headY + bob),     18,     CAT_DARK);
    DrawEllipse((int)cx, (int)(headY + 5 + bob), 13, 12, CAT_CREAM);  // face
    DrawEllipse((int)cx, (int)(headY - 6 + bob),  4,  8, CAT_CREAM);  // forehead

    // ---------- EYES (blink on frame 3) ----------
    float eyeY = headY + 1 + bob;
    if (pl.frame != 3) {
        DrawEllipse((int)(cx - 7), (int)eyeY, 4, 5, CAT_EYE);
        DrawEllipse((int)(cx + 7), (int)eyeY, 4, 5, CAT_EYE);
        DrawCircle ((int)(cx - 6), (int)(eyeY - 2), 1.6f, WHITE);  // shine
        DrawCircle ((int)(cx + 8), (int)(eyeY - 2), 1.6f, WHITE);  // shine
    } else {
        // Blink = a happy upward curve
        DrawLine((int)(cx - 11), (int)eyeY,       (int)(cx - 7), (int)(eyeY - 3), CAT_EYE);
        DrawLine((int)(cx -  7), (int)(eyeY - 3), (int)(cx - 3), (int)eyeY,       CAT_EYE);
        DrawLine((int)(cx +  3), (int)eyeY,       (int)(cx + 7), (int)(eyeY - 3), CAT_EYE);
        DrawLine((int)(cx +  7), (int)(eyeY - 3), (int)(cx + 11), (int)eyeY,      CAT_EYE);
    }

    // ---------- NOSE + MOUTH ----------
    float noseY = headY + 8 + bob;
    DrawTriangle((Vector2){cx - 3, noseY},
                 (Vector2){cx,     noseY + 3},
                 (Vector2){cx + 3, noseY}, CAT_NOSE);
    DrawLine((int)cx, (int)(noseY + 3), (int)cx,       (int)(noseY + 5), CAT_EYE);
    DrawLine((int)cx, (int)(noseY + 5), (int)(cx - 4), (int)(noseY + 7), CAT_EYE);
    DrawLine((int)cx, (int)(noseY + 5), (int)(cx + 4), (int)(noseY + 7), CAT_EYE);

    // ---------- WHISKERS ----------
    Color wk = Fade(WHITE, 0.75f);
    float wy = noseY + 1;
    DrawLine((int)(cx - 10), (int)(wy - 2), (int)(cx - 22), (int)(wy - 4), wk);
    DrawLine((int)(cx - 10), (int)(wy + 2), (int)(cx - 22), (int)(wy + 3), wk);
    DrawLine((int)(cx + 10), (int)(wy - 2), (int)(cx + 22), (int)(wy - 4), wk);
    DrawLine((int)(cx + 10), (int)(wy + 2), (int)(cx + 22), (int)(wy + 3), wk);
}

// ============================================================
//  MAIN
// ============================================================
int main(void) {
    InitWindow(SCREEN_W, SCREEN_H, "Dreamrift  --  Player Movement Demo");
    SetTargetFPS(60);

    // Start the cat standing in the middle of the floor
    pl.pos      = (Vector2){ SCREEN_W / 2 - PLAYER_W / 2, GROUND_Y - PLAYER_H };
    pl.onGround = true;

    while (!WindowShouldClose()) {
        float dt = GetFrameTime();

        PlayerUpdate(dt);

        BeginDrawing();
        ClearBackground(BG_SKY);

        // ---------- GROUND ----------
        DrawRectangle(0, GROUND_Y, SCREEN_W, SCREEN_H - GROUND_Y, GND_BODY);
        DrawRectangle(0, GROUND_Y, SCREEN_W, 14, GND_TOP);
        DrawRectangle(0, GROUND_Y - 4, SCREEN_W, 4, GND_EDGE);

        // Scrolling seams so you can SEE the cat moving
        float seam = fmodf(groundScroll, 60.0f);
        for (int i = -1; i * 60 < SCREEN_W + 60; i++) {
            int bx = (int)(i * 60 - seam);
            DrawLine(bx, GROUND_Y + 14, bx, SCREEN_H, Fade(GND_EDGE, 0.3f));
        }

        // ---------- THE CAT ----------
        PlayerDraw();

        // ---------- ON-SCREEN INFO ----------
        DrawText("DREAMRIFT  --  Player Movement", 30, 25, 24, Fade(WHITE, 0.9f));
        DrawText("A/D or Arrows: Walk      Hold SHIFT: Run      W/Up/Space: Jump (tap = hop, hold = high)",
                 30, 60, 17, Fade(WHITE, 0.45f));

        // Live state readout -- useful to point at during a demo
        bool running = IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_RIGHT_SHIFT);
        const char *state = !pl.onGround   ? "JUMPING"
                          : (pl.vel.x != 0) ? (running ? "RUNNING" : "WALKING")
                          : "IDLE";

        DrawRectangle(30, SCREEN_H - 70, 250, 44, Fade(BLACK, 0.35f));
        DrawText(TextFormat("STATE: %s", state), 44, SCREEN_H - 62, 20,
                 (Color){255, 214, 102, 255});
        DrawText(TextFormat("animation frame: %d", pl.frame),
                 44, SCREEN_H - 40, 14, Fade(WHITE, 0.55f));

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
