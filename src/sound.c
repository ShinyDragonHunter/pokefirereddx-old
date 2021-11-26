#include "global.h"
#include "day_night.h"
#include "gba/m4a_internal.h"
#include "sound.h"
#include "battle.h"
#include "m4a.h"
#include "main.h"
#include "pokemon.h"
#include "task.h"
#include "constants/day_night.h"
#include "constants/songs.h"
#include "constants/species.h"

struct Fanfare
{
    u16 songNum;
    u16 duration;
};

// ewram
EWRAM_DATA struct MusicPlayerInfo* gMPlay_PokemonCry = NULL;
EWRAM_DATA u8 gPokemonCryBGMDuckingCounter = 0;

// iwram bss
static u16 sCurrentMapMusic;
static u16 sNextMapMusic;
static u8 sMapMusicState;
static u8 sMapMusicFadeInSpeed;
static u16 sFanfareCounter;

// iwram common
bool8 gDisableMusic;

extern struct MusicPlayerInfo gMPlayInfo_BGM;
extern struct MusicPlayerInfo gMPlayInfo_SE1;
extern struct MusicPlayerInfo gMPlayInfo_SE2;
extern struct MusicPlayerInfo gMPlayInfo_SE3;
extern struct ToneData gCryTable[];
extern struct ToneData gCryTable2[];

static void Task_Fanfare(u8 taskId);
static void CreateFanfareTask(void);
static void Task_DuckBGMForPokemonCry(u8 taskId);
static void RestoreBGMVolumeAfterPokemonCry(void);

#include "data/pokemon/cry_ids.h"

static const struct Fanfare sFanfares[] = {
    [FANFARE_LEVEL_UP]            = { MUS_LEVEL_UP,             80 },
    [FANFARE_OBTAIN_ITEM]         = { MUS_OBTAIN_ITEM,         160 },
    [FANFARE_EVOLVED]             = { MUS_EVOLVED,             220 },
    [FANFARE_OBTAIN_TMHM]         = { MUS_OBTAIN_TMHM,         220 },
    [FANFARE_HEAL]                = { MUS_HEAL,                160 },
    [FANFARE_OBTAIN_BADGE]        = { MUS_OBTAIN_BADGE,        340 },
    [FANFARE_MOVE_DELETED]        = { MUS_MOVE_DELETED,        180 },
    [FANFARE_OBTAIN_BERRY]        = { MUS_OBTAIN_BERRY,        120 },
    [FANFARE_AWAKEN_LEGEND]       = { MUS_AWAKEN_LEGEND,       710 },
    [FANFARE_SLOTS_JACKPOT]       = { MUS_SLOTS_JACKPOT,       250 },
    [FANFARE_SLOTS_WIN]           = { MUS_SLOTS_WIN,           150 },
    [FANFARE_TOO_BAD]             = { MUS_TOO_BAD,             160 },
    [FANFARE_RG_POKE_FLUTE]       = { MUS_RG_POKE_FLUTE,       450 },
    [FANFARE_RG_OBTAIN_KEY_ITEM]  = { MUS_RG_OBTAIN_KEY_ITEM,  170 },
    [FANFARE_RG_DEX_RATING]       = { MUS_RG_DEX_RATING,       196 },
    [FANFARE_OBTAIN_B_POINTS]     = { MUS_OBTAIN_B_POINTS,     313 },
    [FANFARE_OBTAIN_SYMBOL]       = { MUS_OBTAIN_SYMBOL,       318 },
    [FANFARE_REGISTER_MATCH_CALL] = { MUS_REGISTER_MATCH_CALL, 135 },
};

// sfx are skipped so we decrement the counter by the first valid song.
static const u16 sDayNightSongs[SONG_COUNT] =
{
    [MUS_LITTLEROOT_TEST - FIRST_SONG] = MUS_LITTLEROOT_TEST,
    [MUS_GSC_ROUTE38 - FIRST_SONG] = MUS_GSC_ROUTE38,
    [MUS_CAUGHT - FIRST_SONG] = MUS_CAUGHT,
    [MUS_VICTORY_WILD - FIRST_SONG] = MUS_VICTORY_WILD,
    [MUS_VICTORY_GYM_LEADER - FIRST_SONG] = MUS_VICTORY_GYM_LEADER,
    [MUS_VICTORY_LEAGUE - FIRST_SONG] = MUS_VICTORY_LEAGUE,
    [MUS_C_COMM_CENTER - FIRST_SONG] = MUS_C_COMM_CENTER,
    [MUS_GSC_PEWTER - FIRST_SONG] = MUS_GSC_PEWTER,
    [MUS_C_VS_LEGEND_BEAST - FIRST_SONG] = MUS_C_VS_LEGEND_BEAST,
    [MUS_ROUTE101 - FIRST_SONG] = MUS_ROUTE101,
    [MUS_ROUTE110 - FIRST_SONG] = MUS_ROUTE110,
    [MUS_ROUTE120 - FIRST_SONG] = MUS_ROUTE120,
    [MUS_PETALBURG - FIRST_SONG] = MUS_PETALBURG,
    [MUS_OLDALE - FIRST_SONG] = MUS_OLDALE,
    [MUS_GYM - FIRST_SONG] = MUS_GYM,
    [MUS_SURF - FIRST_SONG] = MUS_SURF,
    [MUS_PETALBURG_WOODS - FIRST_SONG] = MUS_PETALBURG_WOODS,
    [MUS_LEVEL_UP - FIRST_SONG] = MUS_LEVEL_UP,
    [MUS_HEAL - FIRST_SONG] = MUS_HEAL,
    [MUS_OBTAIN_BADGE - FIRST_SONG] = MUS_OBTAIN_BADGE,
    [MUS_OBTAIN_ITEM - FIRST_SONG] = MUS_OBTAIN_ITEM,
    [MUS_EVOLVED - FIRST_SONG] = MUS_EVOLVED,
    [MUS_OBTAIN_TMHM - FIRST_SONG] = MUS_OBTAIN_TMHM,
    [MUS_LILYCOVE_MUSEUM - FIRST_SONG] = MUS_LILYCOVE_MUSEUM,
    [MUS_ROUTE122 - FIRST_SONG] = MUS_ROUTE122,
    [MUS_OCEANIC_MUSEUM - FIRST_SONG] = MUS_OCEANIC_MUSEUM,
    [MUS_EVOLUTION_INTRO - FIRST_SONG] = MUS_EVOLUTION_INTRO,
    [MUS_EVOLUTION - FIRST_SONG] = MUS_EVOLUTION,
    [MUS_MOVE_DELETED - FIRST_SONG] = MUS_MOVE_DELETED,
    [MUS_ENCOUNTER_GIRL - FIRST_SONG] = MUS_ENCOUNTER_GIRL,
    [MUS_ENCOUNTER_MALE - FIRST_SONG] = MUS_ENCOUNTER_MALE,
    [MUS_ABANDONED_SHIP - FIRST_SONG] = MUS_ABANDONED_SHIP,
    [MUS_FORTREE - FIRST_SONG] = MUS_FORTREE,
    [MUS_BIRCH_LAB - FIRST_SONG] = MUS_BIRCH_LAB,
    [MUS_B_TOWER_RS - FIRST_SONG] = MUS_B_TOWER_RS,
    [MUS_ENCOUNTER_SWIMMER - FIRST_SONG] = MUS_ENCOUNTER_SWIMMER,
    [MUS_CAVE_OF_ORIGIN - FIRST_SONG] = MUS_CAVE_OF_ORIGIN,
    [MUS_OBTAIN_BERRY - FIRST_SONG] = MUS_OBTAIN_BERRY,
    [MUS_AWAKEN_LEGEND - FIRST_SONG] = MUS_AWAKEN_LEGEND,
    [MUS_SLOTS_JACKPOT - FIRST_SONG] = MUS_SLOTS_JACKPOT,
    [MUS_SLOTS_WIN - FIRST_SONG] = MUS_SLOTS_WIN,
    [MUS_TOO_BAD - FIRST_SONG] = MUS_TOO_BAD,
    [MUS_ROULETTE - FIRST_SONG] = MUS_ROULETTE,
    [MUS_LINK_CONTEST_P1 - FIRST_SONG] = MUS_LINK_CONTEST_P1,
    [MUS_LINK_CONTEST_P2 - FIRST_SONG] = MUS_LINK_CONTEST_P2,
    [MUS_LINK_CONTEST_P3 - FIRST_SONG] = MUS_LINK_CONTEST_P3,
    [MUS_LINK_CONTEST_P4 - FIRST_SONG] = MUS_LINK_CONTEST_P4,
    [MUS_ENCOUNTER_RICH - FIRST_SONG] = MUS_ENCOUNTER_RICH,
    [MUS_VERDANTURF - FIRST_SONG] = MUS_VERDANTURF,
    [MUS_RUSTBORO - FIRST_SONG] = MUS_RUSTBORO,
    [MUS_POKE_CENTER - FIRST_SONG] = MUS_POKE_CENTER,
    [MUS_ROUTE104 - FIRST_SONG] = MUS_ROUTE104,
    [MUS_ROUTE119 - FIRST_SONG] = MUS_ROUTE119,
    [MUS_CYCLING - FIRST_SONG] = MUS_CYCLING,
    [MUS_POKE_MART - FIRST_SONG] = MUS_POKE_MART,
    [MUS_LITTLEROOT - FIRST_SONG] = MUS_LITTLEROOT,
    [MUS_MT_CHIMNEY - FIRST_SONG] = MUS_MT_CHIMNEY,
    [MUS_ENCOUNTER_FEMALE - FIRST_SONG] = MUS_ENCOUNTER_FEMALE,
    [MUS_LILYCOVE - FIRST_SONG] = MUS_LILYCOVE,
    [MUS_ROUTE111 - FIRST_SONG] = MUS_ROUTE111,
    [MUS_HELP - FIRST_SONG] = MUS_HELP,
    [MUS_UNDERWATER - FIRST_SONG] = MUS_UNDERWATER,
    [MUS_VICTORY_TRAINER - FIRST_SONG] = MUS_VICTORY_TRAINER,
    [MUS_TITLE - FIRST_SONG] = MUS_TITLE,
    [MUS_INTRO - FIRST_SONG] = MUS_INTRO,
    [MUS_ENCOUNTER_MAY - FIRST_SONG] = MUS_ENCOUNTER_MAY,
    [MUS_ENCOUNTER_INTENSE - FIRST_SONG] = MUS_ENCOUNTER_INTENSE,
    [MUS_ENCOUNTER_COOL - FIRST_SONG] = MUS_ENCOUNTER_COOL,
    [MUS_ROUTE113 - FIRST_SONG] = MUS_ROUTE113,
    [MUS_ENCOUNTER_AQUA - FIRST_SONG] = MUS_ENCOUNTER_AQUA,
    [MUS_FOLLOW_ME - FIRST_SONG] = MUS_FOLLOW_ME,
    [MUS_ENCOUNTER_BRENDAN - FIRST_SONG] = MUS_ENCOUNTER_BRENDAN,
    [MUS_EVER_GRANDE - FIRST_SONG] = MUS_EVER_GRANDE,
    [MUS_ENCOUNTER_SUSPICIOUS - FIRST_SONG] = MUS_ENCOUNTER_SUSPICIOUS,
    [MUS_VICTORY_AQUA_MAGMA - FIRST_SONG] = MUS_VICTORY_AQUA_MAGMA,
    [MUS_CABLE_CAR - FIRST_SONG] = MUS_CABLE_CAR,
    [MUS_GAME_CORNER - FIRST_SONG] = MUS_GAME_CORNER,
    [MUS_DEWFORD - FIRST_SONG] = MUS_DEWFORD,
    [MUS_SAFARI_ZONE - FIRST_SONG] = MUS_SAFARI_ZONE,
    [MUS_VICTORY_ROAD - FIRST_SONG] = MUS_VICTORY_ROAD,
    [MUS_AQUA_MAGMA_HIDEOUT - FIRST_SONG] = MUS_AQUA_MAGMA_HIDEOUT,
    [MUS_SAILING - FIRST_SONG] = MUS_SAILING,
    [MUS_MT_PYRE - FIRST_SONG] = MUS_MT_PYRE,
    [MUS_SLATEPORT - FIRST_SONG] = MUS_SLATEPORT,
    [MUS_MT_PYRE_EXTERIOR - FIRST_SONG] = MUS_MT_PYRE_EXTERIOR,
    [MUS_SCHOOL - FIRST_SONG] = MUS_SCHOOL,
    [MUS_HALL_OF_FAME - FIRST_SONG] = MUS_HALL_OF_FAME,
    [MUS_FALLARBOR - FIRST_SONG] = MUS_FALLARBOR,
    [MUS_SEALED_CHAMBER - FIRST_SONG] = MUS_SEALED_CHAMBER,
    [MUS_CONTEST_WINNER - FIRST_SONG] = MUS_CONTEST_WINNER,
    [MUS_CONTEST - FIRST_SONG] = MUS_CONTEST,
    [MUS_ENCOUNTER_MAGMA - FIRST_SONG] = MUS_ENCOUNTER_MAGMA,
    [MUS_INTRO_BATTLE - FIRST_SONG] = MUS_INTRO_BATTLE,
    [MUS_ABNORMAL_WEATHER - FIRST_SONG] = MUS_ABNORMAL_WEATHER,
    [MUS_WEATHER_GROUDON - FIRST_SONG] = MUS_WEATHER_GROUDON,
    [MUS_SOOTOPOLIS - FIRST_SONG] = MUS_SOOTOPOLIS,
    [MUS_CONTEST_RESULTS - FIRST_SONG] = MUS_CONTEST_RESULTS,
    [MUS_HALL_OF_FAME_ROOM - FIRST_SONG] = MUS_HALL_OF_FAME_ROOM,
    [MUS_TRICK_HOUSE - FIRST_SONG] = MUS_TRICK_HOUSE,
    [MUS_ENCOUNTER_TWINS - FIRST_SONG] = MUS_ENCOUNTER_TWINS,
    [MUS_ENCOUNTER_ELITE_FOUR - FIRST_SONG] = MUS_ENCOUNTER_ELITE_FOUR,
    [MUS_ENCOUNTER_HIKER - FIRST_SONG] = MUS_ENCOUNTER_HIKER,
    [MUS_CONTEST_LOBBY - FIRST_SONG] = MUS_CONTEST_LOBBY,
    [MUS_ENCOUNTER_INTERVIEWER - FIRST_SONG] = MUS_ENCOUNTER_INTERVIEWER,
    [MUS_ENCOUNTER_CHAMPION - FIRST_SONG] = MUS_ENCOUNTER_CHAMPION,
    [MUS_CREDITS - FIRST_SONG] = MUS_CREDITS,
    [MUS_END - FIRST_SONG] = MUS_END,
    [MUS_B_FRONTIER - FIRST_SONG] = MUS_B_FRONTIER,
    [MUS_B_ARENA - FIRST_SONG] = MUS_B_ARENA,
    [MUS_OBTAIN_B_POINTS - FIRST_SONG] = MUS_OBTAIN_B_POINTS,
    [MUS_REGISTER_MATCH_CALL - FIRST_SONG] = MUS_REGISTER_MATCH_CALL,
    [MUS_B_PYRAMID - FIRST_SONG] = MUS_B_PYRAMID,
    [MUS_B_PYRAMID_TOP - FIRST_SONG] = MUS_B_PYRAMID_TOP,
    [MUS_B_PALACE - FIRST_SONG] = MUS_B_PALACE,
    [MUS_RAYQUAZA_APPEARS - FIRST_SONG] = MUS_RAYQUAZA_APPEARS,
    [MUS_B_TOWER - FIRST_SONG] = MUS_B_TOWER,
    [MUS_OBTAIN_SYMBOL - FIRST_SONG] = MUS_OBTAIN_SYMBOL,
    [MUS_B_DOME - FIRST_SONG] = MUS_B_DOME,
    [MUS_B_PIKE - FIRST_SONG] = MUS_B_PIKE,
    [MUS_B_FACTORY - FIRST_SONG] = MUS_B_FACTORY,
    [MUS_VS_RAYQUAZA - FIRST_SONG] = MUS_VS_RAYQUAZA,
    [MUS_VS_FRONTIER_BRAIN - FIRST_SONG] = MUS_VS_FRONTIER_BRAIN,
    [MUS_VS_MEW - FIRST_SONG] = MUS_VS_MEW,
    [MUS_B_DOME_LOBBY - FIRST_SONG] = MUS_B_DOME_LOBBY,
    [MUS_VS_WILD - FIRST_SONG] = MUS_VS_WILD,
    [MUS_VS_AQUA_MAGMA - FIRST_SONG] = MUS_VS_AQUA_MAGMA,
    [MUS_VS_TRAINER - FIRST_SONG] = MUS_VS_TRAINER,
    [MUS_VS_GYM_LEADER - FIRST_SONG] = MUS_VS_GYM_LEADER,
    [MUS_VS_CHAMPION - FIRST_SONG] = MUS_VS_CHAMPION,
    [MUS_VS_REGI - FIRST_SONG] = MUS_VS_REGI,
    [MUS_VS_KYOGRE_GROUDON - FIRST_SONG] = MUS_VS_KYOGRE_GROUDON,
    [MUS_VS_RIVAL - FIRST_SONG] = MUS_VS_RIVAL,
    [MUS_VS_ELITE_FOUR - FIRST_SONG] = MUS_VS_ELITE_FOUR,
    [MUS_VS_AQUA_MAGMA_LEADER - FIRST_SONG] = MUS_VS_AQUA_MAGMA_LEADER,
    // FRLG Music
    [MUS_RG_FOLLOW_ME - FIRST_SONG] = MUS_RG_FOLLOW_ME,
    [MUS_RG_GAME_CORNER - FIRST_SONG] = MUS_RG_GAME_CORNER,
    [MUS_RG_ROCKET_HIDEOUT - FIRST_SONG] = MUS_RG_ROCKET_HIDEOUT,
    [MUS_RG_GYM - FIRST_SONG] = MUS_RG_GYM,
    [MUS_RG_JIGGLYPUFF - FIRST_SONG] = MUS_RG_JIGGLYPUFF,
    [MUS_RG_INTRO_FIGHT - FIRST_SONG] = MUS_RG_INTRO_FIGHT,
    [MUS_RG_TITLE - FIRST_SONG] = MUS_RG_TITLE,
    [MUS_RG_CINNABAR - FIRST_SONG] = MUS_RG_CINNABAR,
    [MUS_RG_LAVENDER - FIRST_SONG] = MUS_RG_LAVENDER,
    [MUS_RG_HEAL - FIRST_SONG] = MUS_RG_HEAL,
    [MUS_RG_CYCLING - FIRST_SONG] = MUS_RG_CYCLING,
    [MUS_RG_ENCOUNTER_ROCKET - FIRST_SONG] = MUS_RG_ENCOUNTER_ROCKET,
    [MUS_RG_ENCOUNTER_GIRL - FIRST_SONG] = MUS_RG_ENCOUNTER_GIRL,
    [MUS_RG_ENCOUNTER_BOY - FIRST_SONG] = MUS_RG_ENCOUNTER_BOY,
    [MUS_RG_HALL_OF_FAME - FIRST_SONG] = MUS_RG_HALL_OF_FAME,
    [MUS_RG_VIRIDIAN_FOREST - FIRST_SONG] = MUS_RG_VIRIDIAN_FOREST,
    [MUS_RG_MT_MOON - FIRST_SONG] = MUS_RG_MT_MOON,
    [MUS_RG_POKE_MANSION - FIRST_SONG] = MUS_RG_POKE_MANSION,
    [MUS_RG_CREDITS - FIRST_SONG] = MUS_RG_CREDITS,
    [MUS_RG_ROUTE1 - FIRST_SONG] = MUS_RG_ROUTE1,
    [MUS_RG_ROUTE24 - FIRST_SONG] = MUS_RG_ROUTE24,
    [MUS_RG_ROUTE3 - FIRST_SONG] = MUS_RG_ROUTE3,
    [MUS_RG_ROUTE11 - FIRST_SONG] = MUS_RG_ROUTE11,
    [MUS_RG_VICTORY_ROAD - FIRST_SONG] = MUS_RG_VICTORY_ROAD,
    [MUS_RG_VS_GYM_LEADER - FIRST_SONG] = MUS_RG_VS_GYM_LEADER,
    [MUS_RG_VS_TRAINER - FIRST_SONG] = MUS_RG_VS_TRAINER,
    [MUS_RG_VS_WILD - FIRST_SONG] = MUS_RG_VS_WILD,
    [MUS_RG_VS_CHAMPION - FIRST_SONG] = MUS_RG_VS_CHAMPION,
    [MUS_RG_PALLET - FIRST_SONG] = MUS_RG_PALLET,
    [MUS_RG_OAK_LAB - FIRST_SONG] = MUS_RG_OAK_LAB,
    [MUS_RG_OAK - FIRST_SONG] = MUS_RG_OAK,
    [MUS_RG_POKE_CENTER - FIRST_SONG] = MUS_RG_POKE_CENTER,
    [MUS_RG_SS_ANNE - FIRST_SONG] = MUS_RG_SS_ANNE,
    [MUS_RG_SURF - FIRST_SONG] = MUS_RG_SURF,
    [MUS_RG_POKE_TOWER - FIRST_SONG] = MUS_RG_POKE_TOWER,
    [MUS_RG_SILPH - FIRST_SONG] = MUS_RG_SILPH,
    [MUS_RG_FUCHSIA - FIRST_SONG] = MUS_RG_FUCHSIA,
    [MUS_RG_CELADON - FIRST_SONG] = MUS_RG_CELADON,
    [MUS_RG_VICTORY_TRAINER - FIRST_SONG] = MUS_RG_VICTORY_TRAINER,
    [MUS_RG_VICTORY_WILD - FIRST_SONG] = MUS_RG_VICTORY_WILD,
    [MUS_RG_VICTORY_GYM_LEADER - FIRST_SONG] = MUS_RG_VICTORY_GYM_LEADER,
    [MUS_RG_VERMILLION - FIRST_SONG] = MUS_RG_VERMILLION,
    [MUS_RG_PEWTER - FIRST_SONG] = MUS_RG_PEWTER,
    [MUS_RG_ENCOUNTER_RIVAL - FIRST_SONG] = MUS_RG_ENCOUNTER_RIVAL,
    [MUS_RG_RIVAL_EXIT - FIRST_SONG] = MUS_RG_RIVAL_EXIT,
    [MUS_RG_DEX_RATING - FIRST_SONG] = MUS_RG_DEX_RATING,
    [MUS_RG_OBTAIN_KEY_ITEM - FIRST_SONG] = MUS_RG_OBTAIN_KEY_ITEM,
    [MUS_RG_CAUGHT_INTRO - FIRST_SONG] = MUS_RG_CAUGHT_INTRO,
    [MUS_RG_PHOTO - FIRST_SONG] = MUS_RG_PHOTO,
    [MUS_RG_GAME_FREAK - FIRST_SONG] = MUS_RG_GAME_FREAK,
    [MUS_RG_CAUGHT - FIRST_SONG] = MUS_RG_CAUGHT,
    [MUS_RG_NEW_GAME_INSTRUCT - FIRST_SONG] = MUS_RG_NEW_GAME_INSTRUCT,
    [MUS_RG_NEW_GAME_INTRO - FIRST_SONG] = MUS_RG_NEW_GAME_INTRO,
    [MUS_RG_NEW_GAME_EXIT - FIRST_SONG] = MUS_RG_NEW_GAME_EXIT,
    [MUS_RG_POKE_JUMP - FIRST_SONG] = MUS_RG_POKE_JUMP,
    [MUS_RG_UNION_ROOM - FIRST_SONG] = MUS_RG_UNION_ROOM,
    [MUS_RG_NET_CENTER - FIRST_SONG] = MUS_RG_NET_CENTER,
    [MUS_RG_MYSTERY_GIFT - FIRST_SONG] = MUS_RG_MYSTERY_GIFT,
    [MUS_RG_BERRY_PICK - FIRST_SONG] = MUS_RG_BERRY_PICK,
    [MUS_RG_SEVII_CAVE - FIRST_SONG] = MUS_RG_SEVII_CAVE,
    [MUS_RG_TEACHY_TV_SHOW - FIRST_SONG] = MUS_RG_TEACHY_TV_SHOW,
    [MUS_RG_SEVII_ROUTE - FIRST_SONG] = MUS_RG_SEVII_ROUTE,
    [MUS_RG_SEVII_DUNGEON - FIRST_SONG] = MUS_RG_SEVII_DUNGEON,
    [MUS_RG_SEVII_123 - FIRST_SONG] = MUS_RG_SEVII_123,
    [MUS_RG_SEVII_45 - FIRST_SONG] = MUS_RG_SEVII_45,
    [MUS_RG_SEVII_67 - FIRST_SONG] = MUS_RG_SEVII_67,
    [MUS_RG_POKE_FLUTE - FIRST_SONG] = MUS_RG_POKE_FLUTE,
    [MUS_RG_VS_DEOXYS - FIRST_SONG] = MUS_RG_VS_DEOXYS,
    [MUS_RG_VS_MEWTWO - FIRST_SONG] = MUS_RG_VS_MEWTWO,
    [MUS_RG_VS_LEGEND - FIRST_SONG] = MUS_RG_VS_LEGEND,
    [MUS_RG_ENCOUNTER_GYM_LEADER - FIRST_SONG] = MUS_RG_ENCOUNTER_GYM_LEADER,
    [MUS_RG_ENCOUNTER_DEOXYS - FIRST_SONG] = MUS_RG_ENCOUNTER_DEOXYS,
    [MUS_RG_TRAINER_TOWER - FIRST_SONG] = MUS_RG_TRAINER_TOWER,
    [MUS_RG_SLOW_PALLET - FIRST_SONG] = MUS_RG_SLOW_PALLET,
    [MUS_RG_TEACHY_TV_MENU - FIRST_SONG] = MUS_RG_TEACHY_TV_MENU
};

u16 GetDayNightSong(u16 songNum)
{
    if (GetCurrentTimeOfDay() == TIME_NIGHT)
        return sDayNightSongs[songNum - FIRST_SONG];
    return songNum;
}

#define CRY_VOLUME  120 // was 125 in R/S

void InitMapMusic(void)
{
    gDisableMusic = FALSE;
    ResetMapMusic();
}

void MapMusicMain(void)
{
    switch (sMapMusicState)
    {
    case 1:
        sMapMusicState = 2;
        PlayBGM(sCurrentMapMusic);
    case 0:
    case 2:
    case 3:
    case 4:
        break;
    case 5:
        if (IsBGMStopped())
        {
            sNextMapMusic = 0;
            sMapMusicState = 0;
        }
        break;
    case 6:
        if (IsBGMStopped() && IsFanfareTaskInactive())
        {
            sCurrentMapMusic = sNextMapMusic;
            sNextMapMusic = 0;
            sMapMusicState = 2;
            PlayBGM(sCurrentMapMusic);
        }
        break;
    case 7:
        if (IsBGMStopped() && IsFanfareTaskInactive())
        {
            FadeInNewBGM(sNextMapMusic, sMapMusicFadeInSpeed);
            sCurrentMapMusic = sNextMapMusic;
            sNextMapMusic = 0;
            sMapMusicState = 2;
            sMapMusicFadeInSpeed = 0;
        }
        break;
    }
}

void ResetMapMusic(void)
{
    sCurrentMapMusic = 0;
    sNextMapMusic = 0;
    sMapMusicState = 0;
    sMapMusicFadeInSpeed = 0;
}

u16 GetCurrentMapMusic(void)
{
    return GetDayNightSong(sCurrentMapMusic);
}

void PlayNewMapMusic(u16 songNum)
{
    sCurrentMapMusic = GetDayNightSong(songNum);
    sNextMapMusic = 0;
    sMapMusicState = 1;
}

void StopMapMusic(void)
{
    sCurrentMapMusic = 0;
    sNextMapMusic = 0;
    sMapMusicState = 1;
}

void FadeOutMapMusic(u8 speed)
{
    if (IsNotWaitingForBGMStop())
        FadeOutBGM(speed);
    sCurrentMapMusic = 0;
    sNextMapMusic = 0;
    sMapMusicState = 5;
}

void FadeOutAndPlayNewMapMusic(u16 songNum, u8 speed)
{
    FadeOutMapMusic(speed);
    sCurrentMapMusic = 0;
    sNextMapMusic = GetDayNightSong(songNum);
    sMapMusicState = 6;
}

void FadeOutAndFadeInNewMapMusic(u16 songNum, u8 fadeOutSpeed, u8 fadeInSpeed)
{
    FadeOutMapMusic(fadeOutSpeed);
    sCurrentMapMusic = 0;
    sNextMapMusic = GetDayNightSong(songNum);
    sMapMusicState = 7;
    sMapMusicFadeInSpeed = fadeInSpeed;
}

void FadeInNewMapMusic(u16 songNum, u8 speed)
{
    FadeInNewBGM(songNum, speed);
    sCurrentMapMusic = GetDayNightSong(songNum);
    sNextMapMusic = 0;
    sMapMusicState = 2;
    sMapMusicFadeInSpeed = 0;
}

bool8 IsNotWaitingForBGMStop(void)
{
    if (sMapMusicState == 5
     || sMapMusicState == 6
     || sMapMusicState == 7)
        return FALSE;
    return TRUE;
}

void PlayFanfareByFanfareNum(u8 fanfareNum)
{
    u16 songNum;
    m4aMPlayStop(&gMPlayInfo_BGM);
    songNum = sFanfares[fanfareNum].songNum;
    sFanfareCounter = sFanfares[fanfareNum].duration;
    m4aSongNumStart(GetDayNightSong(songNum));
}

bool8 WaitFanfare(bool8 stop)
{
    if (sFanfareCounter)
    {
        sFanfareCounter--;
        return FALSE;
    }
    else
    {
        if (stop)
            m4aSongNumStart(MUS_DUMMY);
        else
            m4aMPlayContinue(&gMPlayInfo_BGM);

        return TRUE;
    }
}

void PlayFanfare(u16 songNum)
{
    s32 i;
    for (i = 0; (u32)i < ARRAY_COUNT(sFanfares); i++)
    {
        if (sFanfares[i].songNum == songNum)
        {
            PlayFanfareByFanfareNum(i);
            CreateFanfareTask();
            return;
        }
    }

    // songNum is not in sFanfares
    // Play first fanfare in table instead
    PlayFanfareByFanfareNum(0);
    CreateFanfareTask();
}

bool8 IsFanfareTaskInactive(void)
{
    if (FuncIsActiveTask(Task_Fanfare))
        return FALSE;
    return TRUE;
}

static void Task_Fanfare(u8 taskId)
{
    if (sFanfareCounter)
    {
        sFanfareCounter--;
    }
    else
    {
        m4aMPlayContinue(&gMPlayInfo_BGM);
        DestroyTask(taskId);
    }
}

static void CreateFanfareTask(void)
{
    if (!FuncIsActiveTask(Task_Fanfare))
        CreateTask(Task_Fanfare, 80);
}

void FadeInNewBGM(u16 songNum, u8 speed)
{
    if (gDisableMusic
     || songNum == MUS_NONE)
        songNum = MUS_DUMMY;
    m4aSongNumStart(GetDayNightSong(songNum));
    m4aMPlayImmInit(&gMPlayInfo_BGM);
    m4aMPlayVolumeControl(&gMPlayInfo_BGM, 0xFFFF, 0);
    m4aSongNumStop(GetDayNightSong(songNum));
    m4aMPlayFadeIn(&gMPlayInfo_BGM, speed);
}

void FadeOutBGMTemporarily(u8 speed)
{
    m4aMPlayFadeOutTemporarily(&gMPlayInfo_BGM, speed);
}

bool8 IsBGMPausedOrStopped(void)
{
    if (gMPlayInfo_BGM.status & MUSICPLAYER_STATUS_PAUSE
     || !(gMPlayInfo_BGM.status & MUSICPLAYER_STATUS_TRACK))
        return TRUE;
    return FALSE;
}

void FadeInBGM(u8 speed)
{
    m4aMPlayFadeIn(&gMPlayInfo_BGM, speed);
}

void FadeOutBGM(u8 speed)
{
    m4aMPlayFadeOut(&gMPlayInfo_BGM, speed);
}

bool8 IsBGMStopped(void)
{
    if (!(gMPlayInfo_BGM.status & MUSICPLAYER_STATUS_TRACK))
        return TRUE;
    return FALSE;
}

void PlayCry1(u16 species, s8 pan)
{
    m4aMPlayVolumeControl(&gMPlayInfo_BGM, 0xFFFF, 85);
    PlayCryInternal(species, pan, CRY_VOLUME, 10, 0);
    gPokemonCryBGMDuckingCounter = 2;
    RestoreBGMVolumeAfterPokemonCry();
}

void PlayCry2(u16 species, s8 pan, s8 volume, u8 priority)
{
    PlayCryInternal(species, pan, volume, priority, 0);
}

void PlayCry3(u16 species, s8 pan, u8 mode)
{
    if (mode == 1)
    {
        PlayCryInternal(species, pan, CRY_VOLUME, 10, 1);
    }
    else
    {
        m4aMPlayVolumeControl(&gMPlayInfo_BGM, 0xFFFF, 85);
        PlayCryInternal(species, pan, CRY_VOLUME, 10, mode);
        gPokemonCryBGMDuckingCounter = 2;
        RestoreBGMVolumeAfterPokemonCry();
    }
}

void PlayCry4(u16 species, s8 pan, u8 mode)
{
    if (mode == 1)
    {
        PlayCryInternal(species, pan, CRY_VOLUME, 10, 1);
    }
    else
    {
        if (!(gBattleTypeFlags & BATTLE_TYPE_MULTI))
            m4aMPlayVolumeControl(&gMPlayInfo_BGM, 0xFFFF, 85);
        PlayCryInternal(species, pan, CRY_VOLUME, 10, mode);
    }
}

void PlayCry5(u16 species, u8 mode)
{
    m4aMPlayVolumeControl(&gMPlayInfo_BGM, 0xFFFF, 85);
    PlayCryInternal(species, 0, CRY_VOLUME, 10, mode);
    gPokemonCryBGMDuckingCounter = 2;
    RestoreBGMVolumeAfterPokemonCry();
}

void PlayCry6(u16 species, s8 pan, u8 mode) // not present in R/S
{
    if (mode == 1)
    {
        PlayCryInternal(species, pan, CRY_VOLUME, 10, 1);
    }
    else
    {
        m4aMPlayVolumeControl(&gMPlayInfo_BGM, 0xFFFF, 85);
        PlayCryInternal(species, pan, CRY_VOLUME, 10, mode);
        gPokemonCryBGMDuckingCounter = 2;
    }
}

void PlayCryInternal(u16 species, s8 pan, s8 volume, u8 priority, u8 mode)
{
    bool32 v0;
    u32 release, length, pitch, chorus, index;
    u8 table;

    length = 140;
    v0 = FALSE;
    release = 0;
    pitch = 15360;
    chorus = 0;

    switch (mode)
    {
    case 1:
        length = 20;
        release = 225;
    case 0:
        break;
    case 2:
        release = 225;
        pitch = 15600;
        chorus = 20;
        volume = 90;
        break;
    case 3:
        length = 50;
        release = 200;
        pitch = 15800;
        chorus = 20;
        volume = 90;
        break;
    case 4:
        length = 25;
        v0 = TRUE;
        release = 100;
        pitch = 15600;
        chorus = 192;
        volume = 90;
        break;
    case 5:
        release = 200;
        pitch = 14440;
        break;
    case 6:
        release = 220;
        pitch = 15555;
        chorus = 192;
        volume = 90;
        break;
    case 7:
        length = 10;
        release = 100;
        pitch = 14848;
        break;
    case 8:
        length = 60;
        release = 225;
        pitch = 15616;
        break;
    case 9:
        length = 15;
        v0 = TRUE;
        release = 125;
        pitch = 15200;
        break;
    case 10:
        length = 100;
        release = 225;
        pitch = 15200;
        break;
    case 12:
        length = 20;
        release = 225;
    case 11:
        pitch = 15000;
        break;
    }

    SetPokemonCryVolume(volume);
    SetPokemonCryPanpot(pan);
    SetPokemonCryPitch(pitch);
    SetPokemonCryLength(length);
    SetPokemonCryProgress(0);
    SetPokemonCryRelease(release);
    SetPokemonCryChorus(chorus);
    SetPokemonCryPriority(priority);

    species = sSpeciesIdToCryId[species];
    gMPlay_PokemonCry = SetPokemonCryTone(v0 ? &gCryTable2[species] : &gCryTable[species]);
}

bool8 IsCryFinished(void)
{
    if (FuncIsActiveTask(Task_DuckBGMForPokemonCry))
    {
        return FALSE;
    }
    else
    {
        ClearPokemonCrySongs();
        return TRUE;
    }
}

void StopCryAndClearCrySongs(void)
{
    m4aMPlayStop(gMPlay_PokemonCry);
    ClearPokemonCrySongs();
}

void StopCry(void)
{
    m4aMPlayStop(gMPlay_PokemonCry);
}

bool8 IsCryPlayingOrClearCrySongs(void)
{
    if (IsPokemonCryPlaying(gMPlay_PokemonCry))
    {
        return TRUE;
    }
    else
    {
        ClearPokemonCrySongs();
        return FALSE;
    }
}

bool8 IsCryPlaying(void)
{
    return IsPokemonCryPlaying(gMPlay_PokemonCry);
}

static void Task_DuckBGMForPokemonCry(u8 taskId)
{
    if (gPokemonCryBGMDuckingCounter)
    {
        gPokemonCryBGMDuckingCounter--;
        return;
    }

    if (!IsPokemonCryPlaying(gMPlay_PokemonCry))
    {
        m4aMPlayVolumeControl(&gMPlayInfo_BGM, 0xFFFF, 256);
        DestroyTask(taskId);
    }
}

static void RestoreBGMVolumeAfterPokemonCry(void)
{
    if (!FuncIsActiveTask(Task_DuckBGMForPokemonCry))
        CreateTask(Task_DuckBGMForPokemonCry, 80);
}

void PlayBGM(u16 songNum)
{
    if (gDisableMusic
     || songNum == MUS_NONE)
        songNum = MUS_DUMMY;
    m4aSongNumStart(songNum);
}

void PlaySE(u16 songNum)
{
    m4aSongNumStart(songNum);
}

void PlaySE12WithPanning(u16 songNum, s8 pan)
{
    m4aSongNumStart(songNum);
    m4aMPlayImmInit(&gMPlayInfo_SE1);
    m4aMPlayImmInit(&gMPlayInfo_SE2);
    m4aMPlayPanpotControl(&gMPlayInfo_SE1, 0xFFFF, pan);
    m4aMPlayPanpotControl(&gMPlayInfo_SE2, 0xFFFF, pan);
}

void PlaySE1WithPanning(u16 songNum, s8 pan)
{
    m4aSongNumStart(songNum);
    m4aMPlayImmInit(&gMPlayInfo_SE1);
    m4aMPlayPanpotControl(&gMPlayInfo_SE1, 0xFFFF, pan);
}

void PlaySE2WithPanning(u16 songNum, s8 pan)
{
    m4aSongNumStart(songNum);
    m4aMPlayImmInit(&gMPlayInfo_SE2);
    m4aMPlayPanpotControl(&gMPlayInfo_SE2, 0xFFFF, pan);
}

void SE12PanpotControl(s8 pan)
{
    m4aMPlayPanpotControl(&gMPlayInfo_SE1, 0xFFFF, pan);
    m4aMPlayPanpotControl(&gMPlayInfo_SE2, 0xFFFF, pan);
}

bool8 IsSEPlaying(void)
{
    if (((gMPlayInfo_SE1.status & MUSICPLAYER_STATUS_PAUSE) && (gMPlayInfo_SE2.status & MUSICPLAYER_STATUS_PAUSE))
     || (!(gMPlayInfo_SE1.status & MUSICPLAYER_STATUS_TRACK) && !(gMPlayInfo_SE2.status & MUSICPLAYER_STATUS_TRACK)))
        return FALSE;
    return TRUE;
}

bool8 IsBGMPlaying(void)
{
    if (gMPlayInfo_BGM.status & MUSICPLAYER_STATUS_PAUSE
     || !(gMPlayInfo_BGM.status & MUSICPLAYER_STATUS_TRACK))
        return FALSE;
    return TRUE;
}

bool8 IsSpecialSEPlaying(void)
{
    if (gMPlayInfo_SE3.status & MUSICPLAYER_STATUS_PAUSE
     || !(gMPlayInfo_SE3.status & MUSICPLAYER_STATUS_TRACK))
        return FALSE;
    return TRUE;
}
