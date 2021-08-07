#include "global.h"
#include "main.h"
#include "bike.h"
#include "event_data.h"
#include "event_object_movement.h"
#include "field_camera.h"
#include "field_effect.h"
#include "field_effect_helpers.h"
#include "field_player_avatar.h"
#include "fieldmap.h"
#include "menu.h"
#include "metatile_behavior.h"
#include "overworld.h"
#include "party_menu.h"
#include "random.h"
#include "rotating_gate.h"
#include "script.h"
#include "sound.h"
#include "sprite.h"
#include "strings.h"
#include "task.h"
#include "tv.h"
#include "wild_encounter.h"
#include "constants/abilities.h"
#include "constants/event_objects.h"
#include "constants/event_object_movement.h"
#include "constants/field_effects.h"
#include "constants/items.h"
#include "constants/maps.h"
#include "constants/metatile_behaviors.h"
#include "constants/moves.h"
#include "constants/songs.h"
#include "constants/trainer_types.h"

static EWRAM_DATA u8 sSpinStartFacingDir = 0;
EWRAM_DATA struct ObjectEvent gObjectEvents[OBJECT_EVENTS_COUNT] = {};
EWRAM_DATA struct PlayerAvatar gPlayerAvatar = {};

// static declarations

static u8 ObjectEventCB2_NoMovement2();
static bool8 TryInterruptObjectEventSpecialAnim(struct ObjectEvent *, u8);
static void npc_clear_strange_bits(struct ObjectEvent *);
static void MovePlayerAvatarUsingKeypadInput(u8, u16, u16);
static void PlayerAllowForcedMovementIfMovingSameDirection();
static bool8 TryDoMetatileBehaviorForcedMovement();
static u8 GetForcedMovementByMetatileBehavior();

static bool8 ForcedMovement_None(void);
static bool8 ForcedMovement_Slip(void);
static bool8 ForcedMovement_WalkSouth(void);
static bool8 ForcedMovement_WalkNorth(void);
static bool8 ForcedMovement_WalkWest(void);
static bool8 ForcedMovement_WalkEast(void);
static bool8 ForcedMovement_PushedSouthByCurrent(void);
static bool8 ForcedMovement_PushedNorthByCurrent(void);
static bool8 ForcedMovement_PushedWestByCurrent(void);
static bool8 ForcedMovement_PushedEastByCurrent(void);
static bool8 ForcedMovement_SlideSouth(void);
static bool8 ForcedMovement_SlideNorth(void);
static bool8 ForcedMovement_SlideWest(void);
static bool8 ForcedMovement_SlideEast(void);
static bool8 ForcedMovement_MatJump(void);
static bool8 ForcedMovement_MuddySlope(void);

static void MovePlayerNotOnBike(u8, u16);
static u8 CheckMovementInputNotOnBike(u8);
static void PlayerNotOnBikeNotMoving(u8, u16);
static void PlayerNotOnBikeTurningInPlace(u8, u16);
static void PlayerNotOnBikeMoving(u8, u16);
static u8 CheckForPlayerAvatarCollision(u8);
static u8 sub_808B028(u8);
static bool8 CanStopSurfing(s16, s16, u8);
static bool8 ShouldJumpLedge(s16, s16, u8);
static bool8 TryPushBoulder(s16, s16, u8);

static void DoPlayerAvatarTransition(void);
static void PlayerAvatarTransition_Dummy(struct ObjectEvent *a);
static void PlayerAvatarTransition_Normal(struct ObjectEvent *a);
static void PlayerAvatarTransition_Bike(struct ObjectEvent *a);
static void PlayerAvatarTransition_Surfing(struct ObjectEvent *a);
static void PlayerAvatarTransition_Underwater(struct ObjectEvent *a);
static void PlayerAvatarTransition_ReturnToField(struct ObjectEvent *a);

static bool8 player_is_anim_in_certain_ranges(void);
static bool8 sub_808B618(void);
static bool8 PlayerIsAnimActive(void);
static bool8 PlayerCheckIfAnimFinishedOrInactive(void);

static void PlayerRun(u8);
static void PlayerNotOnBikeCollide(u8);
static void PlayerNotOnBikeCollideWithFarawayIslandMew(u8);

static void PlayCollisionSoundIfNotFacingWarp(u8 a);

static void HideShowWarpArrow(struct ObjectEvent *);

static void StartStrengthAnim(u8, u8);
static void Task_PushBoulder(u8 taskId);
static bool8 PushBoulder_Start(struct Task *task, struct ObjectEvent *playerObject, struct ObjectEvent *strengthObject);
static bool8 PushBoulder_Move(struct Task *task, struct ObjectEvent *playerObject, struct ObjectEvent *strengthObject);
static bool8 PushBoulder_End(struct Task *task, struct ObjectEvent *playerObject, struct ObjectEvent *strengthObject);

static void CreateStopSurfingTask(u8);
static void Task_StopSurfingInit(u8 taskId);
static void Task_WaitStopSurfing(u8 taskId);

static void Task_Fishing(u8 taskId);
static u8 Fishing_Init(struct Task *task);
static u8 Fishing_GetRodOut(struct Task *task);
static u8 Fishing_WaitBeforeDots(struct Task *task);
static u8 Fishing_InitDots(struct Task *task);
static u8 Fishing_ShowDots(struct Task *task);
static u8 Fishing_CheckForBite(struct Task *task);
static u8 Fishing_MonOnHook(struct Task *task);
static u8 Fishing_StartEncounter(struct Task *task);
static u8 Fishing_NotEvenNibble(struct Task *task);
static u8 Fishing_NoMon(struct Task *task);
static u8 Fishing_PutRodAway(struct Task *task);
static u8 Fishing_EndNoMon(struct Task *task);
static void AlignFishingAnimationFrames(void);

static u8 TrySpinPlayerForWarp(struct ObjectEvent *object, s16 *a1);

// .rodata

static bool8 (*const sForcedMovementTestFuncs[])(u8) =
{
    MetatileBehavior_IsTrickHouseSlipperyFloor,
    MetatileBehavior_IsIce_2,
    MetatileBehavior_IsWalkSouth,
    MetatileBehavior_IsWalkNorth,
    MetatileBehavior_IsWalkWest,
    MetatileBehavior_IsWalkEast,
    MetatileBehavior_IsSouthwardCurrent,
    MetatileBehavior_IsNorthwardCurrent,
    MetatileBehavior_IsWestwardCurrent,
    MetatileBehavior_IsEastwardCurrent,
    MetatileBehavior_IsSlideSouth,
    MetatileBehavior_IsSlideNorth,
    MetatileBehavior_IsSlideWest,
    MetatileBehavior_IsSlideEast,
    MetatileBehavior_IsWaterfall,
    MetatileBehavior_IsTrickHouseSlipperyFloor,
    MetatileBehavior_IsTrickHouseSlipperyFloor,
    MetatileBehavior_IsMuddySlope,
};

static bool8 (*const sForcedMovementFuncs[])(void) =
{
    ForcedMovement_None,
    ForcedMovement_Slip,
    ForcedMovement_Slip,
    ForcedMovement_WalkSouth,
    ForcedMovement_WalkNorth,
    ForcedMovement_WalkWest,
    ForcedMovement_WalkEast,
    ForcedMovement_PushedSouthByCurrent,
    ForcedMovement_PushedNorthByCurrent,
    ForcedMovement_PushedWestByCurrent,
    ForcedMovement_PushedEastByCurrent,
    ForcedMovement_SlideSouth,
    ForcedMovement_SlideNorth,
    ForcedMovement_SlideWest,
    ForcedMovement_SlideEast,
    ForcedMovement_PushedSouthByCurrent,
    ForcedMovement_None,
    ForcedMovement_None,
    ForcedMovement_MuddySlope,
};

static void (*const gUnknown_08497490[])(u8, u16) =
{
    PlayerNotOnBikeNotMoving,
    PlayerNotOnBikeTurningInPlace,
    PlayerNotOnBikeMoving,
};

static bool8 (*const sAcroBikeTrickMetatiles[])(u8) =
{
    MetatileBehavior_IsBumpySlope,
    MetatileBehavior_IsIsolatedVerticalRail,
    MetatileBehavior_IsIsolatedHorizontalRail,
    MetatileBehavior_IsVerticalRail,
    MetatileBehavior_IsHorizontalRail,
};

static void (*const sPlayerAvatarTransitionFuncs[])(struct ObjectEvent *) =
{
    [PLAYER_AVATAR_STATE_NORMAL]          = PlayerAvatarTransition_Normal,
    [PLAYER_AVATAR_STATE_BIKE]            = PlayerAvatarTransition_Bike,
    [PLAYER_AVATAR_STATE_SURFING]         = PlayerAvatarTransition_Surfing,
    [PLAYER_AVATAR_STATE_UNDERWATER]      = PlayerAvatarTransition_Underwater,
    [PLAYER_AVATAR_STATE_FIELD_MOVE]      = PlayerAvatarTransition_ReturnToField,
    [PLAYER_AVATAR_STATE_FIELD_MOVE_BIKE] = PlayerAvatarTransition_ReturnToField,
    [PLAYER_AVATAR_STATE_FISHING]         = PlayerAvatarTransition_Dummy,
    [PLAYER_AVATAR_STATE_WATERING]        = PlayerAvatarTransition_Dummy,
};

static bool8 (*const sArrowWarpMetatileBehaviorChecks[])(u8) =
{
    MetatileBehavior_IsSouthArrowWarp,
    MetatileBehavior_IsNorthArrowWarp,
    MetatileBehavior_IsWestArrowWarp,
    MetatileBehavior_IsEastArrowWarp,
};

static const u16 sPlayerAvatarGfxIds[][2] =
{
    {OBJ_EVENT_GFX_RED_NORMAL,          OBJ_EVENT_GFX_LEAF_NORMAL},
    {OBJ_EVENT_GFX_RED_BIKE,            OBJ_EVENT_GFX_LEAF_BIKE},
    {OBJ_EVENT_GFX_RED_SURFING,         OBJ_EVENT_GFX_LEAF_SURFING},
    {OBJ_EVENT_GFX_RED_UNDERWATER,      OBJ_EVENT_GFX_LEAF_UNDERWATER},
    {OBJ_EVENT_GFX_RED_FIELD_MOVE,      OBJ_EVENT_GFX_LEAF_FIELD_MOVE},
    {OBJ_EVENT_GFX_RED_FIELD_MOVE_BIKE, OBJ_EVENT_GFX_LEAF_FIELD_MOVE_BIKE},
    {OBJ_EVENT_GFX_RED_FISHING,         OBJ_EVENT_GFX_LEAF_FISHING},
    {OBJ_EVENT_GFX_RED_WATERING,        OBJ_EVENT_GFX_LEAF_WATERING},
};

static const u16 sCDAvatarGfxIds[] = {OBJ_EVENT_GFX_GOLD, OBJ_EVENT_GFX_KRIS};

static const u16 sHeliodorAvatarGfxIds[] = {OBJ_EVENT_GFX_H_BRENDAN, OBJ_EVENT_GFX_H_MAY};

static const u16 sEAvatarGfxIds[] = {OBJ_EVENT_GFX_E_BRENDAN, OBJ_EVENT_GFX_E_MAY};

static const u16 sRSAvatarGfxIds[] = {OBJ_EVENT_GFX_RS_BRENDAN, OBJ_EVENT_GFX_RS_MAY};

static const u16 sPlayerAvatarGfxToStateFlag[2][4][2] =
{
    [MALE] =
    {
        {OBJ_EVENT_GFX_RED_NORMAL,         PLAYER_AVATAR_FLAG_ON_FOOT},
        {OBJ_EVENT_GFX_RED_BIKE,           PLAYER_AVATAR_FLAG_BIKE},
        {OBJ_EVENT_GFX_RED_SURFING,        PLAYER_AVATAR_FLAG_SURFING},
        {OBJ_EVENT_GFX_RED_UNDERWATER,     PLAYER_AVATAR_FLAG_UNDERWATER},
    },
    [FEMALE] =
    {
        {OBJ_EVENT_GFX_LEAF_NORMAL,        PLAYER_AVATAR_FLAG_ON_FOOT},
        {OBJ_EVENT_GFX_LEAF_BIKE,          PLAYER_AVATAR_FLAG_BIKE},
        {OBJ_EVENT_GFX_LEAF_SURFING,       PLAYER_AVATAR_FLAG_SURFING},
        {OBJ_EVENT_GFX_LEAF_UNDERWATER,    PLAYER_AVATAR_FLAG_UNDERWATER},
    }
};

static bool8 (*const sPushBoulderFuncs[])(struct Task *, struct ObjectEvent *, struct ObjectEvent *) =
{
    PushBoulder_Start,
    PushBoulder_Move,
    PushBoulder_End,
};

// .text

void MovementType_Player(struct Sprite *sprite)
{
    UpdateObjectEventCurrentMovement(&gObjectEvents[sprite->data[0]], sprite, ObjectEventCB2_NoMovement2);
}

static u8 ObjectEventCB2_NoMovement2(void)
{
    return 0;
}

void PlayerStep(u8 direction, u16 newKeys, u16 heldKeys)
{
    struct ObjectEvent *playerObjEvent = &gObjectEvents[gPlayerAvatar.objectEventId];

    HideShowWarpArrow(playerObjEvent);
    if (!gPlayerAvatar.preventStep)
    {
        if (!TryInterruptObjectEventSpecialAnim(playerObjEvent, direction))
        {
            npc_clear_strange_bits(playerObjEvent);
            DoPlayerAvatarTransition();
            if (!TryDoMetatileBehaviorForcedMovement())
            {
                MovePlayerAvatarUsingKeypadInput(direction, newKeys, heldKeys);
                PlayerAllowForcedMovementIfMovingSameDirection();
            }
        }
    }
}

static bool8 TryInterruptObjectEventSpecialAnim(struct ObjectEvent *playerObjEvent, u8 direction)
{
    if (ObjectEventIsMovementOverridden(playerObjEvent)
     && !ObjectEventClearHeldMovementIfFinished(playerObjEvent))
    {
        u8 heldMovementActionId = ObjectEventGetHeldMovementActionId(playerObjEvent);
        if (heldMovementActionId > MOVEMENT_ACTION_WALK_FAST_RIGHT && heldMovementActionId < MOVEMENT_ACTION_WALK_IN_PLACE_NORMAL_DOWN)
        {
            if (playerObjEvent->movementDirection != direction
             || !sub_808B028(direction))
            {
                ObjectEventClearHeldMovement(playerObjEvent);
                return FALSE;
            }
        }

        return TRUE;
    }

    return FALSE;
}

static void npc_clear_strange_bits(struct ObjectEvent *objEvent)
{
    objEvent->inanimate = FALSE;
    objEvent->disableAnim = FALSE;
    objEvent->facingDirectionLocked = FALSE;
    gPlayerAvatar.flags &= ~PLAYER_AVATAR_FLAG_DASH;
}

static void MovePlayerAvatarUsingKeypadInput(u8 direction, u16 newKeys, u16 heldKeys)
{
    if (gPlayerAvatar.flags & (PLAYER_AVATAR_FLAG_BIKE))
        MovePlayerOnBike(direction, newKeys, heldKeys);
    else
        MovePlayerNotOnBike(direction, heldKeys);
}

static void PlayerAllowForcedMovementIfMovingSameDirection(void)
{
    if (gPlayerAvatar.runningState == MOVING)
        gPlayerAvatar.flags &= ~PLAYER_AVATAR_FLAG_4;
}

static bool8 TryDoMetatileBehaviorForcedMovement(void)
{
    return sForcedMovementFuncs[GetForcedMovementByMetatileBehavior()]();
}

static u8 GetForcedMovementByMetatileBehavior(void)
{
    u32 i;

    if (!(gPlayerAvatar.flags & PLAYER_AVATAR_FLAG_4))
    {
        u8 metatileBehavior = gObjectEvents[gPlayerAvatar.objectEventId].currentMetatileBehavior;

        for (i = 0; i < 18; i++)
        {
            if (sForcedMovementTestFuncs[i](metatileBehavior))
                return i + 1;
        }
    }
    return 0;
}

static bool8 ForcedMovement_None(void)
{
    if (gPlayerAvatar.flags & PLAYER_AVATAR_FLAG_FORCED_MOVE)
    {
        struct ObjectEvent *playerObjEvent = &gObjectEvents[gPlayerAvatar.objectEventId];

        playerObjEvent->facingDirectionLocked = FALSE;
        playerObjEvent->enableAnim = TRUE;
        SetObjectEventDirection(playerObjEvent, playerObjEvent->facingDirection);
        gPlayerAvatar.flags &= ~PLAYER_AVATAR_FLAG_FORCED_MOVE;
    }
    return FALSE;
}

static u8 DoForcedMovement(u8 direction, void (*b)(u8))
{
    struct PlayerAvatar *playerAvatar = &gPlayerAvatar;
    u8 collision = CheckForPlayerAvatarCollision(direction);

    playerAvatar->flags |= PLAYER_AVATAR_FLAG_FORCED_MOVE;
    if (collision)
    {
        ForcedMovement_None();
        if (collision < COLLISION_STOP_SURFING)
        {
            return 0;
        }
        else
        {
            if (collision == COLLISION_LEDGE_JUMP)
                PlayerJumpLedge(direction);
            playerAvatar->flags |= PLAYER_AVATAR_FLAG_FORCED_MOVE;
            playerAvatar->runningState = MOVING;
            return 1;
        }
    }
    else
    {
        playerAvatar->runningState = MOVING;
        b(direction);
        return 1;
    }
}

static u8 DoForcedMovementInCurrentDirection(void (*a)(u8))
{
    struct ObjectEvent *playerObjEvent = &gObjectEvents[gPlayerAvatar.objectEventId];

    playerObjEvent->disableAnim = TRUE;
    return DoForcedMovement(playerObjEvent->movementDirection, a);
}

static bool8 ForcedMovement_Slip(void)
{
    return DoForcedMovementInCurrentDirection(PlayerGoSpeed2);
}

static bool8 ForcedMovement_WalkSouth(void)
{
    return DoForcedMovement(DIR_SOUTH, PlayerGoSpeed1);
}

static bool8 ForcedMovement_WalkNorth(void)
{
    return DoForcedMovement(DIR_NORTH, PlayerGoSpeed1);
}

static bool8 ForcedMovement_WalkWest(void)
{
    return DoForcedMovement(DIR_WEST, PlayerGoSpeed1);
}

static bool8 ForcedMovement_WalkEast(void)
{
    return DoForcedMovement(DIR_EAST, PlayerGoSpeed1);
}

static bool8 ForcedMovement_PushedSouthByCurrent(void)
{
    return DoForcedMovement(DIR_SOUTH, PlayerRideWaterCurrent);
}

static bool8 ForcedMovement_PushedNorthByCurrent(void)
{
    return DoForcedMovement(DIR_NORTH, PlayerRideWaterCurrent);
}

static bool8 ForcedMovement_PushedWestByCurrent(void)
{
    return DoForcedMovement(DIR_WEST, PlayerRideWaterCurrent);
}

static bool8 ForcedMovement_PushedEastByCurrent(void)
{
    return DoForcedMovement(DIR_EAST, PlayerRideWaterCurrent);
}

static u8 ForcedMovement_Slide(u8 direction, void (*b)(u8))
{
    struct ObjectEvent *playerObjEvent = &gObjectEvents[gPlayerAvatar.objectEventId];

    playerObjEvent->disableAnim = TRUE;
    playerObjEvent->facingDirectionLocked = TRUE;
    return DoForcedMovement(direction, b);
}

static bool8 ForcedMovement_SlideSouth(void)
{
    return ForcedMovement_Slide(DIR_SOUTH, PlayerGoSpeed2);
}

static bool8 ForcedMovement_SlideNorth(void)
{
    return ForcedMovement_Slide(DIR_NORTH, PlayerGoSpeed2);
}

static bool8 ForcedMovement_SlideWest(void)
{
    return ForcedMovement_Slide(DIR_WEST, PlayerGoSpeed2);
}

static bool8 ForcedMovement_SlideEast(void)
{
    return ForcedMovement_Slide(DIR_EAST, PlayerGoSpeed2);
}

static bool8 ForcedMovement_MuddySlope(void)
{
    struct ObjectEvent *playerObjEvent = &gObjectEvents[gPlayerAvatar.objectEventId];

    if (playerObjEvent->movementDirection != DIR_NORTH || GetPlayerSpeed() <= 3)
    {
        playerObjEvent->facingDirectionLocked = TRUE;
        return DoForcedMovement(DIR_SOUTH, PlayerGoSpeed2);
    }
    else
    {
        return FALSE;
    }
}

static void MovePlayerNotOnBike(u8 direction, u16 heldKeys)
{
    gUnknown_08497490[CheckMovementInputNotOnBike(direction)](direction, heldKeys);
}

static u8 CheckMovementInputNotOnBike(u8 direction)
{
    if (direction == DIR_NONE)
    {
        gPlayerAvatar.runningState = NOT_MOVING;
        return 0;
    }
    else if (direction != GetPlayerMovementDirection() && gPlayerAvatar.runningState != MOVING)
    {
        gPlayerAvatar.runningState = TURN_DIRECTION;
        return 1;
    }
    else
    {
        gPlayerAvatar.runningState = MOVING;
        return 2;
    }
}

static void PlayerNotOnBikeNotMoving(u8 direction, u16 heldKeys)
{
    PlayerFaceDirection(GetPlayerFacingDirection());
}

static void PlayerNotOnBikeTurningInPlace(u8 direction, u16 heldKeys)
{
    PlayerTurnInPlace(direction);
}

static void PlayerNotOnBikeMoving(u8 direction, u16 heldKeys)
{
    u8 collision = CheckForPlayerAvatarCollision(direction);

    if (collision)
    {
        if (collision == COLLISION_LEDGE_JUMP)
        {
            PlayerJumpLedge(direction);
            return;
        }
        else if (collision == COLLISION_OBJECT_EVENT && IsPlayerCollidingWithFarawayIslandMew(direction))
        {
            PlayerNotOnBikeCollideWithFarawayIslandMew(direction);
            return;
        }
        else
        {
            u8 adjustedCollision = collision - COLLISION_STOP_SURFING;
            if (adjustedCollision > 3)
                PlayerNotOnBikeCollide(direction);
            return;
        }
    }

    if (gPlayerAvatar.flags & PLAYER_AVATAR_FLAG_SURFING)
    {
    // speed 2 is fast, same speed as running
        PlayerGoSpeed2(direction);
        return;
    }

    if (!(gPlayerAvatar.flags & PLAYER_AVATAR_FLAG_UNDERWATER) && (heldKeys & B_BUTTON) && FlagGet(FLAG_SYS_B_DASH)
     && IsRunningDisallowed(gObjectEvents[gPlayerAvatar.objectEventId].currentMetatileBehavior) == 0)
    {
        PlayerRun(direction);
        gPlayerAvatar.flags |= PLAYER_AVATAR_FLAG_DASH;
        return;
    }
    else
    {
        PlayerGoSpeed1(direction);
    }
}

static u8 CheckForPlayerAvatarCollision(u8 direction)
{
    s16 x, y;
    struct ObjectEvent *playerObjEvent = &gObjectEvents[gPlayerAvatar.objectEventId];

    x = playerObjEvent->currentCoords.x;
    y = playerObjEvent->currentCoords.y;
    MoveCoords(direction, &x, &y);
    return CheckForObjectEventCollision(playerObjEvent, x, y, direction, MapGridGetMetatileBehaviorAt(x, y));
}

static u8 sub_808B028(u8 direction)
{
    s16 x, y;
    struct ObjectEvent *playerObjEvent = &gObjectEvents[gPlayerAvatar.objectEventId];

    x = playerObjEvent->currentCoords.x;
    y = playerObjEvent->currentCoords.y;
    MoveCoords(direction, &x, &y);
    return CheckForObjectEventCollision(playerObjEvent, x, y, direction, MapGridGetMetatileBehaviorAt(x, y));
}

u8 CheckForObjectEventCollision(struct ObjectEvent *objectEvent, s16 x, s16 y, u8 direction, u8 metatileBehavior)
{
    u8 collision = GetCollisionAtCoords(objectEvent, x, y, direction);
    if (collision == COLLISION_ELEVATION_MISMATCH && CanStopSurfing(x, y, direction))
        return COLLISION_STOP_SURFING;

    if (ShouldJumpLedge(x, y, direction))
    {
        IncrementGameStat(GAME_STAT_JUMPED_DOWN_LEDGES);
        return COLLISION_LEDGE_JUMP;
    }
    if (collision == COLLISION_OBJECT_EVENT && TryPushBoulder(x, y, direction))
        return COLLISION_PUSHED_BOULDER;

    if (collision == COLLISION_NONE)
    {
        if (CheckForRotatingGatePuzzleCollision(direction, x, y))
            return COLLISION_ROTATING_GATE;
    }
    return collision;
}

static bool8 CanStopSurfing(s16 x, s16 y, u8 direction)
{
    if ((gPlayerAvatar.flags & PLAYER_AVATAR_FLAG_SURFING)
     && MapGridGetZCoordAt(x, y) == 3
     && GetObjectEventIdByXYZ(x, y, 3) == OBJECT_EVENTS_COUNT)
    {
        CreateStopSurfingTask(direction);
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

static bool8 ShouldJumpLedge(s16 x, s16 y, u8 direction)
{
    if (GetLedgeJumpDirection(x, y, direction) != DIR_NONE)
        return TRUE;
    return FALSE;
}

static bool8 TryPushBoulder(s16 x, s16 y, u8 direction)
{
    if (FlagGet(FLAG_SYS_USE_STRENGTH))
    {
        u8 objectEventId = GetObjectEventIdByXY(x, y);

        if (objectEventId != 16 && gObjectEvents[objectEventId].graphicsId == OBJ_EVENT_GFX_PUSHABLE_BOULDER)
        {
            x = gObjectEvents[objectEventId].currentCoords.x;
            y = gObjectEvents[objectEventId].currentCoords.y;
            MoveCoords(direction, &x, &y);
            if (MapGridGetMetatileBehaviorAt(x, y) == MB_MT_PYRE_HOLE
             || (GetCollisionAtCoords(&gObjectEvents[objectEventId], x, y, direction) == COLLISION_NONE
             && !MetatileBehavior_IsNonAnimDoor(MapGridGetMetatileBehaviorAt(x, y))))
            {
                StartStrengthAnim(objectEventId, direction);
                return TRUE;
            }
        }
    }
    return FALSE;
}

bool8 IsPlayerCollidingWithFarawayIslandMew(u8 direction)
{
    u8 mewObjectId;
    struct ObjectEvent *object;
    s16 playerX;
    s16 playerY;
    s16 mewPrevX;

    object = &gObjectEvents[gPlayerAvatar.objectEventId];
    playerX = object->currentCoords.x;
    playerY = object->currentCoords.y;

    MoveCoords(direction, &playerX, &playerY);
    mewObjectId = GetObjectEventIdByLocalIdAndMap(1, MAP_NUM(FARAWAY_ISLAND_INTERIOR), MAP_GROUP(FARAWAY_ISLAND_INTERIOR));
    if (mewObjectId == OBJECT_EVENTS_COUNT)
        return FALSE;

    object = &gObjectEvents[mewObjectId];
    mewPrevX = object->previousCoords.x;

    if (mewPrevX == playerX)
    {
        if (object->previousCoords.y != playerY
         || object->currentCoords.x != mewPrevX
         || object->currentCoords.y != object->previousCoords.y)
        {
            if (object->previousCoords.x == playerX
             && object->previousCoords.y == playerY)
                return TRUE;
        }
    }
    return FALSE;
}

void SetPlayerAvatarTransitionFlags(u16 transitionFlags)
{
    gPlayerAvatar.transitionFlags |= transitionFlags;
    DoPlayerAvatarTransition();
}

static void DoPlayerAvatarTransition(void)
{
    u32 i;
    u8 flags = gPlayerAvatar.transitionFlags;

    if (flags != 0)
    {
        for (i = 0; i < ARRAY_COUNT(sPlayerAvatarTransitionFuncs); i++, flags >>= 1)
        {
            if (flags & 1)
                sPlayerAvatarTransitionFuncs[i](&gObjectEvents[gPlayerAvatar.objectEventId]);
        }
        gPlayerAvatar.transitionFlags = 0;
    }
}

static void PlayerAvatarTransition_Dummy(struct ObjectEvent *objEvent)
{

}

static void PlayerAvatarTransition_Normal(struct ObjectEvent *objEvent)
{
    ObjectEventSetGraphicsId(objEvent, GetPlayerAvatarGraphicsIdByStateId(PLAYER_AVATAR_STATE_NORMAL));
    ObjectEventTurn(objEvent, objEvent->movementDirection);
    SetPlayerAvatarStateMask(PLAYER_AVATAR_FLAG_ON_FOOT);
}

static void PlayerAvatarTransition_Bike(struct ObjectEvent *objEvent)
{
    ObjectEventSetGraphicsId(objEvent, GetPlayerAvatarGraphicsIdByStateId(PLAYER_AVATAR_STATE_BIKE));
    ObjectEventTurn(objEvent, objEvent->movementDirection);
    SetPlayerAvatarStateMask(PLAYER_AVATAR_FLAG_BIKE);
    BikeClearState();
}

static void PlayerAvatarTransition_Surfing(struct ObjectEvent *objEvent)
{
    u8 spriteId;

    ObjectEventSetGraphicsId(objEvent, GetPlayerAvatarGraphicsIdByStateId(PLAYER_AVATAR_STATE_SURFING));
    ObjectEventTurn(objEvent, objEvent->movementDirection);
    SetPlayerAvatarStateMask(PLAYER_AVATAR_FLAG_SURFING);
    gFieldEffectArguments[0] = objEvent->currentCoords.x;
    gFieldEffectArguments[1] = objEvent->currentCoords.y;
    gFieldEffectArguments[2] = gPlayerAvatar.objectEventId;
    spriteId = FieldEffectStart(FLDEFF_SURF_BLOB);
    objEvent->fieldEffectSpriteId = spriteId;
    SetSurfBlob_BobState(spriteId, BOB_PLAYER_AND_MON);
}

static void PlayerAvatarTransition_Underwater(struct ObjectEvent *objEvent)
{
    ObjectEventSetGraphicsId(objEvent, GetPlayerAvatarGraphicsIdByStateId(PLAYER_AVATAR_STATE_UNDERWATER));
    ObjectEventTurn(objEvent, objEvent->movementDirection);
    SetPlayerAvatarStateMask(PLAYER_AVATAR_FLAG_UNDERWATER);
    objEvent->fieldEffectSpriteId = StartUnderwaterSurfBlobBobbing(objEvent->spriteId);
}

static void PlayerAvatarTransition_ReturnToField(struct ObjectEvent *objEvent)
{
    gPlayerAvatar.flags |= PLAYER_AVATAR_FLAG_4;
}

void UpdatePlayerAvatarTransitionState(void)
{
    gPlayerAvatar.tileTransitionState = T_NOT_MOVING;
    if (PlayerIsAnimActive())
    {
        if (!PlayerCheckIfAnimFinishedOrInactive())
        {
            if (!player_is_anim_in_certain_ranges())
                gPlayerAvatar.tileTransitionState = T_TILE_TRANSITION;
        }
        else
        {
            if (!sub_808B618())
                gPlayerAvatar.tileTransitionState = T_TILE_CENTER;
        }
    }
}

static bool8 player_is_anim_in_certain_ranges(void)
{
    u8 movementActionId = gObjectEvents[gPlayerAvatar.objectEventId].movementActionId;

    if (movementActionId <= MOVEMENT_ACTION_FACE_RIGHT
     || (movementActionId >= MOVEMENT_ACTION_DELAY_1&& movementActionId <= MOVEMENT_ACTION_DELAY_16)
     || (movementActionId >= MOVEMENT_ACTION_WALK_IN_PLACE_SLOW_DOWN && movementActionId <= MOVEMENT_ACTION_WALK_IN_PLACE_FASTEST_RIGHT))
        return TRUE;
    else
        return FALSE;
}

static bool8 sub_808B618(void)
{
    if (player_is_anim_in_certain_ranges() && gPlayerAvatar.runningState != TURN_DIRECTION)
        return TRUE;
    else
        return FALSE;
}

static bool8 PlayerIsAnimActive(void)
{
    return ObjectEventIsMovementOverridden(&gObjectEvents[gPlayerAvatar.objectEventId]);
}

static bool8 PlayerCheckIfAnimFinishedOrInactive(void)
{
    return ObjectEventCheckHeldMovementStatus(&gObjectEvents[gPlayerAvatar.objectEventId]);
}

static void PlayerSetCopyableMovement(u8 a)
{
    gObjectEvents[gPlayerAvatar.objectEventId].playerCopyableMovement = a;
}

u8 PlayerGetCopyableMovement(void)
{
    return gObjectEvents[gPlayerAvatar.objectEventId].playerCopyableMovement;
}

static void PlayerForceSetHeldMovement(u8 movementActionId)
{
    ObjectEventForceSetHeldMovement(&gObjectEvents[gPlayerAvatar.objectEventId], movementActionId);
}

void PlayerSetAnimId(u8 movementActionId, u8 copyableMovement)
{
    if (!PlayerIsAnimActive())
    {
        PlayerSetCopyableMovement(copyableMovement);
        ObjectEventSetHeldMovement(&gObjectEvents[gPlayerAvatar.objectEventId], movementActionId);
    }
}

// normal speed (1 speed)
void PlayerGoSpeed1(u8 a)
{
    PlayerSetAnimId(GetWalkNormalMovementAction(a), 2);
}

// fast speed (2 speed)
void PlayerGoSpeed2(u8 a)
{
    PlayerSetAnimId(GetWalkFastMovementAction(a), 2);
}

void PlayerRideWaterCurrent(u8 a)
{
    PlayerSetAnimId(GetRideWaterCurrentMovementAction(a), 2);
}

// fastest speed (4 speed)
void PlayerGoSpeed4(u8 a)
{
    PlayerSetAnimId(GetWalkFastestMovementAction(a), 2);
}

static void PlayerRun(u8 a)
{
    PlayerSetAnimId(GetPlayerRunMovementAction(a), 2);
}

void PlayerOnBikeCollide(u8 a)
{
    PlayCollisionSoundIfNotFacingWarp(a);
    PlayerSetAnimId(GetWalkInPlaceNormalMovementAction(a), 2);
}

void PlayerOnBikeCollideWithFarawayIslandMew(u8 a)
{
    PlayerSetAnimId(GetWalkInPlaceNormalMovementAction(a), 2);
}

static void PlayerNotOnBikeCollide(u8 a)
{
    PlayCollisionSoundIfNotFacingWarp(a);
    PlayerSetAnimId(GetWalkInPlaceSlowMovementAction(a), 2);
}

static void PlayerNotOnBikeCollideWithFarawayIslandMew(u8 a)
{
    PlayerSetAnimId(GetWalkInPlaceSlowMovementAction(a), 2);
}

void PlayerFaceDirection(u8 direction)
{
    PlayerSetAnimId(GetFaceDirectionMovementAction(direction), 1);
}

void PlayerTurnInPlace(u8 direction)
{
    PlayerSetAnimId(GetWalkInPlaceFastMovementAction(direction), 1);
}

void PlayerJumpLedge(u8 direction)
{
    PlaySE(SE_LEDGE);
    PlayerSetAnimId(GetJump2MovementAction(direction), 8);
}

// Stop player on current facing direction once they're done moving and if they're not currently Acro Biking on bumpy slope
void PlayerFreeze(void)
{
    if (gPlayerAvatar.tileTransitionState == T_TILE_CENTER || gPlayerAvatar.tileTransitionState == T_NOT_MOVING)
    {
        PlayerForceSetHeldMovement(GetFaceDirectionMovementAction(gObjectEvents[gPlayerAvatar.objectEventId].facingDirection));
    }
}

static void PlayCollisionSoundIfNotFacingWarp(u8 a)
{
    s16 x, y;
    u8 metatileBehavior = gObjectEvents[gPlayerAvatar.objectEventId].currentMetatileBehavior;

    if (!sArrowWarpMetatileBehaviorChecks[a - 1](metatileBehavior))
    {
        if (a == 2)
        {
            PlayerGetDestCoords(&x, &y);
            MoveCoords(2, &x, &y);
            if (MetatileBehavior_IsWarpDoor(MapGridGetMetatileBehaviorAt(x, y)))
                return;
        }
        PlaySE(SE_WALL_HIT);
    }
}

void GetXYCoordsOneStepInFrontOfPlayer(s16 *x, s16 *y)
{
    *x = gObjectEvents[gPlayerAvatar.objectEventId].currentCoords.x;
    *y = gObjectEvents[gPlayerAvatar.objectEventId].currentCoords.y;
    MoveCoords(GetPlayerFacingDirection(), x, y);
}

void PlayerGetDestCoords(s16 *x, s16 *y)
{
    *x = gObjectEvents[gPlayerAvatar.objectEventId].currentCoords.x;
    *y = gObjectEvents[gPlayerAvatar.objectEventId].currentCoords.y;
}

u8 player_get_pos_including_state_based_drift(s16 *x, s16 *y)
{
    struct ObjectEvent *object = &gObjectEvents[gPlayerAvatar.objectEventId];

    if (object->heldMovementActive && !object->heldMovementFinished && !gSprites[object->spriteId].data[2])
    {
        *x = object->currentCoords.x;
        *y = object->currentCoords.y;

        switch (object->movementActionId)
        {
            case MOVEMENT_ACTION_WALK_NORMAL_DOWN:
            case MOVEMENT_ACTION_PLAYER_RUN_DOWN:
                (*y)++;
                return TRUE;
            case MOVEMENT_ACTION_WALK_NORMAL_UP:
            case MOVEMENT_ACTION_PLAYER_RUN_UP:
                (*y)--;
                return TRUE;
            case MOVEMENT_ACTION_WALK_NORMAL_LEFT:
            case MOVEMENT_ACTION_PLAYER_RUN_LEFT:
                (*x)--;
                return TRUE;
            case MOVEMENT_ACTION_WALK_NORMAL_RIGHT:
            case MOVEMENT_ACTION_PLAYER_RUN_RIGHT:
                (*x)++;
                return TRUE;
        }
    }

    *x = -1;
    *y = -1;
    return FALSE;
}

u8 GetPlayerFacingDirection(void)
{
    return gObjectEvents[gPlayerAvatar.objectEventId].facingDirection;
}

u8 GetPlayerMovementDirection(void)
{
    return gObjectEvents[gPlayerAvatar.objectEventId].movementDirection;
}

u8 PlayerGetZCoord(void)
{
    return gObjectEvents[gPlayerAvatar.objectEventId].previousElevation;
}

void sub_808BC90(s16 x, s16 y)
{
    MoveObjectEventToMapCoords(&gObjectEvents[gPlayerAvatar.objectEventId], x, y);
}

u8 TestPlayerAvatarFlags(u8 flag)
{
    return gPlayerAvatar.flags & flag;
}

u8 GetPlayerAvatarFlags(void)
{
    return gPlayerAvatar.flags;
}

u8 GetPlayerAvatarSpriteId(void)
{
    return gPlayerAvatar.spriteId;
}

void sub_808BCE8(void)
{
    ForcedMovement_None();
}

void sub_808BCF4(void)
{
    struct ObjectEvent *playerObjEvent = &gObjectEvents[gPlayerAvatar.objectEventId];

    npc_clear_strange_bits(playerObjEvent);
    SetObjectEventDirection(playerObjEvent, playerObjEvent->facingDirection);
}

u16 GetRSAvatarGraphicsIdByGender(u8 gender)
{
    return sRSAvatarGfxIds[gender];
}

u16 GetHAvatarGraphicsIdByGender(u8 gender)
{
    return sHeliodorAvatarGfxIds[gender];
}

u16 GetEAvatarGraphicsIdByGender(u8 gender)
{
    return sEAvatarGfxIds[gender];
}
u16 GetCDAvatarGraphicsIdByGender(u8 gender)
{
    return sCDAvatarGfxIds[gender];
}

u16 GetPlayerAvatarGraphicsIdByStateIdAndGender(u8 state, u8 gender)
{
    return sPlayerAvatarGfxIds[state][gender];
}

u16 GetPlayerAvatarGraphicsIdByStateId(u8 state)
{
    return GetPlayerAvatarGraphicsIdByStateIdAndGender(state, gPlayerAvatar.gender);
}

bool8 PartyHasMonWithSurf(void)
{
    u32 i;

    if (!TestPlayerAvatarFlags(PLAYER_AVATAR_FLAG_SURFING))
    {
        for (i = 0; i < PARTY_SIZE; i++)
        {
            if (GetMonData(&gPlayerParty[i], MON_DATA_SPECIES) == SPECIES_NONE)
                break;
            if (MonKnowsMove(&gPlayerParty[i], MOVE_SURF))
                return TRUE;
        }
    }
    return FALSE;
}

bool8 IsPlayerSurfingNorth(void)
{
    if (GetPlayerMovementDirection() == DIR_NORTH && TestPlayerAvatarFlags(PLAYER_AVATAR_FLAG_SURFING))
        return TRUE;
    else
        return FALSE;
}

bool8 IsPlayerFacingSurfableFishableWater(void)
{
    struct ObjectEvent *playerObjEvent = &gObjectEvents[gPlayerAvatar.objectEventId];
    s16 x = playerObjEvent->currentCoords.x;
    s16 y = playerObjEvent->currentCoords.y;

    MoveCoords(playerObjEvent->facingDirection, &x, &y);
    if (GetCollisionAtCoords(playerObjEvent, x, y, playerObjEvent->facingDirection) == COLLISION_ELEVATION_MISMATCH
     && PlayerGetZCoord() == 3
     && MetatileBehavior_IsSurfableFishableWater(MapGridGetMetatileBehaviorAt(x, y)))
        return TRUE;
    else
        return FALSE;
}

void ClearPlayerAvatarInfo(void)
{
    memset(&gPlayerAvatar, 0, sizeof(struct PlayerAvatar));
}

void SetPlayerAvatarStateMask(u8 flags)
{
    gPlayerAvatar.flags &= (PLAYER_AVATAR_FLAG_DASH | PLAYER_AVATAR_FLAG_FORCED_MOVE | PLAYER_AVATAR_FLAG_4);
    gPlayerAvatar.flags |= flags;
}

static u8 GetPlayerAvatarStateTransitionByGraphicsId(u8 graphicsId, u8 gender)
{
    u32 i;

    for (i = 0; i < ARRAY_COUNT(sPlayerAvatarGfxToStateFlag[0]); i++)
    {
        if (sPlayerAvatarGfxToStateFlag[gender][i][0] == graphicsId)
            return sPlayerAvatarGfxToStateFlag[gender][i][1];
    }
    return PLAYER_AVATAR_FLAG_ON_FOOT;
}

u16 GetPlayerAvatarGraphicsIdByCurrentState(void)
{
    u32 i;
    u8 flags = gPlayerAvatar.flags;

    for (i = 0; i < ARRAY_COUNT(sPlayerAvatarGfxToStateFlag[0]); i++)
    {
        if (sPlayerAvatarGfxToStateFlag[gPlayerAvatar.gender][i][1] & flags)
            return sPlayerAvatarGfxToStateFlag[gPlayerAvatar.gender][i][0];
    }
    return 0;
}

void SetPlayerAvatarExtraStateTransition(u16 graphicsId, u8 transitionFlag)
{
    u8 stateFlag = GetPlayerAvatarStateTransitionByGraphicsId(graphicsId, gPlayerAvatar.gender);

    gPlayerAvatar.transitionFlags |= stateFlag | transitionFlag;
    DoPlayerAvatarTransition();
}

void InitPlayerAvatar(s16 x, s16 y, u8 direction, u8 gender)
{
    struct ObjectEventTemplate playerObjEventTemplate;
    u8 objectEventId;
    struct ObjectEvent *objectEvent;

    playerObjEventTemplate.localId = OBJ_EVENT_ID_PLAYER;
    playerObjEventTemplate.graphicsId = GetPlayerAvatarGraphicsIdByStateIdAndGender(PLAYER_AVATAR_STATE_NORMAL, gender);
    playerObjEventTemplate.x = x - 7;
    playerObjEventTemplate.y = y - 7;
    playerObjEventTemplate.elevation = 0;
    playerObjEventTemplate.movementType = MOVEMENT_TYPE_PLAYER;
    playerObjEventTemplate.movementRangeX = 0;
    playerObjEventTemplate.movementRangeY = 0;
    playerObjEventTemplate.trainerType = TRAINER_TYPE_NONE;
    playerObjEventTemplate.trainerRange_berryTreeId = 0;
    playerObjEventTemplate.script = NULL;
    playerObjEventTemplate.flagId = 0;
    objectEventId = SpawnSpecialObjectEvent(&playerObjEventTemplate);
    objectEvent = &gObjectEvents[objectEventId];
    objectEvent->isPlayer = TRUE;
    objectEvent->warpArrowSpriteId = CreateWarpArrowSprite();
    ObjectEventTurn(objectEvent, direction);
    ClearPlayerAvatarInfo();
    gPlayerAvatar.runningState = NOT_MOVING;
    gPlayerAvatar.tileTransitionState = T_NOT_MOVING;
    gPlayerAvatar.objectEventId = objectEventId;
    gPlayerAvatar.spriteId = objectEvent->spriteId;
    gPlayerAvatar.gender = gender;
    SetPlayerAvatarStateMask(PLAYER_AVATAR_FLAG_4 | PLAYER_AVATAR_FLAG_ON_FOOT);
}

void SetPlayerInvisibility(bool8 invisible)
{
    gObjectEvents[gPlayerAvatar.objectEventId].invisible = invisible;
    if (TestPlayerAvatarFlags(PLAYER_AVATAR_FLAG_SURFING))
        gSprites[gObjectEvents[gPlayerAvatar.objectEventId].fieldEffectSpriteId].invisible = invisible;
}

void SetPlayerAvatarFieldMove(void)
{
    if (gPlayerAvatar.flags & PLAYER_AVATAR_FLAG_BIKE)
        ObjectEventSetGraphicsId(&gObjectEvents[gPlayerAvatar.objectEventId], GetPlayerAvatarGraphicsIdByStateId(PLAYER_AVATAR_STATE_FIELD_MOVE_BIKE));
    else
        ObjectEventSetGraphicsId(&gObjectEvents[gPlayerAvatar.objectEventId], GetPlayerAvatarGraphicsIdByStateId(PLAYER_AVATAR_STATE_FIELD_MOVE));
    StartSpriteAnim(&gSprites[gPlayerAvatar.spriteId], 0);
}

static void SetPlayerAvatarFishing(u8 direction)
{
    ObjectEventSetGraphicsId(&gObjectEvents[gPlayerAvatar.objectEventId], GetPlayerAvatarGraphicsIdByStateId(PLAYER_AVATAR_STATE_FISHING));
    StartSpriteAnim(&gSprites[gPlayerAvatar.spriteId], GetFishingDirectionAnimNum(direction));
}

void SetPlayerAvatarWatering(u8 direction)
{
    ObjectEventSetGraphicsId(&gObjectEvents[gPlayerAvatar.objectEventId], GetPlayerAvatarGraphicsIdByStateId(PLAYER_AVATAR_STATE_WATERING));
    StartSpriteAnim(&gSprites[gPlayerAvatar.spriteId], GetFaceDirectionAnimNum(direction));
}

static void HideShowWarpArrow(struct ObjectEvent *objectEvent)
{
    s16 x, y;
    u32 direction;
    u8 metatileBehavior = objectEvent->currentMetatileBehavior;

    for (x = 0, direction = DIR_SOUTH; x < 4; x++, direction++)
    {
        if (sArrowWarpMetatileBehaviorChecks[x](metatileBehavior) && direction == objectEvent->movementDirection)
        {
            // Show warp arrow if applicable
            x = objectEvent->currentCoords.x;
            y = objectEvent->currentCoords.y;
            MoveCoords(direction, &x, &y);
            ShowWarpArrowSprite(objectEvent->warpArrowSpriteId, direction, x, y);
            return;
        }
    }
    SetSpriteInvisible(objectEvent->warpArrowSpriteId);
}

/* Strength */

static void StartStrengthAnim(u8 a, u8 b)
{
    u8 taskId = CreateTask(Task_PushBoulder, 0xFF);

    gTasks[taskId].data[1] = a;
    gTasks[taskId].data[2] = b;
    Task_PushBoulder(taskId);
}

static void Task_PushBoulder(u8 taskId)
{
    while (sPushBoulderFuncs[gTasks[taskId].data[0]](&gTasks[taskId],
                                                     &gObjectEvents[gPlayerAvatar.objectEventId],
                                                     &gObjectEvents[gTasks[taskId].data[1]]))
        ;
}

static bool8 PushBoulder_Start(struct Task *task, struct ObjectEvent *playerObject, struct ObjectEvent *strengthObject)
{
    ScriptContext2_Enable();
    gPlayerAvatar.preventStep = TRUE;
    task->data[0]++;
    return FALSE;
}

static bool8 PushBoulder_Move(struct Task *task, struct ObjectEvent *playerObject, struct ObjectEvent *strengthObject)
{
    if (ObjectEventIsHeldMovementActive(playerObject))
    {
        ObjectEventClearHeldMovementIfFinished(playerObject);
    }

    if (ObjectEventIsHeldMovementActive(strengthObject))
    {
        ObjectEventClearHeldMovementIfFinished(strengthObject);
    }

    if (!ObjectEventIsMovementOverridden(playerObject)
     && !ObjectEventIsMovementOverridden(strengthObject))
    {
        ObjectEventClearHeldMovementIfFinished(playerObject);
        ObjectEventClearHeldMovementIfFinished(strengthObject);
        ObjectEventSetHeldMovement(playerObject, GetWalkInPlaceNormalMovementAction((u8)task->data[2]));
        ObjectEventSetHeldMovement(strengthObject, GetWalkSlowMovementAction((u8)task->data[2]));
        gFieldEffectArguments[0] = strengthObject->currentCoords.x;
        gFieldEffectArguments[1] = strengthObject->currentCoords.y;
        gFieldEffectArguments[2] = strengthObject->previousElevation;
        gFieldEffectArguments[3] = gSprites[strengthObject->spriteId].oam.priority;
        FieldEffectStart(FLDEFF_DUST);
        PlaySE(SE_M_STRENGTH);
        task->data[0]++;
    }
    return FALSE;
}

static bool8 PushBoulder_End(struct Task *task, struct ObjectEvent *playerObject, struct ObjectEvent *strengthObject)
{
    if (ObjectEventCheckHeldMovementStatus(playerObject)
     && ObjectEventCheckHeldMovementStatus(strengthObject))
    {
        ObjectEventClearHeldMovementIfFinished(playerObject);
        ObjectEventClearHeldMovementIfFinished(strengthObject);
        gPlayerAvatar.preventStep = FALSE;
        ScriptContext2_Disable();
        DestroyTask(FindTaskIdByFunc(Task_PushBoulder));
    }
    return FALSE;
}

/* Some field effect */

static void CreateStopSurfingTask(u8 direction)
{
    u8 taskId;

    ScriptContext2_Enable();
    Overworld_ClearSavedMusic();
    Overworld_ChangeMusicToDefault();
    gPlayerAvatar.flags &= ~PLAYER_AVATAR_FLAG_SURFING;
    gPlayerAvatar.flags |= PLAYER_AVATAR_FLAG_ON_FOOT;
    gPlayerAvatar.preventStep = TRUE;
    taskId = CreateTask(Task_StopSurfingInit, 0xFF);
    gTasks[taskId].data[0] = direction;
    Task_StopSurfingInit(taskId);
}

static void Task_StopSurfingInit(u8 taskId)
{
    struct ObjectEvent *playerObjEvent = &gObjectEvents[gPlayerAvatar.objectEventId];

    if (ObjectEventIsMovementOverridden(playerObjEvent))
    {
        if (!ObjectEventClearHeldMovementIfFinished(playerObjEvent))
            return;
    }
    SetSurfBlob_BobState(playerObjEvent->fieldEffectSpriteId, BOB_JUST_MON);
    ObjectEventSetHeldMovement(playerObjEvent, GetJumpSpecialMovementAction((u8)gTasks[taskId].data[0]));
    gTasks[taskId].func = Task_WaitStopSurfing;
}

static void Task_WaitStopSurfing(u8 taskId)
{
    struct ObjectEvent *playerObjEvent = &gObjectEvents[gPlayerAvatar.objectEventId];

    if (ObjectEventClearHeldMovementIfFinished(playerObjEvent))
    {
        ObjectEventSetGraphicsId(playerObjEvent, GetPlayerAvatarGraphicsIdByStateId(PLAYER_AVATAR_STATE_NORMAL));
        ObjectEventSetHeldMovement(playerObjEvent, GetFaceDirectionMovementAction(playerObjEvent->facingDirection));
        gPlayerAvatar.preventStep = FALSE;
        ScriptContext2_Disable();
        DestroySprite(&gSprites[playerObjEvent->fieldEffectSpriteId]);
        DestroyTask(taskId);
    }
}

#define tStep              data[0]
#define tFrameCounter      data[1]
#define tNumDots           data[2]
#define tDotsRequired      data[3]
#define tPlayerGfxId       data[14]
#define tFishingRod        data[15]

// Some states are jumped to directly, labeled below
#define FISHING_START_ROUND 3
#define FISHING_ON_HOOK 5
#define FISHING_NO_BITE 8
#define FISHING_SHOW_RESULT 9

static bool8 (*const sFishingStateFuncs[])(struct Task *) =
{
    Fishing_Init,
    Fishing_GetRodOut,
    Fishing_WaitBeforeDots, 
    Fishing_InitDots,       // FISHING_START_ROUND
    Fishing_ShowDots,
    Fishing_CheckForBite,
    Fishing_MonOnHook,      // FISHING_ON_HOOK
    Fishing_StartEncounter,
    Fishing_NotEvenNibble,  // FISHING_NO_BITE
    Fishing_NoMon,          // FISHING_SHOW_RESULT
    Fishing_PutRodAway,
    Fishing_EndNoMon,
};

void StartFishing(u8 rod)
{
    u8 taskId = CreateTask(Task_Fishing, 0xFF);

    gTasks[taskId].tFishingRod = rod;
    Task_Fishing(taskId);
}

static void Task_Fishing(u8 taskId)
{
    while (sFishingStateFuncs[gTasks[taskId].tStep](&gTasks[taskId]))
        ;
}

static bool8 Fishing_Init(struct Task *task)
{
    ScriptContext2_Enable();
    gPlayerAvatar.preventStep = TRUE;
    task->tStep++;
    return FALSE;
}

static bool8 Fishing_GetRodOut(struct Task *task)
{
    struct ObjectEvent *playerObjEvent;
    task->tPlayerGfxId = gObjectEvents[gPlayerAvatar.objectEventId].graphicsId;
    playerObjEvent = &gObjectEvents[gPlayerAvatar.objectEventId];
    ObjectEventClearHeldMovementIfActive(playerObjEvent);
    playerObjEvent->enableAnim = TRUE;
    SetPlayerAvatarFishing(playerObjEvent->facingDirection);
    task->tStep++;
    return FALSE;
}

static bool8 Fishing_WaitBeforeDots(struct Task *task)
{
    AlignFishingAnimationFrames();

    // Wait one second
    task->tFrameCounter++;
    if (task->tFrameCounter >= 60)
        task->tStep++;
    return FALSE;
}

static bool8 Fishing_InitDots(struct Task *task)
{
    u32 randVal;

    sub_819786C(0, TRUE);
    task->tStep++;
    task->tFrameCounter = 0;
    task->tNumDots = 0;
    randVal = Random();
    randVal %= 10;
    task->tDotsRequired = randVal + 1;
    return TRUE;
}

static bool8 Fishing_ShowDots(struct Task *task)
{
    const u8 dot[] = _("·");

    AlignFishingAnimationFrames();
    task->tFrameCounter++;
    if (task->tFrameCounter >= 20)
    {
        task->tFrameCounter = 0;
        if (task->tNumDots >= task->tDotsRequired)
        {
            task->tStep++;
        }
        else
        {
            AddTextPrinterParameterized(0, 2, dot, task->tNumDots * 12, 1, 0, NULL);
            task->tNumDots++;
        }
    }
    return FALSE;
}

static bool8 Fishing_CheckForBite(struct Task *task)
{
    bool8 bite;

    AlignFishingAnimationFrames();
    task->tStep++;
    bite = FALSE;

    if (!DoesCurrentMapHaveFishingMons())
    {
        task->tStep = FISHING_NO_BITE;
    }
    else
    {
        if (!GetMonData(&gPlayerParty[0], MON_DATA_SANITY_IS_EGG))
        {
            u8 ability = GetMonAbility(&gPlayerParty[0]);
            if (ability == ABILITY_SUCTION_CUPS || ability  == ABILITY_STICKY_HOLD)
            {
                if (Random() % 100 > 14)
                    bite = TRUE;
            }
        }

        if (!bite)
        {
            if (Random() & 1)
                task->tStep = FISHING_NO_BITE;
            else
                bite = TRUE;
        }

        if (bite)
            StartSpriteAnim(&gSprites[gPlayerAvatar.spriteId], GetFishingBiteDirectionAnimNum(GetPlayerFacingDirection()));
    }
    return TRUE;
}

static bool8 Fishing_MonOnHook(struct Task *task)
{
    AlignFishingAnimationFrames();
    FillWindowPixelBuffer(0, PIXEL_FILL(1));
    AddTextPrinterParameterized2(0, 2, gText_PokemonOnHook, 1, 0, 2, 1, 3);
    task->tStep++;
    task->tFrameCounter = 0;
    return FALSE;
}

static bool8 Fishing_StartEncounter(struct Task *task)
{
    if (task->tFrameCounter == 0)
        AlignFishingAnimationFrames();

    RunTextPrinters();

    if (task->tFrameCounter == 0)
    {
        if (!IsTextPrinterActive(0))
        {
            struct ObjectEvent *playerObjEvent = &gObjectEvents[gPlayerAvatar.objectEventId];

            ObjectEventSetGraphicsId(playerObjEvent, task->tPlayerGfxId);
            ObjectEventTurn(playerObjEvent, playerObjEvent->movementDirection);
            if (gPlayerAvatar.flags & PLAYER_AVATAR_FLAG_SURFING)
                SetSurfBlob_PlayerOffset(gObjectEvents[gPlayerAvatar.objectEventId].fieldEffectSpriteId, FALSE, 0);
            gSprites[gPlayerAvatar.spriteId].x2 = 0;
            gSprites[gPlayerAvatar.spriteId].y2 = 0;
            ClearDialogWindowAndFrame(0, TRUE);
            task->tFrameCounter++;
            return FALSE;
        }
    }

    if (task->tFrameCounter != 0)
    {
        gPlayerAvatar.preventStep = FALSE;
        ScriptContext2_Disable();
        FishingWildEncounter(task->tFishingRod);
        DestroyTask(FindTaskIdByFunc(Task_Fishing));
    }
    return FALSE;
}

// Not even a nibble
static bool8 Fishing_NotEvenNibble(struct Task *task)
{
    gChainFishingStreak = 0;
    AlignFishingAnimationFrames();
    StartSpriteAnim(&gSprites[gPlayerAvatar.spriteId], GetFishingNoCatchDirectionAnimNum(GetPlayerFacingDirection()));
    FillWindowPixelBuffer(0, PIXEL_FILL(1));
    AddTextPrinterParameterized2(0, 2, gText_NotEvenANibble, 1, 0, 2, 1, 3);
    task->tStep++;
    return TRUE;
}

static bool8 Fishing_NoMon(struct Task *task)
{
    AlignFishingAnimationFrames();
    task->tStep++;
    return FALSE;
}

static bool8 Fishing_PutRodAway(struct Task *task)
{
    AlignFishingAnimationFrames();
    if (gSprites[gPlayerAvatar.spriteId].animEnded)
    {
        struct ObjectEvent *playerObjEvent = &gObjectEvents[gPlayerAvatar.objectEventId];

        ObjectEventSetGraphicsId(playerObjEvent, task->tPlayerGfxId);
        ObjectEventTurn(playerObjEvent, playerObjEvent->movementDirection);
        if (gPlayerAvatar.flags & PLAYER_AVATAR_FLAG_SURFING)
            SetSurfBlob_PlayerOffset(gObjectEvents[gPlayerAvatar.objectEventId].fieldEffectSpriteId, FALSE, 0);
        gSprites[gPlayerAvatar.spriteId].x2 = 0;
        gSprites[gPlayerAvatar.spriteId].y2 = 0;
        task->tStep++;
    }
    return FALSE;
}

static bool8 Fishing_EndNoMon(struct Task *task)
{
    RunTextPrinters();
    if (!IsTextPrinterActive(0))
    {
        gPlayerAvatar.preventStep = FALSE;
        ScriptContext2_Disable();
        UnfreezeObjectEvents();
        ClearDialogWindowAndFrame(0, TRUE);
        DestroyTask(FindTaskIdByFunc(Task_Fishing));
    }
    return FALSE;
}

#undef tStep
#undef tFrameCounter
#undef tFishingRod

static void AlignFishingAnimationFrames(void)
{
    struct Sprite *playerSprite = &gSprites[gPlayerAvatar.spriteId];
    u8 animCmdIndex;
    u8 animType;

    AnimateSprite(playerSprite);
    playerSprite->x2 = 0;
    playerSprite->y2 = 0;
    animCmdIndex = playerSprite->animCmdIndex;
    if (playerSprite->anims[playerSprite->animNum][animCmdIndex].type == -1)
    {
        animCmdIndex--;
    }
    else
    {
        playerSprite->animDelayCounter++;
        if (playerSprite->anims[playerSprite->animNum][animCmdIndex].type == -1)
            animCmdIndex--;
    }
    animType = playerSprite->anims[playerSprite->animNum][animCmdIndex].type;
    if (animType == 1 || animType == 2 || animType == 3)
    {
        playerSprite->x2 = 8;
        if (GetPlayerFacingDirection() == 3)
            playerSprite->x2 = -8;
    }
    if (animType == 5)
        playerSprite->y2 = -8;
    if (animType == 10 || animType == 11)
        playerSprite->y2 = 8;
    if (gPlayerAvatar.flags & PLAYER_AVATAR_FLAG_SURFING)
        SetSurfBlob_PlayerOffset(gObjectEvents[gPlayerAvatar.objectEventId].fieldEffectSpriteId, TRUE, playerSprite->y2);
}

void SetSpinStartFacingDir(u8 direction)
{
    sSpinStartFacingDir = direction;
}

static u8 GetSpinStartFacingDir(void)
{
    if (sSpinStartFacingDir == DIR_NONE)
        return DIR_SOUTH;

    return sSpinStartFacingDir;
}

// Task data for Task_DoPlayerSpinEntrance and Task_DoPlayerSpinExit
#define tState          data[0]
#define tSpinDelayTimer data[1]
#define tSpeed          data[2]
#define tCurY           data[3]
#define tDestY          data[4]
#define tStartDir       data[5]
#define tPriority       data[6]
#define tSubpriority    data[7]
#define tGroundTimer    data[8]

static void Task_DoPlayerSpinExit(u8 taskId)
{
    struct ObjectEvent *object = &gObjectEvents[gPlayerAvatar.objectEventId];
    struct Sprite *sprite = &gSprites[object->spriteId];
    s16 *data = gTasks[taskId].data;

    switch (tState)
    {
        case 0: // Init
            if (!ObjectEventClearHeldMovementIfFinished(object))
                return;

            SetSpinStartFacingDir(object->facingDirection);
            tSpinDelayTimer = 0;
            tSpeed = 1;
            tCurY = (u16)(sprite->y + sprite->y2) << 4;
            sprite->y2 = 0;
            CameraObjectReset2();
            object->fixedPriority = TRUE;
            sprite->oam.priority = 0;
            sprite->subpriority = 0;
            sprite->subspriteMode = SUBSPRITES_OFF;
            tState++;
        case 1: // Spin while rising
            TrySpinPlayerForWarp(object, &tSpinDelayTimer);
            
            // Rise and accelerate
            tCurY -= tSpeed;
            tSpeed += 3;
            sprite->y = tCurY >> 4;

            // Check if offscreen
            if (sprite->y + (s16)gTotalCameraPixelOffsetY < -32)
                tState++;
            break;
        case 2:
            DestroyTask(taskId);
            break;
    }
}

static void Task_DoPlayerSpinEntrance(u8 taskId);

void DoPlayerSpinEntrance(void)
{
    Task_DoPlayerSpinEntrance(CreateTask(Task_DoPlayerSpinEntrance, 0));
}

bool32 IsPlayerSpinEntranceActive(void)
{
    return FuncIsActiveTask(Task_DoPlayerSpinEntrance);
}

void DoPlayerSpinExit(void)
{
    Task_DoPlayerSpinExit(CreateTask(Task_DoPlayerSpinExit, 0));
}

bool32 IsPlayerSpinExitActive(void)
{
    return FuncIsActiveTask(Task_DoPlayerSpinExit);
}

static const u8 sSpinDirections[] = {DIR_SOUTH, DIR_WEST, DIR_EAST, DIR_NORTH, DIR_SOUTH};

static void Task_DoPlayerSpinEntrance(u8 taskId)
{
    struct ObjectEvent *object = &gObjectEvents[gPlayerAvatar.objectEventId];
    struct Sprite *sprite = &gSprites[object->spriteId];
    s16 *data = gTasks[taskId].data;

    switch (tState)
    {
        case 0:
            // Because the spin start facing direction is never set for this
            // warp type, the player will always exit the warp facing South.
            // This may have been intentional, unclear
            tStartDir = GetSpinStartFacingDir();
            ObjectEventForceSetHeldMovement(object, GetFaceDirectionMovementAction(sSpinDirections[tStartDir]));
            tSpinDelayTimer = 0;
            tSpeed = 116;
            tDestY = sprite->y;
            tPriority = sprite->oam.priority;
            tSubpriority = sprite->subpriority;
            tCurY = -((u16)sprite->y2 + 32) * 16;
            sprite->y2 = 0;
            CameraObjectReset2();
            object->fixedPriority = TRUE;
            sprite->oam.priority = 1;
            sprite->subpriority = 0;
            sprite->subspriteMode = SUBSPRITES_OFF;
            tState++;
        case 1: // Spin while descending
            TrySpinPlayerForWarp(object, &tSpinDelayTimer);

            // Fall and decelerate
            tCurY += tSpeed;
            tSpeed -= 3;
            if (tSpeed < 4)
                tSpeed = 4;
            sprite->y = tCurY >> 4;

            // Check if reached dest
            if (sprite->y >= tDestY)
            {
                sprite->y = tDestY;
                tGroundTimer = 0;
                tState++;
            }
            break;
        case 2: // Spin on ground
            TrySpinPlayerForWarp(object, &tSpinDelayTimer);
            if (++tGroundTimer > 8)
                tState++;
            break;
        case 3: // Spin until facing original direction
            if (tStartDir == TrySpinPlayerForWarp(object, &tSpinDelayTimer))
            {
                object->fixedPriority = 0;
                sprite->oam.priority = tPriority;
                sprite->subpriority = tSubpriority;
                CameraObjectReset1();
                DestroyTask(taskId);
            }
            break;
    }
}

static u8 TrySpinPlayerForWarp(struct ObjectEvent *object, s16 *delayTimer)
{
    if (*delayTimer < 8 && ++(*delayTimer) < 8)
        return object->facingDirection;

    if (!ObjectEventCheckHeldMovementStatus(object))
        return object->facingDirection;

    ObjectEventForceSetHeldMovement(object, GetFaceDirectionMovementAction(sSpinDirections[object->facingDirection]));
    *delayTimer = 0;
    return sSpinDirections[object->facingDirection];
}
