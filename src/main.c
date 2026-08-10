#include "raylib.h"
#include "config.h"
#include "textures.h"
#include "game.h"

int main(void)
{
    /* Start windowed and resizable; press F11 any time to go fullscreen.
     * Either way the 640x360 canvas is always scaled by a whole number
     * (see the scale calculation below) so the pixel art never looks blurry. */
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(1280, 720, "Dreamrift");
    SetTargetFPS(60);
    SetExitKey(KEY_NULL); /* don't let ESC instantly close the game */

    GameTextures tex = LoadGameTextures();
    GameState game = GameInit();

    RenderTexture2D canvas = LoadRenderTexture(CANVAS_WIDTH, CANVAS_HEIGHT);
    SetTextureFilter(canvas.texture, TEXTURE_FILTER_POINT);

    while (!WindowShouldClose()) {
        if (IsKeyPressed(KEY_F11)) ToggleFullscreen();

        float dt = GetFrameTime();
        GameUpdate(&game, dt);

        /* ---- draw the whole game to the small fixed-size canvas ---- */
        BeginTextureMode(canvas);
            ClearBackground(BLACK);
            GameDraw(&game, &tex);
        EndTextureMode();

        /* ---- scale that canvas up to fill the real window, in whole numbers ---- */
        int screenW = GetScreenWidth();
        int screenH = GetScreenHeight();
        int scale = screenW / CANVAS_WIDTH;
        int scaleH = screenH / CANVAS_HEIGHT;
        if (scaleH < scale) scale = scaleH;
        if (scale < 1) scale = 1;

        int drawW = CANVAS_WIDTH * scale;
        int drawH = CANVAS_HEIGHT * scale;
        int offsetX = (screenW - drawW) / 2;
        int offsetY = (screenH - drawH) / 2;

        BeginDrawing();
            ClearBackground(BLACK); /* letterbox bars on non-16:9 windows */
            /* raylib render textures are stored upside-down, hence -height here */
            Rectangle src = { 0, 0, (float)canvas.texture.width, -(float)canvas.texture.height };
            Rectangle dst = { (float)offsetX, (float)offsetY, (float)drawW, (float)drawH };
            DrawTexturePro(canvas.texture, src, dst, (Vector2){ 0, 0 }, 0.0f, WHITE);
        EndDrawing();
    }

    UnloadRenderTexture(canvas);
    UnloadGameTextures(&tex);
    CloseWindow();
    return 0;
}
