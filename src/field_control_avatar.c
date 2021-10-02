#include "global.h"
#include "battle_setup.h"
#include "bike.h"
#include "coord_event_weather.h"
#include "daycare.h"
#include "faraway_island.h"
#include "event_data.h"
#include "event_object_movement.h"
#include "event_scripts.h"
#include "fieldmap.h"
#include "field_control_avatar.h"
#include "field_player_avatar.h"
#include "field_poison.h"
#include "field_screen_effect.h"
#include "field_specials.h"
#include "fldeff_misc.h"
#include "item_menu.h"
#include "link.h"
#include "metatile_behavior.h"
#include "overworld.h"
#include "pokemon.h"
#include "safari_zone.h"
#include "script.h"
#include "sound.h"
#include "start_menu.h"
#include "trainer_see.h"
#include "trainer_hill.h"
#include "wild_encounter.h"
#include "constants/event_bg.h"
#include "constants/event_objects.h"
#include "constants/field_poison.h"
#include "constants/map_types.h"
#include "constants/maps.h"
#include "constants/songs.h"
#include "constants/trainer_hill.h"

static EWRAM_DATA u8 sWildEncounterImmunitySteps = 0;
static EWRAM_DATA u16 sPreviousPlayerMetatileBehavior = 0;

u8 gSelectedObjectEvent;

static void GetPlayerPosition(struct MapPosition *);
static void GetInFrontOfPlayerPosition(struct MapPosition *);
static u16 GetPlayerCurMetatileBehavior(void);
static bool8 TryStartInteractionScript(struct MapPosition*, u16, u8);
static const u8 *GetInteractionScript(struct MapPosition*, u8, u8);
static const u8 *GetInteractedObjectEventScript(struct MapPosition *, u8, u8);
static const u8 *GetInteractedBackgroundEventScript(struct MapPosition *, u8, u8);
static const u8 *GetInteractedMetatileScript(struct MapPosition *, u8, u8);
static const u8 *GetInteractedWaterScript(struct MapPosition *, u8, u8);
static bool32 TrySetupDiveDownScript(void);
static bool32 TrySetupDiveEmergeScript(void);
static bool8 TryStartStepBasedScript(struct MapPosition *, u16, u16);
static bool8 CheckStandardWildEncounter(u32 currMetatileAttrs);
static bool8 TryArrowWarp(struct MapPosition *, u16, u8);
static bool8 IsWarpMetatileBehavior(u16);
static bool8 IsArrowWarpMetatileBehavior(u16, u8);
static s8 GetWarpEventAtMapPosition(struct MapHeader *, struct MapPosition *);
static void SetupWarp(struct MapHeader *, s8, struct MapPosition *);
static bool8 TryDoorWarp(struct MapPosition *, u16, u8);
static s8 GetWarpEventAtPosition(struct MapHeader *, u16, u16, u8);
static u8 *GetCoordEventScriptAtPosition(struct MapHeader *, u16, u16, u8);
static struct BgEvent *GetBackgroundEventAtPosition(struct MapHeader *, u16, u16, u8);
static bool8 TryStartCoordEventScript(struct MapPosition *);
static bool8 TryStartWarpEventScript(struct MapPosition *, u16);
static bool8 TryStartMiscWalkingScripts(u16);
static bool8 TryStartStepCountScript(u16);
static void UpdateFriendshipStepCounter(void);
static bool8 UpdatePoisonStepCounter(void);

void FieldClearPlayerInput(struct FieldInput *input)
{
    input->pressedAButton = FALSE;
    input->checkStandardWildEncounter = FALSE;
    input->pressedStartButton = FALSE;
    input->pressedSelectButton = FALSE;
    input->heldDirection = FALSE;
    input->heldDirection2 = FALSE;
    input->tookStep = FALSE;
    input->pressedBButton = FALSE;
    input->input_field_1_0 = FALSE;
    input->input_field_1_1 = FALSE;
    input->input_field_1_2 = FALSE;
    input->input_field_1_3 = FALSE;
    input->dpadDirection = 0;
}

void FieldGetPlayerInput(struct FieldInput *input, u16 newKeys, u16 heldKeys)
{
    u8 tileTransitionState = gPlayerAvatar.tileTransitionState;
    u8 runningState = gPlayerAvatar.runningState;
    bool8 forcedMove = MetatileBehavior_IsForcedMovementTile(GetPlayerCurMetatileBehavior());

    if ((tileTransitionState == T_TILE_CENTER && forcedMove == FALSE) || tileTransitionState == T_NOT_MOVING)
    {
        if (GetPlayerSpeed() != 4)
        {
            if (newKeys & START_BUTTON)
                input->pressedStartButton = TRUE;
            if (newKeys & SELECT_BUTTON)
                input->pressedSelectButton = TRUE;
            if (newKeys & A_BUTTON)
                input->pressedAButton = TRUE;
            if (newKeys & B_BUTTON)
                input->pressedBButton = TRUE;
        }

        if (heldKeys & (DPAD_UP | DPAD_DOWN | DPAD_LEFT | DPAD_RIGHT))
        {
            input->heldDirection = TRUE;
            input->heldDirection2 = TRUE;
        }
    }

    if (forcedMove == FALSE)
    {
        if (tileTransitionState == T_TILE_CENTER && runningState == MOVING)
            input->tookStep = TRUE;
        if (forcedMove == FALSE && tileTransitionState == T_TILE_CENTER)
            input->checkStandardWildEncounter = TRUE;
    }

    if (heldKeys & DPAD_UP)
        input->dpadDirection = DIR_NORTH;
    else if (heldKeys & DPAD_DOWN)
        input->dpadDirection = DIR_SOUTH;
    else if (heldKeys & DPAD_LEFT)
        input->dpadDirection = DIR_WEST;
    else if (heldKeys & DPAD_RIGHT)
        input->dpadDirection = DIR_EAST;
}

int ProcessPlayerFieldInput(struct FieldInput *input)
{
    struct MapPosition position;
    u8 playerDirection;
    u16 metatileBehavior;
    u32 metatileAttributes;

    gSpecialVar_LastTalked = 0;
    gSelectedObjectEvent = 0;
    gSpecialVar_TextColor = 0xFF;

    playerDirection = GetPlayerFacingDirection();
    GetPlayerPosition(&position);
    metatileBehavior = MapGridGetMetatileBehaviorAt(position.x, position.y);
    metatileAttributes = MapGridGetMetatileAttributeAt(position.x, position.y, 0xFF);

    if (CheckForTrainersWantingBattle()
     || TryRunOnFrameMapScript()
     || (input->pressedBButton && TrySetupDiveEmergeScript()))
        return TRUE;

    if (input->tookStep)
    {
        IncrementGameStat(GAME_STAT_STEPS);
        IncrementBirthIslandRockStepCount();
        if (TryStartStepBasedScript(&position, metatileBehavior, playerDirection))
            return TRUE;
    }
    if (input->checkStandardWildEncounter && CheckStandardWildEncounter(metatileAttributes))
        return TRUE;
    if (input->heldDirection && input->dpadDirection == playerDirection)
    {
        if (TryArrowWarp(&position, metatileBehavior, playerDirection))
            return TRUE;
    }

    GetInFrontOfPlayerPosition(&position);
    metatileBehavior = MapGridGetMetatileBehaviorAt(position.x, position.y);
    if (input->pressedAButton && TryStartInteractionScript(&position, metatileBehavior, playerDirection))
        return TRUE;

    if (input->heldDirection2 && input->dpadDirection == playerDirection)
    {
        if (TryDoorWarp(&position, metatileBehavior, playerDirection))
            return TRUE;
    }
    if (input->pressedAButton && TrySetupDiveDownScript())
        return TRUE;
    if (input->pressedStartButton)
    {
        PlaySE(SE_WIN_OPEN);
        ShowStartMenu();
        return TRUE;
    }
    if (input->pressedSelectButton && UseRegisteredKeyItemOnField())
        return TRUE;

    return FALSE;
}

static void GetPlayerPosition(struct MapPosition *position)
{
    PlayerGetDestCoords(&position->x, &position->y);
    position->height = PlayerGetZCoord();
}

static void GetInFrontOfPlayerPosition(struct MapPosition *position)
{
    s16 x, y;

    GetXYCoordsOneStepInFrontOfPlayer(&position->x, &position->y);
    PlayerGetDestCoords(&x, &y);
    if (MapGridGetZCoordAt(x, y) != 0)
        position->height = PlayerGetZCoord();
    else
        position->height = 0;
}

static u16 GetPlayerCurMetatileBehavior(void)
{
    s16 x, y;

    PlayerGetDestCoords(&x, &y);
    return MapGridGetMetatileBehaviorAt(x, y);
}

static bool8 TryStartInteractionScript(struct MapPosition *position, u16 metatileBehavior, u8 direction)
{
    const u8 *script = GetInteractionScript(position, metatileBehavior, direction);
    if (script == NULL)
        return FALSE;

    // Don't play interaction sound for certain scripts.
    if (script != LittlerootTown_BrendansHouse_2F_EventScript_PC
     && script != LittlerootTown_MaysHouse_2F_EventScript_PC
     && script != EventScript_PC)
        PlaySE(SE_SELECT);

    ScriptContext1_SetupScript(script);
    return TRUE;
}

static const u8 *GetInteractionScript(struct MapPosition *position, u8 metatileBehavior, u8 direction)
{
    const u8 *script = GetInteractedObjectEventScript(position, metatileBehavior, direction);
    if (script != NULL)
        return script;

    script = GetInteractedBackgroundEventScript(position, metatileBehavior, direction);
    if (script != NULL)
        return script;

    script = GetInteractedMetatileScript(position, metatileBehavior, direction);
    if (script != NULL)
        return script;

    script = GetInteractedWaterScript(position, metatileBehavior, direction);
    if (script != NULL)
        return script;

    return NULL;
}

const u8 *GetInteractedLinkPlayerScript(struct MapPosition *position, u8 metatileBehavior, u8 direction)
{
    u8 objectEventId;
    s32 i;

    if (!MetatileBehavior_IsCounter(MapGridGetMetatileBehaviorAt(position->x, position->y)))
        objectEventId = GetObjectEventIdByXYZ(position->x, position->y, position->height);
    else
        objectEventId = GetObjectEventIdByXYZ(position->x + gDirectionToVectors[direction].x, position->y + gDirectionToVectors[direction].y, position->height);

    if (objectEventId == OBJECT_EVENTS_COUNT || gObjectEvents[objectEventId].localId == OBJ_EVENT_ID_PLAYER)
        return NULL;

    for (i = 0; i < 4; i++)
    {
        if (gLinkPlayerObjectEvents[i].active && gLinkPlayerObjectEvents[i].objEventId == objectEventId)
            return NULL;
    }

    gSelectedObjectEvent = objectEventId;
    gSpecialVar_LastTalked = gObjectEvents[objectEventId].localId;
    gSpecialVar_Facing = direction;
    return GetObjectEventScriptPointerByObjectEventId(objectEventId);
}

static const u8 *GetInteractedObjectEventScript(struct MapPosition *position, u8 metatileBehavior, u8 direction)
{
    u8 objectEventId;
    const u8 *script;

    objectEventId = GetObjectEventIdByXYZ(position->x, position->y, position->height);
    if (objectEventId == OBJECT_EVENTS_COUNT || gObjectEvents[objectEventId].localId == OBJ_EVENT_ID_PLAYER)
    {
        if (MetatileBehavior_IsCounter(metatileBehavior) != TRUE)
            return NULL;

        // Look for an object event on the other side of the counter.
        objectEventId = GetObjectEventIdByXYZ(position->x + gDirectionToVectors[direction].x, position->y + gDirectionToVectors[direction].y, position->height);
        if (objectEventId == OBJECT_EVENTS_COUNT || gObjectEvents[objectEventId].localId == OBJ_EVENT_ID_PLAYER)
            return NULL;
    }

    gSelectedObjectEvent = objectEventId;
    gSpecialVar_LastTalked = gObjectEvents[objectEventId].localId;
    gSpecialVar_Facing = direction;

    if (InTrainerHill())
        script = GetTrainerHillTrainerScript();
    else
        script = GetObjectEventScriptPointerByObjectEventId(objectEventId);

    script = GetRamScript(gSpecialVar_LastTalked, script);
    return script;
}

static const u8 *GetInteractedBackgroundEventScript(struct MapPosition *position, u8 metatileBehavior, u8 direction)
{
    struct BgEvent *bgEvent = GetBackgroundEventAtPosition(&gMapHeader, position->x - 7, position->y - 7, position->height);

    if (bgEvent == NULL)
        return NULL;
    if (bgEvent->bgUnion.script == NULL)
        return EventScript_TestSignpostMsg;

    switch (bgEvent->kind)
    {
    case BG_EVENT_PLAYER_FACING_ANY:
    default:
        return bgEvent->bgUnion.script;
    case BG_EVENT_PLAYER_FACING_NORTH:
        if (direction != DIR_NORTH)
            return NULL;
        break;
    case BG_EVENT_PLAYER_FACING_SOUTH:
        if (direction != DIR_SOUTH)
            return NULL;
        break;
    case BG_EVENT_PLAYER_FACING_EAST:
        if (direction != DIR_EAST)
            return NULL;
        break;
    case BG_EVENT_PLAYER_FACING_WEST:
        if (direction != DIR_WEST)
            return NULL;
        break;
    case 5:
    case 6:
    case BG_EVENT_HIDDEN_ITEM:
        gSpecialVar_0x8004 = ((u32)bgEvent->bgUnion.script >> 16) + FLAG_HIDDEN_ITEMS_START;
        gSpecialVar_0x8005 = (u32)bgEvent->bgUnion.script;
        if (FlagGet(gSpecialVar_0x8004))
            return NULL;
        return EventScript_HiddenItemScript;
    }

    return bgEvent->bgUnion.script;
}

static const u8 *GetInteractedMetatileScript(struct MapPosition *position, u8 metatileBehavior, u8 direction)
{
    s8 height;

    if (MetatileBehavior_IsPlayerFacingTVScreen(metatileBehavior, direction))
        return EventScript_TV;
    if (MetatileBehavior_IsPC(metatileBehavior))
        return EventScript_PC;
    if (MetatileBehavior_IsClosedSootopolisDoor(metatileBehavior))
        return EventScript_ClosedSootopolisDoor;
    if (MetatileBehavior_IsSkyPillarClosedDoor(metatileBehavior))
        return SkyPillar_Outside_EventScript_ClosedDoor;
    if (MetatileBehavior_IsCableBoxResults1(metatileBehavior))
        return EventScript_CableBoxResults;
    if (MetatileBehavior_IsTrickHousePuzzleDoor(metatileBehavior))
        return Route110_TrickHousePuzzle_EventScript_Door;
    if (MetatileBehavior_IsRegionMap(metatileBehavior))
        return EventScript_RegionMap;
    if (MetatileBehavior_IsRunningShoesManual(metatileBehavior))
        return EventScript_RunningShoesManual;
    if (MetatileBehavior_IsPictureBookShelf(metatileBehavior))
        return EventScript_PictureBookShelf;
    if (MetatileBehavior_IsBookShelf(metatileBehavior))
        return EventScript_BookShelf;
    if (MetatileBehavior_IsPokeCenterBookShelf(metatileBehavior))
        return EventScript_PokemonCenterBookShelf;
    if (MetatileBehavior_IsVase(metatileBehavior))
        return EventScript_Vase;
    if (MetatileBehavior_IsTrashCan(metatileBehavior))
        return EventScript_EmptyTrashCan;
    if (MetatileBehavior_IsShopShelf(metatileBehavior))
        return EventScript_ShopShelf;
    if (MetatileBehavior_IsBlueprint(metatileBehavior))
        return EventScript_Blueprint;
    if (MetatileBehavior_IsPlayerFacingWirelessBoxResults(metatileBehavior, direction))
        return EventScript_WirelessBoxResults;
    if (MetatileBehavior_IsCableBoxResults2(metatileBehavior, direction))
        return EventScript_CableBoxResults;
    if (MetatileBehavior_IsQuestionnaire(metatileBehavior))
        return EventScript_Questionnaire;
    if (MetatileBehavior_IsTrainerHillTimer(metatileBehavior))
        return EventScript_TrainerHillTimer;

    height = position->height;
    return NULL;
}

static const u8 *GetInteractedWaterScript(struct MapPosition *unused1, u8 metatileBehavior, u8 direction)
{
    if (FlagGet(FLAG_BADGE05_GET) && PartyHasMonWithSurf() && IsPlayerFacingSurfableFishableWater())
        return EventScript_UseSurf;

    if (MetatileBehavior_IsWaterfall(metatileBehavior))
    {
        if (FlagGet(FLAG_BADGE08_GET) && IsPlayerSurfingNorth())
            return EventScript_UseWaterfall;
        else
            return EventScript_CannotUseWaterfall;
    }
    return NULL;
}

static bool32 TrySetupDiveDownScript(void)
{
    if (FlagGet(FLAG_BADGE07_GET) && TrySetDiveWarp() == 2)
    {
        ScriptContext1_SetupScript(EventScript_UseDive);
        return TRUE;
    }
    return FALSE;
}

static bool32 TrySetupDiveEmergeScript(void)
{
    if (FlagGet(FLAG_BADGE07_GET) && gMapHeader.mapType == MAP_TYPE_UNDERWATER && TrySetDiveWarp() == 1)
    {
        ScriptContext1_SetupScript(EventScript_UseDiveUnderwater);
        return TRUE;
    }
    return FALSE;
}

static bool8 TryStartStepBasedScript(struct MapPosition *position, u16 metatileBehavior, u16 direction)
{
    if (TryStartCoordEventScript(position)
     || TryStartWarpEventScript(position, metatileBehavior)
     || TryStartMiscWalkingScripts(metatileBehavior)
     || TryStartStepCountScript(metatileBehavior)
     || UpdateRepelCounter())
        return TRUE;
    return FALSE;
}

static bool8 TryStartCoordEventScript(struct MapPosition *position)
{
    u8 *script = GetCoordEventScriptAtPosition(&gMapHeader, position->x - 7, position->y - 7, position->height);

    if (script == NULL)
        return FALSE;
    ScriptContext1_SetupScript(script);
    return TRUE;
}

static bool8 TryStartMiscWalkingScripts(u16 metatileBehavior)
{
    s16 x, y;

    if (MetatileBehavior_IsCrackedFloorHole(metatileBehavior))
    {
        ScriptContext1_SetupScript(EventScript_FallDownHole);
        return TRUE;
    }
    else if (MetatileBehavior_IsBattlePyramidWarp(metatileBehavior))
    {
        ScriptContext1_SetupScript(BattlePyramid_WarpToNextFloor);
        return TRUE;
    }
    return FALSE;
}

static bool8 TryStartStepCountScript(u16 metatileBehavior)
{
    if (InUnionRoom())
        return FALSE;

    UpdateFriendshipStepCounter();
    UpdateFarawayIslandStepCounter();

    if (!(gPlayerAvatar.flags & PLAYER_AVATAR_FLAG_FORCED_MOVE) && !MetatileBehavior_IsForcedMovementTile(metatileBehavior))
    {
        if (UpdatePoisonStepCounter())
        {
            ScriptContext1_SetupScript(EventScript_FieldPoison);
            return TRUE;
        }
        if (ShouldEggHatch())
        {
            IncrementGameStat(GAME_STAT_HATCHED_EGGS);
            ScriptContext1_SetupScript(EventScript_EggHatch);
            return TRUE;
        }
    }

    if (SafariZoneTakeStep())
        return TRUE;
    return FALSE;
}

static void UpdateFriendshipStepCounter(void)
{
    u16 *ptr = GetVarPointer(VAR_FRIENDSHIP_STEP_COUNTER);
    int i;

    (*ptr)++;
    (*ptr) %= 128;
    if (*ptr == 0)
    {
        struct Pokemon *mon = gPlayerParty;
        for (i = 0; i < PARTY_SIZE; i++)
        {
            AdjustFriendship(mon, FRIENDSHIP_EVENT_WALKING);
            mon++;
        }
    }
}

void ClearPoisonStepCounter(void)
{
    VarSet(VAR_POISON_STEP_COUNTER, 0);
}

static bool8 UpdatePoisonStepCounter(void)
{
    u16 *ptr;

    ptr = GetVarPointer(VAR_POISON_STEP_COUNTER);
    (*ptr)++;
    (*ptr) %= 4;
    if (*ptr == 0)
    {
        switch (DoPoisonFieldEffect())
        {
        case FLDPSN_NONE:
        case FLDPSN_PSN:
            return FALSE;
        case FLDPSN_FNT:
            return TRUE;
        }
    }
    return FALSE;
}

void RestartWildEncounterImmunitySteps(void)
{
    // Starts at 0 and counts up to 4 steps.
    sWildEncounterImmunitySteps = 0;
}

static bool8 CheckStandardWildEncounter(u32 currMetatileAttrs)
{
    if (sWildEncounterImmunitySteps < 4)
    {
        sWildEncounterImmunitySteps++;
        sPreviousPlayerMetatileBehavior = ExtractMetatileAttribute(currMetatileAttrs, METATILE_ATTRIBUTE_BEHAVIOR);
        return FALSE;
    }

    if (StandardWildEncounter(currMetatileAttrs, sPreviousPlayerMetatileBehavior))
    {
        sWildEncounterImmunitySteps = 0;
        sPreviousPlayerMetatileBehavior = ExtractMetatileAttribute(currMetatileAttrs, METATILE_ATTRIBUTE_BEHAVIOR);
        return TRUE;
    }

    sPreviousPlayerMetatileBehavior = ExtractMetatileAttribute(currMetatileAttrs, METATILE_ATTRIBUTE_BEHAVIOR);
    return FALSE;
}

static bool8 TryArrowWarp(struct MapPosition *position, u16 metatileBehavior, u8 direction)
{
    s8 warpEventId = GetWarpEventAtMapPosition(&gMapHeader, position);

    if (warpEventId != -1)
    {
        if (IsArrowWarpMetatileBehavior(metatileBehavior, direction))
        {
            StoreInitialPlayerAvatarState();
            SetupWarp(&gMapHeader, warpEventId, position);
            DoWarp();
            return TRUE;
        }
        else if (IsDirectionalStairWarpMetatileBehavior(metatileBehavior, direction))
        {
            u16 delay = 0;

            if (gPlayerAvatar.flags & (PLAYER_AVATAR_FLAG_BIKE))
            {
                SetPlayerAvatarTransitionFlags(PLAYER_AVATAR_FLAG_ON_FOOT);
                delay = 12;
            }
            StoreInitialPlayerAvatarState();
            SetupWarp(&gMapHeader, warpEventId, position);
            DoStairWarp(metatileBehavior, delay);
            return TRUE;
        }
    }
    return FALSE;
}

static bool8 TryStartWarpEventScript(struct MapPosition *position, u16 metatileBehavior)
{
    s8 warpEventId = GetWarpEventAtMapPosition(&gMapHeader, position);

    if (warpEventId != -1 && IsWarpMetatileBehavior(metatileBehavior))
    {
        StoreInitialPlayerAvatarState();
        SetupWarp(&gMapHeader, warpEventId, position);
        if (MetatileBehavior_IsEscalator(metatileBehavior))
        {
            DoEscalatorWarp(metatileBehavior);
            return TRUE;
        }
        if (MetatileBehavior_IsLavaridgeB1FWarp(metatileBehavior))
        {
            DoLavaridgeGymB1FWarp();
            return TRUE;
        }
        if (MetatileBehavior_IsLavaridge1FWarp(metatileBehavior))
        {
            DoLavaridgeGym1FWarp();
            return TRUE;
        }
        if (MetatileBehavior_IsAquaHideoutWarp(metatileBehavior))
        {
            DoTeleportTileWarp();
            return TRUE;
        }
        if (MetatileBehavior_IsWarpOrBridge(metatileBehavior))
        {
            // Maybe unused? This MB is used by log bridges, but there's never a warp event on them
            DoSpinExitWarp();
            return TRUE;
        }
        if (MetatileBehavior_IsMtPyreHole(metatileBehavior))
        {
            ScriptContext1_SetupScript(EventScript_FallDownHoleMtPyre);
            return TRUE;
        }
        if (MetatileBehavior_IsMossdeepGymWarp(metatileBehavior))
        {
            DoMossdeepGymWarp();
            return TRUE;
        }
        DoWarp();
        return TRUE;
    }
    return FALSE;
}

static bool8 IsWarpMetatileBehavior(u16 metatileBehavior)
{
    return MetatileBehavior_IsWarpDoor(metatileBehavior)
        || MetatileBehavior_IsLadder(metatileBehavior)
        || MetatileBehavior_IsEscalator(metatileBehavior)
        || MetatileBehavior_IsNonAnimDoor(metatileBehavior)
        || MetatileBehavior_IsLavaridgeB1FWarp(metatileBehavior)
        || MetatileBehavior_IsLavaridge1FWarp(metatileBehavior)
        || MetatileBehavior_IsAquaHideoutWarp(metatileBehavior)
        || MetatileBehavior_IsMtPyreHole(metatileBehavior)
        || MetatileBehavior_IsMossdeepGymWarp(metatileBehavior)
        || MetatileBehavior_IsWarpOrBridge(metatileBehavior);
}

bool8 IsDirectionalStairWarpMetatileBehavior(u16 metatileBehavior, u8 playerDirection)
{
    switch (playerDirection)
    {
    case DIR_WEST:
            return MetatileBehavior_IsDirectionalUpLeftStairWarp(metatileBehavior)
                || MetatileBehavior_IsDirectionalDownLeftStairWarp(metatileBehavior);
        break;
    case DIR_EAST:
            return MetatileBehavior_IsDirectionalUpRightStairWarp(metatileBehavior)
                || MetatileBehavior_IsDirectionalDownRightStairWarp(metatileBehavior);
        break;
    }
    return FALSE;
}

static bool8 IsArrowWarpMetatileBehavior(u16 metatileBehavior, u8 direction)
{
    switch (direction)
    {
    case DIR_NORTH:
        return MetatileBehavior_IsNorthArrowWarp(metatileBehavior);
    case DIR_SOUTH:
        return MetatileBehavior_IsSouthArrowWarp(metatileBehavior);
    case DIR_WEST:
        return MetatileBehavior_IsWestArrowWarp(metatileBehavior);
    case DIR_EAST:
        return MetatileBehavior_IsEastArrowWarp(metatileBehavior);
    }
    return FALSE;
}

static s8 GetWarpEventAtMapPosition(struct MapHeader *mapHeader, struct MapPosition *position)
{
    return GetWarpEventAtPosition(mapHeader, position->x - 7, position->y - 7, position->height);
}

static void SetupWarp(struct MapHeader *unused, s8 warpEventId, struct MapPosition *position)
{
    const struct WarpEvent *warpEvent;
    u8 trainerHillMapId = GetCurrentTrainerHillMapId();

    if (trainerHillMapId)
    {
        if (trainerHillMapId == GetNumFloorsInTrainerHillChallenge())
        {
            if (warpEventId)
                warpEvent = SetWarpDestinationTrainerHill4F();
            else
                warpEvent = &gMapHeader.events->warps[0];
        }
        else if (trainerHillMapId == TRAINER_HILL_ROOF)
            warpEvent = SetWarpDestinationTrainerHillFinalFloor(warpEventId);
        else
            warpEvent = &gMapHeader.events->warps[warpEventId];
    }
    else
        warpEvent = &gMapHeader.events->warps[warpEventId];

    if (warpEvent->mapNum == MAP_NUM(NONE))
        SetWarpDestinationToDynamicWarp(warpEvent->warpId);
    else
    {
        const struct MapHeader *mapHeader;

        SetWarpDestinationToMapWarp(warpEvent->mapGroup, warpEvent->mapNum, warpEvent->warpId);
        UpdateEscapeWarp(position->x, position->y);
        mapHeader = Overworld_GetMapHeaderByGroupAndId(warpEvent->mapGroup, warpEvent->mapNum);
        if (mapHeader->events->warps[warpEvent->warpId].mapNum == MAP_NUM(NONE))
            SetDynamicWarp(gSaveBlock1Ptr->location.mapGroup, gSaveBlock1Ptr->location.mapNum, warpEventId);
    }
}

static bool8 TryDoorWarp(struct MapPosition *position, u16 metatileBehavior, u8 direction)
{
    s8 warpEventId;

    if (direction == DIR_NORTH)
    {
        if (MetatileBehavior_IsWarpDoor(metatileBehavior))
        {
            warpEventId = GetWarpEventAtMapPosition(&gMapHeader, position);
            if (warpEventId != -1 && IsWarpMetatileBehavior(metatileBehavior))
            {
                StoreInitialPlayerAvatarState();
                SetupWarp(&gMapHeader, warpEventId, position);
                DoDoorWarp();
                return TRUE;
            }
        }
    }
    return FALSE;
}

static s8 GetWarpEventAtPosition(struct MapHeader *mapHeader, u16 x, u16 y, u8 elevation)
{
    s32 i;
    struct WarpEvent *warpEvent = mapHeader->events->warps;
    u8 warpCount = mapHeader->events->warpCount;

    for (i = 0; i < warpCount; i++, warpEvent++)
    {
        if ((u16)warpEvent->x == x && (u16)warpEvent->y == y)
        {
            if (warpEvent->elevation == elevation || warpEvent->elevation == 0)
                return i;
        }
    }
    return -1;
}

static u8 *TryRunCoordEventScript(struct CoordEvent *coordEvent)
{
    if (coordEvent != NULL)
    {
        if (coordEvent->script == NULL)
        {
            DoCoordEventWeather(coordEvent->trigger);
            return NULL;
        }
        if (coordEvent->trigger == 0)
        {
            ScriptContext2_RunNewScript(coordEvent->script);
            return NULL;
        }
        if (VarGet(coordEvent->trigger) == (u8)coordEvent->index)
            return coordEvent->script;
    }
    return NULL;
}

static u8 *GetCoordEventScriptAtPosition(struct MapHeader *mapHeader, u16 x, u16 y, u8 elevation)
{
    s32 i;
    struct CoordEvent *coordEvents = mapHeader->events->coordEvents;
    u8 coordEventCount = mapHeader->events->coordEventCount;

    for (i = 0; i < coordEventCount; i++)
    {
        if ((u16)coordEvents[i].x == x && (u16)coordEvents[i].y == y)
        {
            if (coordEvents[i].elevation == elevation || coordEvents[i].elevation == 0)
            {
                u8 *script = TryRunCoordEventScript(&coordEvents[i]);
                if (script != NULL)
                    return script;
            }
        }
    }
    return NULL;
}

u8 *GetCoordEventScriptAtMapPosition(struct MapPosition *position)
{
    return GetCoordEventScriptAtPosition(&gMapHeader, position->x - 7, position->y - 7, position->height);
}

static struct BgEvent *GetBackgroundEventAtPosition(struct MapHeader *mapHeader, u16 x, u16 y, u8 elevation)
{
    u32 i;
    struct BgEvent *bgEvents = mapHeader->events->bgEvents;
    u8 bgEventCount = mapHeader->events->bgEventCount;

    for (i = 0; i < bgEventCount; i++)
    {
        if ((u16)bgEvents[i].x == x && (u16)bgEvents[i].y == y)
        {
            if (bgEvents[i].elevation == elevation || bgEvents[i].elevation == 0)
                return &bgEvents[i];
        }
    }
    return NULL;
}

bool8 TryDoDiveWarp(struct MapPosition *position, u16 metatileBehavior)
{
    if (gMapHeader.mapType == MAP_TYPE_UNDERWATER && !MetatileBehavior_IsUnableToEmerge(metatileBehavior))
    {
        if (SetDiveWarpEmerge(position->x - 7, position->y - 7))
        {
            StoreInitialPlayerAvatarState();
            DoDiveWarp();
            PlaySE(SE_M_DIVE);
            return TRUE;
        }
    }
    else if (MetatileBehavior_IsDiveable(metatileBehavior))
    {
        if (SetDiveWarpDive(position->x - 7, position->y - 7))
        {
            StoreInitialPlayerAvatarState();
            DoDiveWarp();
            PlaySE(SE_M_DIVE);
            return TRUE;
        }
    }
    return FALSE;
}

u8 TrySetDiveWarp(void)
{
    s16 x, y;
    u8 metatileBehavior;

    PlayerGetDestCoords(&x, &y);
    metatileBehavior = MapGridGetMetatileBehaviorAt(x, y);
    if (gMapHeader.mapType == MAP_TYPE_UNDERWATER && !MetatileBehavior_IsUnableToEmerge(metatileBehavior))
    {
        if (SetDiveWarpEmerge(x - 7, y - 7))
            return 1;
    }
    else if (MetatileBehavior_IsDiveable(metatileBehavior))
    {
        if (SetDiveWarpDive(x - 7, y - 7))
            return 2;
    }
    return 0;
}

const u8 *GetObjectEventScriptPointerPlayerFacing(void)
{
    u8 direction;
    struct MapPosition position;

    direction = GetPlayerMovementDirection();
    GetInFrontOfPlayerPosition(&position);
    return GetInteractedObjectEventScript(&position, MapGridGetMetatileBehaviorAt(position.x, position.y), direction);
}

int SetCableClubWarp(void)
{
    struct MapPosition position;

    GetPlayerPosition(&position);
    SetupWarp(&gMapHeader, GetWarpEventAtMapPosition(&gMapHeader, &position), &position);
    return 0;
}
