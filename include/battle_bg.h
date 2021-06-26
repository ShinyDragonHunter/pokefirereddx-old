#ifndef GUARD_BATTLE_BG_H
#define GUARD_BATTLE_BG_H

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
