#include "global.h"
#include "metatile_behavior.h"
#include "constants/metatile_behaviors.h"

static const bool8 sTileSurfable[] = {
    [MB_POND_WATER] = TRUE,
    [MB_SEMI_DEEP_WATER] = TRUE,
    [MB_DEEP_WATER] = TRUE,
    [MB_WATERFALL] = TRUE,
    [MB_SOOTOPOLIS_DEEP_WATER] = TRUE,
    [MB_OCEAN_WATER] = TRUE,
    [MB_REFLECTION_UNDER_BRIDGE] = TRUE,
    [MB_EASTWARD_CURRENT] = TRUE,
    [MB_WESTWARD_CURRENT] = TRUE,
    [MB_NORTHWARD_CURRENT] = TRUE,
    [MB_SOUTHWARD_CURRENT] = TRUE
};

static const u8 sTileBitAttributes[32] = {
    [0] = 0x00,
    [1] = 0x01,
    [2] = 0x02,
    [3] = 0x04,
    [4] = 0x08,
};

bool8 MetatileBehavior_IsATile(u8 metatileBehavior)
{
    return TRUE;
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

bool8 MetatileBehavior_IsDirectionalUpRightStairWarp(u8 metatileBehavior)
{
    if(metatileBehavior == MB_UP_RIGHT_STAIR_WARP)
        return TRUE;
    return FALSE;
}

bool8 MetatileBehavior_IsDirectionalUpLeftStairWarp(u8 metatileBehavior)
{
    if (metatileBehavior == MB_UP_LEFT_STAIR_WARP)
        return TRUE;
    return FALSE;
}

bool8 MetatileBehavior_IsDirectionalDownRightStairWarp(u8 metatileBehavior)
{
    if (metatileBehavior == MB_DOWN_RIGHT_STAIR_WARP)
        return TRUE;
    return FALSE;
}

bool8 MetatileBehavior_IsDirectionalDownLeftStairWarp(u8 metatileBehavior)
{
    if (metatileBehavior == MB_DOWN_LEFT_STAIR_WARP)
        return TRUE;
    return FALSE;
}

bool8 MetatileBehavior_IsDirectionalStairWarp(u8 metatileBehavior)
{
    if (metatileBehavior >= MB_UP_RIGHT_STAIR_WARP
     && metatileBehavior <= MB_DOWN_LEFT_STAIR_WARP)
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
    if (sTileSurfable[metatileBehavior] & 1)
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

bool8 TestMetatileAttributeBit(u8 arg1, u8 arg2)
{
    if (sTileBitAttributes[arg1] & arg2)
        return TRUE;
    return FALSE;
}
