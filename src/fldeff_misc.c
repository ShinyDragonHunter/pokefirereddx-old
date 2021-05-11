#include "global.h"
#include "gpu_regs.h"
#include "palette.h"
#include "script.h"
#include "sound.h"
#include "task.h"
#include "strings.h"
#include "party_menu.h"
#include "fieldmap.h"
#include "field_effect.h"
#include "field_camera.h"
#include "field_player_avatar.h"
#include "fldeff.h"
#include "fldeff_misc.h"
#include "event_data.h"
#include "event_scripts.h"
#include "event_object_movement.h"
#include "metatile_behavior.h"
#include "string_util.h"
#include "constants/field_effects.h"
#include "constants/metatile_behaviors.h"
#include "constants/metatile_labels.h"
#include "constants/songs.h"


EWRAM_DATA struct MapPosition gPlayerFacingPosition = {0};

static void Task_ComputerScreenOpenEffect(u8);
static void Task_ComputerScreenCloseEffect(u8);
static void CreateComputerScreenEffectTask(TaskFunc, u16, u16, u8);

static void Task_WateringBerryTreeAnim_Start(u8);
static void Task_WateringBerryTreeAnim_Continue(u8);
static void Task_WateringBerryTreeAnim_End(u8);

static const u8 sRecordMixLights_Gfx[] = INCBIN_U8("graphics/field_effects/pics/record_mix_lights.4bpp");
static const u16 sRecordMixLights_Pal[] = INCBIN_U16("graphics/field_effects/palettes/record_mix_lights.gbapal");

static const struct SpriteFrameImage sPicTable_RecordMixLights[] =
{
    overworld_frame(sRecordMixLights_Gfx, 4, 1, 0),
    overworld_frame(sRecordMixLights_Gfx, 4, 1, 1),
    overworld_frame(sRecordMixLights_Gfx, 4, 1, 2),
};

static const struct SpritePalette sSpritePalette_RecordMixLights = {sRecordMixLights_Pal, 0x1000};

static const union AnimCmd sAnim_RecordMixLights[] =
{
    ANIMCMD_FRAME(0, 30),
    ANIMCMD_FRAME(1, 30),
    ANIMCMD_FRAME(2, 30),
    ANIMCMD_JUMP(0),
};

static const union AnimCmd *const sAnimTable_RecordMixLights[] =
{
    sAnim_RecordMixLights,
};

static const struct SpriteTemplate sSpriteTemplate_RecordMixLights =
{
    .tileTag = 0xFFFF,
    .paletteTag = 0x1000,
    .oam = &gObjectEventBaseOam_32x8,
    .anims = sAnimTable_RecordMixLights,
    .images = sPicTable_RecordMixLights,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = SpriteCallbackDummy,
};

// For accessing pokemon storage PC or the Hall of Fame PC
void ComputerScreenOpenEffect(u16 increment, u16 unused, u8 priority)
{
    CreateComputerScreenEffectTask(Task_ComputerScreenOpenEffect, increment, unused, priority);
}

void ComputerScreenCloseEffect(u16 increment, u16 unused, u8 priority)
{
    CreateComputerScreenEffectTask(Task_ComputerScreenCloseEffect, increment, unused, priority);
}

bool8 IsComputerScreenOpenEffectActive(void)
{
    return FuncIsActiveTask(Task_ComputerScreenOpenEffect);
}

bool8 IsComputerScreenCloseEffectActive(void)
{
    return FuncIsActiveTask(Task_ComputerScreenCloseEffect);
}

#define tState         data[0]
#define tHorzIncrement data[1]
#define tVertIncrement data[2]
#define tWinLeft       data[3]
#define tWinRight      data[4]
#define tWinTop        data[5]
#define tWinBottom     data[6]
#define tBlendCnt      data[7]
#define tBlendY        data[8]

static void CreateComputerScreenEffectTask(void (*taskfunc) (u8), u16 increment, u16 unused, u8 priority)
{
    u8 taskId = CreateTask(taskfunc, priority);

    gTasks[taskId].tState = 0;
    gTasks[taskId].tHorzIncrement = increment == 0 ? 16 : increment;
    gTasks[taskId].tVertIncrement = increment == 0 ? 20 : increment;
    gTasks[taskId].func(taskId);
}

static void Task_ComputerScreenOpenEffect(u8 taskId)
{
    struct Task *task = &gTasks[taskId];

    switch (task->tState)
    {
    case 0:
        task->tWinLeft = DISPLAY_WIDTH / 2;
        task->tWinRight = DISPLAY_WIDTH / 2;
        task->tWinTop = DISPLAY_HEIGHT / 2;
        task->tWinBottom = DISPLAY_HEIGHT / 2 + 1;

        SetGpuRegBits(REG_OFFSET_DISPCNT, DISPCNT_WIN0_ON);
        SetGpuReg(REG_OFFSET_WIN0H, WIN_RANGE(task->tWinLeft, task->tWinRight));
        SetGpuReg(REG_OFFSET_WIN0V, WIN_RANGE(task->tWinTop, task->tWinBottom));
        SetGpuReg(REG_OFFSET_WININ, WININ_WIN0_BG_ALL | WININ_WIN0_OBJ | WININ_WIN0_CLR);
        SetGpuReg(REG_OFFSET_WINOUT, 0);

        break;
    case 1:
        task->tBlendCnt = GetGpuReg(REG_OFFSET_BLDCNT);
        task->tBlendY = GetGpuReg(REG_OFFSET_BLDY);

        SetGpuReg(REG_OFFSET_BLDCNT, BLDCNT_TGT1_ALL | BLDCNT_EFFECT_LIGHTEN);
        SetGpuReg(REG_OFFSET_BLDY, 16);

        break;
    case 2:
        task->tWinLeft -= task->tHorzIncrement;
        task->tWinRight += task->tHorzIncrement;

        if (task->tWinLeft < 1 || task->tWinRight > DISPLAY_WIDTH - 1)
        {
            task->tWinLeft = 0;
            task->tWinRight = DISPLAY_WIDTH;
            SetGpuReg(REG_OFFSET_BLDY, 0);
            SetGpuReg(REG_OFFSET_BLDCNT, task->tBlendCnt);
            BlendPalettes(PALETTES_ALL, 0, 0);
            gPlttBufferFaded[0] = 0;
        }
        SetGpuReg(REG_OFFSET_WIN0H, WIN_RANGE(task->tWinLeft, task->tWinRight));

        if (task->tWinLeft)
            return;
        break;
    case 3:
        task->tWinTop -= task->tVertIncrement;
        task->tWinBottom += task->tVertIncrement;

        if (task->tWinTop < 1 || task->tWinBottom > DISPLAY_HEIGHT - 1)
        {
            task->tWinTop = 0;
            task->tWinBottom = DISPLAY_HEIGHT;
            ClearGpuRegBits(REG_OFFSET_DISPCNT, DISPCNT_WIN0_ON);
        }
        SetGpuReg(REG_OFFSET_WIN0V, WIN_RANGE(task->tWinTop, task->tWinBottom));

        if (task->tWinTop != 0)
            return;
        break;
    default:
        SetGpuReg(REG_OFFSET_BLDCNT, task->tBlendCnt);
        DestroyTask(taskId);
        return;
    }
    task->tState++;
}

static void Task_ComputerScreenCloseEffect(u8 taskId)
{
    struct Task *task = &gTasks[taskId];

    switch (task->tState)
    {
    case 0:
        gPlttBufferFaded[0] = 0;
        break;
    case 1:
        task->tWinLeft = 0;
        task->tWinRight = DISPLAY_WIDTH;
        task->tWinTop = 0;
        task->tWinBottom = DISPLAY_HEIGHT;

        SetGpuRegBits(REG_OFFSET_DISPCNT, DISPCNT_WIN0_ON);
        SetGpuReg(REG_OFFSET_WIN0H, WIN_RANGE(task->tWinLeft, task->tWinRight));
        SetGpuReg(REG_OFFSET_WIN0V, WIN_RANGE(task->tWinTop, task->tWinBottom));
        SetGpuReg(REG_OFFSET_WININ, WININ_WIN0_BG_ALL | WININ_WIN0_OBJ | WININ_WIN0_CLR);
        SetGpuReg(REG_OFFSET_WINOUT, 0);
        break;
    case 2:
        task->tWinTop += task->tVertIncrement;
        task->tWinBottom -= task->tVertIncrement;

        if (task->tWinTop >= DISPLAY_HEIGHT / 2 || task->tWinBottom <= DISPLAY_HEIGHT / 2 + 1)
        {
            task->tWinTop = DISPLAY_HEIGHT / 2;
            task->tWinBottom = DISPLAY_HEIGHT / 2 + 1;
            SetGpuReg(REG_OFFSET_BLDCNT, BLDCNT_TGT1_ALL | BLDCNT_EFFECT_LIGHTEN);
            SetGpuReg(REG_OFFSET_BLDY, 16);
        }
        SetGpuReg(REG_OFFSET_WIN0V, WIN_RANGE(task->tWinTop, task->tWinBottom));

        if (task->tWinTop != DISPLAY_HEIGHT / 2)
            return;
        break;
    case 3:
        task->tWinLeft += task->tHorzIncrement;
        task->tWinRight -= task->tHorzIncrement;

        if (task->tWinLeft >= DISPLAY_WIDTH / 2 || task->tWinRight <= DISPLAY_WIDTH / 2)
        {
            task->tWinLeft = DISPLAY_WIDTH / 2;
            task->tWinRight = DISPLAY_WIDTH / 2;
            BlendPalettes(PALETTES_ALL, 16, 0);
            gPlttBufferFaded[0] = 0;
        }
        SetGpuReg(REG_OFFSET_WIN0H, WIN_RANGE(task->tWinLeft, task->tWinRight));

        if (task->tWinLeft != DISPLAY_WIDTH / 2)
            return;
        break;
    default:
        ClearGpuRegBits(REG_OFFSET_DISPCNT, DISPCNT_WIN0_ON);
        SetGpuReg(REG_OFFSET_BLDY, 0);
        SetGpuReg(REG_OFFSET_BLDCNT, 0);
        DestroyTask(taskId);
        return;
    }
    task->tState++;
}

#undef tState
#undef tHorzIncrement
#undef tVertIncrement
#undef tWinLeft
#undef tWinRight
#undef tWinTop
#undef tWinBottom
#undef tBlendCnt
#undef tBlendY

static void Task_FieldPoisonEffect(u8 taskId)
{
    s16 *data = gTasks[taskId].data;

    switch (data[0])
    {
    case 0:
        data[1] += 2;
        if (data[1] > 8)
            data[0]++;
        break;
    case 1:
        data[1] -= 2;
        if (!data[1])
            data[0]++;
        break;
    case 2:
        DestroyTask(taskId);
        return;
    }
    SetGpuReg(REG_OFFSET_MOSAIC, (data[1] << 4) | data[1]);
}

void FldEffPoison_Start(void)
{
    PlaySE(SE_FIELD_POISON);
    CreateTask(Task_FieldPoisonEffect, 80);
}

bool32 FldEffPoison_IsActive(void)
{
    return FuncIsActiveTask(Task_FieldPoisonEffect);
}

static void Task_WateringBerryTreeAnim(u8 taskId)
{
    gTasks[taskId].func = Task_WateringBerryTreeAnim_Start;
}

static void Task_WateringBerryTreeAnim_Start(u8 taskId)
{
    struct ObjectEvent *playerObjEvent = &gObjectEvents[gPlayerAvatar.objectEventId];

    if (!ObjectEventIsMovementOverridden(playerObjEvent)
        || ObjectEventClearHeldMovementIfFinished(playerObjEvent))
    {
        // Start watering
        SetPlayerAvatarWatering(GetPlayerFacingDirection());
        ObjectEventSetHeldMovement(playerObjEvent, GetWalkInPlaceNormalMovementAction(GetPlayerFacingDirection()));
        gTasks[taskId].func = Task_WateringBerryTreeAnim_Continue;
    }
}

static void Task_WateringBerryTreeAnim_Continue(u8 taskId)
{
    struct ObjectEvent *playerObjEvent = &gObjectEvents[gPlayerAvatar.objectEventId];

    if (ObjectEventClearHeldMovementIfFinished(playerObjEvent))
    {
        s16 value = gTasks[taskId].data[1]++;

        // Continue holding watering action 10 times, then end
        if (value < 10)
            ObjectEventSetHeldMovement(playerObjEvent, GetWalkInPlaceNormalMovementAction(GetPlayerFacingDirection()));
        else
            gTasks[taskId].func = Task_WateringBerryTreeAnim_End;
    }
}

static void Task_WateringBerryTreeAnim_End(u8 taskId)
{
    SetPlayerAvatarTransitionFlags(GetPlayerAvatarFlags());
    DestroyTask(taskId);
    EnableBothScriptContexts();
}

void DoWateringBerryTreeAnim(void)
{
    CreateTask(Task_WateringBerryTreeAnim, 80);
}

// The lights that blink on the counter when mixing records in the cable club
u8 CreateRecordMixingLights(void)
{
    u8 spriteId;

    LoadSpritePalette(&sSpritePalette_RecordMixLights);

    spriteId = CreateSprite(&sSpriteTemplate_RecordMixLights, 0, 0, 82);

    if (spriteId == MAX_SPRITES)
    {
        return MAX_SPRITES;
    }
    else
    {
        struct Sprite *sprite = &gSprites[spriteId];
        GetMapCoordsFromSpritePos(16, 13, &sprite->pos1.x, &sprite->pos1.y);
        sprite->coordOffsetEnabled = TRUE;
        sprite->pos1.x += 16;
        sprite->pos1.y += 2;
    }
    return spriteId;
}

void DestroyRecordMixingLights(void)
{
    int i;

    for (i = 0; i < MAX_SPRITES; i++)
    {
        if (gSprites[i].template == &sSpriteTemplate_RecordMixLights)
        {
            FreeSpritePalette(&gSprites[i]);
            DestroySprite(&gSprites[i]);
        }
    }
}
