#include "global.h"
#include "event_object_movement.h"
#include "malloc.h"
#include "bg.h"
#include "blit.h"
#include "dma3.h"
#include "event_data.h"
#include "field_specials.h"
#include "graphics.h"
#include "main.h"
#include "menu.h"
#include "menu_helpers.h"
#include "palette.h"
#include "pokedex.h"
#include "pokemon_icon.h"
#include "region_map.h"
#include "sound.h"
#include "string_util.h"
#include "strings.h"
#include "task.h"
#include "text_window.h"
#include "window.h"
#include "constants/songs.h"

#define DLG_WINDOW_PALETTE_NUM 15
#define DLG_WINDOW_BASE_TILE_NUM 0x200
#define STD_WINDOW_PALETTE_NUM 14
#define STD_WINDOW_BASE_TILE_NUM 0x214

struct Menu
{
    u8 left;
    u8 top;
    s8 cursorPos;
    s8 minCursorPos;
    s8 maxCursorPos;
    u8 windowId;
    u8 fontId;
    u8 optionWidth;
    u8 optionHeight;
    u8 columns;
    u8 rows;
    bool8 APressMuted;
};

static EWRAM_DATA u8 sStartMenuWindowId = 0;
static EWRAM_DATA u8 sMapNamePopupWindowId = 0;
static EWRAM_DATA struct Menu sMenu = {0};
static EWRAM_DATA u16 sTileNum = 0;
static EWRAM_DATA u8 sPaletteNum = 0;
static EWRAM_DATA u8 sYesNoWindowId = 0;
static EWRAM_DATA u8 sWindowId = 0;
static EWRAM_DATA u16 sFiller = 0;  // needed to align
static EWRAM_DATA bool8 sScheduledBgCopiesToVram[4] = {FALSE};
static EWRAM_DATA u16 sTempTileDataBufferIdx = 0;
static EWRAM_DATA void *sTempTileDataBuffer[0x20] = {NULL};

const u16 gUnknown_0860F074[] = INCBIN_U16("graphics/interface/860F074.gbapal");

static const u8 sTextSpeedFrameDelays[] = 
{ 
    [OPTIONS_TEXT_SPEED_SLOW] = 8, 
    [OPTIONS_TEXT_SPEED_MID]  = 4, 
    [OPTIONS_TEXT_SPEED_FAST] = 1 
};

const u16 gUnknown_0860F0B0[] = INCBIN_U16("graphics/interface/860F0B0.gbapal");
const u8 sTextColors[] = { TEXT_DYNAMIC_COLOR_6, TEXT_COLOR_WHITE, TEXT_COLOR_DARK_GRAY };

static const struct WindowTemplate sStandardTextBox_WindowTemplates[] =
{
    {
        .bg = 0,
        .tilemapLeft = 2,
        .tilemapTop = 15,
        .width = 26,
        .height = 4,
        .paletteNum = DLG_WINDOW_PALETTE_NUM,
        .baseBlock = 0x198
    },
    DUMMY_WIN_TEMPLATE
};

static const struct WindowTemplate sYesNo_WindowTemplates =
{
    .bg = 0,
    .tilemapLeft = 21,
    .tilemapTop = 9,
    .width = 6,
    .height = 4,
    .paletteNum = DLG_WINDOW_PALETTE_NUM,
    .baseBlock = 0x125
};

const struct FontInfo gFontInfos[] =
{
    { Font0Func, 0x6, 0x10, 0x0, 0x0, 0x0, 0x2, 0x1, 0x3 },
    { Font1Func, 0x6, 0x10, 0x0, 0x0, 0x0, 0x2, 0x1, 0x3 },
    { Font2Func, 0x6,  0xE, 0x0, 0x0, 0x0, 0x2, 0x1, 0x3 },
    { Font3Func, 0x6, 0x10, 0x0, 0x0, 0x0, 0x2, 0x1, 0x3 },
    { Font4Func, 0x5,  0xC, 0x0, 0x0, 0x0, 0x2, 0x1, 0x3 },
    { Font5Func, 0x5, 0x10, 0x0, 0x0, 0x0, 0x2, 0x1, 0x3 },
    { Font6Func, 0x8, 0x10, 0x0, 0x8, 0x0, 0x2, 0x1, 0x3 },
    { NULL,      0x8,  0x8, 0x0, 0x0, 0x0, 0x1, 0x2, 0xF }
};

const u8 gMenuCursorDimensions[][2] =
{
    { 0x8,  0xF },
    { 0x8,  0xF },
    { 0x8,  0xE },
    { 0x8,  0xF },
    { 0x8,  0xC },
    { 0x8,  0xF },
    { 0x8, 0x10 },
    { 0x0,  0x0 }
};

void WindowFunc_DrawDialogueFrame(u8 bg, u8 tilemapLeft, u8 tilemapTop, u8 width, u8 height, u8 paletteNum);
void WindowFunc_DrawStandardFrame(u8 bg, u8 tilemapLeft, u8 tilemapTop, u8 width, u8 height, u8 paletteNum);
void WindowFunc_ClearDialogWindowAndFrame(u8 bg, u8 tilemapLeft, u8 tilemapTop, u8 width, u8 height, u8 paletteNum);
void WindowFunc_ClearStdWindowAndFrame(u8 bg, u8 tilemapLeft, u8 tilemapTop, u8 width, u8 height, u8 paletteNum);

// Forward declarations
void WindowFunc_DrawDialogFrameWithCustomTileAndPalette(u8, u8, u8, u8, u8, u8);
void WindowFunc_ClearDialogWindowAndFrameNullPalette(u8, u8, u8, u8, u8, u8);
void WindowFunc_DrawStdFrameWithCustomTileAndPalette(u8, u8, u8, u8, u8, u8);
void WindowFunc_ClearStdWindowAndFrameToTransparent(u8, u8, u8, u8, u8, u8);
void sub_8198C78(void);
void task_free_buf_after_copying_tile_data_to_vram(u8 taskId);

void ClearScheduledBgCopiesToVram(void)
{
    memset(sScheduledBgCopiesToVram, 0, sizeof(sScheduledBgCopiesToVram));
}

void ScheduleBgCopyTilemapToVram(u8 bgId)
{
    sScheduledBgCopiesToVram[bgId] = TRUE;
}

void DoScheduledBgTilemapCopiesToVram(void)
{
    if (sScheduledBgCopiesToVram[0])
    {
        CopyBgTilemapBufferToVram(0);
        sScheduledBgCopiesToVram[0] = FALSE;
    }
    if (sScheduledBgCopiesToVram[1])
    {
        CopyBgTilemapBufferToVram(1);
        sScheduledBgCopiesToVram[1] = FALSE;
    }
    if (sScheduledBgCopiesToVram[2])
    {
        CopyBgTilemapBufferToVram(2);
        sScheduledBgCopiesToVram[2] = FALSE;
    }
    if (sScheduledBgCopiesToVram[3])
    {
        CopyBgTilemapBufferToVram(3);
        sScheduledBgCopiesToVram[3] = FALSE;
    }
}

void ResetTempTileDataBuffers(void)
{
    int i;

    for (i = 0; i < (int)ARRAY_COUNT(sTempTileDataBuffer); i++)
        sTempTileDataBuffer[i] = NULL;
    sTempTileDataBufferIdx = 0;
}

bool8 FreeTempTileDataBuffersIfPossible(void)
{
    int i;

    if (!IsDma3ManagerBusyWithBgCopy())
    {
        if (sTempTileDataBufferIdx)
        {
            for (i = 0; i < sTempTileDataBufferIdx; i++)
            {
                FREE_AND_SET_NULL(sTempTileDataBuffer[i]);
            }
            sTempTileDataBufferIdx = 0;
        }
        return FALSE;
    }
    return TRUE;
}

void *DecompressAndCopyTileDataToVram(u8 bgId, const void *src, u32 size, u16 offset, u8 mode)
{
    u32 sizeOut;

    if (sTempTileDataBufferIdx < ARRAY_COUNT(sTempTileDataBuffer))
    {
        void *ptr = malloc_and_decompress(src, &sizeOut);
        if (!size)
            size = sizeOut;
        if (ptr)
        {
            copy_decompressed_tile_data_to_vram(bgId, ptr, size, offset, mode);
            sTempTileDataBuffer[sTempTileDataBufferIdx++] = ptr;
        }
        return ptr;
    }
    return NULL;
}

void DecompressAndLoadBgGfxUsingHeap(u8 bgId, const void *src, u32 size, u16 offset, u8 mode)
{
    u32 sizeOut;
    void *ptr = malloc_and_decompress(src, &sizeOut);

    if (!size)
        size = sizeOut;
    if (ptr)
    {
        u8 taskId = CreateTask(task_free_buf_after_copying_tile_data_to_vram, 0);
        gTasks[taskId].data[0] = copy_decompressed_tile_data_to_vram(bgId, ptr, size, offset, mode);
        SetWordTaskArg(taskId, 1, (u32)ptr);
    }
}

void task_free_buf_after_copying_tile_data_to_vram(u8 taskId)
{
    if (!CheckForSpaceForDma3Request(gTasks[taskId].data[0]))
    {
        Free((void *)GetWordTaskArg(taskId, 1));
        DestroyTask(taskId);
    }
}

void *malloc_and_decompress(const void *src, u32 *size)
{
    void *ptr;
    u8 *sizeAsBytes = (u8 *)size;
    u8 *srcAsBytes = (u8 *)src;

    sizeAsBytes[0] = srcAsBytes[1];
    sizeAsBytes[1] = srcAsBytes[2];
    sizeAsBytes[2] = srcAsBytes[3];
    sizeAsBytes[3] = 0;

    ptr = Alloc(*size);
    if (ptr)
        LZ77UnCompWram(src, ptr);
    return ptr;
}

u16 copy_decompressed_tile_data_to_vram(u8 bgId, const void *src, u16 size, u16 offset, u8 mode)
{
    switch (mode)
    {
        case 0:
            return LoadBgTiles(bgId, src, size, offset);
        case 1:
            return LoadBgTilemap(bgId, src, size, offset);
        default:
            return -1;
    }
}

void SetBgTilemapPalette(u8 bgId, u8 left, u8 top, u8 width, u8 height, u8 palette)
{
    u32 i, j;
    u16 *ptr = GetBgTilemapBuffer(bgId);

    for (i = top; i < top + height; i++)
        for (j = left; j < left + width; j++)
            ptr[(i * 32) + j] = (ptr[(i * 32) + j] & 0xFFF) | (palette << 12);
}

void CopyToBufferFromBgTilemap(u8 bgId, u16 *dest, u8 left, u8 top, u8 width, u8 height)
{
    u32 i, j;
    const u16 *src = GetBgTilemapBuffer(bgId);

    for (i = 0; i < height; i++)
        for (j = 0; j < width; j++)
            dest[(i * width) + j] = src[(i + top) * 32 + j + left];
}

void sub_8199D3C(void *ptr, int delta, int width, int height, bool32 is8BPP)
{
    int i;
    int area = width * height;

    if (is8BPP)
    {
        u8 *as8BPP = ptr;

        for (i = 0; i < area; i++)
            as8BPP[i] += delta;
    }
    else
    {
        u16 *as4BPP = ptr;

        for (i = 0; i < area; i++)
            as4BPP[i] = (as4BPP[i] & 0xFC00) | ((as4BPP[i] + delta) & 0x3FF);
    }
}

void ResetBgPositions(void)
{
    ChangeBgX(0, 0, 0);
    ChangeBgX(1, 0, 0);
    ChangeBgX(2, 0, 0);
    ChangeBgX(3, 0, 0);
    ChangeBgY(0, 0, 0);
    ChangeBgY(1, 0, 0);
    ChangeBgY(2, 0, 0);
    ChangeBgY(3, 0, 0);
}

void InitStandardTextBoxWindows(void)
{
    InitWindows(sStandardTextBox_WindowTemplates);
    sStartMenuWindowId = WINDOW_NONE;
    sMapNamePopupWindowId = WINDOW_NONE;
}

void FreeAllOverworldWindowBuffers(void)
{
    FreeAllWindowBuffers();
}

void InitTextBoxGfxAndPrinters(void)
{
    ChangeBgX(0, 0, 0);
    ChangeBgY(0, 0, 0);
    DeactivateAllTextPrinters();
    LoadMessageBoxAndBorderGfx();
}

u16 RunTextPrintersAndIsPrinter0Active(void)
{
    RunTextPrinters();
    return IsTextPrinterActive(0);
}

u16 AddTextPrinterParameterized2(u8 windowId, u8 fontId, const u8 *str, u8 speed, void (*callback)(struct TextPrinterTemplate *, u16), u8 fgColor, u8 bgColor, u8 shadowColor)
{
    struct TextPrinterTemplate printer;

    printer.currentChar = str;
    printer.windowId = windowId;
    printer.fontId = fontId;
    printer.x = 0;
    printer.y = 1;
    printer.currentX = 0;
    printer.currentY = 1;
    printer.letterSpacing = 1;
    printer.lineSpacing = 1;
    printer.unk = 0;
    printer.fgColor = fgColor;
    printer.bgColor = bgColor;
    printer.shadowColor = shadowColor;
    gTextFlags.useAlternateDownArrow = 0;
    return AddTextPrinter(&printer, speed, callback);
}

void AddTextPrinterDiffStyle(bool8 allowSkippingDelayWithButtonPress)
{
    u8 result = ContextNpcGetTextColor();

    gTextFlags.canABSpeedUpPrint = allowSkippingDelayWithButtonPress;
    AddTextPrinterParameterized2(0, 2, gStringVar4, GetPlayerTextSpeedDelay(), NULL, gSpecialVar_TextColor, 1, 3);      
}

void AddTextPrinterForMessage(bool8 allowSkippingDelayWithButtonPress)
{
    gTextFlags.canABSpeedUpPrint = allowSkippingDelayWithButtonPress;
    AddTextPrinterParameterized2(0, 2, gStringVar4, GetPlayerTextSpeedDelay(), NULL, 2, 1, 3);
}

void AddTextPrinterWithCustomSpeedForMessage(bool8 allowSkippingDelayWithButtonPress, u8 speed)
{
    gTextFlags.canABSpeedUpPrint = allowSkippingDelayWithButtonPress;
    AddTextPrinterParameterized2(0, 2, gStringVar4, speed, NULL, 2, 1, 3);
}

void LoadMessageBoxAndBorderGfx(void)
{
    LoadMessageBoxGfx(0, DLG_WINDOW_BASE_TILE_NUM, DLG_WINDOW_PALETTE_NUM * 0x10);
    LoadUserWindowBorderGfx(0, STD_WINDOW_BASE_TILE_NUM, STD_WINDOW_PALETTE_NUM * 0x10);
}

void DrawDialogueFrame(u8 windowId, bool8 copyToVram)
{
    CallWindowFunction(windowId, WindowFunc_DrawDialogueFrame);
    FillWindowPixelBuffer(windowId, PIXEL_FILL(1));
    PutWindowTilemap(windowId);
    if (copyToVram)
        CopyWindowToVram(windowId, 3);
}

void DrawStdWindowFrame(u8 windowId, bool8 copyToVram)
{
    CallWindowFunction(windowId, WindowFunc_DrawStandardFrame);
    FillWindowPixelBuffer(windowId, PIXEL_FILL(1));
    PutWindowTilemap(windowId);
    if (copyToVram)
        CopyWindowToVram(windowId, 3);
}

void ClearDialogWindowAndFrame(u8 windowId, bool8 copyToVram)
{
    CallWindowFunction(windowId, WindowFunc_ClearDialogWindowAndFrame);
    FillWindowPixelBuffer(windowId, PIXEL_FILL(1));
    ClearWindowTilemap(windowId);
    if (copyToVram)
        CopyWindowToVram(windowId, 3);
}

void ClearStdWindowAndFrame(u8 windowId, bool8 copyToVram)
{
    CallWindowFunction(windowId, WindowFunc_ClearStdWindowAndFrame);
    FillWindowPixelBuffer(windowId, PIXEL_FILL(1));
    ClearWindowTilemap(windowId);
    if (copyToVram)
        CopyWindowToVram(windowId, 3);
}

void WindowFunc_DrawStandardFrame(u8 bg, u8 tilemapLeft, u8 tilemapTop, u8 width, u8 height, u8 paletteNum)
{
    int i;

    FillBgTilemapBufferRect(bg,
                            STD_WINDOW_BASE_TILE_NUM + 0,
                            tilemapLeft - 1,
                            tilemapTop - 1,
                            1,
                            1,
                            STD_WINDOW_PALETTE_NUM);
    FillBgTilemapBufferRect(bg,
                            STD_WINDOW_BASE_TILE_NUM + 1,
                            tilemapLeft,
                            tilemapTop - 1,
                            width,
                            1,
                            STD_WINDOW_PALETTE_NUM);
    FillBgTilemapBufferRect(bg,
                            STD_WINDOW_BASE_TILE_NUM + 2,
                            tilemapLeft + width,
                            tilemapTop - 1,
                            1,
                            1,
                            STD_WINDOW_PALETTE_NUM);

    for (i = tilemapTop; i < tilemapTop + height; i++)
    {
        FillBgTilemapBufferRect(bg,
                                STD_WINDOW_BASE_TILE_NUM + 3,
                                tilemapLeft - 1,
                                i,
                                1,
                                1,
                                STD_WINDOW_PALETTE_NUM);
        FillBgTilemapBufferRect(bg,
                                STD_WINDOW_BASE_TILE_NUM + 5,
                                tilemapLeft + width,
                                i,
                                1,
                                1,
                                STD_WINDOW_PALETTE_NUM);
    }

    FillBgTilemapBufferRect(bg,
                            STD_WINDOW_BASE_TILE_NUM + 6,
                            tilemapLeft - 1,
                            tilemapTop + height,
                            1,
                            1,
                            STD_WINDOW_PALETTE_NUM);
    FillBgTilemapBufferRect(bg,
                            STD_WINDOW_BASE_TILE_NUM + 7,
                            tilemapLeft,
                            tilemapTop + height,
                            width,
                            1,
                            STD_WINDOW_PALETTE_NUM);
    FillBgTilemapBufferRect(bg,
                            STD_WINDOW_BASE_TILE_NUM + 8,
                            tilemapLeft + width,
                            tilemapTop + height,
                            1,
                            1,
                            STD_WINDOW_PALETTE_NUM);
}

void WindowFunc_DrawDialogueFrame(u8 bg, u8 tilemapLeft, u8 tilemapTop, u8 width, u8 height, u8 paletteNum)
{
    FillBgTilemapBufferRect(bg,
                            DLG_WINDOW_BASE_TILE_NUM + 1,
                            tilemapLeft - 1,
                            tilemapTop - 1,
                            1,
                            1,
                            DLG_WINDOW_PALETTE_NUM);
    FillBgTilemapBufferRect(bg,
                            DLG_WINDOW_BASE_TILE_NUM + 2,
                            tilemapLeft - 2,
                            tilemapTop,
                            1,
                            1,
                            DLG_WINDOW_PALETTE_NUM);
    FillBgTilemapBufferRect(bg,
                            DLG_WINDOW_BASE_TILE_NUM + 3,
                            tilemapLeft - 1,
                            tilemapTop,
                            1,
                            1,
                            DLG_WINDOW_PALETTE_NUM);
    FillBgTilemapBufferRect(bg,
                            DLG_WINDOW_BASE_TILE_NUM + 4,
                            tilemapLeft - 2,
                            tilemapTop + 1,
                            1,
                            2,
                            DLG_WINDOW_PALETTE_NUM);
    FillBgTilemapBufferRect(bg,
                            DLG_WINDOW_BASE_TILE_NUM + 5,
                            tilemapLeft - 1,
                            tilemapTop + 1,
                            width + 2,
                            2,
                            DLG_WINDOW_PALETTE_NUM);
    FillBgTilemapBufferRect(bg,
                            DLG_WINDOW_BASE_TILE_NUM + 6,
                            tilemapLeft,
                            tilemapTop - 1,
                            width,
                            1,
                            DLG_WINDOW_PALETTE_NUM);
    FillBgTilemapBufferRect(bg,
                            BG_TILE_H_FLIP(DLG_WINDOW_BASE_TILE_NUM + 1),
                            tilemapLeft + width,
                            tilemapTop - 1,
                            1,
                            1,
                            DLG_WINDOW_PALETTE_NUM);
    FillBgTilemapBufferRect(bg,
                            BG_TILE_H_FLIP(DLG_WINDOW_BASE_TILE_NUM + 2),
                            tilemapLeft + width + 1,
                            tilemapTop,
                            1,
                            1,
                            DLG_WINDOW_PALETTE_NUM);
    FillBgTilemapBufferRect(bg,
                            BG_TILE_H_FLIP(DLG_WINDOW_BASE_TILE_NUM + 3),
                            tilemapLeft + width,
                            tilemapTop,
                            1,
                            1,
                            DLG_WINDOW_PALETTE_NUM);
    FillBgTilemapBufferRect(bg,
                            BG_TILE_H_FLIP(DLG_WINDOW_BASE_TILE_NUM + 4),
                            tilemapLeft + width + 1,
                            tilemapTop + 1,
                            1,
                            2,
                            DLG_WINDOW_PALETTE_NUM);
    FillBgTilemapBufferRect(bg,
                            BG_TILE_V_FLIP(DLG_WINDOW_BASE_TILE_NUM + 1),
                            tilemapLeft - 1,
                            tilemapTop + height,
                            1,
                            1,
                            DLG_WINDOW_PALETTE_NUM);
    FillBgTilemapBufferRect(bg,
                            BG_TILE_V_FLIP(DLG_WINDOW_BASE_TILE_NUM + 2),
                            tilemapLeft - 2,
                            tilemapTop + height - 1,
                            1,
                            1,
                            DLG_WINDOW_PALETTE_NUM);
    FillBgTilemapBufferRect(bg,
                            BG_TILE_V_FLIP(DLG_WINDOW_BASE_TILE_NUM + 3),
                            tilemapLeft - 1,
                            tilemapTop + height - 1,
                            1,
                            1,
                            DLG_WINDOW_PALETTE_NUM);
    FillBgTilemapBufferRect(bg,
                            BG_TILE_V_FLIP(DLG_WINDOW_BASE_TILE_NUM + 6),
                            tilemapLeft,
                            tilemapTop + height,
                            width,
                            1,
                            DLG_WINDOW_PALETTE_NUM);
    FillBgTilemapBufferRect(bg,
                            BG_TILE_HV_FLIP(DLG_WINDOW_BASE_TILE_NUM + 1),
                            tilemapLeft + width,
                            tilemapTop + height,
                            1,
                            1,
                            DLG_WINDOW_PALETTE_NUM);
    FillBgTilemapBufferRect(bg,
                            BG_TILE_HV_FLIP(DLG_WINDOW_BASE_TILE_NUM + 2),
                            tilemapLeft + width + 1,
                            tilemapTop + height - 1,
                            1,
                            1,
                            DLG_WINDOW_PALETTE_NUM);
    FillBgTilemapBufferRect(bg,
                            BG_TILE_HV_FLIP(DLG_WINDOW_BASE_TILE_NUM + 3),
                            tilemapLeft + width,
                            tilemapTop + height - 1,
                            1,
                            1,
                            DLG_WINDOW_PALETTE_NUM);
}

void WindowFunc_ClearStdWindowAndFrame(u8 bg, u8 tilemapLeft, u8 tilemapTop, u8 width, u8 height, u8 paletteNum)
{
    FillBgTilemapBufferRect(bg, 0, tilemapLeft - 1, tilemapTop - 1, width + 2, height + 2, STD_WINDOW_PALETTE_NUM);
}

void WindowFunc_ClearDialogWindowAndFrame(u8 bg, u8 tilemapLeft, u8 tilemapTop, u8 width, u8 height, u8 paletteNum)
{
    FillBgTilemapBufferRect(bg, 0, tilemapLeft - 3, tilemapTop - 1, width + 6, height + 2, STD_WINDOW_PALETTE_NUM);
}

void sub_8197AE8(bool8 copyToVram)
{
    FillBgTilemapBufferRect(0, 0, 0, 0, 32, 32, 0x11);
    if (copyToVram)
        CopyBgTilemapBufferToVram(0);
}

void SetStandardWindowBorderStyle(u8 windowId, bool8 copyToVram)
{
    DrawStdFrameWithCustomTileAndPalette(windowId, copyToVram, STD_WINDOW_BASE_TILE_NUM, STD_WINDOW_PALETTE_NUM);
}

void sub_819786C(u8 windowId, bool8 copyToVram)
{
    LoadMessageBoxGfx(windowId, DLG_WINDOW_BASE_TILE_NUM, DLG_WINDOW_PALETTE_NUM * 0x10);
    DrawDialogFrameWithCustomTileAndPalette(windowId, copyToVram, DLG_WINDOW_BASE_TILE_NUM, DLG_WINDOW_PALETTE_NUM);
}

void sub_819789C(void)
{
    LoadPalette(gUnknown_0860F074, STD_WINDOW_PALETTE_NUM * 0x10, 0x14);
}

void Menu_LoadStdPalAt(u16 offset)
{
    LoadPalette(gUnknown_0860F074, offset, 0x14);
}

const u16 *sub_81978C8(void)
{
    return gUnknown_0860F074;
}

u16 sub_81978D0(u8 colorNum)
{
    if (colorNum > 15)
        colorNum = 0;
    return gUnknown_0860F074[colorNum];
}

void DisplayItemMessageOnField(u8 taskId, const u8 *string, TaskFunc callback)
{
    LoadMessageBoxAndBorderGfx();
    DisplayMessageAndContinueTask(taskId, 0, DLG_WINDOW_BASE_TILE_NUM, DLG_WINDOW_PALETTE_NUM, 2, GetPlayerTextSpeedDelay(), string, callback);
    CopyWindowToVram(0, 3);
}

void DisplayYesNoMenuDefaultYes(void)
{
    CreateYesNoMenu(&sYesNo_WindowTemplates, 2, 0, 2, STD_WINDOW_BASE_TILE_NUM, STD_WINDOW_PALETTE_NUM, 0);
}

void DisplayYesNoMenuWithDefault(u8 initialCursorPos)
{
    CreateYesNoMenu(&sYesNo_WindowTemplates, 2, 0, 2, STD_WINDOW_BASE_TILE_NUM, STD_WINDOW_PALETTE_NUM, initialCursorPos);
}

u32 GetPlayerTextSpeed(void)
{
    if (gTextFlags.forceMidTextSpeed)
        return OPTIONS_TEXT_SPEED_MID;
    return gSaveBlock2Ptr->optionsTextSpeed;
}

u8 GetPlayerTextSpeedDelay(void)
{
    u32 speed;

    if (gSaveBlock2Ptr->optionsTextSpeed > OPTIONS_TEXT_SPEED_FAST)
        gSaveBlock2Ptr->optionsTextSpeed = OPTIONS_TEXT_SPEED_MID;
    speed = GetPlayerTextSpeed();
    return sTextSpeedFrameDelays[speed];
}

u8 sub_81979C4(u8 a1)
{
    if (sStartMenuWindowId == WINDOW_NONE)
        sStartMenuWindowId = sub_8198AA4(0, 0x16, 1, 7, (a1 * 2) - 1, DLG_WINDOW_PALETTE_NUM, 0x13D);
    return sStartMenuWindowId;
}

u8 GetStartMenuWindowId(void)
{
    return sStartMenuWindowId;
}

void RemoveStartMenuWindow(void)
{
    if (sStartMenuWindowId != WINDOW_NONE)
    {
        RemoveWindow(sStartMenuWindowId);
        sStartMenuWindowId = WINDOW_NONE;
    }
}

u8 AddMapNamePopUpWindow(void)
{
    if (sMapNamePopupWindowId == WINDOW_NONE)
        sMapNamePopupWindowId = sub_8198AA4(0, 1, 1, 10, 3, 14, 0x107);
    return sMapNamePopupWindowId;
}

u8 GetMapNamePopUpWindowId(void)
{
    return sMapNamePopupWindowId;
}

void RemoveMapNamePopUpWindow(void)
{
    if (sMapNamePopupWindowId != WINDOW_NONE)
    {
        RemoveWindow(sMapNamePopupWindowId);
        sMapNamePopupWindowId = WINDOW_NONE;
    }
}

void SetDefaultFontsPointer(void)
{
    SetFontsPointer(&gFontInfos[0]);
}

u8 GetFontAttribute(u8 fontId, u8 attributeId)
{
    int result = 0;
    switch (attributeId)
    {
        case FONTATTR_MAX_LETTER_WIDTH:
            result = gFontInfos[fontId].maxLetterWidth;
            break;
        case FONTATTR_MAX_LETTER_HEIGHT:
            result = gFontInfos[fontId].maxLetterHeight;
            break;
        case FONTATTR_LETTER_SPACING:
            result = gFontInfos[fontId].letterSpacing;
            break;
        case FONTATTR_LINE_SPACING:
            result = gFontInfos[fontId].lineSpacing;
            break;
        case FONTATTR_UNKNOWN:
            result = gFontInfos[fontId].unk;
            break;
        case FONTATTR_COLOR_FOREGROUND:
            result = gFontInfos[fontId].fgColor;
            break;
        case FONTATTR_COLOR_BACKGROUND:
            result = gFontInfos[fontId].bgColor;
            break;
        case FONTATTR_COLOR_SHADOW:
            result = gFontInfos[fontId].shadowColor;
            break;
    }
    return result;
}

u8 GetMenuCursorDimensionByFont(u8 fontId, u8 whichDimension)
{
    return gMenuCursorDimensions[fontId][whichDimension];
}

// save_menu_util.c in FRLG
void BufferSaveMenuText(u8 textId, u8 *dest, u8 color)
{
    s32 curFlag;
    s32 flagCount;
    u8 *endOfString;
    u8 *string = dest;

    *(string++) = EXT_CTRL_CODE_BEGIN;
    *(string++) = EXT_CTRL_CODE_COLOR;
    *(string++) = color;
    *(string++) = EXT_CTRL_CODE_BEGIN;
    *(string++) = EXT_CTRL_CODE_SHADOW;
    *(string++) = color + 1;

    switch (textId)
    {
        case SAVE_MENU_NAME:
            StringCopy(string, gSaveBlock2Ptr->playerName);
            break;
        case SAVE_MENU_CAUGHT:
            if (IsNationalPokedexEnabled())
                string = ConvertIntToDecimalStringN(string, GetNationalPokedexCount(FLAG_GET_CAUGHT), STR_CONV_MODE_LEFT_ALIGN, 3);
            else
                string = ConvertIntToDecimalStringN(string, GetKantoPokedexCount(FLAG_GET_CAUGHT), STR_CONV_MODE_LEFT_ALIGN, 3);
            *string = EOS;
            break;
        case SAVE_MENU_PLAY_TIME:
            string = ConvertIntToDecimalStringN(string, gSaveBlock2Ptr->playTimeHours, STR_CONV_MODE_LEFT_ALIGN, 3);
            *(string++) = CHAR_COLON;
            ConvertIntToDecimalStringN(string, gSaveBlock2Ptr->playTimeMinutes, STR_CONV_MODE_LEADING_ZEROS, 2);
            break;
        case SAVE_MENU_LOCATION:
            GetMapNameGeneric(string, gMapHeader.regionMapSectionId);
            break;
        case SAVE_MENU_BADGES:
            for (curFlag = FLAG_BADGE01_GET, flagCount = 0, endOfString = string + 1; curFlag < FLAG_BADGE01_GET + NUM_BADGES; curFlag++)
            {
                if (FlagGet(curFlag))
                    flagCount++;
            }
            *string = flagCount + CHAR_0;
            *endOfString = EOS;
            break;
    }
}

void AddTextPrinterWithCallbackForMessage(bool8 a1, void (*callback)(struct TextPrinterTemplate *, u16))
{
    gTextFlags.canABSpeedUpPrint = a1;
    AddTextPrinterParameterized2(0, 2, gStringVar4, GetPlayerTextSpeedDelay(), callback, 2, 1, 3);
}

void DrawDialogFrameWithCustomTileAndPalette(u8 windowId, bool8 copyToVram, u16 tileNum, u8 paletteNum)
{
    sTileNum = tileNum;
    sPaletteNum = paletteNum;
    CallWindowFunction(windowId, WindowFunc_DrawDialogFrameWithCustomTileAndPalette);
    FillWindowPixelBuffer(windowId, PIXEL_FILL(1));
    PutWindowTilemap(windowId);
    if (copyToVram)
        CopyWindowToVram(windowId, 3);
}

void WindowFunc_DrawDialogFrameWithCustomTileAndPalette(u8 bg, u8 tilemapLeft, u8 tilemapTop, u8 width, u8 height, u8 paletteNum)
{
    FillBgTilemapBufferRect(bg,
                            sTileNum + 1,
                            tilemapLeft - 1,
                            tilemapTop - 1,
                            1,
                            1,
                            sPaletteNum);
    FillBgTilemapBufferRect(bg,
                            sTileNum + 2,
                            tilemapLeft - 2,
                            tilemapTop,
                            1,
                            1,
                            sPaletteNum);
    FillBgTilemapBufferRect(bg,
                            sTileNum + 3,
                            tilemapLeft - 1,
                            tilemapTop,
                            1,
                            1,
                            sPaletteNum);
    FillBgTilemapBufferRect(bg,
                            sTileNum + 4,
                            tilemapLeft - 2,
                            tilemapTop + 1,
                            1,
                            2,
                            sPaletteNum);
    FillBgTilemapBufferRect(bg,
                            sTileNum + 5,
                            tilemapLeft - 1,
                            tilemapTop + 1,
                            width + 2,
                            2,
                            sPaletteNum);
    FillBgTilemapBufferRect(bg,
                            sTileNum + 6,
                            tilemapLeft,
                            tilemapTop - 1,
                            width,
                            1,
                            sPaletteNum);
    FillBgTilemapBufferRect(bg,
                            BG_TILE_H_FLIP(sTileNum + 1),
                            tilemapLeft + width,
                            tilemapTop - 1,
                            1,
                            1,
                            sPaletteNum);
    FillBgTilemapBufferRect(bg,
                            BG_TILE_H_FLIP(sTileNum + 2),
                            tilemapLeft + width + 1,
                            tilemapTop,
                            1,
                            1,
                            sPaletteNum);
    FillBgTilemapBufferRect(bg,
                            BG_TILE_H_FLIP(sTileNum + 3),
                            tilemapLeft + width,
                            tilemapTop,
                            1,
                            1,
                            sPaletteNum);
    FillBgTilemapBufferRect(bg,
                            BG_TILE_H_FLIP(sTileNum + 4),
                            tilemapLeft + width + 1,
                            tilemapTop + 1,
                            1,
                            2,
                            sPaletteNum);
    FillBgTilemapBufferRect(bg,
                            BG_TILE_V_FLIP(sTileNum + 1),
                            tilemapLeft - 1,
                            tilemapTop + height,
                            1,
                            1,
                            sPaletteNum);
    FillBgTilemapBufferRect(bg,
                            BG_TILE_V_FLIP(sTileNum + 2),
                            tilemapLeft - 2,
                            tilemapTop + height - 1,
                            1,
                            1,
                            sPaletteNum);
    FillBgTilemapBufferRect(bg,
                            BG_TILE_V_FLIP(sTileNum + 3),
                            tilemapLeft - 1,
                            tilemapTop + height - 1,
                            1,
                            1,
                            sPaletteNum);
    FillBgTilemapBufferRect(bg,
                            BG_TILE_V_FLIP(sTileNum + 6),
                            tilemapLeft,
                            tilemapTop + height,
                            width,
                            1,
                            sPaletteNum);
    FillBgTilemapBufferRect(bg,
                            BG_TILE_HV_FLIP(sTileNum + 1),
                            tilemapLeft + width,
                            tilemapTop + height,
                            1,
                            1,
                            sPaletteNum);
    FillBgTilemapBufferRect(bg,
                            BG_TILE_HV_FLIP(sTileNum + 2),
                            tilemapLeft + width + 1,
                            tilemapTop + height - 1,
                            1,
                            1,
                            sPaletteNum);
    FillBgTilemapBufferRect(bg,
                            BG_TILE_HV_FLIP(sTileNum + 3),
                            tilemapLeft + width,
                            tilemapTop + height - 1,
                            1,
                            1,
                            sPaletteNum);
}

void ClearDialogWindowAndFrameToTransparent(u8 windowId, bool8 copyToVram)
{
    // The palette slot doesn't matter, since the tiles are transparent.
    CallWindowFunction(windowId, WindowFunc_ClearDialogWindowAndFrameNullPalette);
    FillWindowPixelBuffer(windowId, PIXEL_FILL(0));
    ClearWindowTilemap(windowId);
    if (copyToVram)
        CopyWindowToVram(windowId, 3);
}

void WindowFunc_ClearDialogWindowAndFrameNullPalette(u8 bg, u8 tilemapLeft, u8 tilemapTop, u8 width, u8 height, u8 paletteNum)
{
    FillBgTilemapBufferRect(bg, 0, tilemapLeft - 2, tilemapTop - 1, width + 4, height + 2, 0);
}

void DrawStdFrameWithCustomTileAndPalette(u8 windowId, bool8 copyToVram, u16 baseTileNum, u8 paletteNum)
{
    sTileNum = baseTileNum;
    sPaletteNum = paletteNum;
    CallWindowFunction(windowId, WindowFunc_DrawStdFrameWithCustomTileAndPalette);
    FillWindowPixelBuffer(windowId, PIXEL_FILL(1));
    PutWindowTilemap(windowId);
    if (copyToVram)
        CopyWindowToVram(windowId, 3);
}

void WindowFunc_DrawStdFrameWithCustomTileAndPalette(u8 bg, u8 tilemapLeft, u8 tilemapTop, u8 width, u8 height, u8 paletteNum)
{
    FillBgTilemapBufferRect(bg,
                            sTileNum + 0,
                            tilemapLeft - 1,
                            tilemapTop - 1,
                            1,
                            1,
                            sPaletteNum);
    FillBgTilemapBufferRect(bg,
                            sTileNum + 1,
                            tilemapLeft,
                            tilemapTop - 1,
                            width,
                            1,
                            sPaletteNum);
    FillBgTilemapBufferRect(bg,
                            sTileNum + 2,
                            tilemapLeft + width,
                            tilemapTop - 1,
                            1,
                            1,
                            sPaletteNum);
    FillBgTilemapBufferRect(bg,
                            sTileNum + 3,
                            tilemapLeft - 1,
                            tilemapTop,
                            1,
                            height,
                            sPaletteNum);
    FillBgTilemapBufferRect(bg,
                            sTileNum + 5,
                            tilemapLeft + width,
                            tilemapTop,
                            1,
                            height,
                            sPaletteNum);
    FillBgTilemapBufferRect(bg,
                            sTileNum + 6,
                            tilemapLeft - 1,
                            tilemapTop + height,
                            1,
                            1,
                            sPaletteNum);
    FillBgTilemapBufferRect(bg,
                            sTileNum + 7,
                            tilemapLeft,
                            tilemapTop + height,
                            width,
                            1,
                            sPaletteNum);
    FillBgTilemapBufferRect(bg,
                            sTileNum + 8,
                            tilemapLeft + width,
                            tilemapTop + height,
                            1,
                            1,
                            sPaletteNum);
}

void ClearStdWindowAndFrameToTransparent(u8 windowId, bool8 copyToVram)
{
    CallWindowFunction(windowId, WindowFunc_ClearStdWindowAndFrameToTransparent);
    FillWindowPixelBuffer(windowId, PIXEL_FILL(0));
    ClearWindowTilemap(windowId);
    if (copyToVram)
        CopyWindowToVram(windowId, 3);
}

void WindowFunc_ClearStdWindowAndFrameToTransparent(u8 bg, u8 tilemapLeft, u8 tilemapTop, u8 width, u8 height, u8 paletteNum)
{
    FillBgTilemapBufferRect(bg, 0, tilemapLeft - 1, tilemapTop - 1, width + 2, height + 2, 0);
}

u8 sub_81980F0(u8 bg, u8 xPos, u8 yPos, u8 palette, u16 baseTile)
{
    struct WindowTemplate window;
    memset(&window, 0, sizeof(window));

    if (bg > 3)
        window.bg = 0;
    else
        window.bg = bg;

    window.tilemapTop = yPos;
    window.height = 2;
    window.tilemapLeft = 0x1E - xPos;
    window.width = xPos;
    window.paletteNum = palette;
    window.baseBlock = baseTile;

    sWindowId = AddWindow(&window);

    if (palette > 15)
        palette = 15 * 16;
    else
        palette *= 16;

    LoadPalette(gUnknown_0860F0B0, palette, sizeof(gUnknown_0860F0B0));
    return sWindowId;
}

void sub_8198180(const u8 *string, u8 a2, bool8 copyToVram)
{
    u16 width = 0;

    if (sWindowId != WINDOW_NONE)
    {
        PutWindowTilemap(sWindowId);
        FillWindowPixelBuffer(sWindowId, PIXEL_FILL(15));
        width = GetStringWidth(4, string, 0);
        AddTextPrinterParameterized3(sWindowId,
                  4,
                  0xEC - (GetWindowAttribute(sWindowId, WINDOW_TILEMAP_LEFT) * 8) - a2 - width,
                  1,
                  sTextColors,
                  0,
                  string);
        if (copyToVram)
            CopyWindowToVram(sWindowId, 3);
    }
}

void sub_8198204(const u8 *string, const u8 *string2, u8 a3, u8 a4, bool8 copyToVram)
{
    u8 color[3];
    u16 width = 0;

    if (sWindowId != WINDOW_NONE)
    {
        if (a3)
            color[0] = TEXT_COLOR_TRANSPARENT;
        else
            color[0] = TEXT_DYNAMIC_COLOR_6;
        color[1] = TEXT_COLOR_WHITE;
        color[2] = TEXT_COLOR_DARK_GRAY;
        PutWindowTilemap(sWindowId);
        FillWindowPixelBuffer(sWindowId, PIXEL_FILL(15));
        if (string2 != NULL)
        {
            width = GetStringWidth(4, string2, 0);
            AddTextPrinterParameterized3(sWindowId,
                      4,
                      0xEC - (GetWindowAttribute(sWindowId, WINDOW_TILEMAP_LEFT) * 8) - a4 - width,
                      1,
                      color,
                      0,
                      string2);
        }
        AddTextPrinterParameterized4(sWindowId, 2, 4, 1, 0, 0, color, 0, string);
        if (copyToVram)
            CopyWindowToVram(sWindowId, 3);
    }
}

void sub_81982F0(void)
{
    if (sWindowId != WINDOW_NONE)
    {
        FillWindowPixelBuffer(sWindowId, PIXEL_FILL(15));
        CopyWindowToVram(sWindowId, 3);
    }
}

void sub_8198314(void)
{
    if (sWindowId != WINDOW_NONE)
    {
        FillWindowPixelBuffer(sWindowId, PIXEL_FILL(0));
        ClearWindowTilemap(sWindowId);
        CopyWindowToVram(sWindowId, 3);
        RemoveWindow(sWindowId);
        sWindowId = WINDOW_NONE;
    }
}

u8 InitMenuInUpperLeftCorner(u8 windowId, u8 fontId, u8 left, u8 top, u8 cursorHeight, u8 itemCount, u8 initialCursorPos, bool8 APressMuted)
{
    s32 pos;

    sMenu.left = left;
    sMenu.top = top;
    sMenu.minCursorPos = 0;
    sMenu.maxCursorPos = itemCount - 1;
    sMenu.windowId = windowId;
    sMenu.fontId = fontId;
    sMenu.optionHeight = cursorHeight;
    sMenu.APressMuted = APressMuted;

    pos = initialCursorPos;

    if (pos < 0 || pos > sMenu.maxCursorPos)
        sMenu.cursorPos = 0;
    else
        sMenu.cursorPos = pos;

    return Menu_MoveCursor(0);
}

u8 InitMenuInUpperLeftCornerPlaySoundWhenAPressed(u8 windowId, u8 itemCount, u8 initialCursorPos)
{
    InitMenuInUpperLeftCorner(windowId, 2, 0, 2, 16, itemCount, initialCursorPos, FALSE);
}

void RedrawMenuCursor(u8 oldPos, u8 newPos)
{
    u8 width = GetMenuCursorDimensionByFont(sMenu.fontId, 0);
    u8 height = GetMenuCursorDimensionByFont(sMenu.fontId, 1);

    FillWindowPixelRect(sMenu.windowId, PIXEL_FILL(1), sMenu.left, sMenu.optionHeight * oldPos + sMenu.top, width, height);
    AddTextPrinterParameterized(sMenu.windowId, sMenu.fontId, gText_SelectorArrow3, sMenu.left, sMenu.optionHeight * newPos + sMenu.top, 0, 0);
}

u8 Menu_MoveCursor(s8 cursorDelta)
{
    u8 oldPos = sMenu.cursorPos;
    int newPos = sMenu.cursorPos + cursorDelta;

    if (newPos < sMenu.minCursorPos)
        sMenu.cursorPos = sMenu.maxCursorPos;
    else if (newPos > sMenu.maxCursorPos)
        sMenu.cursorPos = sMenu.minCursorPos;
    else
        sMenu.cursorPos += cursorDelta;

    RedrawMenuCursor(oldPos, sMenu.cursorPos);
    return sMenu.cursorPos;
}

u8 Menu_MoveCursorNoWrapAround(s8 cursorDelta)
{
    u8 oldPos = sMenu.cursorPos;
    int newPos = sMenu.cursorPos + cursorDelta;

    if (newPos < sMenu.minCursorPos)
        sMenu.cursorPos = sMenu.minCursorPos;
    else if (newPos > sMenu.maxCursorPos)
        sMenu.cursorPos = sMenu.maxCursorPos;
    else
        sMenu.cursorPos += cursorDelta;

    RedrawMenuCursor(oldPos, sMenu.cursorPos);
    return sMenu.cursorPos;
}

u8 Menu_GetCursorPos(void)
{
    return sMenu.cursorPos;
}

s8 Menu_ProcessInput(void)
{
    if (JOY_NEW(A_BUTTON))
    {
        if (!sMenu.APressMuted)
            PlaySE(SE_SELECT);
        return sMenu.cursorPos;
    }
    else if (JOY_NEW(B_BUTTON))
        return MENU_B_PRESSED;
    else if (JOY_NEW(DPAD_UP))
    {
        PlaySE(SE_SELECT);
        Menu_MoveCursor(-1);
        return MENU_NOTHING_CHOSEN;
    }
    else if (JOY_NEW(DPAD_DOWN))
    {
        PlaySE(SE_SELECT);
        Menu_MoveCursor(1);
        return MENU_NOTHING_CHOSEN;
    }
    return MENU_NOTHING_CHOSEN;
}

s8 Menu_ProcessInputNoWrap(void)
{
    u8 oldPos = sMenu.cursorPos;

    if (JOY_NEW(A_BUTTON))
    {
        if (!sMenu.APressMuted)
            PlaySE(SE_SELECT);
        return sMenu.cursorPos;
    }
    else if (JOY_NEW(B_BUTTON))
        return MENU_B_PRESSED;
    else if (JOY_NEW(DPAD_UP))
    {
        if (oldPos != Menu_MoveCursorNoWrapAround(-1))
            PlaySE(SE_SELECT);
        return MENU_NOTHING_CHOSEN;
    }
    else if (JOY_NEW(DPAD_DOWN))
    {
        if (oldPos != Menu_MoveCursorNoWrapAround(1))
            PlaySE(SE_SELECT);
        return MENU_NOTHING_CHOSEN;
    }
    return MENU_NOTHING_CHOSEN;
}

s8 ProcessMenuInput_other(void)
{
    if (JOY_NEW(A_BUTTON))
    {
        if (!sMenu.APressMuted)
            PlaySE(SE_SELECT);
        return sMenu.cursorPos;
    }
    else if (JOY_NEW(B_BUTTON))
        return MENU_B_PRESSED;
    else if (JOY_REPEAT(DPAD_ANY) == DPAD_UP)
    {
        PlaySE(SE_SELECT);
        Menu_MoveCursor(-1);
        return MENU_NOTHING_CHOSEN;
    }
    else if (JOY_REPEAT(DPAD_ANY) == DPAD_DOWN)
    {
        PlaySE(SE_SELECT);
        Menu_MoveCursor(1);
        return MENU_NOTHING_CHOSEN;
    }
    return MENU_NOTHING_CHOSEN;
}

s8 Menu_ProcessInputNoWrapAround_other(void)
{
    u8 oldPos = sMenu.cursorPos;

    if (JOY_NEW(A_BUTTON))
    {
        if (!sMenu.APressMuted)
            PlaySE(SE_SELECT);
        return sMenu.cursorPos;
    }
    if (JOY_NEW(B_BUTTON))
        return MENU_B_PRESSED;
    if (JOY_REPEAT(DPAD_ANY) == DPAD_UP)
    {
        if (oldPos != Menu_MoveCursorNoWrapAround(-1))
            PlaySE(SE_SELECT);
        return MENU_NOTHING_CHOSEN;
    }
    if (JOY_REPEAT(DPAD_ANY) == DPAD_DOWN)
    {
        if (oldPos != Menu_MoveCursorNoWrapAround(1))
            PlaySE(SE_SELECT);
        return MENU_NOTHING_CHOSEN;
    }
    return MENU_NOTHING_CHOSEN;
}

void PrintTextArray(u8 windowId, u8 fontId, u8 left, u8 top, u8 lineHeight, u8 itemCount, const struct MenuAction *strs)
{
    u32 i;

    for (i = 0; i < itemCount; i++)
        AddTextPrinterParameterized(windowId, fontId, strs[i].text, left, (lineHeight * i) + top, 0xFF, NULL);
    CopyWindowToVram(windowId, 2);
}

void UnionRoomAndTradeMenuPrintOptions(u8 windowId, u8 fontId, u8 lineHeight, u8 itemCount, const struct MenuAction *strs)
{
    PrintTextArray(windowId, fontId, GetMenuCursorDimensionByFont(fontId, 0), 0, lineHeight, itemCount, strs);
}

void MultichoiceList_PrintItems(u8 windowId, u8 fontId, u8 left, u8 top, u8 lineHeight, u8 itemCount, const struct MenuAction *strs, u8 letterSpacing, u8 lineSpacing)
{
    u32 i;

    for (i = 0; i < itemCount; i++)
        AddTextPrinterParameterized5(windowId, fontId, strs[i].text, left, (lineHeight * i) + top, 0xFF, NULL, letterSpacing, lineSpacing);
    CopyWindowToVram(windowId, 2);
}

void AddItemMenuActionTextPrinters(u8 windowId, u8 fontId, u8 left, u8 top, u8 letterSpacing, u8 lineHeight, u8 itemCount, const struct MenuAction *strs, const u8 *a8)
{
    u32 i;
    struct TextPrinterTemplate printer;

    printer.windowId = windowId;
    printer.fontId = fontId;
    printer.fgColor = GetFontAttribute(fontId, FONTATTR_COLOR_FOREGROUND);
    printer.bgColor = GetFontAttribute(fontId, FONTATTR_COLOR_BACKGROUND);
    printer.shadowColor = GetFontAttribute(fontId, FONTATTR_COLOR_SHADOW);
    printer.unk = GetFontAttribute(fontId, FONTATTR_UNKNOWN);
    printer.letterSpacing = letterSpacing;
    printer.lineSpacing = GetFontAttribute(fontId, FONTATTR_LINE_SPACING);
    printer.x = left;
    printer.currentX = left;

    for (i = 0; i < itemCount; i++)
    {
        printer.currentChar = strs[a8[i]].text;
        printer.y = (lineHeight * i) + top;
        printer.currentY = printer.y;
        AddTextPrinter(&printer, 0xFF, NULL);
    }
    CopyWindowToVram(windowId, 2);
}

void SetWindowTemplateFields(struct WindowTemplate *template, u8 bg, u8 left, u8 top, u8 width, u8 height, u8 paletteNum, u16 baseBlock)
{
    template->bg = bg;
    template->tilemapLeft = left;
    template->tilemapTop = top;
    template->width = width;
    template->height = height;
    template->paletteNum = paletteNum;
    template->baseBlock = baseBlock;
}

struct WindowTemplate CreateWindowTemplate(u8 bg, u8 left, u8 top, u8 width, u8 height, u8 paletteNum, u16 baseBlock)
{
    struct WindowTemplate template;
    SetWindowTemplateFields(&template, bg, left, top, width, height, paletteNum, baseBlock);
    return template;
}

u16 sub_8198AA4(u8 bg, u8 left, u8 top, u8 width, u8 height, u8 paletteNum, u16 baseBlock)
{
    struct WindowTemplate template;
    SetWindowTemplateFields(&template, bg, left, top, width, height, paletteNum, baseBlock);
    return AddWindow(&template);
}

void CreateYesNoMenu(const struct WindowTemplate *window, u8 fontId, u8 left, u8 top, u16 baseTileNum, u8 paletteNum, u8 initialCursorPos)
{
    struct TextPrinterTemplate printer;

    sYesNoWindowId = AddWindow(window);
    DrawStdFrameWithCustomTileAndPalette(sYesNoWindowId, TRUE, baseTileNum, paletteNum);

    printer.currentChar = gText_YesNo;
    printer.windowId = sYesNoWindowId;
    printer.fontId = fontId;
    printer.x = GetMenuCursorDimensionByFont(fontId, FONTATTR_MAX_LETTER_WIDTH) + left;
    printer.y = top;
    printer.currentX = printer.x;
    printer.currentY = printer.y;
    printer.fgColor = GetFontAttribute(fontId, FONTATTR_COLOR_FOREGROUND);
    printer.bgColor = GetFontAttribute(fontId, FONTATTR_COLOR_BACKGROUND);
    printer.shadowColor = GetFontAttribute(fontId, FONTATTR_COLOR_SHADOW);
    printer.unk = GetFontAttribute(fontId, FONTATTR_UNKNOWN);
    printer.letterSpacing = GetFontAttribute(fontId, FONTATTR_LETTER_SPACING);
    printer.lineSpacing = GetFontAttribute(fontId, FONTATTR_LINE_SPACING);

    AddTextPrinter(&printer, 0xFF, NULL);
    InitMenuInUpperLeftCorner(sYesNoWindowId, fontId, left, top, GetFontAttribute(fontId, FONTATTR_MAX_LETTER_HEIGHT) + printer.lineSpacing, 2, initialCursorPos, 0);
}

s8 Menu_ProcessInputNoWrapClearOnChoose(void)
{
    s8 result = Menu_ProcessInputNoWrap();
    if (result != MENU_NOTHING_CHOSEN)
        sub_8198C78();
    return result;
}

void sub_8198C78(void)
{
    ClearStdWindowAndFrameToTransparent(sYesNoWindowId, TRUE);
    RemoveWindow(sYesNoWindowId);
}

void PrintMenuActionGrid(u8 windowId, u8 fontId, u8 left, u8 top, u8 optionWidth, u8 horizontalCount, u8 verticalCount, const struct MenuAction *strs, const u8 *strIds)
{
    u32 i, j;
    struct TextPrinterTemplate printer;

    printer.windowId = windowId;
    printer.fontId = fontId;
    printer.fgColor = GetFontAttribute(fontId, FONTATTR_COLOR_FOREGROUND);
    printer.bgColor = GetFontAttribute(fontId, FONTATTR_COLOR_BACKGROUND);
    printer.shadowColor = GetFontAttribute(fontId, FONTATTR_COLOR_SHADOW);
    printer.unk = GetFontAttribute(fontId, FONTATTR_UNKNOWN);
    printer.letterSpacing = GetFontAttribute(fontId, FONTATTR_LETTER_SPACING);
    printer.lineSpacing = GetFontAttribute(fontId, FONTATTR_LINE_SPACING);

    for (i = 0; i < verticalCount; i++)
    {
        for (j = 0; j < horizontalCount; j++)
        {
            printer.currentChar = strs[strIds[(horizontalCount * i) + j]].text;
            printer.x = (optionWidth * j) + left;
            printer.y = (GetFontAttribute(fontId, FONTATTR_MAX_LETTER_HEIGHT) * i) + top;
            printer.currentX = printer.x;
            printer.currentY = printer.y;
            AddTextPrinter(&printer, 0xFF, NULL);
        }
    }

    CopyWindowToVram(windowId, 2);
}

u8 MultichoiceGrid_InitCursorInternal(u8 windowId, u8 fontId, u8 left, u8 top, u8 optionWidth, u8 cursorHeight, u8 cols, u8 rows, u8 numChoices, u8 cursorPos)
{
    s32 pos;

    sMenu.left = left;
    sMenu.top = top;
    sMenu.minCursorPos = 0;
    sMenu.maxCursorPos = numChoices - 1;
    sMenu.windowId = windowId;
    sMenu.fontId = fontId;
    sMenu.optionWidth = optionWidth;
    sMenu.optionHeight = cursorHeight;
    sMenu.columns = cols;
    sMenu.rows = rows;
    pos = cursorPos;

    if (pos < 0 || pos > sMenu.maxCursorPos)
        sMenu.cursorPos = 0;
    else
        sMenu.cursorPos = pos;
    ChangeListMenuCursorPosition(0, 0);
    return sMenu.cursorPos;
}

static void MultichoiceGrid_RedrawCursor(u8 oldCursorPos, u8 newCursorPos)
{
    u8 cursorWidth = GetMenuCursorDimensionByFont(sMenu.fontId, 0);
    u8 cursorHeight = GetMenuCursorDimensionByFont(sMenu.fontId, 1);
    u8 xPos = (oldCursorPos % sMenu.columns) * sMenu.optionWidth + sMenu.left;
    u8 yPos = (oldCursorPos / sMenu.columns) * sMenu.optionHeight + sMenu.top;

    FillWindowPixelRect(sMenu.windowId, PIXEL_FILL(1), xPos, yPos, cursorWidth, cursorHeight);
    xPos = (newCursorPos % sMenu.columns) * sMenu.optionWidth + sMenu.left;
    yPos = (newCursorPos / sMenu.columns) * sMenu.optionHeight + sMenu.top;
    AddTextPrinterParameterized(sMenu.windowId, sMenu.fontId, gText_SelectorArrow3, xPos, yPos, 0, 0);
}

u8 ChangeListMenuCursorPosition(s8 deltaX, s8 deltaY)
{
    u8 oldPos = sMenu.cursorPos;

    if (deltaX)
    {
        if ((sMenu.cursorPos % sMenu.columns) + deltaX < 0)
            sMenu.cursorPos += sMenu.columns - 1;
        else if ((sMenu.cursorPos % sMenu.columns) + deltaX >= sMenu.columns)
            sMenu.cursorPos = (sMenu.cursorPos / sMenu.columns) * sMenu.columns;
        else
            sMenu.cursorPos += deltaX;
    }

    if (deltaY)
    {
        if ((sMenu.cursorPos / sMenu.columns) + deltaY < 0)
            sMenu.cursorPos += sMenu.columns * (sMenu.rows - 1);
        else if ((sMenu.cursorPos / sMenu.columns) + deltaY >= sMenu.rows)
            sMenu.cursorPos -= sMenu.columns * (sMenu.rows - 1);
        else
            sMenu.cursorPos += (sMenu.columns * deltaY);
    }

    if (sMenu.cursorPos > sMenu.maxCursorPos)
    {
        sMenu.cursorPos = oldPos;
        return sMenu.cursorPos;
    }
    else
    {
        MultichoiceGrid_RedrawCursor(oldPos, sMenu.cursorPos);
        return sMenu.cursorPos;
    }
}

u8 ChangeGridMenuCursorPosition(s8 deltaX, s8 deltaY)
{
    u8 oldPos = sMenu.cursorPos;

    if (deltaX)
    {
        if (((sMenu.cursorPos % sMenu.columns) + deltaX >= 0) &&
        ((sMenu.cursorPos % sMenu.columns) + deltaX < sMenu.columns))
            sMenu.cursorPos += deltaX;
    }

    if (deltaY)
    {
        if (((sMenu.cursorPos / sMenu.columns) + deltaY >= 0) &&
        ((sMenu.cursorPos / sMenu.columns) + deltaY < sMenu.rows))
            sMenu.cursorPos += (sMenu.columns * deltaY);
    }

    if (sMenu.cursorPos > sMenu.maxCursorPos)
    {
        sMenu.cursorPos = oldPos;
        return sMenu.cursorPos;
    }
    else
    {
        MultichoiceGrid_RedrawCursor(oldPos, sMenu.cursorPos);
        return sMenu.cursorPos;
    }
}

s8 Menu_ProcessInputGridLayout(void)
{
    u8 oldPos = sMenu.cursorPos;

    if (JOY_NEW(A_BUTTON))
    {
        PlaySE(SE_SELECT);
        return sMenu.cursorPos;
    }
    else if (JOY_NEW(B_BUTTON))
        return MENU_B_PRESSED;
    else if (JOY_NEW(DPAD_UP))
    {
        if (oldPos != ChangeGridMenuCursorPosition(0, -1))
            PlaySE(SE_SELECT);
        return MENU_NOTHING_CHOSEN;
    }
    else if (JOY_NEW(DPAD_DOWN))
    {
        if (oldPos != ChangeGridMenuCursorPosition(0, 1))
            PlaySE(SE_SELECT);
        return MENU_NOTHING_CHOSEN;
    }
    else if (JOY_NEW(DPAD_LEFT) || GetLRKeysPressed() == MENU_L_PRESSED)
    {
        if (oldPos != ChangeGridMenuCursorPosition(-1, 0))
            PlaySE(SE_SELECT);
        return MENU_NOTHING_CHOSEN;
    }
    else if (JOY_NEW(DPAD_RIGHT) || GetLRKeysPressed() == MENU_R_PRESSED)
    {
        if (oldPos != ChangeGridMenuCursorPosition(1, 0))
            PlaySE(SE_SELECT);
        return MENU_NOTHING_CHOSEN;
    }
    return MENU_NOTHING_CHOSEN;
}

void PrintMenuTable(u8 windowId, u8 itemCount, const struct MenuAction *strs)
{
    u32 i;

    for (i = 0; i < itemCount; i++)
        AddTextPrinterParameterized(windowId, 2, strs[i].text, 8, (i * 14) + 1, 0xFF, NULL);
    CopyWindowToVram(windowId, 2);
}

void sub_81995E4(u8 windowId, u8 itemCount, const struct MenuAction *strs, const u8 *a8)
{
    u32 i;
    struct TextPrinterTemplate printer;

    printer.windowId = windowId;
    printer.fontId = 2;
    printer.fgColor = GetFontAttribute(2, FONTATTR_COLOR_FOREGROUND);
    printer.bgColor = GetFontAttribute(2, FONTATTR_COLOR_BACKGROUND);
    printer.shadowColor = GetFontAttribute(2, FONTATTR_COLOR_SHADOW);
    printer.unk = GetFontAttribute(2, FONTATTR_UNKNOWN);
    printer.letterSpacing = 0;
    printer.lineSpacing = 0;
    printer.x = 8;
    printer.currentX = 8;

    for (i = 0; i < itemCount; i++)
    {
        printer.currentChar = strs[a8[i]].text;
        printer.y = (i * 16) + 1;
        printer.currentY = (i * 16) + 1;
        AddTextPrinter(&printer, 0xFF, NULL);
    }
    CopyWindowToVram(windowId, 2);
}

void PrintMenuGridTable(u8 windowId, u8 optionWidth, u8 columns, u8 rows, const struct MenuAction *strs)
{
    u32 i, j;

    for (i = 0; i < rows; i++)
        for (j = 0; j < columns; j++)
            AddTextPrinterParameterized(windowId, 2, strs[(i * columns) + j].text, (optionWidth * j) + 8, (i * 16) + 1, 0xFF, NULL);
    CopyWindowToVram(windowId, 2);
}

u8 InitMenuActionGrid(u8 windowId, u8 optionWidth, u8 columns, u8 rows, u8 initialCursorPos)
{
    s32 pos;

    sMenu.left = 0;
    sMenu.top = 1;
    sMenu.minCursorPos = 0;
    sMenu.maxCursorPos = (columns * rows) - 1;
    sMenu.windowId = windowId;
    sMenu.fontId = 2;
    sMenu.optionWidth = optionWidth;
    sMenu.optionHeight = 16;
    sMenu.columns = columns;
    sMenu.rows = rows;

    pos = initialCursorPos;

    if (pos < 0 || pos > sMenu.maxCursorPos)
        sMenu.cursorPos = 0;
    else
        sMenu.cursorPos = pos;

    return sMenu.cursorPos;
}

void sub_8199DF0(u32 bg, u8 a1, int a2, int a3)
{
    int temp = (!GetBgAttribute(bg, BG_ATTR_PALETTEMODE)) ? 0x20 : 0x40;
    void *addr = (void *)((GetBgAttribute(bg, BG_ATTR_CHARBASEINDEX) * 0x4000) + (GetBgAttribute(bg, BG_ATTR_BASETILE) + a2) * temp);
    RequestDma3Fill(a1 << 24 | a1 << 16 | a1 << 8 | a1, addr + VRAM, a3 * temp, 1);
}

// menu2.c in FRLG
void AddTextPrinterParameterized3(u8 windowId, u8 fontId, u8 left, u8 top, const u8 *color, s8 speed, const u8 *str)
{
    struct TextPrinterTemplate printer;

    printer.currentChar = str;
    printer.windowId = windowId;
    printer.fontId = fontId;
    printer.x = left;
    printer.y = top;
    printer.currentX = printer.x;
    printer.currentY = printer.y;
    printer.letterSpacing = GetFontAttribute(fontId, 2);
    printer.lineSpacing = GetFontAttribute(fontId, 3);
    printer.unk = 0;
    printer.fgColor = color[1];
    printer.bgColor = color[0];
    printer.shadowColor = color[2];
    AddTextPrinter(&printer, speed, NULL);
}

void AddTextPrinterParameterized4(u8 windowId, u8 fontId, u8 left, u8 top, u8 letterSpacing, u8 lineSpacing, const u8 *color, s8 speed, const u8 *str)
{
    struct TextPrinterTemplate printer;

    printer.currentChar = str;
    printer.windowId = windowId;
    printer.fontId = fontId;
    printer.x = left;
    printer.y = top;
    printer.currentX = printer.x;
    printer.currentY = printer.y;
    printer.letterSpacing = letterSpacing;
    printer.lineSpacing = lineSpacing;
    printer.unk = 0;
    printer.fgColor = color[1];
    printer.bgColor = color[0];
    printer.shadowColor = color[2];
    AddTextPrinter(&printer, speed, NULL);
}

void AddTextPrinterParameterized5(u8 windowId, u8 fontId, const u8 *str, u8 left, u8 top, u8 speed, void (*callback)(struct TextPrinterTemplate *, u16), u8 letterSpacing, u8 lineSpacing)
{
    struct TextPrinterTemplate printer;

    printer.currentChar = str;
    printer.windowId = windowId;
    printer.fontId = fontId;
    printer.x = left;
    printer.y = top;
    printer.currentX = left;
    printer.currentY = top;
    printer.letterSpacing = letterSpacing;
    printer.lineSpacing = lineSpacing;
    printer.unk = 0;
    printer.fgColor = GetFontAttribute(fontId, 5);
    printer.bgColor = GetFontAttribute(fontId, 6);
    printer.shadowColor = GetFontAttribute(fontId, 7);
    AddTextPrinter(&printer, speed, callback);
}

void PrintPlayerNameOnWindow(u8 windowId, const u8 *src, u16 x, u16 y)
{
    int count = 0;
    while (gSaveBlock2Ptr->playerName[count] != EOS)
        count++;

    StringExpandPlaceholders(gStringVar4, src);

    AddTextPrinterParameterized(windowId, 2, gStringVar4, x, y, 0xFF, 0);
}
