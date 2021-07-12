#include "global.h"
#include "task.h"
#include "window.h"
#include "menu.h"
#include "menu_helpers.h"
#include "gpu_regs.h"
#include "bg.h"
#include "main.h"
#include "text.h"
#include "graphics.h"
#include "link.h"
#include "string_util.h"
#include "sound.h"
#include "mail.h"
#include "overworld.h"
#include "decompress.h"
#include "field_specials.h"
#include "constants/songs.h"
#include "constants/items.h"
#include "constants/maps.h"

static void Task_ContinueTaskAfterMessagePrints(u8 taskId);
static void Task_CallYesOrNoCallback(u8 taskId);

EWRAM_DATA static struct YesNoFuncTable gUnknown_0203A138 = {0};
EWRAM_DATA static u8 gUnknown_0203A140 = 0;

static TaskFunc gUnknown_0300117C;

// code
void ResetVramOamAndBgCntRegs(void)
{
    SetGpuReg(REG_OFFSET_DISPCNT, 0);
    SetGpuReg(REG_OFFSET_BG3CNT, 0);
    SetGpuReg(REG_OFFSET_BG2CNT, 0);
    SetGpuReg(REG_OFFSET_BG1CNT, 0);
    SetGpuReg(REG_OFFSET_BG0CNT, 0);
    CpuFill16(0, (void*) VRAM, VRAM_SIZE);
    CpuFill32(0, (void*) OAM, OAM_SIZE);
    CpuFill16(0, (void*) PLTT, PLTT_SIZE);
}

void ResetAllBgsCoordinates(void)
{
    ChangeBgX(0, 0, 0);
    ChangeBgY(0, 0, 0);
    ChangeBgX(1, 0, 0);
    ChangeBgY(1, 0, 0);
    ChangeBgX(2, 0, 0);
    ChangeBgY(2, 0, 0);
    ChangeBgX(3, 0, 0);
    ChangeBgY(3, 0, 0);
}

void SetVBlankHBlankCallbacksToNull(void)
{
    SetVBlankCallback(NULL);
    SetHBlankCallback(NULL);
}

void DisplayMessageAndContinueTask(u8 taskId, u8 windowId, u16 arg2, u8 arg3, u8 fontId, u8 textSpeed, const u8 *string, void *taskFunc)
{
    gUnknown_0203A140 = windowId;
    DrawDialogFrameWithCustomTileAndPalette(windowId, TRUE, arg2, arg3);

    if (string != gStringVar4)
        StringExpandPlaceholders(gStringVar4, string);

    gTextFlags.canABSpeedUpPrint = 1;
    AddTextPrinterParameterized2(windowId, fontId, gStringVar4, textSpeed, NULL, 2, 1, 3);
    gUnknown_0300117C = taskFunc;
    gTasks[taskId].func = Task_ContinueTaskAfterMessagePrints;
}

bool16 RunTextPrintersRetIsActive(u8 textPrinterId)
{
    RunTextPrinters();
    return IsTextPrinterActive(textPrinterId);
}

static void Task_ContinueTaskAfterMessagePrints(u8 taskId)
{
    if (!RunTextPrintersRetIsActive(gUnknown_0203A140))
        gUnknown_0300117C(taskId);
}

void DoYesNoFuncWithChoice(u8 taskId, const struct YesNoFuncTable *data)
{
    gUnknown_0203A138 = *data;
    gTasks[taskId].func = Task_CallYesOrNoCallback;
}

void CreateYesNoMenuWithCallbacks(u8 taskId, const struct WindowTemplate *template, u8 arg2, u8 arg3, u8 arg4, u16 tileStart, u8 palette, const struct YesNoFuncTable *yesNo)
{
    CreateYesNoMenu(template, arg2, arg3, arg4, tileStart, palette, 0);
    gUnknown_0203A138 = *yesNo;
    gTasks[taskId].func = Task_CallYesOrNoCallback;
}

static void Task_CallYesOrNoCallback(u8 taskId)
{
    switch (Menu_ProcessInputNoWrapClearOnChoose())
    {
    case 0:
        PlaySE(SE_SELECT);
        gUnknown_0203A138.yesFunc(taskId);
        break;
    case 1:
    case MENU_B_PRESSED:
        PlaySE(SE_SELECT);
        gUnknown_0203A138.noFunc(taskId);
        break;
    }
}

bool8 AdjustQuantityAccordingToDPadInput(s16 *arg0, u16 arg1)
{
    s16 valBefore = (*arg0);

    if ((JOY_REPEAT(DPAD_ANY)) == DPAD_UP)
    {
        (*arg0)++;
        if ((*arg0) > arg1)
            (*arg0) = 1;

        if ((*arg0) == valBefore)
        {
            return FALSE;
        }
        else
        {
            PlaySE(SE_SELECT);
            return TRUE;
        }
    }
    else if ((JOY_REPEAT(DPAD_ANY)) == DPAD_DOWN)
    {
        (*arg0)--;
        if ((*arg0) <= 0)
            (*arg0) = arg1;

        if ((*arg0) == valBefore)
        {
            return FALSE;
        }
        else
        {
            PlaySE(SE_SELECT);
            return TRUE;
        }
    }
    else if ((JOY_REPEAT(DPAD_ANY)) == DPAD_RIGHT)
    {
        (*arg0) += 10;
        if ((*arg0) > arg1)
            (*arg0) = arg1;

        if ((*arg0) == valBefore)
        {
            return FALSE;
        }
        else
        {
            PlaySE(SE_SELECT);
            return TRUE;
        }
    }
    else if ((JOY_REPEAT(DPAD_ANY)) == DPAD_LEFT)
    {
        (*arg0) -= 10;
        if ((*arg0) <= 0)
            (*arg0) = 1;

        if ((*arg0) == valBefore)
        {
            return FALSE;
        }
        else
        {
            PlaySE(SE_SELECT);
            return TRUE;
        }
    }

    return FALSE;
}

u8 GetLRKeysPressed(void)
{
    if (gSaveBlock2Ptr->optionsButtonMode == OPTIONS_BUTTON_MODE_LR)
    {
        if (JOY_NEW(L_BUTTON))
            return MENU_L_PRESSED;
        if (JOY_NEW(R_BUTTON))
            return MENU_R_PRESSED;
    }

    return 0;
}

u8 GetLRKeysPressedAndHeld(void)
{
    if (gSaveBlock2Ptr->optionsButtonMode == OPTIONS_BUTTON_MODE_LR)
    {
        if (JOY_REPEAT(L_BUTTON))
            return MENU_L_PRESSED;
        if (JOY_REPEAT(R_BUTTON))
            return MENU_R_PRESSED;
    }

    return 0;
}

bool8 sub_8122148(u16 itemId)
{
    if (itemId != ITEM_ENIGMA_BERRY)
        return TRUE;
    else if (gSaveBlock1Ptr->location.mapGroup == MAP_GROUP(TRADE_CENTER) && gSaveBlock1Ptr->location.mapNum == MAP_NUM(TRADE_CENTER))
        return FALSE;
    else if (InUnionRoom() != TRUE)
        return TRUE;
    else
        return FALSE;
}

bool8 IsWritingMailAllowed(u16 itemId)
{
    if (IsUpdateLinkStateCBActive() != TRUE && InUnionRoom() != TRUE)
        return TRUE;
    else if (ItemIsMail(itemId) != TRUE)
        return TRUE;
    else
        return FALSE;
}

bool8 MenuHelpers_LinkSomething(void)
{
    if (IsUpdateLinkStateCBActive() || gReceivedRemoteLinkPlayers == 1)
        return TRUE;
    else
        return FALSE;
}

static bool8 sub_81221D0(void)
{
    if (MenuHelpers_LinkSomething())
        return Overworld_LinkRecvQueueLengthMoreThan2();
    else
        return FALSE;
}

bool8 MenuHelpers_CallLinkSomething(void)
{
    if (sub_81221D0())
        return TRUE;
    else if (IsLinkRecvQueueLengthAtLeast3() != TRUE)
        return FALSE;
    else
        return TRUE;
}

void sub_812220C(struct ItemSlot *slots, u8 count, u8 *arg2, u8 *usedSlotsCount, u8 maxUsedSlotsCount)
{
    u32 i;
    struct ItemSlot *slots_ = slots;

    (*usedSlotsCount) = 0;
    for (i = 0; i < count; i++)
    {
        if (slots_[i].itemId)
            (*usedSlotsCount)++;
    }

    (*usedSlotsCount)++;
    if ((*usedSlotsCount) > maxUsedSlotsCount)
        *arg2 = maxUsedSlotsCount;
    else
        *arg2 = (*usedSlotsCount);
}

void sub_812225C(u16 *scrollOffset, u16 *cursorPos, u8 maxShownItems, u8 numItems)
{
    if (*scrollOffset && *scrollOffset + maxShownItems > numItems)
        *scrollOffset = numItems - maxShownItems;

    if (*scrollOffset + *cursorPos >= numItems)
    {
        if (numItems)
            *cursorPos = numItems - 1;
        else
            *cursorPos = 0;
    }
}
