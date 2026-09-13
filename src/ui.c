#include "ui.h"
#include "config.h"
#include <stdio.h>

static const char *PotionName(PotionType t)
{
    switch (t) {
        case POTION_SHIELD:  return "SHIELD";
        case POTION_SPEED:   return "SPEED";
        case POTION_GRAVITY: return "GRAVITY";
        default:             return "";
    }
}
