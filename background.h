// ============================================================
//  background.h  --  Prototype for the parallax background
// ============================================================
//  Optional: game.h already declares BackgroundDraw() alongside
//  the rest of world.c's prototypes, so you don't strictly need
//  this header. It's here in case you want background.c to be
//  a fully separate, drop-in-anywhere module with its own header.
// ============================================================

#ifndef BACKGROUND_H
#define BACKGROUND_H

// Draws the full parallax backdrop: sky gradient, stars, far/near
// cloud banks, far/mid/near bubble trees, the ground slab, and the
// ceiling strip (for gravity-flip). Call once per frame, before
// drawing obstacles/coins/player so it stays behind everything.
void BackgroundDraw(void);

#endif // BACKGROUND_H
