#include "global.h"
#include "malloc.h"
#include "battle_main.h"
#include "data.h"
#include "decompress.h"
#include "gpu_regs.h"
#include "graphics.h"
#include "menu.h"
#include "international_string_util.h"
#include "menu.h"
#include "menu_specialized.h"
#include "move_relearner.h"
#include "palette.h"
#include "player_pc.h"
#include "pokemon_summary_screen.h"
#include "pokemon_storage_system.h"
#include "scanline_effect.h"
#include "sound.h"
#include "strings.h"
#include "string_util.h"
#include "text.h"
#include "text_window.h"
#include "trig.h"
#include "window.h"
#include "constants/songs.h"
#include "gba/io_reg.h"

extern const struct CompressedSpriteSheet gMonFrontPicTable[];

EWRAM_DATA static u8 sMailboxWindowIds[MAILBOXWIN_COUNT] = {0};
EWRAM_DATA static struct ListMenuItem *sMailboxList = NULL;

static void MailboxMenu_MoveCursorFunc(s32 itemIndex, bool8 onInit, struct ListMenu *list);
static void MoveRelearnerCursorCallback(s32 itemIndex, bool8 onInit, struct ListMenu *list);
static void nullsub_79(void);

static const struct WindowTemplate sWindowTemplates_MailboxMenu[MAILBOXWIN_COUNT] =
{
    [MAILBOXWIN_TITLE] = {
        .bg = 0,
        .tilemapLeft = 1,
        .tilemapTop = 1,
        .width = 8,
        .height = 2,
        .paletteNum = 15,
        .baseBlock = 0x8
    },
    [MAILBOXWIN_LIST] = {
        .bg = 0,
        .tilemapLeft = 21,
        .tilemapTop = 1,
        .width = 8,
        .height = 18,
        .paletteNum = 15,
        .baseBlock = 0x18
    },
    [MAILBOXWIN_OPTIONS] = {
        .bg = 0,
        .tilemapLeft = 1,
        .tilemapTop = 1,
        .width = 11,
        .height = 8,
        .paletteNum = 15,
        .baseBlock = 0x18
    }
};

static const u8 sPlayerNameTextColors[] =
{
    TEXT_COLOR_WHITE, TEXT_COLOR_DARK_GRAY, TEXT_COLOR_LIGHT_GRAY
};

static const u8 sEmptyItemName[] = _("");

static const u8 sUnknown_08625410[] =
{
    4,
    5,
    6,
    7,
    8,
    9, 9,
    10, 10,
    0xB, 0xB,
    0xC, 0xC,
    0xD, 0xD,
    0xD, 0xD,
    0xE, 0xE, 0xE, 0xE,
    0xF, 0xF, 0xF, 0xF,
    0x10, 0x10, 0x10, 0x10, 0x10, 0x10,
    0x11, 0x11, 0x11, 0x11, 0x11, 0x11,
    0x12, 0x12, 0x12, 0x12, 0x12, 0x12,
    0x13, 0x13, 0x13, 0x13, 0x13, 0x13, 0x13, 0x13,
    0x14, 0x14, 0x14, 0x14, 0x14, 0x14, 0x14, 0x14,
    0x15, 0x15, 0x15, 0x15, 0x15, 0x15, 0x15, 0x15,
    0x16, 0x16, 0x16, 0x16, 0x16, 0x16, 0x16, 0x16, 0x16, 0x16,
    0x17, 0x17, 0x17, 0x17, 0x17, 0x17, 0x17, 0x17, 0x17, 0x17,
    0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18,
    0x19, 0x19, 0x19, 0x19, 0x19, 0x19, 0x19, 0x19, 0x19, 0x19, 0x19, 0x19,
    0x1A, 0x1A, 0x1A, 0x1A, 0x1A, 0x1A, 0x1A, 0x1A, 0x1A, 0x1A, 0x1A, 0x1A,
    0x1B, 0x1B, 0x1B, 0x1B, 0x1B, 0x1B, 0x1B, 0x1B, 0x1B, 0x1B, 0x1B, 0x1B, 0x1B, 0x1B,
    0x1C, 0x1C, 0x1C, 0x1C, 0x1C, 0x1C, 0x1C, 0x1C, 0x1C, 0x1C, 0x1C, 0x1C, 0x1C, 0x1C,
    0x1D, 0x1D, 0x1D, 0x1D, 0x1D, 0x1D, 0x1D, 0x1D, 0x1D, 0x1D, 0x1D, 0x1D, 0x1D, 0x1D, 0x1D, 0x1D,
    0x1E, 0x1E, 0x1E, 0x1E, 0x1E, 0x1E, 0x1E, 0x1E, 0x1E, 0x1E, 0x1E, 0x1E, 0x1E, 0x1E, 0x1E, 0x1E,
    0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F,
    0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
    0x21, 0x21, 0x21, 0x21, 0x21, 0x21, 0x21, 0x21, 0x21, 0x21, 0x21, 0x21, 0x21, 0x21, 0x21, 0x21, 0x21, 0x21, 0x21,
    0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22,
    0x23
};


static const struct WindowTemplate sMoveRelearnerWindowTemplates[] =
{
    {
        .bg = 1,
        .tilemapLeft = 1,
        .tilemapTop = 1,
        .width = 16,
        .height = 12,
        .paletteNum = 0xF,
        .baseBlock = 0xA
    },
    {
        .bg = 1,
        .tilemapLeft = 1,
        .tilemapTop = 1,
        .width = 16,
        .height = 12,
        .paletteNum = 0xF,
        .baseBlock = 0xCA
    },
    {
        .bg = 1,
        .tilemapLeft = 19,
        .tilemapTop = 1,
        .width = 10,
        .height = 12,
        .paletteNum = 0xF,
        .baseBlock = 0x18A
    },
    {
        .bg = 1,
        .tilemapLeft = 4,
        .tilemapTop = 15,
        .width = 22,
        .height = 4,
        .paletteNum = 0xF,
        .baseBlock = 0x202
    },
    {
        .bg = 0,
        .tilemapLeft = 22,
        .tilemapTop = 8,
        .width = 5,
        .height = 4,
        .paletteNum = 0xF,
        .baseBlock = 0x25A
    },
    DUMMY_WIN_TEMPLATE
};

static const struct WindowTemplate sMoveRelearnerYesNoMenuTemplate =
{
    .bg = 0,
    .tilemapLeft = 22,
    .tilemapTop = 8,
    .width = 5,
    .height = 4,
    .paletteNum = 0xF,
    .baseBlock = 0x25A
};


static const struct ListMenuTemplate sMoveRelearnerMovesListTemplate =
{
    .items = NULL,
    .moveCursorFunc = MoveRelearnerCursorCallback,
    .itemPrintFunc = NULL,
    .totalItems = 0,
    .maxShowed = 0,
    .windowId = 2,
    .header_X = 0,
    .item_X = 8,
    .cursor_X = 0,
    .upText_Y = 1,
    .cursorPal = 2,
    .fillValue = 1,
    .cursorShadowPal = 3,
    .lettersSpacing = 0,
    .itemVerticalPadding = 0,
    .scrollMultiple = LIST_NO_MULTIPLE_SCROLL,
    .fontId = 2,
    .cursorKind = 0
};

bool8 MailboxMenu_Alloc(u8 count)
{
    u32 i;

    // + 1 to count for 'Cancel'
    sMailboxList = Alloc((count + 1) * sizeof(*sMailboxList));
    if (sMailboxList == NULL)
        return FALSE;

    for (i = 0; i < ARRAY_COUNT(sMailboxWindowIds); i++)
        sMailboxWindowIds[i] = WINDOW_NONE;

    return TRUE;
}

u8 MailboxMenu_AddWindow(u8 windowIdx)
{
    if (sMailboxWindowIds[windowIdx] == WINDOW_NONE)
    {
        if (windowIdx == MAILBOXWIN_OPTIONS)
        {
            struct WindowTemplate template = sWindowTemplates_MailboxMenu[windowIdx];
            template.width = GetMaxWidthInMenuTable(&gMailboxMailOptions[0], 4);
            sMailboxWindowIds[windowIdx] = AddWindow(&template);
        }
        else // MAILBOXWIN_TITLE or MAILBOXWIN_LIST
        {
            sMailboxWindowIds[windowIdx] = AddWindow(&sWindowTemplates_MailboxMenu[windowIdx]);
        }
        SetStandardWindowBorderStyle(sMailboxWindowIds[windowIdx], 0);
    }
    return sMailboxWindowIds[windowIdx];
}

void MailboxMenu_RemoveWindow(u8 windowIdx)
{
    ClearStdWindowAndFrameToTransparent(sMailboxWindowIds[windowIdx], 0);
    ClearWindowTilemap(sMailboxWindowIds[windowIdx]);
    RemoveWindow(sMailboxWindowIds[windowIdx]);
    sMailboxWindowIds[windowIdx] = WINDOW_NONE;
}

// Unused
static u8 MailboxMenu_GetWindowId(u8 windowIdx)
{
    return sMailboxWindowIds[windowIdx];
}

static void MailboxMenu_ItemPrintFunc(u8 windowId, u32 itemId, u8 y)
{
    u8 buffer[30];
    u16 length;

    if (itemId == LIST_CANCEL)
        return;

    StringCopy(buffer, gSaveBlock1Ptr->mail[PARTY_SIZE + itemId].playerName);
    ConvertInternationalPlayerName(buffer);
    length = StringLength(buffer);
    if (length < PLAYER_NAME_LENGTH - 1)
        ConvertInternationalString(buffer, LANGUAGE_JAPANESE);
    AddTextPrinterParameterized4(windowId, 2, 8, y, 0, 0, sPlayerNameTextColors, -1, buffer);
}

u8 MailboxMenu_CreateList(struct PlayerPCItemPageStruct *page)
{
    u32 i;

    for (i = 0; i < page->count; i++)
    {
        sMailboxList[i].name = sEmptyItemName;
        sMailboxList[i].id = i;
    }

    sMailboxList[i].name = gText_Cancel;
    sMailboxList[i].id = LIST_CANCEL;

    gMultiuseListMenuTemplate.items = sMailboxList;
    gMultiuseListMenuTemplate.totalItems = page->count + 1;
    gMultiuseListMenuTemplate.windowId = sMailboxWindowIds[MAILBOXWIN_LIST];
    gMultiuseListMenuTemplate.header_X = 0;
    gMultiuseListMenuTemplate.item_X = 8;
    gMultiuseListMenuTemplate.cursor_X = 0;
    gMultiuseListMenuTemplate.maxShowed = 8;
    gMultiuseListMenuTemplate.upText_Y = 9;
    gMultiuseListMenuTemplate.cursorPal = 2;
    gMultiuseListMenuTemplate.fillValue = 1;
    gMultiuseListMenuTemplate.cursorShadowPal = 3;
    gMultiuseListMenuTemplate.moveCursorFunc = MailboxMenu_MoveCursorFunc;
    gMultiuseListMenuTemplate.itemPrintFunc = MailboxMenu_ItemPrintFunc;
    gMultiuseListMenuTemplate.fontId = 2;
    gMultiuseListMenuTemplate.cursorKind = 0;
    gMultiuseListMenuTemplate.lettersSpacing = 0;
    gMultiuseListMenuTemplate.itemVerticalPadding = 0;
    gMultiuseListMenuTemplate.scrollMultiple = LIST_NO_MULTIPLE_SCROLL;
    return ListMenuInit(&gMultiuseListMenuTemplate, page->itemsAbove, page->cursorPos);
}

static void MailboxMenu_MoveCursorFunc(s32 itemIndex, bool8 onInit, struct ListMenu *list)
{
    if (onInit != TRUE)
        PlaySE(SE_SELECT);
}

void MailboxMenu_AddScrollArrows(struct PlayerPCItemPageStruct *page)
{
    page->scrollIndicatorTaskId = AddScrollIndicatorArrowPairParameterized(2, 0xC8, 12, 0x94, page->count - page->pageItems + 1, 0x6E, 0x6E, &page->itemsAbove);
}

void MailboxMenu_Free(void)
{
    Free(sMailboxList);
}

void sub_81D2754(u8 *arg0, struct UnknownSubStruct_81D1ED4 *arg1)
{
    u8 r2, r7;
    s8 r12;
    u32 i;

    r2 = sUnknown_08625410[*(arg0++)];
    arg1->unk0 = 155;
    arg1->unk2 = 91 - r2;

    r7 = 64;
    r12 = 0;
    for (i = 1; i < 5; i++)
    {
        r7 += 51;
        if (--r12 < 0)
            r12 = 4;

        if (r12 == 2)
            r7++;

        r2 = sUnknown_08625410[*(arg0++)];
        arg1[r12].unk0 = 155 + ((r2 * gSineTable[64 + r7]) >> 8);
        arg1[r12].unk2 = 91  - ((r2 * gSineTable[r7]) >> 8);

        if (r12 < 3 && (r2 != 32 || r12 != 2))
            arg1[r12].unk0 = 156 + ((r2 * gSineTable[64 + r7]) >> 8);
    }
}

void InitMoveRelearnerWindows(bool8 useContextWindow)
{
    u32 i;

    InitWindows(sMoveRelearnerWindowTemplates);
    DeactivateAllTextPrinters();
    LoadUserWindowBorderGfx(0, 1, 0xE0);
    LoadPalette(gUnknown_0860F074, 0xF0, 0x20);

    for (i = 0; i < 5; i++)
        FillWindowPixelBuffer(i, PIXEL_FILL(1));

    if (!useContextWindow)
    {
        PutWindowTilemap(0);
        DrawStdFrameWithCustomTileAndPalette(0, 0, 0x1, 0xE);
    }
    else
    {
        PutWindowTilemap(1);
        DrawStdFrameWithCustomTileAndPalette(1, 0, 1, 0xE);
    }
    PutWindowTilemap(2);
    PutWindowTilemap(3);
    DrawStdFrameWithCustomTileAndPalette(2, 0, 1, 0xE);
    DrawStdFrameWithCustomTileAndPalette(3, 0, 1, 0xE);
    nullsub_79();
    ScheduleBgCopyTilemapToVram(1);
}

static void nullsub_79(void)
{

}

u8 LoadMoveRelearnerMovesList(const struct ListMenuItem *items, u16 numChoices)
{
    gMultiuseListMenuTemplate = sMoveRelearnerMovesListTemplate;
    gMultiuseListMenuTemplate.totalItems = numChoices;
    gMultiuseListMenuTemplate.items = items;

    if (numChoices < 6)
        gMultiuseListMenuTemplate.maxShowed = numChoices;
    else
        gMultiuseListMenuTemplate.maxShowed = 6;
    return gMultiuseListMenuTemplate.maxShowed;
}

static void MoveRelearnerLoadBattleMoveDescription(u32 chosenMove)
{
    s32 x;
    const struct BattleMove *move;
    u8 buffer[0x20];
    const u8 *str;

    FillWindowPixelBuffer(0, PIXEL_FILL(1));
    str = gText_MoveRelearnerBattleMoves;
    x = GetStringCenterAlignXOffset(2, str, 0x80);
    AddTextPrinterParameterized(0, 2, str, x, 1, TEXT_SPEED_FF, NULL);

    str = gText_MoveRelearnerPP;
    AddTextPrinterParameterized(0, 2, str, 4, 0x29, TEXT_SPEED_FF, NULL);

    str = gText_MoveRelearnerPower;
    x = GetStringRightAlignXOffset(2, str, 0x6A);
    AddTextPrinterParameterized(0, 2, str, x, 0x19, TEXT_SPEED_FF, NULL);

    str = gText_MoveRelearnerAccuracy;
    x = GetStringRightAlignXOffset(2, str, 0x6A);
    AddTextPrinterParameterized(0, 2, str, x, 0x29, TEXT_SPEED_FF, NULL);
    if (chosenMove == LIST_CANCEL)
    {
        CopyWindowToVram(0, 2);
        return;
    }
    move = &gBattleMoves[chosenMove];
    str = gTypeNames[move->type];
    AddTextPrinterParameterized(0, 2, str, 4, 0x19, TEXT_SPEED_FF, NULL);

    x = 4 + GetStringWidth(2, gText_MoveRelearnerPP, 0);
    ConvertIntToDecimalStringN(buffer, move->pp, STR_CONV_MODE_LEFT_ALIGN, 2);
    AddTextPrinterParameterized(0, 2, buffer, x, 0x29, TEXT_SPEED_FF, NULL);

    if (move->power < 2)
    {
        str = gText_ThreeDashes;
    }
    else
    {
        ConvertIntToDecimalStringN(buffer, move->power, STR_CONV_MODE_LEFT_ALIGN, 3);
        str = buffer;
    }
    AddTextPrinterParameterized(0, 2, str, 0x6A, 0x19, TEXT_SPEED_FF, NULL);

    if (!move->accuracy)
        str = gText_ThreeDashes;
    else
    {
        ConvertIntToDecimalStringN(buffer, move->accuracy, STR_CONV_MODE_LEFT_ALIGN, 3);
        str = buffer;
    }
    AddTextPrinterParameterized(0, 2, str, 0x6A, 0x29, TEXT_SPEED_FF, NULL);

    str = gMoveDescriptionPointers[chosenMove - 1];
    AddTextPrinterParameterized(0, 5, str, 0, 0x41, 0, NULL);
}

static void MoveRelearnerCursorCallback(s32 itemIndex, bool8 onInit, struct ListMenu *list)
{
    if (onInit != TRUE)
        PlaySE(SE_SELECT);
    MoveRelearnerLoadBattleMoveDescription(itemIndex);
}

void MoveRelearnerPrintText(u8 *str)
{
    FillWindowPixelBuffer(3, PIXEL_FILL(1));
    gTextFlags.canABSpeedUpPrint = TRUE;
    AddTextPrinterParameterized2(3, 2, str, GetPlayerTextSpeedDelay(), NULL, TEXT_COLOR_DARK_GRAY, TEXT_COLOR_WHITE, 3);
}

bool16 MoveRelearnerRunTextPrinters(void)
{
    RunTextPrinters();
    return IsTextPrinterActive(3);
}

void MoveRelearnerCreateYesNoMenu(void)
{
    CreateYesNoMenu(&sMoveRelearnerYesNoMenuTemplate, 2, 0, 2, 1, 0xE, 0);
}

s32 GetBoxOrPartyMonData(u16 boxId, u16 monId, s32 request, u8 *dst)
{
    s32 ret;

    if (boxId == TOTAL_BOXES_COUNT) // Party mon.
    {
        if (request == MON_DATA_NICKNAME || request == MON_DATA_OT_NAME)
            ret = GetMonData(&gPlayerParty[monId], request, dst);
        else
            ret = GetMonData(&gPlayerParty[monId], request);
    }
    else
    {
        if (request == MON_DATA_NICKNAME || request == MON_DATA_OT_NAME)
            ret = GetAndCopyBoxMonDataAt(boxId, monId, request, dst);
        else
            ret = GetBoxMonDataAt(boxId, monId, request);
    }
    return ret;
}

static const u8 *const sLvlUpStatStrings[NUM_STATS] =
{
    gText_MaxHP,
    gText_Attack,
    gText_Defense,
    gText_SpAtk,
    gText_SpDef,
    gText_Speed
};

void DrawLevelUpWindowPg1(u16 windowId, u16 *statsBefore, u16 *statsAfter, u8 bgClr, u8 fgClr, u8 shadowClr)
{
    u32 i;
    u16 x;
    s16 statsDiff[NUM_STATS];
    u8 text[12];
    u8 color[3];

    FillWindowPixelBuffer(windowId, PIXEL_FILL(bgClr));

    statsDiff[0] = statsAfter[STAT_HP]    - statsBefore[STAT_HP];
    statsDiff[1] = statsAfter[STAT_ATK]   - statsBefore[STAT_ATK];
    statsDiff[2] = statsAfter[STAT_DEF]   - statsBefore[STAT_DEF];
    statsDiff[3] = statsAfter[STAT_SPATK] - statsBefore[STAT_SPATK];
    statsDiff[4] = statsAfter[STAT_SPDEF] - statsBefore[STAT_SPDEF];
    statsDiff[5] = statsAfter[STAT_SPEED] - statsBefore[STAT_SPEED];

    color[0] = bgClr;
    color[1] = fgClr;
    color[2] = shadowClr;

    for (i = 0; i < NUM_STATS; i++)
    {

        AddTextPrinterParameterized3(windowId,
                                     2,
                                     0,
                                     15 * i,
                                     color,
                                     -1,
                                     sLvlUpStatStrings[i]);

        StringCopy(text, (statsDiff[i] >= 0) ? gText_Plus : gText_Dash);
        AddTextPrinterParameterized3(windowId,
                                     2,
                                     56,
                                     15 * i,
                                     color,
                                     -1,
                                     text);
        if (abs(statsDiff[i]) <= 9)
            x = 18;
        else
            x = 12;

        ConvertIntToDecimalStringN(text, abs(statsDiff[i]), STR_CONV_MODE_LEFT_ALIGN, 2);
        AddTextPrinterParameterized3(windowId,
                                     2,
                                     56 + x,
                                     15 * i,
                                     color,
                                     -1,
                                     text);
    }
}

void DrawLevelUpWindowPg2(u16 windowId, u16 *currStats, u8 bgClr, u8 fgClr, u8 shadowClr)
{
    u32 i;
    u16 numDigits, x;
    s16 stats[NUM_STATS];
    u8 text[12];
    u8 color[3];

    FillWindowPixelBuffer(windowId, PIXEL_FILL(bgClr));

    stats[0] = currStats[STAT_HP];
    stats[1] = currStats[STAT_ATK];
    stats[2] = currStats[STAT_DEF];
    stats[3] = currStats[STAT_SPATK];
    stats[4] = currStats[STAT_SPDEF];
    stats[5] = currStats[STAT_SPEED];

    color[0] = bgClr;
    color[1] = fgClr;
    color[2] = shadowClr;

    for (i = 0; i < NUM_STATS; i++)
    {
        if (stats[i] > 99)
            numDigits = 3;
        else if (stats[i] > 9)
            numDigits = 2;
        else
            numDigits = 1;

        ConvertIntToDecimalStringN(text, stats[i], STR_CONV_MODE_LEFT_ALIGN, numDigits);
        x = 6 * (4 - numDigits);

        AddTextPrinterParameterized3(windowId,
                                     2,
                                     0,
                                     15 * i,
                                     color,
                                     -1,
                                     sLvlUpStatStrings[i]);

        AddTextPrinterParameterized3(windowId,
                                     2,
                                     56 + x,
                                     15 * i,
                                     color,
                                     -1,
                                     text);
    }
}

void GetMonLevelUpWindowStats(struct Pokemon *mon, u16 *currStats)
{
    currStats[STAT_HP]    = GetMonData(mon, MON_DATA_MAX_HP);
    currStats[STAT_ATK]   = GetMonData(mon, MON_DATA_ATK);
    currStats[STAT_DEF]   = GetMonData(mon, MON_DATA_DEF);
    currStats[STAT_SPEED] = GetMonData(mon, MON_DATA_SPEED);
    currStats[STAT_SPATK] = GetMonData(mon, MON_DATA_SPATK);
    currStats[STAT_SPDEF] = GetMonData(mon, MON_DATA_SPDEF);
}
