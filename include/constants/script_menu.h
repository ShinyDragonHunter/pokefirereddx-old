#ifndef GUARD_SCRIPT_MENU_CONSTANTS_H
#define GUARD_SCRIPT_MENU_CONSTANTS_H

#define MULTICHOICE(name) {name, ARRAY_COUNT(name)}

#define MAX_MULTICHOICE_WIDTH 28

#define MULTI_B_PRESSED  127

// Multichoice Ids
#define MULTI_BRINEY_ON_DEWFORD            0
#define MULTI_PC                           1 // Exit only, populated by CreatePCMultichoice
#define MULTI_ENTERINFO                    2
#define MULTI_BASE_PC_NO_REGISTRY          3
#define MULTI_BASE_PC_WITH_REGISTRY        4
#define MULTI_REGISTER_MENU                5
#define MULTI_SSTIDAL_LILYCOVE             6 // Exit only, populated by CreateLilycoveSSTidalMultichoice
#define MULTI_UNUSED_9                     7
#define MULTI_UNUSED_10                    8
#define MULTI_FRONTIER_PASS_INFO           9
#define MULTI_BIKE                         10
#define MULTI_STATUS_INFO                  11
#define MULTI_BRINEY_OFF_DEWFORD           12
#define MULTI_UNUSED_15                    13
#define MULTI_VIEWED_PAINTINGS             14
#define MULTI_YESNOINFO                    15
#define MULTI_BATTLE_MODE                  16
#define MULTI_UNUSED_19                    17
#define MULTI_YESNOINFO_2                  18
#define MULTI_UNUSED_21                    19
#define MULTI_UNUSED_22                    20
#define MULTI_CHALLENGEINFO                21
#define MULTI_LEVEL_MODE                   22
#define MULTI_MECHADOLL1_Q1                23
#define MULTI_MECHADOLL1_Q2                24
#define MULTI_MECHADOLL1_Q3                25
#define MULTI_MECHADOLL2_Q1                26
#define MULTI_MECHADOLL2_Q2                27
#define MULTI_MECHADOLL2_Q3                28
#define MULTI_MECHADOLL3_Q1                29
#define MULTI_MECHADOLL3_Q2                30
#define MULTI_MECHADOLL3_Q3                31
#define MULTI_MECHADOLL4_Q1                32
#define MULTI_MECHADOLL4_Q2                33
#define MULTI_MECHADOLL4_Q3                34
#define MULTI_MECHADOLL5_Q1                35
#define MULTI_MECHADOLL5_Q2                36
#define MULTI_MECHADOLL5_Q3                37
#define MULTI_UNUSED_40                    38
#define MULTI_UNUSED_41                    39
#define MULTI_VENDING_MACHINE              40
#define MULTI_MACH_BIKE_INFO               41
#define MULTI_ACRO_BIKE_INFO               42
#define MULTI_SATISFACTION                 43
#define MULTI_STERN_DEEPSEA                44
#define MULTI_UNUSED_ASH_VENDOR            45 // Replaced by scrollable multichoice
#define MULTI_GAME_CORNER_DOLLS            46
#define MULTI_GAME_CORNER_COINS            47
#define MULTI_HOWS_FISHING                 48
#define MULTI_UNUSED_51                    49
#define MULTI_SSTIDAL_SLATEPORT_WITH_BF    50
#define MULTI_SSTIDAL_BATTLE_FRONTIER      51
#define MULTI_RIGHTLEFT                    52
#define MULTI_GAME_CORNER_TMS              53
#define MULTI_SSTIDAL_SLATEPORT_NO_BF      54
#define MULTI_FLOORS                       55 
#define MULTI_SHARDS_R                     56
#define MULTI_SHARDS_Y                     57
#define MULTI_SHARDS_RY                    58
#define MULTI_SHARDS_B                     59
#define MULTI_SHARDS_RB                    60
#define MULTI_SHARDS_YB                    61
#define MULTI_SHARDS_RYB                   62
#define MULTI_SHARDS_G                     63
#define MULTI_SHARDS_RG                    64
#define MULTI_SHARDS_YG                    65
#define MULTI_SHARDS_RYG                   66
#define MULTI_SHARDS_BG                    67
#define MULTI_SHARDS_RBG                   68
#define MULTI_SHARDS_YBG                   69
#define MULTI_SHARDS_RYBG                  70
#define MULTI_TOURNEY_WITH_RECORD          71
#define MULTI_CABLE_CLUB_NO_RECORD_MIX     72 
#define MULTI_WIRELESS_NO_RECORD_BERRY     73
#define MULTI_CABLE_CLUB_WITH_RECORD_MIX   74 
#define MULTI_WIRELESS_NO_BERRY            75
#define MULTI_WIRELESS_NO_RECORD           76
#define MULTI_WIRELESS_ALL_SERVICES        77
#define MULTI_WIRELESS_MINIGAME            78
#define MULTI_LINK_LEADER                  79
#define MULTI_FRONTIER_ITEM_CHOOSE         80
#define MULTI_FORCED_START_MENU            81
#define MULTI_FRONTIER_GAMBLER_BET         82
#define MULTI_TENT                         83
#define MULTI_UNUSED_SSTIDAL_1             84 // These 4 were replaced by CreateLilycoveSSTidalMultichoice
#define MULTI_UNUSED_SSTIDAL_2             85 //
#define MULTI_UNUSED_SSTIDAL_3             86 //
#define MULTI_UNUSED_SSTIDAL_4             87 //
#define MULTI_FOSSIL                       88
#define MULTI_YESNO                        89
#define MULTI_FRONTIER_RULES               90
#define MULTI_BATTLE_ARENA_RULES           91
#define MULTI_BATTLE_TOWER_RULES           92
#define MULTI_BATTLE_DOME_RULES            93
#define MULTI_BATTLE_FACTORY_RULES         94
#define MULTI_BATTLE_PALACE_RULES          95
#define MULTI_BATTLE_PYRAMID_RULES         96
#define MULTI_BATTLE_PIKE_RULES            97
#define MULTI_GO_ON_RECORD_REST_RETIRE     98
#define MULTI_GO_ON_REST_RETIRE            99
#define MULTI_GO_ON_RECORD_RETIRE          100
#define MULTI_GO_ON_RETIRE                 101
#define MULTI_TOURNEY_NO_RECORD            102
#define MULTI_TV_LATI                      103
#define MULTI_BATTLE_TOWER_FEELINGS        104
#define MULTI_WHERES_RAYQUAZA              105
#define MULTI_SLATEPORT_TENT_RULES         106
#define MULTI_FALLARBOR_TENT_RULES         107
#define MULTI_TAG_MATCH_TYPE               108

// Lilycove SS Tidal Multichoice Selections
#define SSTIDAL_SELECTION_SLATEPORT        0
#define SSTIDAL_SELECTION_BATTLE_FRONTIER  1
#define SSTIDAL_SELECTION_SOUTHERN_ISLAND  2
#define SSTIDAL_SELECTION_NAVEL_ROCK       3
#define SSTIDAL_SELECTION_BIRTH_ISLAND     4
#define SSTIDAL_SELECTION_FARAWAY_ISLAND   5
#define SSTIDAL_SELECTION_EXIT             6
#define SSTIDAL_SELECTION_COUNT            7

// Std String Ids
#define STDSTRING_ITEMS            0
#define STDSTRING_KEYITEMS         1
#define STDSTRING_POKEBALLS        2
#define STDSTRING_TMHMS            3
#define STDSTRING_BERRIES          4
#define STDSTRING_SINGLE           5
#define STDSTRING_DOUBLE           6
#define STDSTRING_MULTI            7
#define STDSTRING_MULTI_LINK       8
#define STDSTRING_BATTLE_TOWER     9
#define STDSTRING_BATTLE_DOME      10
#define STDSTRING_BATTLE_FACTORY   11
#define STDSTRING_BATTLE_PALACE    12
#define STDSTRING_BATTLE_ARENA     13
#define STDSTRING_BATTLE_PIKE      14
#define STDSTRING_BATTLE_PYRAMID   15

#endif //GUARD_SCRIPT_MENU_CONSTANTS_H
