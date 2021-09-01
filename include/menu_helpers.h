#ifndef GUARD_MENU_HELPERS_H
#define GUARD_MENU_HELPERS_H

#include "task.h"
#include "window.h"

#define MENU_L_PRESSED 1
#define MENU_R_PRESSED 2

#define SWAP_LINE_HAS_MARGIN (1 << 7)

struct YesNoFuncTable
{
    TaskFunc yesFunc;
    TaskFunc noFunc;
};

void ResetVramOamAndBgCntRegs(void);
void ResetAllBgsCoordinates(void);
void SetVBlankHBlankCallbacksToNull(void);
void DisplayMessageAndContinueTask(u8 taskId, u8 windowId, u16 arg2, u8 arg3, u8 fontId, u8 textSpeed, const u8 *string, void *taskFunc);
bool16 RunTextPrintersRetIsActive(u8 textPrinterId);
void DoYesNoFuncWithChoice(u8 taskId, const struct YesNoFuncTable *data);
void CreateYesNoMenuWithCallbacks(u8 taskId, const struct WindowTemplate *template, u8 arg2, u8 arg3, u8 arg4, u16 tileStart, u8 palette, const struct YesNoFuncTable *yesNo);
bool8 AdjustQuantityAccordingToDPadInput(s16 *arg0, u16 arg1);
u8 GetLRKeysPressed(void);
u8 GetLRKeysPressedAndHeld(void);
bool8 IsHoldingItemAllowed(u16 itemId);
bool8 IsWritingMailAllowed(u16 itemId);
bool8 MenuHelpers_LinkSomething(void);
bool8 MenuHelpers_CallLinkSomething(void);
void SetItemListPerPageCount(struct ItemSlot *slots, u8 slotsCount, u8 *pageItems, u8 *totalItems, u8 maxPerPage);
void SetCursorWithinListBounds(u16 *scrollOffset, u16 *cursorPos, u8 maxShownItems, u8 totalItems);

#endif //GUARD_MENU_HELPERS_H
