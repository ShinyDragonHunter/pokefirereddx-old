#ifndef GUARD_POKEMON_DEBUG_H
#define GUARD_POKEMON_DEBUG_H

#include "constants/debug.h"

struct PokemonDebugOptionArrows
{
    u8 arrowSpriteId[2];
    u8 currentDigit;
    void *modifiedValPtr;
};

struct BattleBackground
{
    const void *tileset;
    const void *tilemap;
    const void *entryTileset;
    const void *entryTilemap;
    const void *palette[3];
};

void CB2_Debug_Pokemon(void);

#endif // GUARD_POKEMON_DEBUG_H
