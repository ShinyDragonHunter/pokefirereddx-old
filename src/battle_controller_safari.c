#include "global.h"
#include "battle.h"
#include "battle_anim.h"
#include "battle_controllers.h"
#include "battle_interface.h"
#include "battle_message.h"
#include "bg.h"
#include "data.h"
#include "item_menu.h"
#include "link.h"
#include "main.h"
#include "m4a.h"
#include "palette.h"
#include "pokeball.h"
#include "pokemon.h"
#include "reshow_battle_screen.h"
#include "sound.h"
#include "task.h"
#include "text.h"
#include "util.h"
#include "window.h"
#include "constants/battle_anim.h"
#include "constants/songs.h"
#include "constants/rgb.h"

// this file's functions
static void SafariEndExecution(void);
static void SafariHandleDrawTrainerPic(void);
static void SafariHandleSuccessBallThrowAnim(void);
static void SafariHandleBallThrowAnim(void);
static void SafariHandlePrintString(void);
static void SafariHandlePrintSelectionString(void);
static void SafariHandleChooseAction(void);
static void SafariHandleStatusIconUpdate(void);
static void SafariHandlePlaySE(void);
static void SafariHandlePlayFanfareOrBGM(void);
static void SafariHandleFaintingCry(void);
static void SafariHandleIntroSlide(void);
static void SafariHandleIntroTrainerBallThrow(void);
static void SafariHandleBattleAnimation(void);
static void SafariHandleEndLinkBattle(void);

static void SafariBufferRunCommand(void);
static void SafariBufferExecCompleted(void);

static void (*const sSafariBufferCommands[CONTROLLER_CMDS_COUNT])(void) =
{
    [CONTROLLER_GETMONDATA]               = SafariEndExecution,
    [CONTROLLER_GETRAWMONDATA]            = SafariEndExecution,
    [CONTROLLER_SETMONDATA]               = SafariEndExecution,
    [CONTROLLER_SETRAWMONDATA]            = SafariEndExecution,
    [CONTROLLER_LOADMONSPRITE]            = SafariEndExecution,
    [CONTROLLER_SWITCHINANIM]             = SafariEndExecution,
    [CONTROLLER_RETURNMONTOBALL]          = SafariEndExecution,
    [CONTROLLER_DRAWTRAINERPIC]           = SafariHandleDrawTrainerPic,
    [CONTROLLER_TRAINERSLIDE]             = SafariEndExecution,
    [CONTROLLER_TRAINERSLIDEBACK]         = SafariEndExecution,
    [CONTROLLER_FAINTANIMATION]           = SafariEndExecution,
    [CONTROLLER_PALETTEFADE]              = SafariEndExecution,
    [CONTROLLER_SUCCESSBALLTHROWANIM]     = SafariHandleSuccessBallThrowAnim,
    [CONTROLLER_BALLTHROWANIM]            = SafariHandleBallThrowAnim,
    [CONTROLLER_PAUSE]                    = SafariEndExecution,
    [CONTROLLER_MOVEANIMATION]            = SafariEndExecution,
    [CONTROLLER_PRINTSTRING]              = SafariHandlePrintString,
    [CONTROLLER_PRINTSTRINGPLAYERONLY]    = SafariHandlePrintSelectionString,
    [CONTROLLER_CHOOSEACTION]             = SafariHandleChooseAction,
    [CONTROLLER_YESNOBOX]                 = SafariEndExecution,
    [CONTROLLER_CHOOSEMOVE]               = SafariEndExecution,
    [CONTROLLER_OPENBAG]                  = SafariEndExecution,
    [CONTROLLER_CHOOSEPOKEMON]            = SafariEndExecution,
    [CONTROLLER_23]                       = SafariEndExecution,
    [CONTROLLER_HEALTHBARUPDATE]          = SafariEndExecution,
    [CONTROLLER_EXPUPDATE]                = SafariEndExecution,
    [CONTROLLER_STATUSICONUPDATE]         = SafariHandleStatusIconUpdate,
    [CONTROLLER_STATUSANIMATION]          = SafariEndExecution,
    [CONTROLLER_STATUSXOR]                = SafariEndExecution,
    [CONTROLLER_DATATRANSFER]             = SafariEndExecution,
    [CONTROLLER_DMA3TRANSFER]             = SafariEndExecution,
    [CONTROLLER_PLAYBGM]                  = SafariEndExecution,
    [CONTROLLER_32]                       = SafariEndExecution,
    [CONTROLLER_TWORETURNVALUES]          = SafariEndExecution,
    [CONTROLLER_CHOSENMONRETURNVALUE]     = SafariEndExecution,
    [CONTROLLER_ONERETURNVALUE]           = SafariEndExecution,
    [CONTROLLER_ONERETURNVALUE_DUPLICATE] = SafariEndExecution,
    [CONTROLLER_CLEARUNKVAR]              = SafariEndExecution,
    [CONTROLLER_SETUNKVAR]                = SafariEndExecution,
    [CONTROLLER_CLEARUNKFLAG]             = SafariEndExecution,
    [CONTROLLER_TOGGLEUNKFLAG]            = SafariEndExecution,
    [CONTROLLER_HITANIMATION]             = SafariEndExecution,
    [CONTROLLER_CANTSWITCH]               = SafariEndExecution,
    [CONTROLLER_PLAYSE]                   = SafariHandlePlaySE,
    [CONTROLLER_PLAYFANFAREORBGM]         = SafariHandlePlayFanfareOrBGM,
    [CONTROLLER_FAINTINGCRY]              = SafariHandleFaintingCry,
    [CONTROLLER_INTROSLIDE]               = SafariHandleIntroSlide,
    [CONTROLLER_INTROTRAINERBALLTHROW]    = SafariHandleIntroTrainerBallThrow,
    [CONTROLLER_DRAWPARTYSTATUSSUMMARY]   = SafariEndExecution,
    [CONTROLLER_HIDEPARTYSTATUSSUMMARY]   = SafariEndExecution,
    [CONTROLLER_ENDBOUNCE]                = SafariEndExecution,
    [CONTROLLER_SPRITEINVISIBILITY]       = SafariEndExecution,
    [CONTROLLER_BATTLEANIMATION]          = SafariHandleBattleAnimation,
    [CONTROLLER_LINKSTANDBYMSG]           = SafariEndExecution,
    [CONTROLLER_RESETACTIONMOVESELECTION] = SafariEndExecution,
    [CONTROLLER_ENDLINKBATTLE]            = SafariHandleEndLinkBattle,
    [CONTROLLER_DEBUGMENU]                = SafariEndExecution,
    [CONTROLLER_TERMINATOR_NOP]           = BattleControllerDummy
};

void SetControllerToSafari(void)
{
    gBattlerControllerFuncs[gActiveBattler] = SafariBufferRunCommand;
}

static void SafariBufferRunCommand(void)
{
    if (gBattleControllerExecFlags & gBitTable[gActiveBattler])
    {
        if (gBattleBufferA[gActiveBattler][0] < ARRAY_COUNT(sSafariBufferCommands))
            sSafariBufferCommands[gBattleBufferA[gActiveBattler][0]]();
        else
            SafariBufferExecCompleted();
    }
}

static void HandleInputChooseAction(void)
{
    if (JOY_NEW(A_BUTTON))
    {
        PlaySE(SE_SELECT);

        switch (gActionSelectionCursor[gActiveBattler])
        {
        case 0:
        case 1:
        case 2:
        case 3:
            BtlController_EmitTwoReturnValues(1, gActionSelectionCursor[gActiveBattler] + B_ACTION_SAFARI_BALL, 0);
            break;
        }
        SafariBufferExecCompleted();
    }
    else if (JOY_NEW(DPAD_LEFT))
    {
        if (gActionSelectionCursor[gActiveBattler] & 1)
        {
            PlaySE(SE_SELECT);
            ActionSelectionDestroyCursorAt(gActionSelectionCursor[gActiveBattler]);
            gActionSelectionCursor[gActiveBattler] ^= 1;
            ActionSelectionCreateCursorAt(gActionSelectionCursor[gActiveBattler], 0);
        }
    }
    else if (JOY_NEW(DPAD_RIGHT))
    {
        if (!(gActionSelectionCursor[gActiveBattler] & 1))
        {
            PlaySE(SE_SELECT);
            ActionSelectionDestroyCursorAt(gActionSelectionCursor[gActiveBattler]);
            gActionSelectionCursor[gActiveBattler] ^= 1;
            ActionSelectionCreateCursorAt(gActionSelectionCursor[gActiveBattler], 0);
        }
    }
    else if (JOY_NEW(DPAD_UP))
    {
        if (gActionSelectionCursor[gActiveBattler] & 2)
        {
            PlaySE(SE_SELECT);
            ActionSelectionDestroyCursorAt(gActionSelectionCursor[gActiveBattler]);
            gActionSelectionCursor[gActiveBattler] ^= 2;
            ActionSelectionCreateCursorAt(gActionSelectionCursor[gActiveBattler], 0);
        }
    }
    else if (JOY_NEW(DPAD_DOWN))
    {
        if (!(gActionSelectionCursor[gActiveBattler] & 2))
        {
            PlaySE(SE_SELECT);
            ActionSelectionDestroyCursorAt(gActionSelectionCursor[gActiveBattler]);
            gActionSelectionCursor[gActiveBattler] ^= 2;
            ActionSelectionCreateCursorAt(gActionSelectionCursor[gActiveBattler], 0);
        }
    }
}

static void CompleteOnBattlerSpriteCallbackDummy(void)
{
    if (gSprites[gBattlerSpriteIds[gActiveBattler]].callback == SpriteCallbackDummy)
        SafariBufferExecCompleted();
}

static void CompleteOnInactiveTextPrinter(void)
{
    if (!IsTextPrinterActive(0))
        SafariBufferExecCompleted();
}

static void CompleteOnHealthboxSpriteCallbackDummy(void)
{
    if (gSprites[gHealthboxSpriteIds[gActiveBattler]].callback == SpriteCallbackDummy)
        SafariBufferExecCompleted();
}

static void SafariSetBattleEndCallbacks(void)
{
    if (!gPaletteFade.active)
    {
        gMain.inBattle = FALSE;
        gMain.callback1 = gPreBattleCallback1;
        SetMainCallback2(gMain.savedCallback);
    }
}

static void CompleteOnSpecialAnimDone(void)
{
    if (!gDoingBattleAnim || !gBattleSpritesDataPtr->healthBoxesData[gActiveBattler].specialAnimActive)
        SafariBufferExecCompleted();
}

static void CompleteOnFinishedBattleAnimation(void)
{
    if (!gBattleSpritesDataPtr->healthBoxesData[gActiveBattler].animFromTableActive)
        SafariBufferExecCompleted();
}

static void SafariBufferExecCompleted(void)
{
    gBattlerControllerFuncs[gActiveBattler] = SafariBufferRunCommand;
    if (gBattleTypeFlags & BATTLE_TYPE_LINK)
    {
        u8 playerId = GetMultiplayerId();

        PrepareBufferDataTransferLink(2, 4, &playerId);
        gBattleBufferA[gActiveBattler][0] = CONTROLLER_TERMINATOR_NOP;
    }
    else
    {
        gBattleControllerExecFlags &= ~gBitTable[gActiveBattler];
    }
}

static void CompleteOnFinishedStatusAnimation(void)
{
    if (!gBattleSpritesDataPtr->healthBoxesData[gActiveBattler].statusAnimActive)
        SafariBufferExecCompleted();
}

static void SafariEndExecution(void)
{
    SafariBufferExecCompleted();
}

#define sSpeedX data[0]

static void SafariHandleDrawTrainerPic(void)
{
    LoadPalette(gTrainerBackPicPaletteTable[gSaveBlock2Ptr->playerGender].data, 0x100 + 16 * gActiveBattler, 32);
    SetMultiuseSpriteTemplateToTrainerBack(gSaveBlock2Ptr->playerGender, GetBattlerPosition(gActiveBattler));
    gBattlerSpriteIds[gActiveBattler] = CreateSprite(&gMultiuseSpriteTemplate,
    80,
    80,
    30);
    gSprites[gBattlerSpriteIds[gActiveBattler]].oam.paletteNum = gActiveBattler;
    gSprites[gBattlerSpriteIds[gActiveBattler]].x2 = DISPLAY_WIDTH;
    gSprites[gBattlerSpriteIds[gActiveBattler]].sSpeedX = -2;
    gSprites[gBattlerSpriteIds[gActiveBattler]].callback = SpriteCB_TrainerSlideIn;
    gBattlerControllerFuncs[gActiveBattler] = CompleteOnBattlerSpriteCallbackDummy;
}

#undef sSpeedX

static void SafariHandleSuccessBallThrowAnim(void)
{
    gBattleSpritesDataPtr->animationData->ballThrowCaseId = BALL_3_SHAKES_SUCCESS;
    gDoingBattleAnim = TRUE;
    InitAndLaunchSpecialAnimation(gActiveBattler, gActiveBattler, GetBattlerAtPosition(B_POSITION_OPPONENT_LEFT), B_ANIM_BALL_THROW_WITH_TRAINER);
    gBattlerControllerFuncs[gActiveBattler] = CompleteOnSpecialAnimDone;
}

static void SafariHandleBallThrowAnim(void)
{
    u8 ballThrowCaseId = gBattleBufferA[gActiveBattler][1];

    gBattleSpritesDataPtr->animationData->ballThrowCaseId = ballThrowCaseId;
    gDoingBattleAnim = TRUE;
    InitAndLaunchSpecialAnimation(gActiveBattler, gActiveBattler, GetBattlerAtPosition(B_POSITION_OPPONENT_LEFT), B_ANIM_BALL_THROW_WITH_TRAINER);
    gBattlerControllerFuncs[gActiveBattler] = CompleteOnSpecialAnimDone;
}

static void SafariHandlePrintString(void)
{
    u16 *stringId;

    gBattle_BG0_X = 0;
    gBattle_BG0_Y = 0;
    stringId = (u16*)(&gBattleBufferA[gActiveBattler][2]);
    BufferStringBattle(*stringId);
    BattlePutTextOnWindow(gDisplayedStringBattle, 0);
    gBattlerControllerFuncs[gActiveBattler] = CompleteOnInactiveTextPrinter;
}

static void SafariHandlePrintSelectionString(void)
{
    if (GetBattlerSide(gActiveBattler) == B_SIDE_PLAYER)
        SafariHandlePrintString();
    else
        SafariBufferExecCompleted();
}

static void HandleChooseActionAfterDma3(void)
{
    if (!IsDma3ManagerBusyWithBgCopy())
    {
        gBattle_BG0_X = 0;
        gBattle_BG0_Y = DISPLAY_HEIGHT;
        gBattlerControllerFuncs[gActiveBattler] = HandleInputChooseAction;
    }
}

static void SafariHandleChooseAction(void)
{
    s32 i;

    gBattlerControllerFuncs[gActiveBattler] = HandleChooseActionAfterDma3;
    BattlePutTextOnWindow(gText_SafariZoneMenu, 2);

    for (i = 0; i < 4; i++)
        ActionSelectionDestroyCursorAt(i);

    ActionSelectionCreateCursorAt(gActionSelectionCursor[gActiveBattler], 0);
    BattleStringExpandPlaceholdersToDisplayedString(gText_WhatWillPkmnDo2);
    BattlePutTextOnWindow(gDisplayedStringBattle, 1);
}

static void SafariHandleStatusIconUpdate(void)
{
    UpdateHealthboxAttribute(gHealthboxSpriteIds[gActiveBattler], &gPlayerParty[gBattlerPartyIndexes[gActiveBattler]], HEALTHBOX_SAFARI_BALLS_TEXT);
    SafariBufferExecCompleted();
}

static void SafariHandlePlaySE(void)
{
    s8 pan;

    if (GetBattlerSide(gActiveBattler) == B_SIDE_PLAYER)
        pan = SOUND_PAN_ATTACKER;
    else
        pan = SOUND_PAN_TARGET;

    PlaySE12WithPanning(gBattleBufferA[gActiveBattler][1] | (gBattleBufferA[gActiveBattler][2] << 8), pan);
    SafariBufferExecCompleted();
}

static void SafariHandlePlayFanfareOrBGM(void)
{
    if (gBattleBufferA[gActiveBattler][3])
    {
        BattleStopLowHpSound();
        PlayBGM(gBattleBufferA[gActiveBattler][1] | (gBattleBufferA[gActiveBattler][2] << 8));
    }
    else
    {
        PlayFanfare(gBattleBufferA[gActiveBattler][1] | (gBattleBufferA[gActiveBattler][2] << 8));
    }

    SafariBufferExecCompleted();
}

static void SafariHandleFaintingCry(void)
{
    u16 species = GetMonData(&gPlayerParty[gBattlerPartyIndexes[gActiveBattler]], MON_DATA_SPECIES);

    PlayCry1(species, 25);
    SafariBufferExecCompleted();
}

static void SafariHandleIntroSlide(void)
{
    HandleIntroSlide(gBattleBufferA[gActiveBattler][1]);
    gIntroSlideFlags |= 1;
    SafariBufferExecCompleted();
}

static void SafariHandleIntroTrainerBallThrow(void)
{
    UpdateHealthboxAttribute(gHealthboxSpriteIds[gActiveBattler], &gPlayerParty[gBattlerPartyIndexes[gActiveBattler]], HEALTHBOX_SAFARI_ALL_TEXT);
    StartHealthboxSlideIn(gActiveBattler);
    SetHealthboxSpriteVisible(gHealthboxSpriteIds[gActiveBattler]);
    gBattlerControllerFuncs[gActiveBattler] = CompleteOnHealthboxSpriteCallbackDummy;
}

static void SafariHandleBattleAnimation(void)
{
    u8 animationId = gBattleBufferA[gActiveBattler][1];
    u16 argument = gBattleBufferA[gActiveBattler][2] | (gBattleBufferA[gActiveBattler][3] << 8);

    if (TryHandleLaunchBattleTableAnimation(gActiveBattler, gActiveBattler, gActiveBattler, animationId, argument))
        SafariBufferExecCompleted();
    else
        gBattlerControllerFuncs[gActiveBattler] = CompleteOnFinishedBattleAnimation;
}

static void SafariHandleEndLinkBattle(void)
{
    gBattleOutcome = gBattleBufferA[gActiveBattler][1];
    FadeOutMapMusic(5);
    BeginFastPaletteFade(3);
    SafariBufferExecCompleted();
    if ((gBattleTypeFlags & BATTLE_TYPE_LINK) && !(gBattleTypeFlags & BATTLE_TYPE_IS_MASTER))
        gBattlerControllerFuncs[gActiveBattler] = SafariSetBattleEndCallbacks;
}
