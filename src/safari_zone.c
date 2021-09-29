#include "global.h"
#include "battle.h"
#include "event_data.h"
#include "event_scripts.h"
#include "overworld.h"
#include "main.h"
#include "safari_zone.h"
#include "script.h"
#include "string_util.h"
#include "field_screen_effect.h"

EWRAM_DATA u8 gNumSafariBalls = 0;
EWRAM_DATA static u16 sSafariZoneStepCounter = 0;

bool32 GetSafariZoneFlag(void)
{
    return FlagGet(FLAG_SYS_SAFARI_MODE);
}

void ResetSafariZoneFlag(void)
{
    FlagClear(FLAG_SYS_SAFARI_MODE);
}

void EnterSafariMode(void)
{
    IncrementGameStat(GAME_STAT_ENTERED_SAFARI_ZONE);
    FlagSet(FLAG_SYS_SAFARI_MODE);
    gNumSafariBalls = 30;
    sSafariZoneStepCounter = 600;
}

void ExitSafariMode(void)
{
    ResetSafariZoneFlag();
    gNumSafariBalls = 0;
    sSafariZoneStepCounter = 0;
}

bool8 SafariZoneTakeStep(void)
{
    if (!GetSafariZoneFlag())
        return FALSE;

    sSafariZoneStepCounter--;
    if (!sSafariZoneStepCounter)
    {
        ScriptContext1_SetupScript(SafariZone_EventScript_TimesUp);
        return TRUE;
    }
    return FALSE;
}

void SafariZoneRetirePrompt(void)
{
    ScriptContext1_SetupScript(SafariZone_EventScript_RetirePrompt);
}

void CB2_EndSafariBattle(void)
{
    if (gNumSafariBalls != 0)
        SetMainCallback2(CB2_ReturnToField);
    else if (gBattleOutcome == B_OUTCOME_NO_SAFARI_BALLS)
    {
        ScriptContext2_RunNewScript(SafariZone_EventScript_OutOfBallsMidBattle);
        WarpIntoMap();
        gFieldCallback = FieldCB_ReturnToFieldNoScriptCheckMusic;
        SetMainCallback2(CB2_LoadMap);
    }
    else if (gBattleOutcome == B_OUTCOME_CAUGHT)
    {
        ScriptContext1_SetupScript(SafariZone_EventScript_OutOfBalls);
        ScriptContext1_Stop();
        SetMainCallback2(CB2_ReturnToFieldContinueScriptPlayMapMusic);
    }
}
