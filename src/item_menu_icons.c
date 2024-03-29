#include "global.h"
#include "berry.h"
#include "decompress.h"
#include "graphics.h"
#include "item.h"
#include "item_menu.h"
#include "item_menu_icons.h"
#include "menu_helpers.h"
#include "malloc.h"
#include "sprite.h"
#include "window.h"
#include "constants/items.h"

enum {
    TAG_BAG_GFX = 100,
    TAG_SWAP_LINE,
    TAG_ITEM_ICON,
    TAG_ITEM_ICON_ALT
};

#define TAG_BERRY_CHECK_CIRCLE_GFX 10000
#define TAG_BERRY_PIC_PAL 30020

struct CompressedTilesPal
{
    const u32 *tiles;
    const u32 *pal;
};

static EWRAM_DATA u8 sItemMenuIconSpriteIds[ITEMMENUSPRITE_COUNT] = {0};
EWRAM_DATA u8 *gItemIconDecompressionBuffer = NULL;
EWRAM_DATA u8 *gItemIcon4x4Buffer = NULL;

// this file's functions
static void SpriteCB_BagVisualSwitchingPockets(struct Sprite *sprite);
static void SpriteCB_ShakeBagSprite(struct Sprite *sprite);
static void SpriteCB_SwitchPocketRotatingBallInit(struct Sprite *sprite);
static void SpriteCB_SwitchPocketRotatingBallContinue(struct Sprite *sprite);
static void FreeItemIconTemporaryBuffers(void);

static const struct OamData sBagOamData =
{
    .y = 0,
    .affineMode = ST_OAM_AFFINE_NORMAL,
    .objMode = ST_OAM_OBJ_NORMAL,
    .mosaic = 0,
    .bpp = ST_OAM_4BPP,
    .shape = SPRITE_SHAPE(64x64),
    .x = 0,
    .matrixNum = 0,
    .size = SPRITE_SIZE(64x64),
    .tileNum = 0,
    .priority = 1,
    .paletteNum = 0,
    .affineParam = 0
};

static const union AnimCmd sSpriteAnim_Bag_Items[] =
{
    ANIMCMD_FRAME(0, 0),
    ANIMCMD_END
};

static const union AnimCmd sSpriteAnim_Bag_KeyItems[] =
{
    ANIMCMD_FRAME(64, 0),
    ANIMCMD_END
};

static const union AnimCmd sSpriteAnim_Bag_Pokeballs[] =
{
    ANIMCMD_FRAME(128, 0),
    ANIMCMD_END
};

static const union AnimCmd sSpriteAnim_Bag_TMsHMs[] =
{
    ANIMCMD_FRAME(192, 0),
    ANIMCMD_END
};

static const union AnimCmd sSpriteAnim_Bag_Berries[] =
{
    ANIMCMD_FRAME(256, 0),
    ANIMCMD_END
};

static const union AnimCmd *const sBagSpriteAnimTable[] =
{
    sSpriteAnim_Bag_Items,
    sSpriteAnim_Bag_KeyItems,
    sSpriteAnim_Bag_Pokeballs,
    sSpriteAnim_Bag_TMsHMs,
    sSpriteAnim_Bag_Berries
};

static const union AffineAnimCmd sSpriteAffineAnim_BagNormal[] =
{
    AFFINEANIMCMD_FRAME(256, 256, 0, 0),
    AFFINEANIMCMD_END
};

static const union AffineAnimCmd sSpriteAffineAnim_BagShake[] =
{
    AFFINEANIMCMD_FRAME(0, 0, -2, 2),
    AFFINEANIMCMD_FRAME(0, 0,  2, 4),
    AFFINEANIMCMD_FRAME(0, 0, -2, 4),
    AFFINEANIMCMD_FRAME(0, 0,  2, 2),
    AFFINEANIMCMD_END
};

static const union AffineAnimCmd *const sBagAffineAnimCmds[] =
{
    sSpriteAffineAnim_BagNormal,
    sSpriteAffineAnim_BagShake
};

const struct CompressedSpriteSheet gBagMaleSpriteSheet =
{
    gBagMaleTiles, 0x2800, TAG_BAG_GFX
};

const struct CompressedSpriteSheet gBagFemaleSpriteSheet =
{
    gBagFemaleTiles, 0x2800, TAG_BAG_GFX
};

const struct CompressedSpritePalette gBagPaletteTable =
{
    gBagPalette, TAG_BAG_GFX
};

static const struct SpriteTemplate sBagSpriteTemplate =
{
    .tileTag = TAG_BAG_GFX,
    .paletteTag = TAG_BAG_GFX,
    .oam = &sBagOamData,
    .anims = sBagSpriteAnimTable,
    .images = NULL,
    .affineAnims = sBagAffineAnimCmds,
    .callback = SpriteCallbackDummy,
};

static const struct OamData sOamData_SwapLine =
{
    .y = 0,
    .affineMode = ST_OAM_AFFINE_OFF,
    .objMode = ST_OAM_OBJ_NORMAL,
    .mosaic = 0,
    .bpp = ST_OAM_4BPP,
    .shape = SPRITE_SHAPE(16x16),
    .x = 0,
    .matrixNum = 0,
    .size = SPRITE_SIZE(16x16),
    .tileNum = 0,
    .priority = 1,
    .paletteNum = 1,
    .affineParam = 0
};

static const union AnimCmd sAnim_SwapLine_Line[] =
{
    ANIMCMD_FRAME(4, 0),
    ANIMCMD_END
};

static const union AnimCmd sAnim_SwapLine_LeftArrow[] =
{
    ANIMCMD_FRAME(0, 0, .hFlip = TRUE),
    ANIMCMD_END
};

static const union AnimCmd *const sAnims_SwapLine[] =
{
    sSpriteAnim_Bag_Items,
    sAnim_SwapLine_Line,
    sAnim_SwapLine_LeftArrow
};

static const struct CompressedSpriteSheet sSpriteSheet_SwapLine =
{
    gBagSwapLineGfx, 0x100, TAG_SWAP_LINE
};

static const struct CompressedSpritePalette sSpritePalette_SwapLine =
{
    gBagSwapLinePal, TAG_SWAP_LINE
};

static const struct SpriteTemplate sSpriteTemplate_SwapLine =
{
    .tileTag = TAG_SWAP_LINE,
    .paletteTag = TAG_SWAP_LINE,
    .oam = &sOamData_SwapLine,
    .anims = sAnims_SwapLine,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = SpriteCallbackDummy,
};

static const struct OamData sOamData_ItemIcon =
{
    .y = 0,
    .affineMode = ST_OAM_AFFINE_OFF,
    .objMode = ST_OAM_OBJ_NORMAL,
    .mosaic = 0,
    .bpp = ST_OAM_4BPP,
    .shape = SPRITE_SHAPE(32x32),
    .x = 0,
    .matrixNum = 0,
    .size = SPRITE_SIZE(32x32),
    .tileNum = 0,
    .priority = 1,
    .paletteNum = 2,
    .affineParam = 0
};

static const union AnimCmd *const sSpriteAnimTable_ItemIcon[] =
{
    sSpriteAnim_Bag_Items
};

const struct SpriteTemplate gItemIconSpriteTemplate =
{
    .tileTag = TAG_ITEM_ICON,
    .paletteTag = TAG_ITEM_ICON,
    .oam = &sOamData_ItemIcon,
    .anims = sSpriteAnimTable_ItemIcon,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = SpriteCallbackDummy,
};

#include "data/item_icon_table.h"

// code
void ResetItemMenuIconState(void)
{
    memset(sItemMenuIconSpriteIds, 0xFF, sizeof(sItemMenuIconSpriteIds));
}

void AddBagVisualSprite(u8 bagPocketId)
{
    sItemMenuIconSpriteIds[ITEMMENUSPRITE_BAG] = CreateSprite(&sBagSpriteTemplate, 40, 68, 0);
    SetBagVisualPocketId(bagPocketId);
}

void SetBagVisualPocketId(u8 bagPocketId)
{
    struct Sprite *sprite = &gSprites[sItemMenuIconSpriteIds[ITEMMENUSPRITE_BAG]];
    sprite->y2 = -5;
    sprite->callback = SpriteCB_BagVisualSwitchingPockets;
    StartSpriteAnim(sprite, bagPocketId);
}

static void SpriteCB_BagVisualSwitchingPockets(struct Sprite *sprite)
{
    if (sprite->y2)
        sprite->y2++;
    else
        sprite->callback = SpriteCallbackDummy;
}

void ShakeBagSprite(void)
{
    struct Sprite *sprite = &gSprites[sItemMenuIconSpriteIds[ITEMMENUSPRITE_BAG]];
    if (sprite->affineAnimEnded)
    {
        StartSpriteAffineAnim(sprite, 1);
        sprite->callback = SpriteCB_ShakeBagSprite;
    }
}

static void SpriteCB_ShakeBagSprite(struct Sprite *sprite)
{
    if (sprite->affineAnimEnded)
    {
        StartSpriteAffineAnim(sprite, 0);
        sprite->callback = SpriteCallbackDummy;
    }
}

void CreateSwapLineSprites(u8 *spriteIds, u8 count)
{
    u32 i;
    spriteIds = &sItemMenuIconSpriteIds[ITEMMENUSPRITE_SWAP_LINE];

    for (i = 0; i < count; i++)
    {
        spriteIds[i] = CreateSprite(&sSpriteTemplate_SwapLine, i * 16 + 0x60, 7, 0);
        switch (i)
        {
        case 8:
            StartSpriteAnim(&gSprites[spriteIds[i]], 2);
        case 0:
            break;
        default:
            StartSpriteAnim(&gSprites[spriteIds[i]], 1);
            break;
        }
        gSprites[spriteIds[i]].invisible = TRUE;
    }
}

void SetSwapLineSpritesInvisibility(u8 *spriteIds, u8 count, bool8 invisible)
{
    u32 i;
    spriteIds = &sItemMenuIconSpriteIds[ITEMMENUSPRITE_SWAP_LINE];

    for (i = 0; i < count; i++)
        gSprites[spriteIds[i]].invisible = invisible;
}

void UpdateSwapLineSpritesPos(u8 *spriteIds, u8 count, s16 x, u16 y)
{
    u32 i;
    spriteIds = &sItemMenuIconSpriteIds[ITEMMENUSPRITE_SWAP_LINE];

    for (i = 0; i < count; i++)
    {
        gSprites[spriteIds[i]].x2 = x;
        gSprites[spriteIds[i]].y = y + 7;
    }
}

void DestroySwapLineSprites(u8 *spriteIds, u8 count)
{
    u32 i;

    for (i = 0; i < count; i++)
    {
        if (i == count - 1)
            DestroySpriteAndFreeResources(&gSprites[spriteIds[i]]);
        else
            DestroySprite(&gSprites[spriteIds[i]]);
    }
}

void CreateItemMenuSwapLine(void)
{
    CreateSwapLineSprites(&sItemMenuIconSpriteIds[ITEMMENUSPRITE_SWAP_LINE], ITEMMENU_SWAP_LINE_LENGTH);
}

void SetItemMenuSwapLineInvisibility(bool8 invisible)
{
    SetSwapLineSpritesInvisibility(&sItemMenuIconSpriteIds[ITEMMENUSPRITE_SWAP_LINE], ITEMMENU_SWAP_LINE_LENGTH, invisible);
}

void UpdateItemMenuSwapLinePos(s16 x, s16 y)
{
    UpdateSwapLineSpritesPos(&sItemMenuIconSpriteIds[ITEMMENUSPRITE_SWAP_LINE], ITEMMENU_SWAP_LINE_LENGTH, x, y);
}

bool8 AllocItemIconTemporaryBuffers(void)
{
    gItemIconDecompressionBuffer = Alloc(0x120);
    if (!gItemIconDecompressionBuffer)
        return FALSE;

    gItemIcon4x4Buffer = AllocZeroed(0x200);
    if (!gItemIcon4x4Buffer)
    {
        Free(gItemIconDecompressionBuffer);
        return FALSE;
    }
    return TRUE;
}

void CopyItemIconPicTo4x4Buffer(const void *src, void *dest)
{
    u32 i;

    for (i = 0; i < 3; i++)
        CpuCopy16(src + 0x60 * i, dest + 0x80 * i, 0x60);
}

void LoadListMenuArrowsGfx(void)
{
    LoadCompressedSpriteSheet(&sSpriteSheet_SwapLine);
    LoadCompressedSpritePalette(&sSpritePalette_SwapLine);
}

u8 AddItemIconSprite(u16 tilesTag, u16 paletteTag, u16 itemId)
{
    if (AllocItemIconTemporaryBuffers())
    {
        u8 spriteId;
        struct SpriteSheet spriteSheet;
        struct CompressedSpritePalette spritePalette;
        struct SpriteTemplate *spriteTemplate;

        LZDecompressWram(GetItemIconPicOrPalette(itemId, 0), gItemIconDecompressionBuffer);
        CopyItemIconPicTo4x4Buffer(gItemIconDecompressionBuffer, gItemIcon4x4Buffer);
        spriteSheet.data = gItemIcon4x4Buffer;
        spriteSheet.size = 0x200;
        spriteSheet.tag = tilesTag;
        LoadSpriteSheet(&spriteSheet);

        spritePalette.data = GetItemIconPicOrPalette(itemId, 1);
        spritePalette.tag = paletteTag;
        LoadCompressedSpritePalette(&spritePalette);

        spriteTemplate = Alloc(sizeof(*spriteTemplate));
        CpuCopy16(&gItemIconSpriteTemplate, spriteTemplate, sizeof(*spriteTemplate));
        spriteTemplate->tileTag = tilesTag;
        spriteTemplate->paletteTag = paletteTag;
        spriteId = CreateSprite(spriteTemplate, 0, 0, 0);
        FreeItemIconTemporaryBuffers();
        return spriteId;
    }
    return MAX_SPRITES;
}

u8 AddCustomItemIconSprite(const struct SpriteTemplate *customSpriteTemplate, u16 tilesTag, u16 paletteTag, u16 itemId)
{
    if (AllocItemIconTemporaryBuffers())
    {
        u8 spriteId;
        struct SpriteSheet spriteSheet;
        struct CompressedSpritePalette spritePalette;
        struct SpriteTemplate spriteTemplate;

        LZDecompressWram(GetItemIconPicOrPalette(itemId, 0), gItemIconDecompressionBuffer);
        CopyItemIconPicTo4x4Buffer(gItemIconDecompressionBuffer, gItemIcon4x4Buffer);
        spriteSheet.data = gItemIcon4x4Buffer;
        spriteSheet.size = 0x200;
        spriteSheet.tag = tilesTag;
        LoadSpriteSheet(&spriteSheet);

        spritePalette.data = GetItemIconPicOrPalette(itemId, 1);
        spritePalette.tag = paletteTag;
        LoadCompressedSpritePalette(&spritePalette);

        CpuCopy16(customSpriteTemplate, &spriteTemplate, sizeof(spriteTemplate));
        spriteTemplate.tileTag = tilesTag;
        spriteTemplate.paletteTag = paletteTag;
        spriteId = CreateSprite(&spriteTemplate, 0, 0, 0);
        FreeItemIconTemporaryBuffers();
        return spriteId;
    }
    return MAX_SPRITES;
}

static void FreeItemIconTemporaryBuffers(void)
{
    Free(gItemIconDecompressionBuffer);
    Free(gItemIcon4x4Buffer);
}

void AddBagItemIconSprite(u16 itemId, u8 id)
{
    u8 *spriteId = &sItemMenuIconSpriteIds[ITEMMENUSPRITE_ITEM];

    if (spriteId[id] == SPRITE_NONE)
    {
        u8 iconSpriteId;

        // Either TAG_ITEM_ICON or TAG_ITEM_ICON_ALT
        FreeSpriteTilesByTag(id + TAG_ITEM_ICON);
        FreeSpritePaletteByTag(id + TAG_ITEM_ICON);
        iconSpriteId = AddItemIconSprite(id + TAG_ITEM_ICON, id + TAG_ITEM_ICON, itemId);
        if (iconSpriteId != MAX_SPRITES)
        {
            spriteId[id] = iconSpriteId;
            gSprites[iconSpriteId].x2 = 24;
            gSprites[iconSpriteId].y2 = 140;
        }
    }
}

void RemoveBagItemIconSprite(u8 id)
{
    u8 *spriteId = &sItemMenuIconSpriteIds[ITEMMENUSPRITE_ITEM];

    if (spriteId[id ^ 1] != SPRITE_NONE)
        gSprites[spriteId[id ^ 1]].invisible = TRUE;

    if (spriteId[id] != SPRITE_NONE)
    {
        DestroySpriteAndFreeResources(&gSprites[spriteId[id]]);
        spriteId[id] = SPRITE_NONE;
    }
}

const void *GetItemIconPicOrPalette(u16 itemId, u8 which)
{
    if (itemId == 0xFFFF)
        itemId = ITEM_FIELD_ARROW;
    else if (itemId >= ITEMS_COUNT)
        itemId = ITEM_NONE;
    return gItemIconTable[itemId][which];
}

static const struct OamData sBerryPicOamData =
{
    .y = 0,
    .affineMode = ST_OAM_AFFINE_OFF,
    .objMode = ST_OAM_OBJ_NORMAL,
    .mosaic = 0,
    .bpp = ST_OAM_4BPP,
    .shape = SPRITE_SHAPE(64x64),
    .x = 0,
    .matrixNum = 0,
    .size = SPRITE_SIZE(64x64),
    .tileNum = 0,
    .priority = 1,
    .paletteNum = 7,
    .affineParam = 0
};

static const struct OamData sBerryPicRotatingOamData =
{
    .y = 0,
    .affineMode = ST_OAM_AFFINE_DOUBLE,
    .objMode = ST_OAM_OBJ_NORMAL,
    .mosaic = 0,
    .bpp = ST_OAM_4BPP,
    .shape = SPRITE_SHAPE(64x64),
    .x = 0,
    .matrixNum = 0,
    .size = SPRITE_SIZE(64x64),
    .tileNum = 0,
    .priority = 0,
    .paletteNum = 7,
    .affineParam = 0
};

static const struct SpriteFrameImage sBerryPicSpriteImageTable[] =
{
    {&gDecompressionBuffer[0], 0x800},
};

static const struct SpriteTemplate gBerryPicSpriteTemplate =
{
    .tileTag = TAG_NONE,
    .paletteTag = TAG_BERRY_PIC_PAL,
    .oam = &sBerryPicOamData,
    .anims = sSpriteAnimTable_ItemIcon,
    .images = sBerryPicSpriteImageTable,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = SpriteCallbackDummy,
};

static const union AffineAnimCmd sSpriteAffineAnim_BerryPicRotation1[] =
{
    AFFINEANIMCMD_FRAME(-1, -1, 253, 96),
    AFFINEANIMCMD_FRAME(0, 0, 0, 16),
    AFFINEANIMCMD_FRAME(-2, -2, 255, 64),
    AFFINEANIMCMD_FRAME(-8, 0, 0, 16),
    AFFINEANIMCMD_FRAME(0, -8, 0, 16),
    AFFINEANIMCMD_FRAME(256, 256, 0, 0),
    AFFINEANIMCMD_JUMP(0)
};

static const union AffineAnimCmd sSpriteAffineAnim_BerryPicRotation2[] =
{
    AFFINEANIMCMD_FRAME(-1, -1, 3, 96),
    AFFINEANIMCMD_FRAME(0, 0, 0, 16),
    AFFINEANIMCMD_FRAME(-2, -2, 1, 64),
    AFFINEANIMCMD_FRAME(-8, 0, 0, 16),
    AFFINEANIMCMD_FRAME(0, -8, 0, 16),
    AFFINEANIMCMD_FRAME(256, 256, 0, 0),
    AFFINEANIMCMD_JUMP(0)
};

static const union AffineAnimCmd *const sBerryPicRotatingAnimCmds[] =
{
    sSpriteAffineAnim_BerryPicRotation1,
    sSpriteAffineAnim_BerryPicRotation2
};

static const struct SpriteTemplate gBerryPicRotatingSpriteTemplate =
{
    .tileTag = TAG_NONE,
    .paletteTag = TAG_BERRY_PIC_PAL,
    .oam = &sBerryPicRotatingOamData,
    .anims = sSpriteAnimTable_ItemIcon,
    .images = sBerryPicSpriteImageTable,
    .affineAnims = sBerryPicRotatingAnimCmds,
    .callback = SpriteCallbackDummy,
};

static const struct CompressedTilesPal sBerryPicTable[] =
{
    [ITEM_TO_BERRY(ITEM_CHERI_BERRY)  - 1] = {gBerryPic_Cheri,  gBerryPalette_Cheri},
    [ITEM_TO_BERRY(ITEM_CHESTO_BERRY) - 1] = {gBerryPic_Chesto, gBerryPalette_Chesto},
    [ITEM_TO_BERRY(ITEM_PECHA_BERRY)  - 1] = {gBerryPic_Pecha,  gBerryPalette_Pecha},
    [ITEM_TO_BERRY(ITEM_RAWST_BERRY)  - 1] = {gBerryPic_Rawst,  gBerryPalette_Rawst},
    [ITEM_TO_BERRY(ITEM_ASPEAR_BERRY) - 1] = {gBerryPic_Aspear, gBerryPalette_Aspear},
    [ITEM_TO_BERRY(ITEM_LEPPA_BERRY)  - 1] = {gBerryPic_Leppa,  gBerryPalette_Leppa},
    [ITEM_TO_BERRY(ITEM_ORAN_BERRY)   - 1] = {gBerryPic_Oran,   gBerryPalette_Oran},
    [ITEM_TO_BERRY(ITEM_PERSIM_BERRY) - 1] = {gBerryPic_Persim, gBerryPalette_Persim},
    [ITEM_TO_BERRY(ITEM_LUM_BERRY)    - 1] = {gBerryPic_Lum,    gBerryPalette_Lum},
    [ITEM_TO_BERRY(ITEM_SITRUS_BERRY) - 1] = {gBerryPic_Sitrus, gBerryPalette_Sitrus},
    [ITEM_TO_BERRY(ITEM_FIGY_BERRY)   - 1] = {gBerryPic_Figy,   gBerryPalette_Figy},
    [ITEM_TO_BERRY(ITEM_WIKI_BERRY)   - 1] = {gBerryPic_Wiki,   gBerryPalette_Wiki},
    [ITEM_TO_BERRY(ITEM_MAGO_BERRY)   - 1] = {gBerryPic_Mago,   gBerryPalette_Mago},
    [ITEM_TO_BERRY(ITEM_AGUAV_BERRY)  - 1] = {gBerryPic_Aguav,  gBerryPalette_Aguav},
    [ITEM_TO_BERRY(ITEM_IAPAPA_BERRY) - 1] = {gBerryPic_Iapapa, gBerryPalette_Iapapa},
    [ITEM_TO_BERRY(ITEM_RAZZ_BERRY)   - 1] = {gBerryPic_Razz,   gBerryPalette_Razz},
    [ITEM_TO_BERRY(ITEM_BLUK_BERRY)   - 1] = {gBerryPic_Bluk,   gBerryPalette_Bluk},
    [ITEM_TO_BERRY(ITEM_NANAB_BERRY)  - 1] = {gBerryPic_Nanab,  gBerryPalette_Nanab},
    [ITEM_TO_BERRY(ITEM_WEPEAR_BERRY) - 1] = {gBerryPic_Wepear, gBerryPalette_Wepear},
    [ITEM_TO_BERRY(ITEM_PINAP_BERRY)  - 1] = {gBerryPic_Pinap,  gBerryPalette_Pinap},
    [ITEM_TO_BERRY(ITEM_POMEG_BERRY)  - 1] = {gBerryPic_Pomeg,  gBerryPalette_Pomeg},
    [ITEM_TO_BERRY(ITEM_KELPSY_BERRY) - 1] = {gBerryPic_Kelpsy, gBerryPalette_Kelpsy},
    [ITEM_TO_BERRY(ITEM_QUALOT_BERRY) - 1] = {gBerryPic_Qualot, gBerryPalette_Qualot},
    [ITEM_TO_BERRY(ITEM_HONDEW_BERRY) - 1] = {gBerryPic_Hondew, gBerryPalette_Hondew},
    [ITEM_TO_BERRY(ITEM_GREPA_BERRY)  - 1] = {gBerryPic_Grepa,  gBerryPalette_Grepa},
    [ITEM_TO_BERRY(ITEM_TAMATO_BERRY) - 1] = {gBerryPic_Tamato, gBerryPalette_Tamato},
    [ITEM_TO_BERRY(ITEM_CORNN_BERRY)  - 1] = {gBerryPic_Cornn,  gBerryPalette_Cornn},
    [ITEM_TO_BERRY(ITEM_MAGOST_BERRY) - 1] = {gBerryPic_Magost, gBerryPalette_Magost},
    [ITEM_TO_BERRY(ITEM_RABUTA_BERRY) - 1] = {gBerryPic_Rabuta, gBerryPalette_Rabuta},
    [ITEM_TO_BERRY(ITEM_NOMEL_BERRY)  - 1] = {gBerryPic_Nomel,  gBerryPalette_Nomel},
    [ITEM_TO_BERRY(ITEM_SPELON_BERRY) - 1] = {gBerryPic_Spelon, gBerryPalette_Spelon},
    [ITEM_TO_BERRY(ITEM_PAMTRE_BERRY) - 1] = {gBerryPic_Pamtre, gBerryPalette_Pamtre},
    [ITEM_TO_BERRY(ITEM_WATMEL_BERRY) - 1] = {gBerryPic_Watmel, gBerryPalette_Watmel},
    [ITEM_TO_BERRY(ITEM_DURIN_BERRY)  - 1] = {gBerryPic_Durin,  gBerryPalette_Durin},
    [ITEM_TO_BERRY(ITEM_BELUE_BERRY)  - 1] = {gBerryPic_Belue,  gBerryPalette_Belue},
    [ITEM_TO_BERRY(ITEM_LIECHI_BERRY) - 1] = {gBerryPic_Liechi, gBerryPalette_Liechi},
    [ITEM_TO_BERRY(ITEM_GANLON_BERRY) - 1] = {gBerryPic_Ganlon, gBerryPalette_Ganlon},
    [ITEM_TO_BERRY(ITEM_SALAC_BERRY)  - 1] = {gBerryPic_Salac,  gBerryPalette_Salac},
    [ITEM_TO_BERRY(ITEM_PETAYA_BERRY) - 1] = {gBerryPic_Petaya, gBerryPalette_Petaya},
    [ITEM_TO_BERRY(ITEM_APICOT_BERRY) - 1] = {gBerryPic_Apicot, gBerryPalette_Apicot},
    [ITEM_TO_BERRY(ITEM_LANSAT_BERRY) - 1] = {gBerryPic_Lansat, gBerryPalette_Lansat},
    [ITEM_TO_BERRY(ITEM_STARF_BERRY)  - 1] = {gBerryPic_Starf,  gBerryPalette_Starf},
    [ITEM_TO_BERRY(ITEM_ENIGMA_BERRY) - 1] = {gBerryPic_Enigma, gBerryPalette_Enigma},
};

const struct CompressedSpriteSheet gBerryCheckCircleSpriteSheet =
{
    gBerryCheckCircle_Gfx, 0x800, TAG_BERRY_CHECK_CIRCLE_GFX
};

const struct CompressedSpritePalette gBerryCheckCirclePaletteTable =
{
    gBerryCheck_Pal, TAG_BERRY_CHECK_CIRCLE_GFX
};

static const struct OamData sBerryCheckCircleOamData =
{
    .y = 0,
    .affineMode = ST_OAM_AFFINE_OFF,
    .objMode = ST_OAM_OBJ_NORMAL,
    .mosaic = 0,
    .bpp = ST_OAM_4BPP,
    .shape = SPRITE_SHAPE(64x64),
    .x = 0,
    .matrixNum = 0,
    .size = SPRITE_SIZE(64x64),
    .tileNum = 0,
    .priority = 1,
    .paletteNum = 0,
    .affineParam = 0
};

static const struct SpriteTemplate gBerryCheckCircleSpriteTemplate =
{
    .tileTag = TAG_BERRY_CHECK_CIRCLE_GFX,
    .paletteTag = TAG_BERRY_CHECK_CIRCLE_GFX,
    .oam = &sBerryCheckCircleOamData,
    .anims = sSpriteAnimTable_ItemIcon,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = SpriteCallbackDummy,
};

static void ArrangeBerryGfx(void *src, void *dest)
{
    u32 i, j;

    memset(dest, 0, 0x800);

    // Create top margin
    dest += 0x100;

    for (i = 0; i < 6; i++)
    {
        // Create left margin
        dest += 0x20;

        // Copy one row of berry's icon
        for (j = 0; j < 6; j++)
        {
            memcpy(dest, src, 0x20);
            dest += 0x20;
            src += 0x20;
        }

        // Create right margin
        if (i != 5)
            dest += 0x20;
    }
}

static void LoadBerryGfx(u8 berryId)
{
    struct CompressedSpritePalette pal;
    pal.data = sBerryPicTable[berryId].pal;
    pal.tag = TAG_BERRY_PIC_PAL;
    LoadCompressedSpritePalette(&pal);
    LZDecompressWram(sBerryPicTable[berryId].tiles, &gDecompressionBuffer[0x1000]);
    ArrangeBerryGfx(&gDecompressionBuffer[0x1000], &gDecompressionBuffer[0]);
}

u8 CreateBerryTagSprite(u8 id, s16 x, s16 y)
{
    LoadBerryGfx(id);
    return CreateSprite(&gBerryPicSpriteTemplate, x, y, 0);
}

void FreeBerryTagSpritePalette(void)
{
    FreeSpritePaletteByTag(TAG_BERRY_PIC_PAL);
}

u8 CreateBerryFlavorCircleSprite(s16 x)
{
    return CreateSprite(&gBerryCheckCircleSpriteTemplate, x, 116, 0);
}
