#ifndef GUARD_BATTLE_BG_H
#define GUARD_BATTLE_BG_H

extern const struct BattleBackground gBattleTerrainTable[];

extern const u32 gBattleTerrainTiles_TallGrass[];
extern const u32 gBattleTerrainTilemap_TallGrass[];
extern const u32 gBattleTerrainAnimTiles_TallGrass[];
extern const u32 gBattleTerrainAnimTilemap_TallGrass[];
extern const u32 gBattleTerrainPalette_TallGrassMorning[];
extern const u32 gBattleTerrainPalette_TallGrassDay[];
extern const u32 gBattleTerrainPalette_TallGrassNight[];

extern const u32 gBattleTerrainAnimTiles_LongGrass[];
extern const u32 gBattleTerrainAnimTilemap_LongGrass[];
extern const u32 gBattleTerrainPalette_LongGrassMorning[];
extern const u32 gBattleTerrainPalette_LongGrassDay[];
extern const u32 gBattleTerrainPalette_LongGrassNight[];

extern const u32 gBattleTerrainTiles_Sand[];
extern const u32 gBattleTerrainTilemap_Sand[];
extern const u32 gBattleTerrainAnimTiles_Sand[];
extern const u32 gBattleTerrainAnimTilemap_Sand[];
extern const u32 gBattleTerrainPalette_SandMorning[];
extern const u32 gBattleTerrainPalette_SandDay[];
extern const u32 gBattleTerrainPalette_SandNight[];

extern const u32 gBattleTerrainTiles_Underwater[];
extern const u32 gBattleTerrainTilemap_Underwater[];
extern const u32 gBattleTerrainAnimTiles_Underwater[];
extern const u32 gBattleTerrainAnimTilemap_Underwater[];
extern const u32 gBattleTerrainPalette_Underwater[];

extern const u32 gBattleTerrainTiles_Water[];
extern const u32 gBattleTerrainTilemap_Water[];
extern const u32 gBattleTerrainAnimTiles_Water[];
extern const u32 gBattleTerrainAnimTilemap_Water[];
extern const u32 gBattleTerrainPalette_WaterMorning[];
extern const u32 gBattleTerrainPalette_WaterDay[];
extern const u32 gBattleTerrainPalette_WaterNight[];

extern const u32 gBattleTerrainTiles_Pond[];
extern const u32 gBattleTerrainTilemap_Pond[];
extern const u32 gBattleTerrainAnimTiles_Pond[];
extern const u32 gBattleTerrainAnimTilemap_Pond[];
extern const u32 gBattleTerrainPalette_PondMorning[];
extern const u32 gBattleTerrainPalette_PondDay[];
extern const u32 gBattleTerrainPalette_PondNight[];

extern const u32 gBattleTerrainTiles_Mountain[];
extern const u32 gBattleTerrainTilemap_Mountain[];
extern const u32 gBattleTerrainAnimTiles_Mountain[];
extern const u32 gBattleTerrainAnimTilemap_Mountain[];
extern const u32 gBattleTerrainPalette_MountainMorning[];
extern const u32 gBattleTerrainPalette_MountainDay[];
extern const u32 gBattleTerrainPalette_MountainNight[];

extern const u32 gBattleTerrainTiles_Cave[];
extern const u32 gBattleTerrainTilemap_Cave[];
extern const u32 gBattleTerrainAnimTiles_Cave[];
extern const u32 gBattleTerrainAnimTilemap_Cave[];
extern const u32 gBattleTerrainPalette_Cave[];

extern const u32 gBattleTerrainTiles_Building[];
extern const u32 gBattleTerrainTilemap_Building[];
extern const u32 gBattleTerrainAnimTiles_Building[];
extern const u32 gBattleTerrainAnimTilemap_Building[];
extern const u32 gBattleTerrainPalette_Building[];

extern const u32 gBattleTerrainPalette_Plain[];

extern const u32 gBattleTerrainPalette_Link[];

extern const u32 gBattleTerrainPalette_Gym[];

extern const u32 gBattleTerrainPalette_Indoor1[];

extern const u32 gBattleTerrainPalette_Indoor2[];

extern const u32 gBattleTerrainPalette_Lorelei[];

extern const u32 gBattleTerrainPalette_Bruno[];

extern const u32 gBattleTerrainPalette_Agatha[];

extern const u32 gBattleTerrainPalette_Lance[];

extern const u32 gBattleTerrainPalette_Champion[];

void LoadBattleTerrainGfx(u16 terrain);
void BattleInitBgsAndWindows(void);
void InitBattleBgsVideo(void);
void LoadBattleMenuWindowGfx(void);
void LoadBattleTextboxAndBackground(void);
void InitLinkBattleVsScreen(u8 taskId);
void DrawBattleEntryBackground(void);
u8 GetBattleTerrainOverride(void);
bool8 LoadChosenBattleElement(u8 caseId);

#endif // GUARD_BATTLE_BG_H
