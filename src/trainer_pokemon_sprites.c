#include "global.h"
#include "sprite.h"
#include "window.h"
#include "malloc.h"
#include "palette.h"
#include "decompress.h"
#include "trainer_pokemon_sprites.h"
#include "data.h"
#include "pokemon.h"
#include "constants/trainers.h"

// Static type declarations

struct PicData
{
    u8 *frames;
    struct SpriteFrameImage *images;
    u16 paletteTag;
    u8 spriteId;
    u8 active;
};

// Static RAM declarations
#define PICS_COUNT 8

static EWRAM_DATA struct SpriteTemplate sCreatingSpriteTemplate = {};
static EWRAM_DATA struct PicData sSpritePics[PICS_COUNT] = {};

// Static ROM declarations

// .rodata

static const struct PicData sDummyPicData = {};

static const struct OamData gUnknown_0860B064 =
{
    .shape = SPRITE_SHAPE(64x64),
    .size = SPRITE_SIZE(64x64)
};

static const struct OamData gUnknown_0860B06C =
{
    .affineMode = ST_OAM_AFFINE_NORMAL,
    .shape = SPRITE_SHAPE(64x64),
    .size = SPRITE_SIZE(64x64)
};

// .text

static void DummyPicSpriteCallback(struct Sprite *sprite)
{

}

bool16 ResetAllPicSprites(void)
{
    int i;

    for (i = 0; i < PICS_COUNT; i ++)
        sSpritePics[i] = sDummyPicData;

    return FALSE;
}

static bool16 DecompressPic(u16 species, u32 personality, bool8 isFrontPic, u8 *dest, bool8 isTrainer)
{
    if (isTrainer)
    {
        DecompressPicFromTable(&gTrainerFrontPicTable[species], dest, species);
    }
    else
    {
        if (isFrontPic)
        {
            LoadSpecialPokePic(&gMonFrontPicTable[species], dest, species, personality, isFrontPic, 0);
        }
        else
        {
            LoadSpecialPokePic(&gMonBackPicTable[species], dest, species, personality, isFrontPic, 0);
        }
    }
    return FALSE;
}

static void LoadPicPaletteByTagOrSlot(u16 species, u32 otId, u32 personality, u8 paletteSlot, u16 paletteTag, bool8 isTrainer)
{
    if (paletteTag == TAG_NONE)
    {
        sCreatingSpriteTemplate.paletteTag = TAG_NONE;

        if (isTrainer)
            LoadPalette(gTrainerFrontPicPaletteTable[species].data, 0x100 + paletteSlot * 0x10, 0x20);
        else
            LoadPalette(GetMonSpritePalFromSpeciesAndPersonality(species, otId, personality), 0x100 + paletteSlot * 0x10, 0x20);
    }
    else
    {
        sCreatingSpriteTemplate.paletteTag = paletteTag;

        if (isTrainer)
            LoadSpritePalette(&gTrainerFrontPicPaletteTable[species]);
        else
            LoadSpritePalette(GetMonSpritePalStructFromOtIdPersonality(species, otId, personality));   
    }
}

static void LoadPicPaletteBySlot(u16 species, u32 otId, u32 personality, u8 paletteSlot, bool8 isTrainer)
{
    if (isTrainer)
        LoadPalette(gTrainerFrontPicPaletteTable[species].data, paletteSlot * 0x10, 0x20);
    else
        LoadPalette(GetMonSpritePalFromSpeciesAndPersonality(species, otId, personality), paletteSlot * 0x10, 0x20);
}

static void AssignSpriteAnimsTable(bool8 isTrainer)
{
    if (isTrainer)
        sCreatingSpriteTemplate.anims = gTrainerFrontAnimsPtrTable[0];
    else
        sCreatingSpriteTemplate.anims = gAnims_MonPic;
}

static u16 CreatePicSprite(u16 species, u32 otId, u32 personality, bool8 isFrontPic, s16 x, s16 y, u8 paletteSlot, u16 paletteTag, bool8 isTrainer)
{
    u32 i;
    u8 *framePics;
    struct SpriteFrameImage *images;
    int j;
    u8 spriteId;

    for (i = 0; i < PICS_COUNT; i ++)
    {
        if (!sSpritePics[i].active)
        {
            break;
        }
    }
    if (i == PICS_COUNT)
    {
        return 0xFFFF;
    }
    framePics = Alloc(4 * 0x800);
    if (!framePics)
    {
        return 0xFFFF;
    }
    images = Alloc(4 * sizeof(struct SpriteFrameImage));
    if (!images)
    {
        Free(framePics);
        return 0xFFFF;
    }
    if (DecompressPic(species, personality, isFrontPic, framePics, isTrainer))
    {
        // debug trap?
        return 0xFFFF;
    }
    for (j = 0; j < 4; j ++)
    {
        images[j].data = framePics + 0x800 * j;
        images[j].size = 0x800;
    }
    sCreatingSpriteTemplate.tileTag = TAG_NONE;
    sCreatingSpriteTemplate.oam = &gUnknown_0860B064;
    AssignSpriteAnimsTable(isTrainer);
    sCreatingSpriteTemplate.images = images;
    sCreatingSpriteTemplate.affineAnims = gDummySpriteAffineAnimTable;
    sCreatingSpriteTemplate.callback = DummyPicSpriteCallback;
    LoadPicPaletteByTagOrSlot(species, otId, personality, paletteSlot, paletteTag, isTrainer);
    spriteId = CreateSprite(&sCreatingSpriteTemplate, x, y, 0);
    if (paletteTag == TAG_NONE)
    {
        gSprites[spriteId].oam.paletteNum = paletteSlot;
    }
    sSpritePics[i].frames = framePics;
    sSpritePics[i].images = images;
    sSpritePics[i].paletteTag = paletteTag;
    sSpritePics[i].spriteId = spriteId;
    sSpritePics[i].active = TRUE;
    return spriteId;
}

u16 CreatePicSprite2(u16 species, u32 otId, u32 personality, u8 flags, s16 x, s16 y, u8 paletteSlot, u16 paletteTag, u8 form)
{
    u8 *framePics;
    struct SpriteFrameImage *images;
    int j;
    u32 i;
    u8 spriteId;
    u8 flags2;
    u16 formSpecies = GetFormSpecies(species, form);

    for (i = 0; i < PICS_COUNT; i ++)
    {
        if (!sSpritePics[i].active)
        {
            break;
        }
    }
    if (i == PICS_COUNT)
    {
        return 0xFFFF;
    }
    framePics = Alloc(4 * 0x800);
    if (!framePics)
    {
        return 0xFFFF;
    }
    if (flags & 0x80)
    {
        flags &= 0x7F;
        flags2 = 3;
    }
    else
    {
        flags2 = flags;
    }
    images = Alloc(4 * sizeof(struct SpriteFrameImage));
    if (!images)
    {
        Free(framePics);
        return 0xFFFF;
    }
    if (DecompressPic(formSpecies, personality, flags, framePics, FALSE))
    {
        // debug trap?
        return 0xFFFF;
    }
    for (j = 0; j < 4; j ++)
    {
        images[j].data = framePics + 0x800 * j;
        images[j].size = 0x800;
    }
    sCreatingSpriteTemplate.tileTag = TAG_NONE;
    sCreatingSpriteTemplate.anims = gMonFrontAnimsPtrTable[formSpecies];
    sCreatingSpriteTemplate.images = images;
    if (flags2 == 0x01)
    {
        sCreatingSpriteTemplate.affineAnims = gAffineAnims_BattleSpriteOpponentSide;
        sCreatingSpriteTemplate.oam = &gUnknown_0860B06C;
    }
    else if (flags2 == 0x00)
    {
        sCreatingSpriteTemplate.affineAnims = gAffineAnims_BattleSpritePlayerSide;
        sCreatingSpriteTemplate.oam = &gUnknown_0860B06C;
    }
    else
    {
        sCreatingSpriteTemplate.oam = &gUnknown_0860B064;
        sCreatingSpriteTemplate.affineAnims = gDummySpriteAffineAnimTable;
    }
    sCreatingSpriteTemplate.callback = DummyPicSpriteCallback;
    LoadPicPaletteByTagOrSlot(formSpecies, otId, personality, paletteSlot, paletteTag, FALSE);
    spriteId = CreateSprite(&sCreatingSpriteTemplate, x, y, 0);
    if (paletteTag == TAG_NONE)
    {
        gSprites[spriteId].oam.paletteNum = paletteSlot;
    }
    sSpritePics[i].frames = framePics;
    sSpritePics[i].images = images;
    sSpritePics[i].paletteTag = paletteTag;
    sSpritePics[i].spriteId = spriteId;
    sSpritePics[i].active = TRUE;
    return spriteId;
}

static u16 FreeAndDestroyPicSpriteInternal(u16 spriteId)
{
    u32 i;
    u8 *framePics;
    struct SpriteFrameImage *images;

    for (i = 0; i < PICS_COUNT; i ++)
    {
        if (sSpritePics[i].spriteId == spriteId)
        {
            break;
        }
    }
    if (i == PICS_COUNT)
    {
        return 0xFFFF;
    }
    framePics = sSpritePics[i].frames;
    images = sSpritePics[i].images;
    if (sSpritePics[i].paletteTag != TAG_NONE)
    {
        FreeSpritePaletteByTag(GetSpritePaletteTagByPaletteNum(gSprites[spriteId].oam.paletteNum));
    }
    DestroySprite(&gSprites[spriteId]);
    Free(framePics);
    Free(images);
    sSpritePics[i] = sDummyPicData;
    return 0;
}

static u16 sub_818D65C(u16 species, u32 otId, u32 personality, bool8 isFrontPic, u8 paletteSlot, u8 windowId, bool8 isTrainer)
{
    if (DecompressPic(species, personality, isFrontPic, (u8 *)GetWindowAttribute(windowId, WINDOW_TILE_DATA), FALSE))
    {
        return 0xFFFF;
    }
    LoadPicPaletteBySlot(species, otId, personality, paletteSlot, isTrainer);
    return 0;
}

u16 CreateTrainerCardTrainerPicSprite(u16 species, u16 destX, u16 destY)
{
    u8 *framePics;

    framePics = Alloc(4 * 0x800);
    if (framePics && !DecompressPic(species, 0, TRUE, framePics, TRUE))
    {
        BlitBitmapRectToWindow(2, framePics, 0, 0, 0x40, 0x40, destX, destY, 0x40, 0x40);
        LoadPicPaletteBySlot(species, 0, 0, 8, TRUE);
        Free(framePics);
        return 0;
    }
    return 0xFFFF;
}

u16 CreateMonPicSprite(u16 species, u32 otId, u32 personality, bool8 isFrontPic, s16 x, s16 y, u8 paletteSlot, u16 paletteTag, u8 form)
{
    return CreatePicSprite(GetFormSpecies(species, form), otId, personality, isFrontPic, x, y, paletteSlot, paletteTag, FALSE);
}

u16 FreeAndDestroyMonPicSprite(u16 spriteId)
{
    return FreeAndDestroyPicSpriteInternal(spriteId);
}

u16 sub_818D834(u16 species, u32 otId, u32 personality, bool8 isFrontPic, u8 paletteSlot, u8 windowId)
{
    return sub_818D65C(species, otId, personality, isFrontPic, paletteSlot, windowId, FALSE);
}

u16 CreateTrainerPicSprite(u16 species, bool8 isFrontPic, s16 x, s16 y, u8 paletteSlot, u16 paletteTag)
{
    return CreatePicSprite(species, 0, 0, isFrontPic, x, y, paletteSlot, paletteTag, TRUE);
}

u16 FreeAndDestroyTrainerPicSprite(u16 spriteId)
{
    return FreeAndDestroyPicSpriteInternal(spriteId);
}

u16 sub_818D904(u16 species, bool8 isFrontPic, u8 paletteSlot, u8 windowId)
{
    return sub_818D65C(species, 0, 0, isFrontPic, paletteSlot, windowId, TRUE);
}
