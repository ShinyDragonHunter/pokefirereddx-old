#include "global.h"
#include "rtc.h"
#include "overworld.h"
#include "random.h"
#include "event_data.h"
#include "fieldmap.h"
#include "field_camera.h"
#include "field_specials.h"
#include "fldeff.h"
#include "strings.h"
#include "string_util.h"
#include "international_string_util.h"
#include "pokemon_storage_system.h"
#include "field_message_box.h"
#include "easy_chat.h"
#include "battle.h"
#include "battle_tower.h"
#include "item.h"
#include "link.h"
#include "main.h"
#include "event_scripts.h"
#include "shop.h"
#include "pokedex.h"
#include "event_object_movement.h"
#include "text.h"
#include "script_menu.h"
#include "naming_screen.h"
#include "malloc.h"
#include "region_map.h"
#include "tv.h"
#include "pokeball.h"
#include "data.h"
#include "constants/battle_frontier.h"
#include "constants/event_objects.h"
#include "constants/items.h"
#include "constants/layouts.h"
#include "constants/maps.h"
#include "constants/metatile_behaviors.h"
#include "constants/metatile_labels.h"
#include "constants/moves.h"
#include "constants/region_map_sections.h"
#include "constants/script_menu.h"

#define LAST_TVSHOW_IDX (TV_SHOWS_COUNT - 1)

#define rbernoulli(num, den) BernoulliTrial(0xFFFF * (num) / (den))

enum {
    TVGROUP_NONE,
    TVGROUP_UNUSED,
    TVGROUP_NORMAL,
    TVGROUP_RECORD_MIX,
    TVGROUP_OUTBREAK,
};

enum {
    SLOT_MACHINE,
    ROULETTE,
};

s8 sCurTVShowSlot;
struct {
    u8 level;
    u16 species;
    u16 move;
} sTV_SecretBaseVisitMonsTemp[10];

static u8 sTVShowMixingNumPlayers;
static u8 sTVShowNewsMixingNumPlayers;
static s8 sTVShowMixingCurSlot;

static EWRAM_DATA u16 sPokemonAnglerSpecies = 0;
static EWRAM_DATA u16 sPokemonAnglerAttemptCounters = 0;
static EWRAM_DATA u16 sFindThatGamerCoinsSpent = 0;
static EWRAM_DATA u8 sFindThatGamerWhichGame = SLOT_MACHINE;
static EWRAM_DATA ALIGNED(4) u8 sRecordMixingPartnersWithoutShowsToShare = 0;
static EWRAM_DATA ALIGNED(4) u8 sTVShowState = 0;

static void ClearPokeNews(void);
static u8 GetTVGroupByShowId(u8);
static u8 FindFirstActiveTVShowThatIsNotAMassOutbreak(void);
static void SetTVMetatilesOnMap(int, int, u16);
static u8 FindAnyPokeNewsOnTheAir(void);
static void TakeGabbyAndTyOffTheAir(void);
static bool8 BernoulliTrial(u16 ratio);
static s8 FindFirstEmptyRecordMixTVShowSlot(TVShow *);
static bool8 IsRecordMixShowAlreadySpawned(u8, bool8);
static void StorePlayerIdInRecordMixShow(TVShow *);
static void DeleteTVShowInArrayByIdx(TVShow *, u8);
static s8 FindFirstEmptyNormalTVShowSlot(TVShow *);
static void TryReplaceOldTVShowOfKind(u8);
static void InterviewBefore_NameRater(void);
static u16 GetRandomDifferentSpeciesSeenByPlayer(u16);
static void Script_FindFirstEmptyNormalTVShowSlot(void);
static void CompactTVShowArray(TVShow *);
static s8 GetFirstEmptyPokeNewsSlot(PokeNews *);
static bool8 IsAddingPokeNewsDisallowed(u8);
static void ClearPokeNewsBySlot(u8);
static void TranslateRubyShows(TVShow *);
static void TranslateJapaneseEmeraldShows(TVShow *);
static void SetMixedTVShows(TVShow *, TVShow *, TVShow *, TVShow *);
static void DeleteExcessMixedShows(void);
static void DeactivateShowsWithUnseenSpecies(void);
static void DeactivateGameCompleteShowsIfNotUnlocked(void);
static s8 FindInactiveShowInArray(TVShow *);
static bool8 TryMixTVShow(TVShow *[], TVShow *[], u8);
static bool8 TryMixNormalTVShow(TVShow *, TVShow *, u8);
static bool8 TryMixRecordMixTVShow(TVShow *, TVShow *, u8);
static bool8 TryMixOutbreakTVShow(TVShow *, TVShow *, u8);
static void DeactivateShow(u8 showIdx);
static void DeactivateShowIfNotSeenSpecies(u16, u8);
static void SetMixedPokeNews(PokeNews *, PokeNews *, PokeNews *, PokeNews *);
static void ClearInvalidPokeNews(void);
static void ClearPokeNewsIfGameNotComplete(void);
static s8 GetPokeNewsSlotIfActive(PokeNews *, u8);
static void InitTryMixPokeNewsShow(PokeNews *[], PokeNews *[]);
static bool8 TryMixPokeNewsShow(PokeNews *, PokeNews *, s8);
static void TVShowDone(void);
static void InterviewAfter_FanClubLetter(void);
static void InterviewAfter_RecentHappenings(void);
static void InterviewAfter_PkmnFanClubOpinions(void);
static void InterviewAfter_Dummy(void);
static void InterviewAfter_BravoTrainerPokemonProfile(void);
static void InterviewAfter_BravoTrainerBattleTowerProfile(void);
static void InitWorldOfMastersShowAttempt(void);
static void TryPutPokemonTodayFailedOnTheAir(void);
static void TryStartRandomMassOutbreak(void);
static void TryPutRandomPokeNewsOnAir(void);
static void SortPurchasesByQuantity(void);
static void UpdateMassOutbreakTimeLeft(u16);
static void TryEndMassOutbreak(u16);
static void UpdatePokeNewsTimeLeft(u16);
static void ResolveWorldOfMastersShow(u16);
static void ResolveNumberOneShow(u16);
static void TryPutFishingAdviceOnAir(void);
static u8 MonDataIdxToRibbon(u8);
static void TryPutNumberOneOnAir(u8);
static bool8 IsPriceDiscounted(u8);
static void TryPutWorldOfMastersOnAir(void);
static void InterviewBefore_FanClubLetter(void);
static void InterviewBefore_RecentHappenings(void);
static void InterviewBefore_PkmnFanClubOpinions(void);
static void InterviewBefore_Dummy(void);
static void InterviewBefore_BravoTrainerBTProfile(void);
static void InterviewBefore_FanClubSpecial(void);
static void ChangeBoxPokemonNickname_CB(void);
static void DoTVShowPokemonFanClubLetter(void);
static void DoTVShowRecentHappenings(void);
static void DoTVShowPokemonFanClubOpinions(void);
static void DoTVShowDummiedOut(void);
static void DoTVShowPokemonNewsMassOutbreak(void);
static void DoTVShowBravoTrainerPokemonProfile(void);
static void DoTVShowBravoTrainerBattleTower(void);
static void DoTVShowPokemonTodaySuccessfulCapture(void);
static void DoTVShowTodaysSmartShopper(void);
static void DoTVShowTheNameRaterShow(void);
static void DoTVShowPokemonBattleUpdate(void);
static void DoTVShowPokemonTodayFailedCapture(void);
static void DoTVShowPokemonAngler(void);
static void DoTVShowTheWorldOfMasters(void);
static void DoTVShowTodaysRivalTrainer(void);
static void DoTVShowDewfordTrendWatcherNetwork(void);
static void DoTVShowHoennTreasureInvestigators(void);
static void DoTVShowFindThatGamer(void);
static void DoTVShowBreakingNewsTV(void);
static void DoTVShowPokemonLotteryWinnerFlashReport(void);
static void DoTVShowThePokemonBattleSeminar(void);
static void DoTVShowTrainerFanClubSpecial(void);
static void DoTVShowTrainerFanClub(void);
static void DoTVShowPokemonNewsBattleFrontier(void);
static void DoTVShowWhatsNo1InHoennToday(void);
static void DoTVShowSafariFanClub(void);

static const struct {
    u16 species;
    u16 moves[MAX_MON_MOVES];
    u8 level;
    u8 location;
} sPokeOutbreakSpeciesList[] = {
    {
        .species = SPECIES_SEEDOT,
        .moves = {MOVE_BIDE, MOVE_HARDEN, MOVE_LEECH_SEED},
        .level = 3, 
        .location = MAP_NUM(ROUTE102)
    },
    {
        .species = SPECIES_NUZLEAF,
        .moves = {MOVE_HARDEN, MOVE_GROWTH, MOVE_NATURE_POWER, MOVE_LEECH_SEED},
        .level = 15, 
        .location = MAP_NUM(ROUTE114),
    },
    {
        .species = SPECIES_SEEDOT,
        .moves = {MOVE_HARDEN, MOVE_GROWTH, MOVE_NATURE_POWER, MOVE_LEECH_SEED},
        .level = 13, 
        .location = MAP_NUM(ROUTE117),
    },
    {
        .species = SPECIES_SEEDOT,
        .moves = {MOVE_GIGA_DRAIN, MOVE_FRUSTRATION, MOVE_SOLAR_BEAM, MOVE_LEECH_SEED},
        .level = 25, 
        .location = MAP_NUM(ROUTE120),
    },
    {
        .species = SPECIES_SKITTY,
        .moves = {MOVE_GROWL, MOVE_TACKLE, MOVE_TAIL_WHIP, MOVE_ATTRACT},
        .level = 8, 
        .location = MAP_NUM(ROUTE116),
    }
};

static const u16 sGoldSymbolFlags[] = {
    FLAG_SYS_TOWER_GOLD,
    FLAG_SYS_DOME_GOLD,
    FLAG_SYS_PALACE_GOLD,
    FLAG_SYS_ARENA_GOLD,
    FLAG_SYS_FACTORY_GOLD,
    FLAG_SYS_PIKE_GOLD,
    FLAG_SYS_PYRAMID_GOLD
};

static const u16 sSilverSymbolFlags[] = {
    FLAG_SYS_TOWER_SILVER,
    FLAG_SYS_DOME_SILVER,
    FLAG_SYS_PALACE_SILVER,
    FLAG_SYS_ARENA_SILVER,
    FLAG_SYS_FACTORY_SILVER,
    FLAG_SYS_PIKE_SILVER,
    FLAG_SYS_PYRAMID_SILVER
};

static const u16 sNumberOneVarsAndThresholds[][2] = {
    {VAR_DAILY_SLOTS, 100},
    {VAR_DAILY_ROULETTE,  50},
    {VAR_DAILY_WILDS, 100},
    {VAR_DAILY_PLANTED_BERRIES,  20},
    {VAR_DAILY_PICKED_BERRIES,  20},
    {VAR_DAILY_BP,  30}
};

static const u8 *const sPokeNewsTextGroup_Upcoming[] = {
    NULL,
    gPokeNewsTextSlateport_Upcoming,
    gPokeNewsTextGameCorner_Upcoming,
    gPokeNewsTextLilycove_Upcoming,
    gPokeNewsTextBlendMaster_Upcoming
};

static const u8 *const sPokeNewsTextGroup_Ongoing[] = {
    NULL,
    gPokeNewsTextSlateport_Ongoing,
    gPokeNewsTextGameCorner_Ongoing,
    gPokeNewsTextLilycove_Ongoing,
    gPokeNewsTextBlendMaster_Ongoing
};

static const u8 *const sPokeNewsTextGroup_Ending[] = {
    NULL,
    gPokeNewsTextSlateport_Ending,
    gPokeNewsTextGameCorner_Ending,
    gPokeNewsTextLilycove_Ending,
    gPokeNewsTextBlendMaster_Ending
};

u8 *const gTVStringVarPtrs[] = {
    gStringVar1,
    gStringVar2,
    gStringVar3
};

static const u8 *const sTVFanClubTextGroup[] = {
    gTVFanClubText00,
    gTVFanClubText01,
    gTVFanClubText02,
    gTVFanClubText03,
    gTVFanClubText04,
    gTVFanClubText05,
    gTVFanClubText06,
    gTVFanClubText07
};

static const u8 *const sTVRecentHappeninssTextGroup[] = {
    gTVRecentHappeningsText00,
    gTVRecentHappeningsText01,
    gTVRecentHappeningsText02,
    gTVRecentHappeningsText03,
    gTVRecentHappeningsText04,
    gTVRecentHappeningsText05
};

static const u8 *const sTVFanClubOpinionsTextGroup[] = {
    gTVFanClubOpinionsText00,
    gTVFanClubOpinionsText01,
    gTVFanClubOpinionsText02,
    gTVFanClubOpinionsText03,
    gTVFanClubOpinionsText04
};

static const u8 *const sTVMassOutbreakTextGroup[] = {
    gTVMassOutbreakText00
};

static const u8 *const sTVPokemonTodaySuccessfulTextGroup[] = {
    gTVPokemonTodaySuccessfulText00,
    gTVPokemonTodaySuccessfulText01,
    gTVPokemonTodaySuccessfulText02,
    gTVPokemonTodaySuccessfulText03,
    gTVPokemonTodaySuccessfulText04,
    gTVPokemonTodaySuccessfulText05,
    gTVPokemonTodaySuccessfulText06,
    gTVPokemonTodaySuccessfulText07,
    gTVPokemonTodaySuccessfulText08,
    gTVPokemonTodaySuccessfulText09,
    gTVPokemonTodaySuccessfulText10,
    gTVPokemonTodaySuccessfulText11
};

static const u8 *const sTVTodaysSmartShopperTextGroup[] = {
    [SMARTSHOPPER_STATE_INTRO]          = SmartShopper_Text_Intro,
    [SMARTSHOPPER_STATE_CLERK_NORMAL]   = SmartShopper_Text_ClerkNormal,
    [SMARTSHOPPER_STATE_RAND_COMMENT_1] = SmartShopper_Text_RandomComment1,
    [SMARTSHOPPER_STATE_RAND_COMMENT_2] = SmartShopper_Text_RandomComment2,
    [SMARTSHOPPER_STATE_RAND_COMMENT_3] = SmartShopper_Text_RandomComment3,
    [SMARTSHOPPER_STATE_RAND_COMMENT_4] = SmartShopper_Text_RandomComment4,
    [SMARTSHOPPER_STATE_SECOND_ITEM]    = SmartShopper_Text_SecondItem,
    [SMARTSHOPPER_STATE_THIRD_ITEM]     = SmartShopper_Text_ThirdItem,
    [SMARTSHOPPER_STATE_DURING_SALE]    = SmartShopper_Text_DuringSale,
    [SMARTSHOPPER_STATE_OUTRO_NORMAL]   = SmartShopper_Text_OutroNormal,
    [SMARTSHOPPER_STATE_IS_VIP]         = SmartShopper_Text_IsVIP,
    [SMARTSHOPPER_STATE_CLERK_MAX]      = SmartShopper_Text_ClerkMax,
    [SMARTSHOPPER_STATE_OUTRO_MAX]      = SmartShopper_Text_OutroMax
};

static const u8 *const sTVBravoTrainerBattleTowerTextGroup[] = {
    gTVBravoTrainerBattleTowerText00,
    gTVBravoTrainerBattleTowerText01,
    gTVBravoTrainerBattleTowerText02,
    gTVBravoTrainerBattleTowerText03,
    gTVBravoTrainerBattleTowerText04,
    gTVBravoTrainerBattleTowerText05,
    gTVBravoTrainerBattleTowerText06,
    gTVBravoTrainerBattleTowerText07,
    gTVBravoTrainerBattleTowerText07,
    gTVBravoTrainerBattleTowerText07,
    gTVBravoTrainerBattleTowerText07,
    gTVBravoTrainerBattleTowerText11,
    gTVBravoTrainerBattleTowerText12,
    gTVBravoTrainerBattleTowerText13,
    gTVBravoTrainerBattleTowerText14
};

static const u8 *const sTVPokemonBattleUpdateTextGroup[] = {
    gTVPokemonBattleUpdateText00,
    gTVPokemonBattleUpdateText01,
    gTVPokemonBattleUpdateText02,
    gTVPokemonBattleUpdateText03,
    gTVPokemonBattleUpdateText04,
    gTVPokemonBattleUpdateText05,
    gTVPokemonBattleUpdateText06,
    gTVPokemonBattleUpdateText07
};

static const u8 *const sTVTrainerFanClubSpecialTextGroup[] = {
    gTVTrainerFanClubSpecialText00,
    gTVTrainerFanClubSpecialText01,
    gTVTrainerFanClubSpecialText02,
    gTVTrainerFanClubSpecialText03,
    gTVTrainerFanClubSpecialText04,
    gTVTrainerFanClubSpecialText05
};

static const u8 *const sTVNameRaterTextGroup[] = {
    gTVNameRaterText00,
    gTVNameRaterText01,
    gTVNameRaterText02,
    gTVNameRaterText03,
    gTVNameRaterText04,
    gTVNameRaterText05,
    gTVNameRaterText06,
    gTVNameRaterText07,
    gTVNameRaterText08,
    gTVNameRaterText09,
    gTVNameRaterText10,
    gTVNameRaterText11,
    gTVNameRaterText12,
    gTVNameRaterText13,
    gTVNameRaterText14,
    gTVNameRaterText15,
    gTVNameRaterText16,
    gTVNameRaterText17,
    gTVNameRaterText18
};

static const u8 *const sTVPokemonTodayFailedTextGroup[] = {
    gTVPokemonTodayFailedText00,
    gTVPokemonTodayFailedText01,
    gTVPokemonTodayFailedText02,
    gTVPokemonTodayFailedText03,
    gTVPokemonTodayFailedText04,
    gTVPokemonTodayFailedText05,
    gTVPokemonTodayFailedText06
};

static const u8 *const sTVPokemonAnslerTextGroup[] = {
    gTVPokemonAnglerText00,
    gTVPokemonAnglerText01
};

static const u8 *const sTVWorldOfMastersTextGroup[] = {
    gTVWorldOfMastersText00,
    gTVWorldOfMastersText01,
    gTVWorldOfMastersText02
};

static const u8 *const sTVTodaysRivalTrainerTextGroup[] = {
    gTVTodaysRivalTrainerText00,
    gTVTodaysRivalTrainerText01,
    gTVTodaysRivalTrainerText02,
    gTVTodaysRivalTrainerText03,
    gTVTodaysRivalTrainerText04,
    gTVTodaysRivalTrainerText05,
    gTVTodaysRivalTrainerText06,
    gTVTodaysRivalTrainerText07,
    gTVTodaysRivalTrainerText08,
    gTVTodaysRivalTrainerText09,
    gTVTodaysRivalTrainerText10
};

static const u8 *const sTVDewfordTrendWatcherNetworkTextGroup[] = {
    [TRENDWATCHER_STATE_INTRO]           = TrendWatcher_Text_Intro,
    [TRENDWATCHER_STATE_TAUGHT_MALE]     = TrendWatcher_Text_MaleTaughtMePhrase,
    [TRENDWATCHER_STATE_TAUGHT_FEMALE]   = TrendWatcher_Text_FemaleTaughtMePhrase,
    [TRENDWATCHER_STATE_PHRASE_HOPELESS] = TrendWatcher_Text_PhraseWasHopeless,
    [TRENDWATCHER_STATE_BIGGER_MALE]     = TrendWatcher_Text_MaleTellMeBigger,
    [TRENDWATCHER_STATE_BIGGER_FEMALE]   = TrendWatcher_Text_FemaleTellMeBigger,
    [TRENDWATCHER_STATE_OUTRO]           = TrendWatcher_Text_Outro
};

static const u8 *const sTVHoennTreasureInvestisatorsTextGroup[] = {
    gTVHoennTreasureInvestigatorsText00,
    gTVHoennTreasureInvestigatorsText01,
    gTVHoennTreasureInvestigatorsText02
};

static const u8 *const sTVFindThatGamerTextGroup[] = {
    gTVFindThatGamerText00,
    gTVFindThatGamerText01,
    gTVFindThatGamerText02,
    gTVFindThatGamerText03
};

static const u8 *const sTVBreakingNewsTextGroup[] = {
    gTVBreakingNewsText00,
    gTVBreakingNewsText01,
    gTVBreakingNewsText02,
    gTVBreakingNewsText03,
    gTVBreakingNewsText04,
    gTVBreakingNewsText05,
    gTVBreakingNewsText06,
    gTVBreakingNewsText07,
    gTVBreakingNewsText08,
    gTVBreakingNewsText09,
    gTVBreakingNewsText10,
    gTVBreakingNewsText11,
    gTVBreakingNewsText12
};

static const u8 *const sTVPokemonLotteryWinnerFlashReportTextGroup[] = {
    gTVPokemonLotteryWinnerFlashReportText00
};

static const u8 *const sTVThePokemonBattleSeminarTextGroup[] = {
    gTVThePokemonBattleSeminarText00,
    gTVThePokemonBattleSeminarText01,
    gTVThePokemonBattleSeminarText02,
    gTVThePokemonBattleSeminarText03,
    gTVThePokemonBattleSeminarText04,
    gTVThePokemonBattleSeminarText05,
    gTVThePokemonBattleSeminarText06
};

static const u8 *const sTVTrainerFanClubTextGroup[] = {
    gTVTrainerFanClubText00,
    gTVTrainerFanClubText01,
    gTVTrainerFanClubText02,
    gTVTrainerFanClubText03,
    gTVTrainerFanClubText04,
    gTVTrainerFanClubText05,
    gTVTrainerFanClubText06,
    gTVTrainerFanClubText07,
    gTVTrainerFanClubText08,
    gTVTrainerFanClubText09,
    gTVTrainerFanClubText10,
    gTVTrainerFanClubText11
};

static const u8 *const sTVPokemonNewsBattleFrontierTextGroup[] = {
    gTVPokemonNewsBattleFrontierText00,
    gTVPokemonNewsBattleFrontierText01,
    gTVPokemonNewsBattleFrontierText02,
    gTVPokemonNewsBattleFrontierText03,
    gTVPokemonNewsBattleFrontierText04,
    gTVPokemonNewsBattleFrontierText05,
    gTVPokemonNewsBattleFrontierText06,
    gTVPokemonNewsBattleFrontierText07,
    gTVPokemonNewsBattleFrontierText08,
    gTVPokemonNewsBattleFrontierText09,
    gTVPokemonNewsBattleFrontierText10,
    gTVPokemonNewsBattleFrontierText11,
    gTVPokemonNewsBattleFrontierText12,
    gTVPokemonNewsBattleFrontierText13,
    gTVPokemonNewsBattleFrontierText14,
    gTVPokemonNewsBattleFrontierText15,
    gTVPokemonNewsBattleFrontierText16,
    gTVPokemonNewsBattleFrontierText17,
    gTVPokemonNewsBattleFrontierText18
};

static const u8 *const sTVWhatsNo1InHoennTodayTextGroup[] = {
    gTVWhatsNo1InHoennTodayText00,
    gTVWhatsNo1InHoennTodayText01,
    gTVWhatsNo1InHoennTodayText02,
    gTVWhatsNo1InHoennTodayText03,
    gTVWhatsNo1InHoennTodayText04,
    gTVWhatsNo1InHoennTodayText05,
    gTVWhatsNo1InHoennTodayText06,
    gTVWhatsNo1InHoennTodayText07,
    gTVWhatsNo1InHoennTodayText08
};

static const u8 *const sTVSafariFanClubTextGroup[] = {
    gTVSafariFanClubText00,
    gTVSafariFanClubText01,
    gTVSafariFanClubText02,
    gTVSafariFanClubText03,
    gTVSafariFanClubText04,
    gTVSafariFanClubText05,
    gTVSafariFanClubText06,
    gTVSafariFanClubText07,
    gTVSafariFanClubText08,
    gTVSafariFanClubText09,
    gTVSafariFanClubText10
};

static const u8 *const sTVInSearchOfTrainersTextGroup[] = {
    gTVInSearchOfTrainersText00,
    gTVInSearchOfTrainersText01,
    gTVInSearchOfTrainersText02,
    gTVInSearchOfTrainersText03,
    gTVInSearchOfTrainersText04,
    gTVInSearchOfTrainersText05,
    gTVInSearchOfTrainersText06,
    gTVInSearchOfTrainersText07,
    gTVInSearchOfTrainersText08
};

void ClearTVShowData(void)
{
    u8 i, j;

    for (i = 0; i < ARRAY_COUNT(gSaveBlock1Ptr->tvShows); i++)
    {
        gSaveBlock1Ptr->tvShows[i].commonInit.kind = 0;
        gSaveBlock1Ptr->tvShows[i].commonInit.active = 0;
        for (j = 0; j < ARRAY_COUNT(gSaveBlock1Ptr->tvShows[i].commonInit.data); j++)
            gSaveBlock1Ptr->tvShows[i].commonInit.data[j] = 0;
    }
    ClearPokeNews();
}

u8 GetRandomActiveShowIdx(void)
{
    u8 i;
    u8 j;
    u8 selIdx;
    TVShow *show;

    // Include all normal TV shows, and up through any present Record Mix shows
    for (i = NUM_NORMAL_TVSHOW_SLOTS; i < LAST_TVSHOW_IDX; i++)
    {
        if (gSaveBlock1Ptr->tvShows[i].common.kind == TVSHOW_OFF_AIR)
            break;
    }
    j = Random() % i;
    selIdx = j;
    do
    {
        if (GetTVGroupByShowId(gSaveBlock1Ptr->tvShows[j].common.kind) != TVGROUP_OUTBREAK)
        {
            if (gSaveBlock1Ptr->tvShows[j].common.active == TRUE)
                return j;
        }
        else
        {
            show = &gSaveBlock1Ptr->tvShows[j];
            if (show->massOutbreak.daysLeft == 0 && show->massOutbreak.active == TRUE)
                return j;
        }

        if (j == 0)
            j = ARRAY_COUNT(gSaveBlock1Ptr->tvShows) - 2;
        else
            j--;

    } while (j != selIdx);
    return 0xFF;
}

u8 FindAnyTVShowOnTheAir(void)
{
    u8 slot = GetRandomActiveShowIdx();
    if (slot == 0xFF)
        return 0xFF;

    if (gSaveBlock1Ptr->outbreakPokemonSpecies != SPECIES_NONE 
     && gSaveBlock1Ptr->tvShows[slot].common.kind == TVSHOW_MASS_OUTBREAK)
        return FindFirstActiveTVShowThatIsNotAMassOutbreak();

    return slot;
}

void UpdateTVScreensOnMap(int width, int height)
{
    FlagSet(FLAG_SYS_TV_WATCH);
    switch (CheckForPlayersHouseNews())
    {
    case PLAYERS_HOUSE_TV_LATI:
        SetTVMetatilesOnMap(width, height, METATILE_Building_TV_On);
        break;
    case PLAYERS_HOUSE_TV_MOVIE:
        // Don't flash TV for movie text in player's house
        break;
//  case PLAYERS_HOUSE_TV_NONE:
    default:
        if (gSaveBlock1Ptr->location.mapGroup == MAP_GROUP(LILYCOVE_CITY_COVE_LILY_MOTEL_1F) 
         && gSaveBlock1Ptr->location.mapNum == MAP_NUM(LILYCOVE_CITY_COVE_LILY_MOTEL_1F))
        {
            // NPC in Lilycove Hotel is always watching TV
            SetTVMetatilesOnMap(width, height, METATILE_Building_TV_On);
        }
        else if (FlagGet(FLAG_SYS_TV_START) && (FindAnyTVShowOnTheAir() != 0xFF || FindAnyPokeNewsOnTheAir() != 0xFF || IsGabbyAndTyShowOnTheAir()))
        {
            FlagClear(FLAG_SYS_TV_WATCH);
            SetTVMetatilesOnMap(width, height, METATILE_Building_TV_On);
        }
        break;
    }
}

static void SetTVMetatilesOnMap(int width, int height, u16 tileId)
{
    int x;
    int y;

    for (y = 0; y < height; y++)
    {
        for (x = 0; x < width; x++)
        {
            if (MapGridGetMetatileBehaviorAt(x, y) == MB_TELEVISION)
                MapGridSetMetatileIdAt(x, y, tileId | METATILE_COLLISION_MASK);
        }
    }
}

void TurnOffTVScreen(void)
{
    SetTVMetatilesOnMap(gBackupMapLayout.width, gBackupMapLayout.height, METATILE_Building_TV_Off);
    DrawWholeMapView();
}

void TurnOnTVScreen(void)
{
    SetTVMetatilesOnMap(gBackupMapLayout.width, gBackupMapLayout.height, METATILE_Building_TV_On);
    DrawWholeMapView();
}

// gSpecialVar_0x8004 here is set from GetRandomActiveShowIdx in EventScript_TryDoTVShow
u8 GetSelectedTVShow(void)
{
    return gSaveBlock1Ptr->tvShows[gSpecialVar_0x8004].common.kind;
}

static u8 FindFirstActiveTVShowThatIsNotAMassOutbreak(void)
{
    u8 i;

    for (i = 0; i < ARRAY_COUNT(gSaveBlock1Ptr->tvShows) - 1; i++)
    {
        if (gSaveBlock1Ptr->tvShows[i].common.kind != TVSHOW_OFF_AIR 
         && gSaveBlock1Ptr->tvShows[i].common.kind != TVSHOW_MASS_OUTBREAK 
         && gSaveBlock1Ptr->tvShows[i].common.active == TRUE)
            return i;
    }
    return 0xFF;
}

u8 GetNextActiveShowIfMassOutbreak(void)
{
    TVShow *tvShow;

    tvShow = &gSaveBlock1Ptr->tvShows[gSpecialVar_0x8004];
    if (tvShow->common.kind == TVSHOW_MASS_OUTBREAK && gSaveBlock1Ptr->outbreakPokemonSpecies != SPECIES_NONE)
        return FindFirstActiveTVShowThatIsNotAMassOutbreak();

    return gSpecialVar_0x8004;
}

// IN SEARCH OF TRAINERS

void ResetGabbyAndTy(void)
{
    gSaveBlock1Ptr->gabbyAndTyData.mon1 = SPECIES_NONE;
    gSaveBlock1Ptr->gabbyAndTyData.mon2 = SPECIES_NONE;
    gSaveBlock1Ptr->gabbyAndTyData.lastMove = MOVE_NONE;
    gSaveBlock1Ptr->gabbyAndTyData.quote[0] = -1;
    gSaveBlock1Ptr->gabbyAndTyData.battleTookMoreThanOneTurn = FALSE;
    gSaveBlock1Ptr->gabbyAndTyData.playerLostAMon = FALSE;
    gSaveBlock1Ptr->gabbyAndTyData.playerUsedHealingItem = FALSE;
    gSaveBlock1Ptr->gabbyAndTyData.playerThrewABall = FALSE;
    gSaveBlock1Ptr->gabbyAndTyData.onAir = FALSE;
    gSaveBlock1Ptr->gabbyAndTyData.valA_5 = 0;
    gSaveBlock1Ptr->gabbyAndTyData.battleTookMoreThanOneTurn2 = FALSE;
    gSaveBlock1Ptr->gabbyAndTyData.playerLostAMon2 = FALSE;
    gSaveBlock1Ptr->gabbyAndTyData.playerUsedHealingItem2 = FALSE;
    gSaveBlock1Ptr->gabbyAndTyData.playerThrewABall2 = FALSE;
    gSaveBlock1Ptr->gabbyAndTyData.valB_4 = 0;
    gSaveBlock1Ptr->gabbyAndTyData.mapnum = 0;
    gSaveBlock1Ptr->gabbyAndTyData.battleNum = 0;
}

void GabbyAndTyBeforeInterview(void)
{
    u8 i;

    gSaveBlock1Ptr->gabbyAndTyData.mon1 = gBattleResults.playerMon1Species;
    gSaveBlock1Ptr->gabbyAndTyData.mon2 = gBattleResults.playerMon2Species;
    gSaveBlock1Ptr->gabbyAndTyData.lastMove = gBattleResults.lastUsedMovePlayer;
    if (gSaveBlock1Ptr->gabbyAndTyData.battleNum != 0xFF)
    {
        gSaveBlock1Ptr->gabbyAndTyData.battleNum++;
    }
    gSaveBlock1Ptr->gabbyAndTyData.battleTookMoreThanOneTurn = gBattleResults.playerMonWasDamaged;
    
    if (gBattleResults.playerFaintCounter != 0)
        gSaveBlock1Ptr->gabbyAndTyData.playerLostAMon = TRUE;
    else
        gSaveBlock1Ptr->gabbyAndTyData.playerLostAMon = FALSE;

    if (gBattleResults.numHealingItemsUsed != 0)
        gSaveBlock1Ptr->gabbyAndTyData.playerUsedHealingItem = TRUE;
    else
        gSaveBlock1Ptr->gabbyAndTyData.playerUsedHealingItem = FALSE;

    if (!gBattleResults.usedMasterBall)
    {
        for (i = 0; i < ARRAY_COUNT(gBattleResults.catchAttempts); i++)
        {
            if (gBattleResults.catchAttempts[i])
            {
                gSaveBlock1Ptr->gabbyAndTyData.playerThrewABall = TRUE;
                break;
            }
        }
    }
    else
    {
        // Player threw a Master Ball at Gabby and Ty
        gSaveBlock1Ptr->gabbyAndTyData.playerThrewABall = TRUE;
    }

    TakeGabbyAndTyOffTheAir();
    if (gSaveBlock1Ptr->gabbyAndTyData.lastMove == MOVE_NONE)
    {
        FlagSet(FLAG_TEMP_1);
    }
}

void GabbyAndTyAfterInterview(void)
{
    gSaveBlock1Ptr->gabbyAndTyData.battleTookMoreThanOneTurn2 = gSaveBlock1Ptr->gabbyAndTyData.battleTookMoreThanOneTurn;
    gSaveBlock1Ptr->gabbyAndTyData.playerLostAMon2 = gSaveBlock1Ptr->gabbyAndTyData.playerLostAMon;
    gSaveBlock1Ptr->gabbyAndTyData.playerUsedHealingItem2 = gSaveBlock1Ptr->gabbyAndTyData.playerUsedHealingItem;
    gSaveBlock1Ptr->gabbyAndTyData.playerThrewABall2 = gSaveBlock1Ptr->gabbyAndTyData.playerThrewABall;
    gSaveBlock1Ptr->gabbyAndTyData.onAir = TRUE;
    gSaveBlock1Ptr->gabbyAndTyData.mapnum = gMapHeader.regionMapSectionId;
    IncrementGameStat(GAME_STAT_GOT_INTERVIEWED);
}

static void TakeGabbyAndTyOffTheAir(void)
{
    gSaveBlock1Ptr->gabbyAndTyData.onAir = FALSE;
}

u8 GabbyAndTyGetBattleNum(void)
{
    if (gSaveBlock1Ptr->gabbyAndTyData.battleNum > 5)
        return (gSaveBlock1Ptr->gabbyAndTyData.battleNum % 3) + 6;

    return gSaveBlock1Ptr->gabbyAndTyData.battleNum;
}

bool8 IsGabbyAndTyShowOnTheAir(void)
{
    return gSaveBlock1Ptr->gabbyAndTyData.onAir;
}

bool8 GabbyAndTyGetLastQuote(void)
{
    if (gSaveBlock1Ptr->gabbyAndTyData.quote[0] == EC_EMPTY_WORD)
    {
        return FALSE;
    }
    CopyEasyChatWord(gStringVar1, gSaveBlock1Ptr->gabbyAndTyData.quote[0]);
    gSaveBlock1Ptr->gabbyAndTyData.quote[0] = -1;
    return TRUE;
}

u8 GabbyAndTyGetLastBattleTrivia(void)
{
    if (!gSaveBlock1Ptr->gabbyAndTyData.battleTookMoreThanOneTurn2)
        return 1;

    if (gSaveBlock1Ptr->gabbyAndTyData.playerThrewABall2)
        return 2;

    if (gSaveBlock1Ptr->gabbyAndTyData.playerUsedHealingItem2)
        return 3;

    if (gSaveBlock1Ptr->gabbyAndTyData.playerLostAMon2)
        return 4;

    return 0;
}

void GetGabbyAndTyLocalIds(void)
{
    switch (GabbyAndTyGetBattleNum())
    {
    case 1:
        gSpecialVar_0x8004 = 14;
        gSpecialVar_0x8005 = 13;
        break;
    case 2:
        gSpecialVar_0x8004 = 5;
        gSpecialVar_0x8005 = 6;
        break;
    case 3:
        gSpecialVar_0x8004 = 18;
        gSpecialVar_0x8005 = 17;
        break;
    case 4:
        gSpecialVar_0x8004 = 21;
        gSpecialVar_0x8005 = 22;
        break;
    case 5:
        gSpecialVar_0x8004 = 8;
        gSpecialVar_0x8005 = 9;
        break;
    case 6:
        gSpecialVar_0x8004 = 19;
        gSpecialVar_0x8005 = 20;
        break;
    case 7:
        gSpecialVar_0x8004 = 23;
        gSpecialVar_0x8005 = 24;
        break;
    case 8:
        gSpecialVar_0x8004 = 10;
        gSpecialVar_0x8005 = 11;
        break;
    }
}

void InterviewAfter(void)
{
    switch (gSpecialVar_0x8005)
    {
    case TVSHOW_FAN_CLUB_LETTER:
        InterviewAfter_FanClubLetter();
        break;
    case TVSHOW_RECENT_HAPPENINGS:
        InterviewAfter_RecentHappenings();
        break;
    case TVSHOW_PKMN_FAN_CLUB_OPINIONS:
        InterviewAfter_PkmnFanClubOpinions();
        break;
    case TVSHOW_DUMMY:
        InterviewAfter_Dummy();
        break;
    case TVSHOW_BRAVO_TRAINER_BATTLE_TOWER_PROFILE:
        InterviewAfter_BravoTrainerBattleTowerProfile();
        break;
    }
}

void TryPutPokemonTodayOnAir(void)
{
    u8 i;
    u16 ballsUsed;
    TVShow *show;
    u32 language2;
    u16 itemLastUsed;

    ballsUsed = 0;
    TryPutRandomPokeNewsOnAir();
    TryStartRandomMassOutbreak();

    // Try either the Failed or Caught version of the show
    if (gBattleResults.caughtMonSpecies == SPECIES_NONE)
    {
        TryPutPokemonTodayFailedOnTheAir();
    }
    else
    {
        InitWorldOfMastersShowAttempt();
        if (!rbernoulli(1, 1) && StringCompare(gSpeciesNames[gBattleResults.caughtMonSpecies], gBattleResults.caughtMonNick))
        {
            sCurTVShowSlot = FindFirstEmptyRecordMixTVShowSlot(gSaveBlock1Ptr->tvShows);
            if (sCurTVShowSlot != -1 && IsRecordMixShowAlreadySpawned(TVSHOW_POKEMON_TODAY_CAUGHT, FALSE) != TRUE)
            {
                for (i = 0; i < ARRAY_COUNT(gBattleResults.catchAttempts); i++)
                {
                    ballsUsed += gBattleResults.catchAttempts[i];
                }
                if (ballsUsed != 0 || gBattleResults.usedMasterBall)
                {
                    ballsUsed = 0;
                    show = &gSaveBlock1Ptr->tvShows[sCurTVShowSlot];
                    show->pokemonToday.kind = TVSHOW_POKEMON_TODAY_CAUGHT;
                    show->pokemonToday.active = FALSE; // NOTE: Show is not active until passed via Record Mix.
                    if (gBattleResults.usedMasterBall)
                    {
                        ballsUsed = 1;
                        itemLastUsed = BALL_MASTER;
                    }
                    else
                    {
                        for (i = 0; i < ARRAY_COUNT(gBattleResults.catchAttempts); i++)
                            ballsUsed += gBattleResults.catchAttempts[i];
                        if (ballsUsed > 255)
                            ballsUsed = 255;
                        itemLastUsed = gLastUsedItem;
                    }
                    show->pokemonToday.nBallsUsed = ballsUsed;
                    show->pokemonToday.ball = itemLastUsed;
                    StringCopy(show->pokemonToday.playerName, gSaveBlock2Ptr->playerName);
                    StringCopy(show->pokemonToday.nickname, gBattleResults.caughtMonNick);
                    language2 = sub_81DB604(show->pokemonToday.nickname);
                    StripExtCtrlCodes(show->pokemonToday.nickname);
                    show->pokemonToday.species = gBattleResults.caughtMonSpecies;
                    StorePlayerIdInRecordMixShow(show);
                    show->pokemonToday.language = gGameLanguage;
                    show->pokemonToday.language2 = language2;
                }
            }
        }
    }
}

// Show is initialized in last slot and updated there until it's
// either triggered or deleted at the end of the day by ResolveWorldOfMastersShow
static void InitWorldOfMastersShowAttempt(void)
{
    TVShow *show = &gSaveBlock1Ptr->tvShows[LAST_TVSHOW_IDX];
    if (show->common.kind != TVSHOW_WORLD_OF_MASTERS)
    {
        DeleteTVShowInArrayByIdx(gSaveBlock1Ptr->tvShows, LAST_TVSHOW_IDX);
        show->worldOfMasters.steps = GetGameStat(GAME_STAT_STEPS);
        show->worldOfMasters.kind = TVSHOW_WORLD_OF_MASTERS;
    }
    show->worldOfMasters.numPokeCaught++;
    show->worldOfMasters.caughtPoke = gBattleResults.caughtMonSpecies;
    show->worldOfMasters.species = gBattleResults.playerMon1Species;
    show->worldOfMasters.location = gMapHeader.regionMapSectionId;
}

static void TryPutPokemonTodayFailedOnTheAir(void)
{
    u16 ballsUsed;
    u8 i;
    TVShow *show;

    if (!rbernoulli(1, 1))
    {
        for (i = 0, ballsUsed = 0; i < ARRAY_COUNT(gBattleResults.catchAttempts); i++)
            ballsUsed += gBattleResults.catchAttempts[i];
        if (ballsUsed > 255)
            ballsUsed = 255;
        if (ballsUsed > 2 && (gBattleOutcome == B_OUTCOME_MON_FLED || gBattleOutcome == B_OUTCOME_WON))
        {
            sCurTVShowSlot = FindFirstEmptyRecordMixTVShowSlot(gSaveBlock1Ptr->tvShows);
            if (sCurTVShowSlot != -1 && IsRecordMixShowAlreadySpawned(TVSHOW_POKEMON_TODAY_FAILED, FALSE) != TRUE)
            {
                show = &gSaveBlock1Ptr->tvShows[sCurTVShowSlot];
                show->pokemonTodayFailed.kind = TVSHOW_POKEMON_TODAY_FAILED;
                show->pokemonTodayFailed.active = FALSE; // NOTE: Show is not active until passed via Record Mix.
                show->pokemonTodayFailed.species = gBattleResults.playerMon1Species;
                show->pokemonTodayFailed.species2 = gBattleResults.lastOpponentSpecies;
                show->pokemonTodayFailed.nBallsUsed = ballsUsed;
                show->pokemonTodayFailed.outcome = gBattleOutcome;
                show->pokemonTodayFailed.location = gMapHeader.regionMapSectionId;
                StringCopy(show->pokemonTodayFailed.playerName, gSaveBlock2Ptr->playerName);
                StorePlayerIdInRecordMixShow(show);
                show->pokemonTodayFailed.language = gGameLanguage;
            }
        }
    }
}

static void StorePlayerIdInRecordMixShow(TVShow *show)
{
    u32 id = GetPlayerIDAsU32();
    show->common.srcTrainerId2Lo = id;
    show->common.srcTrainerId2Hi = id >> 8;
    show->common.srcTrainerIdLo = id;
    show->common.srcTrainerIdHi = id >> 8;
    show->common.trainerIdLo = id;
    show->common.trainerIdHi = id >> 8;
}

static void StorePlayerIdInNormalShow(TVShow *show)
{
    u32 id = GetPlayerIDAsU32();
    show->common.srcTrainerIdLo = id;
    show->common.srcTrainerIdHi = id >> 8;
    show->common.trainerIdLo = id;
    show->common.trainerIdHi = id >> 8;
}

void PutBattleUpdateOnTheAir(u8 opponentLinkPlayerId, u16 move, u16 speciesPlayer, u16 speciesOpponent)
{
    TVShow *show;
    u8 name[32];

    sCurTVShowSlot = FindFirstEmptyNormalTVShowSlot(gSaveBlock1Ptr->tvShows);
    if (sCurTVShowSlot != -1)
    {
        TryReplaceOldTVShowOfKind(TVSHOW_BATTLE_UPDATE);
        if (gSpecialVar_Result != TRUE)
        {
            show = &gSaveBlock1Ptr->tvShows[sCurTVShowSlot];
            show->battleUpdate.kind = TVSHOW_BATTLE_UPDATE;
            show->battleUpdate.active = TRUE;
            StringCopy(show->battleUpdate.playerName, gSaveBlock2Ptr->playerName);

            if (gBattleTypeFlags & BATTLE_TYPE_MULTI)
                show->battleUpdate.battleType = 2;
            else if (gBattleTypeFlags & BATTLE_TYPE_DOUBLE)
                show->battleUpdate.battleType = 1;
            else
                show->battleUpdate.battleType = 0;

            show->battleUpdate.move = move;
            show->battleUpdate.speciesPlayer = speciesPlayer;
            show->battleUpdate.speciesOpponent = speciesOpponent;
            StringCopy(name, gLinkPlayers[opponentLinkPlayerId].name);
            StripExtCtrlCodes(name);
            StringCopy(show->battleUpdate.linkOpponentName, name);
            StorePlayerIdInNormalShow(show);
            show->battleUpdate.language = gGameLanguage;
            if (show->battleUpdate.language == LANGUAGE_JAPANESE || gLinkPlayers[opponentLinkPlayerId].language == LANGUAGE_JAPANESE)
                show->battleUpdate.linkOpponentLanguage = LANGUAGE_JAPANESE;
            else
                show->battleUpdate.linkOpponentLanguage = gLinkPlayers[opponentLinkPlayerId].language;
        }
    }
}

void PutFanClubSpecialOnTheAir(void)
{
    TVShow *show;
    u8 name[32];
    u32 id;

    show = &gSaveBlock1Ptr->tvShows[gSpecialVar_0x8006];
    show->fanClubSpecial.score = gSpecialVar_0x8005 * 10;
    StringCopy(show->fanClubSpecial.playerName, gSaveBlock2Ptr->playerName);
    show->fanClubSpecial.kind = TVSHOW_FAN_CLUB_SPECIAL;
    show->fanClubSpecial.active = TRUE;
    id = GetPlayerIDAsU32();
    show->fanClubSpecial.idLo = id;
    show->fanClubSpecial.idHi = id >> 8;
    StringCopy(name, gStringVar1);
    StripExtCtrlCodes(name);
    StringCopy(show->fanClubSpecial.idolName, name);
    StorePlayerIdInNormalShow(show);
    show->fanClubSpecial.language = gGameLanguage;
    if (show->fanClubSpecial.language == LANGUAGE_JAPANESE || gSaveBlock1Ptr->linkBattleRecords.languages[0] == LANGUAGE_JAPANESE)
        show->fanClubSpecial.idolNameLanguage = LANGUAGE_JAPANESE;
    else
        show->fanClubSpecial.idolNameLanguage = gSaveBlock1Ptr->linkBattleRecords.languages[0];
}

static void InterviewAfter_BravoTrainerBattleTowerProfile(void)
{
    TVShow *show = &gSaveBlock1Ptr->tvShows[sCurTVShowSlot];
    show->bravoTrainerTower.kind = TVSHOW_BRAVO_TRAINER_BATTLE_TOWER_PROFILE;
    show->bravoTrainerTower.active = TRUE;
    StringCopy(show->bravoTrainerTower.trainerName, gSaveBlock2Ptr->playerName);
    StringCopy(show->bravoTrainerTower.pokemonName, gSaveBlock2Ptr->frontier.towerInterview.opponentName);
    show->bravoTrainerTower.species = gSaveBlock2Ptr->frontier.towerInterview.playerSpecies;
    show->bravoTrainerTower.defeatedSpecies = gSaveBlock2Ptr->frontier.towerInterview.opponentSpecies;
    show->bravoTrainerTower.numFights = GetCurrentBattleTowerWinStreak(gSaveBlock2Ptr->frontier.towerLvlMode, 0);
    show->bravoTrainerTower.wonTheChallenge = gSaveBlock2Ptr->frontier.towerBattleOutcome;
    if (gSaveBlock2Ptr->frontier.towerLvlMode == FRONTIER_LVL_50)
        show->bravoTrainerTower.btLevel = 50;
    else
        show->bravoTrainerTower.btLevel = 100;
    show->bravoTrainerTower.interviewResponse = gSpecialVar_0x8004;
    StorePlayerIdInNormalShow(show);
    show->bravoTrainerTower.language = gGameLanguage;
    if (show->bravoTrainerTower.language == LANGUAGE_JAPANESE || gSaveBlock2Ptr->frontier.towerInterview.opponentLanguage == LANGUAGE_JAPANESE)
        show->bravoTrainerTower.pokemonNameLanguage = LANGUAGE_JAPANESE;
    else
        show->bravoTrainerTower.pokemonNameLanguage = gSaveBlock2Ptr->frontier.towerInterview.opponentLanguage;
}

void TryPutSmartShopperOnAir(void)
{
    TVShow *show;
    u8 i;

    if (!(gSaveBlock1Ptr->location.mapGroup == MAP_GROUP(TRAINER_HILL_ENTRANCE) && gSaveBlock1Ptr->location.mapNum == MAP_NUM(TRAINER_HILL_ENTRANCE))
     && !(gSaveBlock1Ptr->location.mapGroup == MAP_GROUP(BATTLE_FRONTIER_MART) && gSaveBlock1Ptr->location.mapNum == MAP_NUM(BATTLE_FRONTIER_MART))
     && !rbernoulli(1, 3))
    {
        sCurTVShowSlot = FindFirstEmptyRecordMixTVShowSlot(gSaveBlock1Ptr->tvShows);
        if (sCurTVShowSlot != -1 && IsRecordMixShowAlreadySpawned(TVSHOW_SMART_SHOPPER, FALSE) != TRUE)
        {
            SortPurchasesByQuantity();
            if (gMartPurchaseHistory[0].quantity >= 20)
            {
                show = &gSaveBlock1Ptr->tvShows[sCurTVShowSlot];
                show->smartshopperShow.kind = TVSHOW_SMART_SHOPPER;
                show->smartshopperShow.active = FALSE; // NOTE: Show is not active until passed via Record Mix.
                show->smartshopperShow.shopLocation = gMapHeader.regionMapSectionId;
                for (i = 0; i < SMARTSHOPPER_NUM_ITEMS; i++)
                {
                    show->smartshopperShow.itemIds[i] = gMartPurchaseHistory[i].itemId;
                    show->smartshopperShow.itemAmounts[i] = gMartPurchaseHistory[i].quantity;
                }
                show->smartshopperShow.priceReduced = GetPriceReduction(POKENEWS_SLATEPORT);
                StringCopy(show->smartshopperShow.playerName, gSaveBlock2Ptr->playerName);
                StorePlayerIdInRecordMixShow(show);
                show->smartshopperShow.language = gGameLanguage;
            }
        }
    }
}

void PutNameRaterShowOnTheAir(void)
{
    TVShow *show;

    InterviewBefore_NameRater();
    if (gSpecialVar_Result != 1)
    {
        GetMonData(&gPlayerParty[gSpecialVar_0x8004], MON_DATA_NICKNAME, gStringVar1);
        if (StringLength(gSaveBlock2Ptr->playerName) > 1 && StringLength(gStringVar1) > 1)
        {
            show = &gSaveBlock1Ptr->tvShows[sCurTVShowSlot];
            show->nameRaterShow.kind = TVSHOW_NAME_RATER_SHOW;
            show->nameRaterShow.active = TRUE;
            show->nameRaterShow.species = GetMonData(&gPlayerParty[gSpecialVar_0x8004], MON_DATA_SPECIES, NULL);
            show->nameRaterShow.random = Random() % 3;
            show->nameRaterShow.random2 = Random() % 2;
            show->nameRaterShow.randomSpecies = GetRandomDifferentSpeciesSeenByPlayer(show->nameRaterShow.species);
            StringCopy(show->nameRaterShow.trainerName, gSaveBlock2Ptr->playerName);
            GetMonData(&gPlayerParty[gSpecialVar_0x8004], MON_DATA_NICKNAME, show->nameRaterShow.pokemonName);
            StripExtCtrlCodes(show->nameRaterShow.pokemonName);
            StorePlayerIdInNormalShow(show);
            show->nameRaterShow.language = gGameLanguage;
            show->nameRaterShow.pokemonNameLanguage = GetMonData(&gPlayerParty[gSpecialVar_0x8004], MON_DATA_LANGUAGE);
        }
    }
}

void StartMassOutbreak(void)
{
    TVShow *show = &gSaveBlock1Ptr->tvShows[gSpecialVar_0x8004];
    gSaveBlock1Ptr->outbreakPokemonSpecies = show->massOutbreak.species;
    gSaveBlock1Ptr->outbreakLocationMapNum = show->massOutbreak.locationMapNum;
    gSaveBlock1Ptr->outbreakLocationMapGroup = show->massOutbreak.locationMapGroup;
    gSaveBlock1Ptr->outbreakPokemonLevel = show->massOutbreak.level;
    gSaveBlock1Ptr->outbreakUnk1 = show->massOutbreak.var02;
    gSaveBlock1Ptr->outbreakUnk2 = show->massOutbreak.var0E;
    gSaveBlock1Ptr->outbreakPokemonMoves[0] = show->massOutbreak.moves[0];
    gSaveBlock1Ptr->outbreakPokemonMoves[1] = show->massOutbreak.moves[1];
    gSaveBlock1Ptr->outbreakPokemonMoves[2] = show->massOutbreak.moves[2];
    gSaveBlock1Ptr->outbreakPokemonMoves[3] = show->massOutbreak.moves[3];
    gSaveBlock1Ptr->outbreakUnk4 = show->massOutbreak.var03;
    gSaveBlock1Ptr->outbreakPokemonProbability = show->massOutbreak.probability;
    gSaveBlock1Ptr->outbreakDaysLeft = 2;
}

static void InterviewAfter_FanClubLetter(void)
{
    TVShow *show = &gSaveBlock1Ptr->tvShows[sCurTVShowSlot];
    show->fanclubLetter.kind = TVSHOW_FAN_CLUB_LETTER;
    show->fanclubLetter.active = TRUE;
    StringCopy(show->fanclubLetter.playerName, gSaveBlock2Ptr->playerName);
    show->fanclubLetter.species = GetMonData(&gPlayerParty[GetLeadMonIndex()], MON_DATA_SPECIES, NULL);
    StorePlayerIdInNormalShow(show);
    show->fanclubLetter.language = gGameLanguage;
}

static void InterviewAfter_RecentHappenings(void)
{
    TVShow *show = &gSaveBlock1Ptr->tvShows[sCurTVShowSlot];
    show->recentHappenings.kind = TVSHOW_RECENT_HAPPENINGS;
    show->recentHappenings.active = TRUE;
    StringCopy(show->recentHappenings.playerName, gSaveBlock2Ptr->playerName);
    show->recentHappenings.species = SPECIES_NONE;
    StorePlayerIdInNormalShow(show);
    show->recentHappenings.language = gGameLanguage;
}

static void InterviewAfter_PkmnFanClubOpinions(void)
{
    TVShow *show = &gSaveBlock1Ptr->tvShows[sCurTVShowSlot];
    show->fanclubOpinions.kind = TVSHOW_PKMN_FAN_CLUB_OPINIONS;
    show->fanclubOpinions.active = TRUE;
    show->fanclubOpinions.friendshipHighNybble = GetMonData(&gPlayerParty[GetLeadMonIndex()], MON_DATA_FRIENDSHIP, NULL) >> 4;
    show->fanclubOpinions.questionAsked = gSpecialVar_0x8007;
    StringCopy(show->fanclubOpinions.playerName, gSaveBlock2Ptr->playerName);
    GetMonData(&gPlayerParty[GetLeadMonIndex()], MON_DATA_NICKNAME, show->fanclubOpinions.nickname);
    StripExtCtrlCodes(show->fanclubOpinions.nickname);
    show->fanclubOpinions.species = GetMonData(&gPlayerParty[GetLeadMonIndex()], MON_DATA_SPECIES, NULL);
    StorePlayerIdInNormalShow(show);
    show->fanclubOpinions.language = gGameLanguage;
    if (gGameLanguage == LANGUAGE_JAPANESE || GetMonData(&gPlayerParty[GetLeadMonIndex()], MON_DATA_LANGUAGE) == LANGUAGE_JAPANESE)
        show->fanclubOpinions.pokemonNameLanguage = LANGUAGE_JAPANESE;
    else
        show->fanclubOpinions.pokemonNameLanguage = GetMonData(&gPlayerParty[GetLeadMonIndex()], MON_DATA_LANGUAGE);
}

static void InterviewAfter_Dummy(void)
{
    TVShow *show = &gSaveBlock1Ptr->tvShows[sCurTVShowSlot];
}

static void TryStartRandomMassOutbreak(void)
{
    u8 i;
    u16 outbreakIdx;
    TVShow *show;

    if (FlagGet(FLAG_SYS_GAME_CLEAR))
    {
        for (i = 0; i < LAST_TVSHOW_IDX; i++)
        {
            if (gSaveBlock1Ptr->tvShows[i].common.kind == TVSHOW_MASS_OUTBREAK)
                return;
        }
        if (!rbernoulli(1, 200))
        {
            sCurTVShowSlot = FindFirstEmptyNormalTVShowSlot(gSaveBlock1Ptr->tvShows);
            if (sCurTVShowSlot != -1)
            {
                outbreakIdx = Random() % ARRAY_COUNT(sPokeOutbreakSpeciesList);
                show = &gSaveBlock1Ptr->tvShows[sCurTVShowSlot];
                show->massOutbreak.kind = TVSHOW_MASS_OUTBREAK;
                show->massOutbreak.active = TRUE;
                show->massOutbreak.level = sPokeOutbreakSpeciesList[outbreakIdx].level;
                show->massOutbreak.var02 = 0;
                show->massOutbreak.var03 = 0;
                show->massOutbreak.species = sPokeOutbreakSpeciesList[outbreakIdx].species;
                show->massOutbreak.var0E = 0;
                show->massOutbreak.moves[0] = sPokeOutbreakSpeciesList[outbreakIdx].moves[0];
                show->massOutbreak.moves[1] = sPokeOutbreakSpeciesList[outbreakIdx].moves[1];
                show->massOutbreak.moves[2] = sPokeOutbreakSpeciesList[outbreakIdx].moves[2];
                show->massOutbreak.moves[3] = sPokeOutbreakSpeciesList[outbreakIdx].moves[3];
                show->massOutbreak.locationMapNum = sPokeOutbreakSpeciesList[outbreakIdx].location;
                show->massOutbreak.locationMapGroup = 0;
                show->massOutbreak.var12 = 0;
                show->massOutbreak.probability = 50;
                show->massOutbreak.var15 = 0;
                show->massOutbreak.daysLeft = 1;
                StorePlayerIdInNormalShow(show);
                show->massOutbreak.language = gGameLanguage;
            }
        }
    }
}

void EndMassOutbreak(void)
{
    gSaveBlock1Ptr->outbreakPokemonSpecies = SPECIES_NONE;
    gSaveBlock1Ptr->outbreakLocationMapNum = 0;
    gSaveBlock1Ptr->outbreakLocationMapGroup = 0;
    gSaveBlock1Ptr->outbreakPokemonLevel = 0;
    gSaveBlock1Ptr->outbreakUnk1 = 0;
    gSaveBlock1Ptr->outbreakUnk2 = 0;
    gSaveBlock1Ptr->outbreakPokemonMoves[0] = MOVE_NONE;
    gSaveBlock1Ptr->outbreakPokemonMoves[1] = MOVE_NONE;
    gSaveBlock1Ptr->outbreakPokemonMoves[2] = MOVE_NONE;
    gSaveBlock1Ptr->outbreakPokemonMoves[3] = MOVE_NONE;
    gSaveBlock1Ptr->outbreakUnk4 = 0;
    gSaveBlock1Ptr->outbreakPokemonProbability = 0;
    gSaveBlock1Ptr->outbreakDaysLeft = 0;
}

void UpdateTVShowsPerDay(u16 days)
{
    UpdateMassOutbreakTimeLeft(days);
    TryEndMassOutbreak(days);
    UpdatePokeNewsTimeLeft(days);
    ResolveWorldOfMastersShow(days);
    ResolveNumberOneShow(days);
}

static void UpdateMassOutbreakTimeLeft(u16 days)
{
    u8 i;
    TVShow *show;

    if (gSaveBlock1Ptr->outbreakPokemonSpecies == SPECIES_NONE)
    {
        for (i = 0; i < LAST_TVSHOW_IDX; i++)
        {
            if (gSaveBlock1Ptr->tvShows[i].massOutbreak.kind == TVSHOW_MASS_OUTBREAK && gSaveBlock1Ptr->tvShows[i].massOutbreak.active == TRUE)
            {
                show = &gSaveBlock1Ptr->tvShows[i];
                if (show->massOutbreak.daysLeft < days)
                    show->massOutbreak.daysLeft = 0;
                else
                    show->massOutbreak.daysLeft -= days;

                break;
            }
        }
    }
}

static void TryEndMassOutbreak(u16 days)
{
    if (gSaveBlock1Ptr->outbreakDaysLeft <= days)
        EndMassOutbreak();
    else
        gSaveBlock1Ptr->outbreakDaysLeft -= days;
}

void RecordFishingAttemptForTV(bool8 caughtFish)
{
    if (caughtFish)
    {
        if (sPokemonAnglerAttemptCounters >> 8 > 4)
            TryPutFishingAdviceOnAir();

        sPokemonAnglerAttemptCounters &= 0xFF;
        if (sPokemonAnglerAttemptCounters != 0xFF)
            sPokemonAnglerAttemptCounters += 0x01;
    }
    else
    {
        if ((u8)sPokemonAnglerAttemptCounters > 4)
            TryPutFishingAdviceOnAir();

        sPokemonAnglerAttemptCounters &= 0xFF00;
        if (sPokemonAnglerAttemptCounters >> 8 != 0xFF)
            sPokemonAnglerAttemptCounters += 0x0100;
    }
}

static void TryPutFishingAdviceOnAir(void)
{
    TVShow *show;

    sCurTVShowSlot = FindFirstEmptyRecordMixTVShowSlot(gSaveBlock1Ptr->tvShows);
    if (sCurTVShowSlot != -1 && IsRecordMixShowAlreadySpawned(TVSHOW_FISHING_ADVICE, FALSE) != TRUE)
    {
        show = &gSaveBlock1Ptr->tvShows[sCurTVShowSlot];
        show->pokemonAngler.kind = TVSHOW_FISHING_ADVICE;
        show->pokemonAngler.active = FALSE; // NOTE: Show is not active until passed via Record Mix.
        show->pokemonAngler.nBites = sPokemonAnglerAttemptCounters;
        show->pokemonAngler.nFails = sPokemonAnglerAttemptCounters >> 8;
        show->pokemonAngler.species = sPokemonAnglerSpecies;
        StringCopy(show->pokemonAngler.playerName, gSaveBlock2Ptr->playerName);
        StorePlayerIdInRecordMixShow(show);
        show->pokemonAngler.language = gGameLanguage;
    }
}

void SetPokemonAnglerSpecies(u16 species)
{
    sPokemonAnglerSpecies = species;
}

// World of Masters is initialized in the last slot by InitWorldOfMastersShowAttempt
// If enough Pokémon were caught during the day the show can be put on air (and will
// be moved out of the last slot).
// Either way the temporary version of the show in the last slot is deleted.
static void ResolveWorldOfMastersShow(u16 days)
{
    TVShow *show;

    show = &gSaveBlock1Ptr->tvShows[LAST_TVSHOW_IDX];
    if (show->worldOfMasters.kind == TVSHOW_WORLD_OF_MASTERS)
    {
        if (show->worldOfMasters.numPokeCaught >= 20)
            TryPutWorldOfMastersOnAir();

        DeleteTVShowInArrayByIdx(gSaveBlock1Ptr->tvShows, LAST_TVSHOW_IDX);
    }
}

static void TryPutWorldOfMastersOnAir(void)
{
    TVShow *show;
    TVShow *show2;

    show = &gSaveBlock1Ptr->tvShows[LAST_TVSHOW_IDX];
    if (!rbernoulli(1, 1))
    {
        sCurTVShowSlot = FindFirstEmptyRecordMixTVShowSlot(gSaveBlock1Ptr->tvShows);
        if (sCurTVShowSlot != -1 && IsRecordMixShowAlreadySpawned(TVSHOW_WORLD_OF_MASTERS, FALSE) != TRUE)
        {
            show2 = &gSaveBlock1Ptr->tvShows[sCurTVShowSlot];
            show2->worldOfMasters.kind = TVSHOW_WORLD_OF_MASTERS;
            show2->worldOfMasters.active = FALSE; // NOTE: Show is not active until passed via Record Mix.
            show2->worldOfMasters.numPokeCaught = show->worldOfMasters.numPokeCaught;
            show2->worldOfMasters.steps = GetGameStat(GAME_STAT_STEPS) - show->worldOfMasters.steps;
            show2->worldOfMasters.caughtPoke = show->worldOfMasters.caughtPoke;
            show2->worldOfMasters.species = show->worldOfMasters.species;
            show2->worldOfMasters.location = show->worldOfMasters.location;
            StringCopy(show2->worldOfMasters.playerName, gSaveBlock2Ptr->playerName);
            StorePlayerIdInRecordMixShow(show2);
            show2->worldOfMasters.language = gGameLanguage;
            DeleteTVShowInArrayByIdx(gSaveBlock1Ptr->tvShows, LAST_TVSHOW_IDX);
        }
    }
}

void TryPutTodaysRivalTrainerOnAir(void)
{
    TVShow *show;
    u32 i;
    u8 nBadges;

    IsRecordMixShowAlreadySpawned(TVSHOW_TODAYS_RIVAL_TRAINER, TRUE); // Delete old version of show
    sCurTVShowSlot = FindFirstEmptyRecordMixTVShowSlot(gSaveBlock1Ptr->tvShows);
    if (sCurTVShowSlot != -1)
    {
        show = &gSaveBlock1Ptr->tvShows[sCurTVShowSlot];
        show->rivalTrainer.kind = TVSHOW_TODAYS_RIVAL_TRAINER;
        show->rivalTrainer.active = FALSE; // NOTE: Show is not active until passed via Record Mix.
        for (i = FLAG_BADGE01_GET, nBadges = 0; i < FLAG_BADGE01_GET + NUM_BADGES; i++)
        {
            if (FlagGet(i))
                nBadges++;
        }
        show->rivalTrainer.badgeCount = nBadges;
        if (IsNationalPokedexEnabled())
            show->rivalTrainer.dexCount = GetNationalPokedexCount(FLAG_GET_CAUGHT);
        else
            show->rivalTrainer.dexCount = GetHoennPokedexCount(FLAG_GET_CAUGHT);
        show->rivalTrainer.location = gMapHeader.regionMapSectionId;
        show->rivalTrainer.mapLayoutId = gMapHeader.mapLayoutId;
        show->rivalTrainer.nSilverSymbols = 0;
        show->rivalTrainer.nGoldSymbols = 0;
        for (i = 0; i < 7; i++)
        {
            if (FlagGet(sSilverSymbolFlags[i]) == TRUE)
                show->rivalTrainer.nSilverSymbols++;

            if (FlagGet(sGoldSymbolFlags[i]) == TRUE)
                show->rivalTrainer.nGoldSymbols++;
        }
        show->rivalTrainer.battlePoints = gSaveBlock2Ptr->frontier.battlePoints;
        StringCopy(show->rivalTrainer.playerName, gSaveBlock2Ptr->playerName);
        StorePlayerIdInRecordMixShow(show);
        show->rivalTrainer.language = gGameLanguage;
    }
}

void TryPutTrendWatcherOnAir(const u16 *words)
{
    TVShow *show;

    sCurTVShowSlot = FindFirstEmptyRecordMixTVShowSlot(gSaveBlock1Ptr->tvShows);
    if (sCurTVShowSlot != -1 && IsRecordMixShowAlreadySpawned(TVSHOW_TREND_WATCHER, FALSE) != TRUE)
    {
        show = &gSaveBlock1Ptr->tvShows[sCurTVShowSlot];
        show->trendWatcher.kind = TVSHOW_TREND_WATCHER;
        show->trendWatcher.active = FALSE; // NOTE: Show is not active until passed via Record Mix.
        show->trendWatcher.gender = gSaveBlock2Ptr->playerGender;
        show->trendWatcher.words[0] = words[0];
        show->trendWatcher.words[1] = words[1];
        StringCopy(show->trendWatcher.playerName, gSaveBlock2Ptr->playerName);
        StorePlayerIdInRecordMixShow(show);
        show->trendWatcher.language = gGameLanguage;
    }
}

void TryPutTreasureInvestigatorsOnAir(void)
{
    TVShow *show;

    sCurTVShowSlot = FindFirstEmptyRecordMixTVShowSlot(gSaveBlock1Ptr->tvShows);
    if (sCurTVShowSlot != -1 && IsRecordMixShowAlreadySpawned(TVSHOW_TREASURE_INVESTIGATORS, FALSE) != TRUE)
    {
        show = &gSaveBlock1Ptr->tvShows[sCurTVShowSlot];
        show->treasureInvestigators.kind = TVSHOW_TREASURE_INVESTIGATORS;
        show->treasureInvestigators.active = FALSE; // NOTE: Show is not active until passed via Record Mix.
        show->treasureInvestigators.item = gSpecialVar_0x8005;
        show->treasureInvestigators.location = gMapHeader.regionMapSectionId;
        show->treasureInvestigators.mapLayoutId = gMapHeader.mapLayoutId;
        StringCopy(show->treasureInvestigators.playerName, gSaveBlock2Ptr->playerName);
        StorePlayerIdInRecordMixShow(show);
        show->treasureInvestigators.language = gGameLanguage;
    }
}

void TryPutFindThatGamerOnAir(u16 nCoinsPaidOut)
{
    TVShow *show;
    bool8 flag;
    u16 nCoinsWon;

    sCurTVShowSlot = FindFirstEmptyRecordMixTVShowSlot(gSaveBlock1Ptr->tvShows);
    if (sCurTVShowSlot != -1 && IsRecordMixShowAlreadySpawned(TVSHOW_FIND_THAT_GAMER, FALSE) != TRUE)
    {
        flag = FALSE;
        switch (sFindThatGamerWhichGame)
        {
        case SLOT_MACHINE:
            if (nCoinsPaidOut >= sFindThatGamerCoinsSpent + 200)
            {
                flag = TRUE;
                nCoinsWon = nCoinsPaidOut - sFindThatGamerCoinsSpent;
                break;
            }
            if (sFindThatGamerCoinsSpent >= 100 && nCoinsPaidOut <= sFindThatGamerCoinsSpent - 100)
            {
                nCoinsWon = sFindThatGamerCoinsSpent - nCoinsPaidOut;
                break;
            }
            return;
        case ROULETTE:
            if (nCoinsPaidOut >= sFindThatGamerCoinsSpent + 50)
            {
                flag = TRUE;
                nCoinsWon = nCoinsPaidOut - sFindThatGamerCoinsSpent;
                break;
            }
            if (sFindThatGamerCoinsSpent >= 50 && nCoinsPaidOut <= sFindThatGamerCoinsSpent - 50)
            {
                nCoinsWon = sFindThatGamerCoinsSpent - nCoinsPaidOut;
                break;
            }
            return;
        default:
            return;
        }
        show = &gSaveBlock1Ptr->tvShows[sCurTVShowSlot];
        show->findThatGamer.kind = TVSHOW_FIND_THAT_GAMER;
        show->findThatGamer.active = FALSE; // NOTE: Show is not active until passed via Record Mix.
        show->findThatGamer.nCoins = nCoinsWon;
        show->findThatGamer.whichGame = sFindThatGamerWhichGame;
        show->findThatGamer.won = flag;
        StringCopy(show->findThatGamer.playerName, gSaveBlock2Ptr->playerName);
        StorePlayerIdInRecordMixShow(show);
        show->findThatGamer.language = gGameLanguage;
    }
}

void AlertTVThatPlayerPlayedSlotMachine(u16 nCoinsSpent)
{
    sFindThatGamerWhichGame = SLOT_MACHINE;
    sFindThatGamerCoinsSpent = nCoinsSpent;
}

void AlertTVThatPlayerPlayedRoulette(u16 nCoinsSpent)
{
    sFindThatGamerWhichGame = ROULETTE;
    sFindThatGamerCoinsSpent = nCoinsSpent;
}

void TryPutBreakingNewsOnAir(void)
{
    TVShow *show;
    u8 i;
    u16 balls;

    sCurTVShowSlot = FindFirstEmptyRecordMixTVShowSlot(gSaveBlock1Ptr->tvShows);
    if (sCurTVShowSlot != -1 && IsRecordMixShowAlreadySpawned(TVSHOW_BREAKING_NEWS, FALSE) != TRUE)
    {
        show = &gSaveBlock1Ptr->tvShows[sCurTVShowSlot];
        show->breakingNews.kind = TVSHOW_BREAKING_NEWS;
        show->breakingNews.active = FALSE; // NOTE: Show is not active until passed via Record Mix.
        balls = 0;
        for (i = 0; i < ARRAY_COUNT(gBattleResults.catchAttempts); i++)
        {
            balls += gBattleResults.catchAttempts[i];
        }
        if (gBattleResults.usedMasterBall)
            balls++;
        show->breakingNews.location = gMapHeader.regionMapSectionId;
        StringCopy(show->breakingNews.playerName, gSaveBlock2Ptr->playerName);
        show->breakingNews.poke1Species = gBattleResults.playerMon1Species;
        switch (gBattleOutcome)
        {
        case B_OUTCOME_LOST:
        case B_OUTCOME_DREW:
            show->breakingNews.kind = TVSHOW_OFF_AIR;
            return;
        case B_OUTCOME_CAUGHT:
            show->breakingNews.outcome = 0;
            break;
        case B_OUTCOME_WON:
            show->breakingNews.outcome = 1;
            break;
        case B_OUTCOME_RAN:
        case B_OUTCOME_PLAYER_TELEPORTED:
        case B_OUTCOME_NO_SAFARI_BALLS:
            show->breakingNews.outcome = 2;
            break;
        case B_OUTCOME_MON_FLED:
        case B_OUTCOME_MON_TELEPORTED:
            show->breakingNews.outcome = 3;
            break;
        }
        show->breakingNews.lastOpponentSpecies = gBattleResults.lastOpponentSpecies;
        switch (show->breakingNews.outcome)
        {
        case 0:
            if (gBattleResults.usedMasterBall)
                show->breakingNews.caughtMonBall = BALL_MASTER;
            else
                show->breakingNews.caughtMonBall = gBattleResults.caughtMonBall;
            show->breakingNews.balls = balls;
            break;
        case 1:
            show->breakingNews.lastUsedMove = gBattleResults.lastUsedMovePlayer;
        case 2:
        case 3:
            break;
        }
        StorePlayerIdInRecordMixShow(show);
        show->breakingNews.language = gGameLanguage;
    }
}

void TryPutLotteryWinnerReportOnAir(void)
{
    TVShow *show;

    sCurTVShowSlot = FindFirstEmptyRecordMixTVShowSlot(gSaveBlock1Ptr->tvShows);
    if (sCurTVShowSlot != -1 && IsRecordMixShowAlreadySpawned(TVSHOW_LOTTO_WINNER, FALSE) != TRUE)
    {
        show = &gSaveBlock1Ptr->tvShows[sCurTVShowSlot];
        show->lottoWinner.kind = TVSHOW_LOTTO_WINNER;
        show->lottoWinner.active = FALSE; // NOTE: Show is not active until passed via Record Mix.
        StringCopy(show->lottoWinner.playerName, gSaveBlock2Ptr->playerName);
        show->lottoWinner.whichPrize = 4 - gSpecialVar_0x8004;
        show->lottoWinner.item = gSpecialVar_0x8005;
        StorePlayerIdInRecordMixShow(show);
        show->lottoWinner.language = gGameLanguage;
    }
}

void TryPutBattleSeminarOnAir(u16 foeSpecies, u16 species, u8 moveIdx, const u16 *movePtr, u16 betterMove)
{
    TVShow *show;
    u8 i;
    u8 j;

    sCurTVShowSlot = FindFirstEmptyRecordMixTVShowSlot(gSaveBlock1Ptr->tvShows);
    if (sCurTVShowSlot != -1 && IsRecordMixShowAlreadySpawned(TVSHOW_BATTLE_SEMINAR, FALSE) != TRUE)
    {
        show = &gSaveBlock1Ptr->tvShows[sCurTVShowSlot];
        show->battleSeminar.kind = TVSHOW_BATTLE_SEMINAR;
        show->battleSeminar.active = FALSE; // NOTE: Show is not active until passed via Record Mix.
        StringCopy(show->battleSeminar.playerName, gSaveBlock2Ptr->playerName);
        show->battleSeminar.foeSpecies = foeSpecies;
        show->battleSeminar.species = species;
        show->battleSeminar.move = movePtr[moveIdx];
        for (i = 0, j = 0; i < MAX_MON_MOVES; i++)
        {
            if (i != moveIdx && movePtr[i])
            {
                show->battleSeminar.otherMoves[j] = movePtr[i];
                j++;
            }
        }
        show->battleSeminar.nOtherMoves = j;
        show->battleSeminar.betterMove = betterMove;
        StorePlayerIdInRecordMixShow(show);
        show->battleSeminar.language = gGameLanguage;
    }
}

void TryPutSafariFanClubOnAir(u8 nMonsCaught, u8 nPkblkUsed)
{
    TVShow *show;

    sCurTVShowSlot = FindFirstEmptyRecordMixTVShowSlot(gSaveBlock1Ptr->tvShows);
    if (sCurTVShowSlot != -1 && IsRecordMixShowAlreadySpawned(TVSHOW_SAFARI_FAN_CLUB, FALSE) != TRUE)
    {
        show = &gSaveBlock1Ptr->tvShows[sCurTVShowSlot];
        show->safariFanClub.kind = TVSHOW_SAFARI_FAN_CLUB;
        show->safariFanClub.active = FALSE; // NOTE: Show is not active until passed via Record Mix.
        StringCopy(show->safariFanClub.playerName, gSaveBlock2Ptr->playerName);
        show->safariFanClub.nMonsCaught = nMonsCaught;
        show->safariFanClub.nPkblkUsed = nPkblkUsed;
        StorePlayerIdInRecordMixShow(show);
        show->safariFanClub.language = gGameLanguage;
    }
}

u8 GetRibbonCount(struct Pokemon *pokemon)
{
    u8 nRibbons;

    nRibbons = 0;
    nRibbons += GetMonData(pokemon, MON_DATA_COOL_RIBBON);
    nRibbons += GetMonData(pokemon, MON_DATA_BEAUTY_RIBBON);
    nRibbons += GetMonData(pokemon, MON_DATA_CUTE_RIBBON);
    nRibbons += GetMonData(pokemon, MON_DATA_SMART_RIBBON);
    nRibbons += GetMonData(pokemon, MON_DATA_TOUGH_RIBBON);
    nRibbons += GetMonData(pokemon, MON_DATA_CHAMPION_RIBBON);
    nRibbons += GetMonData(pokemon, MON_DATA_WINNING_RIBBON);
    nRibbons += GetMonData(pokemon, MON_DATA_VICTORY_RIBBON);
    nRibbons += GetMonData(pokemon, MON_DATA_ARTIST_RIBBON);
    nRibbons += GetMonData(pokemon, MON_DATA_EFFORT_RIBBON);
    nRibbons += GetMonData(pokemon, MON_DATA_MARINE_RIBBON);
    nRibbons += GetMonData(pokemon, MON_DATA_LAND_RIBBON);
    nRibbons += GetMonData(pokemon, MON_DATA_SKY_RIBBON);
    nRibbons += GetMonData(pokemon, MON_DATA_COUNTRY_RIBBON);
    nRibbons += GetMonData(pokemon, MON_DATA_NATIONAL_RIBBON);
    nRibbons += GetMonData(pokemon, MON_DATA_EARTH_RIBBON);
    nRibbons += GetMonData(pokemon, MON_DATA_WORLD_RIBBON);
    return nRibbons;
}

static u8 MonDataIdxToRibbon(u8 monDataIdx)
{
    if (monDataIdx == MON_DATA_CHAMPION_RIBBON) return CHAMPION_RIBBON;
    if (monDataIdx == MON_DATA_COOL_RIBBON)     return COOL_RIBBON_NORMAL;
    if (monDataIdx == MON_DATA_BEAUTY_RIBBON)   return BEAUTY_RIBBON_NORMAL;
    if (monDataIdx == MON_DATA_CUTE_RIBBON)     return CUTE_RIBBON_NORMAL;
    if (monDataIdx == MON_DATA_SMART_RIBBON)    return SMART_RIBBON_NORMAL;
    if (monDataIdx == MON_DATA_TOUGH_RIBBON)    return TOUGH_RIBBON_NORMAL;
    if (monDataIdx == MON_DATA_WINNING_RIBBON)  return WINNING_RIBBON;
    if (monDataIdx == MON_DATA_VICTORY_RIBBON)  return VICTORY_RIBBON;
    if (monDataIdx == MON_DATA_ARTIST_RIBBON)   return ARTIST_RIBBON;
    if (monDataIdx == MON_DATA_EFFORT_RIBBON)   return EFFORT_RIBBON;
    if (monDataIdx == MON_DATA_MARINE_RIBBON)   return MARINE_RIBBON;
    if (monDataIdx == MON_DATA_LAND_RIBBON)     return LAND_RIBBON;
    if (monDataIdx == MON_DATA_SKY_RIBBON)      return SKY_RIBBON;
    if (monDataIdx == MON_DATA_COUNTRY_RIBBON)  return COUNTRY_RIBBON;
    if (monDataIdx == MON_DATA_NATIONAL_RIBBON) return NATIONAL_RIBBON;
    if (monDataIdx == MON_DATA_EARTH_RIBBON)    return EARTH_RIBBON;
    if (monDataIdx == MON_DATA_WORLD_RIBBON)    return WORLD_RIBBON;
    return CHAMPION_RIBBON;
}

void TryPutTrainerFanClubOnAir(void)
{
    TVShow *show;

    sCurTVShowSlot = FindFirstEmptyRecordMixTVShowSlot(gSaveBlock1Ptr->tvShows);
    if (sCurTVShowSlot != -1 && IsRecordMixShowAlreadySpawned(TVSHOW_TRAINER_FAN_CLUB, FALSE) != TRUE)
    {
        show = &gSaveBlock1Ptr->tvShows[sCurTVShowSlot];
        show->trainerFanClub.kind = TVSHOW_TRAINER_FAN_CLUB;
        show->trainerFanClub.active = FALSE; // NOTE: Show is not active until passed via Record Mix.
        StringCopy(show->trainerFanClub.playerName, gSaveBlock2Ptr->playerName);
        show->trainerFanClub.words[0] = gSaveBlock1Ptr->easyChatProfile[0];
        show->trainerFanClub.words[1] = gSaveBlock1Ptr->easyChatProfile[1];
        StorePlayerIdInRecordMixShow(show);
        show->trainerFanClub.language = gGameLanguage;
    }
}

bool8 ShouldHideFanClubInterviewer(void)
{
    sCurTVShowSlot = FindFirstEmptyNormalTVShowSlot(gSaveBlock1Ptr->tvShows);
    if (sCurTVShowSlot == -1)
        return TRUE;

    TryReplaceOldTVShowOfKind(TVSHOW_FAN_CLUB_SPECIAL);
    if (gSpecialVar_Result == TRUE)
        return TRUE;

    if (gSaveBlock1Ptr->linkBattleRecords.entries[0].name[0] == EOS)
        return TRUE;
    
    return FALSE;
}

bool8 ShouldAirFrontierTVShow(void)
{
    u32 playerId;
    u8 showIdx;
    TVShow *shows;

    if (IsRecordMixShowAlreadySpawned(TVSHOW_FRONTIER, FALSE) == TRUE)
    {
        shows = gSaveBlock1Ptr->tvShows;
        playerId = GetPlayerIDAsU32();
        for (showIdx = NUM_NORMAL_TVSHOW_SLOTS; showIdx < LAST_TVSHOW_IDX; showIdx++)
        {
            if (shows[showIdx].common.kind == TVSHOW_FRONTIER && (playerId & 0xFF) == shows[showIdx].common.trainerIdLo && ((playerId >> 8) & 0xFF) == shows[showIdx].common.trainerIdHi)
            {
                DeleteTVShowInArrayByIdx(gSaveBlock1Ptr->tvShows, showIdx);
                CompactTVShowArray(gSaveBlock1Ptr->tvShows);
                return TRUE;
            }
        }
    }
    sCurTVShowSlot = FindFirstEmptyRecordMixTVShowSlot(gSaveBlock1Ptr->tvShows);
    if (sCurTVShowSlot == -1)
        return FALSE;

    return TRUE;
}

void TryPutFrontierTVShowOnAir(u16 winStreak, u8 facilityAndMode)
{
    TVShow *show;

    sCurTVShowSlot = FindFirstEmptyRecordMixTVShowSlot(gSaveBlock1Ptr->tvShows);
    if (sCurTVShowSlot != -1)
    {
        show = &gSaveBlock1Ptr->tvShows[sCurTVShowSlot];
        show->frontier.kind = TVSHOW_FRONTIER;
        show->frontier.active = FALSE; // NOTE: Show is not active until passed via Record Mix.
        StringCopy(show->frontier.playerName, gSaveBlock2Ptr->playerName);
        show->frontier.winStreak = winStreak;
        show->frontier.facilityAndMode = facilityAndMode;
        switch (facilityAndMode)
        {
        case FRONTIER_SHOW_TOWER_SINGLES:
        case FRONTIER_SHOW_DOME_SINGLES:
        case FRONTIER_SHOW_DOME_DOUBLES:
        case FRONTIER_SHOW_FACTORY_SINGLES:
        case FRONTIER_SHOW_FACTORY_DOUBLES:
        case FRONTIER_SHOW_PIKE:
        case FRONTIER_SHOW_ARENA:
        case FRONTIER_SHOW_PALACE_SINGLES:
        case FRONTIER_SHOW_PALACE_DOUBLES:
        case FRONTIER_SHOW_PYRAMID:
            show->frontier.species1 = GetMonData(&gPlayerParty[0], MON_DATA_SPECIES, NULL);
            show->frontier.species2 = GetMonData(&gPlayerParty[1], MON_DATA_SPECIES, NULL);
            show->frontier.species3 = GetMonData(&gPlayerParty[2], MON_DATA_SPECIES, NULL);
            break;
        case FRONTIER_SHOW_TOWER_DOUBLES:
            show->frontier.species1 = GetMonData(&gPlayerParty[0], MON_DATA_SPECIES, NULL);
            show->frontier.species2 = GetMonData(&gPlayerParty[1], MON_DATA_SPECIES, NULL);
            show->frontier.species3 = GetMonData(&gPlayerParty[2], MON_DATA_SPECIES, NULL);
            show->frontier.species4 = GetMonData(&gPlayerParty[3], MON_DATA_SPECIES, NULL);
            break;
        case FRONTIER_SHOW_TOWER_MULTIS:
            show->frontier.species1 = GetMonData(&gPlayerParty[0], MON_DATA_SPECIES, NULL);
            show->frontier.species2 = GetMonData(&gPlayerParty[1], MON_DATA_SPECIES, NULL);
            break;
        case FRONTIER_SHOW_TOWER_LINK_MULTIS:
            show->frontier.species1 = GetMonData(&gSaveBlock1Ptr->playerParty[gSaveBlock2Ptr->frontier.selectedPartyMons[0] - 1], MON_DATA_SPECIES, NULL);
            show->frontier.species2 = GetMonData(&gSaveBlock1Ptr->playerParty[gSaveBlock2Ptr->frontier.selectedPartyMons[1] - 1], MON_DATA_SPECIES, NULL);
            break;
        }
        StorePlayerIdInRecordMixShow(show);
        show->frontier.language = gGameLanguage;
    }
}

// Check var thresholds required to trigger the Number One show
// The vars are reset afterwards regardless
static void ResolveNumberOneShow(u16 days)
{
    u8 i;

    for (i = 0; i < ARRAY_COUNT(sNumberOneVarsAndThresholds); i++)
    {
        if (VarGet(sNumberOneVarsAndThresholds[i][0]) >= sNumberOneVarsAndThresholds[i][1])
        {
            TryPutNumberOneOnAir(i);
            break;
        }
    }

    for (i = 0; i < ARRAY_COUNT(sNumberOneVarsAndThresholds); i++)
        VarSet(sNumberOneVarsAndThresholds[i][0], 0);
}

static void TryPutNumberOneOnAir(u8 actionIdx)
{
    TVShow *show;

    IsRecordMixShowAlreadySpawned(TVSHOW_NUMBER_ONE, TRUE); // Delete old version of show
    sCurTVShowSlot = FindFirstEmptyRecordMixTVShowSlot(gSaveBlock1Ptr->tvShows);
    if (sCurTVShowSlot != -1)
    {
        show = &gSaveBlock1Ptr->tvShows[sCurTVShowSlot];
        show->numberOne.kind = TVSHOW_NUMBER_ONE;
        show->numberOne.active = FALSE; // NOTE: Show is not active until passed via Record Mix.
        StringCopy(show->numberOne.playerName, gSaveBlock2Ptr->playerName);
        show->numberOne.actionIdx = actionIdx;
        show->numberOne.count = VarGet(sNumberOneVarsAndThresholds[actionIdx][0]);
        StorePlayerIdInRecordMixShow(show);
        show->numberOne.language = gGameLanguage;
    }
}

void IncrementDailySlotsUses(void)
{
    VarSet(VAR_DAILY_SLOTS, VarGet(VAR_DAILY_SLOTS) + 1);
}

void IncrementDailyRouletteUses(void)
{
    VarSet(VAR_DAILY_ROULETTE, VarGet(VAR_DAILY_ROULETTE) + 1);
}

void IncrementDailyWildBattles(void)
{
    VarSet(VAR_DAILY_WILDS, VarGet(VAR_DAILY_WILDS) + 1);
}

void IncrementDailyPlantedBerries(void)
{
    VarSet(VAR_DAILY_PLANTED_BERRIES, VarGet(VAR_DAILY_PLANTED_BERRIES) + 1);
}

void IncrementDailyPickedBerries(void)
{
    VarSet(VAR_DAILY_PICKED_BERRIES, VarGet(VAR_DAILY_PICKED_BERRIES) + gSpecialVar_0x8006);
}

void IncrementDailyBattlePoints(u16 delta)
{
    VarSet(VAR_DAILY_BP, VarGet(VAR_DAILY_BP) + delta);
}

// PokeNews

static void TryPutRandomPokeNewsOnAir(void)
{
    if (FlagGet(FLAG_SYS_GAME_CLEAR))
    {
        sCurTVShowSlot = GetFirstEmptyPokeNewsSlot(gSaveBlock1Ptr->pokeNews);
        if (sCurTVShowSlot != -1 && rbernoulli(1, 100) != TRUE)
        {
            u8 newsKind = (Random() % NUM_POKENEWS_TYPES) + POKENEWS_SLATEPORT;
            if (IsAddingPokeNewsDisallowed(newsKind) != TRUE)
            {
                gSaveBlock1Ptr->pokeNews[sCurTVShowSlot].kind = newsKind;
                gSaveBlock1Ptr->pokeNews[sCurTVShowSlot].days = 4;
                gSaveBlock1Ptr->pokeNews[sCurTVShowSlot].state = 1;
            }
        }
    }
}

static s8 GetFirstEmptyPokeNewsSlot(PokeNews *pokeNews)
{
    s8 i;

    for (i = 0; i < POKE_NEWS_COUNT; i++)
    {
        if (pokeNews[i].kind == POKENEWS_NONE)
            return i;
    }
    return -1;
}

static void ClearPokeNews(void)
{
    u8 i;

    for (i = 0; i < POKE_NEWS_COUNT; i++)
        ClearPokeNewsBySlot(i);
}

static void ClearPokeNewsBySlot(u8 i)
{
    gSaveBlock1Ptr->pokeNews[i].kind = POKENEWS_NONE;
    gSaveBlock1Ptr->pokeNews[i].state = FALSE;
    gSaveBlock1Ptr->pokeNews[i].days = 0;
}

static void CompactPokeNews(void)
{
    u8 i;
    u8 j;

    for (i = 0; i < POKE_NEWS_COUNT - 1; i++)
    {
        if (gSaveBlock1Ptr->pokeNews[i].kind == POKENEWS_NONE)
        {
            for (j = i + 1; j < POKE_NEWS_COUNT; j++)
            {
                if (gSaveBlock1Ptr->pokeNews[j].kind != POKENEWS_NONE)
                {
                    gSaveBlock1Ptr->pokeNews[i] = gSaveBlock1Ptr->pokeNews[j];
                    ClearPokeNewsBySlot(j);
                    break;
                }
            }
        }
    }
}

static u8 FindAnyPokeNewsOnTheAir(void)
{
    u8 i;

    for (i = 0; i < POKE_NEWS_COUNT; i++)
    {
        if (gSaveBlock1Ptr->pokeNews[i].kind != POKENEWS_NONE 
         && gSaveBlock1Ptr->pokeNews[i].state == 1 
         && gSaveBlock1Ptr->pokeNews[i].days < 3)
            return i;
    }
    return 0xFF;
}

void DoPokeNews(void)
{
    u8 i;
    u16 n;

    i = FindAnyPokeNewsOnTheAir();
    if (i == 0xFF)
    {
        gSpecialVar_Result = FALSE;
    }
    else
    {
        if (gSaveBlock1Ptr->pokeNews[i].days == 0)
        {
            gSaveBlock1Ptr->pokeNews[i].state = 2;
            if (gLocalTime.hours < 20)
                ShowFieldMessage(sPokeNewsTextGroup_Ongoing[gSaveBlock1Ptr->pokeNews[i].kind]);
            else
                ShowFieldMessage(sPokeNewsTextGroup_Ending[gSaveBlock1Ptr->pokeNews[i].kind]);
        }
        else
        {
            n = gSaveBlock1Ptr->pokeNews[i].days;
            ConvertIntToDecimalStringN(gStringVar1, n, STR_CONV_MODE_LEFT_ALIGN, 1);
            gSaveBlock1Ptr->pokeNews[i].state = 0;
            ShowFieldMessage(sPokeNewsTextGroup_Upcoming[gSaveBlock1Ptr->pokeNews[i].kind]);
        }
        gSpecialVar_Result = TRUE;
    }
}

bool8 GetPriceReduction(u8 newsKind)
{
    u8 i;

    if (newsKind == POKENEWS_NONE)
        return FALSE;

    for (i = 0; i < POKE_NEWS_COUNT; i++)
    {
        if (gSaveBlock1Ptr->pokeNews[i].kind == newsKind)
        {
            if (gSaveBlock1Ptr->pokeNews[i].state == 2 && IsPriceDiscounted(newsKind))
                return TRUE;

            return FALSE;
        }
    }
    return FALSE;
}

static bool8 IsPriceDiscounted(u8 newsKind)
{
    switch (newsKind)
    {
    case POKENEWS_SLATEPORT:
        if (gSaveBlock1Ptr->location.mapGroup == MAP_GROUP(SLATEPORT_CITY) && gSaveBlock1Ptr->location.mapNum == MAP_NUM(SLATEPORT_CITY) && gSpecialVar_LastTalked == 25)
            return TRUE;
        return FALSE;
    case POKENEWS_LILYCOVE:
        if (gSaveBlock1Ptr->location.mapGroup == MAP_GROUP(LILYCOVE_CITY_DEPARTMENT_STORE_ROOFTOP) && gSaveBlock1Ptr->location.mapNum == MAP_NUM(LILYCOVE_CITY_DEPARTMENT_STORE_ROOFTOP))
            return TRUE;
        return FALSE;
    }
    return TRUE;
}

static bool8 IsAddingPokeNewsDisallowed(u8 newsKind)
{
    u8 i;
    if (newsKind == POKENEWS_NONE)
        return TRUE;

    // Check if this type of news is already active
    for (i = 0; i < POKE_NEWS_COUNT; i++)
    {
        if (gSaveBlock1Ptr->pokeNews[i].kind == newsKind)
            return TRUE;
    }
    return FALSE;
}

static void UpdatePokeNewsTimeLeft(u16 days)
{
    u8 i;

    for (i = 0; i < POKE_NEWS_COUNT; i++)
    {
        if (gSaveBlock1Ptr->pokeNews[i].kind != POKENEWS_NONE)
        {
            if (gSaveBlock1Ptr->pokeNews[i].days < days)
            {
                ClearPokeNewsBySlot(i);
            }
            else
            {
                if (gSaveBlock1Ptr->pokeNews[i].state == 0 && FlagGet(FLAG_SYS_GAME_CLEAR) == TRUE)
                    gSaveBlock1Ptr->pokeNews[i].state = 1;

                gSaveBlock1Ptr->pokeNews[i].days -= days;
            }
        }
    }
    CompactPokeNews();
}

void ConvertIntToDecimalString(u8 varIdx, int value)
{
    int nDigits = CountDigits(value);
    ConvertIntToDecimalStringN(gTVStringVarPtrs[varIdx], value, STR_CONV_MODE_LEFT_ALIGN, nDigits);
}

size_t CountDigits(int value)
{
    if (value / 10 == 0)        return 1;
    if (value / 100 == 0)       return 2;
    if (value / 1000 == 0)      return 3;
    if (value / 10000 == 0)     return 4;
    if (value / 100000 == 0)    return 5;
    if (value / 1000000 == 0)   return 6;
    if (value / 10000000 == 0)  return 7;
    if (value / 100000000 == 0) return 8;

    return 1;
}

static void SmartShopper_BufferPurchaseTotal(u8 varIdx, TVShow *show)
{
    u8 i;
    int price;

    price = 0;
    for (i = 0; i < SMARTSHOPPER_NUM_ITEMS; i++)
    {
        if (show->smartshopperShow.itemIds[i] != ITEM_NONE)
            price += ItemId_GetPrice(show->smartshopperShow.itemIds[i]) * show->smartshopperShow.itemAmounts[i];
    }

    if (show->smartshopperShow.priceReduced == TRUE)
        ConvertIntToDecimalString(varIdx, price >> 1);
    else
        ConvertIntToDecimalString(varIdx, price);
}

static bool8 IsRecordMixShowAlreadySpawned(u8 kind, bool8 delete)
{
    u32 playerId;
    TVShow *shows;
    u8 i;

    shows = gSaveBlock1Ptr->tvShows;
    playerId = GetPlayerIDAsU32();
    for (i = NUM_NORMAL_TVSHOW_SLOTS; i < LAST_TVSHOW_IDX; i++)
    {
        if (shows[i].common.kind == kind 
         && (playerId & 0xFF) == shows[i].common.trainerIdLo 
         && ((playerId >> 8) & 0xFF) == shows[i].common.trainerIdHi)
        {
            if (delete == TRUE)
            {
                DeleteTVShowInArrayByIdx(gSaveBlock1Ptr->tvShows, i);
                CompactTVShowArray(gSaveBlock1Ptr->tvShows);
            }
            return TRUE;
        }
    }
    return FALSE;
}

static void SortPurchasesByQuantity(void)
{
    u8 i, j;
    u16 tmpId;
    u16 tmpQn;

    for (i = 0; i < SMARTSHOPPER_NUM_ITEMS - 1; i++)
    {
        for (j = i + 1; j < SMARTSHOPPER_NUM_ITEMS; j++)
        {
            if (gMartPurchaseHistory[i].quantity < gMartPurchaseHistory[j].quantity)
            {
                tmpId = gMartPurchaseHistory[i].itemId;
                tmpQn = gMartPurchaseHistory[i].quantity;
                gMartPurchaseHistory[i].itemId = gMartPurchaseHistory[j].itemId;
                gMartPurchaseHistory[i].quantity = gMartPurchaseHistory[j].quantity;
                gMartPurchaseHistory[j].itemId = tmpId;
                gMartPurchaseHistory[j].quantity = tmpQn;
            }
        }
    }
}

static void TryReplaceOldTVShowOfKind(u8 kind)
{
    u8 i;
    for (i = 0; i < NUM_NORMAL_TVSHOW_SLOTS; i++)
    {
        if (gSaveBlock1Ptr->tvShows[i].common.kind == kind)
        {
            if (gSaveBlock1Ptr->tvShows[i].common.active == TRUE)
            {
                // Old TV show is still active, don't replace
                gSpecialVar_Result = TRUE;
            }
            else
            {
                // Old TV show is inactive, replace it and get new slot
                DeleteTVShowInArrayByIdx(gSaveBlock1Ptr->tvShows, i);
                CompactTVShowArray(gSaveBlock1Ptr->tvShows);
                Script_FindFirstEmptyNormalTVShowSlot();
            }
            return;
        }
    }

    // Old TV show doesn't exist, just get new slot
    Script_FindFirstEmptyNormalTVShowSlot();
}

void InterviewBefore(void)
{
    gSpecialVar_Result = FALSE;
    switch (gSpecialVar_0x8005)
    {
    case TVSHOW_FAN_CLUB_LETTER:
        InterviewBefore_FanClubLetter();
        break;
    case TVSHOW_RECENT_HAPPENINGS:
        InterviewBefore_RecentHappenings();
        break;
    case TVSHOW_PKMN_FAN_CLUB_OPINIONS:
        InterviewBefore_PkmnFanClubOpinions();
        break;
    case TVSHOW_DUMMY:
        InterviewBefore_Dummy();
        break;
    case TVSHOW_NAME_RATER_SHOW:
        InterviewBefore_NameRater();
        break;
    case TVSHOW_BRAVO_TRAINER_BATTLE_TOWER_PROFILE:
        InterviewBefore_BravoTrainerBTProfile();
        break;
    case TVSHOW_FAN_CLUB_SPECIAL:
        InterviewBefore_FanClubSpecial();
        break;
    }
}

static void InterviewBefore_FanClubLetter(void)
{
    TryReplaceOldTVShowOfKind(TVSHOW_FAN_CLUB_LETTER);
    if (!gSpecialVar_Result)
    {
        StringCopy(gStringVar1, gSpeciesNames[GetMonData(&gPlayerParty[GetLeadMonIndex()], MON_DATA_SPECIES, NULL)]);
        InitializeEasyChatWordArray(gSaveBlock1Ptr->tvShows[sCurTVShowSlot].fanclubLetter.words, 6);
    }
}

static void InterviewBefore_RecentHappenings(void)
{
    TryReplaceOldTVShowOfKind(TVSHOW_RECENT_HAPPENINGS);
    if (!gSpecialVar_Result)
    {
        InitializeEasyChatWordArray(gSaveBlock1Ptr->tvShows[sCurTVShowSlot].recentHappenings.words, 6);
    }
}

static void InterviewBefore_PkmnFanClubOpinions(void)
{
    TryReplaceOldTVShowOfKind(TVSHOW_PKMN_FAN_CLUB_OPINIONS);
    if (!gSpecialVar_Result)
    {
        StringCopy(gStringVar1, gSpeciesNames[GetMonData(&gPlayerParty[GetLeadMonIndex()], MON_DATA_SPECIES, NULL)]);
        GetMonData(&gPlayerParty[GetLeadMonIndex()], MON_DATA_NICKNAME, gStringVar2);
        StringGetEnd10(gStringVar2);
        InitializeEasyChatWordArray(gSaveBlock1Ptr->tvShows[sCurTVShowSlot].fanclubOpinions.words, 2);
    }
}

static void InterviewBefore_Dummy(void)
{
    gSpecialVar_Result = TRUE;
}

static void InterviewBefore_NameRater(void)
{
    TryReplaceOldTVShowOfKind(TVSHOW_NAME_RATER_SHOW);
}

static void InterviewBefore_BravoTrainerBTProfile(void)
{
    TryReplaceOldTVShowOfKind(TVSHOW_BRAVO_TRAINER_BATTLE_TOWER_PROFILE);
    if (!gSpecialVar_Result)
        InitializeEasyChatWordArray(gSaveBlock1Ptr->tvShows[sCurTVShowSlot].bravoTrainerTower.words, 1);
}

static void InterviewBefore_FanClubSpecial(void)
{
    TryReplaceOldTVShowOfKind(TVSHOW_FAN_CLUB_SPECIAL);
    if (!gSpecialVar_Result)
        InitializeEasyChatWordArray(gSaveBlock1Ptr->tvShows[sCurTVShowSlot].fanClubSpecial.words, 1);
}

static bool8 IsPartyMonNicknamedOrNotEnglish(u8 monIdx)
{
    struct Pokemon *pokemon;
    u8 language;

    pokemon = &gPlayerParty[monIdx];
    GetMonData(pokemon, MON_DATA_NICKNAME, gStringVar1);
    language = GetMonData(pokemon, MON_DATA_LANGUAGE, &language);
    if (language == GAME_LANGUAGE && !StringCompare(gSpeciesNames[GetMonData(pokemon, MON_DATA_SPECIES, NULL)], gStringVar1))
        return FALSE;

    return TRUE;
}

bool8 IsLeadMonNicknamedOrNotEnglish(void)
{
    return IsPartyMonNicknamedOrNotEnglish(GetLeadMonIndex());
}

static void DeleteTVShowInArrayByIdx(TVShow *shows, u8 idx)
{
    u8 i;

    shows[idx].commonInit.kind = TVSHOW_OFF_AIR;
    shows[idx].commonInit.active = FALSE;
    for (i = 0; i < ARRAY_COUNT(shows[idx].commonInit.data); i++)
        shows[idx].commonInit.data[i] = 0;
}

static void CompactTVShowArray(TVShow *shows)
{
    u8 i;
    u8 j;

    // Compact normal TV shows
    for (i = 0; i < NUM_NORMAL_TVSHOW_SLOTS - 1; i++)
    {
        if (shows[i].common.kind == TVSHOW_OFF_AIR)
        {
            for (j = i + 1; j < NUM_NORMAL_TVSHOW_SLOTS; j++)
            {
                if (shows[j].common.kind != TVSHOW_OFF_AIR)
                {
                    shows[i] = shows[j];
                    DeleteTVShowInArrayByIdx(shows, j);
                    break;
                }
            }
        }
    }

    // Compact Record Mix TV shows
    for (i = NUM_NORMAL_TVSHOW_SLOTS; i < LAST_TVSHOW_IDX; i++)
    {
        if (shows[i].common.kind == TVSHOW_OFF_AIR)
        {
            for (j = i + 1; j < LAST_TVSHOW_IDX; j++)
            {
                if (shows[j].common.kind != TVSHOW_OFF_AIR)
                {
                    shows[i] = shows[j];
                    DeleteTVShowInArrayByIdx(shows, j);
                    break;
                }
            }
        }
    }
}

static u16 GetRandomDifferentSpeciesAndNameSeenByPlayer(u8 varIdx, u16 passedSpecies)
{
    u16 species;

    species = GetRandomDifferentSpeciesSeenByPlayer(passedSpecies);
    StringCopy(gTVStringVarPtrs[varIdx], gSpeciesNames[species]);
    return species;
}

static u16 GetRandomDifferentSpeciesSeenByPlayer(u16 passedSpecies)
{
    u16 species;
    u16 initSpecies;

    species = (Random() % (NUM_SPECIES)) + 1;
    initSpecies = species;
    while (GetSetPokedexFlag(SpeciesToNationalPokedexNum(species), FLAG_GET_SEEN) != TRUE || species == passedSpecies)
    {
        if (species == SPECIES_NONE)
            species = NUM_SPECIES;
        else
            species--;

        if (species == initSpecies)
        {
            species = passedSpecies;
            return species;
        }
    };
    return species;
}

static void Script_FindFirstEmptyNormalTVShowSlot(void)
{
    sCurTVShowSlot = FindFirstEmptyNormalTVShowSlot(gSaveBlock1Ptr->tvShows);
    gSpecialVar_0x8006 = sCurTVShowSlot;
    if (sCurTVShowSlot == -1)
        gSpecialVar_Result = TRUE;  // Failed to find empty slot
    else
        gSpecialVar_Result = FALSE; // Found empty slot
}

static s8 FindFirstEmptyNormalTVShowSlot(TVShow *shows)
{
    u8 i;

    for (i = 0; i < NUM_NORMAL_TVSHOW_SLOTS; i++)
    {
        if (shows[i].common.kind == TVSHOW_OFF_AIR)
            return i;
    }
    return -1;
}

static s8 FindFirstEmptyRecordMixTVShowSlot(TVShow *shows)
{
    s8 i;

    for (i = NUM_NORMAL_TVSHOW_SLOTS; i < LAST_TVSHOW_IDX; i++)
    {
        if (shows[i].common.kind == TVSHOW_OFF_AIR)
            return i;
    }
    return -1;
}

static bool8 BernoulliTrial(u16 ratio)
{
    if (Random() <= ratio)
        return FALSE;

    return TRUE;
}

// For TVSHOW_FAN_CLUB_LETTER / TVSHOW_RECENT_HAPPENINGS
// Both are assumed to have the same struct layout
static void GetRandomWordFromShow(TVShow *show)
{
    u8 i;

    i = Random() % ARRAY_COUNT(show->fanclubLetter.words);
    
    // From random point, get first non-empty word
    while (TRUE)
    {
        if (i == ARRAY_COUNT(show->fanclubLetter.words))
            i = 0;

        if (show->fanclubLetter.words[i] != EC_EMPTY_WORD)
            break;
        i++;
    }
    CopyEasyChatWord(gStringVar3, show->fanclubLetter.words[i]);
}

static u8 GetRandomNameRaterStateFromName(TVShow *show)
{
    u8 i;
    u16 nameSum;

    nameSum = 0;
    for (i = 0; i < POKEMON_NAME_LENGTH + 1; i++)
    {
        if (show->nameRaterShow.pokemonName[i] == EOS)
            break;

        nameSum += show->nameRaterShow.pokemonName[i];
    }
    return nameSum & 7;
}

static void GetNicknameSubstring(u8 varIdx, u8 whichPosition, u8 charParam, u16 whichString, u16 species, TVShow *show)
{
    u8 buff[16];
    u8 i;
    u16 strlen;

    for (i = 0; i < 3; i++)
        buff[i] = EOS;

    if (whichString == 0)
    {
        strlen = StringLength(show->nameRaterShow.trainerName);
        if (charParam == 0)
        {
            buff[0] = show->nameRaterShow.trainerName[whichPosition];
        }
        else if (charParam == 1)
        {
            buff[0] = show->nameRaterShow.trainerName[strlen - whichPosition];
        }
        else if (charParam == 2)
        {
            buff[0] = show->nameRaterShow.trainerName[whichPosition];
            buff[1] = show->nameRaterShow.trainerName[whichPosition + 1];
        }
        else
        {
            buff[0] = show->nameRaterShow.trainerName[strlen - (whichPosition + 2)];
            buff[1] = show->nameRaterShow.trainerName[strlen - (whichPosition + 1)];
        }
        ConvertInternationalString(buff, show->nameRaterShow.language);
    }
    else if (whichString == 1)
    {
        strlen = StringLength(show->nameRaterShow.pokemonName);
        if (charParam == 0)
        {
            buff[0] = show->nameRaterShow.pokemonName[whichPosition];
        }
        else if (charParam == 1)
        {
            buff[0] = show->nameRaterShow.pokemonName[strlen - whichPosition];
        }
        else if (charParam == 2)
        {
            buff[0] = show->nameRaterShow.pokemonName[whichPosition];
            buff[1] = show->nameRaterShow.pokemonName[whichPosition + 1];
        }
        else
        {
            buff[0] = show->nameRaterShow.pokemonName[strlen - (whichPosition + 2)];
            buff[1] = show->nameRaterShow.pokemonName[strlen - (whichPosition + 1)];
        }
        ConvertInternationalString(buff, show->nameRaterShow.pokemonNameLanguage);
    }
    else
    {
        strlen = StringLength(gSpeciesNames[species]);
        if (charParam == 0)
        {
            buff[0] = gSpeciesNames[species][whichPosition];
        }
        else if (charParam == 1)
        {
            buff[0] = gSpeciesNames[species][strlen - whichPosition];
        }
        else if (charParam == 2)
        {
            buff[0] = gSpeciesNames[species][whichPosition];
            buff[1] = gSpeciesNames[species][whichPosition + 1];
        }
        else
        {
            buff[0] = gSpeciesNames[species][strlen - (whichPosition + 2)];
            buff[1] = gSpeciesNames[species][strlen - (whichPosition + 1)];
        }
    }
    StringCopy(gTVStringVarPtrs[varIdx], buff);
}

// Unused script special
bool8 IsTVShowAlreadyInQueue(void)
{
    u8 i;

    for (i = 0; i < NUM_NORMAL_TVSHOW_SLOTS; i++)
    {
        if (gSaveBlock1Ptr->tvShows[i].common.kind == gSpecialVar_0x8004)
            return TRUE;
    }
    return FALSE;
}

bool8 TryPutNameRaterShowOnTheAir(void)
{
    GetMonData(&gPlayerParty[gSpecialVar_0x8004], MON_DATA_NICKNAME, gStringVar1);

    // Nickname wasnt changed
    if (!StringCompare(gStringVar3, gStringVar1))
        return FALSE;

    PutNameRaterShowOnTheAir();
    return TRUE;
}

void ChangePokemonNickname(void)
{
    void ChangePokemonNickname_CB(void);

    GetMonData(&gPlayerParty[gSpecialVar_0x8004], MON_DATA_NICKNAME, gStringVar3);
    GetMonData(&gPlayerParty[gSpecialVar_0x8004], MON_DATA_NICKNAME, gStringVar2);
    DoNamingScreen(NAMING_SCREEN_NICKNAME, gStringVar2, GetMonData(&gPlayerParty[gSpecialVar_0x8004], MON_DATA_SPECIES, NULL), GetMonGender(&gPlayerParty[gSpecialVar_0x8004]), GetMonData(&gPlayerParty[gSpecialVar_0x8004], MON_DATA_PERSONALITY, NULL), ChangePokemonNickname_CB, 0);
}

void ChangePokemonNickname_CB(void)
{
    SetMonData(&gPlayerParty[gSpecialVar_0x8004], MON_DATA_NICKNAME, gStringVar2);
    CB2_ReturnToFieldContinueScriptPlayMapMusic();
}

void ChangeBoxPokemonNickname(void)
{
    struct BoxPokemon *boxMon;

    boxMon = GetBoxedMonPtr(gSpecialVar_MonBoxId, gSpecialVar_MonBoxPos);
    GetBoxMonData(boxMon, MON_DATA_NICKNAME, gStringVar3);
    GetBoxMonData(boxMon, MON_DATA_NICKNAME, gStringVar2);
    DoNamingScreen(NAMING_SCREEN_NICKNAME, gStringVar2, GetBoxMonData(boxMon, MON_DATA_SPECIES, NULL), GetBoxMonGender(boxMon), GetBoxMonData(boxMon, MON_DATA_PERSONALITY, NULL), ChangeBoxPokemonNickname_CB, 0);
}

static void ChangeBoxPokemonNickname_CB(void)
{
    SetBoxMonNickAt(gSpecialVar_MonBoxId, gSpecialVar_MonBoxPos, gStringVar2);
    CB2_ReturnToFieldContinueScriptPlayMapMusic();
}

void BufferMonNickname(void)
{
    GetMonData(&gPlayerParty[gSpecialVar_0x8004], MON_DATA_NICKNAME, gStringVar1);
    StringGetEnd10(gStringVar1);
}

void IsMonOTIDNotPlayers(void)
{
    if (GetPlayerIDAsU32() == GetMonData(&gPlayerParty[gSpecialVar_0x8004], MON_DATA_OT_ID, NULL))
        gSpecialVar_Result = FALSE;
    else
        gSpecialVar_Result = TRUE;
}

static u8 GetTVGroupByShowId(u8 kind)
{
    if (kind == TVSHOW_OFF_AIR)
        return TVGROUP_NONE;

    if (kind >= TVGROUP_NORMAL_START && kind <= TVGROUP_NORMAL_END)
        return TVGROUP_NORMAL;

    if (kind >= TVGROUP_RECORD_MIX_START && kind <= TVGROUP_RECORD_MIX_END)
        return TVGROUP_RECORD_MIX;

    if (kind >= TVGROUP_OUTBREAK_START && kind <= TVGROUP_OUTBREAK_END)
        return TVGROUP_OUTBREAK;

    return TVGROUP_NONE;
}

u32 GetPlayerIDAsU32(void)
{
    return (gSaveBlock2Ptr->playerTrainerId[3] << 24) | (gSaveBlock2Ptr->playerTrainerId[2] << 16) | (gSaveBlock2Ptr->playerTrainerId[1] << 8) | gSaveBlock2Ptr->playerTrainerId[0];
}

u8 CheckForPlayersHouseNews(void)
{
    // Check if not in Littleroot house map group
    if (gSaveBlock1Ptr->location.mapGroup != MAP_GROUP(LITTLEROOT_TOWN_BRENDANS_HOUSE_1F))
        return PLAYERS_HOUSE_TV_NONE;

    // Check if not in player's house (dependent on gender)
    if (gSaveBlock2Ptr->playerGender == MALE)
    {
        if (gSaveBlock1Ptr->location.mapNum != MAP_NUM(LITTLEROOT_TOWN_BRENDANS_HOUSE_1F))
            return PLAYERS_HOUSE_TV_NONE;
    }
    else
    {
        if (gSaveBlock1Ptr->location.mapNum != MAP_NUM(LITTLEROOT_TOWN_MAYS_HOUSE_1F))
            return PLAYERS_HOUSE_TV_NONE;
    }

    if (FlagGet(FLAG_SYS_TV_LATIAS_LATIOS) == TRUE)
        return PLAYERS_HOUSE_TV_LATI;

    if (FlagGet(FLAG_SYS_TV_HOME) == TRUE)
        return PLAYERS_HOUSE_TV_MOVIE;

    return PLAYERS_HOUSE_TV_LATI;
}

void GetMomOrDadStringForTVMessage(void)
{
    if (gSaveBlock1Ptr->location.mapGroup == MAP_GROUP(LITTLEROOT_TOWN_BRENDANS_HOUSE_1F))
    {
        if (gSaveBlock2Ptr->playerGender == MALE)
        {
            if (gSaveBlock1Ptr->location.mapNum == MAP_NUM(LITTLEROOT_TOWN_BRENDANS_HOUSE_1F))
            {
                StringCopy(gStringVar1, gText_Mom);
                VarSet(VAR_TEMP_3, 1);
            }
        }
        else
        {
            if (gSaveBlock1Ptr->location.mapNum == MAP_NUM(LITTLEROOT_TOWN_MAYS_HOUSE_1F))
            {
                StringCopy(gStringVar1, gText_Mom);
                VarSet(VAR_TEMP_3, 1);
            }
        }
    }
    if (VarGet(VAR_TEMP_3) == 1)
    {
        StringCopy(gStringVar1, gText_Mom);
    }
    else if (VarGet(VAR_TEMP_3) == 2)
    {
        StringCopy(gStringVar1, gText_Dad);
    }
    else if (VarGet(VAR_TEMP_3) > 2)
    {
        if (VarGet(VAR_TEMP_3) % 2)
            StringCopy(gStringVar1, gText_Dad);
        else
            StringCopy(gStringVar1, gText_Mom);
    }
    else
    {
        if (Random() % 2 != 0)
        {
            StringCopy(gStringVar1, gText_Mom);
            VarSet(VAR_TEMP_3, 1);
        }
        else
        {
            StringCopy(gStringVar1, gText_Dad);
            VarSet(VAR_TEMP_3, 2);
        }
    }
}

void HideBattleTowerReporter(void)
{
    VarSet(VAR_BRAVO_TRAINER_BATTLE_TOWER_ON, 0);
    RemoveObjectEventByLocalIdAndMap(LOCALID_BATTLE_TOWER_LOBBY_REPORTER, gSaveBlock1Ptr->location.mapNum, gSaveBlock1Ptr->location.mapGroup);
    FlagSet(FLAG_HIDE_BATTLE_TOWER_REPORTER);
}

void ReceiveTvShowsData(void *src, u32 size, u8 playersLinkId)
{
    u8 i;
    u16 version;
    TVShow (*rmBuffer2)[MAX_LINK_PLAYERS][TV_SHOWS_COUNT];
    TVShow (*rmBuffer)[MAX_LINK_PLAYERS][TV_SHOWS_COUNT];

    rmBuffer2 = malloc(MAX_LINK_PLAYERS * TV_SHOWS_COUNT * sizeof(TVShow));
    if (rmBuffer2 != NULL)
    {
        for (i = 0; i < MAX_LINK_PLAYERS; i++)
            memcpy((*rmBuffer2)[i], src + i * size, sizeof((*rmBuffer2)[i]));

        rmBuffer = rmBuffer2;
        for (i = 0; i < GetLinkPlayerCount(); i++)
        {
            version = (u8)gLinkPlayers[i].version;
            if (version == VERSION_RUBY || version == VERSION_SAPPHIRE)
                TranslateRubyShows((*rmBuffer)[i]);
            else if (version == VERSION_EMERALD && gLinkPlayers[i].language == LANGUAGE_JAPANESE)
                TranslateJapaneseEmeraldShows((*rmBuffer)[i]);
        }
        
        // Position player's TV shows in argument list depending on link id
        switch (playersLinkId)
        {
        case 0:
            SetMixedTVShows(gSaveBlock1Ptr->tvShows, (*rmBuffer)[1], (*rmBuffer)[2], (*rmBuffer)[3]);
            break;
        case 1:
            SetMixedTVShows((*rmBuffer)[0], gSaveBlock1Ptr->tvShows, (*rmBuffer)[2], (*rmBuffer)[3]);
            break;
        case 2:
            SetMixedTVShows((*rmBuffer)[0], (*rmBuffer)[1], gSaveBlock1Ptr->tvShows, (*rmBuffer)[3]);
            break;
        case 3:
            SetMixedTVShows((*rmBuffer)[0], (*rmBuffer)[1], (*rmBuffer)[2], gSaveBlock1Ptr->tvShows);
            break;
        }

        CompactTVShowArray(gSaveBlock1Ptr->tvShows);
        DeleteExcessMixedShows();
        CompactTVShowArray(gSaveBlock1Ptr->tvShows);
        DeactivateShowsWithUnseenSpecies();
        DeactivateGameCompleteShowsIfNotUnlocked();
        free(rmBuffer2);
    }
}

static void SetMixedTVShows(TVShow player1[TV_SHOWS_COUNT], TVShow player2[TV_SHOWS_COUNT], TVShow player3[TV_SHOWS_COUNT], TVShow player4[TV_SHOWS_COUNT])
{
    u8 i;
    u8 j;
    TVShow **tvShows[MAX_LINK_PLAYERS];

    tvShows[0] = &player1;
    tvShows[1] = &player2;
    tvShows[2] = &player3;
    tvShows[3] = &player4;
    sTVShowMixingNumPlayers = GetLinkPlayerCount();
    while (1)
    {
        for (i = 0; i < sTVShowMixingNumPlayers; i++)
        {
            if (i == 0)
                sRecordMixingPartnersWithoutShowsToShare = 0;

            sTVShowMixingCurSlot = FindInactiveShowInArray(tvShows[i][0]);
            if (sTVShowMixingCurSlot == -1)
            {
                sRecordMixingPartnersWithoutShowsToShare++;
                if (sRecordMixingPartnersWithoutShowsToShare == sTVShowMixingNumPlayers)
                    return;
            }
            else
            {
                for (j = 0; j < sTVShowMixingNumPlayers - 1; j++)
                {
                    sCurTVShowSlot = FindFirstEmptyRecordMixTVShowSlot(tvShows[(i + j + 1) % sTVShowMixingNumPlayers][0]);
                    if (sCurTVShowSlot != -1
                        && TryMixTVShow(&tvShows[(i + j + 1) % sTVShowMixingNumPlayers][0], &tvShows[i][0], (i + j + 1) % sTVShowMixingNumPlayers) == 1)
                        break;
                }
                if (j == sTVShowMixingNumPlayers - 1)
                    DeleteTVShowInArrayByIdx(tvShows[i][0], sTVShowMixingCurSlot);
            }
        }
    }
}

static bool8 TryMixTVShow(TVShow *dest[TV_SHOWS_COUNT], TVShow *src[TV_SHOWS_COUNT], u8 idx)
{
    bool8 success;
    u8 type;
    TVShow *tv1 = *dest;
    TVShow *tv2 = *src;

    success = FALSE;
    type = GetTVGroupByShowId(tv2[sTVShowMixingCurSlot].common.kind);
    switch (type)
    {
    case TVGROUP_NORMAL:
        success = TryMixNormalTVShow(&tv1[sCurTVShowSlot], &tv2[sTVShowMixingCurSlot], idx);
        break;
    case TVGROUP_RECORD_MIX:
        success = TryMixRecordMixTVShow(&tv1[sCurTVShowSlot], &tv2[sTVShowMixingCurSlot], idx);
        break;
    case TVGROUP_OUTBREAK:
        success = TryMixOutbreakTVShow(&tv1[sCurTVShowSlot], &tv2[sTVShowMixingCurSlot], idx);
        break;
    }
    
    // Show was mixed, delete from array
    if (success)
    {
        DeleteTVShowInArrayByIdx(tv2, sTVShowMixingCurSlot);
        return TRUE;
    }
    return FALSE;
}

static bool8 TryMixNormalTVShow(TVShow *dest, TVShow *src, u8 idx)
{
    u32 linkTrainerId = GetLinkPlayerTrainerId(idx);

    if ((linkTrainerId & 0xFF) == src->common.trainerIdLo 
    && ((linkTrainerId >> 8) & 0xFF) == src->common.trainerIdHi)
        return FALSE;

    src->common.trainerIdLo = src->common.srcTrainerIdLo;
    src->common.trainerIdHi = src->common.srcTrainerIdHi;
    src->common.srcTrainerIdLo = linkTrainerId & 0xFF;
    src->common.srcTrainerIdHi = linkTrainerId >> 8;
    *dest = *src;
    dest->common.active = TRUE;
    return TRUE;
}

static bool8 TryMixRecordMixTVShow(TVShow *dest, TVShow *src, u8 idx)
{
    u32 linkTrainerId = GetLinkPlayerTrainerId(idx);

    if ((linkTrainerId & 0xFF) == src->common.srcTrainerIdLo 
    && ((linkTrainerId >> 8) & 0xFF) == src->common.srcTrainerIdHi)
        return FALSE;

    if ((linkTrainerId & 0xFF) == src->common.trainerIdLo 
    && ((linkTrainerId >> 8) & 0xFF) == src->common.trainerIdHi)
        return FALSE;

    src->common.srcTrainerIdLo = src->common.srcTrainerId2Lo;
    src->common.srcTrainerIdHi = src->common.srcTrainerId2Hi;
    src->common.srcTrainerId2Lo = linkTrainerId & 0xFF;
    src->common.srcTrainerId2Hi = linkTrainerId >> 8;
    *dest = *src;
    dest->common.active = TRUE;
    return TRUE;
}

static bool8 TryMixOutbreakTVShow(TVShow *dest, TVShow *src, u8 idx)
{
    u32 linkTrainerId = GetLinkPlayerTrainerId(idx);

    if ((linkTrainerId & 0xFF) == src->common.trainerIdLo 
    && ((linkTrainerId >> 8) & 0xFF) == src->common.trainerIdHi)
        return FALSE;

    src->common.trainerIdLo = src->common.srcTrainerIdLo;
    src->common.trainerIdHi = src->common.srcTrainerIdHi;
    src->common.srcTrainerIdLo = linkTrainerId & 0xFF;
    src->common.srcTrainerIdHi = linkTrainerId >> 8;
    *dest = *src;
    dest->common.active = TRUE;
    dest->massOutbreak.daysLeft = 1;
    return TRUE;
}

static s8 FindInactiveShowInArray(TVShow *tvShows)
{
    u8 i;

    for (i = 0; i < LAST_TVSHOW_IDX; i++)
    {
        // Second check is to make sure its a valid show (not too high, not TVSHOW_OFF_AIR)
        if (tvShows[i].common.active == FALSE && (u8)(tvShows[i].common.kind - 1) < TVGROUP_OUTBREAK_END)
            return i;
    }
    return -1;
}

static void DeactivateShowsWithUnseenSpecies(void)
{
    u16 i;
    u16 species;

    for (i = 0; i < LAST_TVSHOW_IDX; i++)
    {
        switch (gSaveBlock1Ptr->tvShows[i].common.kind)
        {
        case TVSHOW_BATTLE_UPDATE:
            species = (&gSaveBlock1Ptr->tvShows[i])->battleUpdate.speciesPlayer;
            DeactivateShowIfNotSeenSpecies(species, i);
            species = (&gSaveBlock1Ptr->tvShows[i])->battleUpdate.speciesOpponent;
            DeactivateShowIfNotSeenSpecies(species, i);
            break;
        case TVSHOW_FAN_CLUB_LETTER:
            species = (&gSaveBlock1Ptr->tvShows[i])->fanclubLetter.species;
            DeactivateShowIfNotSeenSpecies(species, i);
            break;
        case TVSHOW_PKMN_FAN_CLUB_OPINIONS:
            species = (&gSaveBlock1Ptr->tvShows[i])->fanclubOpinions.species;
            DeactivateShowIfNotSeenSpecies(species, i);
            break;
        case TVSHOW_DUMMY:
            species = (&gSaveBlock1Ptr->tvShows[i])->dummy.species;
            DeactivateShowIfNotSeenSpecies(species, i);
            break;
        case TVSHOW_NAME_RATER_SHOW:
            species = (&gSaveBlock1Ptr->tvShows[i])->nameRaterShow.species;
            DeactivateShowIfNotSeenSpecies(species, i);
            species = (&gSaveBlock1Ptr->tvShows[i])->nameRaterShow.randomSpecies;
            DeactivateShowIfNotSeenSpecies(species, i);
            break;
        case TVSHOW_BRAVO_TRAINER_BATTLE_TOWER_PROFILE:
            species = (&gSaveBlock1Ptr->tvShows[i])->bravoTrainerTower.species;
            DeactivateShowIfNotSeenSpecies(species, i);
            species = (&gSaveBlock1Ptr->tvShows[i])->bravoTrainerTower.defeatedSpecies;
            DeactivateShowIfNotSeenSpecies(species, i);
            break;
        case TVSHOW_POKEMON_TODAY_CAUGHT:
            species = (&gSaveBlock1Ptr->tvShows[i])->pokemonToday.species;
            DeactivateShowIfNotSeenSpecies(species, i);
            break;
        case TVSHOW_POKEMON_TODAY_FAILED:
            species = (&gSaveBlock1Ptr->tvShows[i])->pokemonTodayFailed.species;
            DeactivateShowIfNotSeenSpecies(species, i);
            species = (&gSaveBlock1Ptr->tvShows[i])->pokemonTodayFailed.species2;
            DeactivateShowIfNotSeenSpecies(species, i);
            break;
        case TVSHOW_FISHING_ADVICE:
            species = (&gSaveBlock1Ptr->tvShows[i])->pokemonAngler.species;
            DeactivateShowIfNotSeenSpecies(species, i);
            break;
        case TVSHOW_WORLD_OF_MASTERS:
            species = (&gSaveBlock1Ptr->tvShows[i])->worldOfMasters.species;
            DeactivateShowIfNotSeenSpecies(species, i);
            species = (&gSaveBlock1Ptr->tvShows[i])->worldOfMasters.caughtPoke;
            DeactivateShowIfNotSeenSpecies(species, i);
            break;
        case TVSHOW_BREAKING_NEWS:
            species = (&gSaveBlock1Ptr->tvShows[i])->breakingNews.lastOpponentSpecies;
            DeactivateShowIfNotSeenSpecies(species, i);
            species = (&gSaveBlock1Ptr->tvShows[i])->breakingNews.poke1Species;
            DeactivateShowIfNotSeenSpecies(species, i);
            break;
        case TVSHOW_BATTLE_SEMINAR:
            species = (&gSaveBlock1Ptr->tvShows[i])->battleSeminar.species;
            DeactivateShowIfNotSeenSpecies(species, i);
            species = (&gSaveBlock1Ptr->tvShows[i])->battleSeminar.foeSpecies;
            DeactivateShowIfNotSeenSpecies(species, i);
            break;
        case TVSHOW_FRONTIER:
            species = (&gSaveBlock1Ptr->tvShows[i])->frontier.species1;
            DeactivateShowIfNotSeenSpecies(species, i);
            species = (&gSaveBlock1Ptr->tvShows[i])->frontier.species2;
            DeactivateShowIfNotSeenSpecies(species, i);
            // Species var re-used here
            species = (&gSaveBlock1Ptr->tvShows[i])->frontier.facilityAndMode;
            switch (species)
            {
            case FRONTIER_SHOW_TOWER_MULTIS:
            case FRONTIER_SHOW_TOWER_LINK_MULTIS:
                break;
            case FRONTIER_SHOW_TOWER_SINGLES:
            case FRONTIER_SHOW_DOME_SINGLES:
            case FRONTIER_SHOW_DOME_DOUBLES:
            case FRONTIER_SHOW_FACTORY_SINGLES:
            case FRONTIER_SHOW_FACTORY_DOUBLES:
            case FRONTIER_SHOW_PIKE:
            case FRONTIER_SHOW_ARENA:
            case FRONTIER_SHOW_PALACE_SINGLES:
            case FRONTIER_SHOW_PALACE_DOUBLES:
            case FRONTIER_SHOW_PYRAMID:
                species = (&gSaveBlock1Ptr->tvShows[i])->frontier.species3;
                DeactivateShowIfNotSeenSpecies(species, i);
                break;
            case FRONTIER_SHOW_TOWER_DOUBLES:
                species = (&gSaveBlock1Ptr->tvShows[i])->frontier.species3;
                DeactivateShowIfNotSeenSpecies(species, i);
                species = (&gSaveBlock1Ptr->tvShows[i])->frontier.species4;
                DeactivateShowIfNotSeenSpecies(species, i);
                break;
            }
        // Shows with no species
        case TVSHOW_OFF_AIR:
        case TVSHOW_RECENT_HAPPENINGS:
        case TVSHOW_TODAYS_RIVAL_TRAINER:
        case TVSHOW_TREND_WATCHER:
        case TVSHOW_TREASURE_INVESTIGATORS:
        case TVSHOW_FIND_THAT_GAMER:
        case TVSHOW_TRAINER_FAN_CLUB:
        case TVSHOW_SMART_SHOPPER:
        case TVSHOW_FAN_CLUB_SPECIAL:
        case TVSHOW_LOTTO_WINNER:
        case TVSHOW_NUMBER_ONE:
        case TVSHOW_SAFARI_FAN_CLUB:
        case TVSHOW_MASS_OUTBREAK:
            break;
        default:
            DeactivateShow(i);
            break;
        }
    }
}

static void DeactivateShow(u8 showIdx)
{
    gSaveBlock1Ptr->tvShows[showIdx].common.active = FALSE;
}

static void DeactivateShowIfNotSeenSpecies(u16 species, u8 showIdx)
{
    if (!GetSetPokedexFlag(SpeciesToNationalPokedexNum(species), FLAG_GET_SEEN))
        gSaveBlock1Ptr->tvShows[showIdx].common.active = FALSE;
}

static void DeactivateGameCompleteShowsIfNotUnlocked(void)
{
    u16 i;

    if (FlagGet(FLAG_SYS_GAME_CLEAR) != TRUE)
    {
        for (i = 0; i < LAST_TVSHOW_IDX; i++)
        {
            if (gSaveBlock1Ptr->tvShows[i].common.kind == TVSHOW_BRAVO_TRAINER_BATTLE_TOWER_PROFILE)
                gSaveBlock1Ptr->tvShows[i].common.active = FALSE;
            else if (gSaveBlock1Ptr->tvShows[i].common.kind == TVSHOW_MASS_OUTBREAK)
                gSaveBlock1Ptr->tvShows[i].common.active = FALSE;
        }
    }
}

void DeactivateAllNormalTVShows(void)
{
    u8 i;

    for (i = 0; i < NUM_NORMAL_TVSHOW_SLOTS; i++)
    {
        if (GetTVGroupByShowId(gSaveBlock1Ptr->tvShows[i].common.kind) == TVGROUP_NORMAL)
            gSaveBlock1Ptr->tvShows[i].common.active = FALSE;
    }
}

// Ensures a minimum of 5 empty mixed show slots
static void DeleteExcessMixedShows(void)
{
    s8 i;
    s8 numEmptyMixSlots = 0;
    for (i = NUM_NORMAL_TVSHOW_SLOTS; i < LAST_TVSHOW_IDX; i++)
    {
        if (gSaveBlock1Ptr->tvShows[i].common.kind == TVSHOW_OFF_AIR)
            numEmptyMixSlots++;
    }
    for (i = 0; i < NUM_NORMAL_TVSHOW_SLOTS - numEmptyMixSlots; i++)
        DeleteTVShowInArrayByIdx(gSaveBlock1Ptr->tvShows, i + NUM_NORMAL_TVSHOW_SLOTS);
}

void ReceivePokeNewsData(void *src, u32 size, u8 playersLinkId)
{
    u8 i;
    PokeNews (*rmBuffer2)[MAX_LINK_PLAYERS][POKE_NEWS_COUNT];
    PokeNews (*rmBuffer)[MAX_LINK_PLAYERS][POKE_NEWS_COUNT];

    rmBuffer2 = malloc(MAX_LINK_PLAYERS * POKE_NEWS_COUNT * sizeof(PokeNews));
    if (rmBuffer2 != NULL)
    {
        for (i = 0; i < MAX_LINK_PLAYERS; i++)
            memcpy((*rmBuffer2)[i], src + i * size, sizeof((*rmBuffer2)[i]));

        rmBuffer = rmBuffer2;
        
        // Position player's PokeNews in argument list depending on link id
        switch (playersLinkId)
        {
        case 0:
            SetMixedPokeNews(gSaveBlock1Ptr->pokeNews, (*rmBuffer)[1], (*rmBuffer)[2], (*rmBuffer)[3]);
            break;
        case 1:
            SetMixedPokeNews((*rmBuffer)[0], gSaveBlock1Ptr->pokeNews, (*rmBuffer)[2], (*rmBuffer)[3]);
            break;
        case 2:
            SetMixedPokeNews((*rmBuffer)[0], (*rmBuffer)[1], gSaveBlock1Ptr->pokeNews, (*rmBuffer)[3]);
            break;
        case 3:
            SetMixedPokeNews((*rmBuffer)[0], (*rmBuffer)[1], (*rmBuffer)[2], gSaveBlock1Ptr->pokeNews);
            break;
        }
        ClearInvalidPokeNews();
        ClearPokeNewsIfGameNotComplete();
        free(rmBuffer2);
    }
}

static void SetMixedPokeNews(PokeNews player1[POKE_NEWS_COUNT], PokeNews player2[POKE_NEWS_COUNT], PokeNews player3[POKE_NEWS_COUNT], PokeNews player4[POKE_NEWS_COUNT])
{
    u8 i, j, k;
    PokeNews **pokeNews[MAX_LINK_PLAYERS];

    pokeNews[0] = &player1;
    pokeNews[1] = &player2;
    pokeNews[2] = &player3;
    pokeNews[3] = &player4;
    sTVShowNewsMixingNumPlayers = GetLinkPlayerCount();
    for (i = 0; i < POKE_NEWS_COUNT; i++)
    {
        for (j = 0; j < sTVShowNewsMixingNumPlayers; j++)
        {
            sTVShowMixingCurSlot = GetPokeNewsSlotIfActive(*pokeNews[j], i);
            if (sTVShowMixingCurSlot != -1)
            {
                for (k = 0; k < sTVShowNewsMixingNumPlayers - 1; k++)
                {
                    sCurTVShowSlot = GetFirstEmptyPokeNewsSlot(*pokeNews[(j + k + 1) % sTVShowNewsMixingNumPlayers]);
                    if (sCurTVShowSlot != -1)
                        InitTryMixPokeNewsShow(pokeNews[(j + k + 1) % sTVShowNewsMixingNumPlayers], pokeNews[j]);
                }
            }
        }
    }
}

static void InitTryMixPokeNewsShow(PokeNews *dest[POKE_NEWS_COUNT], PokeNews *src[POKE_NEWS_COUNT])
{
    PokeNews *ptr1 = *dest;
    PokeNews *ptr2 = *src;
    ptr2 += sTVShowMixingCurSlot;
    TryMixPokeNewsShow(ptr1, ptr2, sCurTVShowSlot);
}

static bool8 TryMixPokeNewsShow(PokeNews *dest, PokeNews *src, s8 slot)
{
    u8 i;

    if (src->kind == POKENEWS_NONE)
        return FALSE;

    for (i = 0; i < POKE_NEWS_COUNT; i++)
    {
        if (dest[i].kind == src->kind)
            return FALSE;
    }
    dest[slot].kind = src->kind;
    dest[slot].state = 1;
    dest[slot].days = src->days;
    return TRUE;
}

static s8 GetPokeNewsSlotIfActive(PokeNews *pokeNews, u8 idx)
{
    if (pokeNews[idx].kind == POKENEWS_NONE)
        return -1;

    return idx;
}

static void ClearInvalidPokeNews(void)
{
    u8 i;

    for (i = 0; i < POKE_NEWS_COUNT; i++)
    {
        if (gSaveBlock1Ptr->pokeNews[i].kind > POKENEWS_BLENDMASTER)
            ClearPokeNewsBySlot(i);
    }
    CompactPokeNews();
}

static void ClearPokeNewsIfGameNotComplete(void)
{
    u8 i;

    if (FlagGet(FLAG_SYS_GAME_CLEAR) != TRUE)
    {
        for (i = 0; i < POKE_NEWS_COUNT; i++)
            gSaveBlock1Ptr->pokeNews[i].state = 0;
    }
}

void SanitizeTVShowsForRuby(TVShow *shows)
{
    TVShow *curShow;

    SanitizeTVShowLocationsForRuby(shows);
    for (curShow = shows; curShow < shows + LAST_TVSHOW_IDX; curShow++)
    {
        if (curShow->bravoTrainerTower.kind == TVSHOW_BRAVO_TRAINER_BATTLE_TOWER_PROFILE)
        {
            if ((curShow->bravoTrainerTower.language == LANGUAGE_JAPANESE && curShow->bravoTrainerTower.pokemonNameLanguage != LANGUAGE_JAPANESE) 
             || (curShow->bravoTrainerTower.language != LANGUAGE_JAPANESE && curShow->bravoTrainerTower.pokemonNameLanguage == LANGUAGE_JAPANESE))
                memset(curShow, 0, sizeof(TVShow));
        }
    }
}

static void TranslateRubyShows(TVShow *shows)
{
    TVShow *curShow;

    for (curShow = shows; curShow < shows + LAST_TVSHOW_IDX; curShow++)
    {
        if (curShow->bravoTrainerTower.kind == TVSHOW_BRAVO_TRAINER_BATTLE_TOWER_PROFILE)
        {
            if (IsStringJapanese(curShow->bravoTrainerTower.pokemonName))
                curShow->bravoTrainerTower.pokemonNameLanguage = LANGUAGE_JAPANESE;
            else
                curShow->bravoTrainerTower.pokemonNameLanguage = GAME_LANGUAGE;
        }
    }
}

static u8 GetStringLanguage(u8 *str)
{
    return IsStringJapanese(str) ? LANGUAGE_JAPANESE : GAME_LANGUAGE;
}

static void TranslateJapaneseEmeraldShows(TVShow *shows)
{
    TVShow *curShow;

    for (curShow = shows; curShow < shows + LAST_TVSHOW_IDX; curShow++)
    {
        switch(curShow->common.kind)
        {
        case TVSHOW_FAN_CLUB_LETTER:
            curShow->fanclubLetter.language = GetStringLanguage(curShow->fanclubLetter.playerName);
            break;
        case TVSHOW_RECENT_HAPPENINGS:
            curShow->recentHappenings.language = GetStringLanguage(curShow->recentHappenings.playerName);
            break;
        case TVSHOW_PKMN_FAN_CLUB_OPINIONS:
            curShow->fanclubOpinions.language = GetStringLanguage(curShow->fanclubOpinions.playerName);
            curShow->fanclubOpinions.pokemonNameLanguage = GetStringLanguage(curShow->fanclubOpinions.nickname);
            break;
        case TVSHOW_DUMMY:
            curShow->dummy.language = GetStringLanguage(curShow->dummy.name);
            break;
        case TVSHOW_NAME_RATER_SHOW:
            curShow->nameRaterShow.language = GetStringLanguage(curShow->nameRaterShow.trainerName);
            curShow->nameRaterShow.pokemonNameLanguage = GetStringLanguage(curShow->nameRaterShow.pokemonName);
            break;
        case TVSHOW_BRAVO_TRAINER_BATTLE_TOWER_PROFILE:
            curShow->bravoTrainerTower.language = GetStringLanguage(curShow->bravoTrainerTower.trainerName);
            curShow->bravoTrainerTower.pokemonNameLanguage = GetStringLanguage(curShow->bravoTrainerTower.pokemonName);
            break;
        case TVSHOW_BATTLE_UPDATE:
            curShow->battleUpdate.language = GetStringLanguage(curShow->battleUpdate.playerName);
            curShow->battleUpdate.linkOpponentLanguage = GetStringLanguage(curShow->battleUpdate.linkOpponentName);
            break;
        case TVSHOW_FAN_CLUB_SPECIAL:
            curShow->fanClubSpecial.language = GetStringLanguage(curShow->fanClubSpecial.playerName);
            curShow->fanClubSpecial.idolNameLanguage = GetStringLanguage(curShow->fanClubSpecial.idolName);
            break;
        case TVSHOW_POKEMON_TODAY_CAUGHT:
            curShow->pokemonToday.language = GetStringLanguage(curShow->pokemonToday.playerName);
            curShow->pokemonToday.language2 = GetStringLanguage(curShow->pokemonToday.nickname);
            break;
        case TVSHOW_SMART_SHOPPER:
            curShow->smartshopperShow.language = GetStringLanguage(curShow->smartshopperShow.playerName);
            break;
        case TVSHOW_POKEMON_TODAY_FAILED:
            curShow->pokemonTodayFailed.language = GetStringLanguage(curShow->pokemonTodayFailed.playerName);
            break;
        case TVSHOW_FISHING_ADVICE:
            curShow->pokemonAngler.language = GetStringLanguage(curShow->pokemonAngler.playerName);
            break;
        case TVSHOW_WORLD_OF_MASTERS:
            curShow->worldOfMasters.language = GetStringLanguage(curShow->worldOfMasters.playerName);
            break;
        case TVSHOW_TREND_WATCHER:
            curShow->trendWatcher.language = GetStringLanguage(curShow->trendWatcher.playerName);
            break;
        case TVSHOW_BREAKING_NEWS:
            curShow->breakingNews.language = GetStringLanguage(curShow->breakingNews.playerName);
            break;
        case TVSHOW_BATTLE_SEMINAR:
            curShow->battleSeminar.language = GetStringLanguage(curShow->battleSeminar.playerName);
            break;
        case TVSHOW_FIND_THAT_GAMER:
        case TVSHOW_TRAINER_FAN_CLUB:
            curShow->trainerFanClub.language = GetStringLanguage(curShow->trainerFanClub.playerName);
            break;
        case TVSHOW_TODAYS_RIVAL_TRAINER:
        case TVSHOW_FRONTIER:
            curShow->rivalTrainer.language = GetStringLanguage(curShow->rivalTrainer.playerName);
            break;
        case TVSHOW_TREASURE_INVESTIGATORS:
        case TVSHOW_LOTTO_WINNER:
        case TVSHOW_NUMBER_ONE:
            curShow->treasureInvestigators.language = GetStringLanguage(curShow->treasureInvestigators.playerName);
            break;
        case TVSHOW_SAFARI_FAN_CLUB:
            curShow->safariFanClub.language = GetStringLanguage(curShow->safariFanClub.playerName);
            break;
        case TVSHOW_MASS_OUTBREAK:
            break;
        }
    }
}

void SanitizeTVShowLocationsForRuby(TVShow *shows)
{
    int i;

    for (i = 0; i < LAST_TVSHOW_IDX; i++)
    {
        switch (shows[i].common.kind)
        {
        case TVSHOW_WORLD_OF_MASTERS:
            if (shows[i].worldOfMasters.location > KANTO_MAPSEC_START)
                memset(&shows[i], 0, sizeof(TVShow));
            break;
        case TVSHOW_POKEMON_TODAY_FAILED:
            if (shows[i].pokemonTodayFailed.location > KANTO_MAPSEC_START)
                memset(&shows[i], 0, sizeof(TVShow));
            break;
        }
    }
}

// gSpecialVar_0x8004 here is set from GetRandomActiveShowIdx in EventScript_TryDoTVShow
void DoTVShow(void)
{
    if (gSaveBlock1Ptr->tvShows[gSpecialVar_0x8004].common.active)
    {
        switch (gSaveBlock1Ptr->tvShows[gSpecialVar_0x8004].common.kind)
        {
        case TVSHOW_FAN_CLUB_LETTER:
            DoTVShowPokemonFanClubLetter();
            break;
        case TVSHOW_RECENT_HAPPENINGS:
            DoTVShowRecentHappenings();
            break;
        case TVSHOW_PKMN_FAN_CLUB_OPINIONS:
            DoTVShowPokemonFanClubOpinions();
            break;
        case TVSHOW_DUMMY:
            DoTVShowDummiedOut();
            break;
        case TVSHOW_MASS_OUTBREAK:
            DoTVShowPokemonNewsMassOutbreak();
            break;
        case TVSHOW_BRAVO_TRAINER_BATTLE_TOWER_PROFILE:
            DoTVShowBravoTrainerBattleTower();
            break;
        case TVSHOW_POKEMON_TODAY_CAUGHT:
            DoTVShowPokemonTodaySuccessfulCapture();
            break;
        case TVSHOW_SMART_SHOPPER:
            DoTVShowTodaysSmartShopper();
            break;
        case TVSHOW_NAME_RATER_SHOW:
            DoTVShowTheNameRaterShow();
            break;
        case TVSHOW_BATTLE_UPDATE:
            DoTVShowPokemonBattleUpdate();
            break;
        case TVSHOW_POKEMON_TODAY_FAILED:
            DoTVShowPokemonTodayFailedCapture();
            break;
        case TVSHOW_FISHING_ADVICE:
            DoTVShowPokemonAngler();
            break;
        case TVSHOW_WORLD_OF_MASTERS:
            DoTVShowTheWorldOfMasters();
            break;
        case TVSHOW_TODAYS_RIVAL_TRAINER:
            DoTVShowTodaysRivalTrainer();
            break;
        case TVSHOW_TREND_WATCHER:
            DoTVShowDewfordTrendWatcherNetwork();
            break;
        case TVSHOW_TREASURE_INVESTIGATORS:
            DoTVShowHoennTreasureInvestigators();
            break;
        case TVSHOW_FIND_THAT_GAMER:
            DoTVShowFindThatGamer();
            break;
        case TVSHOW_BREAKING_NEWS:
            DoTVShowBreakingNewsTV();
            break;
        case TVSHOW_LOTTO_WINNER:
            DoTVShowPokemonLotteryWinnerFlashReport();
            break;
        case TVSHOW_BATTLE_SEMINAR:
            DoTVShowThePokemonBattleSeminar();
            break;
        case TVSHOW_FAN_CLUB_SPECIAL:
            DoTVShowTrainerFanClubSpecial();
            break;
        case TVSHOW_TRAINER_FAN_CLUB:
            DoTVShowTrainerFanClub();
            break;
        case TVSHOW_FRONTIER:
            DoTVShowPokemonNewsBattleFrontier();
            break;
        case TVSHOW_NUMBER_ONE:
            DoTVShowWhatsNo1InHoennToday();
            break;
        case TVSHOW_SAFARI_FAN_CLUB:
            DoTVShowSafariFanClub();
            break;
        }
    }
}

static void DoTVShowBravoTrainerBattleTower(void)
{
    TVShow *show;
    u8 state;

    show = &gSaveBlock1Ptr->tvShows[gSpecialVar_0x8004];
    gSpecialVar_Result = FALSE;
    state = sTVShowState;
    switch(state)
    {
    case 0:
        TVShowConvertInternationalString(gStringVar1, show->bravoTrainerTower.trainerName, show->bravoTrainerTower.language);
        StringCopy(gStringVar2, gSpeciesNames[show->bravoTrainerTower.species]);
        if (show->bravoTrainerTower.numFights >= 7)
            sTVShowState = 1;
        else
            sTVShowState = 2;
        break;
    case 1:
        if (show->bravoTrainerTower.btLevel == 50)
        {
            StringCopy(gStringVar1, gText_Lv50);
        }
        else
        {
            StringCopy(gStringVar1, gText_OpenLevel);
        }
        ConvertIntToDecimalString(1, show->bravoTrainerTower.numFights);
        if (show->bravoTrainerTower.wonTheChallenge == TRUE)
            sTVShowState = 3;
        else
            sTVShowState = 4;
        break;
    case 2:
        TVShowConvertInternationalString(gStringVar1, show->bravoTrainerTower.pokemonName, show->bravoTrainerTower.pokemonNameLanguage);
        ConvertIntToDecimalString(1, show->bravoTrainerTower.numFights + 1);
        if (show->bravoTrainerTower.interviewResponse == 0)
            sTVShowState = 5;
        else
            sTVShowState = 6;
        break;
    case 3:
        TVShowConvertInternationalString(gStringVar1, show->bravoTrainerTower.pokemonName, show->bravoTrainerTower.pokemonNameLanguage);
        StringCopy(gStringVar2, gSpeciesNames[show->bravoTrainerTower.defeatedSpecies]);
        if (show->bravoTrainerTower.interviewResponse == 0)
            sTVShowState = 5;
        else
            sTVShowState = 6;
        break;
    case 4:
        TVShowConvertInternationalString(gStringVar1, show->bravoTrainerTower.pokemonName, show->bravoTrainerTower.pokemonNameLanguage);
        StringCopy(gStringVar2, gSpeciesNames[show->bravoTrainerTower.defeatedSpecies]);
        if (show->bravoTrainerTower.interviewResponse == 0)
            sTVShowState = 5;
        else
            sTVShowState = 6;
        break;
    case 5:
        TVShowConvertInternationalString(gStringVar1, show->bravoTrainerTower.pokemonName, show->bravoTrainerTower.pokemonNameLanguage);
        sTVShowState = 11;
        break;
    case 6:
        TVShowConvertInternationalString(gStringVar1, show->bravoTrainerTower.pokemonName, show->bravoTrainerTower.pokemonNameLanguage);
        sTVShowState = 11;
        break;
    case 7:
        sTVShowState = 11;
        break;
    case 8:
    case 9:
    case 10:
        TVShowConvertInternationalString(gStringVar1, show->bravoTrainerTower.trainerName, show->bravoTrainerTower.language);
        sTVShowState = 11;
        break;
    case 11:
        CopyEasyChatWord(gStringVar1, show->bravoTrainerTower.words[0]);
        if (show->bravoTrainerTower.interviewResponse == 0)
            sTVShowState = 12;
        else
            sTVShowState = 13;
        break;
    case 12:
    case 13:
        CopyEasyChatWord(gStringVar1, show->bravoTrainerTower.words[0]);
        TVShowConvertInternationalString(gStringVar2, show->bravoTrainerTower.trainerName, show->bravoTrainerTower.language);
        TVShowConvertInternationalString(gStringVar3, show->bravoTrainerTower.pokemonName, show->bravoTrainerTower.pokemonNameLanguage);
        sTVShowState = 14;
        break;
    case 14:
        TVShowConvertInternationalString(gStringVar1, show->bravoTrainerTower.trainerName, show->bravoTrainerTower.language);
        StringCopy(gStringVar2, gSpeciesNames[show->bravoTrainerTower.species]);
        TVShowDone();
        break;
    }
    ShowFieldMessage(sTVBravoTrainerBattleTowerTextGroup[state]);
}

static void DoTVShowTodaysSmartShopper(void)
{
    TVShow *show;
    u8 state;

    show = &gSaveBlock1Ptr->tvShows[gSpecialVar_0x8004];
    gSpecialVar_Result = FALSE;
    state = sTVShowState;
    switch(state)
    {
    case SMARTSHOPPER_STATE_INTRO:
        TVShowConvertInternationalString(gStringVar1, show->smartshopperShow.playerName, show->smartshopperShow.language);
        GetMapName(gStringVar2, show->smartshopperShow.shopLocation, 0);
        if (show->smartshopperShow.itemAmounts[0] >= 255)
            sTVShowState = SMARTSHOPPER_STATE_CLERK_MAX;
        else
            sTVShowState = SMARTSHOPPER_STATE_CLERK_NORMAL;
        break;
    case SMARTSHOPPER_STATE_CLERK_NORMAL:
        TVShowConvertInternationalString(gStringVar1, show->smartshopperShow.playerName, show->smartshopperShow.language);
        StringCopy(gStringVar2, ItemId_GetName(show->smartshopperShow.itemIds[0]));
        ConvertIntToDecimalString(2, show->smartshopperShow.itemAmounts[0]);
        // Pick a random comment (SMARTSHOPPER_STATE_RAND_COMMENT_#)
        sTVShowState += SMARTSHOPPER_STATE_CLERK_NORMAL + (Random() % (SMARTSHOPPER_STATE_RAND_COMMENT_4 - SMARTSHOPPER_STATE_RAND_COMMENT_1 + 1));
        break;
    case SMARTSHOPPER_STATE_RAND_COMMENT_1:
    case SMARTSHOPPER_STATE_RAND_COMMENT_3:
    case SMARTSHOPPER_STATE_RAND_COMMENT_4:
        if (show->smartshopperShow.itemIds[1] != ITEM_NONE)
            sTVShowState = SMARTSHOPPER_STATE_SECOND_ITEM;
        else
            sTVShowState = SMARTSHOPPER_STATE_IS_VIP;
        break;
    case SMARTSHOPPER_STATE_RAND_COMMENT_2:
        ConvertIntToDecimalString(2, show->smartshopperShow.itemAmounts[0] + 1);
        if (show->smartshopperShow.itemIds[1] != ITEM_NONE)
            sTVShowState = SMARTSHOPPER_STATE_SECOND_ITEM;
        else
            sTVShowState = SMARTSHOPPER_STATE_IS_VIP;
        break;
    case SMARTSHOPPER_STATE_SECOND_ITEM:
        // Clerk describes 2nd type of item player purchased
        StringCopy(gStringVar2, ItemId_GetName(show->smartshopperShow.itemIds[1]));
        ConvertIntToDecimalString(2, show->smartshopperShow.itemAmounts[1]);
        if (show->smartshopperShow.itemIds[2] != ITEM_NONE)
            sTVShowState = SMARTSHOPPER_STATE_THIRD_ITEM;
        else if (show->smartshopperShow.priceReduced == TRUE)
            sTVShowState = SMARTSHOPPER_STATE_DURING_SALE;
        else
            sTVShowState = SMARTSHOPPER_STATE_OUTRO_NORMAL;
        break;
    case SMARTSHOPPER_STATE_THIRD_ITEM:
        // Clerk describes 3rd type of item player purchased
        StringCopy(gStringVar2, ItemId_GetName(show->smartshopperShow.itemIds[2]));
        ConvertIntToDecimalString(2, show->smartshopperShow.itemAmounts[2]);
        if (show->smartshopperShow.priceReduced == TRUE)
            sTVShowState = SMARTSHOPPER_STATE_DURING_SALE;
        else
            sTVShowState = SMARTSHOPPER_STATE_OUTRO_NORMAL;
        break;
    case SMARTSHOPPER_STATE_DURING_SALE:
        if (show->smartshopperShow.itemAmounts[0] >= 255)
            sTVShowState = SMARTSHOPPER_STATE_OUTRO_MAX;
        else
            sTVShowState = SMARTSHOPPER_STATE_OUTRO_NORMAL;
        break;
    case SMARTSHOPPER_STATE_OUTRO_NORMAL:
        SmartShopper_BufferPurchaseTotal(1, show);
        TVShowDone();
        break;
    case SMARTSHOPPER_STATE_IS_VIP:
        // Clerk says customer is a VIP
        // Said if player only purchased one type of item
        if (show->smartshopperShow.priceReduced == TRUE)
            sTVShowState = SMARTSHOPPER_STATE_DURING_SALE;
        else
            sTVShowState = SMARTSHOPPER_STATE_OUTRO_NORMAL;
        break;
    case SMARTSHOPPER_STATE_CLERK_MAX:
        // Clerk's comments if player purchased maximum number of 1st item
        TVShowConvertInternationalString(gStringVar1, show->smartshopperShow.playerName, show->smartshopperShow.language);
        StringCopy(gStringVar2, ItemId_GetName(show->smartshopperShow.itemIds[0]));
        if (show->smartshopperShow.priceReduced == TRUE)
            sTVShowState = SMARTSHOPPER_STATE_DURING_SALE;
        else
            sTVShowState = SMARTSHOPPER_STATE_OUTRO_MAX;
        break;
    case SMARTSHOPPER_STATE_OUTRO_MAX:
        // Outro comments if player purchased maximum number of 1st item
        TVShowConvertInternationalString(gStringVar1, show->smartshopperShow.playerName, show->smartshopperShow.language);
        TVShowDone();
        break;
    }
    ShowFieldMessage(sTVTodaysSmartShopperTextGroup[state]);
}

static void DoTVShowTheNameRaterShow(void)
{
    TVShow *show;
    u8 state;

    show = &gSaveBlock1Ptr->tvShows[gSpecialVar_0x8004];
    gSpecialVar_Result = FALSE;
    state = sTVShowState;
    switch (state)
    {
    case 0:
        TVShowConvertInternationalString(gStringVar1, show->nameRaterShow.trainerName, show->nameRaterShow.language);
        StringCopy(gStringVar2, gSpeciesNames[show->nameRaterShow.species]);
        TVShowConvertInternationalString(gStringVar3, show->nameRaterShow.pokemonName, show->nameRaterShow.pokemonNameLanguage);
        sTVShowState = GetRandomNameRaterStateFromName(show) + 1;
        break;
    case 1:
    case 3:
    case 4:
    case 5:
    case 6:
    case 7:
    case 8:
        if (show->nameRaterShow.random == 0)
            sTVShowState = 9;
        else if (show->nameRaterShow.random == 1)
            sTVShowState = 10;
        else if (show->nameRaterShow.random == 2)
            sTVShowState = 11;
        break;
    case 2:
        TVShowConvertInternationalString(gStringVar1, show->nameRaterShow.trainerName, show->nameRaterShow.language);
        if (show->nameRaterShow.random == 0)
            sTVShowState = 9;
        else if (show->nameRaterShow.random == 1)
            sTVShowState = 10;
        else if (show->nameRaterShow.random == 2)
            sTVShowState = 11;
        break;
    case 9:
    case 10:
    case 11:
        TVShowConvertInternationalString(gStringVar1, show->nameRaterShow.pokemonName, show->nameRaterShow.pokemonNameLanguage);
        GetNicknameSubstring(1, 0, 0, 1, 0, show);
        GetNicknameSubstring(2, 1, 0, 1, 0, show);
        sTVShowState = 12;
        break;
    case 13:
        TVShowConvertInternationalString(gStringVar1, show->nameRaterShow.trainerName, show->nameRaterShow.language);
        GetNicknameSubstring(1, 0, 2, 0, 0, show);
        GetNicknameSubstring(2, 0, 3, 1, 0, show);
        sTVShowState = 14;
        break;
    case 14:
        GetNicknameSubstring(1, 0, 2, 1, 0, show);
        GetNicknameSubstring(2, 0, 3, 0, 0, show);
        sTVShowState = 18;
        break;
    case 15:
        GetNicknameSubstring(0, 0, 2, 1, 0, show);
        StringCopy(gStringVar2, gSpeciesNames[show->nameRaterShow.species]);
        GetNicknameSubstring(2, 0, 3, 2, show->nameRaterShow.species, show);
        sTVShowState = 16;
        break;
    case 16:
        GetNicknameSubstring(0, 0, 2, 2, show->nameRaterShow.species, show);
        GetNicknameSubstring(2, 0, 3, 1, 0, show);
        sTVShowState = 17;
        break;
    case 17:
        GetNicknameSubstring(0, 0, 2, 1, 0, show);
        StringCopy(gStringVar2, gSpeciesNames[show->nameRaterShow.randomSpecies]);
        GetNicknameSubstring(2, 0, 3, 2, show->nameRaterShow.randomSpecies, show);
        sTVShowState = 18;
        break;
    case 12:
        state = 18;
        sTVShowState = 18;
    case 18:
        TVShowConvertInternationalString(gStringVar1, show->nameRaterShow.pokemonName, show->nameRaterShow.pokemonNameLanguage);
        TVShowDone();
        break;
    }
    ShowFieldMessage(sTVNameRaterTextGroup[state]);
}

static void DoTVShowPokemonTodaySuccessfulCapture(void)
{
    TVShow *show;
    u8 state;

    show = &gSaveBlock1Ptr->tvShows[gSpecialVar_0x8004];
    gSpecialVar_Result = FALSE;
    state = sTVShowState;
    switch (state)
    {
    case 0:
        TVShowConvertInternationalString(gStringVar1, show->pokemonToday.playerName, show->pokemonToday.language);
        StringCopy(gStringVar2, gSpeciesNames[show->pokemonToday.species]);
        TVShowConvertInternationalString(gStringVar3, show->pokemonToday.nickname, show->pokemonToday.language2);
        if (show->pokemonToday.ball == BALL_MASTER)
            sTVShowState = 5;
        else
            sTVShowState = 1;
        break;
    case 1:
        sTVShowState = 2;
        break;
    case 2:
        StringCopy(gStringVar2, ItemId_GetName(show->pokemonToday.ball));
        ConvertIntToDecimalString(2, show->pokemonToday.nBallsUsed);
        if (show->pokemonToday.nBallsUsed < 4)
            sTVShowState = 3;
        else
            sTVShowState = 4;
        break;
    case 3:
        TVShowConvertInternationalString(gStringVar1, show->pokemonToday.playerName, show->pokemonToday.language);
        StringCopy(gStringVar2, gSpeciesNames[show->pokemonToday.species]);
        TVShowConvertInternationalString(gStringVar3, show->pokemonToday.nickname, show->pokemonToday.language2);
        sTVShowState = 6;
        break;
    case 4:
        sTVShowState = 6;
        break;
    case 5:
        TVShowConvertInternationalString(gStringVar1, show->pokemonToday.playerName, show->pokemonToday.language);
        StringCopy(gStringVar2, gSpeciesNames[show->pokemonToday.species]);
        sTVShowState = 6;
        break;
    case 6:
        TVShowConvertInternationalString(gStringVar1, show->pokemonToday.playerName, show->pokemonToday.language);
        StringCopy(gStringVar2, gSpeciesNames[show->pokemonToday.species]);
        TVShowConvertInternationalString(gStringVar3, show->pokemonToday.nickname, show->pokemonToday.language2);
        sTVShowState += 1 + (Random() % 4);
        break;
    case 7:
    case 8:
        StringCopy(gStringVar1, gSpeciesNames[show->pokemonToday.species]);
        TVShowConvertInternationalString(gStringVar2, show->pokemonToday.nickname, show->pokemonToday.language2);
        GetRandomDifferentSpeciesAndNameSeenByPlayer(2, show->pokemonToday.species);
        sTVShowState = 11;
        break;
    case 9:
    case 10:
        StringCopy(gStringVar1, gSpeciesNames[show->pokemonToday.species]);
        TVShowConvertInternationalString(gStringVar2, show->pokemonToday.nickname, show->pokemonToday.language2);
        sTVShowState = 11;
        break;
    case 11:
        TVShowDone();
        break;
    }
    ShowFieldMessage(sTVPokemonTodaySuccessfulTextGroup[state]);
}

static void DoTVShowPokemonTodayFailedCapture(void)
{
    TVShow *show;
    u8 state;

    show = &gSaveBlock1Ptr->tvShows[gSpecialVar_0x8004];
    gSpecialVar_Result = FALSE;
    state = sTVShowState;
    switch (state)
    {
    case 0:
        TVShowConvertInternationalString(gStringVar1, show->pokemonTodayFailed.playerName, show->pokemonTodayFailed.language);
        StringCopy(gStringVar2, gSpeciesNames[show->pokemonTodayFailed.species]);
        sTVShowState = 1;
        break;
    case 1:
        TVShowConvertInternationalString(gStringVar1, show->pokemonTodayFailed.playerName, show->pokemonTodayFailed.language);
        GetMapName(gStringVar2, show->pokemonTodayFailed.location, 0);
        StringCopy(gStringVar3, gSpeciesNames[show->pokemonTodayFailed.species2]);
        if (show->pokemonTodayFailed.outcome == 1)
            sTVShowState = 3;
        else
            sTVShowState = 2;
        break;
    case 2:
    case 3:
        TVShowConvertInternationalString(gStringVar1, show->pokemonTodayFailed.playerName, show->pokemonTodayFailed.language);
        ConvertIntToDecimalString(1, show->pokemonTodayFailed.nBallsUsed);
        if (Random() % 3 == 0)
            sTVShowState = 5;
        else
            sTVShowState = 4;
        break;
    case 4:
    case 5:
        TVShowConvertInternationalString(gStringVar1, show->pokemonTodayFailed.playerName, show->pokemonTodayFailed.language);
        sTVShowState = 6;
        break;
    case 6:
        TVShowDone();
        break;
    }
    ShowFieldMessage(sTVPokemonTodayFailedTextGroup[state]);
}

static void DoTVShowPokemonFanClubLetter(void)
{
    TVShow *show;
    u8 state;
    u16 rval;

    show = &gSaveBlock1Ptr->tvShows[gSpecialVar_0x8004];
    gSpecialVar_Result = FALSE;
    state = sTVShowState;
    switch (state)
    {
    case 0:
        TVShowConvertInternationalString(gStringVar1, show->fanclubLetter.playerName, show->fanclubLetter.language);
        StringCopy(gStringVar2, gSpeciesNames[show->fanclubLetter.species]);
        sTVShowState = 50;
        break;
    case 1:
        rval = (Random() % 4) + 1;
        if (rval == 1)
            sTVShowState = 2;
        else
            sTVShowState = rval + 2;
        break;
    case 2:
        sTVShowState = 51;
        break;
    case 3:
        sTVShowState += (Random() % 3) + 1;
        break;
    case 4:
    case 5:
    case 6:
        GetRandomWordFromShow(show);
        sTVShowState = 7;
        break;
    case 7:
        rval = (Random() % 0x1f) + 0x46;
        ConvertIntToDecimalString(2, rval);
        TVShowDone();
        break;
    case 50:
        ConvertEasyChatWordsToString(gStringVar4, show->fanclubLetter.words, 2, 2);
        ShowFieldMessage(gStringVar4);
        sTVShowState = 1;
        return;
    case 51:
        ConvertEasyChatWordsToString(gStringVar4, show->fanclubLetter.words, 2, 2);
        ShowFieldMessage(gStringVar4);
        sTVShowState = 3;
        return;
    }
    ShowFieldMessage(sTVFanClubTextGroup[state]);
}

static void DoTVShowRecentHappenings(void)
{
    TVShow *show;
    u8 state;

    show = &gSaveBlock1Ptr->tvShows[gSpecialVar_0x8004];
    gSpecialVar_Result = FALSE;
    state = sTVShowState;
    switch (state)
    {
    case 0:
        TVShowConvertInternationalString(gStringVar1, show->recentHappenings.playerName, show->recentHappenings.language);
        GetRandomWordFromShow(show);
        sTVShowState = 50;
        break;
    case 1:
        sTVShowState += 1 + (Random() % 3);
        break;
    case 2:
    case 3:
    case 4:
        sTVShowState = 5;
        break;
    case 5:
        TVShowDone();
        break;
    case 50:
        ConvertEasyChatWordsToString(gStringVar4, show->recentHappenings.words, 2, 2);
        ShowFieldMessage(gStringVar4);
        sTVShowState = 1;
        return;
    }
    ShowFieldMessage(sTVRecentHappeninssTextGroup[state]);
}

static void DoTVShowPokemonFanClubOpinions(void)
{
    TVShow *show;
    u8 state;

    show = &gSaveBlock1Ptr->tvShows[gSpecialVar_0x8004];
    gSpecialVar_Result = FALSE;
    state = sTVShowState;
    switch (state)
    {
    case 0:
        TVShowConvertInternationalString(gStringVar1, show->fanclubOpinions.playerName, show->fanclubOpinions.language);
        StringCopy(gStringVar2, gSpeciesNames[show->fanclubOpinions.species]);
        TVShowConvertInternationalString(gStringVar3, show->fanclubOpinions.nickname, show->fanclubOpinions.pokemonNameLanguage);
        sTVShowState = show->fanclubOpinions.questionAsked + 1;
        break;
    case 1:
    case 2:
    case 3:
        TVShowConvertInternationalString(gStringVar1, show->fanclubOpinions.playerName, show->fanclubOpinions.language);
        StringCopy(gStringVar2, gSpeciesNames[show->fanclubOpinions.species]);
        CopyEasyChatWord(gStringVar3, show->fanclubOpinions.words[0]);
        sTVShowState = 4;
        break;
    case 4:
        TVShowConvertInternationalString(gStringVar1, show->fanclubOpinions.playerName, show->fanclubOpinions.language);
        CopyEasyChatWord(gStringVar3, show->fanclubOpinions.words[1]);
        TVShowDone();
        break;
    }
    ShowFieldMessage(sTVFanClubOpinionsTextGroup[state]);
}

static void DoTVShowDummiedOut(void)
{

}

static void DoTVShowPokemonNewsMassOutbreak(void)
{
    TVShow *show;

    show = &gSaveBlock1Ptr->tvShows[gSpecialVar_0x8004];
    GetMapName(gStringVar1, show->massOutbreak.locationMapNum, 0);
    StringCopy(gStringVar2, gSpeciesNames[show->massOutbreak.species]);
    TVShowDone();
    StartMassOutbreak();
    ShowFieldMessage(sTVMassOutbreakTextGroup[sTVShowState]);
}

static void DoTVShowPokemonBattleUpdate(void)
{
    TVShow *show;
    u8 state;

    show = &gSaveBlock1Ptr->tvShows[gSpecialVar_0x8004];
    gSpecialVar_Result = FALSE;
    state = sTVShowState;
    switch (state)
    {
    case 0:
        switch (show->battleUpdate.battleType)
        {
        case 0:
        case 1:
            sTVShowState = 1;
            break;
        case 2:
            sTVShowState = 5;
            break;
        }
        break;
    case 1:
        TVShowConvertInternationalString(gStringVar1, show->battleUpdate.playerName, show->battleUpdate.language);
        TVShowConvertInternationalString(gStringVar2, show->battleUpdate.linkOpponentName, show->battleUpdate.linkOpponentLanguage);
        if (show->battleUpdate.battleType == 0)
        {
            StringCopy(gStringVar3, gText_Single);
        }
        else
        {
            StringCopy(gStringVar3, gText_Double);
        }
        sTVShowState = 2;
        break;
    case 2:
        TVShowConvertInternationalString(gStringVar1, show->battleUpdate.playerName, show->battleUpdate.language);
        StringCopy(gStringVar2, gSpeciesNames[show->battleUpdate.speciesPlayer]);
        StringCopy(gStringVar3, gMoveNames[show->battleUpdate.move]);
        sTVShowState = 3;
        break;
    case 3:
        TVShowConvertInternationalString(gStringVar1, show->battleUpdate.linkOpponentName, show->battleUpdate.linkOpponentLanguage);
        StringCopy(gStringVar2, gSpeciesNames[show->battleUpdate.speciesOpponent]);
        sTVShowState = 4;
        break;
    case 4:
        TVShowConvertInternationalString(gStringVar1, show->battleUpdate.playerName, show->battleUpdate.language);
        TVShowConvertInternationalString(gStringVar2, show->battleUpdate.linkOpponentName, show->battleUpdate.linkOpponentLanguage);
        TVShowDone();
        break;
    case 5:
        TVShowConvertInternationalString(gStringVar1, show->battleUpdate.playerName, show->battleUpdate.language);
        TVShowConvertInternationalString(gStringVar2, show->battleUpdate.linkOpponentName, show->battleUpdate.linkOpponentLanguage);
        sTVShowState = 6;
        break;
    case 6:
        TVShowConvertInternationalString(gStringVar1, show->battleUpdate.playerName, show->battleUpdate.language);
        StringCopy(gStringVar2, gSpeciesNames[show->battleUpdate.speciesPlayer]);
        StringCopy(gStringVar3, gMoveNames[show->battleUpdate.move]);
        sTVShowState = 7;
        break;
    case 7:
        TVShowConvertInternationalString(gStringVar1, show->battleUpdate.playerName, show->battleUpdate.language);
        TVShowConvertInternationalString(gStringVar2, show->battleUpdate.linkOpponentName, show->battleUpdate.linkOpponentLanguage);
        StringCopy(gStringVar3, gSpeciesNames[show->battleUpdate.speciesOpponent]);
        TVShowDone();
        break;
    }
    ShowFieldMessage(sTVPokemonBattleUpdateTextGroup[state]);
}

void DoTVShowInSearchOfTrainers(void)
{
    u8 state;

    gSpecialVar_Result = FALSE;
    state = sTVShowState;
    switch (state)
    {
    case 0:
        GetMapName(gStringVar1, gSaveBlock1Ptr->gabbyAndTyData.mapnum, 0);
        if (gSaveBlock1Ptr->gabbyAndTyData.battleNum > 1)
            sTVShowState = 1;
        else
            sTVShowState = 2;
        break;
    case 1:
        sTVShowState = 2;
        break;
    case 2:
        if (!gSaveBlock1Ptr->gabbyAndTyData.battleTookMoreThanOneTurn)
            sTVShowState = 4;
        else if (gSaveBlock1Ptr->gabbyAndTyData.playerThrewABall)
            sTVShowState = 5;
        else if (gSaveBlock1Ptr->gabbyAndTyData.playerUsedHealingItem)
            sTVShowState = 6;
        else if (gSaveBlock1Ptr->gabbyAndTyData.playerLostAMon)
            sTVShowState = 7;
        else
            sTVShowState = 3;
        break;
    case 3:
        StringCopy(gStringVar1, gSpeciesNames[gSaveBlock1Ptr->gabbyAndTyData.mon1]);
        StringCopy(gStringVar2, gMoveNames[gSaveBlock1Ptr->gabbyAndTyData.lastMove]);
        StringCopy(gStringVar3, gSpeciesNames[gSaveBlock1Ptr->gabbyAndTyData.mon2]);
        sTVShowState = 8;
        break;
    case 4:
    case 5:
    case 6:
    case 7:
        sTVShowState = 8;
        break;
    case 8:
        CopyEasyChatWord(gStringVar1, gSaveBlock1Ptr->gabbyAndTyData.quote[0]);
        StringCopy(gStringVar2, gSpeciesNames[gSaveBlock1Ptr->gabbyAndTyData.mon1]);
        StringCopy(gStringVar3, gSpeciesNames[gSaveBlock1Ptr->gabbyAndTyData.mon2]);
        gSpecialVar_Result = TRUE;
        sTVShowState = 0;
        TakeGabbyAndTyOffTheAir();
        break;
    }
    ShowFieldMessage(sTVInSearchOfTrainersTextGroup[state]);
}

static void DoTVShowPokemonAngler(void)
{
    TVShow *show;
    u8 state;

    show = &gSaveBlock1Ptr->tvShows[gSpecialVar_0x8004];
    gSpecialVar_Result = FALSE;
    if (show->pokemonAngler.nBites < show->pokemonAngler.nFails)
        sTVShowState = 0;
    else
        sTVShowState = 1;
    state = sTVShowState;
    switch (state)
    {
    case 0:
        TVShowConvertInternationalString(gStringVar1, show->pokemonAngler.playerName, show->pokemonAngler.language);
        StringCopy(gStringVar2, gSpeciesNames[show->pokemonAngler.species]);
        ConvertIntToDecimalString(2, show->pokemonAngler.nFails);
        TVShowDone();
        break;
    case 1:
        TVShowConvertInternationalString(gStringVar1, show->pokemonAngler.playerName, show->pokemonAngler.language);
        StringCopy(gStringVar2, gSpeciesNames[show->pokemonAngler.species]);
        ConvertIntToDecimalString(2, show->pokemonAngler.nBites);
        TVShowDone();
        break;
    }
    ShowFieldMessage(sTVPokemonAnslerTextGroup[state]);
}

static void DoTVShowTheWorldOfMasters(void)
{
    TVShow *show;
    u8 state;

    show = &gSaveBlock1Ptr->tvShows[gSpecialVar_0x8004];
    gSpecialVar_Result = FALSE;
    state = sTVShowState;
    switch (state)
    {
    case 0:
        TVShowConvertInternationalString(gStringVar1, show->worldOfMasters.playerName, show->worldOfMasters.language);
        ConvertIntToDecimalString(1, show->worldOfMasters.steps);
        ConvertIntToDecimalString(2, show->worldOfMasters.numPokeCaught);
        sTVShowState = 1;
        break;
    case 1:
        StringCopy(gStringVar1, gSpeciesNames[show->worldOfMasters.species]);
        sTVShowState = 2;
        break;
    case 2:
        TVShowConvertInternationalString(gStringVar1, show->worldOfMasters.playerName, show->worldOfMasters.language);
        GetMapName(gStringVar2, show->worldOfMasters.location, 0);
        StringCopy(gStringVar3, gSpeciesNames[show->worldOfMasters.caughtPoke]);
        TVShowDone();
        break;
    }
    ShowFieldMessage(sTVWorldOfMastersTextGroup[state]);
}

static void DoTVShowTodaysRivalTrainer(void)
{
    TVShow *show;
    u8 state;

    show = &gSaveBlock1Ptr->tvShows[gSpecialVar_0x8004];
    gSpecialVar_Result = FALSE;
    state = sTVShowState;
    switch (state)
    {
    case 0:
        switch (show->rivalTrainer.location)
        {
        default:
            sTVShowState = 7;
            break;
        case MAPSEC_SECRET_BASE:
            sTVShowState = 8;
            break;
        case MAPSEC_DYNAMIC:
            switch (show->rivalTrainer.mapLayoutId)
            {
            case LAYOUT_SS_TIDAL_CORRIDOR:
            case LAYOUT_SS_TIDAL_LOWER_DECK:
            case LAYOUT_SS_TIDAL_ROOMS:
                sTVShowState = 10;
                break;
            default:
                sTVShowState = 9;
                break;
            }
            break;
        }
        break;
    case 7:
        TVShowConvertInternationalString(gStringVar1, show->rivalTrainer.playerName, show->rivalTrainer.language);
        ConvertIntToDecimalString(1, show->rivalTrainer.dexCount);
        GetMapName(gStringVar3, show->rivalTrainer.location, 0);
        if (show->rivalTrainer.badgeCount != 0)
            sTVShowState = 1;
        else
            sTVShowState = 2;
        break;
    case 8:
        TVShowConvertInternationalString(gStringVar1, show->rivalTrainer.playerName, show->rivalTrainer.language);
        ConvertIntToDecimalString(1, show->rivalTrainer.dexCount);
        if (show->rivalTrainer.badgeCount != 0)
            sTVShowState = 1;
        else
            sTVShowState = 2;
        break;
    case 9:
        TVShowConvertInternationalString(gStringVar1, show->rivalTrainer.playerName, show->rivalTrainer.language);
        ConvertIntToDecimalString(1, show->rivalTrainer.dexCount);
        if (show->rivalTrainer.badgeCount != 0)
            sTVShowState = 1;
        else
            sTVShowState = 2;
        break;
    case 10:
        TVShowConvertInternationalString(gStringVar1, show->rivalTrainer.playerName, show->rivalTrainer.language);
        ConvertIntToDecimalString(1, show->rivalTrainer.dexCount);
        if (show->rivalTrainer.badgeCount != 0)
            sTVShowState = 1;
        else
            sTVShowState = 2;
        break;
    case 1:
        ConvertIntToDecimalString(0, show->rivalTrainer.badgeCount);
        if (FlagGet(FLAG_LANDMARK_BATTLE_FRONTIER))
        {
            if (show->rivalTrainer.nSilverSymbols || show->rivalTrainer.nGoldSymbols)
                sTVShowState = 4;
            else
                sTVShowState = 3;
        }
        else
        {
            sTVShowState = 6;
        }
        break;
    case 2:
        if (FlagGet(FLAG_LANDMARK_BATTLE_FRONTIER))
        {
            if (show->rivalTrainer.nSilverSymbols || show->rivalTrainer.nGoldSymbols)
                sTVShowState = 4;
            else
                sTVShowState = 3;
        }
        else
        {
            sTVShowState = 6;
        }
        break;
    case 3:
        if (show->rivalTrainer.battlePoints == 0)
            sTVShowState = 6;
        else
            sTVShowState = 5;
        break;
    case 4:
        ConvertIntToDecimalString(0, show->rivalTrainer.nGoldSymbols);
        ConvertIntToDecimalString(1, show->rivalTrainer.nSilverSymbols);
        if (show->rivalTrainer.battlePoints == 0)
            sTVShowState = 6;
        else
            sTVShowState = 5;
        break;
    case 5:
        ConvertIntToDecimalString(0, show->rivalTrainer.battlePoints);
        sTVShowState = 6;
        break;
    case 6:
        TVShowConvertInternationalString(gStringVar1, show->rivalTrainer.playerName, show->rivalTrainer.language);
        TVShowDone();
    }
    ShowFieldMessage(sTVTodaysRivalTrainerTextGroup[state]);
}

static void DoTVShowDewfordTrendWatcherNetwork(void)
{
    TVShow *show;
    u8 state;

    show = &gSaveBlock1Ptr->tvShows[gSpecialVar_0x8004];
    gSpecialVar_Result = FALSE;
    state = sTVShowState;
    switch (state)
    {
    case TRENDWATCHER_STATE_INTRO:
        CopyEasyChatWord(gStringVar1, show->trendWatcher.words[0]);
        CopyEasyChatWord(gStringVar2, show->trendWatcher.words[1]);
        if (show->trendWatcher.gender == MALE)
            sTVShowState = TRENDWATCHER_STATE_TAUGHT_MALE;
        else
            sTVShowState = TRENDWATCHER_STATE_TAUGHT_FEMALE;
        break;
    case TRENDWATCHER_STATE_TAUGHT_MALE:
    case TRENDWATCHER_STATE_TAUGHT_FEMALE:
        CopyEasyChatWord(gStringVar1, show->trendWatcher.words[0]);
        CopyEasyChatWord(gStringVar2, show->trendWatcher.words[1]);
        TVShowConvertInternationalString(gStringVar3, show->trendWatcher.playerName, show->trendWatcher.language);
        sTVShowState = TRENDWATCHER_STATE_PHRASE_HOPELESS;
        break;
    case TRENDWATCHER_STATE_PHRASE_HOPELESS:
        CopyEasyChatWord(gStringVar1, show->trendWatcher.words[0]);
        CopyEasyChatWord(gStringVar2, show->trendWatcher.words[1]);
        if (show->trendWatcher.gender == MALE)
            sTVShowState = TRENDWATCHER_STATE_BIGGER_MALE;
        else
            sTVShowState = TRENDWATCHER_STATE_BIGGER_FEMALE;
        break;
    case TRENDWATCHER_STATE_BIGGER_MALE:
    case TRENDWATCHER_STATE_BIGGER_FEMALE:
        CopyEasyChatWord(gStringVar1, show->trendWatcher.words[0]);
        CopyEasyChatWord(gStringVar2, show->trendWatcher.words[1]);
        TVShowConvertInternationalString(gStringVar3, show->trendWatcher.playerName, show->trendWatcher.language);
        sTVShowState = TRENDWATCHER_STATE_OUTRO;
        break;
    case TRENDWATCHER_STATE_OUTRO:
        CopyEasyChatWord(gStringVar1, show->trendWatcher.words[0]);
        CopyEasyChatWord(gStringVar2, show->trendWatcher.words[1]);
        TVShowDone();
    }
    ShowFieldMessage(sTVDewfordTrendWatcherNetworkTextGroup[state]);
}

static void DoTVShowHoennTreasureInvestigators(void)
{
    TVShow *show;
    u8 state;

    show = &gSaveBlock1Ptr->tvShows[gSpecialVar_0x8004];
    gSpecialVar_Result = FALSE;
    state = sTVShowState;
    switch (state)
    {
    case 0:
        StringCopy(gStringVar1, ItemId_GetName(show->treasureInvestigators.item));
        if (show->treasureInvestigators.location == MAPSEC_DYNAMIC)
        {
            switch (show->treasureInvestigators.mapLayoutId)
            {
            case LAYOUT_SS_TIDAL_CORRIDOR:
            case LAYOUT_SS_TIDAL_LOWER_DECK:
            case LAYOUT_SS_TIDAL_ROOMS:
                sTVShowState = 2;
                break;
            default:
                sTVShowState = 1;
                break;
            }
        }
        else
        {
            sTVShowState = 1;
        }
        break;
    case 1:
        StringCopy(gStringVar1, ItemId_GetName(show->treasureInvestigators.item));
        TVShowConvertInternationalString(gStringVar2, show->treasureInvestigators.playerName, show->treasureInvestigators.language);
        GetMapName(gStringVar3, show->treasureInvestigators.location, 0);
        TVShowDone();
        break;
    case 2:
        StringCopy(gStringVar1, ItemId_GetName(show->treasureInvestigators.item));
        TVShowConvertInternationalString(gStringVar2, show->treasureInvestigators.playerName, show->treasureInvestigators.language);
        TVShowDone();
        break;
    }
    ShowFieldMessage(sTVHoennTreasureInvestisatorsTextGroup[state]);
}

static void DoTVShowFindThatGamer(void)
{
    TVShow *show;
    u8 state;

    show = &gSaveBlock1Ptr->tvShows[gSpecialVar_0x8004];
    gSpecialVar_Result = FALSE;
    state = sTVShowState;
    switch (state)
    {
    case 0:
        TVShowConvertInternationalString(gStringVar1, show->findThatGamer.playerName, show->findThatGamer.language);
        switch (show->findThatGamer.whichGame)
        {
        case 0:
            StringCopy(gStringVar2, gText_Slots);
            break;
        case 1:
            StringCopy(gStringVar2, gText_Roulette);
            break;
        }
        if (show->findThatGamer.won == TRUE)
            sTVShowState = 1;
        else
            sTVShowState = 2;
        break;
    case 1:
        TVShowConvertInternationalString(gStringVar1, show->findThatGamer.playerName, show->findThatGamer.language);
        switch (show->findThatGamer.whichGame)
        {
        case 0:
            StringCopy(gStringVar2, gText_Slots);
            break;
        case 1:
            StringCopy(gStringVar2, gText_Roulette);
            break;
        }
        ConvertIntToDecimalString(2, show->findThatGamer.nCoins);
        TVShowDone(); break;
    case 2:
        TVShowConvertInternationalString(gStringVar1, show->findThatGamer.playerName, show->findThatGamer.language);
        switch (show->findThatGamer.whichGame)
        {
        case 0:
            StringCopy(gStringVar2, gText_Slots);
            break;
        case 1:
            StringCopy(gStringVar2, gText_Roulette);
            break;
        }
        ConvertIntToDecimalString(2, show->findThatGamer.nCoins);
        sTVShowState = 3;
        break;
    case 3:
        TVShowConvertInternationalString(gStringVar1, show->findThatGamer.playerName, show->findThatGamer.language);
        switch (show->findThatGamer.whichGame)
        {
        case 0:
            StringCopy(gStringVar2, gText_Roulette);
            break;
        case 1:
            StringCopy(gStringVar2, gText_Slots);
            break;
        }
        TVShowDone();
        break;
    }
    ShowFieldMessage(sTVFindThatGamerTextGroup[state]);
}

static void DoTVShowBreakingNewsTV(void)
{
    TVShow *show;
    u8 state;

    show = &gSaveBlock1Ptr->tvShows[gSpecialVar_0x8004];
    gSpecialVar_Result = FALSE;
    state = sTVShowState;
    switch (state)
    {
    case 0:
        if (show->breakingNews.outcome == 0)
            sTVShowState = 1;
        else
            sTVShowState = 5;
        break;
    case 1:
        TVShowConvertInternationalString(gStringVar1, show->breakingNews.playerName, show->breakingNews.language);
        StringCopy(gStringVar2, gSpeciesNames[show->breakingNews.lastOpponentSpecies]);
        GetMapName(gStringVar3, show->breakingNews.location, 0);
        sTVShowState = 2;
        break;
    case 2:
        TVShowConvertInternationalString(gStringVar1, show->breakingNews.playerName, show->breakingNews.language);
        StringCopy(gStringVar2, gSpeciesNames[show->breakingNews.lastOpponentSpecies]);
        StringCopy(gStringVar3, gSpeciesNames[show->breakingNews.poke1Species]);
        sTVShowState = 3;
        break;
    case 3:
        ConvertIntToDecimalString(0, show->breakingNews.balls);
        StringCopy(gStringVar2, ItemId_GetName(show->breakingNews.caughtMonBall));
        sTVShowState = 4;
        break;
    case 4:
        TVShowConvertInternationalString(gStringVar1, show->breakingNews.playerName, show->breakingNews.language);
        GetMapName(gStringVar2, show->breakingNews.location, 0);
        TVShowDone();
        break;
    case 5:
        TVShowConvertInternationalString(gStringVar1, show->breakingNews.playerName, show->breakingNews.language);
        StringCopy(gStringVar2, gSpeciesNames[show->breakingNews.lastOpponentSpecies]);
        GetMapName(gStringVar3, show->breakingNews.location, 0);
        sTVShowState = 6;
        break;
    case 6:
        TVShowConvertInternationalString(gStringVar1, show->breakingNews.playerName, show->breakingNews.language);
        StringCopy(gStringVar2, gSpeciesNames[show->breakingNews.lastOpponentSpecies]);
        StringCopy(gStringVar3, gSpeciesNames[show->breakingNews.poke1Species]);
        switch (show->breakingNews.outcome)
        {
        case 1:
            if (show->breakingNews.lastUsedMove == MOVE_NONE)
                sTVShowState = 12;
            else
                sTVShowState = 7;
            break;
        case 2:
            sTVShowState = 9;
            break;
        case 3:
            sTVShowState = 10;
            break;
        }
        break;
    case 7:
        StringCopy(gStringVar1, gMoveNames[show->breakingNews.lastUsedMove]);
        StringCopy(gStringVar2, gSpeciesNames[show->breakingNews.poke1Species]);
        sTVShowState = 8;
        break;
    case 12:
        TVShowConvertInternationalString(gStringVar1, show->breakingNews.playerName, show->breakingNews.language);
        StringCopy(gStringVar2, gSpeciesNames[show->breakingNews.lastOpponentSpecies]);
        StringCopy(gStringVar3, gSpeciesNames[show->breakingNews.poke1Species]);
        sTVShowState = 8;
        break;
    case 8:
        TVShowConvertInternationalString(gStringVar1, show->breakingNews.playerName, show->breakingNews.language);
        GetMapName(gStringVar2, show->breakingNews.location, 0);
        sTVShowState = 11;
        break;
    case 9:
    case 10:
        TVShowConvertInternationalString(gStringVar1, show->breakingNews.playerName, show->breakingNews.language);
        StringCopy(gStringVar2, gSpeciesNames[show->breakingNews.lastOpponentSpecies]);
        GetMapName(gStringVar3, show->breakingNews.location, 0);
        sTVShowState = 11;
        break;
    case 11:
        TVShowConvertInternationalString(gStringVar1, show->breakingNews.playerName, show->breakingNews.language);
        TVShowDone();
        break;
    }
    ShowFieldMessage(sTVBreakingNewsTextGroup[state]);
}

static void DoTVShowPokemonLotteryWinnerFlashReport(void)
{
    TVShow *show;
    u8 state;

    show = &gSaveBlock1Ptr->tvShows[gSpecialVar_0x8004];
    gSpecialVar_Result = FALSE;
    state = sTVShowState;
    TVShowConvertInternationalString(gStringVar1, show->lottoWinner.playerName, show->lottoWinner.language);
    if (show->lottoWinner.whichPrize == 0)
    {
        StringCopy(gStringVar2, gText_Jackpot);
    }
    else if (show->lottoWinner.whichPrize == 1)
    {
        StringCopy(gStringVar2, gText_First);
    }
    else if (show->lottoWinner.whichPrize == 2)
    {
        StringCopy(gStringVar2, gText_Second);
    }
    else
    {
        StringCopy(gStringVar2, gText_Third);
    }
    StringCopy(gStringVar3, ItemId_GetName(show->lottoWinner.item));
    TVShowDone();
    ShowFieldMessage(sTVPokemonLotteryWinnerFlashReportTextGroup[state]);
}

static void DoTVShowThePokemonBattleSeminar(void)
{
    TVShow *show;
    u8 state;

    show = &gSaveBlock1Ptr->tvShows[gSpecialVar_0x8004];
    gSpecialVar_Result = FALSE;
    state = sTVShowState;
    switch (state)
    {
    case 0:
        TVShowConvertInternationalString(gStringVar1, show->battleSeminar.playerName, show->battleSeminar.language);
        StringCopy(gStringVar2, gSpeciesNames[show->battleSeminar.species]);
        StringCopy(gStringVar3, gSpeciesNames[show->battleSeminar.foeSpecies]);
        sTVShowState = 1;
        break;
    case 1:
        TVShowConvertInternationalString(gStringVar1, show->battleSeminar.playerName, show->battleSeminar.language);
        StringCopy(gStringVar2, gSpeciesNames[show->battleSeminar.foeSpecies]);
        StringCopy(gStringVar3, gMoveNames[show->battleSeminar.move]);
        sTVShowState = 2;
        break;
    case 2:
        StringCopy(gStringVar1, gSpeciesNames[show->battleSeminar.species]);
        switch (show->battleSeminar.nOtherMoves)
        {
        case 1:
            sTVShowState = 5;
            break;
        case 2:
            sTVShowState = 4;
            break;
        case 3:
            sTVShowState = 3;
            break;
        default:
            sTVShowState = 6;
            break;
        }
        break;
    case 3:
        StringCopy(gStringVar1, gMoveNames[show->battleSeminar.otherMoves[0]]);
        StringCopy(gStringVar2, gMoveNames[show->battleSeminar.otherMoves[1]]);
        StringCopy(gStringVar3, gMoveNames[show->battleSeminar.otherMoves[2]]);
        sTVShowState = 6;
        break;
    case 4:
        StringCopy(gStringVar1, gMoveNames[show->battleSeminar.otherMoves[0]]);
        StringCopy(gStringVar2, gMoveNames[show->battleSeminar.otherMoves[1]]);
        sTVShowState = 6;
        break;
    case 5:
        StringCopy(gStringVar2, gMoveNames[show->battleSeminar.otherMoves[0]]);
        sTVShowState = 6;
        break;
    case 6:
        StringCopy(gStringVar1, gMoveNames[show->battleSeminar.betterMove]);
        StringCopy(gStringVar2, gMoveNames[show->battleSeminar.move]);
        TVShowDone();
        break;
    }
    ShowFieldMessage(sTVThePokemonBattleSeminarTextGroup[state]);
}

static void DoTVShowTrainerFanClubSpecial(void)
{
    TVShow *show;
    u8 state;

    show = &gSaveBlock1Ptr->tvShows[gSpecialVar_0x8004];
    gSpecialVar_Result = FALSE;
    state = sTVShowState;
    switch (state)
    {
    case 0:
        TVShowConvertInternationalString(gStringVar1, show->fanClubSpecial.idolName, show->fanClubSpecial.idolNameLanguage);
        TVShowConvertInternationalString(gStringVar2, show->fanClubSpecial.playerName, show->fanClubSpecial.language);
        CopyEasyChatWord(gStringVar3, show->fanClubSpecial.words[0]);
        if (show->fanClubSpecial.score >= 90)
            sTVShowState = 1;
        else if (show->fanClubSpecial.score >= 70)
            sTVShowState = 2;
        else if (show->fanClubSpecial.score >= 30)
            sTVShowState = 3;
        else
            sTVShowState = 4;
        break;
    case 1:
        TVShowConvertInternationalString(gStringVar1, show->fanClubSpecial.idolName, show->fanClubSpecial.idolNameLanguage);
        TVShowConvertInternationalString(gStringVar2, show->fanClubSpecial.playerName, show->fanClubSpecial.language);
        ConvertIntToDecimalString(2, show->fanClubSpecial.score);
        sTVShowState = 5;
        break;
    case 2:
        TVShowConvertInternationalString(gStringVar1, show->fanClubSpecial.idolName, show->fanClubSpecial.idolNameLanguage);
        TVShowConvertInternationalString(gStringVar2, show->fanClubSpecial.playerName, show->fanClubSpecial.language);
        ConvertIntToDecimalString(2, show->fanClubSpecial.score);
        sTVShowState = 5;
        break;
    case 3:
        TVShowConvertInternationalString(gStringVar1, show->fanClubSpecial.idolName, show->fanClubSpecial.idolNameLanguage);
        TVShowConvertInternationalString(gStringVar2, show->fanClubSpecial.playerName, show->fanClubSpecial.language);
        ConvertIntToDecimalString(2, show->fanClubSpecial.score);
        sTVShowState = 5;
        break;
    case 4:
        TVShowConvertInternationalString(gStringVar1, show->fanClubSpecial.idolName, show->fanClubSpecial.idolNameLanguage);
        TVShowConvertInternationalString(gStringVar2, show->fanClubSpecial.playerName, show->fanClubSpecial.language);
        ConvertIntToDecimalString(2, show->fanClubSpecial.score);
        sTVShowState = 5;
        break;
    case 5:
        TVShowConvertInternationalString(gStringVar1, show->fanClubSpecial.idolName, show->fanClubSpecial.idolNameLanguage);
        TVShowConvertInternationalString(gStringVar2, show->fanClubSpecial.playerName, show->fanClubSpecial.language);
        CopyEasyChatWord(gStringVar3, show->fanClubSpecial.words[0]);
        TVShowDone();
        break;
    }
    ShowFieldMessage(sTVTrainerFanClubSpecialTextGroup[state]);
}

static void DoTVShowTrainerFanClub(void)
{
    TVShow *show;
    u8 state;
    u32 playerId;

    show = &gSaveBlock1Ptr->tvShows[gSpecialVar_0x8004];
    gSpecialVar_Result = FALSE;
    state = sTVShowState;
    switch (state)
    {
    case 0:
        TVShowConvertInternationalString(gStringVar1, show->trainerFanClub.playerName, show->trainerFanClub.language);
        playerId = ((show->common.trainerIdHi << 8) + show->common.trainerIdLo);
        switch (playerId % 10)
        {
        case 0:
            sTVShowState = 1;
            break;
        case 1:
            sTVShowState = 2;
            break;
        case 2:
            sTVShowState = 3;
            break;
        case 3:
            sTVShowState = 4;
            break;
        case 4:
            sTVShowState = 5;
            break;
        case 5:
            sTVShowState = 6;
            break;
        case 6:
            sTVShowState = 7;
            break;
        case 7:
            sTVShowState = 8;
            break;
        case 8:
            sTVShowState = 9;
            break;
        case 9:
            sTVShowState = 10;
            break;
        }
        break;
    case 1:
        sTVShowState = 11;
        break;
    case 2:
        sTVShowState = 11;
        break;
    case 3:
        sTVShowState = 11;
        break;
    case 4:
        sTVShowState = 11;
        break;
    case 5:
        sTVShowState = 11;
        break;
    case 6:
        sTVShowState = 11;
        break;
    case 7:
        sTVShowState = 11;
        break;
    case 8:
        sTVShowState = 11;
        break;
    case 9:
        sTVShowState = 11;
        break;
    case 10:
        sTVShowState = 11;
        break;
    case 11:
        TVShowConvertInternationalString(gStringVar1, show->trainerFanClub.playerName, show->trainerFanClub.language);
        CopyEasyChatWord(gStringVar2, show->trainerFanClub.words[0]);
        CopyEasyChatWord(gStringVar3, show->trainerFanClub.words[1]);
        TVShowDone();
        break;
    }
    ShowFieldMessage(sTVTrainerFanClubTextGroup[state]);
}

static void DoTVShowPokemonNewsBattleFrontier(void)
{
    TVShow *show;
    u8 state;

    show = &gSaveBlock1Ptr->tvShows[gSpecialVar_0x8004];
    gSpecialVar_Result = FALSE;
    state = sTVShowState;
    switch (state)
    {
    case 0:
        switch (show->frontier.facilityAndMode)
        {
        case 1:
            sTVShowState = 1;
            break;
        case 2:
            sTVShowState = 2;
            break;
        case 3:
            sTVShowState = 3;
            break;
        case 4:
            sTVShowState = 4;
            break;
        case 5:
            sTVShowState = 5;
            break;
        case 6:
            sTVShowState = 6;
            break;
        case 7:
            sTVShowState = 7;
            break;
        case 8:
            sTVShowState = 8;
            break;
        case 9:
            sTVShowState = 9;
            break;
        case 10:
            sTVShowState = 10;
            break;
        case 11:
            sTVShowState = 11;
            break;
        case 12:
            sTVShowState = 12;
            break;
        case 13:
            sTVShowState = 13;
            break;
        }
        break;
    case 1:
        TVShowConvertInternationalString(gStringVar1, show->frontier.playerName, show->frontier.language);
        ConvertIntToDecimalString(1, show->frontier.winStreak);
        sTVShowState = 14;
        break;
    case 2:
        TVShowConvertInternationalString(gStringVar1, show->frontier.playerName, show->frontier.language);
        ConvertIntToDecimalString(1, show->frontier.winStreak);
        sTVShowState = 16;
        break;
    case 3:
        TVShowConvertInternationalString(gStringVar1, show->frontier.playerName, show->frontier.language);
        ConvertIntToDecimalString(1, show->frontier.winStreak);
        sTVShowState = 15;
        break;
    case 4:
        TVShowConvertInternationalString(gStringVar1, show->frontier.playerName, show->frontier.language);
        ConvertIntToDecimalString(1, show->frontier.winStreak);
        sTVShowState = 15;
        break;
    case 5:
        TVShowConvertInternationalString(gStringVar1, show->frontier.playerName, show->frontier.language);
        ConvertIntToDecimalString(1, show->frontier.winStreak);
        sTVShowState = 14;
        break;
    case 6:
        TVShowConvertInternationalString(gStringVar1, show->frontier.playerName, show->frontier.language);
        ConvertIntToDecimalString(1, show->frontier.winStreak);
        sTVShowState = 14;
        break;
    case 7:
        TVShowConvertInternationalString(gStringVar1, show->frontier.playerName, show->frontier.language);
        ConvertIntToDecimalString(1, show->frontier.winStreak);
        sTVShowState = 14;
        break;
    case 8:
        TVShowConvertInternationalString(gStringVar1, show->frontier.playerName, show->frontier.language);
        ConvertIntToDecimalString(1, show->frontier.winStreak);
        sTVShowState = 14;
        break;
    case 9:
        TVShowConvertInternationalString(gStringVar1, show->frontier.playerName, show->frontier.language);
        ConvertIntToDecimalString(1, show->frontier.winStreak);
        sTVShowState = 14;
        break;
    case 10:
        TVShowConvertInternationalString(gStringVar1, show->frontier.playerName, show->frontier.language);
        ConvertIntToDecimalString(1, show->frontier.winStreak);
        sTVShowState = 14;
        break;
    case 11:
        TVShowConvertInternationalString(gStringVar1, show->frontier.playerName, show->frontier.language);
        ConvertIntToDecimalString(1, show->frontier.winStreak);
        sTVShowState = 14;
        break;
    case 12:
        TVShowConvertInternationalString(gStringVar1, show->frontier.playerName, show->frontier.language);
        ConvertIntToDecimalString(1, show->frontier.winStreak);
        sTVShowState = 14;
        break;
    case 13:
        TVShowConvertInternationalString(gStringVar1, show->frontier.playerName, show->frontier.language);
        ConvertIntToDecimalString(1, show->frontier.winStreak);
        sTVShowState = 14;
        break;
    case 14:
        StringCopy(gStringVar1, gSpeciesNames[show->frontier.species1]);
        StringCopy(gStringVar2, gSpeciesNames[show->frontier.species2]);
        StringCopy(gStringVar3, gSpeciesNames[show->frontier.species3]);
        sTVShowState = 18;
        break;
    case 15:
        StringCopy(gStringVar1, gSpeciesNames[show->frontier.species1]);
        StringCopy(gStringVar2, gSpeciesNames[show->frontier.species2]);
        sTVShowState = 18;
        break;
    case 16:
        StringCopy(gStringVar1, gSpeciesNames[show->frontier.species1]);
        StringCopy(gStringVar2, gSpeciesNames[show->frontier.species2]);
        StringCopy(gStringVar3, gSpeciesNames[show->frontier.species3]);
        sTVShowState = 17;
        break;
    case 17:
        StringCopy(gStringVar1, gSpeciesNames[show->frontier.species4]);
        sTVShowState = 18;
        break;
    case 18:
        TVShowConvertInternationalString(gStringVar1, show->frontier.playerName, show->frontier.language);
        TVShowDone();
        break;
    }
    ShowFieldMessage(sTVPokemonNewsBattleFrontierTextGroup[state]);
}

static void DoTVShowWhatsNo1InHoennToday(void)
{
    TVShow *show;
    u8 state;

    show = &gSaveBlock1Ptr->tvShows[gSpecialVar_0x8004];
    gSpecialVar_Result = FALSE;
    state = sTVShowState;
    switch (state)
    {
    case 0:
        TVShowConvertInternationalString(gStringVar1, show->numberOne.playerName, show->numberOne.language);
        switch (show->numberOne.actionIdx)
        {
        case 0:
            sTVShowState = 1;
            break;
        case 1:
            sTVShowState = 2;
            break;
        case 2:
            sTVShowState = 3;
            break;
        case 3:
            sTVShowState = 4;
            break;
        case 4:
            sTVShowState = 5;
            break;
        case 5:
            sTVShowState = 6;
            break;
        case 6:
            sTVShowState = 7;
            break;
        }
        break;
    case 1:
        TVShowConvertInternationalString(gStringVar1, show->numberOne.playerName, show->numberOne.language);
        ConvertIntToDecimalString(1, show->numberOne.count);
        sTVShowState = 8;
        break;
    case 2:
        TVShowConvertInternationalString(gStringVar1, show->numberOne.playerName, show->numberOne.language);
        ConvertIntToDecimalString(1, show->numberOne.count);
        sTVShowState = 8;
        break;
    case 3:
        TVShowConvertInternationalString(gStringVar1, show->numberOne.playerName, show->numberOne.language);
        ConvertIntToDecimalString(1, show->numberOne.count);
        sTVShowState = 8;
        break;
    case 4:
        TVShowConvertInternationalString(gStringVar1, show->numberOne.playerName, show->numberOne.language);
        ConvertIntToDecimalString(1, show->numberOne.count);
        sTVShowState = 8;
        break;
    case 5:
        TVShowConvertInternationalString(gStringVar1, show->numberOne.playerName, show->numberOne.language);
        ConvertIntToDecimalString(1, show->numberOne.count);
        sTVShowState = 8;
        break;
    case 6:
        TVShowConvertInternationalString(gStringVar1, show->numberOne.playerName, show->numberOne.language);
        ConvertIntToDecimalString(1, show->numberOne.count);
        sTVShowState = 8;
        break;
    case 7:
        TVShowConvertInternationalString(gStringVar1, show->numberOne.playerName, show->numberOne.language);
        ConvertIntToDecimalString(1, show->numberOne.count);
        sTVShowState = 8;
        break;
    case 8:
        TVShowConvertInternationalString(gStringVar1, show->numberOne.playerName, show->numberOne.language);
        TVShowDone();
        break;
    }
    ShowFieldMessage(sTVWhatsNo1InHoennTodayTextGroup[state]);
}

static void DoTVShowSafariFanClub(void)
{
    TVShow *show;
    u8 state;

    show = &gSaveBlock1Ptr->tvShows[gSpecialVar_0x8004];
    gSpecialVar_Result = FALSE;
    state = sTVShowState;
    switch (state)
    {
    case 0:
        if (show->safariFanClub.nMonsCaught == 0)
            sTVShowState = 6;
        else if (show->safariFanClub.nMonsCaught < 4)
            sTVShowState = 5;
        else
            sTVShowState = 1;
        break;
    case 1:
        TVShowConvertInternationalString(gStringVar1, show->safariFanClub.playerName, show->safariFanClub.language);
        ConvertIntToDecimalString(1, show->safariFanClub.nMonsCaught);
        if (show->safariFanClub.nPkblkUsed == 0)
            sTVShowState = 3;
        else
            sTVShowState = 2;
        break;
    case 2:
        ConvertIntToDecimalString(1, show->safariFanClub.nPkblkUsed);
        sTVShowState = 4;
        break;
    case 3:
        sTVShowState = 4;
        break;
    case 4:
        TVShowConvertInternationalString(gStringVar1, show->safariFanClub.playerName, show->safariFanClub.language);
        sTVShowState = 10;
        break;
    case 5:
        TVShowConvertInternationalString(gStringVar1, show->safariFanClub.playerName, show->safariFanClub.language);
        ConvertIntToDecimalString(1, show->safariFanClub.nMonsCaught);
        if (show->safariFanClub.nPkblkUsed == 0)
            sTVShowState = 8;
        else
            sTVShowState = 7;
        break;
    case 6:
        TVShowConvertInternationalString(gStringVar1, show->safariFanClub.playerName, show->safariFanClub.language);
        if (show->safariFanClub.nPkblkUsed == 0)
            sTVShowState = 8;
        else
            sTVShowState = 7;
        break;
    case 7:
        ConvertIntToDecimalString(1, show->safariFanClub.nPkblkUsed);
        sTVShowState = 9;
        break;
    case 8:
        sTVShowState = 9;
        break;
    case 9:
        TVShowConvertInternationalString(gStringVar1, show->safariFanClub.playerName, show->safariFanClub.language);
        sTVShowState = 10;
        break;
    case 10:
        TVShowDone();
        break;
    }
    ShowFieldMessage(sTVSafariFanClubTextGroup[state]);
}

static void TVShowDone(void)
{
    gSpecialVar_Result = TRUE;
    sTVShowState = 0;
    gSaveBlock1Ptr->tvShows[gSpecialVar_0x8004].common.active = FALSE;
}

void ResetTVShowState(void)
{
    sTVShowState = 0;
}
