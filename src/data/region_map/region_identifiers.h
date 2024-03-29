#ifndef GUARD_DATA_REGION_IDENTIFIERS_H
#define GUARD_DATA_REGION_IDENTIFIERS_H

const u8 gMapsecToRegion[METLOC_FATEFUL_ENCOUNTER + 1] =
{
    [MAPSEC_PALLET_TOWN] = REGION_KANTO,
    [MAPSEC_VIRIDIAN_CITY] = REGION_KANTO,
    [MAPSEC_PEWTER_CITY] = REGION_KANTO,
    [MAPSEC_CERULEAN_CITY] = REGION_KANTO,
    [MAPSEC_LAVENDER_TOWN] = REGION_KANTO,
    [MAPSEC_VERMILION_CITY] = REGION_KANTO,
    [MAPSEC_CELADON_CITY] = REGION_KANTO,
    [MAPSEC_FUCHSIA_CITY] = REGION_KANTO,
    [MAPSEC_CINNABAR_ISLAND] = REGION_KANTO,
    [MAPSEC_INDIGO_PLATEAU] = REGION_KANTO,
    [MAPSEC_SAFFRON_CITY] = REGION_KANTO,
    [MAPSEC_ROUTE_4_FLYDUP]  = REGION_KANTO,
    [MAPSEC_ROUTE_10_FLYDUP] = REGION_KANTO,
    [MAPSEC_ROUTE_1] = REGION_KANTO,
    [MAPSEC_ROUTE_2] = REGION_KANTO,
    [MAPSEC_ROUTE_3] = REGION_KANTO,
    [MAPSEC_ROUTE_4] = REGION_KANTO,
    [MAPSEC_ROUTE_5] = REGION_KANTO,
    [MAPSEC_ROUTE_6] = REGION_KANTO,
    [MAPSEC_ROUTE_7] = REGION_KANTO,
    [MAPSEC_ROUTE_8] = REGION_KANTO,
    [MAPSEC_ROUTE_9] = REGION_KANTO,
    [MAPSEC_ROUTE_10] = REGION_KANTO,
    [MAPSEC_ROUTE_11] = REGION_KANTO,
    [MAPSEC_ROUTE_12] = REGION_KANTO,
    [MAPSEC_ROUTE_13] = REGION_KANTO,
    [MAPSEC_ROUTE_14] = REGION_KANTO,
    [MAPSEC_ROUTE_15] = REGION_KANTO,
    [MAPSEC_ROUTE_16] = REGION_KANTO,
    [MAPSEC_ROUTE_17] = REGION_KANTO,
    [MAPSEC_ROUTE_18] = REGION_KANTO,
    [MAPSEC_ROUTE_19] = REGION_KANTO,
    [MAPSEC_ROUTE_20] = REGION_KANTO,
    [MAPSEC_ROUTE_21] = REGION_KANTO,
    [MAPSEC_ROUTE_22] = REGION_KANTO,
    [MAPSEC_ROUTE_23] = REGION_KANTO,
    [MAPSEC_ROUTE_24] = REGION_KANTO,
    [MAPSEC_ROUTE_25] = REGION_KANTO,
    [MAPSEC_VIRIDIAN_FOREST] = REGION_KANTO,
    [MAPSEC_MT_MOON] = REGION_KANTO,
    [MAPSEC_S_S_ANNE] = REGION_KANTO,
    [MAPSEC_UNDERGROUND_PATH] = REGION_KANTO,
    [MAPSEC_UNDERGROUND_PATH_2] = REGION_KANTO,
    [MAPSEC_DIGLETTS_CAVE] = REGION_KANTO,
    [MAPSEC_KANTO_VICTORY_ROAD] = REGION_KANTO,
    [MAPSEC_ROCKET_HIDEOUT] = REGION_KANTO,
    [MAPSEC_SILPH_CO] = REGION_KANTO,
    [MAPSEC_POKEMON_MANSION] = REGION_KANTO,
    [MAPSEC_KANTO_SAFARI_ZONE] = REGION_KANTO,
    [MAPSEC_POKEMON_LEAGUE] = REGION_KANTO,
    [MAPSEC_ROCK_TUNNEL] = REGION_KANTO,
    [MAPSEC_SEAFOAM_ISLANDS] = REGION_KANTO,
    [MAPSEC_POKEMON_TOWER] = REGION_KANTO,
    [MAPSEC_CERULEAN_CAVE] = REGION_KANTO,
    [MAPSEC_POWER_PLANT] = REGION_KANTO,
    [MAPSEC_ONE_ISLAND] = REGION_SEVII,
    [MAPSEC_TWO_ISLAND] = REGION_SEVII,
    [MAPSEC_THREE_ISLAND] = REGION_SEVII,
    [MAPSEC_FOUR_ISLAND] = REGION_SEVII,
    [MAPSEC_FIVE_ISLAND] = REGION_SEVII,
    [MAPSEC_SEVEN_ISLAND] = REGION_SEVII,
    [MAPSEC_SIX_ISLAND] = REGION_SEVII,
    [MAPSEC_KINDLE_ROAD] = REGION_SEVII,
    [MAPSEC_TREASURE_BEACH] = REGION_SEVII,
    [MAPSEC_CAPE_BRINK] = REGION_SEVII,
    [MAPSEC_BOND_BRIDGE] = REGION_SEVII,
    [MAPSEC_THREE_ISLE_PORT] = REGION_SEVII,
    [MAPSEC_SEVII_ISLE_6] = REGION_SEVII,
    [MAPSEC_SEVII_ISLE_7] = REGION_SEVII,
    [MAPSEC_SEVII_ISLE_8] = REGION_SEVII,
    [MAPSEC_SEVII_ISLE_9] = REGION_SEVII,
    [MAPSEC_RESORT_GORGEOUS] = REGION_SEVII,
    [MAPSEC_WATER_LABYRINTH] = REGION_SEVII,
    [MAPSEC_FIVE_ISLE_MEADOW] = REGION_SEVII,
    [MAPSEC_MEMORIAL_PILLAR] = REGION_SEVII,
    [MAPSEC_OUTCAST_ISLAND] = REGION_SEVII,
    [MAPSEC_GREEN_PATH] = REGION_SEVII,
    [MAPSEC_WATER_PATH] = REGION_SEVII,
    [MAPSEC_RUIN_VALLEY] = REGION_SEVII,
    [MAPSEC_TRAINER_TOWER] = REGION_SEVII,
    [MAPSEC_CANYON_ENTRANCE] = REGION_SEVII,
    [MAPSEC_SEVAULT_CANYON] = REGION_SEVII,
    [MAPSEC_TANOBY_RUINS] = REGION_SEVII,
    [MAPSEC_SEVII_ISLE_22] = REGION_SEVII,
    [MAPSEC_SEVII_ISLE_23] = REGION_SEVII,
    [MAPSEC_SEVII_ISLE_24] = REGION_SEVII,
    [MAPSEC_NAVEL_ROCK_FRLG] = REGION_SEVII,
    [MAPSEC_MT_EMBER] = REGION_SEVII,
    [MAPSEC_BERRY_FOREST] = REGION_SEVII,
    [MAPSEC_ICEFALL_CAVE] = REGION_SEVII,
    [MAPSEC_ROCKET_WAREHOUSE] = REGION_SEVII,
    [MAPSEC_TRAINER_TOWER_2] = REGION_SEVII,
    [MAPSEC_DOTTED_HOLE] = REGION_SEVII,
    [MAPSEC_LOST_CAVE] = REGION_SEVII,
    [MAPSEC_PATTERN_BUSH] = REGION_SEVII,
    [MAPSEC_ALTERING_CAVE_FRLG] = REGION_SEVII,
    [MAPSEC_TANOBY_CHAMBERS] = REGION_SEVII,
    [MAPSEC_THREE_ISLE_PATH] = REGION_SEVII,
    [MAPSEC_TANOBY_KEY] = REGION_SEVII,
    [MAPSEC_BIRTH_ISLAND_FRLG] = REGION_SEVII,
    [MAPSEC_MONEAN_CHAMBER] = REGION_SEVII,
    [MAPSEC_LIPTOO_CHAMBER] = REGION_SEVII,
    [MAPSEC_WEEPTH_CHAMBER] = REGION_SEVII,
    [MAPSEC_DILFORD_CHAMBER] = REGION_SEVII,
    [MAPSEC_SCUFIB_CHAMBER] = REGION_SEVII,
    [MAPSEC_RIXY_CHAMBER] = REGION_SEVII,
    [MAPSEC_VIAPOIS_CHAMBER] = REGION_SEVII,
    [MAPSEC_EMBER_SPA] = REGION_SEVII,
    [MAPSEC_SPECIAL_AREA] = REGION_KANTO
};

#endif //GUARD_DATA_REGION_IDENTIFIERS_H
