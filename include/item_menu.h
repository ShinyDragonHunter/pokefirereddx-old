#ifndef GUARD_item_menu_H
#define GUARD_item_menu_H

#include "item.h"
#include "menu_helpers.h"

#define ITEMMENULOCATION_FIELD 0
#define ITEMMENULOCATION_BATTLE 1
#define ITEMMENULOCATION_PARTY 2
#define ITEMMENULOCATION_SHOP 3
#define ITEMMENULOCATION_BERRY_TREE 4
#define ITEMMENULOCATION_BERRY_BLENDER_CRUSH 5
#define ITEMMENULOCATION_ITEMPC 6
#define ITEMMENULOCATION_APPRENTICE 7
#define ITEMMENULOCATION_WALLY 8
#define ITEMMENULOCATION_PCBOX 9
#define ITEMMENULOCATION_LAST 10

#define ITEMMENUACTION_USE           0
#define ITEMMENUACTION_TOSS          1
#define ITEMMENUACTION_REGISTER      2
#define ITEMMENUACTION_GIVE          3
#define ITEMMENUACTION_CANCEL        4
#define ITEMMENUACTION_BATTLE_USE    5
#define ITEMMENUACTION_CHECK         6
#define ITEMMENUACTION_WALK          7
#define ITEMMENUACTION_DESELECT      8
#define ITEMMENUACTION_CHECK_TAG     9
#define ITEMMENUACTION_CONFIRM      10
#define ITEMMENUACTION_SHOW         11
#define ITEMMENUACTION_DUMMY        12

// Exported type declarations
struct BagStruct
{
    void (*bagCallback)(void);
    u8 location;
    u8 pocket;
    bool8 bagOpen;
    u16 cursorPosition[POCKETS_COUNT];
    u16 scrollPosition[POCKETS_COUNT];
};

extern struct BagStruct gBagPositionStruct;

struct BagMenuStruct
{
    void (*exitCallback)(void);
    u8 tilemapBuffer[0x800];
    u8 windowPointers[11];
    u8 itemOriginalLocation;
    u8 pocketSwitchDisabled:4;
    u8 itemIconSlot:2;
    u8 inhibitItemDescriptionPrint:1;
    u8 hideCloseBagText:1;
    u16 contextMenuSelectedItem;
    u8 pocketScrollArrowsTask;
    u8 pocketSwitchArrowsTask;
    const u8* contextMenuItemsPtr;
    u8 contextMenuItemsBuffer[4];
    u8 contextMenuNumItems;
    u8 numItemStacks[POCKETS_COUNT];
    u8 numShownItems[6];
    s16 graphicsLoadState;
    u8 filler4[0xE];
    u8 filler2[4];
};

extern struct BagMenuStruct *gBagMenu;

// Exported RAM declarations

extern u16 gSpecialVar_ItemId;

// Exported ROM declarations
void CB2_GoToItemDepositMenu(void);
void ApprenticeOpenBagMenu(void);
void CB2_BagMenuFromBattle(void);
void SetInitialScrollAndCursorPositions(u8 pocketId);
void CB2_ReturnToBagMenuPocket(void);
void CB2_BagMenuFromStartMenu(void);
void MoveItemSlotInList(struct ItemSlot* itemSlots_, u32 from, u32 to_);
u8 GetSelectedItemIndex(u8 pocketId);
bool8 UseRegisteredKeyItemOnField(void);
void GoToBagMenu(u8 bagMenuType, u8 pocketId, void ( *postExitMenuMainCallback2)());
void DoWallyTutorialBagMenu(void);
void CB2_SetUpReshowBattleScreenAfterMenu(void);
void ResetBagCursorPositions(void);
void ChooseBerryForMachine(void (*exitCallback)(void));
void CB2_ChooseBerry(void);
void ItemMenu_StartFadeToExitCallback(u8 taskId);
void BagCreateYesNoMenuBottomRight(u8, const struct YesNoFuncTable*);
void Task_ReturnToBagFromContextMenu(u8 taskId);
void UpdatePocketItemList(u8 pocketId);
void DisplayItemMessageInBag(u8 taskId, u8 fontId, const u8 *str, void ( *callback)(u8 taskId));
void DisplayItemMessageOnField(u8 taskId, const u8 *src, TaskFunc callback);

#endif //GUARD_item_menu_H
