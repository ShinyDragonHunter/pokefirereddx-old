#include "global.h"
#include "bike.h"
#include "event_object_movement.h"
#include "field_player_avatar.h"
#include "fieldmap.h"
#include "field_specials.h"
#include "metatile_behavior.h"
#include "overworld.h"
#include "sound.h"
#include "constants/region_map_sections.h"
#include "constants/map_types.h"
#include "constants/songs.h"

// this file's functions
static u8 CheckMovementInputBike(u8 *, u16, u16);
static u8 BikeHandleInputNormal(u8 *, u16, u16);
static u8 BikeHandleInputTurning(u8 *, u16, u16);
static void BikeTransition_FaceDirection(u8);
static void BikeTransition_TurnDirection(u8);
static void BikeTransition_Moving(u8);
static u8 Bike_DPadToDirection(u16);
static u8 GetBikeCollision(u8);
static u8 GetBikeCollisionAt(struct ObjectEvent *, s16, s16, u8, u8);
static bool8 IsRunningDisallowedByMetatile(u8);
static u8 CanBikeFaceDirOnMetatile(u8, u8);
static bool8 WillPlayerCollideWithCollision(u8, u8);

// const rom data

/*
    A bike transition is a type of callback for the bike that actually
    modifies the bicycle's direction or momentum or otherwise movement.
    Alternatively, a bike may also have input handlers which process the
    bike transition to call.
*/

static void (*const sBikeTransitions[])(u8) =
{
    [BIKE_TRANS_FACE_DIRECTION]    = BikeTransition_FaceDirection,
    [BIKE_TRANS_TURNING]           = BikeTransition_TurnDirection,
    [BIKE_TRANS_MOVING]            = BikeTransition_Moving,
    [BIKE_TRANS_DOWNHILL]          = BikeTransition_FaceDirection,
    [BIKE_TRANS_UPHILL]            = BikeTransition_FaceDirection,
};

static u8 (*const sBikeInputHandlers[])(u8 *, u16, u16) =
{
    [BIKE_STATE_NORMAL]           = BikeHandleInputNormal,
    [BIKE_STATE_TURNING]          = BikeHandleInputTurning,
    [BIKE_STATE_SLOPE]            = BikeHandleInputNormal,
};

static const u16 sBikeMusicTable[NUM_REGION] =
{
    [REGION_HOENN] = MUS_CYCLING,
    [REGION_KANTO] = MUS_RG_CYCLING,
    [REGION_SEVII] = MUS_RG_CYCLING,
};

// code
void MovePlayerOnBike(u8 direction, u16 newKeys, u16 heldKeys)
{
    sBikeTransitions[CheckMovementInputBike(&direction, newKeys, heldKeys)](direction);
}

static u8 CheckMovementInputBike(u8 *newDirection, u16 newKeys, u16 heldKeys)
{
    return sBikeInputHandlers[gPlayerAvatar.bikeState](newDirection, newKeys, heldKeys);
}

static u8 BikeHandleInputNormal(u8 *newDirection, u16 newKeys, u16 heldKeys)
{
    u8 direction = GetPlayerMovementDirection();

    if (*newDirection == DIR_NONE)
    {
        *newDirection = direction;
        gPlayerAvatar.runningState = NOT_MOVING;
        return BIKE_TRANS_FACE_DIRECTION;
    }
    if (*newDirection != direction && gPlayerAvatar.runningState != MOVING)
    {
        gPlayerAvatar.bikeState = BIKE_STATE_TURNING;
        gPlayerAvatar.runningState = NOT_MOVING;
        return CheckMovementInputBike(newDirection, newKeys, heldKeys);
    }
    gPlayerAvatar.runningState = MOVING;
    return BIKE_TRANS_MOVING;
}

static u8 BikeHandleInputTurning(u8 *newDirection, u16 newKeys, u16 heldKeys)
{
    gPlayerAvatar.runningState = TURN_DIRECTION;
    gPlayerAvatar.bikeState = BIKE_STATE_NORMAL;
    return BIKE_TRANS_TURNING;
}

static void BikeTransition_FaceDirection(u8 direction)
{
    PlayerFaceDirection(direction);
}

static void BikeTransition_TurnDirection(u8 direction)
{
    struct ObjectEvent *playerObjEvent = &gObjectEvents[gPlayerAvatar.objectEventId];

    if (!CanBikeFaceDirOnMetatile(direction, playerObjEvent->currentMetatileBehavior))
        direction = playerObjEvent->movementDirection;
    PlayerFaceDirection(direction);
}

static void BikeTransition_Moving(u8 direction)
{
    u8 collision;
    struct ObjectEvent *playerObjEvent = &gObjectEvents[gPlayerAvatar.objectEventId];

    if (!CanBikeFaceDirOnMetatile(direction, playerObjEvent->currentMetatileBehavior))
    {
        BikeTransition_FaceDirection(playerObjEvent->movementDirection);
        return;
    }
    collision = GetBikeCollision(direction);
    if (collision > 0 && collision < COLLISION_VERTICAL_RAIL)
    {
        if (collision == COLLISION_LEDGE_JUMP)
            PlayerJumpLedge(direction);
        else if (collision == COLLISION_OBJECT_EVENT && IsPlayerCollidingWithFarawayIslandMew(direction))
            PlayerOnBikeCollideWithFarawayIslandMew(direction);
        else if (collision < COLLISION_STOP_SURFING || collision > COLLISION_ROTATING_GATE)
            PlayerOnBikeCollide(direction);
    }
    else
        PlayerRideWaterCurrent(direction);
}

static u8 GetBikeCollision(u8 direction)
{
    u8 metatitleBehavior;
    struct ObjectEvent *playerObjEvent = &gObjectEvents[gPlayerAvatar.objectEventId];
    s16 x = playerObjEvent->currentCoords.x;
    s16 y = playerObjEvent->currentCoords.y;
    MoveCoords(direction, &x, &y);
    metatitleBehavior = MapGridGetMetatileBehaviorAt(x, y);
    return GetBikeCollisionAt(playerObjEvent, x, y, direction, metatitleBehavior);
}

static u8 GetBikeCollisionAt(struct ObjectEvent *objectEvent, s16 x, s16 y, u8 direction, u8 metatitleBehavior)
{
    u8 collision = CheckForObjectEventCollision(objectEvent, x, y, direction, metatitleBehavior);

    if (collision > COLLISION_OBJECT_EVENT)
        return collision;

    if (collision == COLLISION_NONE && IsRunningDisallowedByMetatile(metatitleBehavior))
        collision = COLLISION_IMPASSABLE;
    return collision;
}

bool32 IsRunningDisallowed(u8 metatile)
{
    if (IsRunningDisallowedByMetatile(metatile))
        return TRUE;
    return FALSE;
}

static bool8 IsRunningDisallowedByMetatile(u8 tile)
{
    if (MetatileBehavior_IsRunningDisallowed(tile)
     || (MetatileBehavior_IsFortreeBridge(tile) && (PlayerGetZCoord() & 1) == 0))
        return TRUE;
    return FALSE;
}

static bool8 CanBikeFaceDirOnMetatile(u8 direction, u8 tile)
{
    if (direction == DIR_EAST || direction == DIR_WEST)
    {
        // Bike cannot face east or west on a vertical rail
        if (MetatileBehavior_IsIsolatedVerticalRail(tile)
         || MetatileBehavior_IsVerticalRail(tile))
            return FALSE;
    }
    else
    {
        // Bike cannot face north or south on a horizontal rail
        if (MetatileBehavior_IsIsolatedHorizontalRail(tile)
         || MetatileBehavior_IsHorizontalRail(tile))
            return FALSE;
    }
    return TRUE;
}

static bool8 WillPlayerCollideWithCollision(u8 newTileCollision, u8 direction)
{
    if (direction == DIR_NORTH || direction == DIR_SOUTH)
    {
        if (newTileCollision == COLLISION_ISOLATED_VERTICAL_RAIL || newTileCollision == COLLISION_VERTICAL_RAIL)
            return FALSE;
    }
    else if (newTileCollision == COLLISION_ISOLATED_HORIZONTAL_RAIL || newTileCollision == COLLISION_HORIZONTAL_RAIL)
        return FALSE;
    return TRUE;
}

bool8 IsBikingDisallowedByPlayer(void)
{
    s16 x, y;
    u8 tileBehavior;

    if (!(gPlayerAvatar.flags & (PLAYER_AVATAR_FLAG_SURFING | PLAYER_AVATAR_FLAG_UNDERWATER)))
    {
        PlayerGetDestCoords(&x, &y);
        tileBehavior = MapGridGetMetatileBehaviorAt(x, y);
        if (!IsRunningDisallowedByMetatile(tileBehavior))
            return FALSE;
    }
    return TRUE;
}

void GetOnOffBike(u8 transitionFlags)
{
    gUnusedBikeCameraAheadPanback = FALSE;
    if (gPlayerAvatar.flags & (PLAYER_AVATAR_FLAG_BIKE))
    {
        SetPlayerAvatarTransitionFlags(PLAYER_AVATAR_FLAG_ON_FOOT);
        Overworld_ClearSavedMusic();
        Overworld_PlaySpecialMapMusic();
    }
    else
    {
        u16 music = sBikeMusicTable[gMapsecToRegion[gMapHeader.regionMapSectionId]];
        PlaySE(SE_BIKE_BELL);
        SetPlayerAvatarTransitionFlags(transitionFlags);
        Overworld_SetSavedMusic(music);
        Overworld_ChangeMusicTo(music);
    }
}

void BikeClearState(void)
{
    gPlayerAvatar.bikeState = BIKE_STATE_NORMAL;
}

s16 GetPlayerSpeed(void)
{
    if (gPlayerAvatar.flags & PLAYER_AVATAR_FLAG_BIKE)
        return SPEED_FASTER;
    else if (gPlayerAvatar.flags & (PLAYER_AVATAR_FLAG_SURFING | PLAYER_AVATAR_FLAG_DASH))
        return SPEED_FAST;
    return SPEED_NORMAL;
}
