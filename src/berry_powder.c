#include "global.h"
#include "berry_powder.h"
#include "bg.h"
#include "event_data.h"
#include "load_save.h"
#include "menu.h"
#include "string_util.h"
#include "strings.h"
#include "text.h"
#include "text_window.h"
#include "window.h"

#define MAX_BERRY_POWDER 99999

static EWRAM_DATA u8 sBerryPowderVendorWindowId = 0;

static u32 DecryptBerryPowder(u32 *powder)
{
    return *powder ^ gSaveBlock2Ptr->encryptionKey;
}

void SetBerryPowder(u32 *powder, u32 amount)
{
    *powder = amount ^ gSaveBlock2Ptr->encryptionKey;
}

void ApplyNewEncryptionKeyToBerryPowder(u32 encryptionKey)
{
    u32 *powder = &gSaveBlock2Ptr->berryCrush.berryPowderAmount;
    ApplyNewEncryptionKeyToWord(powder, encryptionKey);
}

static bool8 HasEnoughBerryPowder_(u32 cost)
{
    u32 *powder = &gSaveBlock2Ptr->berryCrush.berryPowderAmount;
    if (DecryptBerryPowder(powder) < cost)
        return FALSE;
    else
        return TRUE;
}

bool8 HasEnoughBerryPowder(void)
{
    u32 *powder = &gSaveBlock2Ptr->berryCrush.berryPowderAmount;
    if (DecryptBerryPowder(powder) < gSpecialVar_0x8004)
        return FALSE;
    else
        return TRUE;
}

bool8 GiveBerryPowder(u32 amountToAdd)
{
    u32 *powder = &gSaveBlock2Ptr->berryCrush.berryPowderAmount;
    u32 amount = DecryptBerryPowder(powder) + amountToAdd;
    if (amount > MAX_BERRY_POWDER)
    {
        SetBerryPowder(powder, MAX_BERRY_POWDER);
        return FALSE;
    }
    else
    {
        SetBerryPowder(powder, amount);
        return TRUE;
    }
}

static bool8 TakeBerryPowder_(u32 cost)
{
    u32 *powder = &gSaveBlock2Ptr->berryCrush.berryPowderAmount;
    if (!HasEnoughBerryPowder_(cost))
        return FALSE;

    SetBerryPowder(powder, DecryptBerryPowder(powder) - cost);
    return TRUE;
}

bool8 TakeBerryPowder(void)
{
    u32 *powder = &gSaveBlock2Ptr->berryCrush.berryPowderAmount;
    if (!HasEnoughBerryPowder_(gSpecialVar_0x8004))
        return FALSE;

    SetBerryPowder(powder, DecryptBerryPowder(powder) - gSpecialVar_0x8004);
    return TRUE;
}

u32 GetBerryPowder(void)
{
    return DecryptBerryPowder(&gSaveBlock2Ptr->berryCrush.berryPowderAmount);
}

static void PrintBerryPowderAmount(u8 windowId, int amount, u8 x, u8 y, u8 speed)
{
    ConvertIntToDecimalStringN(gStringVar1, amount, STR_CONV_MODE_RIGHT_ALIGN, 5);
    AddTextPrinterParameterized(windowId, 4, gStringVar1, x, y, speed, NULL);
}

static void DrawPlayerPowderAmount(u8 windowId, u16 baseTileOffset, u8 paletteNum, u32 amount)
{
    DrawStdFrameWithCustomTileAndPalette(windowId, FALSE, baseTileOffset, paletteNum);
    AddTextPrinterParameterized(windowId, 4, gText_Powder, 0, 0, TEXT_SPEED_FF, NULL);
    PrintBerryPowderAmount(windowId, amount, 39, 12, 0);
}

void PrintPlayerBerryPowderAmount(void)
{
    PrintBerryPowderAmount(sBerryPowderVendorWindowId, GetBerryPowder(), 26, 17, 0);
}

void DisplayBerryPowderVendorMenu(void)
{
    struct WindowTemplate template;

    SetWindowTemplateFields(&template, 0, 1, 1, 8, 3, 15, 32);
    sBerryPowderVendorWindowId = AddWindow(&template);
    FillWindowPixelBuffer(sBerryPowderVendorWindowId, PIXEL_FILL(0));
    PutWindowTilemap(sBerryPowderVendorWindowId);
    LoadThinWindowBorderGfx(sBerryPowderVendorWindowId, 0x21D, 0xD0);
    DrawPlayerPowderAmount(sBerryPowderVendorWindowId, 0x21D, 13, GetBerryPowder());
}

void RemoveBerryPowderVendorMenu(void)
{
    ClearWindowTilemap(sBerryPowderVendorWindowId);
    ClearStdWindowAndFrameToTransparent(sBerryPowderVendorWindowId, TRUE);
    RemoveWindow(sBerryPowderVendorWindowId);
}
