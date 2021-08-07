#include "global.h"
#include "item_menu.h"
#include "battle.h"
#include "battle_controllers.h"
#include "battle_pyramid.h"
#include "frontier_util.h"
#include "battle_pyramid_bag.h"
#include "berry_tag_screen.h"
#include "bg.h"
#include "constants/items.h"
#include "constants/songs.h"
#include "data.h"
#include "decompress.h"
#include "event_data.h"
#include "event_object_movement.h"
#include "field_player_avatar.h"
#include "field_specials.h"
#include "graphics.h"
#include "gpu_regs.h"
#include "international_string_util.h"
#include "item.h"
#include "item_menu_icons.h"
#include "item_use.h"
#include "list_menu.h"
#include "link.h"
#include "mail.h"
#include "main.h"
#include "malloc.h"
#include "map_name_popup.h"
#include "menu.h"
#include "money.h"
#include "overworld.h"
#include "palette.h"
#include "party_menu.h"
#include "player_pc.h"
#include "pokemon.h"
#include "pokemon_summary_screen.h"
#include "scanline_effect.h"
#include "script.h"
#include "shop.h"
#include "sound.h"
#include "sprite.h"
#include "strings.h"
#include "string_util.h"
#include "task.h"
#include "text_window.h"
#include "menu_helpers.h"
#include "window.h"
#include "apprentice.h"
#include "battle_pike.h"
#include "constants/rgb.h"

#define FREE_IF_SET(ptr)            \
{                                   \
    if (ptr)                        \
    {                               \
        Free(ptr);                  \
        ptr = NULL;                 \
    }                               \
}

enum
{
    SWITCH_POCKET_NONE,
    SWITCH_POCKET_LEFT,
    SWITCH_POCKET_RIGHT
};

void GoToBagMenu(u8 bagMenuType, u8 pocketId, void (*postExitMenuMainCallback2)());
static void FadeOutOfBagMenu(void);
static void Task_WaitFadeOutOfBagMenu(u8 taskId);
static void NullBagMenuBufferPtrs(void);
static void CB2_OpenBagMenu(void);
static bool8 LoadBagMenuGraphics(void);
static void BagMenuInitBgsAndAllocTilemapBuffer(void);
static bool8 DoLoadBagGraphics(void);
void InitBagWindows(void);
static bool8 TryAllocListMenuBuffers(void);
static void Bag_BuildListMenuTemplate(u8);
static void PrintBagPocketName(void);
void BagDrawDepositItemTextBox(void);
static void CreatePocketScrollArrowPair(void);
static void CreatePocketSwitchArrowPair(void);
static bool8 BagIsTutorial(void);
static void Task_Bag_WallyTutorialBagMenu(u8);
static void Task_BagMenu_HandleInput(u8);
static void GetItemName(s8*, u16);
u16 ItemIdToBattleMoveId(u16);
u16 BagGetItemIdByPocketPosition(u8, u16);
static void PrintItemDescriptionOnMessageWindow(s32 itemIndex);
static void BagMenu_PrintCursor(u8, u8);
void BagPrintTextOnWindow(u8, u8, const u8*, u8, u8, u8, u8, u8, u8);
bool8 ItemId_GetImportance(u16);
u16 BagGetQuantityByPocketPosition(u8, u16);
static void BagDestroyPocketSwitchArrowPair(void);
static void Task_ItemMenu_WaitFadeAndSwitchToExitCallback(u8);
static void Task_AnimateWin0v(u8 taskId);
u8 OpenBagWindow(u8);
void CloseBagWindow(u8);
static void Task_RedrawArrowsAndReturnToBagMenuSelect(u8);
void PrintItemDepositAmount(u8, s16);
static u8 ShowBagWindow(u8, u8);
static u8 ProcessPocketSwitchInput(void);
static void SwitchPockets(u8, s16, u16);
static bool8 CanSwapItems(void);
static void BeginMovingItemInPocket(u8);
static void Task_AnimateSwitchPockets(u8);
static void Task_MoveItemInPocket_HandleInput(u8);
static void ExecuteMoveItemInPocket(u8, u32);
static void AbortMovingItemInPocket(u8, u32);
static void Task_FieldItemContextMenuHandleInput(u8);
void HideBagWindow(u8);
static void Task_PrintThereIsNoPokemon(u8);
static void Task_SelectQuantityToToss(u8);
static void Task_ConfirmTossItems(u8);
void BagCreateYesNoMenuTopRight(u8 taskId, const struct YesNoFuncTable * ptrs);
static void Task_WaitAB_RedrawAndReturnToBag(u8);
static void Task_WaitAButtonAndCloseContextMenu(u8);
static void Task_PrintItemCantBeHeld(u8);
void BagPrintMoneyAmount(void);
void BagDrawTextBoxOnWindow(u8);
static void Task_PrintSaleConfirmationText(u8);
static void Task_InitSaleQuantitySelectInterface(u8);
static void Task_ShowSellYesNoMenu(u8);
static void RemoveMoneyWindow(void);
static void Task_SelectQuantityToSell(u8);
static void Task_FinalizeSaleToShop(u8);
static void Task_WaitPressAB_AfterSell(u8);
static void Task_TryDoItemDeposit(u8);
static void Task_SelectQuantityToDeposit(u8 taskId);
static void BagMenu_Deposit_WaitForABPress(u8);
static void CB2_ApprenticeExitBagMenu(void);
static void All_CalculateNItemsAndMaxShowed(void);
static void CalculateInitialCursorPosAndItemsAbove(void);
static void UpdatePocketScrollPositions(void);
static u8 CreateBagInputHandlerTask(u8);
static void CopyBagListBgTileRowToTilemapBuffer(u8);
static void BagListMenuMoveCursorFunc(s32 itemIndex, bool8 onInit, struct ListMenu *unused);
static void BagListMenuItemPrintFunc(u8 windowId, u32 itemIndex, u8 y);
static void Task_ItemMenuAction_Use(u8 taskId);
static void Task_ItemMenuAction_Toss(u8 taskId);
static void Task_ItemMenuAction_ToggleSelect(u8 taskId);
static void Task_ItemMenuAction_Give(u8 taskId);
static void Task_ItemMenuAction_Cancel(u8 taskId);
static void Task_ItemMenuAction_BattleUse(u8 taskId);
static void Task_ItemMenuAction_ViewTag(u8 taskId);
void ItemMenu_StartFadeToExitCallback(u8 taskId);
static void Task_Dude_FadeFromBag(u8 taskId);
static void Task_Dude_WaitFadeAndExitBag(u8 taskId);
static void Task_ItemContext_FieldOrBattle(u8 taskId);
static void Task_ItemContext_FieldGive(u8 taskId);
static void Task_ItemContext_Sell(u8 taskId);
static void Task_ItemContext_Deposit(u8 taskId);
static void Task_ItemContext_PcBoxGive(u8 taskId);
static void Task_TossItem_Yes(u8 taskId);
static void Task_TossItem_No(u8 taskId);
static void Task_SellItem_Yes(u8 taskId);
static void Task_SellItem_No(u8 taskId);
static void ShowBagOrBeginWin0OpenTask(void);
static void DestroyBagMenuResources(void);
u8 GetBagWindow(u8 a);
static void Task_ItemContextMenuByLocation(u8 taskId);
static void Bag_FillMessageBoxWithPalette(u32 a0);

// .rodata

static const struct BgTemplate sBgTemplates[] =
{
    {
        .bg = 0,
        .charBaseIndex = 0,
        .mapBaseIndex = 31,
        .screenSize = 0,
        .paletteMode = 0,
        .priority = 0,
        .baseTile = 0,
    },
    {
        .bg = 1,
        .charBaseIndex = 3,
        .mapBaseIndex = 30,
        .screenSize = 0,
        .paletteMode = 0,
        .priority = 1,
        .baseTile = 0,
    }
};

static const u16 sBagListBgTiles[][18] =
{
    INCBIN_U16("graphics/item_menu/bagmap_0.bin"),
    INCBIN_U16("graphics/item_menu/bagmap_1.bin"),
    INCBIN_U16("graphics/item_menu/bagmap_2.bin"),
    INCBIN_U16("graphics/item_menu/bagmap_3.bin"),
    INCBIN_U16("graphics/item_menu/bagmap_4.bin"),
    INCBIN_U16("graphics/item_menu/bagmap_5.bin"),
    INCBIN_U16("graphics/item_menu/bagmap_6.bin"),
    INCBIN_U16("graphics/item_menu/bagmap_7.bin"),
    INCBIN_U16("graphics/item_menu/bagmap_8.bin"),
    INCBIN_U16("graphics/item_menu/bagmap_9.bin"),
    INCBIN_U16("graphics/item_menu/bagmap_A.bin"),
    INCBIN_U16("graphics/item_menu/bagmap_B.bin")
};

static const struct MenuAction sItemMenuActions[] =
{
    [ITEMMENUACTION_USE] =          {gMenuText_Use, Task_ItemMenuAction_Use},
    [ITEMMENUACTION_TOSS] =         {gMenuText_Toss, Task_ItemMenuAction_Toss},
    [ITEMMENUACTION_REGISTER] =     {gMenuText_Register, Task_ItemMenuAction_ToggleSelect},
    [ITEMMENUACTION_GIVE] =         {gMenuText_Give, Task_ItemMenuAction_Give},
    [ITEMMENUACTION_CANCEL] =       {gText_Cancel, Task_ItemMenuAction_Cancel},
    [ITEMMENUACTION_BATTLE_USE] =   {gMenuText_Use, Task_ItemMenuAction_BattleUse},
    [ITEMMENUACTION_CHECK] =        {gMenuText_Check, Task_ItemMenuAction_Use},
    [ITEMMENUACTION_WALK] =         {gMenuText_Walk, Task_ItemMenuAction_Use},
    [ITEMMENUACTION_DESELECT] =     {gMenuText_Deselect, Task_ItemMenuAction_ToggleSelect},
    [ITEMMENUACTION_VIEW_TAG] =    {gMenuText_CheckTag, Task_ItemMenuAction_ViewTag},
    [ITEMMENUACTION_CONFIRM] =      {gMenuText_Confirm, ItemMenu_StartFadeToExitCallback},
    [ITEMMENUACTION_SHOW] =         {gText_ExpandedPlaceholder_Empty, NULL},
    [ITEMMENUACTION_DUMMY] =        {gText_ExpandedPlaceholder_Empty, NULL}
};

// ITEMMENUACTION_DUMMY is used to represent blank spaces
static const u8 sContextMenuItems_Field[][5] =
{
    {
        ITEMMENUACTION_USE,
        ITEMMENUACTION_GIVE,
        ITEMMENUACTION_TOSS,
        ITEMMENUACTION_CANCEL,
        ITEMMENUACTION_DUMMY
    },
    {
        ITEMMENUACTION_GIVE,
        ITEMMENUACTION_TOSS,
        ITEMMENUACTION_CANCEL,
        ITEMMENUACTION_DUMMY,
        ITEMMENUACTION_DUMMY
    },
    {
        ITEMMENUACTION_USE,
        ITEMMENUACTION_GIVE,
        ITEMMENUACTION_CANCEL,
        ITEMMENUACTION_DUMMY,
        ITEMMENUACTION_DUMMY
    },
    {
        ITEMMENUACTION_VIEW_TAG,
        ITEMMENUACTION_USE,
        ITEMMENUACTION_GIVE,
        ITEMMENUACTION_TOSS,
        ITEMMENUACTION_CANCEL
    },
    {
        ITEMMENUACTION_USE,
        ITEMMENUACTION_REGISTER,
        ITEMMENUACTION_CANCEL,
        ITEMMENUACTION_DUMMY,
        ITEMMENUACTION_DUMMY
    }
};

static const u8 sContextMenuItems_CheckGiveTossCancel[] =
{
    ITEMMENUACTION_CHECK,
    ITEMMENUACTION_GIVE,
    ITEMMENUACTION_TOSS,
    ITEMMENUACTION_CANCEL
};

static const u8 sContextMenuItems_TossCancel[] =
{
    ITEMMENUACTION_TOSS,
    ITEMMENUACTION_CANCEL
};

static const u8 sContextMenuItems_BattleUse[] =
{
    ITEMMENUACTION_BATTLE_USE,
    ITEMMENUACTION_CANCEL
};

static const u8 sContextMenuItems_Give[] =
{
    ITEMMENUACTION_GIVE,
    ITEMMENUACTION_CANCEL
};

static const u8 sContextMenuItems_Cancel[] =
{
    ITEMMENUACTION_CANCEL
};

static const u8 sContextMenuItems_BerryBlenderCrush[] =
{
    ITEMMENUACTION_CONFIRM,
    ITEMMENUACTION_VIEW_TAG,
    ITEMMENUACTION_CANCEL
};

static const u8 sContextMenuItems_Apprentice[] =
{
    ITEMMENUACTION_SHOW,
    ITEMMENUACTION_CANCEL
};

static const TaskFunc sItemContextTaskFuncs[] =
{
    [ITEMMENULOCATION_FIELD] =                  Task_ItemContext_FieldOrBattle,
    [ITEMMENULOCATION_BATTLE] =                 Task_ItemContext_FieldOrBattle,
    [ITEMMENULOCATION_PARTY] =                  Task_ItemContext_FieldGive,
    [ITEMMENULOCATION_SHOP] =                   Task_ItemContext_Sell,
    [ITEMMENULOCATION_BERRY_TREE] =             ItemMenu_StartFadeToExitCallback,
    [ITEMMENULOCATION_BERRY_BLENDER_CRUSH] =    Task_ItemContext_FieldOrBattle,
    [ITEMMENULOCATION_ITEMPC] =                 Task_ItemContext_Deposit,
    [ITEMMENULOCATION_WALLY] =                  NULL,
    [ITEMMENULOCATION_PCBOX] =                  Task_ItemContext_PcBoxGive
};

static const struct YesNoFuncTable sYesNoTossFunctions =
{
    Task_TossItem_Yes,
    Task_TossItem_No
};

static const struct YesNoFuncTable sYesNoSellItemFunctions =
{
    Task_SellItem_Yes,
    Task_SellItem_No
};

// .text

struct TempWallyStruct
{
    struct ItemSlot bagPocket_Items[BAG_ITEMS_COUNT];
    struct ItemSlot bagPocket_PokeBalls[BAG_POKEBALLS_COUNT];
    u16 cursorPosition[POCKETS_COUNT];
    u16 scrollPosition[POCKETS_COUNT];
    u8 filler[2];
    u16 pocket;
};

EWRAM_DATA struct BagStruct gBagPositionStruct = {};
EWRAM_DATA struct BagMenuStruct *gBagMenu = NULL;
static EWRAM_DATA struct ListMenuItem * sListMenuItems = NULL;
static EWRAM_DATA u8 (*sListMenuItemStrings)[24] = NULL;
EWRAM_DATA u16 gSpecialVar_ItemId = ITEM_NONE;
static EWRAM_DATA struct TempWallyStruct *sTempWallyBag = 0;

extern u8 *const gPocketNamesStringsTable[];
extern const u8 EventScript_SelectWithoutRegisteredItem[];
extern const u16 gUnknown_0860F074[];

static const struct ScrollArrowsTemplate sBagScrollArrowsTemplate =
{
    .firstArrowType = SCROLL_ARROW_LEFT,
    .firstX = 8,
    .firstY = 72,
    .secondArrowType = SCROLL_ARROW_RIGHT,
    .secondX = 72,
    .secondY = 72,
    .fullyUpThreshold = 0,
    .fullyDownThreshold = 2,
    .tileTag = 111,
    .palTag = 111,
    .palNum = 0,
};

static const u8 sRegisteredSelect_Gfx[] = INCBIN_U8("graphics/item_menu/select_button.4bpp");

void GoToBagMenu(u8 location, u8 pocket, void ( *postExitMenuMainCallback2)())
{
    NullBagMenuBufferPtrs();
    gBagMenu = AllocZeroed(sizeof(struct BagMenuStruct));
    if (gBagMenu)
    {
        if (location != ITEMMENULOCATION_LAST)
            gBagPositionStruct.location = location;
        if (postExitMenuMainCallback2)
            gBagPositionStruct.bagCallback = postExitMenuMainCallback2;
        gBagMenu->exitCallback = NULL;
        gBagMenu->itemOriginalLocation = 0xFF;
        gBagMenu->itemIconSlot = 0;
        gBagMenu->inhibitItemDescriptionPrint = FALSE;
        gBagMenu->pocketScrollArrowsTask = 0xFF;
        gBagMenu->pocketSwitchArrowsTask = 0xFF;
        if (gBagPositionStruct.location == ITEMMENULOCATION_BERRY_TREE
         || gBagPositionStruct.location == ITEMMENULOCATION_BERRY_BLENDER_CRUSH)
            gBagMenu->pocketSwitchDisabled = TRUE;
        else
            gBagMenu->pocketSwitchDisabled = FALSE;
        if (pocket < POCKETS_COUNT)
            gBagPositionStruct.pocket = pocket;
        gSpecialVar_ItemId = ITEM_NONE;
        SetMainCallback2(CB2_OpenBagMenu);
    }
    else
        SetMainCallback2(postExitMenuMainCallback2);
}

void CB2_BagMenuFromStartMenu(void)
{
    GoToBagMenu(ITEMMENULOCATION_FIELD, POCKETS_COUNT, CB2_ReturnToFieldWithOpenMenu);
}

void CB2_BagMenuFromBattle(void)
{
    if (InBattlePyramid())
        GoToBattlePyramidBagMenu(PYRAMIDBAG_LOC_BATTLE, SetCB2ToReshowScreenAfterMenu);
    else
        GoToBagMenu(ITEMMENULOCATION_BATTLE, POCKETS_COUNT, SetCB2ToReshowScreenAfterMenu);
}

static void CB2_BagMenuRun(void)
{
    RunTasks();
    AnimateSprites();
    BuildOamBuffer();
    DoScheduledBgTilemapCopiesToVram();
    UpdatePaletteFade();
}

static void VBlankCB_BagMenuRun(void)
{
    LoadOam();
    ProcessSpriteCopyRequests();
    TransferPlttBuffer();
}

#define tItemCount data[8]

static void CB2_OpenBagMenu(void)
{
    while (!MenuHelpers_CallLinkSomething() && !LoadBagMenuGraphics() && !MenuHelpers_LinkSomething()) {};
}

static bool8 LoadBagMenuGraphics(void)
{
    u8 taskId;

    switch (gMain.state)
    {
    case 0:
        SetVBlankHBlankCallbacksToNull();
        ClearScheduledBgCopiesToVram();
        gMain.state++;
        break;
    case 1:
        ScanlineEffect_Stop();
        gMain.state++;
        break;
    case 2:
        FreeAllSpritePalettes();
        gMain.state++;
        break;
    case 3:
        ResetPaletteFade();
        gPaletteFade.bufferTransferDisabled = TRUE;
        gMain.state++;
        break;
    case 4:
        ResetSpriteData();
        gMain.state++;
        break;
    case 5:
        ResetItemMenuIconState();
        gMain.state++;
        break;
    case 6:
        if (!MenuHelpers_LinkSomething())
            ResetTasks();
        gMain.state++;
        break;
    case 7:
        BagMenuInitBgsAndAllocTilemapBuffer();
        gBagMenu->graphicsLoadState = 0;
        gMain.state++;
        break;
    case 8:
        if (DoLoadBagGraphics())
            gMain.state++;
        break;
    case 9:
        InitBagWindows();
        gMain.state++;
        break;
    case 10:
        All_CalculateNItemsAndMaxShowed();
        CalculateInitialCursorPosAndItemsAbove();
        UpdatePocketScrollPositions();
        gMain.state++;
        break;
    case 11:
        if (!TryAllocListMenuBuffers())
        {
            FadeOutOfBagMenu();
            return TRUE;
        }
        gMain.state++;
        break;
    case 12:
        Bag_BuildListMenuTemplate(gBagPositionStruct.pocket);
        gMain.state++;
        break;
    case 13:
        PrintBagPocketName();
        gMain.state++;
        break;
    case 14:
        taskId = CreateBagInputHandlerTask(gBagPositionStruct.location);
        gTasks[taskId].data[0] = ListMenuInit(&gMultiuseListMenuTemplate, gBagPositionStruct.scrollPosition[gBagPositionStruct.pocket], gBagPositionStruct.cursorPosition[gBagPositionStruct.pocket]);
        gTasks[taskId].data[3] = 0;
        gTasks[taskId].tItemCount = 0;
        gMain.state++;
        break;
    case 15:
        AddBagVisualSprite(gBagPositionStruct.pocket);
        gMain.state++;
        break;
    case 16:
        CreatePocketScrollArrowPair();
        CreatePocketSwitchArrowPair();
        gMain.state++;
        break;
    case 17:
        CreateItemMenuSwapLine();
        gMain.state++;
        break;
    case 18:
        ShowBagOrBeginWin0OpenTask();
        gMain.state++;
        break;
    case 19:
        gPaletteFade.bufferTransferDisabled = FALSE;
        gMain.state++;
        break;
    default:
        SetVBlankCallback(VBlankCB_BagMenuRun);
        SetMainCallback2(CB2_BagMenuRun);
        return TRUE;
    }
    return FALSE;
}

static void FadeOutOfBagMenu(void)
{
    BeginNormalPaletteFade(PALETTES_ALL, -2, 0, 16, RGB_BLACK);
    CreateTask(Task_WaitFadeOutOfBagMenu, 0);
    SetVBlankCallback(VBlankCB_BagMenuRun);
    SetMainCallback2(CB2_BagMenuRun);
}

static void Task_WaitFadeOutOfBagMenu(u8 taskId)
{
    if (!gPaletteFade.active)
    {
        SetMainCallback2(gBagPositionStruct.bagCallback);
        DestroyBagMenuResources();
        DestroyTask(taskId);
    }
}

static void NullBagMenuBufferPtrs(void)
{
    gBagMenu = NULL;
    sListMenuItems = NULL;
    sListMenuItemStrings = NULL;
}

static void BagMenuInitBgsAndAllocTilemapBuffer(void)
{
    ResetVramOamAndBgCntRegs();
    ResetAllBgsCoordinates();
    memset(gBagMenu->tilemapBuffer, 0, 0x800);
    ResetBgsAndClearDma3BusyFlags(FALSE);
    InitBgsFromTemplates(0, sBgTemplates, ARRAY_COUNT(sBgTemplates));
    SetBgTilemapBuffer(1, gBagMenu->tilemapBuffer);
    ScheduleBgCopyTilemapToVram(1);
    SetGpuReg(REG_OFFSET_DISPCNT, DISPCNT_OBJ_1D_MAP | DISPCNT_OBJ_ON | DISPCNT_WIN0_ON);
    ShowBg(0);
    ShowBg(1);
    SetGpuReg(REG_OFFSET_BLDCNT, 0);
}

static bool8 DoLoadBagGraphics(void)
{
    switch (gBagMenu->graphicsLoadState)
    {
    case 0:
        ResetTempTileDataBuffers();
        DecompressAndCopyTileDataToVram(1, gBagScreen_Gfx, 0, 0, 0);
        gBagMenu->graphicsLoadState++;
        break;
    case 1:
        if (!FreeTempTileDataBuffersIfPossible())
        {
            LZDecompressWram(gBagScreen_GfxTileMap, gBagMenu->tilemapBuffer);
            gBagMenu->graphicsLoadState++;
        }
        break;
    case 2:
        LoadCompressedPalette(gBagScreenMale_Pal, 0, 0x60);
        if (!BagIsTutorial() && gSaveBlock2Ptr->playerGender)
            LoadCompressedPalette(gBagScreenFemale_Pal, 0, 0x20);
        gBagMenu->graphicsLoadState++;
        break;
    case 3:
        if (BagIsTutorial() || !gSaveBlock2Ptr->playerGender)
            LoadCompressedSpriteSheet(&gBagMaleSpriteSheet);
        else
            LoadCompressedSpriteSheet(&gBagFemaleSpriteSheet);
        gBagMenu->graphicsLoadState++;
        break;
    case 4:
        LoadCompressedSpritePalette(&gBagPaletteTable);
        gBagMenu->graphicsLoadState++;
        break;
    default:
        LoadListMenuArrowsGfx();
        gBagMenu->graphicsLoadState = 0;
        return TRUE;
    }
    return FALSE;
}

static u8 CreateBagInputHandlerTask(u8 location)
{
    u8 taskId;
    if (location == ITEMMENULOCATION_WALLY)
        taskId = CreateTask(Task_Bag_WallyTutorialBagMenu, 0);
    else
        taskId = CreateTask(Task_BagMenu_HandleInput, 0);
    return taskId;
}

static bool8 TryAllocListMenuBuffers(void)
{
    // The items pocket has the highest capacity, + 1 for CANCEL
    sListMenuItems = Alloc((BAG_ITEMS_COUNT + 1) * sizeof(struct ListMenuItem));
    if (!sListMenuItems)
        return FALSE;
    sListMenuItemStrings = Alloc((BAG_ITEMS_COUNT + 1) * sizeof(*sListMenuItemStrings));
    if (!sListMenuItemStrings)
        return FALSE;
    return TRUE;
}

static void Bag_BuildListMenuTemplate(u8 pocketId)
{
    u32 i;
    struct BagPocket *pocket = &gBagPockets[pocketId];

    for (i = 0; i < gBagMenu->numItemStacks[pocketId]; i++)
    {
        GetItemName(sListMenuItemStrings[i], pocket->itemSlots[i].itemId);
        sListMenuItems[i].name = sListMenuItemStrings[i];
        sListMenuItems[i].id = i;
    }
    StringCopy(sListMenuItemStrings[i], gText_CloseBag);
    sListMenuItems[i].name = sListMenuItemStrings[i];
    sListMenuItems[i].id = i;
    gMultiuseListMenuTemplate.items = sListMenuItems;
    gMultiuseListMenuTemplate.totalItems = gBagMenu->numItemStacks[pocketId] + 1;
    gMultiuseListMenuTemplate.windowId = 0;
    gMultiuseListMenuTemplate.header_X = 0;
    gMultiuseListMenuTemplate.item_X = 9;
    gMultiuseListMenuTemplate.cursor_X = 1;
    gMultiuseListMenuTemplate.lettersSpacing = 0;
    gMultiuseListMenuTemplate.itemVerticalPadding = 2;
    gMultiuseListMenuTemplate.upText_Y = 2;
    gMultiuseListMenuTemplate.maxShowed = gBagMenu->numShownItems[pocketId];
    gMultiuseListMenuTemplate.fontId = 2;
    gMultiuseListMenuTemplate.cursorPal = 2;
    gMultiuseListMenuTemplate.fillValue = 0;
    gMultiuseListMenuTemplate.cursorShadowPal = 3;
    gMultiuseListMenuTemplate.moveCursorFunc = BagListMenuMoveCursorFunc;
    gMultiuseListMenuTemplate.itemPrintFunc = BagListMenuItemPrintFunc;
    gMultiuseListMenuTemplate.cursorKind = 0;
    gMultiuseListMenuTemplate.scrollMultiple = 0;
}

static void GetItemName(s8 *dest, u16 itemId)
{
    switch (gBagPositionStruct.pocket)
    {
        case TMHM_POCKET:
            StringCopy(gStringVar2, gMoveNames[ItemIdToBattleMoveId(itemId)]);
            if (itemId >= ITEM_HM01)
            {
                ConvertIntToDecimalStringN(gStringVar1, itemId - ITEM_HM01 + 1, STR_CONV_MODE_LEADING_ZEROS, 1);
                StringExpandPlaceholders(dest, gText_ClearTo11Var1Clear5Var2);
            }
            else
            {
                ConvertIntToDecimalStringN(gStringVar1, itemId - ITEM_TM01 + 1, STR_CONV_MODE_LEADING_ZEROS, 2);
                StringExpandPlaceholders(dest, gText_NumberVar1Clear7Var2);
            }
            break;
        case BERRIES_POCKET:
            ConvertIntToDecimalStringN(gStringVar1, itemId - ITEM_CHERI_BERRY + 1, STR_CONV_MODE_LEADING_ZEROS, 2);
            CopyItemName(itemId, gStringVar2);
            StringExpandPlaceholders(dest, gText_NumberVar1Clear7Var2);
            break;
        default:
            CopyItemName(itemId, dest);
            break;
    }
}

static void BagListMenuMoveCursorFunc(s32 itemIndex, bool8 onInit, struct ListMenu *unused)
{
    if (!onInit)
    {
        PlaySE(SE_RG_BAG_CURSOR);
        ShakeBagSprite();
    }
    if (gBagMenu->itemOriginalLocation == 0xFF)
    {
        // Hide current icon, remove icon from two instances ago
        HideBagItemIconSprite(gBagMenu->itemIconSlot ^ 1);
        RemoveBagItemIconSprite(gBagMenu->itemIconSlot);
        if (gBagMenu->numItemStacks[gBagPositionStruct.pocket] != itemIndex)
           AddBagItemIconSprite(BagGetItemIdByPocketPosition(gBagPositionStruct.pocket + 1, itemIndex), gBagMenu->itemIconSlot);
        else
           AddBagItemIconSprite(-1, gBagMenu->itemIconSlot);
        gBagMenu->itemIconSlot ^= 1;
        if (!gBagMenu->inhibitItemDescriptionPrint)
            PrintItemDescriptionOnMessageWindow(itemIndex);
    }
}

static void BagListMenuItemPrintFunc(u8 windowId, u32 itemIndex, u8 y)
{
    u16 itemId, itemQuantity;

    if (gBagMenu->itemOriginalLocation != 0xFF)
    {
        if (gBagMenu->itemOriginalLocation == (u8)itemIndex)
            BagMenu_PrintCursor(y, 2);
        else
            BagMenu_PrintCursor(y, -1);
    }
    if (itemIndex != LIST_CANCEL && gBagMenu->numItemStacks[gBagPositionStruct.pocket] != itemIndex)
    {
        itemId = BagGetItemIdByPocketPosition(gBagPositionStruct.pocket + 1, itemIndex);
        itemQuantity = BagGetQuantityByPocketPosition(gBagPositionStruct.pocket + 1, itemIndex);

        if (itemId >= ITEM_HM01 && itemId <= ITEM_HM08)
            BlitBitmapToWindow(windowId, gBagMenuHMIcon_Gfx, 9, y, 16, 12);

        if (gBagPositionStruct.pocket == BERRIES_POCKET || (gBagPositionStruct.pocket != KEYITEMS_POCKET && ItemId_GetImportance(itemId) < 2))
        {
            ConvertIntToDecimalStringN(gStringVar1, itemQuantity, STR_CONV_MODE_RIGHT_ALIGN, 3);
            StringExpandPlaceholders(gStringVar4, gText_xVar1);
            BagPrintTextOnWindow(windowId, 4, gStringVar4, 110, y, 0, 0, -1, 1);
        }
        else if (gSaveBlock1Ptr->registeredItem && gSaveBlock1Ptr->registeredItem == itemId)
            BlitBitmapToWindow(windowId, sRegisteredSelect_Gfx, 0x70, y, 0x18, 0x10);
    }
}

static void BagMenu_PrintCursor_(u8 listTaskId, u8 colorIndex)
{
    BagMenu_PrintCursor(ListMenuGetYCoordForPrintingArrowCursor(listTaskId), colorIndex);
}

static void BagMenu_PrintCursor(u8 y, u8 colorIndex)
{
    if (colorIndex == 0xFF)
        FillWindowPixelRect(0, PIXEL_FILL(0), 1, y, GetMenuCursorDimensionByFont(2, 0), GetMenuCursorDimensionByFont(2, 1));
    else
        BagPrintTextOnWindow(0, 2, gText_SelectorArrow2, 1, y, 0, 0, 0, colorIndex);
}

static void PrintBagPocketName(void)
{
    u32 x;

    FillWindowPixelBuffer(2, PIXEL_FILL(0));
    x = GetStringCenterAlignXOffset(2, gPocketNamesStringsTable[gBagPositionStruct.pocket], 0x48);
    BagPrintTextOnWindow(2, 2, gPocketNamesStringsTable[gBagPositionStruct.pocket], x, 1, GetFontAttribute(2, FONTATTR_LETTER_SPACING), GetFontAttribute(2, FONTATTR_LINE_SPACING), 0, 0);
}

static void PrintItemDescriptionOnMessageWindow(s32 itemIndex)
{
    const u8 *description;

    if (itemIndex != gBagMenu->numItemStacks[gBagPositionStruct.pocket])
        description = ItemId_GetDescription(BagGetItemIdByPocketPosition(gBagPositionStruct.pocket + 1, itemIndex));
    else
    {
        StringCopy(gStringVar1, gBagMenu_ReturnToStrings[gBagPositionStruct.location]);
        StringExpandPlaceholders(gStringVar4, gText_ReturnToVar1);
        description = gStringVar4;
    }
    FillWindowPixelBuffer(1, PIXEL_FILL(0));
    BagPrintTextOnWindow(1, 2, description, 0, 3, 2, 0, 0, 0);
}

static void CreatePocketScrollArrowPair(void)
{
    gBagMenu->pocketScrollArrowsTask = AddScrollIndicatorArrowPairParameterized(
        SCROLL_ARROW_UP,
        160,
        8,
        104,
        gBagMenu->numItemStacks[gBagPositionStruct.pocket] - gBagMenu->numShownItems[gBagPositionStruct.pocket] + 1,
        110,
        110,
        &gBagPositionStruct.scrollPosition[gBagPositionStruct.pocket]
    );
}

static void CreatePocketSwitchArrowPair(void)
{
    if (!gBagMenu->pocketSwitchDisabled)
    {
        gBagMenu->contextMenuSelectedItem = 1;
        gBagMenu->pocketSwitchArrowsTask = AddScrollIndicatorArrowPair(&sBagScrollArrowsTemplate, &gBagMenu->contextMenuSelectedItem);
    }
}

static void CreatePocketScrollArrowPair_SellQuantity(void)
{
    gBagMenu->contextMenuSelectedItem = 1;
    gBagMenu->pocketScrollArrowsTask = AddScrollIndicatorArrowPairParameterized(
        SCROLL_ARROW_UP,
        152,
        72,
        104,
        2,
        110,
        110,
        &gBagMenu->contextMenuSelectedItem
    );
}

static void CreateArrowPair_QuantitySelect(void)
{
    gBagMenu->contextMenuSelectedItem = 1;
    gBagMenu->pocketScrollArrowsTask = AddScrollIndicatorArrowPairParameterized(
        SCROLL_ARROW_UP,
        212,
        120,
        152,
        2,
        110,
        110,
        &gBagMenu->contextMenuSelectedItem
    );
}

static void BagDestroyPocketScrollArrowPair(void)
{
    if (gBagMenu->pocketScrollArrowsTask != 0xFF)
    {
        RemoveScrollIndicatorArrowPair(gBagMenu->pocketScrollArrowsTask);
        gBagMenu->pocketScrollArrowsTask = 0xFF;
    }
    BagDestroyPocketSwitchArrowPair();
}

static void BagDestroyPocketSwitchArrowPair(void)
{
    if (gBagMenu->pocketSwitchArrowsTask != 0xFF)
    {
        RemoveScrollIndicatorArrowPair(gBagMenu->pocketSwitchArrowsTask);
        gBagMenu->pocketSwitchArrowsTask = 0xFF;
    }
}

void ResetBagCursorPositions(void)
{
    gBagPositionStruct.pocket = ITEMS_POCKET;
    memset(gBagPositionStruct.cursorPosition, 0, 10);
    memset(gBagPositionStruct.scrollPosition, 0, 10);
}

void PocketCalculateInitialCursorPosAndItemsAbove(u8 pocketId)
{
    sub_812225C(&gBagPositionStruct.scrollPosition[pocketId], &gBagPositionStruct.cursorPosition[pocketId], gBagMenu->numShownItems[pocketId], gBagMenu->numItemStacks[pocketId] + 1);
}

static void CalculateInitialCursorPosAndItemsAbove(void)
{
    u32 i;

    for (i = 0; i < POCKETS_COUNT; i++)
        PocketCalculateInitialCursorPosAndItemsAbove(i);
}

static void UpdatePocketScrollPositions(void)
{
    u32 i, j;

    for (i = 0; i < POCKETS_COUNT; i++)
    {
        if (gBagPositionStruct.cursorPosition[i] > 3)
        {
            for (j = 0; j <= gBagPositionStruct.cursorPosition[i] - 3; gBagPositionStruct.cursorPosition[i]--, gBagPositionStruct.scrollPosition[i]++, j++)
            {
                if (gBagPositionStruct.scrollPosition[i] + gBagMenu->numShownItems[i] == gBagMenu->numItemStacks[i] + 1)
                    break;
            }
        }
    }
}

static void DestroyBagMenuResources(void)
{
    FREE_IF_SET(gBagMenu);
    FREE_IF_SET(sListMenuItems);
    FREE_IF_SET(sListMenuItemStrings);
    FreeAllWindowBuffers();
}

void ItemMenu_StartFadeToExitCallback(u8 taskId)
{
    BeginNormalPaletteFade(PALETTES_ALL, -2, 0, 16, RGB_BLACK);
    gTasks[taskId].func = Task_ItemMenu_WaitFadeAndSwitchToExitCallback;
}

static void Task_ItemMenu_WaitFadeAndSwitchToExitCallback(u8 taskId)
{
    s16* data = gTasks[taskId].data;
    if (!gPaletteFade.active && !FuncIsActiveTask(Task_AnimateWin0v))
    {
        DestroyListMenuTask(data[0], &gBagPositionStruct.scrollPosition[gBagPositionStruct.pocket], &gBagPositionStruct.cursorPosition[gBagPositionStruct.pocket]);
        if (gBagMenu->exitCallback)
            SetMainCallback2(gBagMenu->exitCallback);
        else
            SetMainCallback2(gBagPositionStruct.bagCallback);
        BagDestroyPocketScrollArrowPair();
        DestroyBagMenuResources();
        DestroyTask(taskId);
    }
}

static void ShowBagOrBeginWin0OpenTask(void)
{
    u16 paldata = RGB_BLACK;
    u8 taskId;

    LoadPalette(&paldata, 0x00, 0x02);
    SetGpuReg(REG_OFFSET_WININ, 0);
    SetGpuReg(REG_OFFSET_WINOUT, WININ_WIN0_BG_ALL | WININ_WIN0_OBJ | WININ_WIN0_CLR);
    BlendPalettes(PALETTES_ALL, 16, RGB_BLACK);
    BeginNormalPaletteFade(PALETTES_ALL, 0, 16, 0, RGB_BLACK);
    if (gBagPositionStruct.bagOpen)
    {
        SetGpuReg(REG_OFFSET_WIN0H, WIN_RANGE(0, 240));
        SetGpuReg(REG_OFFSET_WIN0V, WIN_RANGE(0, 0));
    }
    else
    {
        SetGpuReg(REG_OFFSET_WIN0H, WIN_RANGE(0, 240));
        SetGpuReg(REG_OFFSET_WIN0V, WIN_RANGE(0, 160));
        taskId = CreateTask(Task_AnimateWin0v, 0);
        gTasks[taskId].data[0] = 192;
        gTasks[taskId].data[1] = -16;
        gBagPositionStruct.bagOpen = TRUE;
    }
}

void Bag_BeginCloseWin0Animation(void)
{

    u8 taskId = CreateTask(Task_AnimateWin0v, 0);
    gTasks[taskId].data[0] = -16;
    gTasks[taskId].data[1] =  16;
    gBagPositionStruct.bagOpen = FALSE;
}

void CB2_SetUpReshowBattleScreenAfterMenu(void)
{
    gBagPositionStruct.bagOpen = FALSE;
}

static void Task_AnimateWin0v(u8 taskId)
{
    s16 *data = gTasks[taskId].data;
    data[0] += data[1];
    if (data[0] > 160)
        SetGpuReg(REG_OFFSET_WIN0V, WIN_RANGE(0, 160));
    else
        SetGpuReg(REG_OFFSET_WIN0V, data[0]);
    if ((data[1] == 16 && data[0] == 160) || (data[1] == -16 && !data[0]))
        DestroyTask(taskId);
}

void MoveItemSlotInList(struct ItemSlot* itemSlots_, u32 from, u32 to_)
{
    // dumb assignments needed to match
    struct ItemSlot *itemSlots = itemSlots_;
    u32 to = to_;

    if (from != to)
    {
        s32 i, count;
        struct ItemSlot firstSlot = itemSlots[from];

        if (to > from)
        {
            to--;
            for (i = from, count = to; i < count; i++)
                itemSlots[i] = itemSlots[i + 1];
        }
        else
        {
            for (i = from, count = to; i > count; i--)
                itemSlots[i] = itemSlots[i - 1];
        }
        itemSlots[to] = firstSlot;
    }
}

void Pocket_CalculateNItemsAndMaxShowed(u8 pocketId)
{
    u32 i;
    struct BagPocket *pocket = &gBagPockets[pocketId];

    switch (pocketId)
    {
        case TMHM_POCKET:
        case BERRIES_POCKET:
            SortBerriesOrTMHMs(pocket);
            break;
        default:
            CompactItemsInBagPocket(pocket);
            break;
    }

    gBagMenu->numItemStacks[pocketId] = 0;

    for (i = 0; i < pocket->capacity && pocket->itemSlots[i].itemId; i++)
        gBagMenu->numItemStacks[pocketId]++;

    if (gBagMenu->numItemStacks[pocketId] + 1 > 6)
        gBagMenu->numShownItems[pocketId] = 6;
    else
        gBagMenu->numShownItems[pocketId] = gBagMenu->numItemStacks[pocketId] + 1;
}

static void All_CalculateNItemsAndMaxShowed(void)
{
    u32 i;

    for (i = 0; i < POCKETS_COUNT; i++)
        Pocket_CalculateNItemsAndMaxShowed(i);
}

void DisplayItemMessageInBag(u8 taskId, u8 fontId, const u8 *str, void (*callback)(u8 taskId))
{
    s16* data = gTasks[taskId].data;

    data[10] = OpenBagWindow(5);
    FillWindowPixelBuffer(data[10], PIXEL_FILL(1));
    DisplayMessageAndContinueTask(taskId, data[10], 0x6D, 0xD, fontId, GetPlayerTextSpeedDelay(), str, callback);
    ScheduleBgCopyTilemapToVram(0);
}

u8 GetSelectedItemIndex(u8 pocketId)
{
    return gBagPositionStruct.cursorPosition[pocketId] + gBagPositionStruct.scrollPosition[pocketId];
}

void PrintItemDepositAmount(u8 windowId, s16 numDeposited)
{
    u8 numDigits = (gBagPositionStruct.pocket == BERRIES_POCKET) ? BERRY_CAPACITY_DIGITS : BAG_ITEM_CAPACITY_DIGITS;
    ConvertIntToDecimalStringN(gStringVar1, numDeposited, STR_CONV_MODE_LEADING_ZEROS, numDigits);
    StringExpandPlaceholders(gStringVar4, gText_xVar1);
    AddTextPrinterParameterized(windowId, 2, gStringVar4, GetStringCenterAlignXOffset(2, gStringVar4, 0x28), 2, 0, 0);
}

void PrintItemSoldAmount(int windowId, int numSold, int moneyEarned)
{
    u8 numDigits = (gBagPositionStruct.pocket == BERRIES_POCKET) ? BERRY_CAPACITY_DIGITS : BAG_ITEM_CAPACITY_DIGITS;
    FillWindowPixelBuffer(windowId, PIXEL_FILL(1));
    ConvertIntToDecimalStringN(gStringVar1, numSold, STR_CONV_MODE_LEADING_ZEROS, numDigits);
    StringExpandPlaceholders(gStringVar4, gText_xVar1);
    BagPrintTextOnWindow(windowId, 4, gStringVar4, 4, 10, 1, 0, -1, 1);
    PrintMoneyAmount(windowId, 56, 10, moneyEarned, 0);
}

static bool8 CanSwapItems(void)
{
    if (gBagPositionStruct.location <= ITEMMENULOCATION_BATTLE)
    {
        u8 temp = gBagPositionStruct.pocket - 2;
        if (temp > 1)
            return TRUE;
    }
    return FALSE;
}

static void Task_BagMenu_HandleInput(u8 taskId)
{
    s16* data = gTasks[taskId].data;
    u16 scrollPos, cursorPos;
    s32 listPosition = ListMenu_ProcessInput(data[0]);

    if (gPaletteFade.active
     || FuncIsActiveTask(Task_AnimateWin0v)
     || MenuHelpers_CallLinkSomething())
        return;

    switch (ProcessPocketSwitchInput())
    {
    case SWITCH_POCKET_LEFT:
        SwitchPockets(taskId, MENU_CURSOR_DELTA_LEFT, 0);
        return;
    case SWITCH_POCKET_RIGHT:
        SwitchPockets(taskId, MENU_CURSOR_DELTA_RIGHT, 0);
        return;
    default:
        if (JOY_NEW(SELECT_BUTTON) && CanSwapItems())
        {
            ListMenuGetScrollAndRow(data[0], &cursorPos, &scrollPos);
            if (cursorPos + scrollPos != gBagMenu->numItemStacks[gBagPositionStruct.pocket])
            {
                PlaySE(SE_SELECT);
                BeginMovingItemInPocket(taskId);
                return;
            }
        }
        break;
    }
    ListMenuGetScrollAndRow(data[0], &gBagPositionStruct.scrollPosition[gBagPositionStruct.pocket], &gBagPositionStruct.cursorPosition[gBagPositionStruct.pocket]);
    switch (listPosition)
    {
    case LIST_CANCEL:
        if (gBagPositionStruct.location == ITEMMENULOCATION_BERRY_BLENDER_CRUSH)
        {
            PlaySE(SE_FAILURE);
            break;
        }
        PlaySE(SE_SELECT);
        gSpecialVar_ItemId = ITEM_NONE;
        Bag_BeginCloseWin0Animation();
        gTasks[taskId].func = ItemMenu_StartFadeToExitCallback;
        break;
    case LIST_NOTHING_CHOSEN:
        return;
    default: // A_BUTTON
        PlaySE(SE_SELECT);
        if (listPosition == gBagMenu->numItemStacks[gBagPositionStruct.pocket])
        {
            gSpecialVar_ItemId = ITEM_NONE;
            Bag_BeginCloseWin0Animation();
            gTasks[taskId].func = ItemMenu_StartFadeToExitCallback;
        }
        else
        {
            BagDestroyPocketScrollArrowPair();
            BagMenu_PrintCursor_(data[0], 2);
            data[1] = listPosition;
            data[2] = BagGetQuantityByPocketPosition(gBagPositionStruct.pocket + 1, listPosition);
            gSpecialVar_ItemId = BagGetItemIdByPocketPosition(gBagPositionStruct.pocket + 1, listPosition);
            gTasks[taskId].func = Task_ItemContextMenuByLocation;
        }
        break;
    }
}

static void Task_ItemContextMenuByLocation(u8 taskId)
{
    Bag_FillMessageBoxWithPalette(1);
    sItemContextTaskFuncs[gBagPositionStruct.location](taskId);
}

static void Task_RedrawArrowsAndReturnToBagMenuSelect(u8 taskId)
{
    Bag_FillMessageBoxWithPalette(0);
    CreatePocketScrollArrowPair();
    CreatePocketSwitchArrowPair();
    gTasks[taskId].func = Task_BagMenu_HandleInput;
}

static void Bag_FillMessageBoxWithPalette(u32 a0)
{
    SetBgTilemapPalette(1, 0, 14, 30, 6, a0 + 1);
    ScheduleBgCopyTilemapToVram(1);
}

static u8 ProcessPocketSwitchInput(void)
{
    u8 LRKeys;
    if (gBagMenu->pocketSwitchDisabled)
        return SWITCH_POCKET_NONE;
    LRKeys = GetLRKeysPressed();
    if (JOY_NEW(DPAD_LEFT) || LRKeys == MENU_L_PRESSED)
    {
        PlaySE(SE_RG_BAG_POCKET);
        return SWITCH_POCKET_LEFT;
    }
    if (JOY_NEW(DPAD_RIGHT) || LRKeys == MENU_R_PRESSED)
    {
        PlaySE(SE_RG_BAG_POCKET);
        return SWITCH_POCKET_RIGHT;
    }
    return SWITCH_POCKET_NONE;
}

static void ChangeBagPocketId(u8 *bagPocketId, s8 deltaBagPocketId)
{
    if (deltaBagPocketId == 1 && *bagPocketId == POCKETS_COUNT - 1)
        *bagPocketId = 0;
    else if (deltaBagPocketId == -1 && *bagPocketId == 0)
        *bagPocketId = POCKETS_COUNT - 1;
    else
        *bagPocketId += deltaBagPocketId;
}

static void SwitchPockets(u8 taskId, s16 deltaBagPocketId, u16 a3)
{
    s16* data = gTasks[taskId].data;
    u8 pocketId = gBagPositionStruct.pocket;

    data[13] = 0;
    data[12] = 0;
    data[11] = deltaBagPocketId;

    if (!a3)
    {
        ClearWindowTilemap(0);
        ClearWindowTilemap(1);
        ClearWindowTilemap(2);
        DestroyListMenuTask(data[0], &gBagPositionStruct.scrollPosition[gBagPositionStruct.pocket], &gBagPositionStruct.cursorPosition[gBagPositionStruct.pocket]);
        ScheduleBgCopyTilemapToVram(0);
        RemoveBagItemIconSprite(gBagMenu->itemIconSlot ^ 1);
        BagDestroyPocketScrollArrowPair();
    }
    FillBgTilemapBufferRect_Palette0(1, 0x02D, 11, 1, 18, 12);
    ScheduleBgCopyTilemapToVram(1);
    ChangeBagPocketId(&pocketId, deltaBagPocketId);
    SetBagVisualPocketId(pocketId);
    SetTaskFuncWithFollowupFunc(taskId, Task_AnimateSwitchPockets, gTasks[taskId].func);
}

static void Task_AnimateSwitchPockets(u8 taskId)
{
    s16* data = gTasks[taskId].data;

    if (!MenuHelpers_LinkSomething() && !BagIsTutorial())
    {
        switch (ProcessPocketSwitchInput())
        {
            case SWITCH_POCKET_LEFT:
                ChangeBagPocketId(&gBagPositionStruct.pocket, data[11]);
                SwitchTaskToFollowupFunc(taskId);
                SwitchPockets(taskId, MENU_CURSOR_DELTA_LEFT, 1);
                return;
            case SWITCH_POCKET_RIGHT:
                ChangeBagPocketId(&gBagPositionStruct.pocket, data[11]);
                SwitchTaskToFollowupFunc(taskId);
                SwitchPockets(taskId, MENU_CURSOR_DELTA_RIGHT, 1);
                return;
        }
    }
    switch (data[13])
    {
        case 0:
            if (data[12] != 0x7FFF)
            {
                data[12]++;
                CopyBagListBgTileRowToTilemapBuffer(data[12]);
                if (data[12] == 12)
                    data[12] = 0x7FFF;
            }
            if (data[12] == 0x7FFF)
                data[13]++;
            break;
        case 1:
            ChangeBagPocketId(&gBagPositionStruct.pocket, data[11]);
            PrintBagPocketName();
            Bag_BuildListMenuTemplate(gBagPositionStruct.pocket);
            data[0] = ListMenuInit(&gMultiuseListMenuTemplate, gBagPositionStruct.scrollPosition[gBagPositionStruct.pocket], gBagPositionStruct.cursorPosition[gBagPositionStruct.pocket]);
            PutWindowTilemap(1);
            PutWindowTilemap(2);
            ScheduleBgCopyTilemapToVram(0);
            CreatePocketScrollArrowPair();
            CreatePocketSwitchArrowPair();
            SwitchTaskToFollowupFunc(taskId);
            break;
    }
}

static void BeginMovingItemInPocket(u8 taskId)
{
    u16 cursorPos, itemsAbove;
    s16* data = gTasks[taskId].data;

    ListMenuGetScrollAndRow(data[0], &cursorPos, &itemsAbove);
    ListMenuSetUnkIndicatorsStructField(data[0], 0x10, 1);
    data[1] = gBagPositionStruct.scrollPosition[gBagPositionStruct.pocket] + gBagPositionStruct.cursorPosition[gBagPositionStruct.pocket];
    gBagMenu->itemOriginalLocation = data[1];
    CopyItemName(BagGetItemIdByPocketPosition(gBagPositionStruct.pocket + 1, data[1]), gStringVar1);
    StringExpandPlaceholders(gStringVar4, gText_MoveVar1Where);
    FillWindowPixelBuffer(1, PIXEL_FILL(0));
    BagPrintTextOnWindow(1, 2, gStringVar4, 0, 3, 2, 0, 0, 0);
    UpdateItemMenuSwapLinePos(0, ListMenuGetYCoordForPrintingArrowCursor(data[0]));
    SetItemMenuSwapLineInvisibility(FALSE);
    BagDestroyPocketSwitchArrowPair();
    BagMenu_PrintCursor_(data[0], 2);
    gTasks[taskId].func = Task_MoveItemInPocket_HandleInput;
}

static void Task_MoveItemInPocket_HandleInput(u8 taskId)
{
    s16* data = gTasks[taskId].data;
    int input;
    u16 itemsAbove, cursorPos;

    if (!MenuHelpers_CallLinkSomething())
    {
        input = ListMenu_ProcessInput(data[0]);
        ListMenuGetScrollAndRow(data[0], &gBagPositionStruct.scrollPosition[gBagPositionStruct.pocket], &gBagPositionStruct.cursorPosition[gBagPositionStruct.pocket]);
        UpdateItemMenuSwapLinePos(0, ListMenuGetYCoordForPrintingArrowCursor(data[0]));
        if (JOY_NEW(SELECT_BUTTON))
        {
            PlaySE(SE_SELECT);
            gBagMenu->itemOriginalLocation = 0xFF;
            ListMenuGetScrollAndRow(data[0], &cursorPos, &itemsAbove);
            ExecuteMoveItemInPocket(taskId, cursorPos + itemsAbove);
            return;
        }
        switch (input)
        {
            case LIST_CANCEL:
                PlaySE(SE_SELECT);
                gBagMenu->itemOriginalLocation = 0xFF;
                ListMenuGetScrollAndRow(data[0], &cursorPos, &itemsAbove);
                AbortMovingItemInPocket(taskId, cursorPos + itemsAbove);
                break;
            case LIST_NOTHING_CHOSEN:
                return;
            default:
                PlaySE(SE_SELECT);
                gBagMenu->itemOriginalLocation = 0xFF;
                ExecuteMoveItemInPocket(taskId, input);
                break;
        }
    }
}

static void ExecuteMoveItemInPocket(u8 taskId, u32 itemIndex)
{
    s16* data = gTasks[taskId].data;
    if (data[1] == itemIndex || data[1] == itemIndex - 1)
        AbortMovingItemInPocket(taskId, itemIndex);
    else
    {
        MoveItemSlotInList(gBagPockets[gBagPositionStruct.pocket].itemSlots, data[1], itemIndex);
        DestroyListMenuTask(data[0], &gBagPositionStruct.cursorPosition[gBagPositionStruct.pocket], &gBagPositionStruct.cursorPosition[gBagPositionStruct.pocket]);
        if (data[1] < itemIndex)
            gBagPositionStruct.cursorPosition[gBagPositionStruct.pocket]--;
        Bag_BuildListMenuTemplate(gBagPositionStruct.pocket);
        data[0] = ListMenuInit(&gMultiuseListMenuTemplate, gBagPositionStruct.cursorPosition[gBagPositionStruct.pocket], gBagPositionStruct.cursorPosition[gBagPositionStruct.pocket]);
        SetItemMenuSwapLineInvisibility(TRUE);
        CreatePocketSwitchArrowPair();
        gTasks[taskId].func = Task_BagMenu_HandleInput;
    }
}

static void AbortMovingItemInPocket(u8 taskId, u32 itemIndex)
{
    s16* data = gTasks[taskId].data;
    DestroyListMenuTask(data[0], &gBagPositionStruct.scrollPosition[gBagPositionStruct.pocket], &gBagPositionStruct.cursorPosition[gBagPositionStruct.pocket]);
    if (data[1] < itemIndex)
        gBagPositionStruct.cursorPosition[gBagPositionStruct.pocket]--;
    Bag_BuildListMenuTemplate(gBagPositionStruct.pocket);
    data[0] = ListMenuInit(&gMultiuseListMenuTemplate, gBagPositionStruct.scrollPosition[gBagPositionStruct.pocket], gBagPositionStruct.cursorPosition[gBagPositionStruct.pocket]);
    SetItemMenuSwapLineInvisibility(TRUE);
    CreatePocketSwitchArrowPair();
    gTasks[taskId].func = Task_BagMenu_HandleInput;
}

static void InitQuantityToTossOrDeposit(u16 cursorPos, const u8 *str)
{
    u8 itemQtyWindowId;
    u8 itemNameWindowId = ShowBagWindow(6, 2);
    CopyItemName(BagGetItemIdByPocketPosition(gBagPositionStruct.pocket + 1, cursorPos), gStringVar1);
    StringExpandPlaceholders(gStringVar4, str);
    BagPrintTextOnWindow(itemNameWindowId, 2, gStringVar4, 0, 2, 1, 0, 0, 1);
    itemQtyWindowId = ShowBagWindow(0, 0);
    ConvertIntToDecimalStringN(gStringVar1, 1, STR_CONV_MODE_LEADING_ZEROS, 3);
    StringExpandPlaceholders(gStringVar4, gText_xVar1);
    BagPrintTextOnWindow(itemQtyWindowId, 4, gStringVar4, 4, 10, 1, 0, 0, 1);
    CreateArrowPair_QuantitySelect();
}

static void UpdateQuantityToTossOrDeposit(s16 value, u8 ndigits)
{
    u8 windowId = GetBagWindow(0);
    FillWindowPixelBuffer(windowId, PIXEL_FILL(1));
    ConvertIntToDecimalStringN(gStringVar1, value, STR_CONV_MODE_LEADING_ZEROS, ndigits);
    StringExpandPlaceholders(gStringVar4, gText_xVar1);
    BagPrintTextOnWindow(windowId, 4, gStringVar4, 4, 10, 1, 0, 0, 1);
}

static void CopyBagListBgTileRowToTilemapBuffer(u8 frame)
{
    CopyToBgTilemapBufferRect(1, sBagListBgTiles[12 - frame], 11, 13 - frame, 18, 1);
    ScheduleBgCopyTilemapToVram(1);
}

static void OpenContextMenu(u8 taskId)
{
    u8 windowId;

    switch (gBagPositionStruct.location)
    {
        case ITEMMENULOCATION_BATTLE:
        case ITEMMENULOCATION_WALLY:
            if (ItemId_GetBattleUsage(gSpecialVar_ItemId))
            {
                gBagMenu->contextMenuItemsPtr = sContextMenuItems_BattleUse;
                gBagMenu->contextMenuNumItems = ARRAY_COUNT(sContextMenuItems_BattleUse);
            }
            else
            {
                gBagMenu->contextMenuItemsPtr = sContextMenuItems_Cancel;
                gBagMenu->contextMenuNumItems = ARRAY_COUNT(sContextMenuItems_Cancel);
            }
            break;
        case ITEMMENULOCATION_BERRY_BLENDER_CRUSH:
            gBagMenu->contextMenuItemsPtr = sContextMenuItems_BerryBlenderCrush;
            gBagMenu->contextMenuNumItems = ARRAY_COUNT(sContextMenuItems_BerryBlenderCrush);
            break;
        case ITEMMENULOCATION_APPRENTICE:
            if (!ItemId_GetImportance(gSpecialVar_ItemId) && gSpecialVar_ItemId != ITEM_ENIGMA_BERRY)
            {
                gBagMenu->contextMenuItemsPtr = sContextMenuItems_Apprentice;
                gBagMenu->contextMenuNumItems = ARRAY_COUNT(sContextMenuItems_Apprentice);
            }
            else
            {
                gBagMenu->contextMenuItemsPtr = sContextMenuItems_Cancel;
                gBagMenu->contextMenuNumItems = ARRAY_COUNT(sContextMenuItems_Cancel);
            }
            break;
        default:
            if (MenuHelpers_LinkSomething() || InUnionRoom())
            {
                if (gBagPositionStruct.pocket == KEYITEMS_POCKET)
                {
                    gBagMenu->contextMenuItemsPtr = sContextMenuItems_Cancel;
                    gBagMenu->contextMenuNumItems = ARRAY_COUNT(sContextMenuItems_Cancel);
                }
                else
                {
                    gBagMenu->contextMenuItemsPtr = sContextMenuItems_Give;
                    gBagMenu->contextMenuNumItems = ARRAY_COUNT(sContextMenuItems_Give);
                }
            }
            else if (ItemId_GetImportance(gSpecialVar_ItemId) == 1)
            {
                gBagMenu->contextMenuItemsPtr = sContextMenuItems_TossCancel;
                gBagMenu->contextMenuNumItems = ARRAY_COUNT(sContextMenuItems_TossCancel);
            }
            else
            {
                switch (gBagPositionStruct.pocket)
                {
                    case ITEMS_POCKET:
                        if (ItemIsMail(gSpecialVar_ItemId))
                        {
                            gBagMenu->contextMenuItemsPtr = sContextMenuItems_CheckGiveTossCancel;
                            gBagMenu->contextMenuNumItems = ARRAY_COUNT(sContextMenuItems_CheckGiveTossCancel);
                        }
                        else
                        {
                            gBagMenu->contextMenuItemsPtr = sContextMenuItems_Field[gBagPositionStruct.pocket];
                            gBagMenu->contextMenuNumItems = ARRAY_COUNT(sContextMenuItems_Field[gBagPositionStruct.pocket]) - 1;
                        }
                        break;
                    case KEYITEMS_POCKET:
                        gBagMenu->contextMenuItemsPtr = gBagMenu->contextMenuItemsBuffer;
                        gBagMenu->contextMenuNumItems = ARRAY_COUNT(sContextMenuItems_Field[gBagPositionStruct.pocket]) - 2;
                        memcpy(&gBagMenu->contextMenuItemsBuffer, &sContextMenuItems_Field[gBagPositionStruct.pocket], sizeof(sContextMenuItems_Field[gBagPositionStruct.pocket] - 2));
                        if (gSaveBlock1Ptr->registeredItem == gSpecialVar_ItemId)
                            gBagMenu->contextMenuItemsBuffer[1] = ITEMMENUACTION_DESELECT;
                        if (gSpecialVar_ItemId == ITEM_BICYCLE && TestPlayerAvatarFlags(PLAYER_AVATAR_FLAG_BIKE))
                            gBagMenu->contextMenuItemsBuffer[0] = ITEMMENUACTION_WALK;
                        break;
                    case BALLS_POCKET:
                    case TMHM_POCKET:
                        gBagMenu->contextMenuItemsPtr = sContextMenuItems_Field[gBagPositionStruct.pocket];
                        gBagMenu->contextMenuNumItems = ARRAY_COUNT(sContextMenuItems_Field[gBagPositionStruct.pocket]) - 2;
                        break;
                    case BERRIES_POCKET:
                        gBagMenu->contextMenuItemsPtr = sContextMenuItems_Field[gBagPositionStruct.pocket];
                        gBagMenu->contextMenuNumItems = ARRAY_COUNT(sContextMenuItems_Field[gBagPositionStruct.pocket]);
                        break;
                }
            }
    }
    windowId = ShowBagWindow(10, gBagMenu->contextMenuNumItems - 1);
    AddItemMenuActionTextPrinters(
        windowId,
        2,
        GetMenuCursorDimensionByFont(2, 0),
        2,
        GetFontAttribute(2, FONTATTR_LETTER_SPACING),
        GetFontAttribute(2, FONTATTR_MAX_LETTER_HEIGHT) + 2,
        gBagMenu->contextMenuNumItems,
        sItemMenuActions,
        gBagMenu->contextMenuItemsPtr
    );
    InitMenuInUpperLeftCorner(windowId, 2, 0, 2, GetFontAttribute(2, FONTATTR_MAX_LETTER_HEIGHT) + 2, gBagMenu->contextMenuNumItems, 0, 0);
    windowId = ShowBagWindow(6, 0);
    CopyItemName(gSpecialVar_ItemId, gStringVar1);
    StringExpandPlaceholders(gStringVar4, gText_Var1IsSelected);
    BagPrintTextOnWindow(windowId, 2, gStringVar4, 0, 2, 1, 0, 0, 1);
}

static void Task_ItemContext_FieldOrBattle(u8 taskId)
{
    OpenContextMenu(taskId);
    gTasks[taskId].func = Task_FieldItemContextMenuHandleInput;
}

static void Task_FieldItemContextMenuHandleInput(u8 taskId)
{
    if (!MenuHelpers_CallLinkSomething())
    {
        s8 selection = Menu_ProcessInputNoWrap();
        switch (selection)
        {
            case MENU_B_PRESSED:
                PlaySE(SE_SELECT);
                sItemMenuActions[ITEMMENUACTION_CANCEL].func.void_u8(taskId);
            case MENU_NOTHING_CHOSEN:
                break;
            default:
                PlaySE(SE_SELECT);
                sItemMenuActions[gBagMenu->contextMenuItemsPtr[selection]].func.void_u8(taskId);
                break;
        }
    }
}

static void Task_ItemMenuAction_Use(u8 taskId)
{
    if (ItemId_GetFieldFunc(gSpecialVar_ItemId))
    {
        HideBagWindow(10);
        HideBagWindow(6);
        PutWindowTilemap(0);
        PutWindowTilemap(1);
        ScheduleBgCopyTilemapToVram(0);
        if (!CalculatePlayerPartyCount() && ItemId_GetType(gSpecialVar_ItemId) == ITEM_USE_PARTY_MENU)
            Task_PrintThereIsNoPokemon(taskId);
        else
        {
            if (gBagPositionStruct.pocket != BERRIES_POCKET)
                ItemId_GetFieldFunc(gSpecialVar_ItemId)(taskId);
            else
                ItemUseOutOfBattle_Berry(taskId);
        }
    }
}

static void Task_ItemMenuAction_Toss(u8 taskId)
{
    s16* data = gTasks[taskId].data;

    ClearWindowTilemap(GetBagWindow(10));
    ClearWindowTilemap(GetBagWindow(6));
    HideBagWindow(10);
    HideBagWindow(6);
    PutWindowTilemap(0);
    tItemCount = 1;
    if (data[2] == 1)
        Task_ConfirmTossItems(taskId);
    else
    {
        InitQuantityToTossOrDeposit(data[1], gText_TossHowManyVar1s);
        gTasks[taskId].func = Task_SelectQuantityToToss;
    }
}

static void Task_ConfirmTossItems(u8 taskId)
{
    s16* data = gTasks[taskId].data;

    CopyItemName(gSpecialVar_ItemId, gStringVar1);
    ConvertIntToDecimalStringN(gStringVar2, tItemCount, STR_CONV_MODE_LEFT_ALIGN, MAX_ITEM_DIGITS);
    StringExpandPlaceholders(gStringVar4, gText_ConfirmTossItems);
    BagPrintTextOnWindow(ShowBagWindow(6, 1), 2, gStringVar4, 0, 2, 1, 0, 0, 1);
    BagCreateYesNoMenuBottomRight(taskId, &sYesNoTossFunctions);
}

static void Task_TossItem_No(u8 taskId)
{
    s16* data = gTasks[taskId].data;

    HideBagWindow(6);
    PutWindowTilemap(1);
    ScheduleBgCopyTilemapToVram(0);
    BagMenu_PrintCursor_(data[0], 1);
    Task_RedrawArrowsAndReturnToBagMenuSelect(taskId);
}

static void Task_SelectQuantityToToss(u8 taskId)
{
    s16* data = gTasks[taskId].data;

    if (AdjustQuantityAccordingToDPadInput(&tItemCount, data[2]))
        UpdateQuantityToTossOrDeposit(tItemCount, 3);
    else if (JOY_NEW(A_BUTTON))
    {
        PlaySE(SE_SELECT);
        ClearWindowTilemap(GetBagWindow(6));
        HideBagWindow(6);
        HideBagWindow(0);
        ScheduleBgCopyTilemapToVram(0);
        BagDestroyPocketScrollArrowPair();
        Task_ConfirmTossItems(taskId);
    }
    else if (JOY_NEW(B_BUTTON))
    {
        PlaySE(SE_SELECT);
        HideBagWindow(6);
        HideBagWindow(0);
        PutWindowTilemap(0);
        PutWindowTilemap(1);
        ScheduleBgCopyTilemapToVram(0);
        BagMenu_PrintCursor_(data[0], 1);
        BagDestroyPocketScrollArrowPair();
        Task_RedrawArrowsAndReturnToBagMenuSelect(taskId);
    }
}

static void Task_TossItem_Yes(u8 taskId)
{
    s16* data = gTasks[taskId].data;

    HideBagWindow(6);
    CopyItemName(gSpecialVar_ItemId, gStringVar1);
    ConvertIntToDecimalStringN(gStringVar2, tItemCount, STR_CONV_MODE_LEFT_ALIGN, 3);
    StringExpandPlaceholders(gStringVar4, gText_ThrewAwayVar2Var1s);
    BagPrintTextOnWindow(ShowBagWindow(6, 3), 2, gStringVar4, 0, 2, 1, 0, 0, 1);
    gTasks[taskId].func = Task_WaitAB_RedrawAndReturnToBag;
}

static void Task_WaitAB_RedrawAndReturnToBag(u8 taskId)
{
    s16* data = gTasks[taskId].data;
    u16* scrollPos = &gBagPositionStruct.scrollPosition[gBagPositionStruct.pocket];
    u16* cursorPos = &gBagPositionStruct.cursorPosition[gBagPositionStruct.pocket];

    if (JOY_NEW(A_BUTTON | B_BUTTON))
    {
        PlaySE(SE_SELECT);
        RemoveBagItem(gSpecialVar_ItemId, tItemCount);
        HideBagWindow(6);
        DestroyListMenuTask(data[0], scrollPos, cursorPos);
        Pocket_CalculateNItemsAndMaxShowed(gBagPositionStruct.pocket);
        PocketCalculateInitialCursorPosAndItemsAbove(gBagPositionStruct.pocket);
        Bag_BuildListMenuTemplate(gBagPositionStruct.pocket);
        data[0] = ListMenuInit(&gMultiuseListMenuTemplate, *scrollPos, *cursorPos);
        PutWindowTilemap(1);
        ScheduleBgCopyTilemapToVram(0);
        BagMenu_PrintCursor_(data[0], 1);
        Task_RedrawArrowsAndReturnToBagMenuSelect(taskId);
    }
}

static void Task_ItemMenuAction_ToggleSelect(u8 taskId)
{
    s16* data = gTasks[taskId].data;
    u16* scrollPos = &gBagPositionStruct.scrollPosition[gBagPositionStruct.pocket];
    u16* cursorPos = &gBagPositionStruct.cursorPosition[gBagPositionStruct.pocket];

    if (gSaveBlock1Ptr->registeredItem == gSpecialVar_ItemId)
        gSaveBlock1Ptr->registeredItem = ITEM_NONE;
    else
        gSaveBlock1Ptr->registeredItem = gSpecialVar_ItemId;

    DestroyListMenuTask(data[0], scrollPos, cursorPos);
    Bag_BuildListMenuTemplate(gBagPositionStruct.pocket);
    data[0] = ListMenuInit(&gMultiuseListMenuTemplate, *scrollPos, *cursorPos);
    CopyWindowToVram(0, 1);
    Task_ItemMenuAction_Cancel(taskId);
}

static void Task_ItemMenuAction_Give(u8 taskId)
{
    HideBagWindow(10);
    HideBagWindow(6);
    PutWindowTilemap(0);
    PutWindowTilemap(1);
    CopyWindowToVram(0, 1);
    if (!IsWritingMailAllowed(gSpecialVar_ItemId))
        DisplayItemMessageInBag(taskId, 2, gText_CantWriteMail, Task_WaitAButtonAndCloseContextMenu);
    else if (!ItemId_GetImportance(gSpecialVar_ItemId))
    {
        if (CalculatePlayerPartyCount())
        {
            gBagMenu->exitCallback = CB2_ChooseMonToGiveItem;
            ItemMenu_StartFadeToExitCallback(taskId);
        }
        else
            Task_PrintThereIsNoPokemon(taskId);
    }
    else
        Task_PrintItemCantBeHeld(taskId);
}

static void Task_PrintThereIsNoPokemon(u8 taskId)
{
    DisplayItemMessageInBag(taskId, 2, gText_NoPokemon, Task_WaitAButtonAndCloseContextMenu);
}

static void Task_PrintItemCantBeHeld(u8 taskId)
{
    CopyItemName(gSpecialVar_ItemId, gStringVar1);
    StringExpandPlaceholders(gStringVar4, gText_Var1CantBeHeld);
    DisplayItemMessageInBag(taskId, 2, gStringVar4, Task_WaitAButtonAndCloseContextMenu);
}

static void Task_WaitAButtonAndCloseContextMenu(u8 taskId)
{
    if (JOY_NEW(A_BUTTON))
    {
        PlaySE(SE_SELECT);
        Task_ReturnToBagFromContextMenu(taskId);
    }
}

void Task_ReturnToBagFromContextMenu(u8 taskId)
{
    s16* data = gTasks[taskId].data;
    u16* scrollPos = &gBagPositionStruct.scrollPosition[gBagPositionStruct.pocket];
    u16* cursorPos = &gBagPositionStruct.cursorPosition[gBagPositionStruct.pocket];

    CloseBagWindow(5);
    DestroyListMenuTask(data[0], scrollPos, cursorPos);
    Pocket_CalculateNItemsAndMaxShowed(gBagPositionStruct.pocket);
    PocketCalculateInitialCursorPosAndItemsAbove(gBagPositionStruct.pocket);
    Bag_BuildListMenuTemplate(gBagPositionStruct.pocket);
    data[0] = ListMenuInit(&gMultiuseListMenuTemplate, *scrollPos, *cursorPos);
    ScheduleBgCopyTilemapToVram(0);
    BagMenu_PrintCursor_(data[0], 1);
    Task_RedrawArrowsAndReturnToBagMenuSelect(taskId);
}

static void Task_ItemMenuAction_ViewTag(u8 taskId)
{
    gBagMenu->exitCallback = DoBerryTagScreen;
    Bag_BeginCloseWin0Animation();
    ItemMenu_StartFadeToExitCallback(taskId);
}

static void Task_ItemMenuAction_Cancel(u8 taskId)
{
    HideBagWindow(10);
    HideBagWindow(6);
    PutWindowTilemap(0);
    PutWindowTilemap(1);
    ScheduleBgCopyTilemapToVram(0);
    BagMenu_PrintCursor_(gTasks[taskId].data[0], 1);
    Task_RedrawArrowsAndReturnToBagMenuSelect(taskId);
}

static void Task_ItemMenuAction_BattleUse(u8 taskId)
{
    if (ItemId_GetBattleFunc(gSpecialVar_ItemId))
    {
        HideBagWindow(10);
        HideBagWindow(6);
        PutWindowTilemap(0);
        PutWindowTilemap(1);
        CopyWindowToVram(0, 1);
        ItemId_GetBattleFunc(gSpecialVar_ItemId)(taskId);
    }
}

static void Task_ItemContext_FieldGive(u8 taskId)
{
    s16* data = gTasks[taskId].data;

    if (!IsWritingMailAllowed(gSpecialVar_ItemId))
        DisplayItemMessageInBag(taskId, 2, gText_CantWriteMail, Task_WaitAButtonAndCloseContextMenu);
    else if (!sub_8122148(gSpecialVar_ItemId))
    {
        CopyItemName(gSpecialVar_ItemId, gStringVar1);
        StringExpandPlaceholders(gStringVar4, gText_Var1CantBeHeldHere);
        DisplayItemMessageInBag(taskId, 2, gStringVar4, Task_WaitAButtonAndCloseContextMenu);
    }
    else if (gBagPositionStruct.pocket != KEYITEMS_POCKET && !ItemId_GetImportance(gSpecialVar_ItemId))
    {
        Bag_BeginCloseWin0Animation();
        gTasks[taskId].func = ItemMenu_StartFadeToExitCallback;
    }
    else
        Task_PrintItemCantBeHeld(taskId);
}

void CB2_ReturnToBagMenuPocket(void)
{
    GoToBagMenu(ITEMMENULOCATION_LAST, POCKETS_COUNT, NULL);
}

// Choosing berry to plant
void CB2_ChooseBerry(void)
{
    GoToBagMenu(ITEMMENULOCATION_BERRY_TREE, BERRIES_POCKET, CB2_ReturnToFieldContinueScript);
}

// Choosing berry for Berry Crush
void ChooseBerryForMachine(void (*exitCallback)(void))
{
    GoToBagMenu(ITEMMENULOCATION_BERRY_BLENDER_CRUSH, BERRIES_POCKET, exitCallback);
}

void CB2_GoToSellMenu(void)
{
    GoToBagMenu(ITEMMENULOCATION_SHOP, POCKETS_COUNT, CB2_ExitSellMenu);
}

void CB2_GoToItemDepositMenu(void)
{
    GoToBagMenu(ITEMMENULOCATION_ITEMPC, POCKETS_COUNT, sub_816B31C);
}

void ApprenticeOpenBagMenu(void)
{
    GoToBagMenu(ITEMMENULOCATION_APPRENTICE, POCKETS_COUNT, CB2_ApprenticeExitBagMenu);
    gSpecialVar_0x8005 = ITEM_NONE;
    gSpecialVar_Result = FALSE;
}

static void Task_ItemContext_PcBoxGive(u8 taskId)
{
    if (ItemIsMail(gSpecialVar_ItemId))
        DisplayItemMessageInBag(taskId, 2, gText_CantWriteMail, Task_WaitAButtonAndCloseContextMenu);
    else if (gBagPositionStruct.pocket != KEYITEMS_POCKET && !ItemId_GetImportance(gSpecialVar_ItemId))
    {
        Bag_BeginCloseWin0Animation();
        gTasks[taskId].func = ItemMenu_StartFadeToExitCallback;
    }
    else
        Task_PrintItemCantBeHeld(taskId);
}

static void Task_ItemContext_Sell(u8 taskId)
{
    s16* data = gTasks[taskId].data;

    if (ItemId_GetPrice(gSpecialVar_ItemId))
    {
        tItemCount = 1;
        if (data[2] == 1)
        {
            BagPrintMoneyAmount();
            Task_PrintSaleConfirmationText(taskId);
        }
        else
        {
            CopyItemName(gSpecialVar_ItemId, gStringVar1);
            StringExpandPlaceholders(gStringVar4, gText_HowManyToSell);
            DisplayItemMessageInBag(taskId, 2, gStringVar4, Task_InitSaleQuantitySelectInterface);
        }
    }
    else
    {
        CopyItemName(gSpecialVar_ItemId, gStringVar1);
        StringExpandPlaceholders(gStringVar4, gText_CantBuyKeyItem);
        DisplayItemMessageInBag(taskId, 2, gStringVar4, Task_ReturnToBagFromContextMenu);
    }
}

static void Task_PrintSaleConfirmationText(u8 taskId)
{
    s16* data = gTasks[taskId].data;

    ConvertIntToDecimalStringN(gStringVar3, (ItemId_GetPrice(gSpecialVar_ItemId) / 2) * tItemCount, STR_CONV_MODE_LEFT_ALIGN, 6);
    StringExpandPlaceholders(gStringVar4, gText_ICanPayVar1);
    DisplayItemMessageInBag(taskId, 2, gStringVar4, Task_ShowSellYesNoMenu);
}

static void Task_ShowSellYesNoMenu(u8 taskId)
{
    BagCreateYesNoMenuTopRight(taskId, &sYesNoSellItemFunctions);
}

static void Task_SellItem_No(u8 taskId)
{
    s16* data = gTasks[taskId].data;
    RemoveMoneyWindow();
    CloseBagWindow(5);
    PutWindowTilemap(2);
    PutWindowTilemap(0);
    PutWindowTilemap(1);
    ScheduleBgCopyTilemapToVram(0);
    BagMenu_PrintCursor_(data[0], 1);
    Task_RedrawArrowsAndReturnToBagMenuSelect(taskId);
}

static void Task_InitSaleQuantitySelectInterface(u8 taskId)
{
    s16* data = gTasks[taskId].data;
    u8 windowId = ShowBagWindow(0, 1);

    PrintItemSoldAmount(windowId, 2, (ItemId_GetPrice(gSpecialVar_ItemId) / 2) * tItemCount);
    BagPrintMoneyAmount();

    gTasks[taskId].func = Task_SelectQuantityToSell;
}

static void Task_SelectQuantityToSell(u8 taskId)
{
    s16* data = gTasks[taskId].data;

    if (AdjustQuantityAccordingToDPadInput(&tItemCount, data[2]))
    {
        PrintItemSoldAmount(GetBagWindow(0), tItemCount, (ItemId_GetPrice(gSpecialVar_ItemId) / 2) * tItemCount);
    }
    else if (JOY_NEW(A_BUTTON))
    {
        PlaySE(SE_SELECT);
        HideBagWindow(0);
        PutWindowTilemap(0);
        ScheduleBgCopyTilemapToVram(0);
        BagDestroyPocketScrollArrowPair();
        Task_PrintSaleConfirmationText(taskId);
    }
    else if (JOY_NEW(B_BUTTON))
    {
        PlaySE(SE_SELECT);
        HideBagWindow(0);
        RemoveMoneyWindow();
        CloseBagWindow(5);
        PutWindowTilemap(2);
        PutWindowTilemap(0);
        PutWindowTilemap(1);
        ScheduleBgCopyTilemapToVram(0);
        BagDestroyPocketScrollArrowPair();
        BagMenu_PrintCursor_(data[0], 1);
        Task_RedrawArrowsAndReturnToBagMenuSelect(taskId);
    }
}

static void Task_SellItem_Yes(u8 taskId)
{
    s16* data = gTasks[taskId].data;

    PutWindowTilemap(0);
    ScheduleBgCopyTilemapToVram(0);
    CopyItemName(gSpecialVar_ItemId, gStringVar1);
    ConvertIntToDecimalStringN(gStringVar3, (ItemId_GetPrice(gSpecialVar_ItemId) / 2) * tItemCount, STR_CONV_MODE_LEFT_ALIGN, 6);
    StringExpandPlaceholders(gStringVar4, gText_TurnedOverVar1ForVar2);
    DisplayItemMessageInBag(taskId, 2, gStringVar4, Task_FinalizeSaleToShop);
}

static void Task_FinalizeSaleToShop(u8 taskId)
{
    s16* data = gTasks[taskId].data;
    u16* scrollPos = &gBagPositionStruct.scrollPosition[gBagPositionStruct.pocket];
    u16* cursorPos = &gBagPositionStruct.cursorPosition[gBagPositionStruct.pocket];

    PlaySE(SE_RG_SHOP);
    RemoveBagItem(gSpecialVar_ItemId, tItemCount);
    AddMoney(&gSaveBlock1Ptr->money, (ItemId_GetPrice(gSpecialVar_ItemId) / 2) * tItemCount);
    DestroyListMenuTask(data[0], scrollPos, cursorPos);
    Pocket_CalculateNItemsAndMaxShowed(gBagPositionStruct.pocket);
    PocketCalculateInitialCursorPosAndItemsAbove(gBagPositionStruct.pocket);
    gBagMenu->inhibitItemDescriptionPrint = TRUE;
    Bag_BuildListMenuTemplate(gBagPositionStruct.pocket);
    data[0] = ListMenuInit(&gMultiuseListMenuTemplate, *scrollPos, *cursorPos);
    BagMenu_PrintCursor_(data[0], 2);
    BagDrawTextBoxOnWindow(GetBagWindow(2));
    PrintMoneyAmountInMoneyBox(GetBagWindow(2), GetMoney(&gSaveBlock1Ptr->money), 0);
    gTasks[taskId].func = Task_WaitPressAB_AfterSell;
}

static void Task_WaitPressAB_AfterSell(u8 taskId)
{
    if (JOY_NEW(A_BUTTON | B_BUTTON))
    {
        PlaySE(SE_SELECT);
        RemoveMoneyWindow();
        PutWindowTilemap(2);
        gBagMenu->inhibitItemDescriptionPrint = FALSE;
        Task_ReturnToBagFromContextMenu(taskId);
    }
}

static void BagMenu_Deposit_WaitForABPress(u8 taskId)
{
    s16* data = gTasks[taskId].data;

    if (JOY_NEW(A_BUTTON | B_BUTTON))
    {
        PlaySE(SE_SELECT);
        Task_ReturnToBagFromContextMenu(taskId);
    }
}

static void CB2_ApprenticeExitBagMenu(void)
{
    gFieldCallback = Apprentice_EnableBothScriptContexts;
    SetMainCallback2(CB2_ReturnToField);
}

static void Task_ItemContext_Deposit(u8 taskId)
{
    s16* data = gTasks[taskId].data;

    tItemCount = 1;
    if (data[2] == 1)
        Task_TryDoItemDeposit(taskId);
    else
    {
        InitQuantityToTossOrDeposit(data[1], gText_DepositHowManyVar1);
        gTasks[taskId].func = Task_SelectQuantityToDeposit;
    }
}

static void Task_SelectQuantityToDeposit(u8 taskId)
{
    s16* data = gTasks[taskId].data;

    if (AdjustQuantityAccordingToDPadInput(&tItemCount, data[2]))
        UpdateQuantityToTossOrDeposit(tItemCount, 3);
    else if (JOY_NEW(A_BUTTON))
    {
        PlaySE(SE_SELECT);
        ClearWindowTilemap(GetBagWindow(6));
        HideBagWindow(6);
        HideBagWindow(0);
        ScheduleBgCopyTilemapToVram(0);
        BagDestroyPocketScrollArrowPair();
        Task_TryDoItemDeposit(taskId);
    }
    else if (JOY_NEW(B_BUTTON))
    {
        PlaySE(SE_SELECT);
        HideBagWindow(6);
        HideBagWindow(0);
        PutWindowTilemap(1);
        ScheduleBgCopyTilemapToVram(0);
        BagMenu_PrintCursor_(data[0], 1);
        BagDestroyPocketScrollArrowPair();
        Task_RedrawArrowsAndReturnToBagMenuSelect(taskId);
    }
}

static void Task_TryDoItemDeposit(u8 taskId)
{
    s16* data = gTasks[taskId].data;

    FillWindowPixelBuffer(1, PIXEL_FILL(0));
    if (ItemId_GetImportance(gSpecialVar_ItemId))
        DisplayItemMessageInBag(taskId, 2, gText_CantStoreImportantItems, BagMenu_Deposit_WaitForABPress);
    else if (AddPCItem(gSpecialVar_ItemId, tItemCount))
    {
        CopyItemName(gSpecialVar_ItemId, gStringVar1);
        ConvertIntToDecimalStringN(gStringVar2, tItemCount, STR_CONV_MODE_LEFT_ALIGN, MAX_ITEM_DIGITS);
        StringExpandPlaceholders(gStringVar4, gText_DepositedVar2Var1s);
        BagPrintTextOnWindow(ShowBagWindow(6, 3), 2, gStringVar4, 0, 2, 1, 0, 0, 1);
        gTasks[taskId].func = Task_WaitAB_RedrawAndReturnToBag;
    }
    else
        DisplayItemMessageInBag(taskId, 2, gText_NoRoomForItems, BagMenu_Deposit_WaitForABPress);
}

#undef tItemCount

#define tUsingRegisteredKeyItem data[3]

bool8 UseRegisteredKeyItemOnField(void)
{
    u8 taskId;

    if (InUnionRoom() || InBattlePyramid() || InBattlePike() || InMultiPartnerRoom())
        return FALSE;
    HideMapNamePopUpWindow();
    ChangeBgY_ScreenOff(0, 0, 0);
    if (gSaveBlock1Ptr->registeredItem != ITEM_NONE)
    {
        if (CheckBagHasItem(gSaveBlock1Ptr->registeredItem, 1))
        {
            ScriptContext2_Enable();
            FreezeObjectEvents();
            PlayerFreeze();
            sub_808BCF4();
            gSpecialVar_ItemId = gSaveBlock1Ptr->registeredItem;
            taskId = CreateTask(ItemId_GetFieldFunc(gSaveBlock1Ptr->registeredItem), 8);
            gTasks[taskId].tUsingRegisteredKeyItem = TRUE;
            return TRUE;
        }
        gSaveBlock1Ptr->registeredItem = ITEM_NONE;
    }
    ScriptContext1_SetupScript(EventScript_SelectWithoutRegisteredItem);
    return TRUE;
}

#undef tUsingRegisteredKeyItem

static bool8 BagIsTutorial(void)
{
    if (gBagPositionStruct.location == 10)
        return TRUE;
    return FALSE;
}

static void BackUpPlayerBag(void)
{
    u32 i;

    sTempWallyBag = AllocZeroed(sizeof(struct TempWallyStruct));
    memcpy(sTempWallyBag->bagPocket_Items, gSaveBlock1Ptr->bagPocket_Items, sizeof(gSaveBlock1Ptr->bagPocket_Items));
    memcpy(sTempWallyBag->bagPocket_PokeBalls, gSaveBlock1Ptr->bagPocket_PokeBalls, sizeof(gSaveBlock1Ptr->bagPocket_PokeBalls));
    sTempWallyBag->pocket = gBagPositionStruct.pocket;
    for (i = 0; i < 3; i++)
    {
        sTempWallyBag->cursorPosition[i] = gBagPositionStruct.cursorPosition[i];
        sTempWallyBag->scrollPosition[i] = gBagPositionStruct.scrollPosition[i];
    }
    ClearItemSlots(gSaveBlock1Ptr->bagPocket_Items, BAG_ITEMS_COUNT);
    ClearItemSlots(gSaveBlock1Ptr->bagPocket_PokeBalls, BAG_POKEBALLS_COUNT);
    ResetBagCursorPositions();
}

static void RestorePlayerBag(void)
{
    u32 i;

    memcpy(gSaveBlock1Ptr->bagPocket_Items, sTempWallyBag->bagPocket_Items, sizeof(sTempWallyBag->bagPocket_Items));
    memcpy(gSaveBlock1Ptr->bagPocket_PokeBalls, sTempWallyBag->bagPocket_PokeBalls, sizeof(sTempWallyBag->bagPocket_PokeBalls));
    gBagPositionStruct.pocket = sTempWallyBag->pocket;
    for (i = 0; i <= 4; i++)
    {
        gBagPositionStruct.cursorPosition[i] = sTempWallyBag->cursorPosition[i];
        gBagPositionStruct.scrollPosition[i] = sTempWallyBag->scrollPosition[i];
    }
    Free(sTempWallyBag);
}

void DoWallyTutorialBagMenu(void)
{
    BackUpPlayerBag();
    AddBagItem(ITEM_POTION, 1);
    AddBagItem(ITEM_POKE_BALL, 1);
    GoToBagMenu(ITEMMENULOCATION_WALLY, ITEMS_POCKET, SetCB2ToReshowScreenAfterMenu);
}

static void Task_Bag_WallyTutorialBagMenu(u8 taskId)
{
    s16* data = gTasks[taskId].data;

    if (!gPaletteFade.active)
    {
        switch (data[8])
        {
            case 0x66:
                PlaySE(SE_RG_BAG_POCKET);
                SwitchPockets(taskId, MENU_CURSOR_DELTA_RIGHT, 0);
                data[8]++;
                break;
            case 0xCC:
                PlaySE(SE_SELECT);
                BagMenu_PrintCursor_(data[0], 2);
                Bag_FillMessageBoxWithPalette(1);
                gSpecialVar_ItemId = ITEM_POKE_BALL;
                OpenContextMenu(taskId);
            default:
                data[8]++;
                break;
            case 0x132:
                PlaySE(SE_SELECT);
                HideBagWindow(10);
                HideBagWindow(6);
                PutWindowTilemap(0);
                PutWindowTilemap(1);
                CopyWindowToVram(0, 1);
                DestroyListMenuTask(data[0], 0, 0);
                RestorePlayerBag();
                Bag_BeginCloseWin0Animation();
                ItemMenu_StartFadeToExitCallback(taskId);
                break;
        }
    }
}

static const u16 sBagWindowPalF[] = INCBIN_U16("graphics/item_menu/bag_window_pal.gbapal");

static const u8 sFontColorTable[][3] =
{
    // bgColor, textColor, shadowColor
    {0, 1, 2},
    {0, 2, 3},
    {0, 3, 2},
    {0, 8, 9}
};

static const struct WindowTemplate sDefaultBagWindows[] =
{
    {
        .bg = 0,
        .tilemapLeft = 0xB,
        .tilemapTop = 0x1,
        .width = 0x12,
        .height = 0xC,
        .paletteNum = 0xF,
        .baseBlock = 0x8A
    },
    {
        .bg = 0,
        .tilemapLeft = 0x5,
        .tilemapTop = 0xE,
        .width = 0x19,
        .height = 0x6,
        .paletteNum = 0xF,
        .baseBlock = 0x162
    },
    {
        .bg = 0,
        .tilemapLeft = 0x1,
        .tilemapTop = 0x1,
        .width = 0x9,
        .height = 0x2,
        .paletteNum = 0xF,
        .baseBlock = 0x1F8
    },
    DUMMY_WIN_TEMPLATE
};

const struct WindowTemplate sContextMenuWindowTemplates[] =
{
    {
        .bg = 0,
        .tilemapLeft = 24,
        .tilemapTop = 15,
        .width = 5,
        .height = 4,
        .paletteNum = 0xF,
        .baseBlock = 0x242
    },
    {
        .bg = 0,
        .tilemapLeft = 17,
        .tilemapTop = 9,
        .width = 12,
        .height = 4,
        .paletteNum = 0xF,
        .baseBlock = 0x242
    },
    {
        .bg = 0,
        .tilemapLeft = 1,
        .tilemapTop = 1,
        .width = 8,
        .height = 3,
        .paletteNum = 0xC,
        .baseBlock = 0x272
    },
    {
        .bg = 0,
        .tilemapLeft = 23,
        .tilemapTop = 15,
        .width = 6,
        .height = 4,
        .paletteNum = 0xF,
        .baseBlock = 0x28a
    },
    {
        .bg = 0,
        .tilemapLeft = 21,
        .tilemapTop = 9,
        .width = 6,
        .height = 4,
        .paletteNum = 0xF,
        .baseBlock = 0x28a
    },
    {
        .bg = 0,
        .tilemapLeft = 2,
        .tilemapTop = 15,
        .width = 26,
        .height = 4,
        .paletteNum = 0xF,
        .baseBlock = 0x2a2
    },
    {
        .bg = 0,
        .tilemapLeft = 6,
        .tilemapTop = 15,
        .width = 14,
        .height = 4,
        .paletteNum = 0xC,
        .baseBlock = 0x2a2
    },
    {
        .bg = 0,
        .tilemapLeft = 6,
        .tilemapTop = 15,
        .width = 15,
        .height = 4,
        .paletteNum = 0xC,
        .baseBlock = 0x2da
    },
    {
        .bg = 0,
        .tilemapLeft = 6,
        .tilemapTop = 15,
        .width = 16,
        .height = 4,
        .paletteNum = 0xC,
        .baseBlock = 0x316
    },
    {
        .bg = 0,
        .tilemapLeft = 6,
        .tilemapTop = 15,
        .width = 23,
        .height = 4,
        .paletteNum = 0xC,
        .baseBlock = 0x356
    },
    {
        .bg = 0,
        .tilemapLeft = 22,
        .tilemapTop = 17,
        .width = 7,
        .height = 2,
        .paletteNum = 0xF,
        .baseBlock = 0x20a
    },
    {
        .bg = 0,
        .tilemapLeft = 22,
        .tilemapTop = 15,
        .width = 7,
        .height = 4,
        .paletteNum = 0xF,
        .baseBlock = 0x20a
    },
    {
        .bg = 0,
        .tilemapLeft = 22,
        .tilemapTop = 13,
        .width = 7,
        .height = 6,
        .paletteNum = 0xF,
        .baseBlock = 0x20a
    },
    {
        .bg = 0,
        .tilemapLeft = 22,
        .tilemapTop = 11,
        .width = 7,
        .height = 8,
        .paletteNum = 0xF,
        .baseBlock = 0x20a
    },
    {
        .bg = 0,
        .tilemapLeft = 22,
        .tilemapTop = 9,
        .width = 7,
        .height = 10,
        .paletteNum = 0xF,
        .baseBlock = 0x20a
    }
};

void InitBagWindows(void)
{
    u32 i;
    u8 *ptr = &gBagMenu->windowPointers[i];

    InitWindows(sDefaultBagWindows);
    DeactivateAllTextPrinters();
    LoadUserWindowBorderGfx(0, 0x64, 0xE0);
    LoadMessageBoxGfx(0, 0x6D, 0xD0);
    LoadThinWindowBorderGfx(0, 0x81, 0xC0);
    LoadPalette(&sBagWindowPalF, 0xF0, 0x20);
    for (i = 0; i < 3; i++)
    {
        FillWindowPixelBuffer(i, PIXEL_FILL(0));
        PutWindowTilemap(i);
    }
    ScheduleBgCopyTilemapToVram(0);
    memset(gBagMenu->windowPointers, 0xFF, sizeof(gBagMenu->windowPointers));
}

void BagPrintTextOnWindow(u8 windowId, u8 fontId, const u8 *str, u8 left, u8 top, u8 letterSpacing, u8 lineSpacing, u8 speed, u8 colorIndex)
{
    AddTextPrinterParameterized4(windowId, fontId, left, top, letterSpacing, lineSpacing, sFontColorTable[colorIndex], speed, str);
}

/*void BagDrawDepositItemTextBox(void)
{
    u32 x = GetStringCenterAlignXOffset(4, gText_DepositItem, 0x40);
    DrawStdFrameWithCustomTileAndPalette(2, FALSE, 0x81, 0xC);
    AddTextPrinterParameterized(2, 4, gText_DepositItem, x / 2, 1, 0, NULL);
}*/

u8 ShowBagWindow(u8 which, u8 nItems)
{
    u8 *ptr = &gBagMenu->windowPointers[which];

    if (*ptr == WINDOW_NONE)
    {
        *ptr = AddWindow(&sContextMenuWindowTemplates[which + nItems]);
        if (which != 6)
            DrawStdFrameWithCustomTileAndPalette(*ptr, FALSE, 0x64, 0xE);
        else
            DrawStdFrameWithCustomTileAndPalette(*ptr, FALSE, 0x81, 0xC);
        ScheduleBgCopyTilemapToVram(0);
    }
    return *ptr;
}

void HideBagWindow(u8 which)
{
    u8 *ptr = &gBagMenu->windowPointers[which];

    ClearStdWindowAndFrameToTransparent(*ptr, FALSE);
    ClearWindowTilemap(*ptr);
    RemoveWindow(*ptr);
    ScheduleBgCopyTilemapToVram(0);
    *ptr = WINDOW_NONE;
}

u8 OpenBagWindow(u8 which)
{
    u8 *ptr = &gBagMenu->windowPointers[which];

    if (*ptr == WINDOW_NONE)
        *ptr = AddWindow(&sContextMenuWindowTemplates[which]);
    return *ptr;
}

void CloseBagWindow(u8 which)
{
    u8 *ptr = &gBagMenu->windowPointers[which];

    ClearDialogWindowAndFrameToTransparent(*ptr, FALSE);
    ClearWindowTilemap(*ptr);
    RemoveWindow(*ptr);
    PutWindowTilemap(1);
    ScheduleBgCopyTilemapToVram(0);
    *ptr = WINDOW_NONE;
}

u8 GetBagWindow(u8 which)
{
    return gBagMenu->windowPointers[which];
}

void BagCreateYesNoMenuBottomRight(u8 taskId, const struct YesNoFuncTable * ptrs)
{
    CreateYesNoMenuWithCallbacks(taskId, &sContextMenuWindowTemplates[3], 2, 0, 2, 0x64, 0xE, ptrs);
}

void BagCreateYesNoMenuTopRight(u8 taskId, const struct YesNoFuncTable * funcTable)
{
    CreateYesNoMenuWithCallbacks(taskId, &sContextMenuWindowTemplates[4], 2, 0, 2, 0x64, 0xE, funcTable);
}

void BagPrintMoneyAmount(void)
{
    PrintMoneyAmountInMoneyBoxWithBorder(ShowBagWindow(2, 0), 0x81, 0xC, GetMoney(&gSaveBlock1Ptr->money));
    AddMoneyLabelObject(19, 11);
}

static void RemoveMoneyWindow(void)
{
    HideBagWindow(2);
    RemoveMoneyLabelObject();
}

void BagDrawTextBoxOnWindow(u8 windowId)
{
    DrawTextBorderOuter(windowId, 0x64, 0xE);
}
