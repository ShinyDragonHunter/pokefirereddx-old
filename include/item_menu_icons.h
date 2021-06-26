#ifndef GUARD_ITEM_MENU_ICONS_H
#define GUARD_ITEM_MENU_ICONS_H

extern u8 *gItemIconDecompressionBuffer;
extern u8 *gItemIcon4x4Buffer;

extern const struct CompressedSpriteSheet gBagMaleSpriteSheet;
extern const struct CompressedSpriteSheet gBagFemaleSpriteSheet;
extern const struct CompressedSpritePalette gBagPaletteTable;
extern const struct CompressedSpriteSheet gBerryCheckCircleSpriteSheet;
extern const struct CompressedSpritePalette gBerryCheckCirclePaletteTable;
extern const struct SpriteTemplate gItemIconSpriteTemplate;

bool8 AllocItemIconTemporaryBuffers(void);
void CopyItemIconPicTo4x4Buffer(const void *src, void *dest);
u8 AddItemIconSprite(u16 tilesTag, u16 paletteTag, u16 itemId);
u8 AddCustomItemIconSprite(const struct SpriteTemplate *customSpriteTemplate, u16 tilesTag, u16 paletteTag, u16 itemId);
void HideBagItemIconSprite(u8 id);
const void *GetItemIconPicOrPalette(u16 itemId, u8 which);
void AddBagVisualSprite(u8 bagPocketId);
void SetBagVisualPocketId(u8 bagPocketId);
void ShakeBagSprite(void);
void CreateSwapLineSprites(u8 *spriteIds, u8 count);
void DestroySwapLineSprites(u8 *spriteIds, u8 count);
void SetSwapLineSpritesInvisibility(u8 *spriteIds, u8 count, bool8 invisible);
void UpdateSwapLineSpritesPos(u8 *spriteIds, u8 count, s16 x, u16 y);
void LoadListMenuArrowsGfx(void);
void AddSwitchPocketRotatingBallSprite(s16 rotationDirection);
void AddBagItemIconSprite(u16 itemId, u8 id);
void RemoveBagItemIconSprite(u8 id);
void CreateItemMenuSwapLine(void);
void SetItemMenuSwapLineInvisibility(bool8 invisible);
void UpdateItemMenuSwapLinePos(s16 x, s16 y);
u8 CreateBerryTagSprite(u8 id, s16 x, s16 y);
void FreeBerryTagSpritePalette(void);
u8 CreateSpinningBerrySprite(u8 berryId, u8 x, u8 y, bool8 startAffine);
u8 CreateBerryFlavorCircleSprite(s16 x);
void ResetItemMenuIconState(void);

#define TAG_BAG_GFX 100
#define TAG_ROTATING_BALL_GFX 101
#define TAG_BERRY_CHECK_CIRCLE_GFX 10000
#define TAG_BERRY_PIC_TILE 0xFFFF
#define TAG_BERRY_PIC_PAL 0x7544

#endif // GUARD_ITEM_MENU_ICONS_H
