#ifndef GUARD_SCRIPT_POKEMON_UTIL
#define GUARD_SCRIPT_POKEMON_UTIL

u8 ScriptGiveMon(u16 species, u8 level, u16 item, u8 form);
u8 ScriptGiveEgg(u16 species);
void CreateScriptedWildMon(u16 species, u8 level, u16 item);
void CreateScriptedDoubleWildMon(u16 species, u8 level, u16 item, u16 species2, u8 level2, u16 item2);
void ScriptSetMonMoveSlot(u8 monIndex, u16 move, u8 slot);
void ReducePlayerPartyToSelectedMons(void);
void HealPlayerParty(void);

#endif // GUARD_SCRIPT_POKEMON_UTIL
