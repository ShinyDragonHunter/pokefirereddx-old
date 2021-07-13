#include "global.h"
#include "battle.h"
#include "battle_anim.h"
#include "battle_bg.h"
#include "battle_controllers.h"
#include "battle_interface.h"
#include "bg.h"
#include "decompress.h"
#include "dma3.h"
#include "gpu_regs.h"
#include "graphics.h"
#include "main.h"
#include "m4a.h"
#include "palette.h"
#include "pokemon.h"
#include "sound.h"
#include "sprite.h"
#include "task.h"
#include "constants/battle_anim.h"
#include "constants/moves.h"

#define ANIM_SPRITE_INDEX_COUNT 8

extern struct MusicPlayerInfo gMPlayInfo_BGM;
extern struct MusicPlayerInfo gMPlayInfo_SE1;
extern struct MusicPlayerInfo gMPlayInfo_SE2;

extern const u16 gMovesWithQuietBGM[];
extern const u8 *const gBattleAnims_Moves[];

// this file's functions
static void ScriptCmd_loadspritegfx(void);
static void ScriptCmd_unloadspritegfx(void);
static void ScriptCmd_createsprite(void);
static void ScriptCmd_createvisualtask(void);
static void ScriptCmd_delay(void);
static void ScriptCmd_waitforvisualfinish(void);
static void ScriptCmd_hang(void);
static void ScriptCmd_end(void);
static void ScriptCmd_playse(void);
static void ScriptCmd_monbg(void);
static void ScriptCmd_clearmonbg(void);
static void ScriptCmd_setalpha(void);
static void ScriptCmd_blendoff(void);
static void ScriptCmd_call(void);
static void ScriptCmd_return(void);
static void ScriptCmd_setarg(void);
static void ScriptCmd_choosetwoturnanim(void);
static void ScriptCmd_jumpifmoveturn(void);
static void ScriptCmd_goto(void);
static void ScriptCmd_fadetobg(void);
static void ScriptCmd_restorebg(void);
static void ScriptCmd_waitbgfadeout(void);
static void ScriptCmd_waitbgfadein(void);
static void ScriptCmd_changebg(void);
static void ScriptCmd_playsewithpan(void);
static void ScriptCmd_setpan(void);
static void ScriptCmd_panse_1B(void);
static void ScriptCmd_loopsewithpan(void);
static void ScriptCmd_waitplaysewithpan(void);
static void ScriptCmd_setbldcnt(void);
static void ScriptCmd_createsoundtask(void);
static void ScriptCmd_waitsound(void);
static void ScriptCmd_jumpargeq(void);
static void ScriptCmd_monbg_22(void);
static void ScriptCmd_clearmonbg_23(void);
static void ScriptCmd_fadetobgfromset(void);
static void ScriptCmd_panse_26(void);
static void ScriptCmd_panse_27(void);
static void ScriptCmd_monbgprio_28(void);
static void ScriptCmd_monbgprio_29(void);
static void ScriptCmd_monbgprio_2A(void);
static void ScriptCmd_invisible(void);
static void ScriptCmd_visible(void);
static void ScriptCmd_doublebattle_2D(void);
static void ScriptCmd_doublebattle_2E(void);
static void ScriptCmd_stopsound(void);

static void RunAnimScriptCommand(void);
static void task_pA_ma0A_obj_to_bg_pal(u8 taskId);
static void sub_80A4980(u8 taskId);
static void sub_80A4BB0(u8 taskId);
static void Task_FadeToBg(u8 taskId);
static void Task_PanFromInitialToTarget(u8 taskId);
static void Task_LoopAndPlaySE(u8 taskId);
static void Task_WaitAndPlaySE(u8 taskId);
static void LoadDefaultBg(void);
static void LoadMoveBg(u16 bgId);

// ewram
EWRAM_DATA static const u8 *sBattleAnimScriptPtr = NULL;
EWRAM_DATA static const u8 *sBattleAnimScriptRetAddr = NULL;
EWRAM_DATA void (*gAnimScriptCallback)(void) = NULL;
EWRAM_DATA static s8 gAnimFramesToWait = 0;
EWRAM_DATA bool8 gAnimScriptActive = FALSE;
EWRAM_DATA u8 gAnimVisualTaskCount = 0;
EWRAM_DATA u8 gAnimSoundTaskCount = 0;
EWRAM_DATA struct DisableStruct *gAnimDisableStructPtr = NULL;
EWRAM_DATA s32 gAnimMoveDmg = 0;
EWRAM_DATA u16 gAnimMovePower = 0;
EWRAM_DATA static u16 sAnimSpriteIndexArray[ANIM_SPRITE_INDEX_COUNT] = {0};
EWRAM_DATA u8 gAnimFriendship = 0;
EWRAM_DATA u16 gWeatherMoveAnim = 0;
EWRAM_DATA s16 gBattleAnimArgs[ANIM_ARGS_COUNT] = {0};
EWRAM_DATA static u16 sSoundAnimFramesToWait = 0;
EWRAM_DATA static u8 sMonAnimTaskIdArray[2] = {0};
EWRAM_DATA u8 gAnimMoveTurn = 0;
EWRAM_DATA static u8 sAnimBackgroundFadeState = 0;
EWRAM_DATA static u16 sAnimMoveIndex = 0; // Set but unused.
EWRAM_DATA u8 gBattleAnimAttacker = 0;
EWRAM_DATA u8 gBattleAnimTarget = 0;
EWRAM_DATA u16 gAnimBattlerSpecies[MAX_BATTLERS_COUNT] = {0};
EWRAM_DATA u8 gAnimCustomPanning = 0;

const struct OamData gOamData_AffineOff_ObjNormal_8x8 =
{
    .y = 0,
    .affineMode = ST_OAM_AFFINE_OFF,
    .objMode = ST_OAM_OBJ_NORMAL,
    .bpp = ST_OAM_4BPP,
    .shape = SPRITE_SHAPE(8x8),
    .x = 0,
    .size = SPRITE_SIZE(8x8),
    .tileNum = 0,
    .priority = 2,
    .paletteNum = 0,
};


const struct OamData gOamData_AffineOff_ObjNormal_16x16 =
{
    .y = 0,
    .affineMode = ST_OAM_AFFINE_OFF,
    .objMode = ST_OAM_OBJ_NORMAL,
    .bpp = ST_OAM_4BPP,
    .shape = SPRITE_SHAPE(16x16),
    .x = 0,
    .size = SPRITE_SIZE(16x16),
    .tileNum = 0,
    .priority = 2,
    .paletteNum = 0,
};

const struct OamData gOamData_AffineOff_ObjNormal_32x32 =
{
    .y = 0,
    .affineMode = ST_OAM_AFFINE_OFF,
    .objMode = ST_OAM_OBJ_NORMAL,
    .bpp = ST_OAM_4BPP,
    .shape = SPRITE_SHAPE(32x32),
    .x = 0,
    .size = SPRITE_SIZE(32x32),
    .tileNum = 0,
    .priority = 2,
    .paletteNum = 0,
};

const struct OamData gOamData_AffineOff_ObjNormal_64x64 =
{
    .y = 0,
    .affineMode = ST_OAM_AFFINE_OFF,
    .objMode = ST_OAM_OBJ_NORMAL,
    .bpp = ST_OAM_4BPP,
    .shape = SPRITE_SHAPE(64x64),
    .x = 0,
    .size = SPRITE_SIZE(64x64),
    .tileNum = 0,
    .priority = 2,
    .paletteNum = 0,
};

const struct OamData gOamData_AffineOff_ObjNormal_16x8 =
{
    .y = 0,
    .affineMode = ST_OAM_AFFINE_OFF,
    .objMode = ST_OAM_OBJ_NORMAL,
    .bpp = ST_OAM_4BPP,
    .shape = SPRITE_SHAPE(16x8),
    .x = 0,
    .size = SPRITE_SIZE(16x8),
    .tileNum = 0,
    .priority = 2,
    .paletteNum = 0,
};

const struct OamData gOamData_AffineOff_ObjNormal_32x8 =
{
    .y = 0,
    .affineMode = ST_OAM_AFFINE_OFF,
    .objMode = ST_OAM_OBJ_NORMAL,
    .bpp = ST_OAM_4BPP,
    .shape = SPRITE_SHAPE(32x8),
    .x = 0,
    .size = SPRITE_SIZE(32x8),
    .tileNum = 0,
    .priority = 2,
    .paletteNum = 0,
};

const struct OamData gOamData_AffineOff_ObjNormal_32x16 =
{
    .y = 0,
    .affineMode = ST_OAM_AFFINE_OFF,
    .objMode = ST_OAM_OBJ_NORMAL,
    .bpp = ST_OAM_4BPP,
    .shape = SPRITE_SHAPE(32x16),
    .x = 0,
    .size = SPRITE_SIZE(32x16),
    .tileNum = 0,
    .priority = 2,
    .paletteNum = 0,
};

const struct OamData gOamData_AffineOff_ObjNormal_64x32 =
{
    .y = 0,
    .affineMode = ST_OAM_AFFINE_OFF,
    .objMode = ST_OAM_OBJ_NORMAL,
    .bpp = ST_OAM_4BPP,
    .shape = SPRITE_SHAPE(64x32),
    .x = 0,
    .size = SPRITE_SIZE(64x32),
    .tileNum = 0,
    .priority = 2,
    .paletteNum = 0,
};

const struct OamData gOamData_AffineOff_ObjNormal_8x16 =
{
    .y = 0,
    .affineMode = ST_OAM_AFFINE_OFF,
    .objMode = ST_OAM_OBJ_NORMAL,
    .bpp = ST_OAM_4BPP,
    .shape = SPRITE_SHAPE(8x16),
    .x = 0,
    .size = SPRITE_SIZE(8x16),
    .tileNum = 0,
    .priority = 2,
    .paletteNum = 0,
};

const struct OamData gOamData_AffineOff_ObjNormal_8x32 =
{
    .y = 0,
    .affineMode = ST_OAM_AFFINE_OFF,
    .objMode = ST_OAM_OBJ_NORMAL,
    .bpp = ST_OAM_4BPP,
    .shape = SPRITE_SHAPE(8x32),
    .x = 0,
    .size = SPRITE_SIZE(8x32),
    .tileNum = 0,
    .priority = 2,
    .paletteNum = 0,
};

const struct OamData gOamData_AffineOff_ObjNormal_16x32 =
{
    .y = 0,
    .affineMode = ST_OAM_AFFINE_OFF,
    .objMode = ST_OAM_OBJ_NORMAL,
    .bpp = ST_OAM_4BPP,
    .shape = SPRITE_SHAPE(16x32),
    .x = 0,
    .size = SPRITE_SIZE(16x32),
    .tileNum = 0,
    .priority = 2,
    .paletteNum = 0,
};

const struct OamData gOamData_AffineOff_ObjNormal_32x64 =
{
    .y = 0,
    .affineMode = ST_OAM_AFFINE_OFF,
    .objMode = ST_OAM_OBJ_NORMAL,
    .bpp = ST_OAM_4BPP,
    .shape = SPRITE_SHAPE(32x64),
    .x = 0,
    .size = SPRITE_SIZE(32x64),
    .tileNum = 0,
    .priority = 2,
    .paletteNum = 0,
};

const struct OamData gOamData_AffineNormal_ObjNormal_8x8 =
{
    .y = 0,
    .affineMode = ST_OAM_AFFINE_NORMAL,
    .objMode = ST_OAM_OBJ_NORMAL,
    .bpp = ST_OAM_4BPP,
    .shape = SPRITE_SHAPE(8x8),
    .x = 0,
    .size = SPRITE_SIZE(8x8),
    .tileNum = 0,
    .priority = 2,
    .paletteNum = 0,
};

const struct OamData gOamData_AffineNormal_ObjNormal_16x16 =
{
    .y = 0,
    .affineMode = ST_OAM_AFFINE_NORMAL,
    .objMode = ST_OAM_OBJ_NORMAL,
    .bpp = ST_OAM_4BPP,
    .shape = SPRITE_SHAPE(16x16),
    .x = 0,
    .size = SPRITE_SIZE(16x16),
    .tileNum = 0,
    .priority = 2,
    .paletteNum = 0,
};

const struct OamData gOamData_AffineNormal_ObjNormal_32x32 =
{
    .y = 0,
    .affineMode = ST_OAM_AFFINE_NORMAL,
    .objMode = ST_OAM_OBJ_NORMAL,
    .bpp = ST_OAM_4BPP,
    .shape = SPRITE_SHAPE(32x32),
    .x = 0,
    .size = SPRITE_SIZE(32x32),
    .tileNum = 0,
    .priority = 2,
    .paletteNum = 0,
};

const struct OamData gOamData_AffineNormal_ObjNormal_64x64 =
{
    .y = 0,
    .affineMode = ST_OAM_AFFINE_NORMAL,
    .objMode = ST_OAM_OBJ_NORMAL,
    .bpp = ST_OAM_4BPP,
    .shape = SPRITE_SHAPE(64x64),
    .x = 0,
    .size = SPRITE_SIZE(64x64),
    .tileNum = 0,
    .priority = 2,
    .paletteNum = 0,
};

const struct OamData gOamData_AffineNormal_ObjNormal_16x8 =
{
    .y = 0,
    .affineMode = ST_OAM_AFFINE_NORMAL,
    .objMode = ST_OAM_OBJ_NORMAL,
    .bpp = ST_OAM_4BPP,
    .shape = SPRITE_SHAPE(16x8),
    .x = 0,
    .size = SPRITE_SIZE(16x8),
    .tileNum = 0,
    .priority = 2,
    .paletteNum = 0,
};

const struct OamData gOamData_AffineNormal_ObjNormal_32x8 =
{
    .y = 0,
    .affineMode = ST_OAM_AFFINE_NORMAL,
    .objMode = ST_OAM_OBJ_NORMAL,
    .bpp = ST_OAM_4BPP,
    .shape = SPRITE_SHAPE(32x8),
    .x = 0,
    .size = SPRITE_SIZE(32x8),
    .tileNum = 0,
    .priority = 2,
    .paletteNum = 0,
};

const struct OamData gOamData_AffineNormal_ObjNormal_32x16 =
{
    .y = 0,
    .affineMode = ST_OAM_AFFINE_NORMAL,
    .objMode = ST_OAM_OBJ_NORMAL,
    .bpp = ST_OAM_4BPP,
    .shape = SPRITE_SHAPE(32x16),
    .x = 0,
    .size = SPRITE_SIZE(32x16),
    .tileNum = 0,
    .priority = 2,
    .paletteNum = 0,
};

const struct OamData gOamData_AffineNormal_ObjNormal_64x32 =
{
    .y = 0,
    .affineMode = ST_OAM_AFFINE_NORMAL,
    .objMode = ST_OAM_OBJ_NORMAL,
    .bpp = ST_OAM_4BPP,
    .shape = SPRITE_SHAPE(64x32),
    .x = 0,
    .size = SPRITE_SIZE(64x32),
    .tileNum = 0,
    .priority = 2,
    .paletteNum = 0,
};

const struct OamData gOamData_AffineNormal_ObjNormal_8x16 =
{
    .y = 0,
    .affineMode = ST_OAM_AFFINE_NORMAL,
    .objMode = ST_OAM_OBJ_NORMAL,
    .bpp = ST_OAM_4BPP,
    .shape = SPRITE_SHAPE(8x16),
    .x = 0,
    .size = SPRITE_SIZE(8x16),
    .tileNum = 0,
    .priority = 2,
    .paletteNum = 0,
};

const struct OamData gOamData_AffineNormal_ObjNormal_8x32 =
{
    .y = 0,
    .affineMode = ST_OAM_AFFINE_NORMAL,
    .objMode = ST_OAM_OBJ_NORMAL,
    .bpp = ST_OAM_4BPP,
    .shape = SPRITE_SHAPE(8x32),
    .x = 0,
    .size = SPRITE_SIZE(8x32),
    .tileNum = 0,
    .priority = 2,
    .paletteNum = 0,
};

const struct OamData gOamData_AffineNormal_ObjNormal_16x32 =
{
    .y = 0,
    .affineMode = ST_OAM_AFFINE_NORMAL,
    .objMode = ST_OAM_OBJ_NORMAL,
    .bpp = ST_OAM_4BPP,
    .shape = SPRITE_SHAPE(16x32),
    .x = 0,
    .size = SPRITE_SIZE(16x32),
    .tileNum = 0,
    .priority = 2,
    .paletteNum = 0,
};

const struct OamData gOamData_AffineNormal_ObjNormal_32x64 =
{
    .y = 0,
    .affineMode = ST_OAM_AFFINE_NORMAL,
    .objMode = ST_OAM_OBJ_NORMAL,
    .bpp = ST_OAM_4BPP,
    .shape = SPRITE_SHAPE(32x64),
    .x = 0,
    .size = SPRITE_SIZE(32x64),
    .tileNum = 0,
    .priority = 2,
    .paletteNum = 0,
};

const struct OamData gOamData_AffineDouble_ObjNormal_8x8 =
{
    .y = 0,
    .affineMode = ST_OAM_AFFINE_DOUBLE,
    .objMode = ST_OAM_OBJ_NORMAL,
    .bpp = ST_OAM_4BPP,
    .shape = SPRITE_SHAPE(8x8),
    .x = 0,
    .size = SPRITE_SIZE(8x8),
    .tileNum = 0,
    .priority = 2,
    .paletteNum = 0,
};

const struct OamData gOamData_AffineDouble_ObjNormal_16x16 =
{
    .y = 0,
    .affineMode = ST_OAM_AFFINE_DOUBLE,
    .objMode = ST_OAM_OBJ_NORMAL,
    .bpp = ST_OAM_4BPP,
    .shape = SPRITE_SHAPE(16x16),
    .x = 0,
    .size = SPRITE_SIZE(16x16),
    .tileNum = 0,
    .priority = 2,
    .paletteNum = 0,
};

const struct OamData gOamData_AffineDouble_ObjNormal_32x32 =
{
    .y = 0,
    .affineMode = ST_OAM_AFFINE_DOUBLE,
    .objMode = ST_OAM_OBJ_NORMAL,
    .bpp = ST_OAM_4BPP,
    .shape = SPRITE_SHAPE(32x32),
    .x = 0,
    .size = SPRITE_SIZE(32x32),
    .tileNum = 0,
    .priority = 2,
    .paletteNum = 0,
};

const struct OamData gOamData_AffineDouble_ObjNormal_64x64 =
{
    .y = 0,
    .affineMode = ST_OAM_AFFINE_DOUBLE,
    .objMode = ST_OAM_OBJ_NORMAL,
    .bpp = ST_OAM_4BPP,
    .shape = SPRITE_SHAPE(64x64),
    .x = 0,
    .size = SPRITE_SIZE(64x64),
    .tileNum = 0,
    .priority = 2,
    .paletteNum = 0,
};

const struct OamData gOamData_AffineDouble_ObjNormal_16x8 =
{
    .y = 0,
    .affineMode = ST_OAM_AFFINE_DOUBLE,
    .objMode = ST_OAM_OBJ_NORMAL,
    .bpp = ST_OAM_4BPP,
    .shape = SPRITE_SHAPE(16x8),
    .x = 0,
    .size = SPRITE_SIZE(16x8),
    .tileNum = 0,
    .priority = 2,
    .paletteNum = 0,
};

const struct OamData gOamData_AffineDouble_ObjNormal_32x8 =
{
    .y = 0,
    .affineMode = ST_OAM_AFFINE_DOUBLE,
    .objMode = ST_OAM_OBJ_NORMAL,
    .bpp = ST_OAM_4BPP,
    .shape = SPRITE_SHAPE(32x8),
    .x = 0,
    .size = SPRITE_SIZE(32x8),
    .tileNum = 0,
    .priority = 2,
    .paletteNum = 0,
};

const struct OamData gOamData_AffineDouble_ObjNormal_32x16 =
{
    .y = 0,
    .affineMode = ST_OAM_AFFINE_DOUBLE,
    .objMode = ST_OAM_OBJ_NORMAL,
    .bpp = ST_OAM_4BPP,
    .shape = SPRITE_SHAPE(32x16),
    .x = 0,
    .size = SPRITE_SIZE(32x16),
    .tileNum = 0,
    .priority = 2,
    .paletteNum = 0,
};

const struct OamData gOamData_AffineDouble_ObjNormal_64x32 =
{
    .y = 0,
    .affineMode = ST_OAM_AFFINE_DOUBLE,
    .objMode = ST_OAM_OBJ_NORMAL,
    .bpp = ST_OAM_4BPP,
    .shape = SPRITE_SHAPE(64x32),
    .x = 0,
    .size = SPRITE_SIZE(64x32),
    .tileNum = 0,
    .priority = 2,
    .paletteNum = 0,
};

const struct OamData gOamData_AffineDouble_ObjNormal_8x16 =
{
    .y = 0,
    .affineMode = ST_OAM_AFFINE_DOUBLE,
    .objMode = ST_OAM_OBJ_NORMAL,
    .bpp = ST_OAM_4BPP,
    .shape = SPRITE_SHAPE(8x16),
    .x = 0,
    .size = SPRITE_SIZE(8x16),
    .tileNum = 0,
    .priority = 2,
    .paletteNum = 0,
};

const struct OamData gOamData_AffineDouble_ObjNormal_8x32 =
{
    .y = 0,
    .affineMode = ST_OAM_AFFINE_DOUBLE,
    .objMode = ST_OAM_OBJ_NORMAL,
    .bpp = ST_OAM_4BPP,
    .shape = SPRITE_SHAPE(8x32),
    .x = 0,
    .size = SPRITE_SIZE(8x32),
    .tileNum = 0,
    .priority = 2,
    .paletteNum = 0,
};

const struct OamData gOamData_AffineDouble_ObjNormal_16x32 =
{
    .y = 0,
    .affineMode = ST_OAM_AFFINE_DOUBLE,
    .objMode = ST_OAM_OBJ_NORMAL,
    .bpp = ST_OAM_4BPP,
    .shape = SPRITE_SHAPE(16x32),
    .x = 0,
    .size = SPRITE_SIZE(16x32),
    .tileNum = 0,
    .priority = 2,
    .paletteNum = 0,
};

const struct OamData gOamData_AffineDouble_ObjNormal_32x64 =
{
    .y = 0,
    .affineMode = ST_OAM_AFFINE_DOUBLE,
    .objMode = ST_OAM_OBJ_NORMAL,
    .bpp = ST_OAM_4BPP,
    .shape = SPRITE_SHAPE(32x64),
    .x = 0,
    .size = SPRITE_SIZE(32x64),
    .tileNum = 0,
    .priority = 2,
    .paletteNum = 0,
};

const struct OamData gOamData_AffineOff_ObjBlend_8x8 =
{
    .y = 0,
    .affineMode = ST_OAM_AFFINE_OFF,
    .objMode = ST_OAM_OBJ_BLEND,
    .bpp = ST_OAM_4BPP,
    .shape = SPRITE_SHAPE(8x8),
    .x = 0,
    .size = SPRITE_SIZE(8x8),
    .tileNum = 0,
    .priority = 2,
    .paletteNum = 0,
};

const struct OamData gOamData_AffineOff_ObjBlend_16x16 =
{
    .y = 0,
    .affineMode = ST_OAM_AFFINE_OFF,
    .objMode = ST_OAM_OBJ_BLEND,
    .bpp = ST_OAM_4BPP,
    .shape = SPRITE_SHAPE(16x16),
    .x = 0,
    .size = SPRITE_SIZE(16x16),
    .tileNum = 0,
    .priority = 2,
    .paletteNum = 0,
};

const struct OamData gOamData_AffineOff_ObjBlend_32x32 =
{
    .y = 0,
    .affineMode = ST_OAM_AFFINE_OFF,
    .objMode = ST_OAM_OBJ_BLEND,
    .bpp = ST_OAM_4BPP,
    .shape = SPRITE_SHAPE(32x32),
    .x = 0,
    .size = SPRITE_SIZE(32x32),
    .tileNum = 0,
    .priority = 2,
    .paletteNum = 0,
};

const struct OamData gOamData_AffineOff_ObjBlend_64x64 =
{
    .y = 0,
    .affineMode = ST_OAM_AFFINE_OFF,
    .objMode = ST_OAM_OBJ_BLEND,
    .bpp = ST_OAM_4BPP,
    .shape = SPRITE_SHAPE(64x64),
    .x = 0,
    .size = SPRITE_SIZE(64x64),
    .tileNum = 0,
    .priority = 2,
    .paletteNum = 0,
};

const struct OamData gOamData_AffineOff_ObjBlend_16x8 =
{
    .y = 0,
    .affineMode = ST_OAM_AFFINE_OFF,
    .objMode = ST_OAM_OBJ_BLEND,
    .bpp = ST_OAM_4BPP,
    .shape = SPRITE_SHAPE(16x8),
    .x = 0,
    .size = SPRITE_SIZE(16x8),
    .tileNum = 0,
    .priority = 2,
    .paletteNum = 0,
};

const struct OamData gOamData_AffineOff_ObjBlend_32x8 =
{
    .y = 0,
    .affineMode = ST_OAM_AFFINE_OFF,
    .objMode = ST_OAM_OBJ_BLEND,
    .bpp = ST_OAM_4BPP,
    .shape = SPRITE_SHAPE(32x8),
    .x = 0,
    .size = SPRITE_SIZE(32x8),
    .tileNum = 0,
    .priority = 2,
    .paletteNum = 0,
};

const struct OamData gOamData_AffineOff_ObjBlend_32x16 =
{
    .y = 0,
    .affineMode = ST_OAM_AFFINE_OFF,
    .objMode = ST_OAM_OBJ_BLEND,
    .bpp = ST_OAM_4BPP,
    .shape = SPRITE_SHAPE(32x16),
    .x = 0,
    .size = SPRITE_SIZE(32x16),
    .tileNum = 0,
    .priority = 2,
    .paletteNum = 0,
};

const struct OamData gOamData_AffineOff_ObjBlend_64x32 =
{
    .y = 0,
    .affineMode = ST_OAM_AFFINE_OFF,
    .objMode = ST_OAM_OBJ_BLEND,
    .bpp = ST_OAM_4BPP,
    .shape = SPRITE_SHAPE(64x32),
    .x = 0,
    .size = SPRITE_SIZE(64x32),
    .tileNum = 0,
    .priority = 2,
    .paletteNum = 0,
};

const struct OamData gOamData_AffineOff_ObjBlend_8x16 =
{
    .y = 0,
    .affineMode = ST_OAM_AFFINE_OFF,
    .objMode = ST_OAM_OBJ_BLEND,
    .bpp = ST_OAM_4BPP,
    .shape = SPRITE_SHAPE(8x16),
    .x = 0,
    .size = SPRITE_SIZE(8x16),
    .tileNum = 0,
    .priority = 2,
    .paletteNum = 0,
};

const struct OamData gOamData_AffineOff_ObjBlend_8x32 =
{
    .y = 0,
    .affineMode = ST_OAM_AFFINE_OFF,
    .objMode = ST_OAM_OBJ_BLEND,
    .bpp = ST_OAM_4BPP,
    .shape = SPRITE_SHAPE(8x32),
    .x = 0,
    .size = SPRITE_SIZE(8x32),
    .tileNum = 0,
    .priority = 2,
    .paletteNum = 0,
};

const struct OamData gOamData_AffineOff_ObjBlend_16x32 =
{
    .y = 0,
    .affineMode = ST_OAM_AFFINE_OFF,
    .objMode = ST_OAM_OBJ_BLEND,
    .bpp = ST_OAM_4BPP,
    .shape = SPRITE_SHAPE(16x32),
    .x = 0,
    .size = SPRITE_SIZE(16x32),
    .tileNum = 0,
    .priority = 2,
    .paletteNum = 0,
};

const struct OamData gOamData_AffineOff_ObjBlend_32x64 =
{
    .y = 0,
    .affineMode = ST_OAM_AFFINE_OFF,
    .objMode = ST_OAM_OBJ_BLEND,
    .bpp = ST_OAM_4BPP,
    .shape = SPRITE_SHAPE(32x64),
    .x = 0,
    .size = SPRITE_SIZE(32x64),
    .tileNum = 0,
    .priority = 2,
    .paletteNum = 0,
};

const struct OamData gOamData_AffineNormal_ObjBlend_8x8 =
{
    .y = 0,
    .affineMode = ST_OAM_AFFINE_NORMAL,
    .objMode = ST_OAM_OBJ_BLEND,
    .bpp = ST_OAM_4BPP,
    .shape = SPRITE_SHAPE(8x8),
    .x = 0,
    .size = SPRITE_SIZE(8x8),
    .tileNum = 0,
    .priority = 2,
    .paletteNum = 0,
};

const struct OamData gOamData_AffineNormal_ObjBlend_16x16 =
{
    .y = 0,
    .affineMode = ST_OAM_AFFINE_NORMAL,
    .objMode = ST_OAM_OBJ_BLEND,
    .bpp = ST_OAM_4BPP,
    .shape = SPRITE_SHAPE(16x16),
    .x = 0,
    .size = SPRITE_SIZE(16x16),
    .tileNum = 0,
    .priority = 2,
    .paletteNum = 0,
};

const struct OamData gOamData_AffineNormal_ObjBlend_32x32 =
{
    .y = 0,
    .affineMode = ST_OAM_AFFINE_NORMAL,
    .objMode = ST_OAM_OBJ_BLEND,
    .bpp = ST_OAM_4BPP,
    .shape = SPRITE_SHAPE(32x32),
    .x = 0,
    .size = SPRITE_SIZE(32x32),
    .tileNum = 0,
    .priority = 2,
    .paletteNum = 0,
};

const struct OamData gOamData_AffineNormal_ObjBlend_64x64 =
{
    .y = 0,
    .affineMode = ST_OAM_AFFINE_NORMAL,
    .objMode = ST_OAM_OBJ_BLEND,
    .bpp = ST_OAM_4BPP,
    .shape = SPRITE_SHAPE(64x64),
    .x = 0,
    .size = SPRITE_SIZE(64x64),
    .tileNum = 0,
    .priority = 2,
    .paletteNum = 0,
};

const struct OamData gOamData_AffineNormal_ObjBlend_16x8 =
{
    .y = 0,
    .affineMode = ST_OAM_AFFINE_NORMAL,
    .objMode = ST_OAM_OBJ_BLEND,
    .bpp = ST_OAM_4BPP,
    .shape = SPRITE_SHAPE(16x8),
    .x = 0,
    .size = SPRITE_SIZE(16x8),
    .tileNum = 0,
    .priority = 2,
    .paletteNum = 0,
};

const struct OamData gOamData_AffineNormal_ObjBlend_32x8 =
{
    .y = 0,
    .affineMode = ST_OAM_AFFINE_NORMAL,
    .objMode = ST_OAM_OBJ_BLEND,
    .bpp = ST_OAM_4BPP,
    .shape = SPRITE_SHAPE(32x8),
    .x = 0,
    .size = SPRITE_SIZE(32x8),
    .tileNum = 0,
    .priority = 2,
    .paletteNum = 0,
};

const struct OamData gOamData_AffineNormal_ObjBlend_32x16 =
{
    .y = 0,
    .affineMode = ST_OAM_AFFINE_NORMAL,
    .objMode = ST_OAM_OBJ_BLEND,
    .bpp = ST_OAM_4BPP,
    .shape = SPRITE_SHAPE(32x16),
    .x = 0,
    .size = SPRITE_SIZE(32x16),
    .tileNum = 0,
    .priority = 2,
    .paletteNum = 0,
};

const struct OamData gOamData_AffineNormal_ObjBlend_64x32 =
{
    .y = 0,
    .affineMode = ST_OAM_AFFINE_NORMAL,
    .objMode = ST_OAM_OBJ_BLEND,
    .bpp = ST_OAM_4BPP,
    .shape = SPRITE_SHAPE(64x32),
    .x = 0,
    .size = SPRITE_SIZE(64x32),
    .tileNum = 0,
    .priority = 2,
    .paletteNum = 0,
};

const struct OamData gOamData_AffineNormal_ObjBlend_8x16 =
{
    .y = 0,
    .affineMode = ST_OAM_AFFINE_NORMAL,
    .objMode = ST_OAM_OBJ_BLEND,
    .bpp = ST_OAM_4BPP,
    .shape = SPRITE_SHAPE(8x16),
    .x = 0,
    .size = SPRITE_SIZE(8x16),
    .tileNum = 0,
    .priority = 2,
    .paletteNum = 0,
};

const struct OamData gOamData_AffineNormal_ObjBlend_8x32 =
{
    .y = 0,
    .affineMode = ST_OAM_AFFINE_NORMAL,
    .objMode = ST_OAM_OBJ_BLEND,
    .bpp = ST_OAM_4BPP,
    .shape = SPRITE_SHAPE(8x32),
    .x = 0,
    .size = SPRITE_SIZE(8x32),
    .tileNum = 0,
    .priority = 2,
    .paletteNum = 0,
};

const struct OamData gOamData_AffineNormal_ObjBlend_16x32 =
{
    .y = 0,
    .affineMode = ST_OAM_AFFINE_NORMAL,
    .objMode = ST_OAM_OBJ_BLEND,
    .bpp = ST_OAM_4BPP,
    .shape = SPRITE_SHAPE(16x32),
    .x = 0,
    .size = SPRITE_SIZE(16x32),
    .tileNum = 0,
    .priority = 2,
    .paletteNum = 0,
};

const struct OamData gOamData_AffineNormal_ObjBlend_32x64 =
{
    .y = 0,
    .affineMode = ST_OAM_AFFINE_NORMAL,
    .objMode = ST_OAM_OBJ_BLEND,
    .bpp = ST_OAM_4BPP,
    .shape = SPRITE_SHAPE(32x64),
    .x = 0,
    .size = SPRITE_SIZE(32x64),
    .tileNum = 0,
    .priority = 2,
    .paletteNum = 0,
};

const struct OamData gOamData_AffineDouble_ObjBlend_8x8 =
{
    .y = 0,
    .affineMode = ST_OAM_AFFINE_DOUBLE,
    .objMode = ST_OAM_OBJ_BLEND,
    .bpp = ST_OAM_4BPP,
    .shape = SPRITE_SHAPE(8x8),
    .x = 0,
    .size = SPRITE_SIZE(8x8),
    .tileNum = 0,
    .priority = 2,
    .paletteNum = 0,
};

const struct OamData gOamData_AffineDouble_ObjBlend_16x16 =
{
    .y = 0,
    .affineMode = ST_OAM_AFFINE_DOUBLE,
    .objMode = ST_OAM_OBJ_BLEND,
    .bpp = ST_OAM_4BPP,
    .shape = SPRITE_SHAPE(16x16),
    .x = 0,
    .size = SPRITE_SIZE(16x16),
    .tileNum = 0,
    .priority = 2,
    .paletteNum = 0,
};

const struct OamData gOamData_AffineDouble_ObjBlend_32x32 =
{
    .y = 0,
    .affineMode = ST_OAM_AFFINE_DOUBLE,
    .objMode = ST_OAM_OBJ_BLEND,
    .bpp = ST_OAM_4BPP,
    .shape = SPRITE_SHAPE(32x32),
    .x = 0,
    .size = SPRITE_SIZE(32x32),
    .tileNum = 0,
    .priority = 2,
    .paletteNum = 0,
};

const struct OamData gOamData_AffineDouble_ObjBlend_64x64 =
{
    .y = 0,
    .affineMode = ST_OAM_AFFINE_DOUBLE,
    .objMode = ST_OAM_OBJ_BLEND,
    .bpp = ST_OAM_4BPP,
    .shape = SPRITE_SHAPE(64x64),
    .x = 0,
    .size = SPRITE_SIZE(64x64),
    .tileNum = 0,
    .priority = 2,
    .paletteNum = 0,
};

const struct OamData gOamData_AffineDouble_ObjBlend_16x8 =
{
    .y = 0,
    .affineMode = ST_OAM_AFFINE_DOUBLE,
    .objMode = ST_OAM_OBJ_BLEND,
    .bpp = ST_OAM_4BPP,
    .shape = SPRITE_SHAPE(16x8),
    .x = 0,
    .size = SPRITE_SIZE(16x8),
    .tileNum = 0,
    .priority = 2,
    .paletteNum = 0,
};

const struct OamData gOamData_AffineDouble_ObjBlend_32x8 =
{
    .y = 0,
    .affineMode = ST_OAM_AFFINE_DOUBLE,
    .objMode = ST_OAM_OBJ_BLEND,
    .bpp = ST_OAM_4BPP,
    .shape = SPRITE_SHAPE(32x8),
    .x = 0,
    .size = SPRITE_SIZE(32x8),
    .tileNum = 0,
    .priority = 2,
    .paletteNum = 0,
};

const struct OamData gOamData_AffineDouble_ObjBlend_32x16 =
{
    .y = 0,
    .affineMode = ST_OAM_AFFINE_DOUBLE,
    .objMode = ST_OAM_OBJ_BLEND,
    .bpp = ST_OAM_4BPP,
    .shape = SPRITE_SHAPE(32x16),
    .x = 0,
    .size = SPRITE_SIZE(32x16),
    .tileNum = 0,
    .priority = 2,
    .paletteNum = 0,
};

const struct OamData gOamData_AffineDouble_ObjBlend_64x32 =
{
    .y = 0,
    .affineMode = ST_OAM_AFFINE_DOUBLE,
    .objMode = ST_OAM_OBJ_BLEND,
    .bpp = ST_OAM_4BPP,
    .shape = SPRITE_SHAPE(64x32),
    .x = 0,
    .size = SPRITE_SIZE(64x32),
    .tileNum = 0,
    .priority = 2,
    .paletteNum = 0,
};

const struct OamData gOamData_AffineDouble_ObjBlend_8x16 =
{
    .y = 0,
    .affineMode = ST_OAM_AFFINE_DOUBLE,
    .objMode = ST_OAM_OBJ_BLEND,
    .bpp = ST_OAM_4BPP,
    .shape = SPRITE_SHAPE(8x16),
    .x = 0,
    .size = SPRITE_SIZE(8x16),
    .tileNum = 0,
    .priority = 2,
    .paletteNum = 0,
};

const struct OamData gOamData_AffineDouble_ObjBlend_8x32 =
{
    .y = 0,
    .affineMode = ST_OAM_AFFINE_DOUBLE,
    .objMode = ST_OAM_OBJ_BLEND,
    .bpp = ST_OAM_4BPP,
    .shape = SPRITE_SHAPE(8x32),
    .x = 0,
    .size = SPRITE_SIZE(8x32),
    .tileNum = 0,
    .priority = 2,
    .paletteNum = 0,
};

const struct OamData gOamData_AffineDouble_ObjBlend_16x32 =
{
    .y = 0,
    .affineMode = ST_OAM_AFFINE_DOUBLE,
    .objMode = ST_OAM_OBJ_BLEND,
    .bpp = ST_OAM_4BPP,
    .shape = SPRITE_SHAPE(16x32),
    .x = 0,
    .size = SPRITE_SIZE(16x32),
    .tileNum = 0,
    .priority = 2,
    .paletteNum = 0,
};

const struct OamData gOamData_AffineDouble_ObjBlend_32x64 =
{
    .y = 0,
    .affineMode = ST_OAM_AFFINE_DOUBLE,
    .objMode = ST_OAM_OBJ_BLEND,
    .bpp = ST_OAM_4BPP,
    .shape = SPRITE_SHAPE(32x64),
    .x = 0,
    .size = SPRITE_SIZE(32x64),
    .tileNum = 0,
    .priority = 2,
    .paletteNum = 0,
};

const struct CompressedSpriteSheet gBattleAnimPicTable[] =
{
    {gBattleAnimSpriteGfx_Bone, 0x0200, ANIM_TAG_BONE},
    {gBattleAnimSpriteGfx_Spark, 0x0300, ANIM_TAG_SPARK},
    {gBattleAnimSpriteGfx_Pencil, 0x0200, ANIM_TAG_PENCIL},
    {gBattleAnimSpriteGfx_AirWave, 0x0100, ANIM_TAG_AIR_WAVE},
    {gBattleAnimSpriteGfx_Orb, 0x0200, ANIM_TAG_ORB},
    {gBattleAnimSpriteGfx_Sword, 0x0400, ANIM_TAG_SWORD},
    {gBattleAnimSpriteGfx_Seed, 0x0180, ANIM_TAG_SEED},
    {gBattleAnimSpriteGfx_Explosion6, 0x0800, ANIM_TAG_EXPLOSION_6},
    {gBattleAnimSpriteGfx_Gust, 0x0400, ANIM_TAG_GUST},
    {gBattleAnimSpriteGfx_IceCube, 0x1200, ANIM_TAG_ICE_CUBE},
    {gBattleAnimSpriteGfx_Spark2, 0x0180, ANIM_TAG_SPARK_2},
    {gBattleAnimSpriteGfx_YellowBall, 0x0080, ANIM_TAG_YELLOW_BALL},
    {gBattleAnimSpriteGfx_LockOn, 0x0280, ANIM_TAG_LOCK_ON},
    {gBattleAnimSpriteGfx_TiedBag, 0x0080, ANIM_TAG_TIED_BAG},
    {gBattleAnimSpriteGfx_BlackSmoke, 0x0100, ANIM_TAG_BLACK_SMOKE},
    {gBattleAnimSpriteGfx_BlackBall, 0x0020, ANIM_TAG_BLACK_BALL},
    {gBattleAnimSpriteGfx_Conversion, 0x0080, ANIM_TAG_CONVERSION},
    {gBattleAnimSpriteGfx_HornHit, 0x0200, ANIM_TAG_HORN_HIT},
    {gBattleAnimSpriteGfx_Hit, 0x0A00, ANIM_TAG_HIT},
    {gBattleAnimSpriteGfx_Hit, 0x0A00, ANIM_TAG_HIT_2},
    {gBattleAnimSpriteGfx_Leer, 0x0A00, ANIM_TAG_LEER},
    {gBattleAnimSpriteGfx_SmallEmber, 0x0A00, ANIM_TAG_SMALL_EMBER},
    {gBattleAnimSpriteGfx_GraySmoke, 0x0A00, ANIM_TAG_GRAY_SMOKE},
    {gBattleAnimSpriteGfx_BlueStar, 0x0E00, ANIM_TAG_BLUE_STAR},
    {gBattleAnimSpriteGfx_BubbleBurst, 0x0380, ANIM_TAG_BUBBLE_BURST},
    {gBattleAnimSpriteGfx_Fire, 0x1000, ANIM_TAG_FIRE},
    {gBattleAnimSpriteGfx_FirePlume, 0x0A00, ANIM_TAG_FIRE_PLUME},
    {gBattleAnimSpriteGfx_Lightning, 0x0A00, ANIM_TAG_LIGHTNING},
    {gBattleAnimSpriteGfx_ClawSlash, 0x0A00, ANIM_TAG_CLAW_SLASH},
    {gBattleAnimSpriteGfx_IceChunk, 0x0A00, ANIM_TAG_ICE_CHUNK},
    {gBattleAnimSpriteGfx_Sparkle1, 0x1000, ANIM_TAG_SPARKLE_1},
    {gBattleAnimSpriteGfx_Sparkle1, 0x1000, ANIM_TAG_SPARKLE_2},
    {gBattleAnimSpriteGfx_HumanoidFoot, 0x0200, ANIM_TAG_HUMANOID_FOOT},
    {gBattleAnimSpriteGfx_MonsterFoot, 0x0200, ANIM_TAG_MONSTER_FOOT},
    {gBattleAnimSpriteGfx_NoiseLine, 0x0800, ANIM_TAG_NOISE_LINE},
    {gBattleAnimSpriteGfx_SlamHit, 0x1000, ANIM_TAG_SLAM_HIT},
    {gBattleAnimSpriteGfx_Rocks, 0x0C00, ANIM_TAG_ROCKS},
    {gBattleAnimSpriteGfx_Leaf, 0x0480, ANIM_TAG_LEAF},
    {gBattleAnimSpriteGfx_Finger, 0x0200, ANIM_TAG_FINGER},
    {gBattleAnimSpriteGfx_PoisonPowder, 0x0200, ANIM_TAG_POISON_POWDER},
    {gBattleAnimSpriteGfx_PoisonPowder, 0x0200, ANIM_TAG_SLEEP_POWDER},
    {gBattleAnimSpriteGfx_PoisonPowder, 0x0200, ANIM_TAG_STUN_SPORE},
    {gBattleAnimSpriteGfx_Sparkle3, 0x0200, ANIM_TAG_SPARKLE_3},
    {gBattleAnimSpriteGfx_Sparkle4, 0x0A00, ANIM_TAG_SPARKLE_4},
    {gBattleAnimSpriteGfx_MusicNotes, 0x0300, ANIM_TAG_MUSIC_NOTES},
    {gBattleAnimSpriteGfx_Duck, 0x0180, ANIM_TAG_DUCK},
    {gBattleAnimSpriteGfx_MudSand, 0x00A0, ANIM_TAG_MUD_SAND},
    {gBattleAnimSpriteGfx_Alert, 0x0700, ANIM_TAG_ALERT},
    {gBattleAnimSpriteGfx_Shock, 0x0C00, ANIM_TAG_SHOCK},
    {gBattleAnimSpriteGfx_Breath, 0x0200, ANIM_TAG_BREATH},
    {gBattleAnimSpriteGfx_Anger, 0x0080, ANIM_TAG_ANGER},
    {gBattleAnimSpriteGfx_Amnesia, 0x1000, ANIM_TAG_AMNESIA},
    {gBattleAnimSpriteGfx_BentSpoon, 0x0C00, ANIM_TAG_BENT_SPOON},
    {gBattleAnimSpriteGfx_MilkBottle, 0x0200, ANIM_TAG_MILK_BOTTLE},
    {gBattleAnimSpriteGfx_Coin, 0x0200, ANIM_TAG_COIN},
    {gBattleAnimSpriteGfx_RainDrops, 0x0700, ANIM_TAG_RAIN_DROPS},
    {gBattleAnimSpriteGfx_Impact, 0x0200, ANIM_TAG_IMPACT},
    {gBattleAnimSpriteGfx_CircleImpact, 0x0020, ANIM_TAG_CIRCLE_IMPACT},
    {gBattleAnimSpriteGfx_Scratch, 0x0a00, ANIM_TAG_SCRATCH},
    {gBattleAnimSpriteGfx_Cut, 0x0800, ANIM_TAG_CUT},
    {gBattleAnimSpriteGfx_SharpTeeth, 0x0800, ANIM_TAG_SHARP_TEETH},
    {gBattleAnimSpriteGfx_RainbowRings, 0x00c0, ANIM_TAG_RAINBOW_RINGS},
    {gBattleAnimSpriteGfx_IceCrystals, 0x01c0, ANIM_TAG_ICE_CRYSTALS},
    {gBattleAnimSpriteGfx_IceSpikes, 0x0100, ANIM_TAG_ICE_SPIKES},
    {gBattleAnimSpriteGfx_HandsAndFeet, 0x0800, ANIM_TAG_HANDS_AND_FEET},
    {gBattleAnimSpriteGfx_MistCloud, 0x0200, ANIM_TAG_MIST_CLOUD},
    {gBattleAnimSpriteGfx_Clamp, 0x0800, ANIM_TAG_CLAMP},
    {gBattleAnimSpriteGfx_Bubble, 0x0180, ANIM_TAG_BUBBLE},
    {gBattleAnimSpriteGfx_Orbs, 0x0180, ANIM_TAG_ORBS},
    {gBattleAnimSpriteGfx_WaterImpact, 0x0200, ANIM_TAG_WATER_IMPACT},
    {gBattleAnimSpriteGfx_WaterOrb, 0x0200, ANIM_TAG_WATER_ORB},
    {gBattleAnimSpriteGfx_PoisonBubble, 0x0180, ANIM_TAG_POISON_BUBBLE},
    {gBattleAnimSpriteGfx_ToxicBubble, 0x0400, ANIM_TAG_TOXIC_BUBBLE},
    {gBattleAnimSpriteGfx_Spikes, 0x0080, ANIM_TAG_SPIKES},
    {gBattleAnimSpriteGfx_HornHit2, 0x0100, ANIM_TAG_HORN_HIT_2},
    {gBattleAnimSpriteGfx_AirWave2, 0x0100, ANIM_TAG_AIR_WAVE_2},
    {gBattleAnimSpriteGfx_SmallBubbles, 0x0140, ANIM_TAG_SMALL_BUBBLES},
    {gBattleAnimSpriteGfx_RoundShadow, 0x0800, ANIM_TAG_ROUND_SHADOW},
    {gBattleAnimSpriteGfx_Sunlight, 0x0200, ANIM_TAG_SUNLIGHT},
    {gBattleAnimSpriteGfx_Spore, 0x0100, ANIM_TAG_SPORE},
    {gBattleAnimSpriteGfx_Flower, 0x00a0, ANIM_TAG_FLOWER},
    {gBattleAnimSpriteGfx_RazorLeaf, 0x0100, ANIM_TAG_RAZOR_LEAF},
    {gBattleAnimSpriteGfx_Needle, 0x0080, ANIM_TAG_NEEDLE},
    {gBattleAnimSpriteGfx_WhirlwindLines, 0x0300, ANIM_TAG_WHIRLWIND_LINES},
    {gBattleAnimSpriteGfx_GoldRing, 0x0100, ANIM_TAG_GOLD_RING},
    {gBattleAnimSpriteGfx_GoldRing, 0x0100, ANIM_TAG_PURPLE_RING},
    {gBattleAnimSpriteGfx_GoldRing, 0x0100, ANIM_TAG_BLUE_RING},
    {gBattleAnimSpriteGfx_GreenLightWall, 0x0800, ANIM_TAG_GREEN_LIGHT_WALL},
    {gBattleAnimSpriteGfx_GreenLightWall, 0x0800, ANIM_TAG_BLUE_LIGHT_WALL},
    {gBattleAnimSpriteGfx_GreenLightWall, 0x0800, ANIM_TAG_RED_LIGHT_WALL},
    {gBattleAnimSpriteGfx_GreenLightWall, 0x0800, ANIM_TAG_GRAY_LIGHT_WALL},
    {gBattleAnimSpriteGfx_GreenLightWall, 0x0800, ANIM_TAG_ORANGE_LIGHT_WALL},
    {gBattleAnimSpriteGfx_BlackBall2, 0x0080, ANIM_TAG_BLACK_BALL_2},
    {gBattleAnimSpriteGfx_MistCloud, 0x0200, ANIM_TAG_PURPLE_GAS_CLOUD},
    {gBattleAnimSpriteGfx_SparkH, 0x0200, ANIM_TAG_SPARK_H},
    {gBattleAnimSpriteGfx_YellowStar, 0x0200, ANIM_TAG_YELLOW_STAR},
    {gBattleAnimSpriteGfx_LargeFreshEgg, 0x0080, ANIM_TAG_LARGE_FRESH_EGG},
    {gBattleAnimSpriteGfx_ShadowBall, 0x0200, ANIM_TAG_SHADOW_BALL},
    {gBattleAnimSpriteGfx_Lick, 0x0500, ANIM_TAG_LICK},
    {gBattleAnimSpriteGfx_VoidLines, 0x0800, ANIM_TAG_VOID_LINES},
    {gBattleAnimSpriteGfx_String, 0x0400, ANIM_TAG_STRING},
    {gBattleAnimSpriteGfx_WebThread, 0x0020, ANIM_TAG_WEB_THREAD},
    {gBattleAnimSpriteGfx_SpiderWeb, 0x0800, ANIM_TAG_SPIDER_WEB},
    {gBattleAnimSpriteGfx_Slash, 0x0800, ANIM_TAG_SLASH},
    {gBattleAnimSpriteGfx_FocusEnergy, 0x0400, ANIM_TAG_FOCUS_ENERGY},
    {gBattleAnimSpriteGfx_SphereToCube, 0x0a00, ANIM_TAG_SPHERE_TO_CUBE},
    {gBattleAnimSpriteGfx_Tendrils, 0x1000, ANIM_TAG_TENDRILS},
    {gBattleAnimSpriteGfx_Eye, 0x0800, ANIM_TAG_EYE},
    {gBattleAnimSpriteGfx_WhiteShadow, 0x0400, ANIM_TAG_WHITE_SHADOW},
    {gBattleAnimSpriteGfx_TealAlert, 0x0200, ANIM_TAG_TEAL_ALERT},
    {gBattleAnimSpriteGfx_OpeningEye, 0x0800, ANIM_TAG_OPENING_EYE},
    {gBattleAnimSpriteGfx_RoundWhiteHalo, 0x0800, ANIM_TAG_ROUND_WHITE_HALO},
    {gBattleAnimSpriteGfx_FangAttack, 0x0800, ANIM_TAG_FANG_ATTACK},
    {gBattleAnimSpriteGfx_PurpleHandOutline, 0x0200, ANIM_TAG_PURPLE_HAND_OUTLINE},
    {gBattleAnimSpriteGfx_Moon, 0x0800, ANIM_TAG_MOON},
    {gBattleAnimSpriteGfx_GreenSparkle, 0x0200, ANIM_TAG_GREEN_SPARKLE},
    {gBattleAnimSpriteGfx_Spiral, 0x0800, ANIM_TAG_SPIRAL},
    {gBattleAnimSpriteGfx_SnoreZ, 0x0200, ANIM_TAG_SNORE_Z},
    {gBattleAnimSpriteGfx_Explosion, 0x0800, ANIM_TAG_EXPLOSION},
    {gBattleAnimSpriteGfx_Nail, 0x0400, ANIM_TAG_NAIL},
    {gBattleAnimSpriteGfx_GhostlySpirit, 0x0200, ANIM_TAG_GHOSTLY_SPIRIT},
    {gBattleAnimSpriteGfx_WarmRock, 0x0a80, ANIM_TAG_WARM_ROCK},
    {gBattleAnimSpriteGfx_BreakingEgg, 0x0600, ANIM_TAG_BREAKING_EGG},
    {gBattleAnimSpriteGfx_ThinRing, 0x0800, ANIM_TAG_THIN_RING},
    {gBattleAnimSpriteGfx_Bell, 0x0600, ANIM_TAG_BELL},
    {gBattleAnimSpriteGfx_MusicNotes2, 0x0800, ANIM_TAG_MUSIC_NOTES_2},
    {gBattleAnimSpriteGfx_SpeedDust, 0x0180, ANIM_TAG_SPEED_DUST},
    {gBattleAnimSpriteGfx_TornMetal, 0x0800, ANIM_TAG_TORN_METAL},
    {gBattleAnimSpriteGfx_ThoughtBubble, 0x0800, ANIM_TAG_THOUGHT_BUBBLE},
    {gBattleAnimSpriteGfx_MagentaHeart, 0x0080, ANIM_TAG_MAGENTA_HEART},
    {gBattleAnimSpriteGfx_ElectricOrbs, 0x0080, ANIM_TAG_ELECTRIC_ORBS},
    {gBattleAnimSpriteGfx_CircleOfLight, 0x0800, ANIM_TAG_CIRCLE_OF_LIGHT},
    {gBattleAnimSpriteGfx_Electricity, 0x0800, ANIM_TAG_ELECTRICITY},
    {gBattleAnimSpriteGfx_Finger2, 0x0600, ANIM_TAG_FINGER_2},
    {gBattleAnimSpriteGfx_MovementWaves, 0x0600, ANIM_TAG_MOVEMENT_WAVES},
    {gBattleAnimSpriteGfx_MagentaHeart, 0x0080, ANIM_TAG_RED_HEART},
    {gBattleAnimSpriteGfx_RedOrb, 0x0080, ANIM_TAG_RED_ORB},
    {gBattleAnimSpriteGfx_EyeSparkle, 0x0180, ANIM_TAG_EYE_SPARKLE},
    {gBattleAnimSpriteGfx_MagentaHeart, 0x0080, ANIM_TAG_PINK_HEART},
    {gBattleAnimSpriteGfx_Angel, 0x0200, ANIM_TAG_ANGEL},
    {gBattleAnimSpriteGfx_Devil, 0x0400, ANIM_TAG_DEVIL},
    {gBattleAnimSpriteGfx_Swipe, 0x0a00, ANIM_TAG_SWIPE},
    {gBattleAnimSpriteGfx_Roots, 0x0800, ANIM_TAG_ROOTS},
    {gBattleAnimSpriteGfx_ItemBag, 0x0200, ANIM_TAG_ITEM_BAG},
    {gBattleAnimSpriteGfx_JaggedMusicNote, 0x0400, ANIM_TAG_JAGGED_MUSIC_NOTE},
    {gBattleAnimSpriteGfx_Pokeball, 0x0080, ANIM_TAG_POKEBALL},
    {gBattleAnimSpriteGfx_Spotlight, 0x0800, ANIM_TAG_SPOTLIGHT},
    {gBattleAnimSpriteGfx_LetterZ, 0x0200, ANIM_TAG_LETTER_Z},
    {gBattleAnimSpriteGfx_RapidSpin, 0x0300, ANIM_TAG_RAPID_SPIN},
    {gBattleAnimSpriteGfx_TriAttackTriangle, 0x0800, ANIM_TAG_TRI_ATTACK_TRIANGLE},
    {gBattleAnimSpriteGfx_WispOrb, 0x0380, ANIM_TAG_WISP_ORB},
    {gBattleAnimSpriteGfx_WispFire, 0x0800, ANIM_TAG_WISP_FIRE},
    {gBattleAnimSpriteGfx_ShinySquares, 0x00c0, ANIM_TAG_SHINY_SQUARES},
    {gBattleAnimSpriteGfx_GoldStars, 0x00c0, ANIM_TAG_GOLD_STARS},
    {gBattleAnimSpriteGfx_EclipsingOrb, 0x0800, ANIM_TAG_ECLIPSING_ORB},
    {gBattleAnimSpriteGfx_GrayOrb, 0x0060, ANIM_TAG_GRAY_ORB},
    {gBattleAnimSpriteGfx_GrayOrb, 0x0060, ANIM_TAG_BLUE_ORB},
    {gBattleAnimSpriteGfx_GrayOrb, 0x0060, ANIM_TAG_RED_ORB_2},
    {gBattleAnimSpriteGfx_PinkPetal, 0x0080, ANIM_TAG_PINK_PETAL},
    {gBattleAnimSpriteGfx_PainSplit, 0x0180, ANIM_TAG_PAIN_SPLIT},
    {gBattleAnimSpriteGfx_Confetti, 0x0180, ANIM_TAG_CONFETTI},
    {gBattleAnimSpriteGfx_GreenStar, 0x0200, ANIM_TAG_GREEN_STAR},
    {gBattleAnimSpriteGfx_PinkCloud, 0x0200, ANIM_TAG_PINK_CLOUD},
    {gBattleAnimSpriteGfx_SweatDrop, 0x0020, ANIM_TAG_SWEAT_DROP},
    {gBattleAnimSpriteGfx_GuardRing, 0x0400, ANIM_TAG_GUARD_RING},
    {gBattleAnimSpriteGfx_PurpleScratch, 0x0600, ANIM_TAG_PURPLE_SCRATCH},
    {gBattleAnimSpriteGfx_PurpleSwipe, 0x1000, ANIM_TAG_PURPLE_SWIPE},
    {gBattleAnimSpriteGfx_TagHand, 0x0400, ANIM_TAG_TAG_HAND},
    {gBattleAnimSpriteGfx_SmallRedEye, 0x0020, ANIM_TAG_SMALL_RED_EYE},
    {gBattleAnimSpriteGfx_HollowOrb, 0x0080, ANIM_TAG_HOLLOW_ORB},
    {gBattleAnimSpriteGfx_XSign, 0x0800, ANIM_TAG_X_SIGN},
    {gBattleAnimSpriteGfx_BluegreenOrb, 0x0080, ANIM_TAG_BLUEGREEN_ORB},
    {gBattleAnimSpriteGfx_PawPrint, 0x0200, ANIM_TAG_PAW_PRINT},
    {gBattleAnimSpriteGfx_PurpleFlame, 0x0400, ANIM_TAG_PURPLE_FLAME},
    {gBattleAnimSpriteGfx_RedBall, 0x0200, ANIM_TAG_RED_BALL},
    {gBattleAnimSpriteGfx_SmellingsaltEffect, 0x0200, ANIM_TAG_SMELLINGSALT_EFFECT},
    {gBattleAnimSpriteGfx_Meteor, 0x0800, ANIM_TAG_METEOR},
    {gBattleAnimSpriteGfx_FlatRock, 0x0280, ANIM_TAG_FLAT_ROCK},
    {gBattleAnimSpriteGfx_MagnifyingGlass, 0x0200, ANIM_TAG_MAGNIFYING_GLASS},
    {gBattleAnimSpriteGfx_WaterOrb, 0x0200, ANIM_TAG_BROWN_ORB},
    {gBattleAnimSpriteGfx_MetalSoundWaves, 0x0400, ANIM_TAG_METAL_SOUND_WAVES},
    {gBattleAnimSpriteGfx_FlyingDirt, 0x0200, ANIM_TAG_FLYING_DIRT},
    {gBattleAnimSpriteGfx_IcicleSpear, 0x0200, ANIM_TAG_ICICLE_SPEAR},
    {gBattleAnimSpriteGfx_Hail, 0x0080, ANIM_TAG_HAIL},
    {gBattleAnimSpriteGfx_GlowyRedOrb, 0x0020, ANIM_TAG_GLOWY_RED_ORB},
    {gBattleAnimSpriteGfx_GlowyRedOrb, 0x0020, ANIM_TAG_GLOWY_GREEN_ORB},
    {gBattleAnimSpriteGfx_GreenSpike, 0x0080, ANIM_TAG_GREEN_SPIKE},
    {gBattleAnimSpriteGfx_CircleOfLight, 0x0800, ANIM_TAG_WHITE_CIRCLE_OF_LIGHT},
    {gBattleAnimSpriteGfx_GlowyRedOrb, 0x0020, ANIM_TAG_GLOWY_BLUE_ORB},
    {gBattleAnimSpriteGfx_Pokeblock, 0x0080, ANIM_TAG_POKEBLOCK},
    {gBattleAnimSpriteGfx_WhiteFeather, 0x0400, ANIM_TAG_WHITE_FEATHER},
    {gBattleAnimSpriteGfx_Sparkle6, 0x0080, ANIM_TAG_SPARKLE_6},
    {gBattleAnimSpriteGfx_Splash, 0x0800, ANIM_TAG_SPLASH},
    {gBattleAnimSpriteGfx_SweatBead, 0x0020, ANIM_TAG_SWEAT_BEAD},
    {gBattleAnimSpriteGfx_SlamHit2, 0x1000, ANIM_TAG_SLAM_HIT_2},
    {gBattleAnimSpriteGfx_Recycle, 0x0800, ANIM_TAG_RECYCLE},
    {gBattleAnimSpriteGfx_Protect, 0x0800, ANIM_TAG_PROTECT},
    {gBattleAnimSpriteGfx_DirtMound, 0x0200, ANIM_TAG_DIRT_MOUND},
    {gBattleAnimSpriteGfx_Shock3, 0x0600, ANIM_TAG_SHOCK_3},
    {gBattleAnimSpriteGfx_WeatherBall, 0x0200, ANIM_TAG_WEATHER_BALL},
    {gBattleAnimSpriteGfx_Bird, 0x0800, ANIM_TAG_BIRD},
    {gBattleAnimSpriteGfx_CrossImpact, 0x0200, ANIM_TAG_CROSS_IMPACT},
    {gBattleAnimSpriteGfx_Slash, 0x0800, ANIM_TAG_SLASH_2},
    {gBattleAnimSpriteGfx_SlamHit, 0x1000, ANIM_TAG_WHIP_HIT},
    {gBattleAnimSpriteGfx_GoldRing, 0x0100, ANIM_TAG_BLUE_RING_2},
};

const struct CompressedSpritePalette gBattleAnimPaletteTable[] =
{
    {gBattleAnimSpritePal_Bone, ANIM_TAG_BONE},
    {gBattleAnimSpritePal_Spark, ANIM_TAG_SPARK},
    {gBattleAnimSpritePal_Pencil, ANIM_TAG_PENCIL},
    {gBattleAnimSpritePal_AirWave, ANIM_TAG_AIR_WAVE},
    {gBattleAnimSpritePal_Orb, ANIM_TAG_ORB},
    {gBattleAnimSpritePal_Sword, ANIM_TAG_SWORD},
    {gBattleAnimSpritePal_Seed, ANIM_TAG_SEED},
    {gBattleAnimSpritePal_Explosion6, ANIM_TAG_EXPLOSION_6},
    {gBattleAnimSpritePal_Gust, ANIM_TAG_GUST},
    {gBattleAnimSpritePal_IceCube, ANIM_TAG_ICE_CUBE},
    {gBattleAnimSpritePal_Spark2, ANIM_TAG_SPARK_2},
    {gBattleAnimSpritePal_YellowBall, ANIM_TAG_YELLOW_BALL},
    {gBattleAnimSpritePal_LockOn, ANIM_TAG_LOCK_ON},
    {gBattleAnimSpritePal_TiedBag, ANIM_TAG_TIED_BAG},
    {gBattleAnimSpritePal_BlackSmoke, ANIM_TAG_BLACK_SMOKE},
    {gBattleAnimSpritePal_BlackSmoke, ANIM_TAG_BLACK_BALL},
    {gBattleAnimSpritePal_Conversion, ANIM_TAG_CONVERSION},
    {gBattleAnimSpritePal_HornHit, ANIM_TAG_HORN_HIT},
    {gBattleAnimSpritePal_Hit, ANIM_TAG_HIT},
    {gBattleAnimSpritePal_Hit2, ANIM_TAG_HIT_2},
    {gBattleAnimSpritePal_Leer, ANIM_TAG_LEER},
    {gBattleAnimSpritePal_SmallEmber, ANIM_TAG_SMALL_EMBER},
    {gBattleAnimSpritePal_GraySmoke, ANIM_TAG_GRAY_SMOKE},
    {gBattleAnimSpritePal_BlueStar, ANIM_TAG_BLUE_STAR},
    {gBattleAnimSpritePal_BubbleBurst, ANIM_TAG_BUBBLE_BURST},
    {gBattleAnimSpritePal_Fire, ANIM_TAG_FIRE},
    {gBattleAnimSpritePal_Fire, ANIM_TAG_FIRE_PLUME},
    {gBattleAnimSpritePal_Lightning, ANIM_TAG_LIGHTNING},
    {gBattleAnimSpritePal_ClawSlash, ANIM_TAG_CLAW_SLASH},
    {gBattleAnimSpritePal_IceChunk, ANIM_TAG_ICE_CHUNK},
    {gBattleAnimSpritePal_Sparkle1, ANIM_TAG_SPARKLE_1},
    {gBattleAnimSpritePal_Sparkle2, ANIM_TAG_SPARKLE_2},
    {gBattleAnimSpritePal_HumanoidFoot, ANIM_TAG_HUMANOID_FOOT},
    {gBattleAnimSpritePal_HumanoidFoot, ANIM_TAG_MONSTER_FOOT},
    {gBattleAnimSpritePal_Hit, ANIM_TAG_NOISE_LINE},
    {gBattleAnimSpritePal_SlamHit, ANIM_TAG_SLAM_HIT},
    {gBattleAnimSpritePal_Rocks, ANIM_TAG_ROCKS},
    {gBattleAnimSpritePal_Leaf, ANIM_TAG_LEAF},
    {gBattleAnimSpritePal_Finger, ANIM_TAG_FINGER},
    {gBattleAnimSpritePal_PoisonPowder, ANIM_TAG_POISON_POWDER},
    {gBattleAnimSpritePal_SleepPowder, ANIM_TAG_SLEEP_POWDER},
    {gBattleAnimSpritePal_StunSpore, ANIM_TAG_STUN_SPORE},
    {gBattleAnimSpritePal_Sparkle3, ANIM_TAG_SPARKLE_3},
    {gBattleAnimSpritePal_Sparkle3, ANIM_TAG_SPARKLE_4},
    {gBattleAnimSpritePal_MusicNotes, ANIM_TAG_MUSIC_NOTES},
    {gBattleAnimSpritePal_Duck, ANIM_TAG_DUCK},
    {gBattleAnimSpritePal_MudSand, ANIM_TAG_MUD_SAND},
    {gBattleAnimSpritePal_Alert, ANIM_TAG_ALERT},
    {gBattleAnimSpritePal_Shock, ANIM_TAG_SHOCK},
    {gBattleAnimSpritePal_Breath, ANIM_TAG_BREATH},
    {gBattleAnimSpritePal_Anger, ANIM_TAG_ANGER},
    {gBattleAnimSpritePal_Amnesia, ANIM_TAG_AMNESIA},
    {gBattleAnimSpritePal_BentSpoon, ANIM_TAG_BENT_SPOON},
    {gBattleAnimSpritePal_MilkBottle, ANIM_TAG_MILK_BOTTLE},
    {gBattleAnimSpritePal_Coin, ANIM_TAG_COIN},
    {gBattleAnimSpritePal_RainDrops, ANIM_TAG_RAIN_DROPS},
    {gBattleAnimSpritePal_Impact, ANIM_TAG_IMPACT},
    {gBattleAnimSpritePal_CircleImpact, ANIM_TAG_CIRCLE_IMPACT},
    {gBattleAnimSpritePal_Impact, ANIM_TAG_SCRATCH},
    {gBattleAnimSpritePal_Impact, ANIM_TAG_CUT},
    {gBattleAnimSpritePal_SharpTeeth, ANIM_TAG_SHARP_TEETH},
    {gBattleAnimSpritePal_RainbowRings, ANIM_TAG_RAINBOW_RINGS},
    {gBattleAnimSpritePal_IceCrystals, ANIM_TAG_ICE_CRYSTALS},
    {gBattleAnimSpritePal_IceCrystals, ANIM_TAG_ICE_SPIKES},
    {gBattleAnimSpritePal_HandsAndFeet, ANIM_TAG_HANDS_AND_FEET},
    {gBattleAnimSpritePal_MistCloud, ANIM_TAG_MIST_CLOUD},
    {gBattleAnimSpritePal_SharpTeeth, ANIM_TAG_CLAMP},
    {gBattleAnimSpritePal_RainDrops, ANIM_TAG_BUBBLE},
    {gBattleAnimSpritePal_Orbs, ANIM_TAG_ORBS},
    {gBattleAnimSpritePal_WaterImpact, ANIM_TAG_WATER_IMPACT},
    {gBattleAnimSpritePal_WaterImpact, ANIM_TAG_WATER_ORB},
    {gBattleAnimSpritePal_PoisonBubble, ANIM_TAG_POISON_BUBBLE},
    {gBattleAnimSpritePal_PoisonBubble, ANIM_TAG_TOXIC_BUBBLE},
    {gBattleAnimSpritePal_Spikes, ANIM_TAG_SPIKES},
    {gBattleAnimSpritePal_HornHit2, ANIM_TAG_HORN_HIT_2},
    {gBattleAnimSpritePal_AirWave2, ANIM_TAG_AIR_WAVE_2},
    {gBattleAnimSpritePal_SmallBubbles, ANIM_TAG_SMALL_BUBBLES},
    {gBattleAnimSpritePal_RoundShadow, ANIM_TAG_ROUND_SHADOW},
    {gBattleAnimSpritePal_Sunlight, ANIM_TAG_SUNLIGHT},
    {gBattleAnimSpritePal_Spore, ANIM_TAG_SPORE},
    {gBattleAnimSpritePal_Flower, ANIM_TAG_FLOWER},
    {gBattleAnimSpritePal_RazorLeaf, ANIM_TAG_RAZOR_LEAF},
    {gBattleAnimSpritePal_Needle, ANIM_TAG_NEEDLE},
    {gBattleAnimSpritePal_WhirlwindLines, ANIM_TAG_WHIRLWIND_LINES},
    {gBattleAnimSpritePal_GoldRing, ANIM_TAG_GOLD_RING},
    {gBattleAnimSpritePal_PurpleRing, ANIM_TAG_PURPLE_RING},
    {gBattleAnimSpritePal_BlueRing, ANIM_TAG_BLUE_RING},
    {gBattleAnimSpritePal_GreenLightWall, ANIM_TAG_GREEN_LIGHT_WALL},
    {gBattleAnimSpritePal_BlueLightWall, ANIM_TAG_BLUE_LIGHT_WALL},
    {gBattleAnimSpritePal_RedLightWall, ANIM_TAG_RED_LIGHT_WALL},
    {gBattleAnimSpritePal_GrayLightWall, ANIM_TAG_GRAY_LIGHT_WALL},
    {gBattleAnimSpritePal_OrangeLightWall, ANIM_TAG_ORANGE_LIGHT_WALL},
    {gBattleAnimSpritePal_BlackBall2, ANIM_TAG_BLACK_BALL_2},
    {gBattleAnimSpritePal_PurpleGasCloud, ANIM_TAG_PURPLE_GAS_CLOUD},
    {gBattleAnimSpritePal_Spark, ANIM_TAG_SPARK_H},
    {gBattleAnimSpritePal_YellowStar, ANIM_TAG_YELLOW_STAR},
    {gBattleAnimSpritePal_LargeFreshEgg, ANIM_TAG_LARGE_FRESH_EGG},
    {gBattleAnimSpritePal_ShadowBall, ANIM_TAG_SHADOW_BALL},
    {gBattleAnimSpritePal_Lick, ANIM_TAG_LICK},
    {gBattleAnimSpritePal_VoidLines, ANIM_TAG_VOID_LINES},
    {gBattleAnimSpritePal_String, ANIM_TAG_STRING},
    {gBattleAnimSpritePal_String, ANIM_TAG_WEB_THREAD},
    {gBattleAnimSpritePal_String, ANIM_TAG_SPIDER_WEB},
    {gBattleAnimSpritePal_Slash, ANIM_TAG_SLASH},
    {gBattleAnimSpritePal_FocusEnergy, ANIM_TAG_FOCUS_ENERGY},
    {gBattleAnimSpritePal_SphereToCube, ANIM_TAG_SPHERE_TO_CUBE},
    {gBattleAnimSpritePal_Tendrils, ANIM_TAG_TENDRILS},
    {gBattleAnimSpritePal_Eye, ANIM_TAG_EYE},
    {gBattleAnimSpritePal_WhiteShadow, ANIM_TAG_WHITE_SHADOW},
    {gBattleAnimSpritePal_TealAlert, ANIM_TAG_TEAL_ALERT},
    {gBattleAnimSpritePal_OpeningEye, ANIM_TAG_OPENING_EYE},
    {gBattleAnimSpritePal_RoundWhiteHalo, ANIM_TAG_ROUND_WHITE_HALO},
    {gBattleAnimSpritePal_FangAttack, ANIM_TAG_FANG_ATTACK},
    {gBattleAnimSpritePal_PurpleHandOutline, ANIM_TAG_PURPLE_HAND_OUTLINE},
    {gBattleAnimSpritePal_Moon, ANIM_TAG_MOON},
    {gBattleAnimSpritePal_GreenSparkle, ANIM_TAG_GREEN_SPARKLE},
    {gBattleAnimSpritePal_Spiral, ANIM_TAG_SPIRAL},
    {gBattleAnimSpritePal_SnoreZ, ANIM_TAG_SNORE_Z},
    {gBattleAnimSpritePal_Explosion, ANIM_TAG_EXPLOSION},
    {gBattleAnimSpritePal_Nail, ANIM_TAG_NAIL},
    {gBattleAnimSpritePal_GhostlySpirit, ANIM_TAG_GHOSTLY_SPIRIT},
    {gBattleAnimSpritePal_WarmRock, ANIM_TAG_WARM_ROCK},
    {gBattleAnimSpritePal_BreakingEgg, ANIM_TAG_BREAKING_EGG},
    {gBattleAnimSpritePal_ThinRing, ANIM_TAG_THIN_RING},
    {gBattleAnimSpritePal_Bell, ANIM_TAG_BELL},
    {gBattleAnimSpritePal_MusicNotes2, ANIM_TAG_MUSIC_NOTES_2},
    {gBattleAnimSpritePal_SpeedDust, ANIM_TAG_SPEED_DUST},
    {gBattleAnimSpritePal_BlueLightWall, ANIM_TAG_TORN_METAL},
    {gBattleAnimSpritePal_ThoughtBubble, ANIM_TAG_THOUGHT_BUBBLE},
    {gBattleAnimSpritePal_MagentaHeart, ANIM_TAG_MAGENTA_HEART},
    {gBattleAnimSpritePal_ElectricOrbs, ANIM_TAG_ELECTRIC_ORBS},
    {gBattleAnimSpritePal_ElectricOrbs, ANIM_TAG_CIRCLE_OF_LIGHT},
    {gBattleAnimSpritePal_ElectricOrbs, ANIM_TAG_ELECTRICITY},
    {gBattleAnimSpritePal_Finger, ANIM_TAG_FINGER_2},
    {gBattleAnimSpritePal_MovementWaves, ANIM_TAG_MOVEMENT_WAVES},
    {gBattleAnimSpritePal_RedHeart, ANIM_TAG_RED_HEART},
    {gBattleAnimSpritePal_RedOrb, ANIM_TAG_RED_ORB},
    {gBattleAnimSpritePal_EyeSparkle, ANIM_TAG_EYE_SPARKLE},
    {gBattleAnimSpritePal_PinkHeart, ANIM_TAG_PINK_HEART},
    {gBattleAnimSpritePal_Angel, ANIM_TAG_ANGEL},
    {gBattleAnimSpritePal_Devil, ANIM_TAG_DEVIL},
    {gBattleAnimSpritePal_Swipe, ANIM_TAG_SWIPE},
    {gBattleAnimSpritePal_Roots, ANIM_TAG_ROOTS},
    {gBattleAnimSpritePal_ItemBag, ANIM_TAG_ITEM_BAG},
    {gBattleAnimSpritePal_JaggedMusicNote, ANIM_TAG_JAGGED_MUSIC_NOTE},
    {gBattleAnimSpritePal_Pokeball, ANIM_TAG_POKEBALL},
    {gBattleAnimSpritePal_Pokeball, ANIM_TAG_SPOTLIGHT},
    {gBattleAnimSpritePal_LetterZ, ANIM_TAG_LETTER_Z},
    {gBattleAnimSpritePal_RapidSpin, ANIM_TAG_RAPID_SPIN},
    {gBattleAnimSpritePal_TriAttackTriangle, ANIM_TAG_TRI_ATTACK_TRIANGLE},
    {gBattleAnimSpritePal_WispOrb, ANIM_TAG_WISP_ORB},
    {gBattleAnimSpritePal_WispOrb, ANIM_TAG_WISP_FIRE},
    {gBattleAnimSpritePal_GoldStars, ANIM_TAG_SHINY_SQUARES},
    {gBattleAnimSpritePal_GoldStars, ANIM_TAG_GOLD_STARS},
    {gBattleAnimSpritePal_EclipsingOrb, ANIM_TAG_ECLIPSING_ORB},
    {gBattleAnimSpritePal_GrayOrb, ANIM_TAG_GRAY_ORB},
    {gBattleAnimSpritePal_BlueOrb, ANIM_TAG_BLUE_ORB},
    {gBattleAnimSpritePal_RedOrb2, ANIM_TAG_RED_ORB_2},
    {gBattleAnimSpritePal_PinkPetal, ANIM_TAG_PINK_PETAL},
    {gBattleAnimSpritePal_PainSplit, ANIM_TAG_PAIN_SPLIT},
    {gBattleAnimSpritePal_Confetti, ANIM_TAG_CONFETTI},
    {gBattleAnimSpritePal_GreenStar, ANIM_TAG_GREEN_STAR},
    {gBattleAnimSpritePal_PinkCloud, ANIM_TAG_PINK_CLOUD},
    {gBattleAnimSpritePal_SweatDrop, ANIM_TAG_SWEAT_DROP},
    {gBattleAnimSpritePal_GuardRing, ANIM_TAG_GUARD_RING},
    {gBattleAnimSpritePal_PurpleScratch, ANIM_TAG_PURPLE_SCRATCH},
    {gBattleAnimSpritePal_PurpleScratch, ANIM_TAG_PURPLE_SWIPE},
    {gBattleAnimSpritePal_Finger, ANIM_TAG_TAG_HAND},
    {gBattleAnimSpritePal_SmallRedEye, ANIM_TAG_SMALL_RED_EYE},
    {gBattleAnimSpritePal_HollowOrb, ANIM_TAG_HOLLOW_ORB},
    {gBattleAnimSpritePal_HollowOrb, ANIM_TAG_X_SIGN},
    {gBattleAnimSpritePal_BluegreenOrb, ANIM_TAG_BLUEGREEN_ORB},
    {gBattleAnimSpritePal_PawPrint, ANIM_TAG_PAW_PRINT},
    {gBattleAnimSpritePal_PurpleFlame, ANIM_TAG_PURPLE_FLAME},
    {gBattleAnimSpritePal_RedBall, ANIM_TAG_RED_BALL},
    {gBattleAnimSpritePal_SmellingsaltEffect, ANIM_TAG_SMELLINGSALT_EFFECT},
    {gBattleAnimSpritePal_Meteor, ANIM_TAG_METEOR},
    {gBattleAnimSpritePal_FlatRock, ANIM_TAG_FLAT_ROCK},
    {gBattleAnimSpritePal_MagnifyingGlass, ANIM_TAG_MAGNIFYING_GLASS},
    {gBattleAnimSpritePal_BrownOrb, ANIM_TAG_BROWN_ORB},
    {gBattleAnimSpritePal_MetalSoundWaves, ANIM_TAG_METAL_SOUND_WAVES},
    {gBattleAnimSpritePal_FlyingDirt, ANIM_TAG_FLYING_DIRT},
    {gBattleAnimSpritePal_IcicleSpear, ANIM_TAG_ICICLE_SPEAR},
    {gBattleAnimSpritePal_Hail, ANIM_TAG_HAIL},
    {gBattleAnimSpritePal_GlowyRedOrb, ANIM_TAG_GLOWY_RED_ORB},
    {gBattleAnimSpritePal_GlowyGreenOrb, ANIM_TAG_GLOWY_GREEN_ORB},
    {gBattleAnimSpritePal_GreenSpike, ANIM_TAG_GREEN_SPIKE},
    {gBattleAnimSpritePal_WhiteCircleOfLight, ANIM_TAG_WHITE_CIRCLE_OF_LIGHT},
    {gBattleAnimSpritePal_GlowyBlueOrb, ANIM_TAG_GLOWY_BLUE_ORB},
    {gBattleAnimSpritePal_Pokeblock, ANIM_TAG_POKEBLOCK},
    {gBattleAnimSpritePal_WhiteFeather, ANIM_TAG_WHITE_FEATHER},
    {gBattleAnimSpritePal_Sparkle6, ANIM_TAG_SPARKLE_6},
    {gBattleAnimSpritePal_Splash, ANIM_TAG_SPLASH},
    {gBattleAnimSpritePal_Splash, ANIM_TAG_SWEAT_BEAD},
    {gBattleAnimSpritePal_SlamHit2, ANIM_TAG_SLAM_HIT_2},
    {gBattleAnimSpritePal_Recycle, ANIM_TAG_RECYCLE},
    {gBattleAnimSpritePal_Protect, ANIM_TAG_PROTECT},
    {gBattleAnimSpritePal_DirtMound, ANIM_TAG_DIRT_MOUND},
    {gBattleAnimSpritePal_Shock3, ANIM_TAG_SHOCK_3},
    {gBattleAnimSpritePal_WeatherBall, ANIM_TAG_WEATHER_BALL},
    {gBattleAnimSpritePal_Bird, ANIM_TAG_BIRD},
    {gBattleAnimSpritePal_CrossImpact, ANIM_TAG_CROSS_IMPACT},
    {gBattleAnimSpritePal_Slash2, ANIM_TAG_SLASH_2},
    {gBattleAnimSpritePal_WhipHit, ANIM_TAG_WHIP_HIT},
    {gBattleAnimSpritePal_BlueRing2, ANIM_TAG_BLUE_RING_2},
};

const struct BattleAnimBackground gBattleAnimBackgroundTable[] =
{
    [BG_DARK_] = {gBattleAnimBgImage_Dark, gBattleAnimBgPalette_Dark, gBattleAnimBgTilemap_Dark},
    [BG_DARK] = {gBattleAnimBgImage_Dark, gBattleAnimBgPalette_Dark, gBattleAnimBgTilemap_Dark},
    [BG_GHOST] = {gBattleAnimBgImage_Ghost, gBattleAnimBgPalette_Ghost, gBattleAnimBgTilemap_Ghost},
    [BG_PSYCHIC] = {gBattleAnimBgImage_Psychic, gBattleAnimBgPalette_Psychic, gBattleAnimBgTilemap_Psychic},
    [BG_IMPACT_OPPONENT] = {gBattleAnimBgImage_Impact, gBattleAnimBgPalette_Impact, gBattleAnimBgTilemap_ImpactOpponent},
    [BG_IMPACT_PLAYER] = {gBattleAnimBgImage_Impact, gBattleAnimBgPalette_Impact, gBattleAnimBgTilemap_ImpactPlayer},
    [BG_DRILL] = {gBattleAnimBgImage_Drill, gBattleAnimBgPalette_Drill, gBattleAnimBgTilemap_Drill},
    [BG_HIGHSPEED_OPPONENT] = {gBattleAnimBgImage_Highspeed, gBattleAnimBgPalette_Highspeed, gBattleAnimBgTilemap_HighspeedOpponent},
    [BG_HIGHSPEED_PLAYER] = {gBattleAnimBgImage_Highspeed, gBattleAnimBgPalette_Highspeed, gBattleAnimBgTilemap_HighspeedPlayer},
    [BG_THUNDER] = {gBattleAnimBgImage_Thunder, gBattleAnimBgPalette_Thunder, gBattleAnimBgTilemap_Thunder},
    [BG_GUILLOTINE_OPPONENT] = {gBattleAnimBgImage_Guillotine, gBattleAnimBgPalette_Guillotine, gBattleAnimBgTilemap_GuillotineOpponent},
    [BG_GUILLOTINE_PLAYER] = {gBattleAnimBgImage_Guillotine, gBattleAnimBgPalette_Guillotine, gBattleAnimBgTilemap_GuillotinePlayer},
    [BG_ICE] = {gBattleAnimBgImage_Ice, gBattleAnimBgPalette_Ice, gBattleAnimBgTilemap_Ice},
    [BG_COSMIC] = {gBattleAnimBgImage_Cosmic, gBattleAnimBgPalette_Cosmic, gBattleAnimBgTilemap_Cosmic},
    [BG_IN_AIR] = {gBattleAnimBgImage_InAir, gBattleAnimBgPalette_InAir, gBattleAnimBgTilemap_InAir},
    [BG_SKY] = {gBattleAnimBgImage_Drill, gBattleAnimBgPalette_Sky, gBattleAnimBgTilemap_Drill},
    [BG_AURORA] = {gBattleAnimBgImage_Aurora, gBattleAnimBgPalette_Aurora, gBattleAnimBgTilemap_Aurora},
    [BG_FISSURE] = {gBattleAnimBgImage_Fissure, gBattleAnimBgPalette_Fissure, gBattleAnimBgTilemap_Fissure},
    [BG_BUG_OPPONENT] = {gBattleAnimBgImage_Highspeed, gBattleAnimBgPalette_Bug, gBattleAnimBgTilemap_HighspeedOpponent},
    [BG_BUG_PLAYER] = {gBattleAnimBgImage_Highspeed, gBattleAnimBgPalette_Bug, gBattleAnimBgTilemap_HighspeedPlayer},
    [BG_SOLARBEAM_OPPONENT] = {gBattleAnimBgImage_Impact, gBattleAnimBgPalette_Solarbeam, gBattleAnimBgTilemap_ImpactOpponent},
    [BG_SOLARBEAM_PLAYER] = {gBattleAnimBgImage_Impact, gBattleAnimBgPalette_Solarbeam, gBattleAnimBgTilemap_ImpactPlayer},
};

static void (* const sScriptCmdTable[])(void) =
{
    ScriptCmd_loadspritegfx,
    ScriptCmd_unloadspritegfx,
    ScriptCmd_createsprite,
    ScriptCmd_createvisualtask,
    ScriptCmd_delay,
    ScriptCmd_waitforvisualfinish,
    ScriptCmd_hang,
    ScriptCmd_hang,
    ScriptCmd_end,
    ScriptCmd_playse,
    ScriptCmd_monbg,
    ScriptCmd_clearmonbg,
    ScriptCmd_setalpha,
    ScriptCmd_blendoff,
    ScriptCmd_call,
    ScriptCmd_return,
    ScriptCmd_setarg,
    ScriptCmd_choosetwoturnanim,
    ScriptCmd_jumpifmoveturn,
    ScriptCmd_goto,
    ScriptCmd_fadetobg,
    ScriptCmd_restorebg,
    ScriptCmd_waitbgfadeout,
    ScriptCmd_waitbgfadein,
    ScriptCmd_changebg,
    ScriptCmd_playsewithpan,
    ScriptCmd_setpan,
    ScriptCmd_panse_1B,
    ScriptCmd_loopsewithpan,
    ScriptCmd_waitplaysewithpan,
    ScriptCmd_setbldcnt,
    ScriptCmd_createsoundtask,
    ScriptCmd_waitsound,
    ScriptCmd_jumpargeq,
    ScriptCmd_monbg_22,
    ScriptCmd_clearmonbg_23,
    ScriptCmd_fadetobgfromset,
    ScriptCmd_panse_26,
    ScriptCmd_panse_27,
    ScriptCmd_monbgprio_28,
    ScriptCmd_monbgprio_29,
    ScriptCmd_monbgprio_2A,
    ScriptCmd_invisible,
    ScriptCmd_visible,
    ScriptCmd_doublebattle_2D,
    ScriptCmd_doublebattle_2E,
    ScriptCmd_stopsound
};

// code
void ClearBattleAnimationVars(void)
{
    s32 i;

    gAnimFramesToWait = 0;
    gAnimScriptActive = FALSE;
    gAnimVisualTaskCount = 0;
    gAnimSoundTaskCount = 0;
    gAnimDisableStructPtr = NULL;
    gAnimMoveDmg = 0;
    gAnimMovePower = 0;
    gAnimFriendship = 0;

    // Clear index array.
    for (i = 0; i < ANIM_SPRITE_INDEX_COUNT; i++)
        sAnimSpriteIndexArray[i] = 0xFFFF;

    // Clear anim args.
    for (i = 0; i < ANIM_ARGS_COUNT; i++)
        gBattleAnimArgs[i] = 0;

    sMonAnimTaskIdArray[0] = TASK_NONE;
    sMonAnimTaskIdArray[1] = TASK_NONE;
    gAnimMoveTurn = 0;
    sAnimBackgroundFadeState = 0;
    sAnimMoveIndex = 0;
    gBattleAnimAttacker = 0;
    gBattleAnimTarget = 0;
    gAnimCustomPanning = 0;
}

void DoMoveAnim(u16 move)
{
    gBattleAnimAttacker = gBattlerAttacker;
    gBattleAnimTarget = gBattlerTarget;
    LaunchBattleAnimation(gBattleAnims_Moves, move, TRUE);
}

void LaunchBattleAnimation(const u8 *const animsTable[], u16 tableId, bool8 isMoveAnim)
{
    s32 i;
    bool32 hideHpBoxes = TRUE;

    if (!isMoveAnim)
    {
        switch (tableId)
        {
        case B_ANIM_TURN_TRAP:
        case B_ANIM_LEECH_SEED_DRAIN:
        case B_ANIM_MON_HIT:
        case B_ANIM_SNATCH_MOVE:
        case B_ANIM_FUTURE_SIGHT_HIT:
        case B_ANIM_DOOM_DESIRE_HIT:
        case B_ANIM_WISH_HEAL:
            hideHpBoxes = TRUE;
            break;
        default:
            hideHpBoxes = FALSE;
            break;
        }
    }
    sub_80A8278();
    UpdateOamPriorityInAllHealthboxes(0, hideHpBoxes);
    for (i = 0; i < MAX_BATTLERS_COUNT; i++)
    {
        if (GetBattlerSide(i))
            gAnimBattlerSpecies[i] = GetMonData(&gEnemyParty[gBattlerPartyIndexes[i]], MON_DATA_SPECIES);
        else
            gAnimBattlerSpecies[i] = GetMonData(&gPlayerParty[gBattlerPartyIndexes[i]], MON_DATA_SPECIES);
    }

    sAnimMoveIndex = (isMoveAnim) ? tableId : 0;

    for (i = 0; i < ANIM_ARGS_COUNT; i++)
        gBattleAnimArgs[i] = 0;

    sMonAnimTaskIdArray[0] = TASK_NONE;
    sMonAnimTaskIdArray[1] = TASK_NONE;
    sBattleAnimScriptPtr = animsTable[tableId];
    gAnimScriptActive = TRUE;
    gAnimFramesToWait = 0;
    gAnimScriptCallback = RunAnimScriptCommand;

    for (i = 0; i < ANIM_SPRITE_INDEX_COUNT; i++)
        sAnimSpriteIndexArray[i] = 0xFFFF;

    if (isMoveAnim)
    {
        for (i = 0; gMovesWithQuietBGM[i] != 0xFFFF; i++)
        {
            if (tableId == gMovesWithQuietBGM[i])
            {
                m4aMPlayVolumeControl(&gMPlayInfo_BGM, 0xFFFF, 128);
                break;
            }
        }
    }

    gBattle_WIN0H = 0;
    gBattle_WIN0V = 0;
    gBattle_WIN1H = 0;
    gBattle_WIN1V = 0;
}

void DestroyAnimSprite(struct Sprite *sprite)
{
    FreeSpriteOamMatrix(sprite);
    DestroySprite(sprite);
    gAnimVisualTaskCount--;
}

void DestroyAnimVisualTask(u8 taskId)
{
    DestroyTask(taskId);
    gAnimVisualTaskCount--;
}

void DestroyAnimSoundTask(u8 taskId)
{
    DestroyTask(taskId);
    gAnimSoundTaskCount--;
}

static void AddSpriteIndex(u16 index)
{
    s32 i;

    for (i = 0; i < ANIM_SPRITE_INDEX_COUNT; i++)
    {
        if (sAnimSpriteIndexArray[i] == 0xFFFF)
        {
            sAnimSpriteIndexArray[i] = index;
            return;
        }
    }
}

static void ClearSpriteIndex(u16 index)
{
    s32 i;

    for (i = 0; i < ANIM_SPRITE_INDEX_COUNT; i++)
    {
        if (sAnimSpriteIndexArray[i] == index)
        {
            sAnimSpriteIndexArray[i] = 0xFFFF;
            return;
        }
    }
}

static void WaitAnimFrameCount(void)
{
    if (gAnimFramesToWait <= 0)
    {
        gAnimScriptCallback = RunAnimScriptCommand;
        gAnimFramesToWait = 0;
    }
    else
    {
        gAnimFramesToWait--;
    }
}

static void RunAnimScriptCommand(void)
{
    do
    {
        sScriptCmdTable[sBattleAnimScriptPtr[0]]();
    } while (gAnimFramesToWait == 0 && gAnimScriptActive);
}

static void ScriptCmd_loadspritegfx(void)
{
    u16 index;

    sBattleAnimScriptPtr++;
    index = T1_READ_16(sBattleAnimScriptPtr);
    LoadCompressedSpriteSheetUsingHeap(&gBattleAnimPicTable[GET_TRUE_SPRITE_INDEX(index)]);
    LoadCompressedSpritePaletteUsingHeap(&gBattleAnimPaletteTable[GET_TRUE_SPRITE_INDEX(index)]);
    sBattleAnimScriptPtr += 2;
    AddSpriteIndex(GET_TRUE_SPRITE_INDEX(index));
    gAnimFramesToWait = 1;
    gAnimScriptCallback = WaitAnimFrameCount;
}

static void ScriptCmd_unloadspritegfx(void)
{
    u16 index;

    sBattleAnimScriptPtr++;
    index = T1_READ_16(sBattleAnimScriptPtr);
    FreeSpriteTilesByTag(gBattleAnimPicTable[GET_TRUE_SPRITE_INDEX(index)].tag);
    FreeSpritePaletteByTag(gBattleAnimPicTable[GET_TRUE_SPRITE_INDEX(index)].tag);
    sBattleAnimScriptPtr += 2;
    ClearSpriteIndex(GET_TRUE_SPRITE_INDEX(index));
}

static void ScriptCmd_createsprite(void)
{
    s32 i;
    const struct SpriteTemplate *template;
    u8 argVar;
    u8 argsCount;
    s16 subpriority;

    sBattleAnimScriptPtr++;
    template = (const struct SpriteTemplate *)(T2_READ_32(sBattleAnimScriptPtr));
    sBattleAnimScriptPtr += 4;

    argVar = sBattleAnimScriptPtr[0];
    sBattleAnimScriptPtr++;

    argsCount = sBattleAnimScriptPtr[0];
    sBattleAnimScriptPtr++;
    for (i = 0; i < argsCount; i++)
    {
        gBattleAnimArgs[i] = T1_READ_16(sBattleAnimScriptPtr);
        sBattleAnimScriptPtr += 2;
    }

    if (argVar & 0x80)
    {
        argVar ^= 0x80;
        if (argVar >= 0x40)
            argVar -= 0x40;
        else
            argVar *= -1;

        subpriority = GetBattlerSpriteSubpriority(gBattleAnimTarget) + (s8)(argVar);
    }
    else
    {
        if (argVar >= 0x40)
            argVar -= 0x40;
        else
            argVar *= -1;

        subpriority = GetBattlerSpriteSubpriority(gBattleAnimAttacker) + (s8)(argVar);
    }

    if (subpriority < 3)
        subpriority = 3;

    CreateSpriteAndAnimate(
        template,
        GetBattlerSpriteCoord(gBattleAnimTarget, BATTLER_COORD_X_2),
        GetBattlerSpriteCoord(gBattleAnimTarget, BATTLER_COORD_Y_PIC_OFFSET),
        subpriority);
    gAnimVisualTaskCount++;
}

static void ScriptCmd_createvisualtask(void)
{
    TaskFunc taskFunc;
    u8 taskPriority;
    u8 taskId;
    u8 numArgs;
    s32 i;

    sBattleAnimScriptPtr++;

    taskFunc = (TaskFunc)T2_READ_32(sBattleAnimScriptPtr);
    sBattleAnimScriptPtr += 4;

    taskPriority = sBattleAnimScriptPtr[0];
    sBattleAnimScriptPtr++;

    numArgs = sBattleAnimScriptPtr[0];
    sBattleAnimScriptPtr++;

    for (i = 0; i < numArgs; i++)
    {
        gBattleAnimArgs[i] = T1_READ_16(sBattleAnimScriptPtr);
        sBattleAnimScriptPtr += 2;
    }

    taskId = CreateTask(taskFunc, taskPriority);
    taskFunc(taskId);
    gAnimVisualTaskCount++;
}

static void ScriptCmd_delay(void)
{
    sBattleAnimScriptPtr++;
    gAnimFramesToWait = sBattleAnimScriptPtr[0];
    if (gAnimFramesToWait == 0)
        gAnimFramesToWait = -1;
    sBattleAnimScriptPtr++;
    gAnimScriptCallback = WaitAnimFrameCount;
}

// Wait for visual tasks to finish.
static void ScriptCmd_waitforvisualfinish(void)
{
    if (gAnimVisualTaskCount)
    {
        gAnimFramesToWait = 1;
    }
    else
    {
        sBattleAnimScriptPtr++;
        gAnimFramesToWait = 0;
    }
}

static void ScriptCmd_hang(void)
{
}

static void ScriptCmd_end(void)
{
    s32 i;
    bool32 continuousAnim = FALSE;

    // Keep waiting as long as there are animations to be done.
    if (gAnimVisualTaskCount || gAnimSoundTaskCount
     || sMonAnimTaskIdArray[0] != TASK_NONE || sMonAnimTaskIdArray[1] != TASK_NONE)
    {
        sSoundAnimFramesToWait = 0;
        gAnimFramesToWait = 1;
        return;
    }

    // Finish the sound effects.
    if (IsSEPlaying())
    {
        if (++sSoundAnimFramesToWait <= 90) // Wait 90 frames, then halt the sound effect.
        {
            gAnimFramesToWait = 1;
            return;
        }
        else
        {
            m4aMPlayStop(&gMPlayInfo_SE1);
            m4aMPlayStop(&gMPlayInfo_SE2);
        }
    }

    // The SE has halted, so set the SE Frame Counter to 0 and continue.
    sSoundAnimFramesToWait = 0;

    for (i = 0; i < ANIM_SPRITE_INDEX_COUNT; i++)
    {
        if (sAnimSpriteIndexArray[i] != 0xFFFF)
        {
            FreeSpriteTilesByTag(gBattleAnimPicTable[sAnimSpriteIndexArray[i]].tag);
            FreeSpritePaletteByTag(gBattleAnimPicTable[sAnimSpriteIndexArray[i]].tag);
            sAnimSpriteIndexArray[i] = 0xFFFF; // set terminator.
        }
    }

    if (!continuousAnim) // May have been used for debug?
    {
        m4aMPlayVolumeControl(&gMPlayInfo_BGM, 0xFFFF, 256);
        sub_80A8278();
        UpdateOamPriorityInAllHealthboxesInvisibility(1);
        gAnimScriptActive = FALSE;
    }
}

static void ScriptCmd_playse(void)
{
    sBattleAnimScriptPtr++;
    PlaySE(T1_READ_16(sBattleAnimScriptPtr));
    sBattleAnimScriptPtr += 2;
}

#define t1_MONBG_BATTLER        0
#define t1_MON_IN_BG2           1
#define t1_CREATE_ANOTHER_TASK  2
#define t1_IS_SECONDMON_BG      3

#define t2_BATTLER_SPRITE_ID    0
#define t2_MON_IN_BG2           5
#define t2_MONBG_BATTLER        6

static void sub_80A40F4(u8 taskId)
{
    u8 newTaskId;

    s16 *selfData = gTasks[taskId].data;
    u8 battlerSpriteId = gBattlerSpriteIds[selfData[t1_MONBG_BATTLER]];
    gSprites[battlerSpriteId].invisible = TRUE;

    if (!selfData[t1_CREATE_ANOTHER_TASK])
    {
        DestroyAnimVisualTask(taskId);
        return;
    }

    newTaskId = CreateTask(task_pA_ma0A_obj_to_bg_pal, 10);
    gTasks[newTaskId].data[t2_BATTLER_SPRITE_ID] = battlerSpriteId;
    gTasks[newTaskId].data[1] = gSprites[battlerSpriteId].pos1.x + gSprites[battlerSpriteId].pos2.x;
    gTasks[newTaskId].data[2] = gSprites[battlerSpriteId].pos1.y + gSprites[battlerSpriteId].pos2.y;

    if (selfData[t1_MON_IN_BG2])
    {
        gTasks[newTaskId].data[3] = gBattle_BG2_X;
        gTasks[newTaskId].data[4] = gBattle_BG2_Y;
    }
    else
    {
        gTasks[newTaskId].data[3] = gBattle_BG1_X;
        gTasks[newTaskId].data[4] = gBattle_BG1_Y;
    }

    gTasks[newTaskId].data[t2_MON_IN_BG2] = selfData[t1_MON_IN_BG2];
    gTasks[newTaskId].data[t2_MONBG_BATTLER] = selfData[t1_MONBG_BATTLER];
    sMonAnimTaskIdArray[selfData[t1_IS_SECONDMON_BG]] = newTaskId;
    DestroyAnimVisualTask(taskId);
}

static void ScriptCmd_monbg(void)
{
    bool8 toBG_2;
    u8 taskId;
    u8 battlerId;
    u8 animBattler;

    sBattleAnimScriptPtr++;

    animBattler = sBattleAnimScriptPtr[0];
    battlerId = (animBattler & ANIM_TARGET) ? gBattleAnimTarget : gBattleAnimAttacker;

    if (IsBattlerSpriteVisible(battlerId))
    {
        u8 position = GetBattlerPosition(battlerId);
        toBG_2 = (position == B_POSITION_OPPONENT_LEFT || position == B_POSITION_PLAYER_RIGHT) ? FALSE : TRUE;

        MoveBattlerSpriteToBG(battlerId, toBG_2, FALSE);
        taskId = CreateTask(sub_80A40F4, 10);
        gAnimVisualTaskCount++;
        gTasks[taskId].data[t1_MONBG_BATTLER] = battlerId;
        gTasks[taskId].data[t1_MON_IN_BG2] = toBG_2;
        gTasks[taskId].data[t1_CREATE_ANOTHER_TASK] = TRUE;
        gTasks[taskId].data[t1_IS_SECONDMON_BG] = 0;

    }

    battlerId ^= BIT_FLANK;
    if (IsBattlerSpriteVisible(battlerId))
    {
        u8 position = GetBattlerPosition(battlerId);
        toBG_2 = (position == B_POSITION_OPPONENT_LEFT || position == B_POSITION_PLAYER_RIGHT) ? FALSE : TRUE;

        MoveBattlerSpriteToBG(battlerId, toBG_2, FALSE);
        taskId = CreateTask(sub_80A40F4, 10);
        gAnimVisualTaskCount++;
        gTasks[taskId].data[0] = battlerId;
        gTasks[taskId].data[1] = toBG_2;
        gTasks[taskId].data[t1_CREATE_ANOTHER_TASK] = TRUE;
        gTasks[taskId].data[t1_IS_SECONDMON_BG] = 1;
    }

    sBattleAnimScriptPtr++;
    gAnimFramesToWait = 1;
    gAnimScriptCallback = WaitAnimFrameCount;
}

bool8 IsBattlerSpriteVisible(u8 battlerId)
{
    if (!IsBattlerSpritePresent(battlerId))
        return FALSE;
    if (!gBattleSpritesDataPtr->battlerData[battlerId].invisible || !gSprites[gBattlerSpriteIds[battlerId]].invisible)
        return TRUE;

    return FALSE;
}

void MoveBattlerSpriteToBG(u8 battlerId, bool8 toBG_2, bool8 setSpriteInvisible)
{
    struct BattleAnimBgData animBg;
    u8 battlerSpriteId;

    if (toBG_2)
    {
        RequestDma3Fill(0, (void*)(BG_SCREEN_ADDR(12)), 0x2000, 1);
        RequestDma3Fill(0, (void*)(BG_SCREEN_ADDR(30)), 0x1000, 1);
        GetBattleAnimBgData(&animBg, 2);
        CpuFill16(0, animBg.bgTiles + 0x1000, 0x1000);
        CpuFill16(0, animBg.bgTilemap + 0x400, 0x800);
        SetAnimBgAttribute(2, BG_ANIM_PRIORITY, 2);
        SetAnimBgAttribute(2, BG_ANIM_SCREEN_SIZE, 1);
        SetAnimBgAttribute(2, BG_ANIM_AREA_OVERFLOW_MODE, 0);

        battlerSpriteId = gBattlerSpriteIds[battlerId];

        gBattle_BG2_X =  -(gSprites[battlerSpriteId].pos1.x + gSprites[battlerSpriteId].pos2.x) + 0x20;
        gBattle_BG2_Y =  -(gSprites[battlerSpriteId].pos1.y + gSprites[battlerSpriteId].pos2.y) + 0x20;

        if (setSpriteInvisible)
            gSprites[gBattlerSpriteIds[battlerId]].invisible = TRUE;

        SetGpuReg(REG_OFFSET_BG2HOFS, gBattle_BG2_X);
        SetGpuReg(REG_OFFSET_BG2VOFS, gBattle_BG2_Y);

        LoadPalette(&gPlttBufferUnfaded[0x100 + battlerId * 16], 0x90, 0x20);
        CpuCopy32(&gPlttBufferUnfaded[0x100 + battlerId * 16], (void*)(BG_PLTT + 0x120), 0x20);

        sub_8118FBC(2, 0, 0, GetBattlerPosition(battlerId), animBg.paletteId, animBg.bgTiles + 0x1000, animBg.bgTilemap + 0x400, animBg.tilesOffset);
    }
    else
    {
        u8 battlerPosition;

        RequestDma3Fill(0, (void*)(BG_SCREEN_ADDR(8)), 0x2000, 1);
        RequestDma3Fill(0, (void*)(BG_SCREEN_ADDR(28)), 0x1000, 0);

        GetBattleAnimBg1Data(&animBg);
        CpuFill16(0, animBg.bgTiles, 0x1000);
        CpuFill16(0, animBg.bgTilemap, 0x800);

        SetAnimBgAttribute(1, BG_ANIM_PRIORITY, 2);
        SetAnimBgAttribute(1, BG_ANIM_SCREEN_SIZE, 1);
        SetAnimBgAttribute(1, BG_ANIM_AREA_OVERFLOW_MODE, 0);

        battlerSpriteId = gBattlerSpriteIds[battlerId];

        gBattle_BG1_X =  -(gSprites[battlerSpriteId].pos1.x + gSprites[battlerSpriteId].pos2.x) + 0x20;

        gBattle_BG1_Y =  -(gSprites[battlerSpriteId].pos1.y + gSprites[battlerSpriteId].pos2.y) + 0x20;
        if (setSpriteInvisible)
            gSprites[gBattlerSpriteIds[battlerId]].invisible = TRUE;

        SetGpuReg(REG_OFFSET_BG1HOFS, gBattle_BG1_X);
        SetGpuReg(REG_OFFSET_BG1VOFS, gBattle_BG1_Y);

        LoadPalette(&gPlttBufferUnfaded[0x100 + battlerId * 16], animBg.paletteId * 16, 0x20);
        CpuCopy32(&gPlttBufferUnfaded[0x100 + battlerId * 16], (void*)(BG_PLTT + animBg.paletteId * 32), 0x20);

        battlerPosition = GetBattlerPosition(battlerId);

        sub_8118FBC(1, 0, 0, battlerPosition, animBg.paletteId, animBg.bgTiles, animBg.bgTilemap, animBg.tilesOffset);
    }
}

void sub_80A4720(u16 a, u16 *b, u32 c, u8 d)
{
    s32 i, j;
    s32 var;

    var = (d) ? 64 : 32;
    a <<= 12;
    for (i = 0; i < var; i++)
    {
        for (j = 0; j < 32; j++)
            b[j + i * 32] = ((b[j + i * 32] & 0xFFF) | a) + c;
    }
}

void ResetBattleAnimBg(bool8 toBG2)
{
    struct BattleAnimBgData animBg;
    GetBattleAnimBg1Data(&animBg);

    if (!toBG2)
    {
        ClearBattleAnimBg(1);
        gBattle_BG1_X = 0;
        gBattle_BG1_Y = 0;
    }
    else
    {
        ClearBattleAnimBg(2);
        gBattle_BG2_X = 0;
        gBattle_BG2_Y = 0;
    }
}

static void task_pA_ma0A_obj_to_bg_pal(u8 taskId)
{
    u8 spriteId, palIndex;
    s16 x, y;
    struct BattleAnimBgData animBg;

    spriteId = gTasks[taskId].data[0];
    palIndex = gTasks[taskId].data[6];
    GetBattleAnimBg1Data(&animBg);
    x = gTasks[taskId].data[1] - (gSprites[spriteId].pos1.x + gSprites[spriteId].pos2.x);
    y = gTasks[taskId].data[2] - (gSprites[spriteId].pos1.y + gSprites[spriteId].pos2.y);

    if (gTasks[taskId].data[5])
    {
        u16 *src;
        u16 *dst;

        gBattle_BG2_X = x + gTasks[taskId].data[3];
        gBattle_BG2_Y = y + gTasks[taskId].data[4];
        src = gPlttBufferFaded + 0x100 + palIndex * 16;
        dst = gPlttBufferFaded + 0x100 - 112;
        CpuCopy32(src, dst, 0x20);
    }
    else
    {
        u16 *src;
        u16 *dst;

        gBattle_BG1_X = x + gTasks[taskId].data[3];
        gBattle_BG1_Y = y + gTasks[taskId].data[4];
        src = gPlttBufferFaded + 0x100 + palIndex * 16;
        dst = gPlttBufferFaded + 0x100 + animBg.paletteId * 16 - 256;
        CpuCopy32(src, dst, 0x20);
    }
}

static void ScriptCmd_clearmonbg(void)
{
    u8 animBattlerId;
    u8 battlerId;
    u8 taskId;

    sBattleAnimScriptPtr++;
    animBattlerId = sBattleAnimScriptPtr[0];

    if (animBattlerId == ANIM_ATTACKER)
        animBattlerId = ANIM_ATK_PARTNER;
    else if (animBattlerId == ANIM_TARGET)
        animBattlerId = ANIM_DEF_PARTNER;

    battlerId = (animBattlerId == ANIM_ATTACKER || animBattlerId == ANIM_ATK_PARTNER) ? gBattleAnimAttacker : gBattleAnimTarget;

    if (sMonAnimTaskIdArray[0] != TASK_NONE)
        gSprites[gBattlerSpriteIds[battlerId]].invisible = FALSE;
    if (animBattlerId > 1 && sMonAnimTaskIdArray[1] != TASK_NONE)
        gSprites[gBattlerSpriteIds[battlerId ^ BIT_FLANK]].invisible = FALSE;
    else
        animBattlerId = 0;

    taskId = CreateTask(sub_80A4980, 5);
    gTasks[taskId].data[0] = animBattlerId;
    gTasks[taskId].data[2] = battlerId;

    sBattleAnimScriptPtr++;
}

static void sub_80A4980(u8 taskId)
{
    gTasks[taskId].data[1]++;
    if (gTasks[taskId].data[1] != 1)
    {
        u8 to_BG2;
        u8 position = GetBattlerPosition(gTasks[taskId].data[2]);
        to_BG2 = (position == B_POSITION_OPPONENT_LEFT || position == B_POSITION_PLAYER_RIGHT) ? FALSE : TRUE;

        if (sMonAnimTaskIdArray[0] != TASK_NONE)
        {
            ResetBattleAnimBg(to_BG2);
            DestroyTask(sMonAnimTaskIdArray[0]);
            sMonAnimTaskIdArray[0] = TASK_NONE;
        }
        if (gTasks[taskId].data[0] > 1)
        {
            ResetBattleAnimBg(to_BG2 ^ 1);
            DestroyTask(sMonAnimTaskIdArray[1]);
            sMonAnimTaskIdArray[1] = TASK_NONE;
        }
        DestroyTask(taskId);
    }
}

static void ScriptCmd_monbg_22(void)
{
    bool8 toBG_2;
    u8 battlerId;
    u8 animBattlerId;

    sBattleAnimScriptPtr++;

    animBattlerId = sBattleAnimScriptPtr[0];

    if (animBattlerId == ANIM_ATTACKER)
        animBattlerId = ANIM_ATK_PARTNER;
    else if (animBattlerId == ANIM_TARGET)
        animBattlerId = ANIM_DEF_PARTNER;

    battlerId = (animBattlerId == ANIM_ATTACKER || animBattlerId == ANIM_ATK_PARTNER) ? gBattleAnimAttacker : gBattleAnimTarget;

    if (IsBattlerSpriteVisible(battlerId))
    {
        u8 position = GetBattlerPosition(battlerId);
        toBG_2 = (position == B_POSITION_OPPONENT_LEFT || position == B_POSITION_PLAYER_RIGHT) ? FALSE : TRUE;

        MoveBattlerSpriteToBG(battlerId, toBG_2, FALSE);
    }

    battlerId ^= BIT_FLANK;
    if (animBattlerId > 1 && IsBattlerSpriteVisible(battlerId))
    {
        u8 position = GetBattlerPosition(battlerId);
        toBG_2 = (position == B_POSITION_OPPONENT_LEFT || position == B_POSITION_PLAYER_RIGHT) ? FALSE : TRUE;

        MoveBattlerSpriteToBG(battlerId, toBG_2, FALSE);
    }

    sBattleAnimScriptPtr++;
}

static void ScriptCmd_clearmonbg_23(void)
{
    u8 animBattlerId;
    u8 battlerId;
    u8 taskId;

    sBattleAnimScriptPtr++;
    animBattlerId = sBattleAnimScriptPtr[0];

    if (animBattlerId == ANIM_ATTACKER)
        animBattlerId = ANIM_ATK_PARTNER;
    else if (animBattlerId == ANIM_TARGET)
        animBattlerId = ANIM_DEF_PARTNER;

    battlerId = (animBattlerId == ANIM_ATTACKER || animBattlerId == ANIM_ATK_PARTNER) ? gBattleAnimAttacker : gBattleAnimTarget;

    if (IsBattlerSpriteVisible(battlerId))
        gSprites[gBattlerSpriteIds[battlerId]].invisible = FALSE;
    if (animBattlerId > 1 && IsBattlerSpriteVisible(battlerId ^ BIT_FLANK))
        gSprites[gBattlerSpriteIds[battlerId ^ BIT_FLANK]].invisible = FALSE;
    else
        animBattlerId = 0;

    taskId = CreateTask(sub_80A4BB0, 5);
    gTasks[taskId].data[0] = animBattlerId;
    gTasks[taskId].data[2] = battlerId;

    sBattleAnimScriptPtr++;
}

static void sub_80A4BB0(u8 taskId)
{
    gTasks[taskId].data[1]++;
    if (gTasks[taskId].data[1] != 1)
    {
        bool8 toBG_2;
        u8 battlerId = gTasks[taskId].data[2];
        u8 position = GetBattlerPosition(battlerId);
        toBG_2 = (position == B_POSITION_OPPONENT_LEFT || position == B_POSITION_PLAYER_RIGHT) ? FALSE : TRUE;

        if (IsBattlerSpriteVisible(battlerId))
            ResetBattleAnimBg(toBG_2);
        if (gTasks[taskId].data[0] > 1 && IsBattlerSpriteVisible(battlerId ^ BIT_FLANK))
            ResetBattleAnimBg(toBG_2 ^ 1);

        DestroyTask(taskId);
    }
}

#undef t1_MONBG_BATTLER
#undef t1_MON_IN_BG2
#undef t1_CREATE_ANOTHER_TASK
#undef t1_IS_SECONDMON_BG

#undef t2_BATTLER_SPRITE_ID
#undef t2_MON_IN_BG2
#undef t2_MONBG_BATTLER

static void ScriptCmd_setalpha(void)
{
    u16 half1, half2;

    sBattleAnimScriptPtr++;
    half1 = *(sBattleAnimScriptPtr++);
    half2 = *(sBattleAnimScriptPtr++) << 8;
    SetGpuReg(REG_OFFSET_BLDCNT, BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL);
    SetGpuReg(REG_OFFSET_BLDALPHA, half1 | half2);
}

static void ScriptCmd_setbldcnt(void)
{
    u16 half1, half2;

    sBattleAnimScriptPtr++;
    half1 = *(sBattleAnimScriptPtr++);
    half2 = *(sBattleAnimScriptPtr++) << 8;
    SetGpuReg(REG_OFFSET_BLDCNT, half1 | half2);
}

static void ScriptCmd_blendoff(void)
{
    sBattleAnimScriptPtr++;
    SetGpuReg(REG_OFFSET_BLDCNT, 0);
    SetGpuReg(REG_OFFSET_BLDALPHA, 0);
}

static void ScriptCmd_call(void)
{
    sBattleAnimScriptPtr++;
    sBattleAnimScriptRetAddr = sBattleAnimScriptPtr + 4;
    sBattleAnimScriptPtr = T2_READ_PTR(sBattleAnimScriptPtr);
}

static void ScriptCmd_return(void)
{
    sBattleAnimScriptPtr = sBattleAnimScriptRetAddr;
}

static void ScriptCmd_setarg(void)
{
    const u8 *addr = sBattleAnimScriptPtr;
    u16 value;
    u8 argId;

    sBattleAnimScriptPtr++;
    argId = sBattleAnimScriptPtr[0];
    sBattleAnimScriptPtr++;
    value = T1_READ_16(sBattleAnimScriptPtr);
    sBattleAnimScriptPtr = addr + 4;
    gBattleAnimArgs[argId] = value;
}

static void ScriptCmd_choosetwoturnanim(void)
{
    sBattleAnimScriptPtr++;
    if (gAnimMoveTurn & 1)
        sBattleAnimScriptPtr += 4;
    sBattleAnimScriptPtr = T2_READ_PTR(sBattleAnimScriptPtr);
}

static void ScriptCmd_jumpifmoveturn(void)
{
    u8 toCheck;
    sBattleAnimScriptPtr++;
    toCheck = sBattleAnimScriptPtr[0];
    sBattleAnimScriptPtr++;

    if (toCheck == gAnimMoveTurn)
        sBattleAnimScriptPtr = T2_READ_PTR(sBattleAnimScriptPtr);
    else
        sBattleAnimScriptPtr += 4;
}

static void ScriptCmd_goto(void)
{
    sBattleAnimScriptPtr++;
    sBattleAnimScriptPtr = T2_READ_PTR(sBattleAnimScriptPtr);
}

#define tBackgroundId   data[0]
#define tState          data[10]

static void ScriptCmd_fadetobg(void)
{
    u8 backgroundId;
    u8 taskId;

    sBattleAnimScriptPtr++;
    backgroundId = sBattleAnimScriptPtr[0];
    sBattleAnimScriptPtr++;
    taskId = CreateTask(Task_FadeToBg, 5);
    gTasks[taskId].tBackgroundId = backgroundId;
    sAnimBackgroundFadeState = 1;
}

static void ScriptCmd_fadetobgfromset(void)
{
    u8 bg1, bg2, taskId;

    sBattleAnimScriptPtr++;
    bg1 = sBattleAnimScriptPtr[0];
    bg2 = sBattleAnimScriptPtr[1];
    sBattleAnimScriptPtr += 3;
    taskId = CreateTask(Task_FadeToBg, 5);

    if (GetBattlerSide(gBattleAnimTarget) == B_SIDE_PLAYER)
        gTasks[taskId].tBackgroundId = bg2;
    else
        gTasks[taskId].tBackgroundId = bg1;

    sAnimBackgroundFadeState = 1;
}

static void Task_FadeToBg(u8 taskId)
{
    if (gTasks[taskId].tState == 0)
    {
        BeginHardwarePaletteFade(0xE8, 0, 0, 16, 0);
        gTasks[taskId].tState++;
        return;
    }
    if (gPaletteFade.active)
        return;
    if (gTasks[taskId].tState == 1)
    {
        gTasks[taskId].tState++;
        sAnimBackgroundFadeState = 2;
    }
    else if (gTasks[taskId].tState == 2)
    {
        s16 bgId = gTasks[taskId].tBackgroundId;

        if (bgId == -1)
            LoadDefaultBg();
        else
            LoadMoveBg(bgId);

        BeginHardwarePaletteFade(0xE8, 0, 16, 0, 1);
        gTasks[taskId].tState++;
        return;
    }
    if (gPaletteFade.active)
        return;
    if (gTasks[taskId].tState == 3)
    {
        DestroyTask(taskId);
        sAnimBackgroundFadeState = 0;
    }
}

static void LoadMoveBg(u16 bgId)
{
    LZDecompressVram(gBattleAnimBackgroundTable[bgId].tilemap, (void *)(BG_SCREEN_ADDR(26)));
    LZDecompressVram(gBattleAnimBackgroundTable[bgId].image, (void *)(BG_CHAR_ADDR(2)));
    LoadCompressedPalette(gBattleAnimBackgroundTable[bgId].palette, 32, 32);
}

static void LoadDefaultBg(void)
{
    LoadBattleTerrainGfx(GetBattleTerrainOverride());
}

static void ScriptCmd_restorebg(void)
{
    u8 taskId;

    sBattleAnimScriptPtr++;
    taskId = CreateTask(Task_FadeToBg, 5);
    gTasks[taskId].tBackgroundId = -1;
    sAnimBackgroundFadeState = 1;
}

#undef tBackgroundId
#undef tState

static void ScriptCmd_waitbgfadeout(void)
{
    if (sAnimBackgroundFadeState == 2)
    {
        sBattleAnimScriptPtr++;
        gAnimFramesToWait = 0;
    }
    else
    {
        gAnimFramesToWait = 1;
    }
}

static void ScriptCmd_waitbgfadein(void)
{
    if (sAnimBackgroundFadeState)
    {
        gAnimFramesToWait = 1;
    }
    else
    {
        sBattleAnimScriptPtr++;
        gAnimFramesToWait = 0;
    }
}

static void ScriptCmd_changebg(void)
{
    sBattleAnimScriptPtr++;
    LoadMoveBg(sBattleAnimScriptPtr[0]);
    sBattleAnimScriptPtr++;
}

s8 BattleAnimAdjustPanning(s8 pan)
{
    if (gBattleSpritesDataPtr->healthBoxesData[gBattleAnimAttacker].statusAnimActive)
    {
        pan = (GetBattlerSide(gBattleAnimAttacker)) ? SOUND_PAN_TARGET : SOUND_PAN_ATTACKER;
    }
    else if (GetBattlerSide(gBattleAnimAttacker) == B_SIDE_PLAYER)
    {
        if (GetBattlerSide(gBattleAnimTarget) == B_SIDE_PLAYER)
        {
            if (pan == SOUND_PAN_TARGET)
                pan = SOUND_PAN_ATTACKER;
            else if (pan != SOUND_PAN_ATTACKER)
                pan *= -1;
        }
    }
    else if (GetBattlerSide(gBattleAnimTarget) == B_SIDE_OPPONENT)
    {
        if (pan == SOUND_PAN_ATTACKER)
            pan = SOUND_PAN_TARGET;
    }
    else
    {
        pan *= -1;
    }

    if (pan > SOUND_PAN_TARGET)
        pan = SOUND_PAN_TARGET;
    else if (pan < SOUND_PAN_ATTACKER)
        pan = SOUND_PAN_ATTACKER;

    return pan;
}

s8 BattleAnimAdjustPanning2(s8 pan)
{
    if (gBattleSpritesDataPtr->healthBoxesData[gBattleAnimAttacker].statusAnimActive)
    {
        pan = (GetBattlerSide(gBattleAnimAttacker)) ? SOUND_PAN_TARGET : SOUND_PAN_ATTACKER;
    }
    else
    {
        if (GetBattlerSide(gBattleAnimAttacker) != B_SIDE_PLAYER)
            pan = -pan;
    }
    return pan;
}

s16 KeepPanInRange(s16 panArg, int oldPan)
{
    s16 pan = panArg;

    if (pan > SOUND_PAN_TARGET)
        pan = SOUND_PAN_TARGET;
    else if (pan < SOUND_PAN_ATTACKER)
        pan = SOUND_PAN_ATTACKER;

    return pan;
}

s16 CalculatePanIncrement(s16 sourcePan, s16 targetPan, s16 incrementPan)
{
    s16 ret;

    if (sourcePan < targetPan)
        ret = ((incrementPan < 0) ? -incrementPan : incrementPan);
    else if (sourcePan > targetPan)
        ret = -((incrementPan < 0) ? -incrementPan : incrementPan);
    else
        ret = 0;

    return ret;
}

static void ScriptCmd_playsewithpan(void)
{
    u16 songId;
    s8 pan;

    sBattleAnimScriptPtr++;
    songId = T1_READ_16(sBattleAnimScriptPtr);
    pan = sBattleAnimScriptPtr[2];
    PlaySE12WithPanning(songId, BattleAnimAdjustPanning(pan));
    sBattleAnimScriptPtr += 3;
}

static void ScriptCmd_setpan(void)
{
    s8 pan;

    sBattleAnimScriptPtr++;
    pan = sBattleAnimScriptPtr[0];
    SE12PanpotControl(BattleAnimAdjustPanning(pan));
    sBattleAnimScriptPtr++;
}

#define tInitialPan     data[0]
#define tTargetPan      data[1]
#define tIncrementPan   data[2]
#define tFramesToWait   data[3]
#define tCurrentPan     data[4]
#define tFrameCounter   data[8]

static void ScriptCmd_panse_1B(void)
{
    u16 songNum;
    s8 currentPanArg, incrementPan, incrementPanArg, currentPan, targetPan;
    u8 framesToWait;
    u8 taskId;

    sBattleAnimScriptPtr++;
    songNum = T1_READ_16(sBattleAnimScriptPtr);
    currentPanArg = sBattleAnimScriptPtr[2];
    incrementPan = sBattleAnimScriptPtr[3];
    incrementPanArg = sBattleAnimScriptPtr[4];
    framesToWait = sBattleAnimScriptPtr[5];

    currentPan = BattleAnimAdjustPanning(currentPanArg);
    targetPan = BattleAnimAdjustPanning(incrementPan);
    incrementPan = CalculatePanIncrement(currentPan, targetPan, incrementPanArg);
    taskId = CreateTask(Task_PanFromInitialToTarget, 1);
    gTasks[taskId].tInitialPan = currentPan;
    gTasks[taskId].tTargetPan = targetPan;
    gTasks[taskId].tIncrementPan = incrementPan;
    gTasks[taskId].tFramesToWait = framesToWait;
    gTasks[taskId].tCurrentPan = currentPan;

    PlaySE12WithPanning(songNum, currentPan);

    gAnimSoundTaskCount++;
    sBattleAnimScriptPtr += 6;
}

void Task_PanFromInitialToTarget(u8 taskId)
{
    bool32 destroyTask = FALSE;
    if (gTasks[taskId].tFrameCounter++ >= gTasks[taskId].tFramesToWait)
    {
        s16 pan;
        s16 initialPanning, targetPanning, currentPan, incrementPan;

        gTasks[taskId].tFrameCounter = 0;
        initialPanning = gTasks[taskId].tInitialPan;
        targetPanning = gTasks[taskId].tTargetPan;
        currentPan = gTasks[taskId].tCurrentPan;
        incrementPan = gTasks[taskId].tIncrementPan;
        pan = currentPan + incrementPan;
        gTasks[taskId].tCurrentPan = pan;

        if (incrementPan == 0) // If we're not incrementing, just cancel the task immediately.
        {
            destroyTask = TRUE;
        }
        else if (initialPanning < targetPanning) // Panning increasing.
        {
            if (pan >= targetPanning) // Target reached.
                destroyTask = TRUE;
        }
        else // Panning decreasing.
        {
            if (pan <= targetPanning) // Target reached.
                destroyTask = TRUE;
        }

        if (destroyTask)
        {
            pan = targetPanning;
            DestroyTask(taskId);
            gAnimSoundTaskCount--;
        }

        SE12PanpotControl(pan);
    }
}

static void ScriptCmd_panse_26(void)
{
    u16 songId;
    s8 currentPan, targetPan, incrementPan;
    u8 framesToWait;
    u8 taskId;

    sBattleAnimScriptPtr++;
    songId = T1_READ_16(sBattleAnimScriptPtr);
    currentPan = sBattleAnimScriptPtr[2];
    targetPan = sBattleAnimScriptPtr[3];
    incrementPan = sBattleAnimScriptPtr[4];
    framesToWait = sBattleAnimScriptPtr[5];

    taskId = CreateTask(Task_PanFromInitialToTarget, 1);
    gTasks[taskId].tInitialPan = currentPan;
    gTasks[taskId].tTargetPan = targetPan;
    gTasks[taskId].tIncrementPan = incrementPan;
    gTasks[taskId].tFramesToWait = framesToWait;
    gTasks[taskId].tCurrentPan = currentPan;

    PlaySE12WithPanning(songId, currentPan);

    gAnimSoundTaskCount++;
    sBattleAnimScriptPtr += 6;
}

static void ScriptCmd_panse_27(void)
{
    u16 songId;
    s8 targetPanArg, incrementPanArg, currentPanArg, currentPan, targetPan, incrementPan;
    u8 framesToWait;
    u8 taskId;

    sBattleAnimScriptPtr++;
    songId = T1_READ_16(sBattleAnimScriptPtr);
    currentPanArg = sBattleAnimScriptPtr[2];
    targetPanArg = sBattleAnimScriptPtr[3];
    incrementPanArg = sBattleAnimScriptPtr[4];
    framesToWait = sBattleAnimScriptPtr[5];

    currentPan = BattleAnimAdjustPanning2(currentPanArg);
    targetPan = BattleAnimAdjustPanning2(targetPanArg);
    incrementPan = BattleAnimAdjustPanning2(incrementPanArg);

    taskId = CreateTask(Task_PanFromInitialToTarget, 1);
    gTasks[taskId].tInitialPan = currentPan;
    gTasks[taskId].tTargetPan = targetPan;
    gTasks[taskId].tIncrementPan = incrementPan;
    gTasks[taskId].tFramesToWait = framesToWait;
    gTasks[taskId].tCurrentPan = currentPan;

    PlaySE12WithPanning(songId, currentPan);

    gAnimSoundTaskCount++;
    sBattleAnimScriptPtr += 6;
}

#undef tInitialPan
#undef tTargetPan
#undef tIncrementPan
#undef tFramesToWait
#undef tCurrentPan
#undef tFrameCounter

#define tSongId         data[0]
#define tPanning        data[1]
#define tFramesToWait   data[2]
#define tNumberOfPlays  data[3]
#define tFrameCounter   data[8]

static void ScriptCmd_loopsewithpan(void)
{
    u16 songId;
    s8 panningArg, panning;
    u8 framesToWait, numberOfPlays;
    u8 taskId;

    sBattleAnimScriptPtr++;
    songId = T1_READ_16(sBattleAnimScriptPtr);
    panningArg = sBattleAnimScriptPtr[2];
    framesToWait = sBattleAnimScriptPtr[3];
    numberOfPlays = sBattleAnimScriptPtr[4];
    panning = BattleAnimAdjustPanning(panningArg);

    taskId = CreateTask(Task_LoopAndPlaySE, 1);
    gTasks[taskId].tSongId = songId;
    gTasks[taskId].tPanning = panning;
    gTasks[taskId].tFramesToWait = framesToWait;
    gTasks[taskId].tNumberOfPlays = numberOfPlays;
    gTasks[taskId].tFrameCounter = framesToWait;
    gTasks[taskId].func(taskId);

    gAnimSoundTaskCount++;
    sBattleAnimScriptPtr += 5;
}

static void Task_LoopAndPlaySE(u8 taskId)
{
    if (gTasks[taskId].tFrameCounter++ >= gTasks[taskId].tFramesToWait)
    {
        u16 songId;
        s8 panning;
        u8 numberOfPlays;

        gTasks[taskId].tFrameCounter = 0;
        songId = gTasks[taskId].tSongId;
        panning = gTasks[taskId].tPanning;
        numberOfPlays = --gTasks[taskId].tNumberOfPlays;
        PlaySE12WithPanning(songId, panning);
        if (numberOfPlays == 0)
        {
            DestroyTask(taskId);
            gAnimSoundTaskCount--;
        }
    }
}

#undef tSongId
#undef tPanning
#undef tFramesToWait
#undef tNumberOfPlays
#undef tFrameCounter

#define tSongId         data[0]
#define tPanning        data[1]
#define tFramesToWait   data[2]

static void ScriptCmd_waitplaysewithpan(void)
{
    u16 songId;
    s8 panningArg, panning;
    u8 framesToWait;
    u8 taskId;

    sBattleAnimScriptPtr++;
    songId = T1_READ_16(sBattleAnimScriptPtr);
    panningArg = sBattleAnimScriptPtr[2];
    framesToWait = sBattleAnimScriptPtr[3];
    panning = BattleAnimAdjustPanning(panningArg);

    taskId = CreateTask(Task_WaitAndPlaySE, 1);
    gTasks[taskId].tSongId = songId;
    gTasks[taskId].tPanning = panning;
    gTasks[taskId].tFramesToWait = framesToWait;

    gAnimSoundTaskCount++;
    sBattleAnimScriptPtr += 4;
}

static void Task_WaitAndPlaySE(u8 taskId)
{
    if (gTasks[taskId].tFramesToWait-- <= 0)
    {
        PlaySE12WithPanning(gTasks[taskId].tSongId, gTasks[taskId].tPanning);
        DestroyTask(taskId);
        gAnimSoundTaskCount--;
    }
}

#undef tSongId
#undef tPanning
#undef tFramesToWait

static void ScriptCmd_createsoundtask(void)
{
    TaskFunc func;
    u8 numArgs, taskId;
    s32 i;

    sBattleAnimScriptPtr++;
    func = (TaskFunc)T2_READ_32(sBattleAnimScriptPtr);
    sBattleAnimScriptPtr += 4;
    numArgs = sBattleAnimScriptPtr[0];
    sBattleAnimScriptPtr++;
    for (i = 0; i < numArgs; i++)
    {
        gBattleAnimArgs[i] = T1_READ_16(sBattleAnimScriptPtr);
        sBattleAnimScriptPtr += 2;
    }
    taskId = CreateTask(func, 1);
    func(taskId);
    gAnimSoundTaskCount++;
}

static void ScriptCmd_waitsound(void)
{
    if (gAnimSoundTaskCount != 0)
    {
        sSoundAnimFramesToWait = 0;
        gAnimFramesToWait = 1;
    }
    else if (IsSEPlaying())
    {
        if (++sSoundAnimFramesToWait > 90)
        {
            m4aMPlayStop(&gMPlayInfo_SE1);
            m4aMPlayStop(&gMPlayInfo_SE2);
            sSoundAnimFramesToWait = 0;
        }
        else
        {
            gAnimFramesToWait = 1;
        }
    }
    else
    {
        sSoundAnimFramesToWait = 0;
        sBattleAnimScriptPtr++;
        gAnimFramesToWait = 0;
    }
}

static void ScriptCmd_jumpargeq(void)
{
    u8 argId;
    s16 valueToCheck;

    sBattleAnimScriptPtr++;
    argId = sBattleAnimScriptPtr[0];
    valueToCheck = T1_READ_16(sBattleAnimScriptPtr + 1);

    if (valueToCheck == gBattleAnimArgs[argId])
        sBattleAnimScriptPtr = T2_READ_PTR(sBattleAnimScriptPtr + 3);
    else
        sBattleAnimScriptPtr += 7;
}

static void ScriptCmd_monbgprio_28(void)
{
    u8 wantedBattler;
    u8 battlerId;
    u8 battlerPosition;

    wantedBattler = sBattleAnimScriptPtr[1];
    sBattleAnimScriptPtr += 2;

    battlerId = (wantedBattler != ANIM_ATTACKER) ? gBattleAnimTarget : gBattleAnimAttacker;

    battlerPosition = GetBattlerPosition(battlerId);
    if ((battlerPosition == B_POSITION_PLAYER_LEFT || battlerPosition == B_POSITION_OPPONENT_RIGHT))
    {
        SetAnimBgAttribute(1, BG_ANIM_PRIORITY, 1);
        SetAnimBgAttribute(2, BG_ANIM_PRIORITY, 2);
    }
}

static void ScriptCmd_monbgprio_29(void)
{
    sBattleAnimScriptPtr++;
    SetAnimBgAttribute(1, BG_ANIM_PRIORITY, 1);
    SetAnimBgAttribute(2, BG_ANIM_PRIORITY, 2);
}

static void ScriptCmd_monbgprio_2A(void)
{
    u8 wantedBattler;
    u8 battlerPosition;
    u8 battlerId;

    wantedBattler = sBattleAnimScriptPtr[1];
    sBattleAnimScriptPtr += 2;
    if (GetBattlerSide(gBattleAnimAttacker) != GetBattlerSide(gBattleAnimTarget))
    {
        battlerId = (wantedBattler != ANIM_ATTACKER) ? gBattleAnimTarget : gBattleAnimAttacker;

        battlerPosition = GetBattlerPosition(battlerId);
        if ((battlerPosition == B_POSITION_PLAYER_LEFT || battlerPosition == B_POSITION_OPPONENT_RIGHT))
        {
            SetAnimBgAttribute(1, BG_ANIM_PRIORITY, 1);
            SetAnimBgAttribute(2, BG_ANIM_PRIORITY, 2);
        }
    }
}

static void ScriptCmd_invisible(void)
{
    u8 spriteId;

    spriteId = GetAnimBattlerSpriteId(sBattleAnimScriptPtr[1]);
    if (spriteId != SPRITE_NONE)
        gSprites[spriteId].invisible = TRUE;

    sBattleAnimScriptPtr += 2;
}

static void ScriptCmd_visible(void)
{
    u8 spriteId;

    spriteId = GetAnimBattlerSpriteId(sBattleAnimScriptPtr[1]);
    if (spriteId != SPRITE_NONE)
        gSprites[spriteId].invisible = FALSE;

    sBattleAnimScriptPtr += 2;
}

static void ScriptCmd_doublebattle_2D(void)
{
    u8 wantedBattler;
    u8 r4;
    u8 spriteId;

    wantedBattler = sBattleAnimScriptPtr[1];
    sBattleAnimScriptPtr += 2;
    if (IsDoubleBattle()
     && GetBattlerSide(gBattleAnimAttacker) == GetBattlerSide(gBattleAnimTarget))
    {
        if (wantedBattler == ANIM_ATTACKER)
        {
            r4 = GetBattlerSpriteBGPriorityRank(gBattleAnimAttacker);
            spriteId = GetAnimBattlerSpriteId(ANIM_ATTACKER);
        }
        else
        {
            r4 = GetBattlerSpriteBGPriorityRank(gBattleAnimTarget);
            spriteId = GetAnimBattlerSpriteId(ANIM_TARGET);
        }
        if (spriteId != SPRITE_NONE)
        {
            gSprites[spriteId].invisible = FALSE;
            if (r4 == 2)
                gSprites[spriteId].oam.priority = 3;

            if (r4 == 1)
                ResetBattleAnimBg(FALSE);
            else
                ResetBattleAnimBg(TRUE);
        }
    }
}

static void ScriptCmd_doublebattle_2E(void)
{
    u8 wantedBattler;
    u8 r4;
    u8 spriteId;

    wantedBattler = sBattleAnimScriptPtr[1];
    sBattleAnimScriptPtr += 2;
    if (IsDoubleBattle()
     && GetBattlerSide(gBattleAnimAttacker) == GetBattlerSide(gBattleAnimTarget))
    {
        if (wantedBattler == ANIM_ATTACKER)
        {
            r4 = GetBattlerSpriteBGPriorityRank(gBattleAnimAttacker);
            spriteId = GetAnimBattlerSpriteId(ANIM_ATTACKER);
        }
        else
        {
            r4 = GetBattlerSpriteBGPriorityRank(gBattleAnimTarget);
            spriteId = GetAnimBattlerSpriteId(ANIM_TARGET);
        }

        if (spriteId != SPRITE_NONE && r4 == 2)
            gSprites[spriteId].oam.priority = 2;
    }
}

static void ScriptCmd_stopsound(void)
{
    m4aMPlayStop(&gMPlayInfo_SE1);
    m4aMPlayStop(&gMPlayInfo_SE2);
    sBattleAnimScriptPtr++;
}
