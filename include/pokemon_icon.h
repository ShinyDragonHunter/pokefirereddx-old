#ifndef GUARD_POKEMON_ICON_H
#define GUARD_POKEMON_ICON_H

extern const u16 gMonIconPalettes[][16];
extern const u8 gMonIconPaletteIndices[];

const u8 *GetMonIconTiles(u16 species);
void sub_80D304C(u16 offset);
u8 GetValidMonIconPalIndex(u16 species, u8 form);
const u8 *GetMonIconPtr(u16 species, u32 personality, u8 form);
const u16 *GetValidMonIconPalettePtr(u16 speciesId, u8 form);
u16 GetIconSpecies(u16 species, u32 personality, u8 form);
u32 GetUnownLetterByPersonality(u32 personality);
u16 GetIconSpeciesNoPersonality(u16 species);
void LoadMonIconPalettes(void);
void LoadMonIconPalette(u16 species);
void FreeMonIconPalettes(void);
u8 CreateMonIconNoPersonality(u16 species, void (*callback)(struct Sprite *), s16 x, s16 y, u8 subpriority);
void FreeMonIconPalette(u16 species);
void FreeAndDestroyMonIconSprite(struct Sprite *sprite);
u8 CreateMonIcon(u16 species, void (*callback)(struct Sprite *), s16 x, s16 y, u8 subpriority, u32 personality, u8 form);
u8 UpdateMonIconFrame(struct Sprite *sprite);
void LoadMonIconPalette(u16 species);
void SpriteCB_MonIcon(struct Sprite *sprite);
void SetPartyHPBarSprite(struct Sprite *sprite, u8 animNum);

#endif // GUARD_POKEMON_ICON_H
