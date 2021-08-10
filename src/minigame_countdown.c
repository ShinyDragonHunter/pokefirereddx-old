#include "global.h"
#include "decompress.h"
#include "link.h"
#include "link_rfu.h"
#include "sound.h"
#include "task.h"
#include "trig.h"
#include "minigame_countdown.h"
#include "constants/songs.h"

static void Task_MinigameCountdown(u8 taskId);
static bool32 RunMinigameCountdownDigitsAnim(u8 spriteId);
static bool32 IsStartGraphicAnimRunning(u8 spriteId);
static void Load321StartGfx(u16 tileTag, u16 palTag);
static u8 CreateNumberSprite(u16 tileTag, u16 palTag, s16 x, s16 y, u8 subpriority);
static void CreateStartSprite(u16 tileTag, u16 palTag, s16 x, s16 y, u8 subpriority, s16 *spriteId1, s16 *spriteId2);
static void InitStartGraphic(u8 spriteId2, u8 spriteId3);
static void SpriteCB_Start(struct Sprite *sprite);

static const u16 s321Start_Pal[] = INCBIN_U16("graphics/minigame_countdown/321start.gbapal");
static const u32 s321Start_Gfx[] = INCBIN_U32("graphics/minigame_countdown/321start.4bpp.lz");

#define tState       data[0]
#define tTilesTag    data[2]
#define tPalTag      data[3]
#define tX           data[4]
#define tY           data[5]
#define tSubpriority data[6]
#define tSpriteId1   data[7]
#define tSpriteId2   data[8]
#define tSpriteId3   data[9]

void StartMinigameCountdown(u16 tilesTag, u16 palTag, s16 x, s16 y, u8 subpriority)
{
    u8 taskId = CreateTask(Task_MinigameCountdown, 80);
    gTasks[taskId].tTilesTag = tilesTag;
    gTasks[taskId].tPalTag = palTag;
    gTasks[taskId].tX = x;
    gTasks[taskId].tY = y;
    gTasks[taskId].tSubpriority = subpriority;
}

bool32 IsMinigameCountdownRunning(void)
{
    return FuncIsActiveTask(Task_MinigameCountdown);
}

static void Task_MinigameCountdown(u8 taskId)
{
    s16 *data = gTasks[taskId].data;

    switch (tState)
    {
    case 0:
        Load321StartGfx(tTilesTag, tPalTag);
        tSpriteId1 = CreateNumberSprite(tTilesTag, tPalTag, tX, tY, tSubpriority);
        CreateStartSprite(tTilesTag, tPalTag, tX, tY, tSubpriority, &tSpriteId2, &tSpriteId3);
        tState++;
        break;
    case 1:
        if (!RunMinigameCountdownDigitsAnim(tSpriteId1))
        {
            InitStartGraphic(tSpriteId2, tSpriteId3);
            FreeSpriteOamMatrix(&gSprites[tSpriteId1]);
            DestroySprite(&gSprites[tSpriteId1]);
            tState++;
        }
        break;
    case 2:
        if (!IsStartGraphicAnimRunning(tSpriteId2))
        {
            DestroySprite(&gSprites[tSpriteId2]);
            DestroySprite(&gSprites[tSpriteId3]);
            FreeSpriteTilesByTag(tTilesTag);
            FreeSpritePaletteByTag(tPalTag);
            DestroyTask(taskId);
        }
        break;
    }
}

#define sState   data[0]
#define sTimer   data[2]
#define sAnimNum data[4]

static bool32 RunMinigameCountdownDigitsAnim(u8 spriteId)
{
    struct Sprite *sprite = &gSprites[spriteId];

    switch (sprite->sState)
    {
    case 0:
        sub_8007E18(sprite, 0x800, 0x1A);
        sprite->sState++;
        // fallthrough
    case 1:
        if (sprite->sTimer == 0)
            PlaySE(SE_BALL_BOUNCE_2);
        
        if (++sprite->sTimer >= 20)
        {
            // Ready for jump
            sprite->sTimer = 0;
            StartSpriteAffineAnim(sprite, 1);
            sprite->sState++;
        }
        break;
    case 2:
        if (sprite->affineAnimEnded)
            sprite->sState++;
        break;
    case 3:
        if (++sprite->sTimer >= 4)
        {
            // Start jump
            sprite->sTimer = 0;
            sprite->sState++;
            StartSpriteAffineAnim(sprite, 2);
        }
        break;
    case 4:
        // Moving up from jump
        sprite->y -= 4;
        if (++sprite->sTimer >= 8)
        {
            if (sprite->sAnimNum < 2)
            {
                // Swap to the next number
                StartSpriteAnim(sprite, sprite->sAnimNum + 1);
                sprite->sTimer = 0;
                sprite->sState++;
            }
            else
            {
                // End
                sprite->sState = 7;
                return FALSE;
            }
        }
        break;
    case 5:
        // Falling after jump
        sprite->y += 4;
        if (++sprite->sTimer >= 8)
        {
            // Land from jump
            sprite->sTimer = 0;
            StartSpriteAffineAnim(sprite, 3);
            sprite->sState++;
        }
        break;
    case 6:
        if (sprite->affineAnimEnded)
        {
            // Restart for next number
            sprite->sAnimNum++;
            sprite->sState = 1;
        }
        break;
    case 7:
        return FALSE;
    }

    return TRUE;
}

#undef sState
#undef sTimer
#undef sAnimNum

static void InitStartGraphic(u8 spriteId2, u8 spriteId3)
{
    gSprites[spriteId2].y2 = -40;
    gSprites[spriteId3].y2 = -40;
    gSprites[spriteId2].invisible = FALSE;
    gSprites[spriteId3].invisible = FALSE;
    gSprites[spriteId2].callback = SpriteCB_Start;
    gSprites[spriteId3].callback = SpriteCB_Start;
}

static bool32 IsStartGraphicAnimRunning(u8 spriteId)
{
    return gSprites[spriteId].callback == SpriteCB_Start;
}

#define sState  data[0]
#define sTimer  data[1]
#define sYSpeed data[4]
#define sY      data[5]

static void SpriteCB_Start(struct Sprite *sprite)
{
    int y;
    s16 *data = sprite->data;

    switch (sState)
    {
    case 0:
        sYSpeed = 64;
        sY = sprite->y2 << 4;
        sState++;
    case 1:
        sY += sYSpeed;
        sYSpeed++;
        sprite->y2 = sY >> 4;
        if (sprite->y2 >= 0)
        {
            PlaySE(SE_BALL_BOUNCE_2);
            sprite->y2 = 0;
            sState++;
        }
        break;
    case 2:
        sTimer += 12;
        if (sTimer >= 128)
        {
            PlaySE(SE_BALL_BOUNCE_2);
            sTimer = 0;
            sState++;
        }
        y = gSineTable[sTimer];
        sprite->y2 = -(y >> 4);
        break;
    case 3:
        sTimer += 16;
        if (sTimer >= 128)
        {
            PlaySE(SE_BALL_BOUNCE_2);
            sTimer = 0;
            sState++;
        }
        sprite->y2 = -(gSineTable[sTimer] >> 5);
        break;
    case 4:
        if (++sTimer > 40)
            sprite->callback = SpriteCallbackDummy;
        break;
    }
}

#undef sState
#undef sTimer
#undef sYSpeed
#undef sY

static void Load321StartGfx(u16 tileTag, u16 palTag)
{
    struct CompressedSpriteSheet spriteSheet = {s321Start_Gfx, 0xE00, 0};
    struct SpritePalette spritePalette = {s321Start_Pal, 0};

    spriteSheet.tag = tileTag;
    spritePalette.tag = palTag;

    LoadCompressedSpriteSheet(&spriteSheet);
    LoadSpritePalette(&spritePalette);
}

static const struct OamData sOamData_Numbers =
{
    .y = 0,
    .affineMode = ST_OAM_AFFINE_DOUBLE,
    .objMode = ST_OAM_OBJ_NORMAL,
    .mosaic = 0,
    .bpp = ST_OAM_4BPP,
    .shape = SPRITE_SHAPE(32x32),
    .x = 0,
    .matrixNum = 0,
    .size = SPRITE_SIZE(32x32),
    .tileNum = 0,
    .priority = 0,
    .paletteNum = 0,
    .affineParam = 0
};

static const struct OamData sOamData_Start =
{
    .y = 0,
    .affineMode = ST_OAM_AFFINE_OFF,
    .objMode = ST_OAM_OBJ_NORMAL,
    .mosaic = 0,
    .bpp = ST_OAM_4BPP,
    .shape = SPRITE_SHAPE(64x32),
    .x = 0,
    .matrixNum = 0,
    .size = SPRITE_SIZE(64x32),
    .tileNum = 0,
    .priority = 0,
    .paletteNum = 0,
    .affineParam = 0
};

static const union AnimCmd sAnim_Numbers_Three[] =
{
    ANIMCMD_FRAME(0, 1),
    ANIMCMD_END
};

static const union AnimCmd sAnim_Numbers_Two[] =
{
    ANIMCMD_FRAME(16, 1),
    ANIMCMD_END
};

static const union AnimCmd sAnim_Numbers_One[] =
{
    ANIMCMD_FRAME(32, 1),
    ANIMCMD_END
};

static const union AnimCmd *const sAnimTable_Numbers[] =
{
    sAnim_Numbers_Three,
    sAnim_Numbers_Two,
    sAnim_Numbers_One
};

static const union AnimCmd sAnim_StartLeft[] =
{
    ANIMCMD_FRAME(48, 1),
    ANIMCMD_END
};

static const union AnimCmd sAnim_StartRight[] =
{
    ANIMCMD_FRAME(80, 1),
    ANIMCMD_END
};

static const union AnimCmd *const sAnimTable_Start[] =
{
    sAnim_StartLeft,
    sAnim_StartRight
};

static const union AffineAnimCmd sAffineAnim_Numbers_Normal[] =
{
    AFFINEANIMCMD_FRAME(256, 256, 0, 0),
    AFFINEANIMCMD_END
};

static const union AffineAnimCmd sAffineAnim_Numbers_Squash[] =
{
    AFFINEANIMCMD_FRAME(256, 256, 0, 0),
    AFFINEANIMCMD_FRAME(16, -16, 0, 8),
    AFFINEANIMCMD_END
};

static const union AffineAnimCmd sAffineAnim_Numbers_Stretch[] =
{
    AFFINEANIMCMD_FRAME(-18, 18, 0, 8),
    AFFINEANIMCMD_END
};

static const union AffineAnimCmd sAffineAnim_Numbers_Land[] =
{
    AFFINEANIMCMD_FRAME(  6,  -6, 0, 8),
    AFFINEANIMCMD_FRAME( -4,   4, 0, 8),
    AFFINEANIMCMD_FRAME(256, 256, 0, 0),
    AFFINEANIMCMD_END
};

static const union AffineAnimCmd *const sAffineAnimTable_Numbers[] =
{
    sAffineAnim_Numbers_Normal,
    sAffineAnim_Numbers_Squash,  // Push down before the jump
    sAffineAnim_Numbers_Stretch, // Stretch up for the jump
    sAffineAnim_Numbers_Land     // Land after the jump
};

static u8 CreateNumberSprite(u16 tileTag, u16 palTag, s16 x, s16 y, u8 subpriority)
{
    struct SpriteTemplate spriteTemplate =
    {
        .oam = &sOamData_Numbers,
        .anims = sAnimTable_Numbers,
        .affineAnims = sAffineAnimTable_Numbers,
        .callback = SpriteCallbackDummy,
    };

    spriteTemplate.tileTag = tileTag;
    spriteTemplate.paletteTag = palTag;
    return CreateSprite(&spriteTemplate, x, y, subpriority);
}

static void CreateStartSprite(u16 tileTag, u16 palTag, s16 x, s16 y, u8 subpriority, s16 *spriteId1, s16 *spriteId2)
{
    struct SpriteTemplate spriteTemplate =
    {
        .oam = &sOamData_Start,
        .anims = sAnimTable_Start,
        .affineAnims = gDummySpriteAffineAnimTable,
        .callback = SpriteCallbackDummy,
    };

    spriteTemplate.tileTag = tileTag;
    spriteTemplate.paletteTag = palTag;
    *spriteId1 = CreateSprite(&spriteTemplate, x - 32, y, subpriority);
    *spriteId2 = CreateSprite(&spriteTemplate, x + 32, y, subpriority);

    gSprites[*spriteId1].invisible = TRUE;
    gSprites[*spriteId2].invisible = TRUE;
    StartSpriteAnim(&gSprites[*spriteId2], 1);
}
