#include "global.h"
#include "metatile_behavior.h"
#include "constants/metatile_behaviors.h"

#define TILE_FLAG_ENCOUNTER_TILE 1
#define TILE_FLAG_SURFABLE 2

#define TILE_ATTRIBUTES(unused, surfable, wildEncounter) (((wildEncounter) ? 1 : 0) | ((surfable) ? 2 : 0) | ((unused) ? 4 : 0))

// wonder what the third flag is supposed to do
static const u8 sTileBitAttributes[] =
{
    [MB_NORMAL] = TILE_ATTRIBUTES(TRUE, FALSE, FALSE),
    [MB_UNUSED_01] = TILE_ATTRIBUTES(FALSE, FALSE, FALSE),
    [MB_TALL_GRASS] = TILE_ATTRIBUTES(TRUE, FALSE, TRUE),
    [MB_LONG_GRASS] = TILE_ATTRIBUTES(TRUE, FALSE, TRUE),
    [MB_UNUSED_04] = TILE_ATTRIBUTES(FALSE, FALSE, FALSE),
    [MB_UNUSED_05] = TILE_ATTRIBUTES(FALSE, FALSE, TRUE),
    [MB_DEEP_SAND] = TILE_ATTRIBUTES(TRUE, FALSE, TRUE),
    [MB_SHORT_GRASS] = TILE_ATTRIBUTES(TRUE, FALSE, FALSE),
    [MB_UNUSED_CAVE] = TILE_ATTRIBUTES(TRUE, FALSE, TRUE),
    [MB_LONG_GRASS_SOUTH_EDGE] = TILE_ATTRIBUTES(TRUE, FALSE, FALSE),
    [MB_NO_RUNNING] = TILE_ATTRIBUTES(TRUE, FALSE, FALSE),
    [MB_INDOOR_ENCOUNTER] = TILE_ATTRIBUTES(TRUE, FALSE, TRUE),
    [MB_MOUNTAIN_TOP] = TILE_ATTRIBUTES(TRUE, FALSE, FALSE),
    [MB_BATTLE_PYRAMID_WARP] = TILE_ATTRIBUTES(TRUE, FALSE, FALSE),
    [MB_MOSSDEEP_GYM_WARP] = TILE_ATTRIBUTES(TRUE, FALSE, FALSE),
    [MB_MT_PYRE_HOLE] = TILE_ATTRIBUTES(TRUE, FALSE, FALSE),
    [MB_POND_WATER] = TILE_ATTRIBUTES(TRUE, TRUE, TRUE),
    [MB_SEMI_DEEP_WATER] = TILE_ATTRIBUTES(TRUE, TRUE, TRUE),
    [MB_DEEP_WATER] = TILE_ATTRIBUTES(TRUE, TRUE, TRUE),
    [MB_WATERFALL] = TILE_ATTRIBUTES(TRUE, TRUE, FALSE),
    [MB_SOOTOPOLIS_DEEP_WATER] = TILE_ATTRIBUTES(TRUE, TRUE, FALSE),
    [MB_OCEAN_WATER] = TILE_ATTRIBUTES(TRUE, TRUE, TRUE),
    [MB_PUDDLE] = TILE_ATTRIBUTES(TRUE, FALSE, FALSE),
    [MB_SHALLOW_WATER] = TILE_ATTRIBUTES(TRUE, FALSE, FALSE),
    [MB_UNUSED_18] = TILE_ATTRIBUTES(FALSE, FALSE, FALSE),
    [MB_NO_SURFACING] = TILE_ATTRIBUTES(TRUE, TRUE, FALSE),
    [MB_UNUSED_1A] = TILE_ATTRIBUTES(FALSE, FALSE, FALSE),
    [MB_STAIRS_OUTSIDE_ABANDONED_SHIP] = TILE_ATTRIBUTES(TRUE, FALSE, FALSE),
    [MB_SHOAL_CAVE_ENTRANCE] = TILE_ATTRIBUTES(TRUE, FALSE, FALSE),
    [MB_UNUSED_1D] = TILE_ATTRIBUTES(FALSE, FALSE, FALSE),
    [MB_UNUSED_1E] = TILE_ATTRIBUTES(FALSE, FALSE, FALSE),
    [MB_UNUSED_1F] = TILE_ATTRIBUTES(FALSE, FALSE, FALSE),
    [MB_ICE] = TILE_ATTRIBUTES(TRUE, FALSE, FALSE),
    [MB_SAND] = TILE_ATTRIBUTES(TRUE, FALSE, FALSE),
    [MB_SEAWEED] = TILE_ATTRIBUTES(TRUE, TRUE, TRUE),
    [MB_UNUSED_23] = TILE_ATTRIBUTES(FALSE, FALSE, FALSE),
    [MB_ASHGRASS] = TILE_ATTRIBUTES(TRUE, FALSE, TRUE),
    [MB_FOOTPRINTS] = TILE_ATTRIBUTES(TRUE, FALSE, TRUE),
    [MB_THIN_ICE] = TILE_ATTRIBUTES(TRUE, FALSE, FALSE),
    [MB_CRACKED_ICE] = TILE_ATTRIBUTES(TRUE, FALSE, FALSE),
    [MB_HOT_SPRINGS] = TILE_ATTRIBUTES(TRUE, FALSE, FALSE),
    [MB_LAVARIDGE_GYM_B1F_WARP] = TILE_ATTRIBUTES(TRUE, FALSE, FALSE),
    [MB_SEAWEED_NO_SURFACING] = TILE_ATTRIBUTES(TRUE, TRUE, TRUE),
    [MB_REFLECTION_UNDER_BRIDGE] = TILE_ATTRIBUTES(TRUE, FALSE, FALSE),
    [MB_UNUSED_2C] = TILE_ATTRIBUTES(FALSE, FALSE, FALSE),
    [MB_UNUSED_2D] = TILE_ATTRIBUTES(FALSE, FALSE, FALSE),
    [MB_UNUSED_2E] = TILE_ATTRIBUTES(FALSE, FALSE, FALSE),
    [MB_UNUSED_2F] = TILE_ATTRIBUTES(FALSE, FALSE, FALSE),
    [MB_IMPASSABLE_EAST] = TILE_ATTRIBUTES(TRUE, FALSE, FALSE),
    [MB_IMPASSABLE_WEST] = TILE_ATTRIBUTES(TRUE, FALSE, FALSE),
    [MB_IMPASSABLE_NORTH] = TILE_ATTRIBUTES(TRUE, FALSE, FALSE),
    [MB_IMPASSABLE_SOUTH] = TILE_ATTRIBUTES(TRUE, FALSE, FALSE),
    [MB_IMPASSABLE_NORTHEAST] = TILE_ATTRIBUTES(TRUE, FALSE, FALSE),
    [MB_IMPASSABLE_NORTHWEST] = TILE_ATTRIBUTES(TRUE, FALSE, FALSE),
    [MB_IMPASSABLE_SOUTHEAST] = TILE_ATTRIBUTES(TRUE, FALSE, FALSE),
    [MB_IMPASSABLE_SOUTHWEST] = TILE_ATTRIBUTES(TRUE, FALSE, FALSE),
    [MB_JUMP_EAST] = TILE_ATTRIBUTES(FALSE, FALSE, FALSE),
    [MB_JUMP_WEST] = TILE_ATTRIBUTES(FALSE, FALSE, FALSE),
    [MB_JUMP_NORTH] = TILE_ATTRIBUTES(FALSE, FALSE, FALSE),
    [MB_JUMP_SOUTH] = TILE_ATTRIBUTES(FALSE, FALSE, FALSE),
    [MB_JUMP_NORTHEAST] = TILE_ATTRIBUTES(TRUE, FALSE, FALSE),
    [MB_JUMP_NORTHWEST] = TILE_ATTRIBUTES(TRUE, FALSE, FALSE),
    [MB_JUMP_SOUTHEAST] = TILE_ATTRIBUTES(TRUE, FALSE, FALSE),
    [MB_JUMP_SOUTHWEST] = TILE_ATTRIBUTES(TRUE, FALSE, FALSE),
    [MB_WALK_EAST] = TILE_ATTRIBUTES(TRUE, FALSE, FALSE),
    [MB_WALK_WEST] = TILE_ATTRIBUTES(TRUE, FALSE, FALSE),
    [MB_WALK_NORTH] = TILE_ATTRIBUTES(TRUE, FALSE, FALSE),
    [MB_WALK_SOUTH] = TILE_ATTRIBUTES(TRUE, FALSE, FALSE),
    [MB_SLIDE_EAST] = TILE_ATTRIBUTES(TRUE, FALSE, FALSE),
    [MB_SLIDE_WEST] = TILE_ATTRIBUTES(TRUE, FALSE, FALSE),
    [MB_SLIDE_NORTH] = TILE_ATTRIBUTES(TRUE, FALSE, FALSE),
    [MB_SLIDE_SOUTH] = TILE_ATTRIBUTES(TRUE, FALSE, FALSE),
    [MB_TRICK_HOUSE_PUZZLE_8_FLOOR] = TILE_ATTRIBUTES(TRUE, FALSE, FALSE),
    [MB_UNUSED_49] = TILE_ATTRIBUTES(FALSE, FALSE, FALSE),
    [MB_UNUSED_4A] = TILE_ATTRIBUTES(FALSE, FALSE, FALSE),
    [MB_UNUSED_4B] = TILE_ATTRIBUTES(FALSE, FALSE, FALSE),
    [MB_UNUSED_4C] = TILE_ATTRIBUTES(FALSE, FALSE, FALSE),
    [MB_UNUSED_4D] = TILE_ATTRIBUTES(FALSE, FALSE, FALSE),
    [MB_UNUSED_4E] = TILE_ATTRIBUTES(FALSE, FALSE, FALSE),
    [MB_UNUSED_4F] = TILE_ATTRIBUTES(FALSE, FALSE, FALSE),
    [MB_EASTWARD_CURRENT] = TILE_ATTRIBUTES(TRUE, TRUE, FALSE),
    [MB_WESTWARD_CURRENT] = TILE_ATTRIBUTES(TRUE, TRUE, FALSE),
    [MB_NORTHWARD_CURRENT] = TILE_ATTRIBUTES(TRUE, TRUE, FALSE),
    [MB_SOUTHWARD_CURRENT] = TILE_ATTRIBUTES(TRUE, TRUE, FALSE),
    [MB_UNUSED_54] = TILE_ATTRIBUTES(FALSE, FALSE, FALSE),
    [MB_UNUSED_55] = TILE_ATTRIBUTES(FALSE, FALSE, FALSE),
    [MB_UNUSED_56] = TILE_ATTRIBUTES(FALSE, FALSE, FALSE),
    [MB_UNUSED_57] = TILE_ATTRIBUTES(FALSE, FALSE, FALSE),
    [MB_UNUSED_58] = TILE_ATTRIBUTES(FALSE, FALSE, FALSE),
    [MB_UNUSED_59] = TILE_ATTRIBUTES(FALSE, FALSE, FALSE),
    [MB_UNUSED_5A] = TILE_ATTRIBUTES(FALSE, FALSE, FALSE),
    [MB_UNUSED_5B] = TILE_ATTRIBUTES(FALSE, FALSE, FALSE),
    [MB_UNUSED_5C] = TILE_ATTRIBUTES(FALSE, FALSE, FALSE),
    [MB_UNUSED_5D] = TILE_ATTRIBUTES(FALSE, FALSE, FALSE),
    [MB_UNUSED_5E] = TILE_ATTRIBUTES(FALSE, FALSE, FALSE),
    [MB_UNUSED_5F] = TILE_ATTRIBUTES(FALSE, FALSE, FALSE),
    [MB_NON_ANIMATED_DOOR] = TILE_ATTRIBUTES(TRUE, FALSE, FALSE),
    [MB_LADDER] = TILE_ATTRIBUTES(TRUE, FALSE, FALSE),
    [MB_EAST_ARROW_WARP] = TILE_ATTRIBUTES(TRUE, FALSE, FALSE),
    [MB_WEST_ARROW_WARP] = TILE_ATTRIBUTES(TRUE, FALSE, FALSE),
    [MB_NORTH_ARROW_WARP] = TILE_ATTRIBUTES(TRUE, FALSE, FALSE),
    [MB_SOUTH_ARROW_WARP] = TILE_ATTRIBUTES(TRUE, FALSE, FALSE),
    [MB_CRACKED_FLOOR_HOLE] = TILE_ATTRIBUTES(TRUE, FALSE, FALSE),
    [MB_AQUA_HIDEOUT_WARP] = TILE_ATTRIBUTES(TRUE, FALSE, FALSE),
    [MB_LAVARIDGE_GYM_1F_WARP] = TILE_ATTRIBUTES(TRUE, FALSE, FALSE),
    [MB_ANIMATED_DOOR] = TILE_ATTRIBUTES(TRUE, FALSE, FALSE),
    [MB_UP_ESCALATOR] = TILE_ATTRIBUTES(TRUE, FALSE, FALSE),
    [MB_DOWN_ESCALATOR] = TILE_ATTRIBUTES(TRUE, FALSE, FALSE),
    [MB_WATER_DOOR] = TILE_ATTRIBUTES(TRUE, TRUE, FALSE),
    [MB_WATER_SOUTH_ARROW_WARP] = TILE_ATTRIBUTES(TRUE, TRUE, FALSE),
    [MB_DEEP_SOUTH_WARP] = TILE_ATTRIBUTES(TRUE, FALSE, FALSE),
    [MB_UNUSED_6F] = TILE_ATTRIBUTES(FALSE, FALSE, FALSE),
    [MB_WARP_OR_BRIDGE] = TILE_ATTRIBUTES(FALSE, FALSE, FALSE),
    [MB_UNUSED_71] = TILE_ATTRIBUTES(FALSE, FALSE, FALSE),
    [MB_ROUTE120_NORTH_BRIDGE_1] = TILE_ATTRIBUTES(TRUE, FALSE, FALSE),
    [MB_ROUTE120_NORTH_BRIDGE_2] = TILE_ATTRIBUTES(TRUE, FALSE, FALSE),
    [MB_PACIFIDLOG_VERTICAL_LOG_1] = TILE_ATTRIBUTES(TRUE, FALSE, FALSE),
    [MB_PACIFIDLOG_VERTICAL_LOG_2] = TILE_ATTRIBUTES(TRUE, FALSE, FALSE),
    [MB_PACIFIDLOG_HORIZONTAL_LOG_1] = TILE_ATTRIBUTES(TRUE, FALSE, FALSE),
    [MB_PACIFIDLOG_HORIZONTAL_LOG_2] = TILE_ATTRIBUTES(TRUE, FALSE, FALSE),
    [MB_FORTREE_BRIDGE] = TILE_ATTRIBUTES(TRUE, FALSE, FALSE),
    [MB_UNUSED_79] = TILE_ATTRIBUTES(FALSE, FALSE, FALSE),
    [MB_ROUTE120_SOUTH_BRIDGE_1] = TILE_ATTRIBUTES(TRUE, FALSE, FALSE),
    [MB_ROUTE120_SOUTH_BRIDGE_2] = TILE_ATTRIBUTES(TRUE, FALSE, FALSE),
    [MB_ROUTE120_NORTH_BRIDGE_3] = TILE_ATTRIBUTES(TRUE, FALSE, FALSE),
    [MB_ROUTE120_NORTH_BRIDGE_4] = TILE_ATTRIBUTES(TRUE, FALSE, FALSE),
    [MB_UNUSED_7E] = TILE_ATTRIBUTES(TRUE, FALSE, FALSE),
    [MB_ROUTE110_BRIDGE] = TILE_ATTRIBUTES(TRUE, FALSE, FALSE),
    [MB_COUNTER] = TILE_ATTRIBUTES(FALSE, FALSE, FALSE),
    [MB_UNUSED_81] = TILE_ATTRIBUTES(FALSE, FALSE, FALSE),
    [MB_UNUSED_82] = TILE_ATTRIBUTES(FALSE, FALSE, FALSE),
    [MB_PC] = TILE_ATTRIBUTES(FALSE, FALSE, FALSE),
    [MB_CABLE_BOX_RESULTS_1] = TILE_ATTRIBUTES(FALSE, FALSE, FALSE),
    [MB_REGION_MAP] = TILE_ATTRIBUTES(FALSE, FALSE, FALSE),
    [MB_TELEVISION] = TILE_ATTRIBUTES(FALSE, FALSE, FALSE),
    [MB_POKEBLOCK_FEEDER] = TILE_ATTRIBUTES(FALSE, FALSE, FALSE),
    [MB_UNUSED_88] = TILE_ATTRIBUTES(FALSE, FALSE, FALSE),
    [MB_SLOT_MACHINE] = TILE_ATTRIBUTES(FALSE, FALSE, FALSE),
    [MB_ROULETTE] = TILE_ATTRIBUTES(FALSE, FALSE, FALSE),
    [MB_CLOSED_SOOTOPOLIS_DOOR] = TILE_ATTRIBUTES(FALSE, FALSE, FALSE),
    [MB_TRICK_HOUSE_PUZZLE_DOOR] = TILE_ATTRIBUTES(FALSE, FALSE, FALSE),
    [MB_PETALBURG_GYM_DOOR] = TILE_ATTRIBUTES(FALSE, FALSE, FALSE),
    [MB_RUNNING_SHOES_INSTRUCTION] = TILE_ATTRIBUTES(FALSE, FALSE, FALSE),
    [MB_QUESTIONNAIRE] = TILE_ATTRIBUTES(FALSE, FALSE, FALSE),
    [MB_UNUSED_90] = TILE_ATTRIBUTES(FALSE, FALSE, FALSE),
    [MB_UNUSED_91] = TILE_ATTRIBUTES(FALSE, FALSE, FALSE),
    [MB_UNUSED_92] = TILE_ATTRIBUTES(FALSE, FALSE, FALSE),
    [MB_UNUSED_93] = TILE_ATTRIBUTES(FALSE, FALSE, FALSE),
    [MB_UNUSED_94] = TILE_ATTRIBUTES(FALSE, FALSE, FALSE),
    [MB_UNUSED_95] = TILE_ATTRIBUTES(FALSE, FALSE, FALSE),
    [MB_UNUSED_96] = TILE_ATTRIBUTES(FALSE, FALSE, FALSE),
    [MB_UNUSED_97] = TILE_ATTRIBUTES(FALSE, FALSE, FALSE),
    [MB_UNUSED_98] = TILE_ATTRIBUTES(FALSE, FALSE, FALSE),
    [MB_UNUSED_99] = TILE_ATTRIBUTES(FALSE, FALSE, FALSE),
    [MB_UNUSED_9A] = TILE_ATTRIBUTES(FALSE, FALSE, FALSE),
    [MB_UNUSED_9B] = TILE_ATTRIBUTES(FALSE, FALSE, FALSE),
    [MB_UNUSED_9C] = TILE_ATTRIBUTES(FALSE, FALSE, FALSE),
    [MB_UNUSED_9D] = TILE_ATTRIBUTES(FALSE, FALSE, FALSE),
    [MB_UNUSED_9E] = TILE_ATTRIBUTES(FALSE, FALSE, FALSE),
    [MB_UNUSED_9F] = TILE_ATTRIBUTES(FALSE, FALSE, FALSE),
    [MB_BERRY_TREE_SOIL] = TILE_ATTRIBUTES(FALSE, FALSE, FALSE),
    [MB_UNUSED_A1] = TILE_ATTRIBUTES(FALSE, FALSE, FALSE),
    [MB_UNUSED_A2] = TILE_ATTRIBUTES(FALSE, FALSE, FALSE),
    [MB_UNUSED_A3] = TILE_ATTRIBUTES(FALSE, FALSE, FALSE),
    [MB_UNUSED_A4] = TILE_ATTRIBUTES(FALSE, FALSE, FALSE),
    [MB_UNUSED_A5] = TILE_ATTRIBUTES(FALSE, FALSE, FALSE),
    [MB_UNUSED_A6] = TILE_ATTRIBUTES(FALSE, FALSE, FALSE),
    [MB_UNUSED_A7] = TILE_ATTRIBUTES(FALSE, FALSE, FALSE),
    [MB_UNUSED_A8] = TILE_ATTRIBUTES(FALSE, FALSE, FALSE),
    [MB_UNUSED_A9] = TILE_ATTRIBUTES(FALSE, FALSE, FALSE),
    [MB_UNUSED_AA] = TILE_ATTRIBUTES(FALSE, FALSE, FALSE),
    [MB_UNUSED_AB] = TILE_ATTRIBUTES(FALSE, FALSE, FALSE),
    [MB_UNUSED_AC] = TILE_ATTRIBUTES(FALSE, FALSE, FALSE),
    [MB_UNUSED_AD] = TILE_ATTRIBUTES(FALSE, FALSE, FALSE),
    [MB_UNUSED_AE] = TILE_ATTRIBUTES(FALSE, FALSE, FALSE),
    [MB_UNUSED_AF] = TILE_ATTRIBUTES(FALSE, FALSE, FALSE),
    [MB_UNUSED_B0] = TILE_ATTRIBUTES(FALSE, FALSE, FALSE),
    [MB_UNUSED_B1] = TILE_ATTRIBUTES(FALSE, FALSE, FALSE),
    [MB_UNUSED_B2] = TILE_ATTRIBUTES(FALSE, FALSE, FALSE),
    [MB_UNUSED_B3] = TILE_ATTRIBUTES(FALSE, FALSE, FALSE),
    [MB_UNUSED_B4] = TILE_ATTRIBUTES(FALSE, FALSE, FALSE),
    [MB_UNUSED_B5] = TILE_ATTRIBUTES(FALSE, FALSE, FALSE),
    [MB_UNUSED_B6] = TILE_ATTRIBUTES(FALSE, FALSE, FALSE),
    [MB_UNUSED_B7] = TILE_ATTRIBUTES(FALSE, FALSE, FALSE),
    [MB_UNUSED_B8] = TILE_ATTRIBUTES(FALSE, FALSE, FALSE),
    [MB_UNUSED_B9] = TILE_ATTRIBUTES(FALSE, FALSE, FALSE),
    [MB_UNUSED_BA] = TILE_ATTRIBUTES(FALSE, FALSE, FALSE),
    [MB_UNUSED_BB] = TILE_ATTRIBUTES(FALSE, FALSE, FALSE),
    [MB_UNUSED_BC] = TILE_ATTRIBUTES(FALSE, FALSE, FALSE),
    [MB_UNUSED_BD] = TILE_ATTRIBUTES(FALSE, FALSE, FALSE),
    [MB_UNUSED_BE] = TILE_ATTRIBUTES(FALSE, FALSE, FALSE),
    [MB_UNUSED_BF] = TILE_ATTRIBUTES(FALSE, FALSE, FALSE),
    [MB_IMPASSABLE_SOUTH_AND_NORTH] = TILE_ATTRIBUTES(TRUE, FALSE, FALSE),
    [MB_IMPASSABLE_WEST_AND_EAST] = TILE_ATTRIBUTES(TRUE, FALSE, FALSE),
    [MB_UNUSED_C2] = TILE_ATTRIBUTES(FALSE, FALSE, FALSE),
    [MB_UNUSED_C3] = TILE_ATTRIBUTES(FALSE, FALSE, FALSE),
    [MB_UNUSED_C4] = TILE_ATTRIBUTES(FALSE, FALSE, FALSE),
    [MB_PLAYER_ROOM_PC_ON] = TILE_ATTRIBUTES(TRUE, FALSE, FALSE),
    [MB_C6] = TILE_ATTRIBUTES(FALSE, FALSE, FALSE),
    [MB_UNUSED_C7] = TILE_ATTRIBUTES(FALSE, FALSE, FALSE),
    [MB_UNUSED_C8] = TILE_ATTRIBUTES(FALSE, FALSE, FALSE),
    [MB_UNUSED_C9] = TILE_ATTRIBUTES(FALSE, FALSE, FALSE),
    [MB_UNUSED_CA] = TILE_ATTRIBUTES(FALSE, FALSE, FALSE),
    [MB_UNUSED_CB] = TILE_ATTRIBUTES(FALSE, FALSE, FALSE),
    [MB_UNUSED_CC] = TILE_ATTRIBUTES(FALSE, FALSE, FALSE),
    [MB_UNUSED_CD] = TILE_ATTRIBUTES(FALSE, FALSE, FALSE),
    [MB_UNUSED_CE] = TILE_ATTRIBUTES(FALSE, FALSE, FALSE),
    [MB_UNUSED_CF] = TILE_ATTRIBUTES(FALSE, FALSE, FALSE),
    [MB_MUDDY_SLOPE] = TILE_ATTRIBUTES(TRUE, FALSE, FALSE),
    [MB_BUMPY_SLOPE] = TILE_ATTRIBUTES(TRUE, FALSE, FALSE),
    [MB_CRACKED_FLOOR] = TILE_ATTRIBUTES(TRUE, FALSE, FALSE),
    [MB_ISOLATED_VERTICAL_RAIL] = TILE_ATTRIBUTES(TRUE, FALSE, FALSE),
    [MB_ISOLATED_HORIZONTAL_RAIL] = TILE_ATTRIBUTES(TRUE, FALSE, FALSE),
    [MB_VERTICAL_RAIL] = TILE_ATTRIBUTES(TRUE, FALSE, FALSE),
    [MB_HORIZONTAL_RAIL] = TILE_ATTRIBUTES(TRUE, FALSE, FALSE),
    [MB_UNUSED_D7] = TILE_ATTRIBUTES(FALSE, FALSE, FALSE),
    [MB_UNUSED_D8] = TILE_ATTRIBUTES(FALSE, FALSE, FALSE),
    [MB_UNUSED_D9] = TILE_ATTRIBUTES(FALSE, FALSE, FALSE),
    [MB_UNUSED_DA] = TILE_ATTRIBUTES(FALSE, FALSE, FALSE),
    [MB_UNUSED_DB] = TILE_ATTRIBUTES(FALSE, FALSE, FALSE),
    [MB_UNUSED_DC] = TILE_ATTRIBUTES(FALSE, FALSE, FALSE),
    [MB_UNUSED_DD] = TILE_ATTRIBUTES(FALSE, FALSE, FALSE),
    [MB_UNUSED_DE] = TILE_ATTRIBUTES(FALSE, FALSE, FALSE),
    [MB_UNUSED_DF] = TILE_ATTRIBUTES(FALSE, FALSE, FALSE),
    [MB_PICTURE_BOOK_SHELF] = TILE_ATTRIBUTES(FALSE, FALSE, FALSE),
    [MB_BOOKSHELF] = TILE_ATTRIBUTES(FALSE, FALSE, FALSE),
    [MB_POKEMON_CENTER_BOOKSHELF] = TILE_ATTRIBUTES(FALSE, FALSE, FALSE),
    [MB_VASE] = TILE_ATTRIBUTES(FALSE, FALSE, FALSE),
    [MB_TRASH_CAN] = TILE_ATTRIBUTES(FALSE, FALSE, FALSE),
    [MB_SHOP_SHELF] = TILE_ATTRIBUTES(FALSE, FALSE, FALSE),
    [MB_BLUEPRINT] = TILE_ATTRIBUTES(FALSE, FALSE, FALSE),
    [MB_CABLE_BOX_RESULTS_2] = TILE_ATTRIBUTES(FALSE, FALSE, FALSE),
    [MB_WIRELESS_BOX_RESULTS] = TILE_ATTRIBUTES(FALSE, FALSE, FALSE),
    [MB_TRAINER_HILL_TIMER] = TILE_ATTRIBUTES(FALSE, FALSE, FALSE),
    [MB_SKY_PILLAR_CLOSED_DOOR] = TILE_ATTRIBUTES(FALSE, FALSE, FALSE),
    [MB_UNUSED_EB] = TILE_ATTRIBUTES(FALSE, FALSE, FALSE),
    [MB_UNUSED_EC] = TILE_ATTRIBUTES(FALSE, FALSE, FALSE),
    [MB_UNUSED_ED] = TILE_ATTRIBUTES(FALSE, FALSE, FALSE),
    [MB_UNUSED_EE] = TILE_ATTRIBUTES(FALSE, FALSE, FALSE),
    [MB_UNUSED_EF] = TILE_ATTRIBUTES(FALSE, FALSE, FALSE),
};

bool8 MetatileBehavior_IsATile(u8 metatileBehavior)
{
    return TRUE;
}

bool8 MetatileBehavior_IsEncounterTile(u8 metatileBehavior)
{
    if ((sTileBitAttributes[metatileBehavior] & TILE_FLAG_ENCOUNTER_TILE))
        return TRUE;
    return FALSE;
}

bool8 MetatileBehavior_IsJumpEast(u8 metatileBehavior)
{
    if (metatileBehavior == MB_JUMP_EAST)
        return TRUE;
    return FALSE;
}

bool8 MetatileBehavior_IsJumpWest(u8 metatileBehavior)
{
    if (metatileBehavior == MB_JUMP_WEST)
        return TRUE;
    return FALSE;
}

bool8 MetatileBehavior_IsJumpNorth(u8 metatileBehavior)
{
    if (metatileBehavior == MB_JUMP_NORTH)
        return TRUE;
    return FALSE;
}

bool8 MetatileBehavior_IsJumpSouth(u8 metatileBehavior)
{
    if (metatileBehavior == MB_JUMP_SOUTH)
        return TRUE;
    return FALSE;
}

bool8 MetatileBehavior_IsPokeGrass(u8 metatileBehavior)
{
    if (metatileBehavior == MB_TALL_GRASS
     || metatileBehavior == MB_LONG_GRASS)
        return TRUE;
    return FALSE;
}

bool8 MetatileBehavior_IsSandOrDeepSand(u8 metatileBehavior)
{
    if (metatileBehavior == MB_SAND
     || metatileBehavior == MB_DEEP_SAND)
        return TRUE;
    return FALSE;
}

bool8 MetatileBehavior_IsDeepSand(u8 metatileBehavior)
{
    if (metatileBehavior == MB_DEEP_SAND)
        return TRUE;
    return FALSE;
}

bool8 MetatileBehavior_IsReflective(u8 metatileBehavior)
{
    if (metatileBehavior == MB_POND_WATER
     || metatileBehavior == MB_PUDDLE
     || metatileBehavior == MB_ICE
     || metatileBehavior == MB_SOOTOPOLIS_DEEP_WATER
     || metatileBehavior == MB_REFLECTION_UNDER_BRIDGE)
        return TRUE;
    return FALSE;
}

bool8 MetatileBehavior_IsIce(u8 metatileBehavior)
{
    if (metatileBehavior == MB_ICE)
        return TRUE;
    return FALSE;
}

bool8 MetatileBehavior_IsWarpDoor(u8 metatileBehavior)
{
    if (metatileBehavior == MB_ANIMATED_DOOR)
        return TRUE;
    return FALSE;
}

bool8 MetatileBehavior_IsDoor(u8 metatileBehavior)
{
    if (metatileBehavior == MB_PETALBURG_GYM_DOOR
     || metatileBehavior == MB_ANIMATED_DOOR)
        return TRUE;
    return FALSE;
}

bool8 MetatileBehavior_IsEscalator(u8 metatileBehavior)
{
    if (metatileBehavior == MB_UP_ESCALATOR
     || metatileBehavior == MB_DOWN_ESCALATOR)
        return TRUE;
    return FALSE;
}

bool8 MetatileBehavior_IsLadder(u8 metatileBehavior)
{
    if (metatileBehavior == MB_LADDER)
        return TRUE;
    return FALSE;
}

bool8 MetatileBehavior_IsNonAnimDoor(u8 metatileBehavior)
{
    if (metatileBehavior == MB_NON_ANIMATED_DOOR
     || metatileBehavior == MB_WATER_DOOR
     || metatileBehavior == MB_DEEP_SOUTH_WARP)
        return TRUE;
    return FALSE;
}

bool8 MetatileBehavior_IsDeepSouthWarp(u8 metatileBehavior)
{
    if (metatileBehavior == MB_DEEP_SOUTH_WARP)
        return TRUE;
    return FALSE;
}

bool8 MetatileBehavior_IsSurfableWaterOrUnderwater(u8 metatileBehavior)
{
    if ((sTileBitAttributes[metatileBehavior] & TILE_FLAG_SURFABLE))
        return TRUE;
    return FALSE;
}

bool8 MetatileBehavior_IsEastArrowWarp(u8 metatileBehavior)
{
    if (metatileBehavior == MB_EAST_ARROW_WARP)
        return TRUE;
    return FALSE;
}

bool8 MetatileBehavior_IsWestArrowWarp(u8 metatileBehavior)
{
    if (metatileBehavior == MB_WEST_ARROW_WARP)
        return TRUE;
    return FALSE;
}

bool8 MetatileBehavior_IsNorthArrowWarp(u8 metatileBehavior)
{
    if (metatileBehavior == MB_NORTH_ARROW_WARP
     || metatileBehavior == MB_STAIRS_OUTSIDE_ABANDONED_SHIP)
        return TRUE;
    return FALSE;
}

bool8 MetatileBehavior_IsSouthArrowWarp(u8 metatileBehavior)
{
    if (metatileBehavior == MB_SOUTH_ARROW_WARP
     || metatileBehavior == MB_WATER_SOUTH_ARROW_WARP
     || metatileBehavior == MB_SHOAL_CAVE_ENTRANCE)
        return TRUE;
    return FALSE;
}

bool8 MetatileBehavior_IsForcedMovementTile(u8 metatileBehavior)
{
    if ((metatileBehavior >= MB_WALK_EAST && metatileBehavior <= MB_TRICK_HOUSE_PUZZLE_8_FLOOR)
     || (metatileBehavior >= MB_EASTWARD_CURRENT && metatileBehavior <= MB_SOUTHWARD_CURRENT)
     || metatileBehavior == MB_MUDDY_SLOPE
     || metatileBehavior == MB_CRACKED_FLOOR
     || metatileBehavior == MB_WATERFALL
     || metatileBehavior == MB_ICE)
        return TRUE;
    return FALSE;
}

bool8 MetatileBehavior_IsIce_2(u8 metatileBehavior)
{
    if (metatileBehavior == MB_ICE)
        return TRUE;
    return FALSE;
}

bool8 MetatileBehavior_IsTrickHouseSlipperyFloor(u8 metatileBehavior)
{
    if (metatileBehavior == MB_TRICK_HOUSE_PUZZLE_8_FLOOR)
        return TRUE;
    return FALSE;
}

bool8 MetatileBehavior_IsWalkNorth(u8 metatileBehavior)
{
    if (metatileBehavior == MB_WALK_NORTH)
        return TRUE;
    return FALSE;
}

bool8 MetatileBehavior_IsWalkSouth(u8 metatileBehavior)
{
    if (metatileBehavior == MB_WALK_SOUTH)
        return TRUE;
    return FALSE;
}

bool8 MetatileBehavior_IsWalkWest(u8 metatileBehavior)
{
    if (metatileBehavior == MB_WALK_WEST)
        return TRUE;
    return FALSE;
}

bool8 MetatileBehavior_IsWalkEast(u8 metatileBehavior)
{
    if (metatileBehavior == MB_WALK_EAST)
        return TRUE;
    return FALSE;
}

bool8 MetatileBehavior_IsNorthwardCurrent(u8 metatileBehavior)
{
    if (metatileBehavior == MB_NORTHWARD_CURRENT)
        return TRUE;
    return FALSE;
}

bool8 MetatileBehavior_IsSouthwardCurrent(u8 metatileBehavior)
{
    if (metatileBehavior == MB_SOUTHWARD_CURRENT)
        return TRUE;
    return FALSE;
}

bool8 MetatileBehavior_IsWestwardCurrent(u8 metatileBehavior)
{
    if (metatileBehavior == MB_WESTWARD_CURRENT)
        return TRUE;
    return FALSE;
}

bool8 MetatileBehavior_IsEastwardCurrent(u8 metatileBehavior)
{
    if (metatileBehavior == MB_EASTWARD_CURRENT)
        return TRUE;
    return FALSE;
}

bool8 MetatileBehavior_IsSlideNorth(u8 metatileBehavior)
{
    if (metatileBehavior == MB_SLIDE_NORTH)
        return TRUE;
    return FALSE;
}

bool8 MetatileBehavior_IsSlideSouth(u8 metatileBehavior)
{
    if (metatileBehavior == MB_SLIDE_SOUTH)
        return TRUE;
    return FALSE;
}

bool8 MetatileBehavior_IsSlideWest(u8 metatileBehavior)
{
    if (metatileBehavior == MB_SLIDE_WEST)
        return TRUE;
    return FALSE;
}

bool8 MetatileBehavior_IsSlideEast(u8 metatileBehavior)
{
    if (metatileBehavior == MB_SLIDE_EAST)
        return TRUE;
    return FALSE;
}

bool8 MetatileBehavior_IsCounter(u8 metatileBehavior)
{
    if (metatileBehavior == MB_COUNTER)
        return TRUE;
    return FALSE;
}

bool8 MetatileBehavior_IsPlayerFacingTVScreen(u8 metatileBehavior, u8 playerDir)
{
    if (playerDir != DIR_NORTH)
        return FALSE;
    else if (metatileBehavior == MB_TELEVISION)
        return TRUE;
    return FALSE;
}

bool8 MetatileBehavior_IsPC(u8 metatileBehavior)
{
    if (metatileBehavior == MB_PC)
        return TRUE;
    return FALSE;
}

bool8 MetatileBehavior_IsCableBoxResults1(u8 metatileBehavior)
{
    if (metatileBehavior == MB_CABLE_BOX_RESULTS_1)
        return TRUE;
    return FALSE;
}

bool8 MetatileBehavior_IsMB_C6(u8 metatileBehavior)
{
    if (metatileBehavior == MB_C6)
        return TRUE;
    return FALSE;
}

bool8 MetatileBehavior_IsNormal(u8 metatileBehavior)
{
    if (metatileBehavior == MB_NORMAL)
        return TRUE;
    return FALSE;
}

bool8 MetatileBehavior_IsPlayerRoomPCOn(u8 metatileBehavior)
{
    if (metatileBehavior == MB_PLAYER_ROOM_PC_ON)
        return TRUE;
    return FALSE;
}

bool8 MetatileBehavior_HasRipples(u8 metatileBehavior)
{
    if (metatileBehavior == MB_POND_WATER
     || metatileBehavior == MB_PUDDLE
     || metatileBehavior == MB_SOOTOPOLIS_DEEP_WATER)
        return TRUE;
    return FALSE;
}

bool8 MetatileBehavior_IsPuddle(u8 metatileBehavior)
{
    if (metatileBehavior == MB_PUDDLE)
        return TRUE;
    return FALSE;
}

bool8 MetatileBehavior_IsTallGrass(u8 metatileBehavior)
{
    if (metatileBehavior == MB_TALL_GRASS)
        return TRUE;
    return FALSE;
}

bool8 MetatileBehavior_IsLongGrass(u8 metatileBehavior)
{
    if (metatileBehavior == MB_LONG_GRASS)
        return TRUE;
    return FALSE;
}

bool8 MetatileBehavior_IsBerryTreeSoil(u8 metatileBehavior)
{
    if (metatileBehavior == MB_BERRY_TREE_SOIL)
        return TRUE;
    return FALSE;
}

bool8 MetatileBehavior_IsAshGrass(u8 metatileBehavior)
{
    if (metatileBehavior == MB_ASHGRASS)
        return TRUE;
    return FALSE;
}

bool8 MetatileBehavior_IsFootprints(u8 metatileBehavior)
{
    // MB_FOOTPRINTS is not used by any metatiles.
    if (metatileBehavior == MB_FOOTPRINTS)
        return TRUE;
    return FALSE;
}

bool8 MetatileBehavior_IsBridge(u8 metatileBehavior)
{
    if ((metatileBehavior == MB_WARP_OR_BRIDGE
     || metatileBehavior == MB_ROUTE120_NORTH_BRIDGE_1
     || metatileBehavior == MB_ROUTE120_NORTH_BRIDGE_2)
     || (metatileBehavior == MB_ROUTE120_NORTH_BRIDGE_3
     || metatileBehavior == MB_ROUTE120_NORTH_BRIDGE_4
     || metatileBehavior == MB_ROUTE110_BRIDGE))
        return TRUE;
    return FALSE;
}

u8 MetatileBehavior_GetBridgeType(u8 metatileBehavior)
{
    u8 result = metatileBehavior - MB_WARP_OR_BRIDGE;
    if (result < 4)
        return result;

    result = metatileBehavior - MB_ROUTE120_SOUTH_BRIDGE_1;
    if (result < 2)
        return 2;

    result = metatileBehavior - MB_ROUTE120_NORTH_BRIDGE_3;
    if (result < 2)
        return 3;
    return 0;
}

u8 MetatileBehavior_8089510(u8 metatileBehavior)
{
    u8 result = metatileBehavior - MB_WARP_OR_BRIDGE;

    if (result < 4)
        return 1;
    return 0;
}

bool8 MetatileBehavior_IsLandWildEncounter(u8 metatileBehavior)
{
    if (!MetatileBehavior_IsSurfableWaterOrUnderwater(metatileBehavior)&& MetatileBehavior_IsEncounterTile(metatileBehavior))
        return TRUE;
    return FALSE;
}

bool8 MetatileBehavior_IsWaterWildEncounter(u8 metatileBehavior)
{
    if (MetatileBehavior_IsSurfableWaterOrUnderwater(metatileBehavior) && MetatileBehavior_IsEncounterTile(metatileBehavior))
        return TRUE;
    return FALSE;
}

bool8 MetatileBehavior_IsIndoorEncounter(u8 metatileBehavior)
{
    if (metatileBehavior == MB_INDOOR_ENCOUNTER)
        return TRUE;
    return FALSE;
}

bool8 MetatileBehavior_IsMountain(u8 metatileBehavior)
{
    if (metatileBehavior == MB_MOUNTAIN_TOP)
        return TRUE;
    else
        return FALSE;
}

bool8 MetatileBehavior_IsDiveable(u8 metatileBehavior)
{
    if (metatileBehavior == MB_SEMI_DEEP_WATER
     || metatileBehavior == MB_DEEP_WATER
     || metatileBehavior == MB_SOOTOPOLIS_DEEP_WATER)
        return TRUE;
    return FALSE;
}

bool8 MetatileBehavior_IsUnableToEmerge(u8 metatileBehavior)
{
    if (metatileBehavior == MB_NO_SURFACING
     || metatileBehavior == MB_SEAWEED_NO_SURFACING
     || metatileBehavior == MB_WATER_DOOR)
        return TRUE;
    return FALSE;
}

bool8 MetatileBehavior_IsShallowFlowingWater(u8 metatileBehavior)
{
    if (metatileBehavior == MB_SHALLOW_WATER
     || metatileBehavior == MB_STAIRS_OUTSIDE_ABANDONED_SHIP
     || metatileBehavior == MB_SHOAL_CAVE_ENTRANCE)
        return TRUE;
    return FALSE;
}

bool8 MetatileBehavior_IsThinIce(u8 metatileBehavior)
{
    if (metatileBehavior == MB_THIN_ICE)
        return TRUE;
    return FALSE;
}

bool8 MetatileBehavior_IsCrackedIce(u8 metatileBehavior)
{
    if (metatileBehavior == MB_CRACKED_ICE)
        return TRUE;
    return FALSE;
}

bool8 MetatileBehavior_IsDeepOrOceanWater(u8 metatileBehavior)
{
    if (metatileBehavior == MB_OCEAN_WATER
     || metatileBehavior == MB_SEMI_DEEP_WATER
     || metatileBehavior == MB_DEEP_WATER)
        return TRUE;
    return FALSE;
}

bool8 MetatileBehavior_IsSurfableAndNotWaterfall(u8 metatileBehavior)
{
    if (MetatileBehavior_IsSurfableWaterOrUnderwater(metatileBehavior)
     && !MetatileBehavior_IsWaterfall(metatileBehavior))
        return TRUE;
    return FALSE;
}

bool8 MetatileBehavior_IsEastBlocked(u8 metatileBehavior)
{
    if (metatileBehavior == MB_IMPASSABLE_EAST
     || metatileBehavior == MB_IMPASSABLE_NORTHEAST
     || metatileBehavior == MB_IMPASSABLE_SOUTHEAST
     || metatileBehavior == MB_IMPASSABLE_WEST_AND_EAST)
        return TRUE;
    return FALSE;
}

bool8 MetatileBehavior_IsWestBlocked(u8 metatileBehavior)
{
    if (metatileBehavior == MB_IMPASSABLE_WEST
     || metatileBehavior == MB_IMPASSABLE_NORTHWEST
     || metatileBehavior == MB_IMPASSABLE_SOUTHWEST
     || metatileBehavior == MB_IMPASSABLE_WEST_AND_EAST)
        return TRUE;
    return FALSE;
}

bool8 MetatileBehavior_IsNorthBlocked(u8 metatileBehavior)
{
    if (metatileBehavior == MB_IMPASSABLE_NORTH
     || metatileBehavior == MB_IMPASSABLE_NORTHEAST
     || metatileBehavior == MB_IMPASSABLE_NORTHWEST
     || metatileBehavior == MB_IMPASSABLE_SOUTH_AND_NORTH)
        return TRUE;
    return FALSE;
}

bool8 MetatileBehavior_IsSouthBlocked(u8 metatileBehavior)
{
    if (metatileBehavior == MB_IMPASSABLE_SOUTH
     || metatileBehavior == MB_IMPASSABLE_SOUTHEAST
     || metatileBehavior == MB_IMPASSABLE_SOUTHWEST
     || metatileBehavior == MB_IMPASSABLE_SOUTH_AND_NORTH)
        return TRUE;
    return FALSE;
}

bool8 MetatileBehavior_IsShortGrass(u8 metatileBehavior)
{
    if (metatileBehavior == MB_SHORT_GRASS)
        return TRUE;
    return FALSE;
}

bool8 MetatileBehavior_IsHotSprings(u8 metatileBehavior)
{
    if (metatileBehavior == MB_HOT_SPRINGS)
        return TRUE;
    return FALSE;
}

bool8 MetatileBehavior_IsWaterfall(u8 metatileBehavior)
{
    if (metatileBehavior == MB_WATERFALL)
        return TRUE;
    return FALSE;
}

bool8 MetatileBehavior_IsFortreeBridge(u8 metatileBehavior)
{
    if (metatileBehavior == MB_FORTREE_BRIDGE)
        return TRUE;
    return FALSE;
}

bool8 MetatileBehavior_IsPacifidlogVerticalLog1(u8 metatileBehavior)
{
    if (metatileBehavior == MB_PACIFIDLOG_VERTICAL_LOG_1)
        return TRUE;
    return FALSE;
}

bool8 MetatileBehavior_IsPacifidlogVerticalLog2(u8 metatileBehavior)
{
    if (metatileBehavior == MB_PACIFIDLOG_VERTICAL_LOG_2)
        return TRUE;
    return FALSE;
}

bool8 MetatileBehavior_IsPacifidlogHorizontalLog1(u8 metatileBehavior)
{
    if (metatileBehavior == MB_PACIFIDLOG_HORIZONTAL_LOG_1)
        return TRUE;
    return FALSE;
}

bool8 MetatileBehavior_IsPacifidlogHorizontalLog2(u8 metatileBehavior)
{
    if (metatileBehavior == MB_PACIFIDLOG_HORIZONTAL_LOG_2)
        return TRUE;
    return FALSE;
}

bool8 MetatileBehavior_IsPacifidlogLog(u8 metatileBehavior)
{
    if (metatileBehavior == MB_PACIFIDLOG_VERTICAL_LOG_1
     || metatileBehavior == MB_PACIFIDLOG_VERTICAL_LOG_2
     || metatileBehavior == MB_PACIFIDLOG_HORIZONTAL_LOG_1
     || metatileBehavior == MB_PACIFIDLOG_HORIZONTAL_LOG_2)
        return TRUE;
    return FALSE;
}

bool8 MetatileBehavior_IsTrickHousePuzzleDoor(u8 metatileBehavior)
{
    if (metatileBehavior == MB_TRICK_HOUSE_PUZZLE_DOOR)
        return TRUE;
    return FALSE;
}

bool8 MetatileBehavior_IsRegionMap(u8 metatileBehavior)
{
    if (metatileBehavior == MB_REGION_MAP)
        return TRUE;
    return FALSE;
}

bool8 MetatileBehavior_IsClosedSootopolisDoor(u8 metatileBehavior)
{
    if (metatileBehavior == MB_CLOSED_SOOTOPOLIS_DOOR)
        return TRUE;
    return FALSE;
}

bool8 MetatileBehavior_IsSkyPillarClosedDoor(u8 metatileBehavior)
{
    if (metatileBehavior == MB_SKY_PILLAR_CLOSED_DOOR)
        return TRUE;
    return FALSE;
}

bool8 MetatileBehavior_IsPokeblockFeeder(u8 metatileBehavior)
{
    if (metatileBehavior == MB_POKEBLOCK_FEEDER)
        return TRUE;
    return FALSE;
}

bool8 MetatileBehavior_IsLavaridgeB1FWarp(u8 metatileBehavior)
{
    if (metatileBehavior == MB_LAVARIDGE_GYM_B1F_WARP)
        return TRUE;
    return FALSE;
}

bool8 MetatileBehavior_IsLavaridge1FWarp(u8 metatileBehavior)
{
    if (metatileBehavior == MB_LAVARIDGE_GYM_1F_WARP)
        return TRUE;
    return FALSE;
}

bool8 MetatileBehavior_IsAquaHideoutWarp(u8 metatileBehavior)
{
    if (metatileBehavior == MB_AQUA_HIDEOUT_WARP)
        return TRUE;
    return FALSE;
}

bool8 MetatileBehavior_IsWarpOrBridge(u8 metatileBehavior)
{
    if (metatileBehavior == MB_WARP_OR_BRIDGE)
        return TRUE;
    return FALSE;
}

bool8 MetatileBehavior_IsMossdeepGymWarp(u8 metatileBehavior)
{
    if (metatileBehavior == MB_MOSSDEEP_GYM_WARP)
        return TRUE;
    return FALSE;
}

bool8 MetatileBehavior_IsSurfableFishableWater(u8 metatileBehavior)
{
    if (metatileBehavior == MB_POND_WATER
     || metatileBehavior == MB_OCEAN_WATER
     || metatileBehavior == MB_SEMI_DEEP_WATER 
     || metatileBehavior == MB_DEEP_WATER
     || metatileBehavior == MB_SOOTOPOLIS_DEEP_WATER
     || (metatileBehavior == MB_EASTWARD_CURRENT
     || metatileBehavior == MB_WESTWARD_CURRENT
     || metatileBehavior == MB_NORTHWARD_CURRENT
     || metatileBehavior == MB_SOUTHWARD_CURRENT))
        return TRUE;
    return FALSE;
}

bool8 MetatileBehavior_IsMtPyreHole(u8 metatileBehavior)
{
    if (metatileBehavior == MB_MT_PYRE_HOLE)
        return TRUE;
    return FALSE;
}

bool8 MetatileBehavior_IsCrackedFloorHole(u8 metatileBehavior)
{
    if (metatileBehavior == MB_CRACKED_FLOOR_HOLE)
        return TRUE;
    return FALSE;
}

bool8 MetatileBehavior_IsCrackedFloor(u8 metatileBehavior)
{
    if (metatileBehavior == MB_CRACKED_FLOOR)
        return TRUE;
    return FALSE;
}

bool8 MetatileBehavior_IsMuddySlope(u8 metatileBehavior)
{
    if (metatileBehavior == MB_MUDDY_SLOPE)
        return TRUE;
    return FALSE;
}

bool8 MetatileBehavior_IsBumpySlope(u8 metatileBehavior)
{
    if (metatileBehavior == MB_BUMPY_SLOPE)
        return TRUE;
    return FALSE;
}

bool8 MetatileBehavior_IsIsolatedVerticalRail(u8 metatileBehavior)
{
    if (metatileBehavior == MB_ISOLATED_VERTICAL_RAIL)
        return TRUE;
    return FALSE;
}

bool8 MetatileBehavior_IsIsolatedHorizontalRail(u8 metatileBehavior)
{
    if (metatileBehavior == MB_ISOLATED_HORIZONTAL_RAIL)
        return TRUE;
    return FALSE;
}

bool8 MetatileBehavior_IsVerticalRail(u8 metatileBehavior)
{
    if (metatileBehavior == MB_VERTICAL_RAIL)
        return TRUE;
    return FALSE;
}

bool8 MetatileBehavior_IsHorizontalRail(u8 metatileBehavior)
{
    if (metatileBehavior == MB_HORIZONTAL_RAIL)
        return TRUE;
    return FALSE;
}

bool8 MetatileBehavior_IsSeaweed(u8 metatileBehavior)
{
    if (metatileBehavior == MB_SEAWEED
     || metatileBehavior == MB_SEAWEED_NO_SURFACING)
        return TRUE;
    return FALSE;
}

bool8 MetatileBehavior_IsRunningDisallowed(u8 metatileBehavior)
{
    if (metatileBehavior == MB_NO_RUNNING
     || metatileBehavior == MB_LONG_GRASS
     || metatileBehavior == MB_HOT_SPRINGS
     || MetatileBehavior_IsPacifidlogLog(metatileBehavior) != FALSE)
        return TRUE;
    return FALSE;
}

bool8 MetatileBehavior_IsCuttableGrass(u8 metatileBehavior)
{
    if (metatileBehavior == MB_TALL_GRASS
     || metatileBehavior == MB_LONG_GRASS
     || metatileBehavior == MB_ASHGRASS
     || metatileBehavior == MB_LONG_GRASS_SOUTH_EDGE)
        return TRUE;
    return FALSE;
}

bool8 MetatileBehavior_IsRunningShoesManual(u8 metatileBehavior)
{
    if (metatileBehavior == MB_RUNNING_SHOES_INSTRUCTION)
        return TRUE;
    return FALSE;
}

bool8 MetatileBehavior_IsPictureBookShelf(u8 metatileBehavior)
{
    if (metatileBehavior == MB_PICTURE_BOOK_SHELF)
        return TRUE;
    return FALSE;
}

bool8 MetatileBehavior_IsBookShelf(u8 metatileBehavior)
{
    if (metatileBehavior == MB_BOOKSHELF)
        return TRUE;
    return FALSE;
}

bool8 MetatileBehavior_IsPokeCenterBookShelf(u8 metatileBehavior)
{
    if (metatileBehavior == MB_POKEMON_CENTER_BOOKSHELF)
        return TRUE;
    return FALSE;
}

bool8 MetatileBehavior_IsVase(u8 metatileBehavior)
{
    if (metatileBehavior == MB_VASE)
        return TRUE;
    return FALSE;
}

bool8 MetatileBehavior_IsTrashCan(u8 metatileBehavior)
{
    if (metatileBehavior == MB_TRASH_CAN)
        return TRUE;
    return FALSE;
}

bool8 MetatileBehavior_IsShopShelf(u8 metatileBehavior)
{
    if (metatileBehavior == MB_SHOP_SHELF)
        return TRUE;
    return FALSE;
}

bool8 MetatileBehavior_IsBlueprint(u8 metatileBehavior)
{
    if (metatileBehavior == MB_BLUEPRINT)
        return TRUE;
    return FALSE;
}

bool8 MetatileBehavior_IsBattlePyramidWarp(u8 metatileBehavior)
{
    if (metatileBehavior == MB_BATTLE_PYRAMID_WARP)
        return TRUE;
    return FALSE;
}

bool8 MetatileBehavior_IsPlayerFacingWirelessBoxResults(u8 tile, u8 playerDir)
{
    if (playerDir != CONNECTION_NORTH) // if the player isn't facing north, forget about it.
        return FALSE;
    else if (tile == MB_WIRELESS_BOX_RESULTS) // is the player's north tile the monitor with results?
        return TRUE;
    return FALSE;
}

bool8 MetatileBehavior_IsCableBoxResults2(u8 tile, u8 playerDir)
{
    if (playerDir != CONNECTION_NORTH) // if the player isn't facing north, forget about it.
        return FALSE;
    else if (tile == MB_CABLE_BOX_RESULTS_2) // is the player's north tile the monitor with results?
        return TRUE;
    return FALSE;
}

bool8 MetatileBehavior_IsQuestionnaire(u8 metatileBehavior)
{
    if (metatileBehavior == MB_QUESTIONNAIRE)
        return TRUE;
    return FALSE;
}

bool8 MetatileBehavior_IsLongGrass_Duplicate(u8 metatileBehavior)
{
    if (metatileBehavior == MB_LONG_GRASS)
        return TRUE;
    return FALSE;
}

bool8 MetatileBehavior_IsLongGrassSouthEdge(u8 metatileBehavior)
{
    if (metatileBehavior == MB_LONG_GRASS_SOUTH_EDGE)
        return TRUE;
    return FALSE;
}

bool8 MetatileBehavior_IsTrainerHillTimer(u8 metatileBehavior)
{
    if (metatileBehavior == MB_TRAINER_HILL_TIMER)
        return TRUE;
    return FALSE;
}
