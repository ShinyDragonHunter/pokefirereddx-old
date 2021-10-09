#ifndef GUARD_CONSTANTS_TV_H
#define GUARD_CONSTANTS_TV_H

#define POKENEWS_NONE         0
#define POKENEWS_SLATEPORT    1
#define POKENEWS_GAME_CORNER  2
#define POKENEWS_LILYCOVE     3
#define POKENEWS_BLENDMASTER  4
#define NUM_POKENEWS_TYPES    4 // Excludes NONE

// TV shows are categorized as being in one of 3 groups
// - TVGROUP_NORMAL, TV shows that can appear without Record Mixing
// - TVGROUP_RECORD_MIX, TV shows that can only appear via Record Mixing
// - TVGROUP_OUTBREAK, just contains TVSHOW_MASS_OUTBREAK
// Each group was allotted 20 spaces arbitrarily, though none use all 20

#define TVSHOW_OFF_AIR                              0

// TVGROUP_NORMAL
#define TVGROUP_NORMAL_START                        1
#define TVSHOW_FAN_CLUB_LETTER                      1
#define TVSHOW_RECENT_HAPPENINGS                    2
#define TVSHOW_PKMN_FAN_CLUB_OPINIONS               3
#define TVSHOW_DUMMY                                4
#define TVSHOW_NAME_RATER_SHOW                      5
#define TVSHOW_BRAVO_TRAINER_BATTLE_TOWER_PROFILE   6
#define TVSHOW_BATTLE_UPDATE                        8
#define TVSHOW_FAN_CLUB_SPECIAL                     9
//
#define TVGROUP_NORMAL_END                          20

// TVGROUP_RECORD_MIX
#define TVGROUP_RECORD_MIX_START                    21
#define TVSHOW_POKEMON_TODAY_CAUGHT                 21
#define TVSHOW_SMART_SHOPPER                        22
#define TVSHOW_POKEMON_TODAY_FAILED                 23
#define TVSHOW_FISHING_ADVICE                       24
#define TVSHOW_WORLD_OF_MASTERS                     25
#define TVSHOW_TODAYS_RIVAL_TRAINER                 26
#define TVSHOW_TREND_WATCHER                        27
#define TVSHOW_TREASURE_INVESTIGATORS               28
#define TVSHOW_FIND_THAT_GAMER                      29
#define TVSHOW_BREAKING_NEWS                        30
#define TVSHOW_LOTTO_WINNER                         31
#define TVSHOW_BATTLE_SEMINAR                       32
#define TVSHOW_TRAINER_FAN_CLUB                     33
#define TVSHOW_FRONTIER                             34
#define TVSHOW_NUMBER_ONE                           35
#define TVSHOW_SAFARI_FAN_CLUB                      36
#define TVGROUP_RECORD_MIX_END                      37

// TVGROUP_OUTBREAK
#define TVGROUP_OUTBREAK_START                      39
#define TVSHOW_MASS_OUTBREAK                        39
//
#define TVGROUP_OUTBREAK_END                        60

// The first 5 elements of gSaveBlock1Ptr->tvShows are reserved
// for TV shows from TVGROUP_NORMAL. The remainder are for TV
// shows from TVGROUP_RECORD_MIX.
#define NUM_NORMAL_TVSHOW_SLOTS 5

#define PLAYERS_HOUSE_TV_NONE  0
#define PLAYERS_HOUSE_TV_LATI  1
#define PLAYERS_HOUSE_TV_MOVIE 2

// TV Show states for Spot the Cuties
#define SPOTCUTIES_STATE_INTRO            0
#define SPOTCUTIES_STATE_RIBBONS_LOW      1
#define SPOTCUTIES_STATE_RIBBONS_MID      2
#define SPOTCUTIES_STATE_RIBBONS_HIGH     3
#define SPOTCUTIES_STATE_RIBBON_INTRO     4
#define SPOTCUTIES_STATE_RIBBON_CHAMPION  5
#define SPOTCUTIES_STATE_RIBBON_COOL      6
#define SPOTCUTIES_STATE_RIBBON_BEAUTY    7
#define SPOTCUTIES_STATE_RIBBON_CUTE      8
#define SPOTCUTIES_STATE_RIBBON_SMART     9
#define SPOTCUTIES_STATE_RIBBON_TOUGH    10
#define SPOTCUTIES_STATE_RIBBON_WINNING  11
#define SPOTCUTIES_STATE_RIBBON_VICTORY  12
#define SPOTCUTIES_STATE_RIBBON_ARTIST   13
#define SPOTCUTIES_STATE_RIBBON_EFFORT   14
#define SPOTCUTIES_STATE_OUTRO           15

// TV Show states for Trend Watcher
#define TRENDWATCHER_STATE_INTRO           0
#define TRENDWATCHER_STATE_TAUGHT_MALE     1
#define TRENDWATCHER_STATE_TAUGHT_FEMALE   2
#define TRENDWATCHER_STATE_PHRASE_HOPELESS 3
#define TRENDWATCHER_STATE_BIGGER_MALE     4
#define TRENDWATCHER_STATE_BIGGER_FEMALE   5
#define TRENDWATCHER_STATE_OUTRO           6

// TV Show states for Smart Shopper
#define SMARTSHOPPER_STATE_INTRO           0
#define SMARTSHOPPER_STATE_CLERK_NORMAL    1
#define SMARTSHOPPER_STATE_RAND_COMMENT_1  2
#define SMARTSHOPPER_STATE_RAND_COMMENT_2  3
#define SMARTSHOPPER_STATE_RAND_COMMENT_3  4
#define SMARTSHOPPER_STATE_RAND_COMMENT_4  5
#define SMARTSHOPPER_STATE_SECOND_ITEM     6
#define SMARTSHOPPER_STATE_THIRD_ITEM      7
#define SMARTSHOPPER_STATE_DURING_SALE     8
#define SMARTSHOPPER_STATE_OUTRO_NORMAL    9
#define SMARTSHOPPER_STATE_IS_VIP         10
#define SMARTSHOPPER_STATE_CLERK_MAX      11
#define SMARTSHOPPER_STATE_OUTRO_MAX      12

#define SMARTSHOPPER_NUM_ITEMS 3

#endif //GUARD_CONSTANTS_TV_H
