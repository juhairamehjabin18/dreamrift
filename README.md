# Dreamrift

A 2D pixel-art endless runner built in C with raylib. See `spec-v3.md` for the
full game specification.

## Project layout

```
dreamrift/
  assets/     all sprites (already included)
  src/        every .c/.h file - see "Module map" below
  Makefile    builds on Windows, macOS and Linux
```

## Module map

| File | Responsibility |
|---|---|
| `config.h` | Every tunable constant (sizes, speeds, timers). No logic. |
| `textures.h/.c` | Loads/unloads every sprite exactly once. |
| `world.h/.c` | Scrolling background + ground. |
| `player.h/.c` | The cat: input, gravity (incl. flipped), potions state, drawing. |
| `obstacle.h/.c` | Blocks: recycling, and letting the cat stand on top. |
| `coin.h/.c` | Coins: recycling, scoring, gravity-aware height. |
| `potion.h/.c` | Potion pickups: spawning, applying effects to the player. |
| `enemy.h/.c` | Dreadwing: timed spawns, flap animation, contact damage. |
| `ui.h/.c` | Health bar, score, high score, potion timer, menu/pause/game-over screens. |
| `game.h/.c` | The state machine (menu/playing/paused/game-over) that ties everything together. |
| `main.c` | Window setup + the 640x360 canvas scaling described in spec section 11.1. |

## Building & running

You said raylib is already installed on all three laptops, so these are just
the commands to compile *this* project against it.

### Windows 11 (using w64devkit or MSYS2/MinGW + `make`)

```bat
cd dreamrift
make
dreamrift.exe
```

If `make` isn't available, compile directly:

```bat
gcc -std=c99 -Wall src\*.c -o dreamrift.exe -lraylib -lopengl32 -lgdi32 -lwinmm
dreamrift.exe
```

> Run it from the `dreamrift` folder (not from inside `src`), so the
> `assets/...` paths resolve correctly.

### macOS (Apple Silicon / M4, raylib via Homebrew)

```bash
cd dreamrift
make
./dreamrift
```

If `make` isn't available, compile directly:

```bash
cd dreamrift
clang -std=c99 -Wall src/*.c -o dreamrift \
  -I/opt/homebrew/include -L/opt/homebrew/lib \
  -lraylib -framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo -framework CoreAudio
./dreamrift
```

### Controls

- Move: Arrow keys or A/D
- Jump: Space or Up
- Pause: P
- Restart after death: R
- Toggle real fullscreen: F11
