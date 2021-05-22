#ifndef GUARD_CONSTANTS_MAP_TYPES_H
#define GUARD_CONSTANTS_MAP_TYPES_H

#define MAP_TYPE_NONE        0
#define MAP_TYPE_TOWN        1
#define MAP_TYPE_CITY        2
#define MAP_TYPE_ROUTE       3
#define MAP_TYPE_UNDERGROUND 4
#define MAP_TYPE_UNDERWATER  5
#define MAP_TYPE_OCEAN_ROUTE 6
#define MAP_TYPE_UNKNOWN     7 // Not used by any map.
#define MAP_TYPE_INDOOR      8
#define MAP_TYPE_SECRET_BASE 9 // Not used by any map.

#define MAP_BATTLE_SCENE_NORMAL   0
#define MAP_BATTLE_SCENE_GYM      1
#define MAP_BATTLE_SCENE_INDOOR_1 2 // Pokemon Tower, Tanoby Ruins
#define MAP_BATTLE_SCENE_INDOOR_2 3 // Pok�mon Mansion, Power Plant, Rocket Hideout/Warehouse
#define MAP_BATTLE_SCENE_LORELEI  4
#define MAP_BATTLE_SCENE_BRUNO    5
#define MAP_BATTLE_SCENE_AGATHA   6
#define MAP_BATTLE_SCENE_LANCE    7
#define MAP_BATTLE_SCENE_LINK     8

#define MAP_BATTLE_SCENE_COUNT    MAP_BATTLE_SCENE_LINK

#endif  // GUARD_CONSTANTS_MAP_TYPES_H
