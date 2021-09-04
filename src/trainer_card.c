#include "global.h"
#include "scanline_effect.h"
#include "palette.h"
#include "task.h"
#include "main.h"
#include "window.h"
#include "malloc.h"
#include "link.h"
#include "bg.h"
#include "sound.h"
#include "frontier_pass.h"
#include "overworld.h"
#include "menu.h"
#include "text.h"
#include "event_data.h"
#include "easy_chat.h"
#include "money.h"
#include "strings.h"
#include "string_util.h"
#include "trainer_card.h"
#include "gpu_regs.h"
#include "international_string_util.h"
#include "pokedex.h"
#include "pokemon_icon.h"
#include "graphics.h"
#include "pokemon_icon.h"
#include "trainer_pokemon_sprites.h"
#include "constants/songs.h"
#include "constants/game_stat.h"
#include "constants/battle_frontier.h"
#include "constants/rgb.h"
#include "constants/trainers.h"

struct TrainerCardData
{
    u8 mainState;
    u8 printState;
    u8 gfxLoadState;
    u8 bgPalLoadState;
    u8 flipDrawState;
    bool8 isLink;
    u8 timeColonBlinkTimer;
    bool8 timeColonInvisible;
    bool8 onBack;
    bool8 allowDMACopy;
    bool8 hasPokedex;
    bool8 hasHofResult;
    bool8 hasLinkResults;
    bool8 hasBattleTowerWins;
    bool8 hasTrades;
    u8 badgeCount[NUM_BADGES];
    u8 easyChatProfile[TRAINER_CARD_PROFILE_LENGTH][13];
    u8 textPlayersCard[70];
    u8 textHofTime[70];
    u8 textLinkBattleType[70];
    u8 textLinkBattleWins[70];
    u8 textLinkBattleLosses[70];
    u8 textNumTrades[70];
    u8 textBerryCrushPts[70];
    u8 textUnionRoomStats[70];
    u8 textNumLinkPokeblocks[70];
    u8 textNumLinkContests[70];
    u8 textBattleTowerWins[70];
    u8 textBattleTowerStreak[70];
    u8 textBattlePoints[70];
    u8 textShinyCount[70];
    u8 textPowerPoints[70];
    u16 monIconPal[16 * PARTY_SIZE];
    s8 flipBlendY;
    bool8 timeColonNeedDraw;
    u8 cardVersion;
    u8 cardLayout;
    u16 blendColor;
    void (*callback2)(void);
    struct TrainerCard trainerCard;
    u16 frontTilemap[600];
    u16 backTilemap[600];
    u16 bgTilemap[600];
    u8 badgeTiles[0x80 * NUM_BADGES];
    u8 stickerTiles[0x200];
    u8 hStickerTiles[0x200];
    u8 cardTiles[0x2300];
    u16 cardTilemapBuffer[0x1000];
    u16 bgTilemapBuffer[0x1000];
    u16 cardTop;
    u8 language;
    u8 stats[6];
};

// EWRAM
EWRAM_DATA struct TrainerCard gTrainerCards[4] = {0};
EWRAM_DATA static struct TrainerCardData *sData = NULL;

//this file's functions
static void VblankCb_TrainerCard(void);
static void HblankCb_TrainerCard(void);
static void BlinkTimeColon(void);
static void CB2_TrainerCard(void);
static void CloseTrainerCard(u8 task);
static bool8 PrintAllOnCardFront(void);
static void DrawTrainerCardWindow(u8);
static void CreateTrainerCardTrainerPic(void);
static void DrawCardScreenBackground(u16*);
static void DrawCardFrontOrBack(u16*);
static void DrawStarsAndBadgesOnCard(void);
static void PrintTimeOnCard(void);
static void FlipTrainerCard(void);
static bool8 IsCardFlipTaskActive(void);
static bool8 LoadCardGfx(void);
static void CB2_InitTrainerCard(void);
static u32 GetCappedGameStat(u8 statId, u32 maxValue);
static bool8 HasAllFrontierSymbols(void);
static u8 CountPlayerTrainerExtraStars(void);
static u16 GetCaughtMonsCount(void);
static void SetDataFromTrainerCard(void);
static void InitGpuRegs(void);
static void ResetGpuRegs(void);
static void InitBgsAndWindows(void);
static void SetTrainerCardCb2(void);
static void SetUpTrainerCardTask(void);
static void InitTrainerCardData(void);
static u8 GetSetCardType(void);
static void PrintNameOnCardFront(void);
static void PrintIdOnCard(void);
static void PrintMoneyOnCard(void);
static void PrintPokedexOnCard(void);
static void PrintProfilePhraseOnCard(void);
static bool8 PrintAllOnCardBack(void);
static void PrintNameOnCardBack(void);
static void PrintHofDebutTimeOnCard(u8 slot);
static void PrintLinkBattleResultsOnCard(u8 slot);
static void PrintTradesStringOnCard(u8 slot);
static void PrintUnionRoomStringOnCard(u8 slot);
static void PrintPokeblockStringOnCard(u8 slot);
static void PrintUnionRoomStringOnCard(u8 slot);
static void PrintBerryCrushStringOnCard(u8 slot);
static void PrintLinkContestStringOnCard(u8 slot);
static void PrintPokemonIconsOnCard(void);
static void PrintBattlePointsStringOnCard(u8 slot);
static void PrintBattleTowerStringOnCard(u8 slot);
static void PrintShinyCountStringOnCard(u8 slot);
static void PrintPowerPointsStringOnCard(u8 slot);
static void PrintStickersOnCard(void);
static void BufferTextsVarsForCardPage2(void);
static void PrintStatBySlot(u8 slot);
static void BufferNameForCardBack(void);
static void BufferHofDebutTime(void);
static void BufferLinkBattleResults(void);
static void BufferNumTrades(void);
static void BufferUnionRoomStats(void);
static void BufferLinkPokeblocksNum(void);
static void BufferUnionRoomStats(void);
static void BufferBerryCrushPoints(void);
static void BufferLinkContestNum(void);
static void BufferBattlePoints(void);
static void BufferBattleTowerWins(void);
static void BufferShinyCount(void);
static void BufferPowerPoints(void);
static void PrintStatOnBackOfCard(u8 top, const u8* str1, u8* str2, const u8* color);
static void LoadStickerGfx(void);
static u8 SetCardBgsAndPals(void);
static void DrawCardBackStats(void);
static void Task_DoCardFlipTask(u8);
static bool8 Task_BeginCardFlip(struct Task* task);
static bool8 Task_AnimateCardFlipDown(struct Task* task);
static bool8 Task_DrawFlippedCardSide(struct Task* task);
static bool8 Task_SetCardFlipped(struct Task* task);
static bool8 Task_AnimateCardFlipUp(struct Task* task);
static bool8 Task_EndCardFlip(struct Task* task);
static void UpdateCardFlipRegs(u16);
static void LoadMonIconGfx(void);

// const rom data
static const u32 sTrainerCardStickers_Gfx[]      = INCBIN_U32("graphics/trainer_card/stickers_fr.4bpp.lz");
static const u32 sTrainerCardHStickers_Gfx[]     = INCBIN_U32("graphics/trainer_card/stickers_h.4bpp.lz");
static const u16 sHoennTrainerCard1Star_Pal[]    = INCBIN_U16("graphics/trainer_card/one_star.gbapal");
static const u16 sKantoTrainerCard1Star_Pal[]    = INCBIN_U16("graphics/trainer_card/one_star_fr.gbapal");
static const u16 sHoennTrainerCard2Star_Pal[]    = INCBIN_U16("graphics/trainer_card/two_stars.gbapal");
static const u16 sKantoTrainerCard2Star_Pal[]    = INCBIN_U16("graphics/trainer_card/two_stars_fr.gbapal");
static const u16 sHoennTrainerCard3Star_Pal[]    = INCBIN_U16("graphics/trainer_card/three_stars.gbapal");
static const u16 sKantoTrainerCard3Star_Pal[]    = INCBIN_U16("graphics/trainer_card/three_stars_fr.gbapal");
static const u16 sHoennTrainerCard4Star_Pal[]    = INCBIN_U16("graphics/trainer_card/four_stars.gbapal");
static const u16 sKantoTrainerCard4Star_Pal[]    = INCBIN_U16("graphics/trainer_card/four_stars_fr.gbapal");
static const u16 sTrainerCard5Star_Pal[]         = INCBIN_U16("graphics/trainer_card/five_stars.gbapal");
static const u16 sHoennTrainerCardFemaleBg_Pal[] = INCBIN_U16("graphics/trainer_card/female_bg.gbapal");
static const u16 sKantoTrainerCardFemaleBg_Pal[] = INCBIN_U16("graphics/trainer_card/female_bg_fr.gbapal");
static const u16 sKantoTrainerCardBadges_Pal[]   = INCBIN_U16("graphics/trainer_card/badges_fr.gbapal");
static const u16 sTrainerCardGold_Pal[]          = INCBIN_U16("graphics/trainer_card/gold.gbapal");
static const u16 sTrainerCardGoldRS_Pal[]        = INCBIN_U16("graphics/trainer_card/gold_rs.gbapal");
static const u16 sTrainerCardSticker1_Pal[]      = INCBIN_U16("graphics/trainer_card/stickers_fr1.gbapal");
static const u16 sTrainerCardSticker2_Pal[]      = INCBIN_U16("graphics/trainer_card/stickers_fr2.gbapal");
static const u16 sTrainerCardSticker3_Pal[]      = INCBIN_U16("graphics/trainer_card/stickers_fr3.gbapal");
static const u16 sTrainerCardSticker4_Pal[]      = INCBIN_U16("graphics/trainer_card/stickers_fr4.gbapal");
static const u32 sKantoTrainerCardBadges_Gfx[]   = INCBIN_U32("graphics/trainer_card/badges_fr.4bpp.lz");

static const struct BgTemplate sTrainerCardBgTemplates[4] =
{
    {
        .bg = 0,
        .charBaseIndex = 0,
        .mapBaseIndex = 27,
        .screenSize = 2,
        .paletteMode = 0,
        .priority = 2,
        .baseTile = 0
    },
    {
        .bg = 1,
        .charBaseIndex = 2,
        .mapBaseIndex = 29,
        .screenSize = 0,
        .paletteMode = 0,
        .priority = 0,
        .baseTile = 0
    },
    {
        .bg = 2,
        .charBaseIndex = 0,
        .mapBaseIndex = 30,
        .screenSize = 0,
        .paletteMode = 0,
        .priority = 3,
        .baseTile = 0
    },
    {
        .bg = 3,
        .charBaseIndex = 0,
        .mapBaseIndex = 31,
        .screenSize = 0,
        .paletteMode = 0,
        .priority = 1,
        .baseTile = 192
    },
};

static const struct WindowTemplate sTrainerCardWindowTemplates[] =
{
    {
        .bg = 1,
        .tilemapLeft = 2,
        .tilemapTop = 15,
        .width = 27,
        .height = 4,
        .paletteNum = 15,
        .baseBlock = 0x253,
    },
    {
        .bg = 1,
        .tilemapLeft = 1,
        .tilemapTop = 1,
        .width = 28,
        .height = 18,
        .paletteNum = 15,
        .baseBlock = 0x1,
    },
    {
        .bg = 3,
        .tilemapLeft = 19,
        .tilemapTop = 5,
        .width = 9,
        .height = 10,
        .paletteNum = 8,
        .baseBlock = 0x150,
    },
    DUMMY_WIN_TEMPLATE
};

static const u16 *const sHoennTrainerCardStarPals[] =
{
    gHoennTrainerCard0Star_Pal,
    sHoennTrainerCard1Star_Pal,
    sHoennTrainerCard2Star_Pal,
    sHoennTrainerCard3Star_Pal,
    sHoennTrainerCard4Star_Pal,
};

static const u16 *const sKantoTrainerCardStarPals[] =
{
    gKantoTrainerCard0Star_Pal,
    sKantoTrainerCard1Star_Pal,
    sKantoTrainerCard2Star_Pal,
    sKantoTrainerCard3Star_Pal,
    sKantoTrainerCard4Star_Pal,
    sTrainerCard5Star_Pal,
};

static const u8 sTrainerCardTextColors[] = {TEXT_COLOR_TRANSPARENT, TEXT_COLOR_DARK_GRAY, TEXT_COLOR_LIGHT_GRAY};
static const u8 sTrainerCardRSTextColors[] = {TEXT_COLOR_TRANSPARENT, TEXT_DYNAMIC_COLOR_1, TEXT_DYNAMIC_COLOR_2};
static const u8 sTrainerCardStatColors[] = {TEXT_COLOR_TRANSPARENT, TEXT_COLOR_RED, TEXT_COLOR_LIGHT_RED};
static const u8 sTrainerCardRSStatColors[] = {TEXT_COLOR_TRANSPARENT, TEXT_DYNAMIC_COLOR_5, TEXT_DYNAMIC_COLOR_6};
static const u8 sTrainerCardRSContentColors[] = {TEXT_COLOR_TRANSPARENT, TEXT_DYNAMIC_COLOR_3, TEXT_DYNAMIC_COLOR_4};
static const u8 sTimeColonInvisibleTextColors[6] = {TEXT_COLOR_TRANSPARENT, TEXT_COLOR_TRANSPARENT, TEXT_COLOR_TRANSPARENT};

static const u8 sTrainerCard5StarPhraseTextColors[] = {TEXT_COLOR_TRANSPARENT, TEXT_DYNAMIC_COLOR_1, TEXT_DYNAMIC_COLOR_4};
static const u8 sTrainerCard5StarStatColors[] = {TEXT_COLOR_TRANSPARENT, TEXT_COLOR_LIGHT_RED, TEXT_COLOR_LIGHT_GRAY};
static const u8 sTrainerCard5StarNameColors[] = {TEXT_COLOR_TRANSPARENT, TEXT_DYNAMIC_COLOR_2, TEXT_DYNAMIC_COLOR_1};

static const u8 sTrainerPicOffset[][2] =
{
    [CARD_LAYOUT_RS]       = {0, 0},
    [CARD_LAYOUT_EMERALD]  = {1, 0},
    [CARD_LAYOUT_FRLG]     = {13, 4},
    [CARD_LAYOUT_HELIODOR] = {13, 4},
};

static const u8 sTrainerPicFacilityClass[][GENDER_COUNT] = 
{
    [CARD_VERSION_FRLG] = 
    {
        [MALE]   = FACILITY_CLASS_RED_ORIGINAL,
        [FEMALE] = FACILITY_CLASS_LEAF_ORIGINAL
    },
    [CARD_VERSION_RS] = 
    {
        [MALE]   = FACILITY_CLASS_RS_BRENDAN,
        [FEMALE] = FACILITY_CLASS_RS_MAY
    },
    [CARD_VERSION_FRLG_DX] = 
    {
        [MALE]   = FACILITY_CLASS_RED,
        [FEMALE] = FACILITY_CLASS_LEAF
    },
    [CARD_VERSION_CRYSTALDUST] = 
    {
        [MALE]   = FACILITY_CLASS_GOLD,
        [FEMALE] = FACILITY_CLASS_KRIS
    },
    [CARD_VERSION_EMERALD] = 
    {
        [MALE]   = FACILITY_CLASS_E_BRENDAN,
        [FEMALE] = FACILITY_CLASS_E_MAY
    },
    [CARD_VERSION_HELIODOR] = 
    {
        [MALE]   = FACILITY_CLASS_H_BRENDAN,
        [FEMALE] = FACILITY_CLASS_H_MAY
    },
};

static const u8 sTrainerPicOutfitFacilityClass[][GENDER_COUNT] = 
{
    [OUTFIT_DX] = 
    {
        [MALE]   = FACILITY_CLASS_RED_DX,
        [FEMALE] = FACILITY_CLASS_LEAF_DX
    },
    [OUTFIT_CLASSIC] = 
    {
        [MALE]   = FACILITY_CLASS_RED_CLASSIC,
        [FEMALE] = FACILITY_CLASS_LEAF_CLASSIC
    },
    [OUTFIT_ALOLA] = 
    {
        [MALE]   = FACILITY_CLASS_RED_ALOLA,
        [FEMALE] = FACILITY_CLASS_LEAF_ALOLA
    },
    [OUTFIT_SYGNA_SUIT] = 
    {
        [MALE]   = FACILITY_CLASS_RED_SYGNA_SUIT,
        [FEMALE] = FACILITY_CLASS_LEAF_SYGNA_SUIT
    },
    [OUTFIT_TEAM_ROCKET] = 
    {
        [MALE]   = FACILITY_CLASS_RED_TEAM_ROCKET,
        [FEMALE] = FACILITY_CLASS_LEAF_TEAM_ROCKET
    },
    [OUTFIT_TEAM_AQUA] = 
    {
        [MALE]   = FACILITY_CLASS_RED_TEAM_AQUA,
        [FEMALE] = FACILITY_CLASS_LEAF_TEAM_AQUA
    },
    [OUTFIT_TEAM_MAGMA] = 
    {
        [MALE]   = FACILITY_CLASS_RED_TEAM_MAGMA,
        [FEMALE] = FACILITY_CLASS_LEAF_TEAM_MAGMA
    },
};

static bool8 (*const sTrainerCardFlipTasks[])(struct Task *) =
{
    Task_BeginCardFlip,
    Task_AnimateCardFlipDown,
    Task_DrawFlippedCardSide,
    Task_SetCardFlipped,
    Task_AnimateCardFlipUp,
    Task_EndCardFlip,
};

static void VblankCb_TrainerCard(void)
{
    LoadOam();
    ProcessSpriteCopyRequests();
    TransferPlttBuffer();
    BlinkTimeColon();
    if (sData->allowDMACopy)
        DmaCopy16(3, &gScanlineEffectRegBuffers[0], &gScanlineEffectRegBuffers[1], 0x140);
}

static void HblankCb_TrainerCard(void)
{
    u16 backup, bgVOffset;

    backup = REG_IME;
    REG_IME = 0;
    bgVOffset = gScanlineEffectRegBuffers[1][REG_VCOUNT & 0xFF];
    REG_BG0VOFS = bgVOffset;
    REG_IME = backup;
}

static void CB2_TrainerCard(void)
{
    RunTasks();
    AnimateSprites();
    BuildOamBuffer();
    UpdatePaletteFade();
}

static void CloseTrainerCard(u8 taskId)
{
    SetMainCallback2(sData->callback2);
    FreeAllWindowBuffers();
    FREE_AND_SET_NULL(sData);
    DestroyTask(taskId);
}

// States for Task_TrainerCard. Skips the initial states, which are done once in order
#define STATE_HANDLE_INPUT_FRONT  10
#define STATE_HANDLE_INPUT_BACK   11
#define STATE_WAIT_FLIP_TO_BACK   12
#define STATE_WAIT_FLIP_TO_FRONT  13
#define STATE_CLOSE_CARD          14
#define STATE_WAIT_LINK_PARTNER   15
#define STATE_CLOSE_CARD_LINK     16

static void Task_TrainerCard(u8 taskId)
{
    switch (sData->mainState)
    {
    // Draw card initially
    case 0:
        if (!IsDma3ManagerBusyWithBgCopy())
        {
            FillWindowPixelBuffer(1, PIXEL_FILL(0));
            sData->mainState++;
        }
        break;
    case 1:
        if (PrintAllOnCardFront())
            sData->mainState++;
        break;
    case 2:
        DrawTrainerCardWindow(1);
        sData->mainState++;
        break;
    case 3:
        FillWindowPixelBuffer(2, PIXEL_FILL(0));
        CreateTrainerCardTrainerPic();
        DrawTrainerCardWindow(2);
        sData->mainState++;
        break;
    case 4:
        DrawCardScreenBackground(sData->bgTilemap);
        sData->mainState++;
        break;
    case 5:
        DrawCardFrontOrBack(sData->frontTilemap);
        sData->mainState++;
        break;
    case 6:
        DrawStarsAndBadgesOnCard();
        sData->mainState++;
        break;
    // Fade in
    case 7:
        if (gWirelessCommType == 1 && gReceivedRemoteLinkPlayers)
        {
            LoadWirelessStatusIndicatorSpriteGfx();
            CreateWirelessStatusIndicatorSprite(230, 150);
        }
        BlendPalettes(PALETTES_ALL, 16, sData->blendColor);
        BeginNormalPaletteFade(PALETTES_ALL, 0, 16, 0, sData->blendColor);
        SetVBlankCallback(VblankCb_TrainerCard);
        sData->mainState++;
        break;
    case 8:
        if (!UpdatePaletteFade() && !IsDma3ManagerBusyWithBgCopy())
        {
            PlaySE(SE_RG_CARD_OPEN);
            sData->mainState = STATE_HANDLE_INPUT_FRONT;
        }
        break;
    case 9:
        if (!IsSEPlaying())
            sData->mainState++;
        break;
    case STATE_HANDLE_INPUT_FRONT:
        // Blink the : in play time
        if (!gReceivedRemoteLinkPlayers && sData->timeColonNeedDraw)
        {
            PrintTimeOnCard();
            DrawTrainerCardWindow(1);
            sData->timeColonNeedDraw = FALSE;
        }
        if (JOY_NEW(A_BUTTON))
        {
            FlipTrainerCard();
            PlaySE(SE_RG_CARD_FLIP);
            sData->mainState = STATE_WAIT_FLIP_TO_BACK;
        }
        else if (JOY_NEW(B_BUTTON))
        {
            if (gReceivedRemoteLinkPlayers && sData->isLink && InUnionRoom())
            {
                sData->mainState = STATE_WAIT_LINK_PARTNER;
            }
            else
            {
                BeginNormalPaletteFade(PALETTES_ALL, 0, 0, 16, sData->blendColor);
                sData->mainState = STATE_CLOSE_CARD;
            }
        }
        break;
    case STATE_WAIT_FLIP_TO_BACK:
        if (IsCardFlipTaskActive() && !Overworld_LinkRecvQueueLengthMoreThan2())
        {
            PlaySE(SE_RG_CARD_OPEN);
            sData->mainState = STATE_HANDLE_INPUT_BACK;
        }
        break;
    case STATE_HANDLE_INPUT_BACK:
        if (JOY_NEW(B_BUTTON))
        {
            if (gReceivedRemoteLinkPlayers && sData->isLink && InUnionRoom())
            {
                sData->mainState = STATE_WAIT_LINK_PARTNER;
            }
            else if (gReceivedRemoteLinkPlayers)
            {
                BeginNormalPaletteFade(PALETTES_ALL, 0, 0, 16, sData->blendColor);
                sData->mainState = STATE_CLOSE_CARD;
            }
            else
            {
                FlipTrainerCard();
                sData->mainState = STATE_WAIT_FLIP_TO_FRONT;
                PlaySE(SE_RG_CARD_FLIP);
            }
        }
        else if (JOY_NEW(A_BUTTON))
        {
           if (gReceivedRemoteLinkPlayers && sData->isLink && InUnionRoom())
           {
               sData->mainState = STATE_WAIT_LINK_PARTNER;
           }
           else
           {
               BeginNormalPaletteFade(PALETTES_ALL, 0, 0, 16, sData->blendColor);
               sData->mainState = STATE_CLOSE_CARD;
           }
        }
        break;
    case STATE_WAIT_LINK_PARTNER:
        SetCloseLinkCallback();
        DrawDialogueFrame(0, 1);
        AddTextPrinterParameterized(0, 2, gText_WaitingTrainerFinishReading, 0, 1, TEXT_SPEED_FF, 0);
        CopyWindowToVram(0, 3);
        sData->mainState = STATE_CLOSE_CARD_LINK;
        break;
    case STATE_CLOSE_CARD_LINK:
        if (!gReceivedRemoteLinkPlayers)
        {
            BeginNormalPaletteFade(PALETTES_ALL, 0, 0, 16, sData->blendColor);
            sData->mainState = STATE_CLOSE_CARD;
        }
        break;
    case STATE_CLOSE_CARD:
        if (!UpdatePaletteFade())
            CloseTrainerCard(taskId);
        break;
    case STATE_WAIT_FLIP_TO_FRONT:
        if (IsCardFlipTaskActive() && !Overworld_LinkRecvQueueLengthMoreThan2())
        {
            sData->mainState = STATE_HANDLE_INPUT_FRONT;
            PlaySE(SE_RG_CARD_OPEN);
        }
        break;
   }
}

static bool8 LoadCardGfx(void)
{
    switch (sData->gfxLoadState)
    {
    case 0:
        if (sData->cardLayout == CARD_LAYOUT_RS || sData->cardLayout == CARD_LAYOUT_EMERALD)
            LZ77UnCompWram(gHoennTrainerCardBg_Tilemap, sData->bgTilemap);
        else
            LZ77UnCompWram(gKantoTrainerCardBg_Tilemap, sData->bgTilemap);
        break;
    case 1:
        if (sData->cardLayout == CARD_LAYOUT_RS || sData->cardLayout == CARD_LAYOUT_EMERALD)
            LZ77UnCompWram(gHoennTrainerCardBack_Tilemap, sData->backTilemap);
        else if (sData->cardLayout == CARD_LAYOUT_FRLG)
            LZ77UnCompWram(gKantoTrainerCardBack_Tilemap, sData->backTilemap);
        else if (sData->cardLayout == CARD_LAYOUT_HELIODOR && sData->trainerCard.displayDotCode)
            LZ77UnCompWram(gHeliodorDotCodeTrainerCardBack_Tilemap, sData->backTilemap);
        else
            LZ77UnCompWram(gHeliodorTrainerCardBack_Tilemap, sData->backTilemap);
        break;
    case 2:
        if (sData->isLink)
        {
            if (sData->cardLayout == CARD_LAYOUT_RS || sData->cardLayout == CARD_LAYOUT_EMERALD)
                LZ77UnCompWram(gHoennTrainerCardFrontLink_Tilemap, sData->frontTilemap);
            else
                LZ77UnCompWram(gKantoTrainerCardFrontLink_Tilemap, sData->frontTilemap);
        }
        else
        {
            if (sData->cardLayout == CARD_LAYOUT_RS || sData->cardLayout == CARD_LAYOUT_EMERALD)
                LZ77UnCompWram(gHoennTrainerCardFront_Tilemap, sData->frontTilemap);
            else
                LZ77UnCompWram(gKantoTrainerCardFront_Tilemap, sData->frontTilemap);
        }
        break;
    case 3:
        LZ77UnCompWram(sKantoTrainerCardBadges_Gfx, sData->badgeTiles);
        break;
    case 4:
        if (sData->cardLayout == CARD_LAYOUT_RS || sData->cardLayout == CARD_LAYOUT_EMERALD)
            LZ77UnCompWram(gHoennTrainerCard_Gfx, sData->cardTiles);
        else
            LZ77UnCompWram(gKantoTrainerCard_Gfx, sData->cardTiles);
        break;
    case 5:
        if (sData->cardLayout == CARD_LAYOUT_FRLG || sData->cardLayout == CARD_LAYOUT_HELIODOR)
            LZ77UnCompWram(sTrainerCardStickers_Gfx, sData->stickerTiles);
        if (sData->cardLayout == CARD_LAYOUT_HELIODOR)
            LZ77UnCompWram(sTrainerCardHStickers_Gfx, sData->hStickerTiles);
        break;
    default:
        sData->gfxLoadState = 0;
        return TRUE;
    }
    sData->gfxLoadState++;
    return FALSE;
}

static void CB2_InitTrainerCard(void)
{
    switch (gMain.state)
    {
    case 0:
        ResetGpuRegs();
        SetUpTrainerCardTask();
        gMain.state++;
        break;
    case 1:
        DmaClear32(3, (void *)OAM, OAM_SIZE);
        gMain.state++;
        break;
    case 2:
        if (!sData->blendColor)
            DmaClear16(3, (void *)PLTT, PLTT_SIZE);
        gMain.state++;
        break;
    case 3:
        ResetSpriteData();
        FreeAllSpritePalettes();
        ResetPaletteFade();
        gMain.state++;
    case 4:
        InitBgsAndWindows();
        gMain.state++;
        break;
    case 5:
        LoadMonIconGfx();
        gMain.state++;
        break;
    case 6:
        if (LoadCardGfx())
            gMain.state++;
        break;
    case 7:
        LoadStickerGfx();
        gMain.state++;
        break;
    case 8:
        InitGpuRegs();
        gMain.state++;
        break;
    case 9:
        BufferTextsVarsForCardPage2();
        gMain.state++;
        break;
    case 10:
        if (SetCardBgsAndPals())
            gMain.state++;
        break;
    default:
        SetTrainerCardCb2();
        break;
    }
}

static u32 GetCappedGameStat(u8 statId, u32 maxValue)
{
    u32 statValue = GetGameStat(statId);

    return min(maxValue, statValue);
}

static bool8 HasAllFrontierSymbols(void)
{
    u32 i;

    for (i = 0; i < NUM_FRONTIER_FACILITIES; i++)
    {
        if (!FlagGet(FLAG_SYS_TOWER_SILVER + 2 * i) || !FlagGet(FLAG_SYS_TOWER_GOLD + 2 * i))
            return FALSE;
    }
    return TRUE;
}

u32 CountPlayerTrainerStars(void)
{
    u8 stars = 0;
    u8 extraStars = 0;

    if (GetGameStat(GAME_STAT_ENTERED_HOF))
        stars++;
    if (HasAllKantoMons())
        stars++;
    if (HasAllMons())
        stars++;
    if (gSaveBlock2Ptr->berryPick.berriesPicked > 199 && gSaveBlock2Ptr->pokeJump.jumpsInRow > 199)
        stars++;
    stars += CountPlayerTrainerExtraStars();
    return stars;
}

static u8 CountPlayerTrainerExtraStars(void)
{
    u8 stars = 0;

    if (HasAllFrontierSymbols())
        stars++;
    return stars;
}

void TrainerCard_GenerateCardForPlayer(struct TrainerCard *trainerCard)
{
    u32 playTime, i;

    memset(trainerCard, 0, sizeof(struct TrainerCard));
    trainerCard->gender = gSaveBlock2Ptr->playerGender;
    trainerCard->playTimeHours = gSaveBlock2Ptr->playTimeHours;
    trainerCard->playTimeMinutes = gSaveBlock2Ptr->playTimeMinutes;

    trainerCard->outfit = gSaveBlock2Ptr->playerOutfit;

    playTime = GetGameStat(GAME_STAT_FIRST_HOF_PLAY_TIME);
    if (!GetGameStat(GAME_STAT_ENTERED_HOF))
        playTime = 0;

    trainerCard->hofDebutHours = playTime >> 16;
    trainerCard->hofDebutMinutes = (playTime >> 8) & 0xFF;
    trainerCard->hofDebutSeconds = playTime & 0xFF;
    if ((playTime >> 16) > 999)
    {
        trainerCard->hofDebutHours = 999;
        trainerCard->hofDebutMinutes = 59;
        trainerCard->hofDebutSeconds = 59;
    }

    trainerCard->hasPokedex = FlagGet(FLAG_SYS_POKEDEX_GET);
    trainerCard->caughtMonsCount = GetCaughtMonsCount();

    trainerCard->trainerId = (gSaveBlock2Ptr->playerTrainerId[1] << 8) | gSaveBlock2Ptr->playerTrainerId[0];

    trainerCard->linkBattleWins = GetCappedGameStat(GAME_STAT_LINK_BATTLE_WINS, 9999);
    trainerCard->linkBattleLosses = GetCappedGameStat(GAME_STAT_LINK_BATTLE_LOSSES, 9999);

    trainerCard->pokemonTrades = GetCappedGameStat(GAME_STAT_POKEMON_TRADES, 0xFFFF);

    trainerCard->berryCrushPoints = GetCappedGameStat(GAME_STAT_PLAYED_BERRY_CRUSH, 0xFFFF);
    trainerCard->unionRoomNum = GetCappedGameStat(GAME_STAT_NUM_UNION_ROOM_BATTLES, 0xFFFF);

    trainerCard->money = GetMoney(&gSaveBlock1Ptr->money);

    for (i = 0; i < TRAINER_CARD_PROFILE_LENGTH; i++)
        trainerCard->easyChatProfile[i] = gSaveBlock1Ptr->easyChatProfile[i];

    StringCopy(trainerCard->playerName, gSaveBlock2Ptr->playerName);

    for (i = 0; i < TRAINER_CARD_STICKER_TYPES; i++)
    {
        trainerCard->stickers[i] = VarGet(VAR_LINK_WIN_BRAG_STATE - i);
        if (trainerCard->stickers[i])
            trainerCard->shouldDrawStickers = TRUE;
    }
    trainerCard->monSpecies[0] = VarGet(VAR_TRAINER_CARD_MON_ICON_0);
    trainerCard->monSpecies[1] = VarGet(VAR_TRAINER_CARD_MON_ICON_1);
    trainerCard->monSpecies[2] = VarGet(VAR_TRAINER_CARD_MON_ICON_2);
    trainerCard->monSpecies[3] = VarGet(VAR_TRAINER_CARD_MON_ICON_3);
    trainerCard->monSpecies[4] = VarGet(VAR_TRAINER_CARD_MON_ICON_4);
    trainerCard->monSpecies[5] = VarGet(VAR_TRAINER_CARD_MON_ICON_5);
    trainerCard->monForm0 = VarGet(VAR_TRAINER_CARD_MON_FORM_ICON_0);
    trainerCard->monForm1 = VarGet(VAR_TRAINER_CARD_MON_FORM_ICON_1);
    trainerCard->monForm2 = VarGet(VAR_TRAINER_CARD_MON_FORM_ICON_2);
    trainerCard->monForm3 = VarGet(VAR_TRAINER_CARD_MON_FORM_ICON_3);
    trainerCard->monForm4 = VarGet(VAR_TRAINER_CARD_MON_FORM_ICON_4);
    trainerCard->monForm5 = VarGet(VAR_TRAINER_CARD_MON_FORM_ICON_5);
    trainerCard->monIconTint = VarGet(VAR_TRAINER_CARD_MON_ICON_TINT);

    trainerCard->stars = CountPlayerTrainerStars();

    if (trainerCard->stars > 4)
    {
        trainerCard->extraStars = trainerCard->stars - 4;
        trainerCard->stars = 4;
    }

    trainerCard->version = GAME_VERSION;
    trainerCard->versionModifier = VERSION_MODIFIER;
    trainerCard->frontierBP = gSaveBlock2Ptr->frontier.cardBattlePoints;

    if (trainerCard->gender)
        trainerCard->facilityClass = gLinkPlayerFacilityClasses[(trainerCard->trainerId % NUM_FEMALE_LINK_FACILITY_CLASSES) + NUM_MALE_LINK_FACILITY_CLASSES];
    else
        trainerCard->facilityClass = gLinkPlayerFacilityClasses[trainerCard->trainerId % NUM_MALE_LINK_FACILITY_CLASSES];
}

void CopyTrainerCardData(struct TrainerCard *dst, u16 *src, u8 gameVersion, u8 versionModifier)
{
    memset(dst, 0, sizeof(struct TrainerCard));
    memcpy(dst, src, sizeof(struct TrainerCard));
}

static void SetDataFromTrainerCard(void)
{
    u32 i, badgeFlag;

    sData->hasPokedex = FALSE;
    sData->hasHofResult = FALSE;
    sData->hasLinkResults = FALSE;
    sData->hasBattleTowerWins = FALSE;
    sData->hasTrades = FALSE;
    memset(sData->badgeCount, 0, sizeof(sData->badgeCount));
    if (sData->trainerCard.hasPokedex)
        sData->hasPokedex++;

    if (sData->trainerCard.hofDebutHours
     || sData->trainerCard.hofDebutMinutes
     || sData->trainerCard.hofDebutSeconds)
        sData->hasHofResult++;

    if (sData->trainerCard.linkBattleWins || sData->trainerCard.linkBattleLosses)
        sData->hasLinkResults++;
    if (sData->trainerCard.pokemonTrades)
        sData->hasTrades++;
    if (sData->trainerCard.battleTowerWins || sData->trainerCard.battleTowerStraightWins)
        sData->hasBattleTowerWins++;

    for (i = 0, badgeFlag = FLAG_BADGE01_GET; badgeFlag < FLAG_BADGE01_GET + NUM_BADGES; badgeFlag++, i++)
    {
        if (FlagGet(badgeFlag))
            sData->badgeCount[i]++;
    }
}

static void InitGpuRegs(void)
{
    SetGpuReg(REG_OFFSET_DISPCNT, DISPCNT_WIN0_ON | DISPCNT_OBJ_ON | DISPCNT_OBJ_1D_MAP);
    ShowBg(0);
    ShowBg(1);
    ShowBg(2);
    ShowBg(3);
    SetGpuReg(REG_OFFSET_BLDCNT, BLDCNT_TGT1_BG0 | BLDCNT_EFFECT_DARKEN);
    SetGpuReg(REG_OFFSET_BLDY, 0);
    SetGpuReg(REG_OFFSET_WININ, WININ_WIN0_ALL);
    SetGpuReg(REG_OFFSET_WINOUT, WINOUT_WIN01_BG1 | WINOUT_WIN01_BG2 | WINOUT_WIN01_BG3 | WINOUT_WIN01_OBJ);
    SetGpuReg(REG_OFFSET_WIN0V, DISPLAY_HEIGHT);
    SetGpuReg(REG_OFFSET_WIN0H, DISPLAY_WIDTH);
    if (gReceivedRemoteLinkPlayers)
        EnableInterrupts(INTR_FLAG_VBLANK | INTR_FLAG_HBLANK | INTR_FLAG_VCOUNT | INTR_FLAG_TIMER3 | INTR_FLAG_SERIAL);
    else
        EnableInterrupts(INTR_FLAG_VBLANK | INTR_FLAG_HBLANK);
}

static void UpdateCardFlipRegs(u16 cardTop)
{
    s8 blendY = (cardTop + 40) / 10;

    if (blendY <= 4)
        blendY = 0;
    sData->flipBlendY = blendY;
    SetGpuReg(REG_OFFSET_BLDY, sData->flipBlendY);
    SetGpuReg(REG_OFFSET_WIN0V, WIN_RANGE(sData->cardTop, DISPLAY_HEIGHT - sData->cardTop));
}

static void ResetGpuRegs(void)
{
    SetVBlankCallback(NULL);
    SetHBlankCallback(NULL);
    SetGpuReg(REG_OFFSET_DISPCNT, 0);
    SetGpuReg(REG_OFFSET_BG0CNT, 0);
    SetGpuReg(REG_OFFSET_BG1CNT, 0);
    SetGpuReg(REG_OFFSET_BG2CNT, 0);
    SetGpuReg(REG_OFFSET_BG3CNT, 0);
}

static void InitBgsAndWindows(void)
{
    ResetBgsAndClearDma3BusyFlags(0);
    InitBgsFromTemplates(0, sTrainerCardBgTemplates, ARRAY_COUNT(sTrainerCardBgTemplates));
    ChangeBgX(0, 0, 0);
    ChangeBgY(0, 0, 0);
    ChangeBgX(1, 0, 0);
    ChangeBgY(1, 0, 0);
    ChangeBgX(2, 0, 0);
    ChangeBgY(2, 0, 0);
    ChangeBgX(3, 0, 0);
    ChangeBgY(3, 0, 0);
    InitWindows(sTrainerCardWindowTemplates);
    DeactivateAllTextPrinters();
    LoadMessageBoxAndBorderGfx();
}

static void SetTrainerCardCb2(void)
{
    SetMainCallback2(CB2_TrainerCard);
}

static void SetUpTrainerCardTask(void)
{
    ResetTasks();
    ScanlineEffect_Stop();
    CreateTask(Task_TrainerCard, 0);
    InitTrainerCardData();
    SetDataFromTrainerCard();
}

static bool8 PrintAllOnCardFront(void)
{
    u16 palette;
    switch (sData->printState)
    {
    case 0:
        if (sData->cardLayout == CARD_LAYOUT_RS)
        {
            palette = RGB(0, 0, 0);
            LoadPalette(&palette, 250, 2);
            palette = RGB(25, 25, 25);
            LoadPalette(&palette, 251, 2);
            palette = RGB(9, 9, 9);
            LoadPalette(&palette, 252, 2);
            palette = RGB(26, 26, 25);
            LoadPalette(&palette, 253, 2);
            palette = RGB(31, 0, 0);
            LoadPalette(&palette, 254, 2);
            palette = RGB(31, 0, 31);
            LoadPalette(&palette, 255, 2);
        }
        else if (sData->cardLayout > CARD_LAYOUT_EMERALD)
        {
            palette = RGB(15, 28, 27);
            LoadPalette(&palette, 250, 2);
            palette = RGB(13, 25, 24);
            LoadPalette(&palette, 251, 2);
            palette = RGB(30, 26, 17);
            LoadPalette(&palette, 252, 2);
            palette = RGB(15, 18, 18);
            LoadPalette(&palette, 253, 2);
            palette = RGB(30, 25, 18);
            LoadPalette(&palette, 254, 2);
            palette = RGB(31, 30, 23);
            LoadPalette(&palette, 255, 2);
        }
        PrintNameOnCardFront();
        break;
    case 1:
        PrintIdOnCard();
        break;
    case 2:
        PrintMoneyOnCard();
        break;
    case 3:
        PrintPokedexOnCard();
        break;
    case 4:
        PrintTimeOnCard();
        break;
    case 5:
        PrintProfilePhraseOnCard();
        break;
    default:
        sData->printState = 0;
        return TRUE;
    }
    sData->printState++;
    return FALSE;
}

static bool8 PrintAllOnCardBack(void)
{
    switch (sData->printState)
    {
    case 0:
        PrintNameOnCardBack();
        break;
    case 1:
        if (sData->cardVersion == CARD_VERSION_HELIODOR && sData->stats[sData->printState - 1])
            PrintStatBySlot(sData->printState - 1);
        else
            PrintHofDebutTimeOnCard(sData->printState - 1);
        break;
    case 2:
        if (sData->cardVersion == CARD_VERSION_HELIODOR && sData->stats[sData->printState - 1])
            PrintStatBySlot(sData->printState - 1);
        else
            PrintLinkBattleResultsOnCard(sData->printState - 1);
        break;
    case 3:
        if (sData->cardVersion == CARD_VERSION_HELIODOR && sData->stats[sData->printState - 1])
            PrintStatBySlot(sData->printState - 1);
        else
            PrintTradesStringOnCard(sData->printState - 1);
        break;
    case 4:
        if (sData->cardVersion == CARD_VERSION_HELIODOR && sData->stats[sData->printState - 1])
            PrintStatBySlot(sData->printState - 1);
        else if (!sData->cardVersion || sData->cardVersion == CARD_VERSION_FRLG_DX)
            PrintUnionRoomStringOnCard(sData->printState - 1);
        else if (sData->cardVersion == CARD_VERSION_RS || sData->cardVersion > CARD_VERSION_FRLG_DX)
            PrintPokeblockStringOnCard(sData->printState - 1);
        break;
    case 5:
        if (sData->cardVersion == CARD_VERSION_HELIODOR && sData->stats[sData->printState - 1])
            PrintStatBySlot(sData->printState - 1);
        else if (!sData->cardVersion || (sData->cardVersion == CARD_VERSION_FRLG_DX && !sData->trainerCard.frontierBP))
            PrintBerryCrushStringOnCard(sData->printState - 1);
        else if (sData->cardVersion == CARD_VERSION_RS || sData->cardVersion > CARD_VERSION_FRLG_DX)
            PrintLinkContestStringOnCard(sData->printState - 1);
        else
            PrintBattlePointsStringOnCard(sData->printState - 1);
        break;
    case 6:
        if (sData->cardVersion == CARD_VERSION_HELIODOR && sData->stats[sData->printState - 1])
            PrintStatBySlot(sData->printState - 1);
        else if (sData->cardVersion == CARD_VERSION_EMERALD)
            PrintBattlePointsStringOnCard(sData->printState - 1);
        else if (sData->cardVersion == CARD_VERSION_RS)
            PrintBattleTowerStringOnCard(sData->printState - 1);
        break;
    case 7:
        PrintPokemonIconsOnCard();
        break;
    case 8:
        PrintStickersOnCard();
        break;
    default:
        sData->printState = 0;
        return TRUE;
    }
    sData->printState++;
    return FALSE;
}

static void PrintStatBySlot(u8 slot)
{
    switch (sData->stats[slot])
    {
        case CARD_STAT_HOF_DEBUT:
            PrintHofDebutTimeOnCard(slot);
            break;
        case CARD_STAT_LINK_BATTLES:
            PrintLinkBattleResultsOnCard(slot);
            break;
        case CARD_STAT_TRADES:
            PrintTradesStringOnCard(slot);
            break;
        case CARD_STAT_POKEBLOCKS:
            PrintPokeblockStringOnCard(slot);
            break;
        case CARD_STAT_CONTESTS:
            PrintLinkContestStringOnCard(slot);
            break;
        case CARD_STAT_BATTLE_TOWER:
            PrintBattleTowerStringOnCard(slot);
            break;
        case CARD_STAT_UNION_ROOM:
            PrintUnionRoomStringOnCard(slot);
            break;
        case CARD_STAT_BERRY_CRUSH:
            PrintBerryCrushStringOnCard(slot);
            break;
        case CARD_STAT_BATTLE_POINTS:
            PrintBattlePointsStringOnCard(slot);
            break;
        case CARD_STAT_SHINIES:
            PrintShinyCountStringOnCard(slot);
            break;
        case CARD_STAT_POWER_POINTS:
            PrintPowerPointsStringOnCard(slot);
            break;
    }
}

static void BufferTextsVarsForCardPage2(void)
{
    BufferNameForCardBack();
    BufferHofDebutTime();
    BufferLinkBattleResults();
    BufferNumTrades();
    BufferUnionRoomStats();
    BufferLinkPokeblocksNum();
    BufferBerryCrushPoints();
    BufferLinkContestNum();
    BufferBattlePoints();
    BufferBattleTowerWins();
    BufferShinyCount();
    BufferPowerPoints();
}

static void PrintNameOnCardFront(void)
{
    u8 buffer[32];
    u8* txtPtr;
    u8 x = (sData->cardLayout == CARD_LAYOUT_RS || sData->cardLayout == CARD_LAYOUT_EMERALD) ? 16 : 20;
    u8 y;
    const u8* txtColor;

    if (sData->cardLayout == CARD_LAYOUT_RS)
        y = 32;
    else if (sData->cardLayout == CARD_LAYOUT_EMERALD)
        y = 33;
    else if (sData->cardLayout == CARD_LAYOUT_FRLG)
        y = 29;
    else
        y = 28;

    if (sData->cardLayout == CARD_LAYOUT_RS)
    {
        AddTextPrinterParameterized3(1, sData->cardLayout, x, y, sTrainerCardRSTextColors, TEXT_SPEED_FF, gText_RSTrainerCardName);
        txtPtr = StringCopy(buffer, sData->trainerCard.playerName);
        x += 32;
    }
    else
    {
        txtPtr = StringCopy(buffer, gText_TrainerCardName);
        StringCopy(txtPtr, sData->trainerCard.playerName);
    }

    ConvertInternationalString(txtPtr, sData->language);
    AddTextPrinterParameterized3(1, sData->cardLayout, x, y, (sData->cardLayout == CARD_LAYOUT_RS) ? sTrainerCardRSTextColors : sTrainerCardTextColors, TEXT_SPEED_FF, buffer);
}

static void PrintIdOnCard(void)
{
    u8 buffer[32];
    u8* txtPtr;
    u8 x, y;
    u8 var = (sData->cardLayout == CARD_LAYOUT_RS || sData->cardLayout == CARD_LAYOUT_EMERALD) ? 96 : 80;
    u8 var2;

    txtPtr = StringCopy(buffer, (sData->cardLayout == CARD_LAYOUT_RS) ? gText_RSTrainerCardIDNo : gText_TrainerCardIDNo);
    ConvertIntToDecimalStringN(txtPtr, sData->trainerCard.trainerId, STR_CONV_MODE_LEADING_ZEROS, 5);

    if (sData->cardLayout == CARD_LAYOUT_RS)
    {
        y = 8;
        var2 = 107;
    }
    else if (sData->cardLayout == CARD_LAYOUT_EMERALD)
    {
        y = 9;
        var2 = 120;
    }
    else
    {
        y = (sData->cardLayout == CARD_LAYOUT_FRLG) ? 10 : 9;
        var2 = 132;
    }

    x = GetStringCenterAlignXOffset(sData->cardLayout, buffer, var) + var2;
    AddTextPrinterParameterized3(1, sData->cardLayout, x, y, ((sData->trainerCard.stars + sData->trainerCard.extraStars) > 4) ? sTrainerCardRSContentColors : sTrainerCardTextColors, TEXT_SPEED_FF, buffer);
}

static void PrintMoneyOnCard(void)
{
    u8 var;
    u8 x = (sData->cardLayout == CARD_LAYOUT_RS || sData->cardLayout == CARD_LAYOUT_EMERALD) ? 16 : 20;
    u8 y = (sData->cardLayout == CARD_LAYOUT_EMERALD) ? 57 : 56;

    if (sData->cardLayout == CARD_LAYOUT_RS)
        var = 120;
    else if (sData->cardLayout == CARD_LAYOUT_EMERALD)
        var = 128;
    else
        var = 136;

    AddTextPrinterParameterized3(1, sData->cardLayout, x, y, (sData->cardLayout == CARD_LAYOUT_RS) ? sTrainerCardRSTextColors : sTrainerCardTextColors, TEXT_SPEED_FF, gText_TrainerCardMoney);
    ConvertIntToDecimalStringN(gStringVar1, sData->trainerCard.money, STR_CONV_MODE_LEFT_ALIGN, 6);
    StringExpandPlaceholders(gStringVar4, gText_PokedollarVar1);

    x = GetStringRightAlignXOffset(sData->cardLayout, gStringVar4, var);
    AddTextPrinterParameterized3(1, sData->cardLayout, x, y, (sData->cardLayout == CARD_LAYOUT_RS) ? sTrainerCardRSContentColors : sTrainerCardTextColors, TEXT_SPEED_FF, gStringVar4);
}

static u16 GetCaughtMonsCount(void)
{
    if (IsNationalPokedexEnabled())
        return GetNationalPokedexCount(FLAG_GET_CAUGHT);
    return GetHoennPokedexCount(FLAG_GET_CAUGHT);
}

static void PrintPokedexOnCard(void)
{
    u8 var;
    u8 x = (sData->cardLayout == CARD_LAYOUT_RS || sData->cardLayout == CARD_LAYOUT_EMERALD) ? 16 : 20;
    u8 y = (sData->cardLayout == CARD_LAYOUT_EMERALD) ? 73 : 72;

    if (sData->cardLayout == CARD_LAYOUT_RS)
        var = 120;
    else if (sData->cardLayout == CARD_LAYOUT_EMERALD)
        var = 128;
    else
        var = 136;

    if (FlagGet(FLAG_SYS_POKEDEX_GET))
    {
        AddTextPrinterParameterized3(1, sData->cardLayout, x, y, (sData->cardLayout == CARD_LAYOUT_RS) ? sTrainerCardRSTextColors : sTrainerCardTextColors, TEXT_SPEED_FF, gText_TrainerCardPokedex);
        StringCopy(ConvertIntToDecimalStringN(gStringVar4, sData->trainerCard.caughtMonsCount, STR_CONV_MODE_LEFT_ALIGN, 3), gText_ExpandedPlaceholder_Empty);

        x = GetStringRightAlignXOffset(sData->cardLayout, gStringVar4, var);
        AddTextPrinterParameterized3(1, sData->cardLayout, x, y, (sData->cardLayout == CARD_LAYOUT_RS) ? sTrainerCardRSContentColors : sTrainerCardTextColors, TEXT_SPEED_FF, gStringVar4);
    }
}

static const u8 *const sTimeColonTextColors[] = {sTrainerCardTextColors, sTimeColonInvisibleTextColors};

static void PrintTimeOnCard(void)
{
    u16 hours, minutes;
    u32 x = (sData->cardLayout == CARD_LAYOUT_RS || sData->cardLayout == CARD_LAYOUT_EMERALD) ? 16 : 20;
    u8 var = (sData->cardLayout == CARD_LAYOUT_EMERALD) ? 2 : 1;
    s32 width;
    u32 totalWidth;
    u8 var2 = (sData->cardLayout == CARD_LAYOUT_RS) ? 34 : 30;

    AddTextPrinterParameterized3(1, sData->cardLayout, x, 90 - var, (sData->cardLayout == CARD_LAYOUT_RS) ? sTrainerCardRSTextColors : sTrainerCardTextColors, TEXT_SPEED_FF, (sData->cardLayout == CARD_LAYOUT_RS) ? gText_RSTrainerCardTime : gText_TrainerCardTime);

    if (sData->isLink)
    {
        hours = sData->trainerCard.playTimeHours;
        minutes = sData->trainerCard.playTimeMinutes;
    }
    else
    {
        hours = gSaveBlock2Ptr->playTimeHours;
        minutes = gSaveBlock2Ptr->playTimeMinutes;
    }

    if (hours > 999)
        hours = 999;
    if (minutes > 59)
        minutes = 59;
    width = GetStringWidth(sData->cardLayout, (sData->cardLayout == CARD_LAYOUT_RS) ? gText_RSTrainerCardColon : gText_Colon2, 0);

    if (sData->cardLayout == CARD_LAYOUT_RS)
        x = 120;
    else if (sData->cardLayout == CARD_LAYOUT_EMERALD)
        x = 128;
    else
        x = 136;

    totalWidth = width + var2;
    x -= totalWidth;

    FillWindowPixelRect(1, PIXEL_FILL(0), x, 90 - var, totalWidth, 15);
    ConvertIntToDecimalStringN(gStringVar4, hours, STR_CONV_MODE_RIGHT_ALIGN, 3);
    AddTextPrinterParameterized3(1, sData->cardLayout, x, 90 - var, (sData->cardLayout == CARD_LAYOUT_RS) ? sTrainerCardRSContentColors : sTrainerCardTextColors, TEXT_SPEED_FF, gStringVar4);
    if (sData->cardLayout == CARD_LAYOUT_RS)
        x += 22;
    else
        x += 18;
    AddTextPrinterParameterized3(1, sData->cardLayout, x, 90 - var, sTimeColonTextColors[sData->timeColonInvisible], TEXT_SPEED_FF, (sData->cardLayout == CARD_LAYOUT_RS) ? gText_RSTrainerCardColon : gText_Colon2);
    x += width;
    ConvertIntToDecimalStringN(gStringVar4, minutes, STR_CONV_MODE_LEADING_ZEROS, 2);
    AddTextPrinterParameterized3(1, sData->cardLayout, x, 90 - var, (sData->cardLayout == CARD_LAYOUT_RS) ? sTrainerCardRSContentColors : sTrainerCardTextColors, TEXT_SPEED_FF, gStringVar4);
}

static void PrintProfilePhraseOnCard(void)
{
    u8 x = (sData->cardLayout == CARD_LAYOUT_RS || sData->cardLayout == CARD_LAYOUT_EMERALD) ? 8 : 10;
    u8 y1 = 104;
    u8 y2 = 120;
    u8 space = (sData->cardLayout == CARD_LAYOUT_RS) ? 3 : 6;
    const u8* txtColor;

    if (sData->cardLayout == CARD_LAYOUT_FRLG)
    {
        y1 = 115;
        y2 = 130;
    }
    else if (sData->cardLayout == CARD_LAYOUT_HELIODOR)
    {
        y1 = 114;
        y2 = 129;
    }

    if (sData->cardLayout == CARD_LAYOUT_RS)
        txtColor = sTrainerCardRSContentColors;
    else if ((sData->trainerCard.stars + sData->trainerCard.extraStars) > 4)
    {
        if (sData->cardVersion == CARD_VERSION_FRLG_DX)
            txtColor = sTrainerCardRSContentColors;
        else
            txtColor = sTrainerCard5StarPhraseTextColors;
    }
    else
        txtColor = sTrainerCardTextColors;

    if (sData->isLink)
    {
        AddTextPrinterParameterized3(1, sData->cardLayout, x, y1, txtColor, TEXT_SPEED_FF, sData->easyChatProfile[0]);
        AddTextPrinterParameterized3(1, sData->cardLayout, GetStringWidth(sData->cardLayout, sData->easyChatProfile[0], 0) + space + x, y1, txtColor, TEXT_SPEED_FF, sData->easyChatProfile[1]);
        AddTextPrinterParameterized3(1, sData->cardLayout, x, y2, txtColor, TEXT_SPEED_FF, sData->easyChatProfile[2]);
        AddTextPrinterParameterized3(1, sData->cardLayout, GetStringWidth(sData->cardLayout, sData->easyChatProfile[2], 0) + space + x, y2, txtColor, TEXT_SPEED_FF, sData->easyChatProfile[3]);
    }
}

static void BufferNameForCardBack(void)
{
    StringCopy(sData->textPlayersCard, sData->trainerCard.playerName);
    ConvertInternationalString(sData->textPlayersCard, sData->language);
    if (sData->cardLayout == CARD_LAYOUT_RS || sData->cardLayout == CARD_LAYOUT_EMERALD)
    {
        StringCopy(gStringVar1, sData->textPlayersCard);
        StringExpandPlaceholders(sData->textPlayersCard, gText_Var1sTrainerCard);
    }
    else if (sData->cardLayout == CARD_LAYOUT_HELIODOR)
    {
        StringCopy(gStringVar1, sData->textPlayersCard);
        StringExpandPlaceholders(sData->textPlayersCard, gText_HeliodorTrainerCardName);
    }
}

static void PrintNameOnCardBack(void)
{
    u8 x, y;
    u8 buffer[32];
    u8* txtPtr;
    const u8* txtColor;

    if (sData->cardLayout == CARD_LAYOUT_RS)
    {
        x = GetStringRightAlignXOffset(sData->cardLayout, sData->textPlayersCard, 216);
        y = 8;
        txtColor = sTrainerCardRSContentColors;
    }
    else if (sData->cardLayout == CARD_LAYOUT_EMERALD)
    {
        x = GetStringRightAlignXOffset(sData->cardLayout, sData->textPlayersCard, 216);
        y = 9;
        txtColor = sTrainerCardTextColors;
    }
    else if (sData->cardLayout == CARD_LAYOUT_FRLG)
    {
        x = 138;
        y = 11;
        if ((sData->trainerCard.stars + sData->trainerCard.extraStars) > 4)
            txtColor = sTrainerCardRSContentColors;
        else
            txtColor = sTrainerCardTextColors;
	}
    else if (sData->cardLayout == CARD_LAYOUT_HELIODOR)
    {
        txtPtr = StringCopy(buffer, sData->textPlayersCard);
        x = GetStringCenterAlignXOffset(sData->cardLayout, buffer, 240) - 7;
        y = 9;
        if ((sData->trainerCard.stars + sData->trainerCard.extraStars) > 4)
            txtColor = sTrainerCard5StarNameColors;
        else
            txtColor = sTrainerCardRSStatColors;
    }
    AddTextPrinterParameterized3(1, (sData->cardLayout == CARD_LAYOUT_HELIODOR) ? 4 : sData->cardLayout, x, y, txtColor, TEXT_SPEED_FF, sData->textPlayersCard);
}

static const u8 sText_RSHofTime[] = _("{STR_VAR_1} : {STR_VAR_2} : {STR_VAR_3}");
static const u8 sText_HofTime[] = _("{STR_VAR_1}:{STR_VAR_2}:{STR_VAR_3}");

static void BufferHofDebutTime(void)
{
    if (sData->hasHofResult)
    {
        ConvertIntToDecimalStringN(gStringVar1, sData->trainerCard.hofDebutHours, STR_CONV_MODE_RIGHT_ALIGN, 3);
        ConvertIntToDecimalStringN(gStringVar2, sData->trainerCard.hofDebutMinutes, STR_CONV_MODE_LEADING_ZEROS, 2);
        ConvertIntToDecimalStringN(gStringVar3, sData->trainerCard.hofDebutSeconds, STR_CONV_MODE_LEADING_ZEROS, 2);
        StringExpandPlaceholders(sData->textHofTime, (sData->cardLayout == CARD_LAYOUT_RS) ? sText_RSHofTime : sText_HofTime);
    }
}

static void PrintStatOnBackOfCard(u8 top, const u8* statName, u8* stat, const u8* color)
{
    u8 x = (sData->cardLayout == CARD_LAYOUT_RS || sData->cardLayout == CARD_LAYOUT_EMERALD) ? 16 : 10;
    u8 y;
	
    if (sData->cardLayout == CARD_LAYOUT_RS)
        y = 32;
    else if (sData->cardLayout == CARD_LAYOUT_EMERALD)
        y = 33;
    else
    {
        y = 35;
        if ((sData->trainerCard.stars + sData->trainerCard.extraStars) > 4)
            color = sTrainerCard5StarStatColors;
    }
    AddTextPrinterParameterized3(1, sData->cardLayout, x, top * 16 + y, (sData->cardLayout == CARD_LAYOUT_RS) ? sTrainerCardRSContentColors : sTrainerCardTextColors, TEXT_SPEED_FF, statName);
    AddTextPrinterParameterized3(1, sData->cardLayout, GetStringRightAlignXOffset(sData->cardLayout, stat, 216), top * 16 + y, color, TEXT_SPEED_FF, stat);
}

static void PrintHofDebutTimeOnCard(u8 slot)
{
    if (sData->hasHofResult)
        PrintStatOnBackOfCard(slot, (sData->cardLayout == CARD_LAYOUT_RS) ? gText_FirstHallOfFame : gText_HallOfFameDebut, sData->textHofTime, (sData->cardLayout == CARD_LAYOUT_RS) ? sTrainerCardRSStatColors : sTrainerCardStatColors);
}

static void BufferLinkBattleResults(void)
{
    if (sData->hasLinkResults)
    {
        StringCopy(sData->textLinkBattleType, (sData->cardLayout == CARD_LAYOUT_RS) ? gText_LinkCableBattles : gText_LinkBattles);
        ConvertIntToDecimalStringN(sData->textLinkBattleWins, sData->trainerCard.linkBattleWins, STR_CONV_MODE_LEFT_ALIGN, 4);
        ConvertIntToDecimalStringN(sData->textLinkBattleLosses, sData->trainerCard.linkBattleLosses, STR_CONV_MODE_LEFT_ALIGN, 4);
    }
}

static void PrintLinkBattleResultsOnCard(u8 slot)
{
    if (sData->hasLinkResults)
    {
        if (sData->cardLayout == CARD_LAYOUT_RS)
        {
            AddTextPrinterParameterized3(1, sData->cardLayout, 16, slot * 16 + 32, sTrainerCardRSContentColors, TEXT_SPEED_FF, sData->textLinkBattleType);
            AddTextPrinterParameterized3(1, sData->cardLayout, GetStringRightAlignXOffset(sData->cardLayout, sData->textLinkBattleWins, 168), slot * 16 + 32, sTrainerCardRSStatColors, TEXT_SPEED_FF, sData->textLinkBattleWins);
            AddTextPrinterParameterized3(1, sData->cardLayout, GetStringRightAlignXOffset(sData->cardLayout, sData->textLinkBattleLosses, 216), slot * 16 + 32, sTrainerCardRSStatColors, TEXT_SPEED_FF, sData->textLinkBattleLosses);
        }
        else if (sData->cardLayout == CARD_LAYOUT_FRLG)
        {
            AddTextPrinterParameterized3(1, sData->cardLayout, 10, slot * 16 + 35, sTrainerCardTextColors, TEXT_SPEED_FF, sData->textLinkBattleType);
            AddTextPrinterParameterized3(1, sData->cardLayout, 130, slot * 16 + 35, sTrainerCardTextColors, TEXT_SPEED_FF, gText_FRLGWinsLosses);
            AddTextPrinterParameterized3(1, sData->cardLayout, 144, slot * 16 + 35, ((sData->trainerCard.stars + sData->trainerCard.extraStars) > 4) ? sTrainerCard5StarStatColors : sTrainerCardStatColors, TEXT_SPEED_FF, sData->textLinkBattleWins);
            AddTextPrinterParameterized3(1, sData->cardLayout, 192, slot * 16 + 35, ((sData->trainerCard.stars + sData->trainerCard.extraStars) > 4) ? sTrainerCard5StarStatColors : sTrainerCardStatColors, TEXT_SPEED_FF, sData->textLinkBattleLosses);
        }
        else if (sData->cardLayout == CARD_LAYOUT_HELIODOR)
        {
            AddTextPrinterParameterized3(1, sData->cardLayout, 10, slot * 16 + 35, sTrainerCardTextColors, TEXT_SPEED_FF, sData->textLinkBattleType);
            AddTextPrinterParameterized3(1, sData->cardLayout, 127, slot * 16 + 35, sTrainerCardTextColors, TEXT_SPEED_FF, gText_FRLGWinsLosses);
            AddTextPrinterParameterized3(1, sData->cardLayout, GetStringRightAlignXOffset(sData->cardLayout, sData->textLinkBattleWins, 168), slot * 16 + 35, ((sData->trainerCard.stars + sData->trainerCard.extraStars) > 4) ? sTrainerCard5StarStatColors : sTrainerCardStatColors, TEXT_SPEED_FF, sData->textLinkBattleWins);
            AddTextPrinterParameterized3(1, sData->cardLayout, GetStringRightAlignXOffset(sData->cardLayout, sData->textLinkBattleLosses, 216), slot * 16 + 35, ((sData->trainerCard.stars + sData->trainerCard.extraStars) > 4) ? sTrainerCard5StarStatColors : sTrainerCardStatColors, TEXT_SPEED_FF, sData->textLinkBattleLosses);
        }
        else
        {
            StringCopy(gStringVar1, sData->textLinkBattleWins);
            StringCopy(gStringVar2, sData->textLinkBattleLosses);
            StringExpandPlaceholders(gStringVar4, gText_WinsLosses);
            PrintStatOnBackOfCard(slot, sData->textLinkBattleType, gStringVar4, sTrainerCardTextColors);
        }
    }
}

static void BufferNumTrades(void)
{
    if (sData->hasTrades)
        ConvertIntToDecimalStringN(sData->textNumTrades, sData->trainerCard.pokemonTrades, STR_CONV_MODE_RIGHT_ALIGN, 5);
}

static void PrintTradesStringOnCard(u8 slot)
{
    if (sData->hasTrades)
        PrintStatOnBackOfCard(slot, gText_PokemonTrades, sData->textNumTrades, (sData->cardLayout == CARD_LAYOUT_RS) ? sTrainerCardRSStatColors : sTrainerCardStatColors);
}

static void BufferBerryCrushPoints(void)
{
    if (!sData->cardVersion || (sData->cardVersion == CARD_VERSION_FRLG_DX && !sData->trainerCard.frontierBP) || sData->cardVersion == CARD_VERSION_HELIODOR)
    {
        if (sData->trainerCard.berryCrushPoints)
            ConvertIntToDecimalStringN(sData->textBerryCrushPts, sData->trainerCard.berryCrushPoints, STR_CONV_MODE_RIGHT_ALIGN, 5);
    }
}

static void PrintBerryCrushStringOnCard(u8 slot)
{
    if (sData->trainerCard.berryCrushPoints)
        PrintStatOnBackOfCard(slot, gText_BerryCrush, sData->textBerryCrushPts, sTrainerCardStatColors);
}

static void BufferUnionRoomStats(void)
{
    if (!sData->cardVersion || sData->cardVersion == CARD_VERSION_FRLG_DX || sData->cardVersion == CARD_VERSION_HELIODOR)
    {
        if (sData->trainerCard.unionRoomNum)
            ConvertIntToDecimalStringN(sData->textUnionRoomStats, sData->trainerCard.unionRoomNum, STR_CONV_MODE_RIGHT_ALIGN, 5);
    }
}

static void PrintUnionRoomStringOnCard(u8 slot)
{
    if (sData->trainerCard.unionRoomNum)
        PrintStatOnBackOfCard(slot, gText_UnionTradesAndBattles, sData->textUnionRoomStats, sTrainerCardStatColors);
}

static void BufferLinkPokeblocksNum(void)
{
    if (sData->cardVersion == CARD_VERSION_RS || sData->cardVersion > CARD_VERSION_FRLG_DX)
    {
        if (sData->trainerCard.pokeblocksWithFriends)
        {
            ConvertIntToDecimalStringN(gStringVar1, sData->trainerCard.pokeblocksWithFriends, STR_CONV_MODE_RIGHT_ALIGN, 5);
            StringExpandPlaceholders(sData->textNumLinkPokeblocks, gText_NumPokeblocks);
        }
    }
}

static void PrintPokeblockStringOnCard(u8 slot)
{
    if (sData->trainerCard.pokeblocksWithFriends)
        PrintStatOnBackOfCard(slot, gText_PokeblocksWithFriends, sData->textNumLinkPokeblocks, (sData->cardLayout == CARD_LAYOUT_RS) ? sTrainerCardRSStatColors : sTrainerCardStatColors);
}

static void BufferLinkContestNum(void)
{
    if (sData->cardVersion == CARD_VERSION_RS || sData->cardVersion > CARD_VERSION_FRLG_DX)
    {
        if (sData->trainerCard.contestsWithFriends)
            ConvertIntToDecimalStringN(sData->textNumLinkContests, sData->trainerCard.contestsWithFriends, STR_CONV_MODE_RIGHT_ALIGN, 5);
    }
}

static void PrintLinkContestStringOnCard(u8 slot)
{
    if (sData->trainerCard.contestsWithFriends)
    {
        if (sData->cardLayout == CARD_LAYOUT_RS)
            PrintStatOnBackOfCard(slot, gText_ContestsWFriends, sData->textNumLinkContests, sTrainerCardRSStatColors);
        else
            PrintStatOnBackOfCard(slot, gText_WonContestsWFriends, sData->textNumLinkContests, ((sData->trainerCard.stars + sData->trainerCard.extraStars) > 4) ? sTrainerCard5StarStatColors : sTrainerCardStatColors);
    }
}

static void BufferBattlePoints(void)
{
    if (sData->cardVersion == CARD_VERSION_FRLG_DX || sData->cardVersion > CARD_VERSION_CRYSTALDUST)
    {
        if (sData->trainerCard.frontierBP)
        {
            ConvertIntToDecimalStringN(gStringVar1, sData->trainerCard.frontierBP, STR_CONV_MODE_RIGHT_ALIGN, 5);
	        StringExpandPlaceholders(sData->textBattlePoints, gText_NumBP);
        }
    }
}

static void PrintBattlePointsStringOnCard(u8 slot)
{
    if (sData->trainerCard.frontierBP)
        PrintStatOnBackOfCard(slot, gText_BattlePtsWon, sData->textBattlePoints, sTrainerCardStatColors);
}

static void BufferBattleTowerWins(void)
{
    if (sData->cardVersion == CARD_VERSION_RS || sData->cardVersion == CARD_VERSION_HELIODOR)
    {
        if (sData->hasBattleTowerWins)
        {
            if (sData->cardLayout == CARD_LAYOUT_RS)
            {
                ConvertIntToDecimalStringN(gStringVar1, sData->trainerCard.battleTowerWins, STR_CONV_MODE_RIGHT_ALIGN, 4);
                ConvertIntToDecimalStringN(gStringVar2, sData->trainerCard.battleTowerStraightWins, STR_CONV_MODE_RIGHT_ALIGN, 4);
                StringExpandPlaceholders(sData->textBattleTowerWins, gText_RSCardTowerStats);
            }
            else
            {
                ConvertIntToDecimalStringN(sData->textBattleTowerWins, sData->trainerCard.battleTowerWins, STR_CONV_MODE_RIGHT_ALIGN, 4);
                ConvertIntToDecimalStringN(sData->textBattleTowerStreak, sData->trainerCard.battleTowerStraightWins, STR_CONV_MODE_RIGHT_ALIGN, 4);
            }
        }
    }
}

static void PrintBattleTowerStringOnCard(u8 slot)
{
    if (sData->hasBattleTowerWins)
    {
        if (sData->cardLayout == CARD_LAYOUT_RS)
        {
            AddTextPrinterParameterized3(1, sData->cardLayout, 16, slot * 16 + 32, sTrainerCardRSContentColors, TEXT_SPEED_FF, gText_RSBattleTower);
            AddTextPrinterParameterized3(1, sData->cardLayout, 104, slot * 16 + 32, sTrainerCardRSStatColors, TEXT_SPEED_FF, sData->textBattleTowerWins);
        }
        else
        {
            AddTextPrinterParameterized3(1, sData->cardLayout, 10, slot * 16 + 35, sTrainerCardTextColors, TEXT_SPEED_FF, gText_BattleTower);
            AddTextPrinterParameterized3(1, sData->cardLayout, 127, slot * 16 + 35, sTrainerCardTextColors, TEXT_SPEED_FF, gText_5StarWinsStraight);
            AddTextPrinterParameterized3(1, sData->cardLayout, GetStringRightAlignXOffset(sData->cardLayout, sData->textBattleTowerWins, 168), slot * 16 + 35, ((sData->trainerCard.stars + sData->trainerCard.extraStars) > 4) ? sTrainerCard5StarStatColors : sTrainerCardStatColors, TEXT_SPEED_FF, sData->textBattleTowerWins);
            AddTextPrinterParameterized3(1, sData->cardLayout, GetStringRightAlignXOffset(sData->cardLayout, sData->textBattleTowerStreak, 216), slot * 16 + 35, ((sData->trainerCard.stars + sData->trainerCard.extraStars) > 4) ? sTrainerCard5StarStatColors : sTrainerCardStatColors, TEXT_SPEED_FF, sData->textBattleTowerStreak);
        }
    }
}

static void BufferShinyCount(void)
{
    if (sData->cardVersion == CARD_VERSION_HELIODOR && sData->trainerCard.shinies)
        ConvertIntToDecimalStringN(sData->textShinyCount, sData->trainerCard.shinies, STR_CONV_MODE_RIGHT_ALIGN, 5);
}

static void PrintShinyCountStringOnCard(u8 slot)
{
    if (sData->trainerCard.shinies)
        PrintStatOnBackOfCard(slot, gText_ShinyCount, sData->textShinyCount, sTrainerCardStatColors);
}

static void BufferPowerPoints(void)
{
    if (sData->cardVersion == CARD_VERSION_HELIODOR && sData->trainerCard.powerPoints)
    {
        ConvertUIntToDecimalStringN(gStringVar1, sData->trainerCard.powerPoints, STR_CONV_MODE_RIGHT_ALIGN, 5);
        StringExpandPlaceholders(sData->textPowerPoints, gText_NumPowerPoints);
    }
}

static void PrintPowerPointsStringOnCard(u8 slot)
{
    if (sData->trainerCard.powerPoints)
        PrintStatOnBackOfCard(slot, gText_PowerPoints, sData->textPowerPoints, sTrainerCardStatColors);
}

static void PrintPokemonIconsOnCard(void)
{
    u32 i;
    u8 paletteSlots[PARTY_SIZE] = {5, 6, 7, 8, 9, 10};
    u8 xOffsets[PARTY_SIZE] = {0, 4, 8, 12, 16, 20};

    if (sData->cardLayout == CARD_LAYOUT_FRLG || sData->cardLayout == CARD_LAYOUT_HELIODOR)
    {
        for (i = 0; i < PARTY_SIZE; i++)
        {
            if (sData->trainerCard.monSpecies[i])
            {
                u8 monSpecies = GetMonIconPaletteIndexFromSpecies(sData->trainerCard.monSpecies[i]);
                WriteSequenceToBgTilemapBuffer(3, 16 * i + 224, xOffsets[i] + 3, 15, 4, 4, paletteSlots[monSpecies], 1);
            }
        }
    }
}

static void LoadMonIconGfx(void)
{
    u32 i;

    CpuSet(gMonIconPalettes, sData->monIconPal, 0x60);
    switch (sData->trainerCard.monIconTint)
    {
    case MON_ICON_TINT_BLACK:
        TintPalette_CustomTone(sData->monIconPal, 96, 0, 0, 0);
    case MON_ICON_TINT_NORMAL:
        break;
    case MON_ICON_TINT_PINK:
        TintPalette_CustomTone(sData->monIconPal, 96, 500, 330, 310);
        break;
    case MON_ICON_TINT_SEPIA:
        TintPalette_SepiaTone(sData->monIconPal, 96);
        break;
    }
    LoadPalette(sData->monIconPal, 80, 192);

    for (i = 0; i < PARTY_SIZE; i++)
    {
        if (sData->trainerCard.monSpecies[i])
            LoadBgTiles(3, GetMonIconTiles(sData->trainerCard.monSpecies[i]), 512, 16 * i + 32);
    }
}

static void PrintStickersOnCard(void)
{
    u32 i;
    u8 sticker;
    u8 paletteSlots[4] = {11, 12, 13, 14};

    if ((sData->cardLayout == CARD_LAYOUT_FRLG || sData->cardLayout == CARD_LAYOUT_HELIODOR) && sData->trainerCard.shouldDrawStickers)
    {
        for (i = 0; i < TRAINER_CARD_STICKER_TYPES; i++)
        {
            sticker = sData->trainerCard.stickers[i];
            if (sData->trainerCard.stickers[i])
                WriteSequenceToBgTilemapBuffer(3, i * 4 + 320, i * 3 + 2, 2, 2, 2, paletteSlots[sticker - 1], 1);
        }
    }
    if (sData->cardLayout == CARD_LAYOUT_HELIODOR)
    {
        sticker = sData->trainerCard.hSticker0;
        if (sData->trainerCard.hSticker0)
            WriteSequenceToBgTilemapBuffer(3, 336, 20, 2, 2, 2, paletteSlots[sticker - 1], 1);
        sticker = sData->trainerCard.hSticker1;
        if (sData->trainerCard.hSticker1)
            WriteSequenceToBgTilemapBuffer(3, 340, 23, 2, 2, 2, paletteSlots[sticker - 1], 1);
        sticker = sData->trainerCard.hSticker2;
        if (sData->trainerCard.hSticker2)
            WriteSequenceToBgTilemapBuffer(3, 344, 26, 2, 2, 2, paletteSlots[sticker - 1], 1);
    }
}

static void LoadStickerGfx(void)
{
    LoadPalette(sTrainerCardSticker1_Pal, 176, 32);
    LoadPalette(sTrainerCardSticker2_Pal, 192, 32);
    LoadPalette(sTrainerCardSticker3_Pal, 208, 32);
    LoadPalette(sTrainerCardSticker4_Pal, 224, 32);
    LoadBgTiles(3, sData->stickerTiles, 1024, 128);
    LoadBgTiles(3, sData->hStickerTiles, 1024, 144);
}

static void DrawTrainerCardWindow(u8 windowId)
{
    PutWindowTilemap(windowId);
    CopyWindowToVram(windowId, 3);
}

static u8 SetCardBgsAndPals(void)
{
    u8 stars;

    switch (sData->bgPalLoadState)
    {
    case 0:
        LoadBgTiles(3, sData->badgeTiles, ARRAY_COUNT(sData->badgeTiles), 0);
        break;
    case 1:
        LoadBgTiles(0, sData->cardTiles, 0x1800, 0);
        break;
    case 2:
        if (!sData->isLink)
            LoadPalette(sKantoTrainerCardBadges_Pal, 48, 32);
        if (sData->cardLayout == CARD_LAYOUT_RS || sData->cardLayout == CARD_LAYOUT_EMERALD)
        {
            LoadPalette(sHoennTrainerCardStarPals[sData->trainerCard.stars], 0, 96);
            if (sData->trainerCard.gender)
                LoadPalette(sHoennTrainerCardFemaleBg_Pal, 16, 32);
        }
        else
        {
            if (sData->trainerCard.versionModifier < MODIFIER_CRYSTALDUST)
            {
                stars = sData->trainerCard.stars + sData->trainerCard.extraStars;
                if (stars > 5)
                    stars = 5;
            }
            else
            {
                stars = sData->trainerCard.stars;
                if (stars > 4)
                    stars = 4;
            }
            LoadPalette(sKantoTrainerCardStarPals[stars], 0, 96);
            if (sData->trainerCard.gender)
                LoadPalette(sKantoTrainerCardFemaleBg_Pal, 16, 32);
        }
		if (sData->cardLayout == CARD_LAYOUT_RS)
			LoadPalette(sTrainerCardGoldRS_Pal, 64, 32);
		else
			LoadPalette(sTrainerCardGold_Pal, 64, 32);
        break;
    case 3:
        SetBgTilemapBuffer(0, sData->cardTilemapBuffer);
        SetBgTilemapBuffer(2, sData->bgTilemapBuffer);
        break;
    case 4:
        FillBgTilemapBufferRect_Palette0(0, 0, 0, 0, 32, 32);
        FillBgTilemapBufferRect_Palette0(2, 0, 0, 0, 32, 32);
        FillBgTilemapBufferRect_Palette0(3, 0, 0, 0, 32, 32);
    default:
        return 1;
    }
    sData->bgPalLoadState++;
    return 0;
}

static void DrawCardScreenBackground(u16 *ptr)
{
    s32 i, j;
    u16 *dst = sData->bgTilemapBuffer;

    for (i = 0; i < 20; i++)
    {
        for (j = 0; j < 32; j++)
        {
            if (j < 30)
                dst[32 * i + j] = ptr[30 * i + j];
            else
                dst[32 * i + j] = ptr[0];
        }
    }
    CopyBgTilemapBufferToVram(2);
}

static void DrawCardFrontOrBack(u16* ptr)
{
    s32 i, j;
    u16 *dst = sData->cardTilemapBuffer;

    for (i = 0; i < 20; i++)
    {
        for (j = 0; j < 32; j++)
        {
            if (j < 30)
                dst[32 * i + j] = ptr[30 * i + j];
            else
                dst[32 * i + j] = ptr[0];
        }
    }
    CopyBgTilemapBufferToVram(0);
}

static void DrawStarsAndBadgesOnCard(void)
{
    s32 i;
    s16 x;
    u16 tileNum = 192;
    u8 palNum = 3;
    u8 var = (sData->cardVersion == CARD_VERSION_FRLG_DX || sData->cardLayout == CARD_LAYOUT_HELIODOR) ? 18 - (sData->trainerCard.stars + sData->trainerCard.extraStars) : 15;
    s16 y = (sData->cardLayout == CARD_LAYOUT_RS) ? 6 : 7;
    u8 stars = (sData->cardVersion == CARD_VERSION_FRLG_DX || sData->cardLayout == CARD_LAYOUT_HELIODOR) ? sData->trainerCard.stars + sData->trainerCard.extraStars : sData->trainerCard.stars;

    FillBgTilemapBufferRect(3, 143, var, y, stars, 1, 4);
    if (!sData->isLink)
    {
        x = 4;
        for (i = 0; i < NUM_BADGES; i++, tileNum += 2, x += 3)
        {
            if (sData->badgeCount[i])
            {
                FillBgTilemapBufferRect(3, tileNum, x, 16, 1, 1, palNum);
                FillBgTilemapBufferRect(3, tileNum + 1, x + 1, 16, 1, 1, palNum);
                FillBgTilemapBufferRect(3, tileNum + 16, x, 17, 1, 1, palNum);
                FillBgTilemapBufferRect(3, tileNum + 17, x + 1, 17, 1, 1, palNum);
            }
        }
    }
    CopyBgTilemapBufferToVram(3);
}

static void DrawCardBackStats(void)
{
    if (sData->cardLayout == CARD_LAYOUT_RS || sData->cardLayout == CARD_LAYOUT_EMERALD)
    {
        if (sData->hasTrades)
        {
            FillBgTilemapBufferRect(3, 141, 27, 9, 1, 1, 0);
            FillBgTilemapBufferRect(3, 157, 27, 10, 1, 1, 0);
        }
        if (sData->trainerCard.contestsWithFriends)
        {
            FillBgTilemapBufferRect(3, 141, 27, 13, 1, 1, 0);
            FillBgTilemapBufferRect(3, 157, 27, 14, 1, 1, 0);
        }
        if (sData->hasBattleTowerWins)
        {
            FillBgTilemapBufferRect(3, 141, 17, 15, 1, 1, 0);
            FillBgTilemapBufferRect(3, 157, 17, 16, 1, 1, 0);
            FillBgTilemapBufferRect(3, 140, 27, 15, 1, 1, 0);
            FillBgTilemapBufferRect(3, 156, 27, 16, 1, 1, 0);
        }
    }
    else if (sData->cardLayout == CARD_LAYOUT_FRLG)
    {
        if (sData->hasTrades)
        {
            FillBgTilemapBufferRect(3, 141, 27, 9, 1, 1, 1);
            FillBgTilemapBufferRect(3, 157, 27, 10, 1, 1, 1);
        }
        if (sData->trainerCard.berryCrushPoints)
        {
            FillBgTilemapBufferRect(3, 141, 21, 13, 1, 1, 1);
            FillBgTilemapBufferRect(3, 157, 21, 14, 1, 1, 1);
        }
        if (sData->trainerCard.unionRoomNum)
        {
            FillBgTilemapBufferRect(3, 141, 27, 11, 1, 1, 1);
            FillBgTilemapBufferRect(3, 157, 27, 12, 1, 1, 1);
        }
    }
    CopyBgTilemapBufferToVram(3);
}

static void BlinkTimeColon(void)
{
    if (++sData->timeColonBlinkTimer > 60)
    {
        sData->timeColonBlinkTimer = 0;
        sData->timeColonInvisible ^= 1;
        sData->timeColonNeedDraw = TRUE;
    }
}

u8 GetTrainerCardStars(u8 cardId)
{
    u8 stars;
    struct TrainerCard* trainerCards = gTrainerCards;

    if (trainerCards[cardId].cardLayout == CARD_LAYOUT_HELIODOR || trainerCards[cardId].versionModifier == MODIFIER_DX)
    {
        stars = trainerCards[cardId].stars + trainerCards[cardId].extraStars;
        if (stars > 5)
	        stars = 5;
    }
    else
        stars = trainerCards[cardId].stars;
    return stars;
}

#define tFlipState data[0]
#define tCardTop   data[1]

static void FlipTrainerCard(void)
{
    u8 taskId = CreateTask(Task_DoCardFlipTask, 0);
    Task_DoCardFlipTask(taskId);
    SetHBlankCallback(HblankCb_TrainerCard);
}

static bool8 IsCardFlipTaskActive(void)
{
    return (FindTaskIdByFunc(Task_DoCardFlipTask) == TASK_NONE) ? TRUE : FALSE;
}

static void Task_DoCardFlipTask(u8 taskId)
{
    while(sTrainerCardFlipTasks[gTasks[taskId].tFlipState](&gTasks[taskId]))
        ;
}

static bool8 Task_BeginCardFlip(struct Task* task)
{
    u32 i;

    HideBg(1);
    HideBg(3);
    ScanlineEffect_Stop();
    ScanlineEffect_Clear();
    for (i = 0; i < DISPLAY_HEIGHT; i++)
        gScanlineEffectRegBuffers[1][i] = 0;
    task->tFlipState++;
    return FALSE;
}

// Note: Cannot be DISPLAY_HEIGHT / 2, or cardHeight will be 0
#define CARD_FLIP_Y ((DISPLAY_HEIGHT / 2) - 3)

static bool8 Task_AnimateCardFlipDown(struct Task* task)
{
    u32 cardHeight, r5, r10, cardTop, r6, var_24, cardBottom, var;
    s32 i;

    sData->allowDMACopy = FALSE;
    if (task->tCardTop >= CARD_FLIP_Y)
        task->tCardTop = CARD_FLIP_Y;
    else
        task->tCardTop += 7;

    sData->cardTop = task->tCardTop;
    UpdateCardFlipRegs(task->tCardTop);

    cardTop = task->tCardTop;
    cardBottom = DISPLAY_HEIGHT - cardTop;
    cardHeight = cardBottom - cardTop;
    r6 = -cardTop << 16;
    r5 = (DISPLAY_HEIGHT << 16) / cardHeight;
    r5 -= 1 << 16;
    var_24 = r6;
    var_24 += r5 * cardHeight;
    r10 = r5 / cardHeight;
    r5 *= 2;

    for (i = 0; i < cardTop; i++)
        gScanlineEffectRegBuffers[0][i] = -i;
    for (; i < (s16)cardBottom; i++)
    {
        var = r6 >> 16;
        r6 += r5;
        r5 -= r10;
        gScanlineEffectRegBuffers[0][i] = var;
    }
    var = var_24 >> 16;
    for (; i < DISPLAY_HEIGHT; i++)
        gScanlineEffectRegBuffers[0][i] = var;

    sData->allowDMACopy = TRUE;
    if (task->tCardTop >= CARD_FLIP_Y)
        task->tFlipState++;
    return FALSE;
}

static bool8 Task_DrawFlippedCardSide(struct Task* task)
{
    sData->allowDMACopy = FALSE;
    if (Overworld_LinkRecvQueueLengthMoreThan2())
        return FALSE;
    do
    {
        switch (sData->flipDrawState)
        {
        case 0:
            FillWindowPixelBuffer(1, PIXEL_FILL(0));
            FillBgTilemapBufferRect_Palette0(3, 0, 0, 0, 0x20, 0x20);
            break;
        case 1:
            if (sData->onBack)
            {
                if (!PrintAllOnCardFront())
                    return FALSE;
            }
            else
            {
                if (!PrintAllOnCardBack())
                    return FALSE;
            }
            break;
        case 2:
            if (sData->onBack)
                DrawTrainerCardWindow(1);
            else
                DrawCardFrontOrBack(sData->backTilemap);
            break;
        case 3:
            if (sData->onBack)
                FillWindowPixelBuffer(2, PIXEL_FILL(0));
            else
                DrawCardBackStats();
            break;
        case 4:
            if (sData->onBack)
                CreateTrainerCardTrainerPic();
            break;
        default:
            task->tFlipState++;
            sData->allowDMACopy = TRUE;
            sData->flipDrawState = 0;
            return FALSE;
        }
        sData->flipDrawState++;
    } while (!gReceivedRemoteLinkPlayers);
    return FALSE;
}

static bool8 Task_SetCardFlipped(struct Task* task)
{
    sData->allowDMACopy = FALSE;

    // If on back of card, draw front of card because its being flipped
    if (sData->onBack)
    {
        DrawTrainerCardWindow(2);
        DrawCardScreenBackground(sData->bgTilemap);
        DrawCardFrontOrBack(sData->frontTilemap);
        DrawStarsAndBadgesOnCard();
    }
    DrawTrainerCardWindow(1);
    sData->onBack ^= 1;
    task->tFlipState++;
    sData->allowDMACopy = TRUE;
    PlaySE(SE_RG_CARD_FLIPPING);
    return FALSE;
}

static bool8 Task_AnimateCardFlipUp(struct Task* task)
{
    u32 cardHeight, r5, r10, cardTop, r6, var_24, cardBottom, var;
    s32 i;

    sData->allowDMACopy = FALSE;
    if (task->tCardTop <= 5)
        task->tCardTop = 0;
    else
        task->tCardTop -= 5;

    sData->cardTop = task->tCardTop;
    UpdateCardFlipRegs(task->tCardTop);

    cardTop = task->tCardTop;
    cardBottom = DISPLAY_HEIGHT - cardTop;
    cardHeight = cardBottom - cardTop;
    r6 = -cardTop << 16;
    r5 = (DISPLAY_HEIGHT << 16) / cardHeight;
    r5 -= 1 << 16;
    var_24 = r6;
    var_24 += r5 * cardHeight;
    r10 = r5 / cardHeight;
    r5 /= 2;

    for (i = 0; i < cardTop; i++)
        gScanlineEffectRegBuffers[0][i] = -i;
    for (; i < (s16)cardBottom; i++)
    {
        var = r6 >> 16;
        r6 += r5;
        r5 += r10;
        gScanlineEffectRegBuffers[0][i] = var;
    }
    var = var_24 >> 16;
    for (; i < DISPLAY_HEIGHT; i++)
        gScanlineEffectRegBuffers[0][i] = var;

    sData->allowDMACopy = TRUE;
    if (task->tCardTop <= 0)
        task->tFlipState++;
    return FALSE;
}

static bool8 Task_EndCardFlip(struct Task *task)
{
    ShowBg(1);
    ShowBg(3);
    SetHBlankCallback(NULL);
    DestroyTask(FindTaskIdByFunc(Task_DoCardFlipTask));
    return FALSE;
}

void ShowPlayerTrainerCard(void (*callback)(void))
{
    sData = AllocZeroed(sizeof(*sData));
    sData->callback2 = callback;
    sData->blendColor = (callback == CB2_ReshowFrontierPass) ? RGB_WHITE : RGB_BLACK;
    sData->isLink = (InUnionRoom()) ? TRUE : FALSE;
    sData->language = GAME_LANGUAGE;
    TrainerCard_GenerateCardForPlayer(&sData->trainerCard);
    SetMainCallback2(CB2_InitTrainerCard);
}

void ShowTrainerCardInLink(u8 cardId, void (*callback)(void))
{
    sData = AllocZeroed(sizeof(*sData));
    sData->callback2 = callback;
    sData->isLink = TRUE;
    sData->trainerCard = gTrainerCards[cardId];
    sData->language = gLinkPlayers[cardId].language;
    SetMainCallback2(CB2_InitTrainerCard);
}

static void InitTrainerCardData(void)
{
    u32 i;

    sData->mainState = 0;
    sData->timeColonBlinkTimer = gSaveBlock2Ptr->playTimeVBlanks;
    sData->timeColonInvisible = FALSE;
    sData->onBack = FALSE;
    sData->flipBlendY = 0;
    sData->cardVersion = GetSetCardType();
    for (i = 0; i < TRAINER_CARD_PROFILE_LENGTH; i++)
        CopyEasyChatWord(sData->easyChatProfile[i], sData->trainerCard.easyChatProfile[i]);
}

static u8 GetSetCardType(void)
{
    switch (sData->trainerCard.version)
    {
    case VERSION_SAPPHIRE:
    case VERSION_RUBY:
        sData->cardLayout = CARD_LAYOUT_RS;
        return CARD_VERSION_RS;
    case VERSION_EMERALD:
        if (sData->trainerCard.versionModifier == MODIFIER_HELIODOR)
        {
            sData->cardLayout = sData->trainerCard.cardLayout;
            if (sData->cardLayout == CARD_LAYOUT_RS)
            {
                sData->stats[0] = CARD_STAT_HOF_DEBUT;
                sData->stats[1] = CARD_STAT_LINK_BATTLES;
                sData->stats[2] = CARD_STAT_TRADES;
                sData->stats[3] = CARD_STAT_POKEBLOCKS;
                sData->stats[4] = CARD_STAT_CONTESTS;
                sData->stats[5] = CARD_STAT_BATTLE_TOWER;
            }
            else if (sData->cardLayout == CARD_LAYOUT_FRLG)
            {
                sData->stats[0] = CARD_STAT_HOF_DEBUT;
                sData->stats[1] = CARD_STAT_LINK_BATTLES;
                sData->stats[2] = CARD_STAT_TRADES;
                sData->stats[3] = CARD_STAT_UNION_ROOM;
                sData->stats[4] = CARD_STAT_BERRY_CRUSH;
                sData->stats[5] = CARD_STAT_NONE;
            }
            else if (sData->cardLayout == CARD_LAYOUT_HELIODOR)
            {
                sData->stats[0] = sData->trainerCard.stat0;
                sData->stats[1] = sData->trainerCard.stat1;
                sData->stats[2] = sData->trainerCard.stat2;
                sData->stats[3] = sData->trainerCard.stat3;
                sData->stats[4] = sData->trainerCard.stat4;
                sData->stats[5] = CARD_STAT_NONE;
            }
            else
            {
                sData->stats[0] = CARD_STAT_HOF_DEBUT;
                sData->stats[1] = CARD_STAT_LINK_BATTLES;
                sData->stats[2] = CARD_STAT_TRADES;
                sData->stats[3] = CARD_STAT_POKEBLOCKS;
                sData->stats[4] = CARD_STAT_CONTESTS;
                sData->stats[5] = CARD_STAT_BATTLE_POINTS;
            }
        }
        else
            sData->cardLayout = CARD_LAYOUT_EMERALD;
        return CARD_VERSION_EMERALD + sData->trainerCard.versionModifier;
    case VERSION_CRYSTAL_DUST:
        sData->cardLayout = CARD_LAYOUT_FRLG;
        return CARD_VERSION_CRYSTALDUST;
    default:
        sData->cardLayout = CARD_LAYOUT_FRLG;
        return CARD_VERSION_FRLG + sData->trainerCard.versionModifier;
    }
}

static void CreateTrainerCardTrainerPic(void)
{
    if (InUnionRoom() && gReceivedRemoteLinkPlayers == 1)
    {
        CreateTrainerCardTrainerPicSprite(FacilityClassToPicIndex(sData->trainerCard.facilityClass),
                    sTrainerPicOffset[sData->cardLayout][0],
                    sTrainerPicOffset[sData->cardLayout][1]);
    }
    else if (sData->trainerCard.outfit)
    {
        CreateTrainerCardTrainerPicSprite(FacilityClassToPicIndex(sTrainerPicOutfitFacilityClass[sData->trainerCard.outfit][sData->trainerCard.gender]),
                    sTrainerPicOffset[sData->cardLayout][0],
                    sTrainerPicOffset[sData->cardLayout][1]);
    }
    else
    {
        CreateTrainerCardTrainerPicSprite(FacilityClassToPicIndex(sTrainerPicFacilityClass[sData->cardVersion][sData->trainerCard.gender]),
                    sTrainerPicOffset[sData->cardLayout][0],
                    sTrainerPicOffset[sData->cardLayout][1]);
    }
}
