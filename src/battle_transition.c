#include "global.h"
#include "battle.h"
#include "battle_transition.h"
#include "battle_transition_frontier.h"
#include "bg.h"
#include "decompress.h"
#include "event_object_movement.h"
#include "field_camera.h"
#include "field_effect.h"
#include "field_weather.h"
#include "gpu_regs.h"
#include "main.h"
#include "malloc.h"
#include "overworld.h"
#include "palette.h"
#include "random.h"
#include "scanline_effect.h"
#include "sound.h"
#include "sprite.h"
#include "task.h"
#include "trig.h"
#include "util.h"
#include "constants/field_effects.h"
#include "constants/songs.h"
#include "constants/trainers.h"
#include "constants/rgb.h"

struct TransitionData
{
    vu8 VBlank_DMA;
    u16 WININ;
    u16 WINOUT;
    u16 WIN0H;
    u16 WIN0V;
    u16 field_A; // unused
    u16 field_C; // unused
    u16 BLDCNT;
    u16 BLDALPHA;
    u16 BLDY;
    s16 field_14;
    s16 field_16;
    s16 BG0HOFS_1;
    s16 BG0HOFS_2;
    s16 BG0VOFS; // used but not set
    s16 field_1E; // unused
    s16 field_20;
    s16 field_22; // unused
    s16 data[11];
};

struct StructRectangularSpiral
{
    u8 field_0;
    s16 field_2;
    u8 field_4;
    s16 field_6;
    u8 field_8;
};

typedef bool8 (*TransitionStateFunc)(struct Task *task);
typedef bool8 (*TransitionSpriteCallback)(struct Sprite *sprite);

// this file's functions
static void LaunchBattleTransitionTask(u8 transitionId);
static void Task_BattleTransitionMain(u8 taskId);
static void Phase1Task_TransitionAll(u8 taskId);
static void Phase2Task_Blur(u8 taskId);
static void Phase2Task_Swirl(u8 taskId);
static void Phase2Task_Shuffle(u8 taskId);
static void Phase2Task_BigPokeball(u8 taskId);
static void Phase2Task_PokeballsTrail(u8 taskId);
static void Phase2Task_Clockwise_BlackFade(u8 taskId);
static void Phase2Task_Ripple(u8 taskId);
static void Phase2Task_Wave(u8 taskId);
static void Phase2Task_Slice(u8 taskId);
static void Phase2Task_WhiteFade(u8 taskId);
static void Phase2Task_GridSquares(u8 taskId);
static void Phase2Task_Shards(u8 taskId);
static void Phase2Task_Sidney(u8 taskId);
static void Phase2Task_Phoebe(u8 taskId);
static void Phase2Task_Glacia(u8 taskId);
static void Phase2Task_Drake(u8 taskId);
static void Phase2Task_Champion(u8 taskId);
static void Phase2Task_AntiClockwiseSpiral(u8 taskId);
static void Phase2Task_Rocket(u8 taskId);
static void Phase2Task_BlackHole(u8 taskId);
static void Phase2Task_RectangularSpiral(u8 taskId);
static void Phase2Task_FrontierLogoWiggle(u8 taskId);
static void Phase2Task_FrontierLogoWave(u8 taskId);
static void Phase2Task_FrontierSquares(u8 taskId);
static void Phase2Task_FrontierSquaresScroll(u8 taskId);
static void Phase2Task_FrontierSquaresSpiral(u8 taskId);
static void VBlankCB_BattleTransition(void);
static void VBlankCB_Phase2_Swirl(void);
static void HBlankCB_Phase2_Swirl(void);
static void VBlankCB_Phase2_Shuffle(void);
static void HBlankCB_Phase2_Shuffle(void);
static void VBlankCB0_Phase2_BigPokeball(void);
static void VBlankCB1_Phase2_BigPokeball(void);
static void VBlankCB_Phase2_Clockwise_BlackFade(void);
static void VBlankCB_Phase2_Ripple(void);
static void HBlankCB_Phase2_Ripple(void);
static void VBlankCB_Phase2_30(void);
static void HBlankCB_Phase2_30(void);
static void VBlankCB_Phase2_Wave(void);
static void VBlankCB_Phase2_Slice(void);
static void HBlankCB_Phase2_Slice(void);
static void VBlankCB0_Phase2_WhiteFade(void);
static void VBlankCB1_Phase2_WhiteFade(void);
static void HBlankCB_Phase2_WhiteFade(void);
static void VBlankCB_Phase2_Shards(void);
static bool8 Phase2_Blur_Func1(struct Task *task);
static bool8 Phase2_Blur_Func2(struct Task *task);
static bool8 Phase2_Blur_Func3(struct Task *task);
static bool8 Phase2_Swirl_Func1(struct Task *task);
static bool8 Phase2_Swirl_Func2(struct Task *task);
static bool8 Phase2_Shuffle_Func1(struct Task *task);
static bool8 Phase2_Shuffle_Func2(struct Task *task);
static bool8 Phase2_Rocket_Func1(struct Task *task);
static bool8 Phase2_Rocket_Func2(struct Task *task);
static bool8 Phase2_FramesCountdown(struct Task *task);
static bool8 Phase2_WaitPaletteFade(struct Task *task);
static bool8 Phase2_BigPokeball_Func1(struct Task *task);
static bool8 Phase2_BigPokeball_Func2(struct Task *task);
static bool8 Phase2_BigPokeball_Func3(struct Task *task);
static bool8 Phase2_BigPokeball_Func4(struct Task *task);
static bool8 Phase2_BigPokeball_Func5(struct Task *task);
static bool8 Phase2_BigPokeball_Func6(struct Task *task);
static bool8 Phase2_PokeballsTrail_Func1(struct Task *task);
static bool8 Phase2_PokeballsTrail_Func2(struct Task *task);
static bool8 Phase2_PokeballsTrail_Func3(struct Task *task);
static bool8 Phase2_Clockwise_BlackFade_Func1(struct Task *task);
static bool8 Phase2_Clockwise_BlackFade_Func2(struct Task *task);
static bool8 Phase2_Clockwise_BlackFade_Func3(struct Task *task);
static bool8 Phase2_Clockwise_BlackFade_Func4(struct Task *task);
static bool8 Phase2_Clockwise_BlackFade_Func5(struct Task *task);
static bool8 Phase2_Clockwise_BlackFade_Func6(struct Task *task);
static bool8 Phase2_Clockwise_BlackFade_Func7(struct Task *task);
static bool8 Phase2_Ripple_Func1(struct Task *task);
static bool8 Phase2_Ripple_Func2(struct Task *task);
static bool8 Phase2_AntiClockwiseSpiral_Func1(struct Task *task);
static bool8 Phase2_AntiClockwiseSpiral_Func2(struct Task *task);
static bool8 Phase2_Wave_Func1(struct Task *task);
static bool8 Phase2_Wave_Func2(struct Task *task);
static bool8 Phase2_Wave_Func3(struct Task *task);
static bool8 Phase2_Slice_Func1(struct Task *task);
static bool8 Phase2_Slice_Func2(struct Task *task);
static bool8 Phase2_Slice_Func3(struct Task *task);
static bool8 Phase2_WhiteFade_Func1(struct Task *task);
static bool8 Phase2_WhiteFade_Func2(struct Task *task);
static bool8 Phase2_WhiteFade_Func3(struct Task *task);
static bool8 Phase2_WhiteFade_Func4(struct Task *task);
static bool8 Phase2_WhiteFade_Func5(struct Task *task);
static bool8 Phase2_GridSquares_Func1(struct Task *task);
static bool8 Phase2_GridSquares_Func2(struct Task *task);
static bool8 Phase2_GridSquares_Func3(struct Task *task);
static bool8 Phase2_Shards_Func1(struct Task *task);
static bool8 Phase2_Shards_Func2(struct Task *task);
static bool8 Phase2_Shards_Func3(struct Task *task);
static bool8 Phase2_Shards_Func4(struct Task *task);
static bool8 Phase2_Shards_Func5(struct Task *task);
static bool8 Phase2_BlackHole_Func1(struct Task *task);
static bool8 Phase2_BlackHole_Func2(struct Task *task);
static bool8 Phase2_BlackHole_Func3(struct Task *task);
static bool8 Phase2_RectangularSpiral_Func1(struct Task *task);
static bool8 Phase2_RectangularSpiral_Func2(struct Task *task);
static bool8 Phase2_RectangularSpiral_Func3(struct Task *task);
static bool8 Phase2_FrontierLogoWiggle_Func1(struct Task *task);
static bool8 Phase2_FrontierLogoWiggle_Func2(struct Task *task);
static bool8 Phase2_FrontierLogoWave_Func1(struct Task *task);
static bool8 Phase2_FrontierLogoWave_Func2(struct Task *task);
static bool8 Phase2_FrontierLogoWave_Func3(struct Task *task);
static bool8 Phase2_FrontierLogoWave_Func4(struct Task *task);
static bool8 Phase2_FrontierSquares_Func1(struct Task *task);
static bool8 Phase2_FrontierSquares_Func2(struct Task *task);
static bool8 Phase2_FrontierSquares_Func3(struct Task *task);
static bool8 Phase2_FrontierSquares_End(struct Task *task);
static bool8 Phase2_FrontierSquaresSpiral_Func1(struct Task *task);
static bool8 Phase2_FrontierSquaresSpiral_Func2(struct Task *task);
static bool8 Phase2_FrontierSquaresSpiral_Func3(struct Task *task);
static bool8 Phase2_FrontierSquaresSpiral_Func4(struct Task *task);
static bool8 Phase2_FrontierSquaresScroll_Func1(struct Task *task);
static bool8 Phase2_FrontierSquaresScroll_Func2(struct Task *task);
static bool8 Phase2_FrontierSquaresScroll_Func3(struct Task *task);
static bool8 Phase2_FrontierSquaresScroll_Func4(struct Task *task);
static bool8 Phase2_FrontierSquaresScroll_Func5(struct Task *task);
static bool8 Phase2_Mugshot_Func1(struct Task *task);
static bool8 Phase2_Mugshot_Func2(struct Task *task);
static bool8 Phase2_Mugshot_Func3(struct Task *task);
static bool8 Phase2_Mugshot_Func4(struct Task *task);
static bool8 Phase2_Mugshot_Func5(struct Task *task);
static bool8 Phase2_Mugshot_Func6(struct Task *task);
static bool8 Phase2_Mugshot_Func7(struct Task *task);
static bool8 Phase2_Mugshot_Func8(struct Task *task);
static bool8 Phase2_Mugshot_Func9(struct Task *task);
static bool8 Phase2_Mugshot_Func10(struct Task *task);
static void Phase2Task_MugShotTransition(u8 taskId);
static void Mugshots_CreateOpponentPlayerSprites(struct Task *task);
static void VBlankCB0_Phase2_Mugshots(void);
static void VBlankCB1_Phase2_Mugshots(void);
static void HBlankCB_Phase2_Mugshots(void);
static void VBlankCB_AntiClockwiseSpiral(void);
static bool8 Transition_Phase1(struct Task *task);
static bool8 Transition_WaitForPhase1(struct Task *task);
static bool8 Transition_Phase2(struct Task *task);
static bool8 Transition_WaitForPhase2(struct Task *task);
static void InitTransitionStructVars(void);
static void FadeScreenBlack(void);
static void CreatePhase1Task(s16 a0, s16 a1, s16 a2, s16 a3, s16 a4);
static void sub_814A014(u16 *a0, s16 a1, s16 a2, s16 a3);
static void sub_8149F98(s16 *array, s16 sinAdd, s16 index, s16 indexIncrementer, s16 amplitude, s16 arrSize);
static void GetBg0TilemapDst(u16 **tileset);
static void sub_814A1AC(s16 *a0, s16 a1, s16 a2, s16 a3, s16 a4, s16 a5, s16 a6);
static bool8 sub_814A228(s16 *a0, bool8 a1, bool8 a2);
static void SetTrainerPicSlideTable(s16 spriteId, s16 arrId);
static void IncrementTrainerPicState(s16 spriteId);
static s16 IsTrainerPicSlideDone(s16 spriteId);
static bool8 Phase1_TransitionAll_Func1(struct Task *task);
static bool8 Phase1_TransitionAll_Func2(struct Task *task);
static bool8 IsPhase1Done(void);
static bool16 sub_8149048(const s16 * const *arg0, struct StructRectangularSpiral *arg1);
static void sub_814713C(struct Sprite *sprite);
static void SpriteCb_TrainerPic(struct Sprite *sprite);
static void sub_8149864(struct Sprite *sprite);
static bool8 TrainerPicCb_Nothing(struct Sprite *sprite);
static bool8 TrainerPicCb_SetSlideOffsets(struct Sprite *sprite);
static bool8 TrainerPicCb_Slide1(struct Sprite *sprite);
static bool8 TrainerPicCb_Slide2(struct Sprite *sprite);

// iwram bss vars
static u8 sTestingTransitionId;
static u8 sTestingTransitionState;
static struct StructRectangularSpiral sRectangularSpiralTransition[4];

// ewram vars
EWRAM_DATA static struct TransitionData *sTransitionStructPtr = NULL;

// const rom data
static const u32 sBigPokeball_Tileset[] = INCBIN_U32("graphics/battle_transitions/big_pokeball.4bpp");
static const u32 sPokeballTrail_Tileset[] = INCBIN_U32("graphics/battle_transitions/pokeball_trail.4bpp");
static const u8 sPokeball_Gfx[] = INCBIN_U8("graphics/battle_transitions/pokeball.4bpp");
static const u32 sEliteFour_Tileset[] = INCBIN_U32("graphics/battle_transitions/elite_four_bg.4bpp");
static const u32 sShrinkingBoxTileset[] = INCBIN_U32("graphics/battle_transitions/shrinking_box.4bpp");
static const u16 sEvilTeam_Palette[] = INCBIN_U16("graphics/battle_transitions/evil_team.gbapal");
static const u32 sTeamRocket_Tileset[] = INCBIN_U32("graphics/battle_transitions/team_rocket.4bpp.lz");
static const u32 sTeamRocket_Tilemap[] = INCBIN_U32("graphics/battle_transitions/team_rocket.bin.lz");
static const u16 sFrontierLogo_Palette[] = INCBIN_U16("graphics/battle_transitions/frontier_logo.gbapal");
static const u32 sFrontierLogo_Tileset[] = INCBIN_U32("graphics/battle_transitions/frontier_logo.4bpp.lz");
static const u32 sFrontierLogo_Tilemap[] = INCBIN_U32("graphics/battle_transitions/frontier_logo.bin.lz");
static const u16 sFrontierSquares_Palette[] = INCBIN_U16("graphics/battle_transitions/frontier_squares_blanktiles.gbapal");
static const u32 sFrontierSquares_FilledBg_Tileset[] = INCBIN_U32("graphics/battle_transitions/frontier_square_1.4bpp.lz");
static const u32 sFrontierSquares_EmptyBg_Tileset[] = INCBIN_U32("graphics/battle_transitions/frontier_square_2.4bpp.lz");
static const u32 sFrontierSquares_Shrink1_Tileset[] = INCBIN_U32("graphics/battle_transitions/frontier_square_3.4bpp.lz");
static const u32 sFrontierSquares_Shrink2_Tileset[] = INCBIN_U32("graphics/battle_transitions/frontier_square_4.4bpp.lz");
static const u32 sFrontierSquares_Tilemap[] = INCBIN_U32("graphics/battle_transitions/frontier_squares.bin");

static const TaskFunc sPhase1_Tasks[B_TRANSITION_COUNT] =
{
    [0 ... B_TRANSITION_COUNT - 1] = &Phase1Task_TransitionAll
};

static const TaskFunc sPhase2_Tasks[B_TRANSITION_COUNT] =
{
    [B_TRANSITION_BLUR] = Phase2Task_Blur,
    [B_TRANSITION_SWIRL] = Phase2Task_Swirl,
    [B_TRANSITION_SHUFFLE] = Phase2Task_Shuffle,
    [B_TRANSITION_BIG_POKEBALL] = Phase2Task_BigPokeball,
    [B_TRANSITION_POKEBALLS_TRAIL] = Phase2Task_PokeballsTrail,
    [B_TRANSITION_CLOCKWISE_BLACKFADE] = Phase2Task_Clockwise_BlackFade,
    [B_TRANSITION_RIPPLE] = Phase2Task_Ripple,
    [B_TRANSITION_WAVE] = Phase2Task_Wave,
    [B_TRANSITION_SLICE] = Phase2Task_Slice,
    [B_TRANSITION_WHITEFADE] = Phase2Task_WhiteFade,
    [B_TRANSITION_GRID_SQUARES] = Phase2Task_GridSquares,
    [B_TRANSITION_SHARDS] = Phase2Task_Shards,
    [B_TRANSITION_LORELEI] = Phase2Task_Sidney,
    [B_TRANSITION_BRUNO] = Phase2Task_Phoebe,
    [B_TRANSITION_AGATHA] = Phase2Task_Glacia,
    [B_TRANSITION_LANCE] = Phase2Task_Drake,
    [B_TRANSITION_CHAMPION] = Phase2Task_Champion,
    [B_TRANSITION_ANTI_CLOCKWISE_SPIRAL] = Phase2Task_AntiClockwiseSpiral,
    [B_TRANSITION_ROCKET] = Phase2Task_Rocket,
    [B_TRANSITION_BLACK_HOLE] = Phase2Task_BlackHole,
    [B_TRANSITION_RECTANGULAR_SPIRAL] = Phase2Task_RectangularSpiral,
    [B_TRANSITION_FRONTIER_LOGO_WIGGLE] = Phase2Task_FrontierLogoWiggle,
    [B_TRANSITION_FRONTIER_LOGO_WAVE] = Phase2Task_FrontierLogoWave,
    [B_TRANSITION_FRONTIER_SQUARES] = Phase2Task_FrontierSquares,
    [B_TRANSITION_FRONTIER_SQUARES_SCROLL] = Phase2Task_FrontierSquaresScroll,
    [B_TRANSITION_FRONTIER_SQUARES_SPIRAL] = Phase2Task_FrontierSquaresSpiral,
    [B_TRANSITION_FRONTIER_CIRCLES_MEET] = Phase2Task_FrontierCirclesMeet,
    [B_TRANSITION_FRONTIER_CIRCLES_CROSS] = Phase2Task_FrontierCirclesCross,
    [B_TRANSITION_FRONTIER_CIRCLES_ASYMMETRIC_SPIRAL] = Phase2Task_FrontierCirclesAsymmetricSpiral,
    [B_TRANSITION_FRONTIER_CIRCLES_SYMMETRIC_SPIRAL] = Phase2Task_FrontierCirclesSymmetricSpiral,
    [B_TRANSITION_FRONTIER_CIRCLES_MEET_IN_SEQ] = Phase2Task_FrontierCirclesMeetInSeq,
    [B_TRANSITION_FRONTIER_CIRCLES_CROSS_IN_SEQ] = Phase2Task_FrontierCirclesCrossInSeq,
    [B_TRANSITION_FRONTIER_CIRCLES_ASYMMETRIC_SPIRAL_IN_SEQ] = Phase2Task_FrontierCirclesAsymmetricSpiralInSeq,
    [B_TRANSITION_FRONTIER_CIRCLES_SYMMETRIC_SPIRAL_IN_SEQ] = Phase2Task_FrontierCirclesSymmetricSpiralInSeq,
};

static const TransitionStateFunc sMainTransitionPhases[] =
{
    &Transition_Phase1,
    &Transition_WaitForPhase1,
    &Transition_Phase2,
    &Transition_WaitForPhase2
};

static const TransitionStateFunc sPhase2_Blur_Funcs[] =
{
    Phase2_Blur_Func1,
    Phase2_Blur_Func2,
    Phase2_Blur_Func3
};

static const TransitionStateFunc sPhase2_Swirl_Funcs[] =
{
    Phase2_Swirl_Func1,
    Phase2_Swirl_Func2,
};

static const TransitionStateFunc sPhase2_Shuffle_Funcs[] =
{
    Phase2_Shuffle_Func1,
    Phase2_Shuffle_Func2,
};

static const TransitionStateFunc sPhase2_Rocket_Funcs[] =
{
    Phase2_Rocket_Func1,
    Phase2_Rocket_Func2,
    Phase2_BigPokeball_Func3,
    Phase2_BigPokeball_Func4,
    Phase2_BigPokeball_Func5,
    Phase2_FramesCountdown,
    Phase2_BigPokeball_Func6
};

static const TransitionStateFunc sPhase2_BigPokeball_Funcs[] =
{
    Phase2_BigPokeball_Func1,
    Phase2_BigPokeball_Func2,
    Phase2_BigPokeball_Func3,
    Phase2_BigPokeball_Func4,
    Phase2_BigPokeball_Func5,
    Phase2_BigPokeball_Func6
};

static const TransitionStateFunc sPhase2_PokeballsTrail_Funcs[] =
{
    Phase2_PokeballsTrail_Func1,
    Phase2_PokeballsTrail_Func2,
    Phase2_PokeballsTrail_Func3
};

static const s16 sUnknown_085C8B88[2] = {-16, 256};
static const s16 sUnknown_085C8B8C[5] = {0, 16, 32, 8, 24};
static const s16 sUnknown_085C8B96[2] = {8, -8};

static const TransitionStateFunc sPhase2_Clockwise_BlackFade_Funcs[] =
{
    Phase2_Clockwise_BlackFade_Func1,
    Phase2_Clockwise_BlackFade_Func2,
    Phase2_Clockwise_BlackFade_Func3,
    Phase2_Clockwise_BlackFade_Func4,
    Phase2_Clockwise_BlackFade_Func5,
    Phase2_Clockwise_BlackFade_Func6,
    Phase2_Clockwise_BlackFade_Func7
};

static const TransitionStateFunc sPhase2_Ripple_Funcs[] =
{
    Phase2_Ripple_Func1,
    Phase2_Ripple_Func2
};

static const TransitionStateFunc sPhase2_Wave_Funcs[] =
{
    Phase2_Wave_Func1,
    Phase2_Wave_Func2,
    Phase2_Wave_Func3
};

static const s16 gUnknown_83FA444[] =
{
    0, 622,
    256, 105,
    0, -105,
    -256, -9838,
    0, 622,
    256, 105,
    0, -105,
    -256, -9838,
};

static const TransitionStateFunc sPhase2_AntiClockwiseSpiral_Funcs[] =
{
    Phase2_AntiClockwiseSpiral_Func1,
    Phase2_AntiClockwiseSpiral_Func2
};

static const TransitionStateFunc sPhase2_Mugshot_Funcs[] =
{
    Phase2_Mugshot_Func1,
    Phase2_Mugshot_Func2,
    Phase2_Mugshot_Func3,
    Phase2_Mugshot_Func4,
    Phase2_Mugshot_Func5,
    Phase2_Mugshot_Func6,
    Phase2_Mugshot_Func7,
    Phase2_Mugshot_Func8,
    Phase2_Mugshot_Func9,
    Phase2_Mugshot_Func10
};

static const u8 sMugshotsTrainerPicIDsTable[MUGSHOTS_COUNT] =
{
    [MUGSHOT_LORELEI] = TRAINER_PIC_LORELEI,
    [MUGSHOT_BRUNO] = TRAINER_PIC_BRUNO,
    [MUGSHOT_AGATHA] = TRAINER_PIC_AGATHA,
    [MUGSHOT_LANCE] = TRAINER_PIC_LANCE,
    [MUGSHOT_CHAMPION] = TRAINER_PIC_CHAMPION_RIVAL,
};

static const s16 sMugshotsOpponentCoords[MUGSHOTS_COUNT][2] =
{
    [MUGSHOT_LORELEI] =  {-8,  0},
    [MUGSHOT_BRUNO] =    {-10, 0},
    [MUGSHOT_AGATHA] =   {0,   0},
    [MUGSHOT_LANCE] =    {-32, 0},
    [MUGSHOT_CHAMPION] = {0,   0},
};

static const TransitionSpriteCallback sTrainerPicSpriteCbs[] =
{
    TrainerPicCb_Nothing,
    TrainerPicCb_SetSlideOffsets,
    TrainerPicCb_Slide1,
    TrainerPicCb_Slide2,
    TrainerPicCb_Nothing,
    TrainerPicCb_Nothing,
    TrainerPicCb_Nothing
};

static const s16 sTrainerPicSlideOffsets1[2] = {12, -12};
static const s16 sTrainerPicSlideOffsets2[2] = {-1, 1};

static const TransitionStateFunc sPhase2_Slice_Funcs[] =
{
    Phase2_Slice_Func1,
    Phase2_Slice_Func2,
    Phase2_Slice_Func3
};

static const u8 gUnknown_085C8C64[] = {39, 119};
static const s16 gUnknown_085C8C66[] = {1, -1};

static const TransitionStateFunc sPhase2_BlackHole_Funcs[] =
{
    Phase2_BlackHole_Func1,
    Phase2_BlackHole_Func2,
    Phase2_BlackHole_Func3
};

static const s16 gUnknown_085C8C80[] = {-6, 4};

static const TransitionStateFunc sPhase2_RectangularSpiral_Funcs[] =
{
    Phase2_RectangularSpiral_Func1,
    Phase2_RectangularSpiral_Func2,
    Phase2_RectangularSpiral_Func3
};

static const s16 gUnknown_085C8C90[] = {1, 27, 275, -1};
static const s16 gUnknown_085C8C98[] = {2, 486, -1};
static const s16 gUnknown_085C8C9E[] = {3, 262, -1};
static const s16 gUnknown_085C8CA4[] = {4, 507, -2};
static const s16 gUnknown_085C8CAA[] = {1, 213, -1};
static const s16 gUnknown_085C8CB0[] = {2, 548, -2};
static const s16 gUnknown_085C8CB6[] = {3, 196, -1};
static const s16 gUnknown_085C8CBC[] = {4, 573, 309, -1};
static const s16 gUnknown_085C8CC4[] = {1, 474, -1};
static const s16 gUnknown_085C8CCA[] = {2, 295, 32, -1};
static const s16 gUnknown_085C8CD2[] = {3, 58, -1};
static const s16 gUnknown_085C8CD8[] = {4, 455, -1};
static const s16 gUnknown_085C8CDE[] = {1, 540, -1};
static const s16 gUnknown_085C8CE4[] = {2, 229, -1};
static const s16 gUnknown_085C8CEA[] = {3, 244, 28, -1};
static const s16 gUnknown_085C8CF2[] = {4, 517, -1};

static const s16 *const gUnknown_085C8CF8[] =
{
    gUnknown_085C8C90,
    gUnknown_085C8CA4,
    gUnknown_085C8C98,
    gUnknown_085C8C9E,
    gUnknown_085C8CEA,
    gUnknown_085C8CE4,
    gUnknown_085C8CF2,
    gUnknown_085C8CDE
};

static const s16 *const gUnknown_085C8D18[] =
{
    gUnknown_085C8CBC,
    gUnknown_085C8CB0,
    gUnknown_085C8CB6,
    gUnknown_085C8CAA,
    gUnknown_085C8CCA,
    gUnknown_085C8CD8,
    gUnknown_085C8CC4,
    gUnknown_085C8CD2
};

static const s16 *const *const gUnknown_085C8D38[] =
{
    gUnknown_085C8CF8,
    gUnknown_085C8D18
};

static const TransitionStateFunc sPhase2_WhiteFade_Funcs[] =
{
    Phase2_WhiteFade_Func1,
    Phase2_WhiteFade_Func2,
    Phase2_WhiteFade_Func3,
    Phase2_WhiteFade_Func4,
    Phase2_WhiteFade_Func5
};

static const s16 sUnknown_085C8DA0[] = {0, 9, 15, 6, 12, 3};

static const TransitionStateFunc sPhase2_GridSquares_Funcs[] =
{
    Phase2_GridSquares_Func1,
    Phase2_GridSquares_Func2,
    Phase2_GridSquares_Func3
};

static const TransitionStateFunc sPhase2_Shards_Funcs[] =
{
    Phase2_Shards_Func1,
    Phase2_Shards_Func2,
    Phase2_Shards_Func3,
    Phase2_Shards_Func4,
    Phase2_Shards_Func5
};

static const s16 sUnknown_085C8DD0[][5] =
{
    {56,    0,      0,      160,    0},
    {104,   160,    240,    88,     1},
    {240,   72,     56,     0,      1},
    {0,     32,     144,    160,    0},
    {144,   160,    184,    0,      1},
    {56,    0,      168,    160,    0},
    {168,   160,    48,     0,      1},
};

static const s16 sUnknown_085C8E16[] = {1, 1, 1, 1, 1, 1, 0};

static const TransitionStateFunc sPhase1_TransitionAll_Funcs[] =
{
    Phase1_TransitionAll_Func1,
    Phase1_TransitionAll_Func2
};

static const struct SpriteFrameImage sSpriteImage_Pokeball[] =
{
    sPokeball_Gfx, sizeof(sPokeball_Gfx)
};

static const union AnimCmd sSpriteAnim_Pokeball[] =
{
    ANIMCMD_FRAME(0, 1),
    ANIMCMD_END
};

static const union AnimCmd *const sSpriteAnimTable_Pokeball[] =
{
    sSpriteAnim_Pokeball
};

static const union AffineAnimCmd sSpriteAffineAnim_Pokeball1[] =
{
    AFFINEANIMCMD_FRAME(0, 0, -4, 1),
    AFFINEANIMCMD_JUMP(0)
};

static const union AffineAnimCmd sSpriteAffineAnim_Pokeball2[] =
{
    AFFINEANIMCMD_FRAME(0, 0, 4, 1),
    AFFINEANIMCMD_JUMP(0)
};

static const union AffineAnimCmd *const sSpriteAffineAnimTable_Pokeball[] =
{
    sSpriteAffineAnim_Pokeball1,
    sSpriteAffineAnim_Pokeball2
};

static const struct SpriteTemplate sSpriteTemplate_Pokeball =
{
    .tileTag = 0xFFFF,
    .paletteTag = FLDEFF_PAL_TAG_POKEBALL,
    .oam = &gObjectEventBaseOam_32x32,
    .anims = sSpriteAnimTable_Pokeball,
    .images = sSpriteImage_Pokeball,
    .affineAnims = sSpriteAffineAnimTable_Pokeball,
    .callback = sub_814713C
};

static const u16 sFieldEffectPal_Pokeball[] = INCBIN_U16("graphics/field_effects/palettes/pokeball.gbapal");

const struct SpritePalette gSpritePalette_Pokeball = {sFieldEffectPal_Pokeball, FLDEFF_PAL_TAG_POKEBALL};

static const u16 sMugshotPal_Sidney[] = INCBIN_U16("graphics/battle_transitions/sidney_bg.gbapal");
static const u16 sMugshotPal_Phoebe[] = INCBIN_U16("graphics/battle_transitions/phoebe_bg.gbapal");
static const u16 sMugshotPal_Glacia[] = INCBIN_U16("graphics/battle_transitions/glacia_bg.gbapal");
static const u16 sMugshotPal_Drake[] = INCBIN_U16("graphics/battle_transitions/drake_bg.gbapal");
static const u16 sMugshotPal_Champion[] = INCBIN_U16("graphics/battle_transitions/wallace_bg.gbapal");
static const u16 sMugshotPal_Brendan[] = INCBIN_U16("graphics/battle_transitions/brendan_bg.gbapal");
static const u16 sMugshotPal_May[] = INCBIN_U16("graphics/battle_transitions/may_bg.gbapal");

static const u16 *const sOpponentMugshotsPals[MUGSHOTS_COUNT] =
{
    [MUGSHOT_LORELEI] = sMugshotPal_Sidney,
    [MUGSHOT_BRUNO] = sMugshotPal_Phoebe,
    [MUGSHOT_AGATHA] = sMugshotPal_Glacia,
    [MUGSHOT_LANCE] = sMugshotPal_Drake,
    [MUGSHOT_CHAMPION] = sMugshotPal_Champion
};

static const u16 *const sPlayerMugshotsPals[GENDER_COUNT] =
{
    [MALE] = sMugshotPal_Brendan,
    [FEMALE] = sMugshotPal_May
};

static const u16 sBigPokeball_Tilemap[] = INCBIN_U16("graphics/battle_transitions/big_pokeball_map.bin");
static const u16 sMugshotsTilemap[] = INCBIN_U16("graphics/battle_transitions/elite_four_bg_map.bin");

static const TransitionStateFunc sPhase2_FrontierLogoWiggle_Funcs[] =
{
    Phase2_FrontierLogoWiggle_Func1,
    Phase2_FrontierLogoWiggle_Func2,
    Phase2_BigPokeball_Func3,
    Phase2_BigPokeball_Func4,
    Phase2_BigPokeball_Func5,
    Phase2_BigPokeball_Func6
};

static const TransitionStateFunc sPhase2_FrontierLogoWave_Funcs[] =
{
    Phase2_FrontierLogoWave_Func1,
    Phase2_FrontierLogoWave_Func2,
    Phase2_FrontierLogoWave_Func3,
    Phase2_FrontierLogoWave_Func4
};

static const TransitionStateFunc sPhase2_FrontierSquares_Funcs[] =
{
    Phase2_FrontierSquares_Func1,
    Phase2_FrontierSquares_Func2,
    Phase2_FrontierSquares_Func3,
    Phase2_FrontierSquares_End
};

static const TransitionStateFunc sPhase2_FrontierSquaresSpiral_Funcs[] =
{
    Phase2_FrontierSquaresSpiral_Func1,
    Phase2_FrontierSquaresSpiral_Func2,
    Phase2_FrontierSquaresSpiral_Func3,
    Phase2_FrontierSquaresSpiral_Func4,
    Phase2_FrontierSquares_End
};

static const TransitionStateFunc sPhase2_FrontierSquaresScroll_Funcs[] =
{
    Phase2_FrontierSquaresScroll_Func1,
    Phase2_FrontierSquaresScroll_Func2,
    Phase2_FrontierSquaresScroll_Func3,
    Phase2_FrontierSquaresScroll_Func4,
    Phase2_FrontierSquaresScroll_Func5
};

static const u8 gUnknown_085C9A30[] = {0x1c, 0x1d, 0x1e, 0x1f, 0x20, 0x21, 0x22, 0x1b, 0x14, 0x0d, 0x06, 0x05, 0x04, 0x03, 0x02, 0x01, 0x00, 0x07, 0x0e, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1a, 0x13, 0x0c, 0x0b, 0x0a, 0x09, 0x08, 0x0f, 0x10, 0x11, 0x12};
static const u8 gUnknown_085C9A53[] = {0x00, 0x10, 0x29, 0x16, 0x2c, 0x02, 0x2b, 0x15, 0x2e, 0x1b, 0x09, 0x30, 0x26, 0x05, 0x39, 0x3b, 0x0c, 0x3f, 0x23, 0x1c, 0x0a, 0x35, 0x07, 0x31, 0x27, 0x17, 0x37, 0x01, 0x3e, 0x11, 0x3d, 0x1e, 0x06, 0x22, 0x0f, 0x33, 0x20, 0x3a, 0x0d, 0x2d, 0x25, 0x34, 0x0b, 0x18, 0x3c, 0x13, 0x38, 0x21, 0x1d, 0x32, 0x28, 0x36, 0x0e, 0x03, 0x2f, 0x14, 0x12, 0x19, 0x04, 0x24, 0x1a, 0x2a, 0x1f, 0x08, 0x00};

void BattleTransition_StartOnField(u8 transitionId)
{
    gMain.callback2 = CB2_OverworldBasic;
    LaunchBattleTransitionTask(transitionId);
}

void BattleTransition_Start(u8 transitionId)
{
    LaunchBattleTransitionTask(transitionId);
}

// in all tasks data[0] is reserved for the state
#define tState          data[0]

// main task that launches sub-tasks for phase1 and phase2
#define tTransitionId   data[1]
#define tTransitionDone data[15]

bool8 IsBattleTransitionDone(void)
{
    u8 taskId = FindTaskIdByFunc(Task_BattleTransitionMain);

    if (gTasks[taskId].tTransitionDone)
    {
        DestroyTask(taskId);
        FREE_AND_SET_NULL(sTransitionStructPtr);
        return TRUE;
    }
    return FALSE;
}

static void LaunchBattleTransitionTask(u8 transitionId)
{
    u8 taskId = CreateTask(Task_BattleTransitionMain, 2);

    gTasks[taskId].tTransitionId = transitionId;
    sTransitionStructPtr = AllocZeroed(sizeof(*sTransitionStructPtr));
}

static void Task_BattleTransitionMain(u8 taskId)
{
    while (sMainTransitionPhases[gTasks[taskId].tState](&gTasks[taskId]));
}

static bool8 Transition_Phase1(struct Task *task)
{
    SetWeatherScreenFadeOut();
    CpuCopy32(gPlttBufferFaded, gPlttBufferUnfaded, 0x400);
    if (sPhase1_Tasks[task->tTransitionId] != NULL)
    {
        CreateTask(sPhase1_Tasks[task->tTransitionId], 4);
        task->tState++;
        return FALSE;
    }
    else
    {
        task->tState = 2;
        return TRUE;
    }
}

static bool8 Transition_WaitForPhase1(struct Task *task)
{
    if (FindTaskIdByFunc(sPhase1_Tasks[task->tTransitionId]) == TASK_NONE)
    {
        task->tState++;
        return TRUE;
    }
    return FALSE;
}

static bool8 Transition_Phase2(struct Task *task)
{
    CreateTask(sPhase2_Tasks[task->tTransitionId], 0);
    task->tState++;
    return FALSE;
}

static bool8 Transition_WaitForPhase2(struct Task *task)
{
    task->tTransitionDone = FALSE;
    if (FindTaskIdByFunc(sPhase2_Tasks[task->tTransitionId]) == TASK_NONE)
        task->tTransitionDone = TRUE;
    return FALSE;
}

#undef tTransitionId
#undef tTransitionDone

static void Phase1Task_TransitionAll(u8 taskId)
{
    if (!gTasks[taskId].tState)
    {
        gTasks[taskId].tState++;
        CreatePhase1Task(0, 0, 2, 2, 2); // creates a sub-task for this sub-task
    }
    else if (IsPhase1Done())
        DestroyTask(taskId);
}

// sub-task for phase2
#define tData1              data[1]
#define tData2              data[2]
#define tData3              data[3]
#define tData4              data[4]
#define tData5              data[5]
#define tData6              data[6]
#define tFuncState          data[7]
#define tFrames             data[8]
#define tOpponentSpriteId   data[13]
#define tPlayerSpriteId     data[14]
#define tMugshotId          data[15]

static void Phase2Task_Blur(u8 taskId)
{
    while (sPhase2_Blur_Funcs[gTasks[taskId].tState](&gTasks[taskId]));
}

static bool8 Phase2_Blur_Func1(struct Task *task)
{
    SetGpuReg(REG_OFFSET_MOSAIC, 0);
    SetGpuRegBits(REG_OFFSET_BG1CNT, BGCNT_MOSAIC);
    SetGpuRegBits(REG_OFFSET_BG2CNT, BGCNT_MOSAIC);
    SetGpuRegBits(REG_OFFSET_BG3CNT, BGCNT_MOSAIC);
    task->tState++;
    return TRUE;
}

static bool8 Phase2_Blur_Func2(struct Task *task)
{
    if (task->tData1)
        task->tData1--;
    else
    {
        task->tData1 = 2;
        if (++task->tData2 == 10)
            BeginNormalPaletteFade(PALETTES_ALL, -1, 0, 0x10, RGB_BLACK);
        SetGpuReg(REG_OFFSET_MOSAIC, (task->tData2 & 15) * 17);
        if (task->tData2 > 14)
            task->tState++;
    }
    return FALSE;
}

static bool8 Phase2_Blur_Func3(struct Task *task)
{
    if (!gPaletteFade.active)
    {
        u8 taskId = FindTaskIdByFunc(Phase2Task_Blur);
        DestroyTask(taskId);
    }
    return FALSE;
}

static void Phase2Task_Swirl(u8 taskId)
{
    while (sPhase2_Swirl_Funcs[gTasks[taskId].tState](&gTasks[taskId]));
}

static bool8 Phase2_Swirl_Func1(struct Task *task)
{
    InitTransitionStructVars();
    ScanlineEffect_Clear();
    BeginNormalPaletteFade(PALETTES_ALL, 4, 0, 0x10, RGB_BLACK);
    sub_8149F98(gScanlineEffectRegBuffers[1], sTransitionStructPtr->field_14, 0, 2, 0, 160);

    SetVBlankCallback(VBlankCB_Phase2_Swirl);
    SetHBlankCallback(HBlankCB_Phase2_Swirl);

    EnableInterrupts(INTR_FLAG_VBLANK | INTR_FLAG_HBLANK);

    task->tState++;
    return FALSE;
}

static bool8 Phase2_Swirl_Func2(struct Task *task)
{
    sTransitionStructPtr->VBlank_DMA = FALSE;
    task->tData1 += 4;
    task->tData2 += 8;

    sub_8149F98(gScanlineEffectRegBuffers[0], sTransitionStructPtr->field_14, task->tData1, 2, task->tData2, 160);

    if (!gPaletteFade.active)
        DestroyTask(FindTaskIdByFunc(Phase2Task_Swirl));

    sTransitionStructPtr->VBlank_DMA++;
    return FALSE;
}

static void VBlankCB_Phase2_Swirl(void)
{
    VBlankCB_BattleTransition();
    if (sTransitionStructPtr->VBlank_DMA)
        DmaCopy16(3, gScanlineEffectRegBuffers[0], gScanlineEffectRegBuffers[1], 320);
}

static void HBlankCB_Phase2_Swirl(void)
{
    s16 var = gScanlineEffectRegBuffers[1][REG_VCOUNT];

    REG_BG1HOFS = var;
    REG_BG2HOFS = var;
    REG_BG3HOFS = var;
}

static void Phase2Task_Shuffle(u8 taskId)
{
    while (sPhase2_Shuffle_Funcs[gTasks[taskId].tState](&gTasks[taskId]));
}

static bool8 Phase2_Shuffle_Func1(struct Task *task)
{
    InitTransitionStructVars();
    ScanlineEffect_Clear();

    BeginNormalPaletteFade(PALETTES_ALL, 4, 0, 0x10, RGB_BLACK);
    memset(gScanlineEffectRegBuffers[1], sTransitionStructPtr->field_16, 0x140);

    SetVBlankCallback(VBlankCB_Phase2_Shuffle);
    SetHBlankCallback(HBlankCB_Phase2_Shuffle);

    EnableInterrupts(INTR_FLAG_VBLANK | INTR_FLAG_HBLANK);

    task->tState++;
    return FALSE;
}

static bool8 Phase2_Shuffle_Func2(struct Task *task)
{
    u32 i, theta;
    u16 amplitude;

    sTransitionStructPtr->VBlank_DMA = FALSE;
    theta = task->tData1;
    amplitude = task->tData2 >> 8;
    task->tData1 += 4224;
    task->tData2 += 384;
    for (i = 0; i < 160; i++, theta += 4224)
        gScanlineEffectRegBuffers[0][i] = sTransitionStructPtr->field_16 + Sin(theta / 256, amplitude);

    if (!gPaletteFade.active)
        DestroyTask(FindTaskIdByFunc(Phase2Task_Shuffle));

    sTransitionStructPtr->VBlank_DMA++;
    return FALSE;
}

static void VBlankCB_Phase2_Shuffle(void)
{
    VBlankCB_BattleTransition();
    if (sTransitionStructPtr->VBlank_DMA)
        DmaCopy16(3, gScanlineEffectRegBuffers[0], gScanlineEffectRegBuffers[1], 320);
}

static void HBlankCB_Phase2_Shuffle(void)
{
    s16 var = gScanlineEffectRegBuffers[1][REG_VCOUNT];

    REG_BG1VOFS = var;
    REG_BG2VOFS = var;
    REG_BG3VOFS = var;
}

static void Phase2Task_BigPokeball(u8 taskId)
{
    while (sPhase2_BigPokeball_Funcs[gTasks[taskId].tState](&gTasks[taskId]));
}

static void Phase2Task_Rocket(u8 taskId)
{
    while (sPhase2_Rocket_Funcs[gTasks[taskId].tState](&gTasks[taskId]));
}

static void sub_814669C(struct Task *task)
{
    s32 i;

    InitTransitionStructVars();
    ScanlineEffect_Clear();

    task->tData1 = 16;
    task->tData2 = 0;
    task->tData4 = 0;
    task->tData5 = 0x4000;
    sTransitionStructPtr->WININ = WININ_WIN0_ALL;
    sTransitionStructPtr->WINOUT = 0;
    sTransitionStructPtr->WIN0H = DISPLAY_WIDTH;
    sTransitionStructPtr->WIN0V = DISPLAY_HEIGHT;
    sTransitionStructPtr->BLDCNT = BLDCNT_TGT1_BG0 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL;
    sTransitionStructPtr->BLDALPHA = BLDALPHA_BLEND(task->tData2, task->tData1);

    for (i = 0; i < 160; i++)
        gScanlineEffectRegBuffers[1][i] = 240;

    SetVBlankCallback(VBlankCB0_Phase2_BigPokeball);
}

static bool8 Phase2_Rocket_Func1(struct Task *task)
{
    u16 *tilemap, *tileset;

    task->tFrames = 30;
    sub_814669C(task);
    GetBg0TilesDst(&tilemap, &tileset);
    CpuFill16(0, tilemap, 0x800);
    LZ77UnCompVram(sTeamRocket_Tileset, tileset);
    LoadPalette(sEvilTeam_Palette, 0xF0, 0x20);

    task->tState++;
    return FALSE;
}

static bool8 Phase2_BigPokeball_Func1(struct Task *task)
{
    u16 *tilemap, *tileset;

    sub_814669C(task);
    GetBg0TilesDst(&tilemap, &tileset);
    CpuFill16(0, tilemap, 0x800);
    CpuCopy16(sBigPokeball_Tileset, tileset, 0x580);
    LoadPalette(sFieldEffectPal_Pokeball, 0xF0, 0x20);

    task->tState++;
    return FALSE;
}

#define SOME_VRAM_STORE(ptr, posY, posX, toStore)                       \
{                                                                       \
    u32 index = (posY) * 32 + posX;                                     \
    ptr[index] = toStore;                                               \
}

static bool8 Phase2_BigPokeball_Func2(struct Task *task)
{
    s32 i, j;
    u16 *tilemap, *tileset;
    const u16 *BigPokeballMap;

    GetBg0TilesDst(&tilemap, &tileset);
    BigPokeballMap = sBigPokeball_Tilemap;
    for (i = 0; i < 20; i++)
    {
        for (j = 0; j < 30; j++, BigPokeballMap++)
        {
            SOME_VRAM_STORE(tilemap, i, j, *BigPokeballMap | 0xF000);
        }
    }
    sub_8149F98(gScanlineEffectRegBuffers[0], 0, task->tData4, 132, task->tData5, 160);

    task->tState++;
    return TRUE;
}

static bool8 Phase2_Rocket_Func2(struct Task *task)
{
    u16 *tilemap, *tileset;

    GetBg0TilesDst(&tilemap, &tileset);
    LZ77UnCompVram(sTeamRocket_Tilemap, tilemap);
    sub_8149F98(gScanlineEffectRegBuffers[0], 0, task->tData4, 132, task->tData5, 160);

    task->tState++;
    return FALSE;
}

static bool8 Phase2_BigPokeball_Func3(struct Task *task)
{
    sTransitionStructPtr->VBlank_DMA = FALSE;
    if (!task->tData3 || --task->tData3 == 0)
    {
        task->tData2++;
        task->tData3 = 2;
    }
    sTransitionStructPtr->BLDALPHA = BLDALPHA_BLEND(task->tData2, task->tData1);
    if (task->tData2 > 15)
        task->tState++;
    task->tData4 += 12;
    task->tData5 -= 384;

    sub_8149F98(gScanlineEffectRegBuffers[0], 0, task->tData4, 132, task->tData5 >> 8, 160);

    sTransitionStructPtr->VBlank_DMA++;
    return FALSE;
}

static bool8 Phase2_BigPokeball_Func4(struct Task *task)
{
    sTransitionStructPtr->VBlank_DMA = FALSE;
    if (!task->tData3 || --task->tData3 == 0)
    {
        task->tData1--;
        task->tData3 = 2;
    }
    sTransitionStructPtr->BLDALPHA = BLDALPHA_BLEND(task->tData2, task->tData1);
    if (!task->tData1)
        task->tState++;
    if (task->tData5 > 0)
    {
        task->tData4 += 12;
        task->tData5 -= 384;
    }
    else
        task->tData5 = 0;

    sub_8149F98(gScanlineEffectRegBuffers[0], 0, task->tData4, 132, task->tData5 >> 8, 160);

    sTransitionStructPtr->VBlank_DMA++;
    return FALSE;
}

static bool8 Phase2_BigPokeball_Func5(struct Task *task)
{
    sTransitionStructPtr->VBlank_DMA = FALSE;
    if (task->tData5 > 0)
    {
        task->tData4 += 12;
        task->tData5 -= 384;
    }
    else
        task->tData5 = 0;

    sub_8149F98(gScanlineEffectRegBuffers[0], 0, task->tData4, 132, task->tData5 >> 8, 160);

    if (task->tData5 <= 0)
    {
        task->tState++;
        task->tData1 = 160;
        task->tData2 = 256;
        task->tData3 = 0;
    }

    sTransitionStructPtr->VBlank_DMA++;
    return FALSE;
}

static bool8 Phase2_FramesCountdown(struct Task *task)
{
    if (--task->tFrames == 0)
        task->tState++;
    return FALSE;
}

static bool8 Phase2_WaitPaletteFade(struct Task *task)
{
    if (!gPaletteFade.active)
        task->tState++;
    return FALSE;
}

static bool8 Phase2_BigPokeball_Func6(struct Task *task)
{
    sTransitionStructPtr->VBlank_DMA = FALSE;
    if (task->tData2 < 2048)
        task->tData2 += 256;
    if (task->tData1)
    {
        task->tData1 -= (task->tData2 >> 8);
        if (task->tData1 < 0)
            task->tData1 = 0;
    }
    sub_814A014(gScanlineEffectRegBuffers[0], 120, 80, task->tData1);
    if (!task->tData1)
    {
        DmaStop(0);
        FadeScreenBlack();
        DestroyTask(FindTaskIdByFunc(task->func));
    }
    if (!task->tData3)
    {
        task->tData3++;
        SetVBlankCallback(VBlankCB1_Phase2_BigPokeball);
    }

    sTransitionStructPtr->VBlank_DMA++;
    return FALSE;
}

static void Transition_BigPokeball_Vblank(void)
{
    DmaStop(0);
    VBlankCB_BattleTransition();
    if (sTransitionStructPtr->VBlank_DMA)
        DmaCopy16(3, gScanlineEffectRegBuffers[0], gScanlineEffectRegBuffers[1], 320);
    REG_WININ = sTransitionStructPtr->WININ;
    REG_WINOUT = sTransitionStructPtr->WINOUT;
    REG_WIN0V = sTransitionStructPtr->WIN0V;
    REG_BLDCNT = sTransitionStructPtr->BLDCNT;
    REG_BLDALPHA = sTransitionStructPtr->BLDALPHA;
}

static void VBlankCB0_Phase2_BigPokeball(void)
{
    Transition_BigPokeball_Vblank();
    DmaSet(0, gScanlineEffectRegBuffers[1], &REG_BG0HOFS, ((DMA_ENABLE | DMA_START_HBLANK | DMA_REPEAT | DMA_16BIT | DMA_SRC_INC | DMA_DEST_FIXED) << 16) | 1);
}

static void VBlankCB1_Phase2_BigPokeball(void)
{
    Transition_BigPokeball_Vblank();
    DmaSet(0, gScanlineEffectRegBuffers[1], &REG_WIN0H, ((DMA_ENABLE | DMA_START_HBLANK | DMA_REPEAT | DMA_16BIT | DMA_SRC_INC | DMA_DEST_FIXED) << 16) | 1);
}

static void Phase2Task_PokeballsTrail(u8 taskId)
{
    while (sPhase2_PokeballsTrail_Funcs[gTasks[taskId].tState](&gTasks[taskId]));
}

static bool8 Phase2_PokeballsTrail_Func1(struct Task *task)
{
    u16 *tilemap, *tileset;

    GetBg0TilesDst(&tilemap, &tileset);
    CpuSet(sPokeballTrail_Tileset, tileset, 0x20);
    CpuFill32(0, tilemap, 0x800);
    LoadPalette(sFieldEffectPal_Pokeball, 0xF0, 0x20);

    task->tState++;
    return FALSE;
}

static bool8 Phase2_PokeballsTrail_Func2(struct Task *task)
{
    s32 i, rand;
    s16 arr0[ARRAY_COUNT(sUnknown_085C8B88)];
    s16 arr1[ARRAY_COUNT(sUnknown_085C8B8C)];

    memcpy(arr0, sUnknown_085C8B88, sizeof(sUnknown_085C8B88));
    memcpy(arr1, sUnknown_085C8B8C, sizeof(sUnknown_085C8B8C));
    rand = Random() & 1;
    for (i = 0; i <= 4; i++, rand ^= 1)
    {
        gFieldEffectArguments[0] = arr0[rand];      // x
        gFieldEffectArguments[1] = (i * 32) + 16;   // y
        gFieldEffectArguments[2] = rand;
        gFieldEffectArguments[3] = arr1[i];
        FieldEffectStart(FLDEFF_POKEBALL);
    }

    task->tState++;
    return FALSE;
}

static bool8 Phase2_PokeballsTrail_Func3(struct Task *task)
{
    if (!FieldEffectActiveListContains(FLDEFF_POKEBALL))
    {
        FadeScreenBlack();
        DestroyTask(FindTaskIdByFunc(Phase2Task_PokeballsTrail));
    }
    return FALSE;
}

bool8 FldEff_Pokeball(void)
{
    u8 spriteId = CreateSpriteAtEnd(&sSpriteTemplate_Pokeball, gFieldEffectArguments[0], gFieldEffectArguments[1], 0);
    gSprites[spriteId].oam.priority = 0;
    gSprites[spriteId].oam.affineMode = ST_OAM_AFFINE_NORMAL;
    gSprites[spriteId].data[0] = gFieldEffectArguments[2];
    gSprites[spriteId].data[1] = gFieldEffectArguments[3];
    gSprites[spriteId].data[2] = -1;
    InitSpriteAffineAnim(&gSprites[spriteId]);
    StartSpriteAffineAnim(&gSprites[spriteId], gFieldEffectArguments[2]);
    return FALSE;
}

static void sub_814713C(struct Sprite *sprite)
{
    s16 arr0[ARRAY_COUNT(sUnknown_085C8B96)];

    memcpy(arr0, sUnknown_085C8B96, sizeof(sUnknown_085C8B96));
    if (sprite->data[1])
        sprite->data[1]--;
    else
    {
        if ((u16)sprite->x <= DISPLAY_WIDTH)
        {
            s16 posX = sprite->x >> 3;
            s16 posY = sprite->y >> 3;

            if (posX != sprite->data[2])
            {
                u16 *ptr;

                sprite->data[2] = posX;
                ptr = (u16 *)BG_SCREEN_ADDR((GetGpuReg(REG_OFFSET_BG0CNT) >> 8) & 0x1F);
                SOME_VRAM_STORE(ptr, posY - 2, posX, 0xF001);
                SOME_VRAM_STORE(ptr, posY - 1, posX, 0xF001);
                SOME_VRAM_STORE(ptr, posY - 0, posX, 0xF001);
                SOME_VRAM_STORE(ptr, posY + 1, posX, 0xF001);
            }
        }
        sprite->x += arr0[sprite->data[0]];
        if (sprite->x < -15 || sprite->x > 255)
            FieldEffectStop(sprite, FLDEFF_POKEBALL);
    }
}

static void Phase2Task_Clockwise_BlackFade(u8 taskId)
{
    while (sPhase2_Clockwise_BlackFade_Funcs[gTasks[taskId].tState](&gTasks[taskId]));
}

static bool8 Phase2_Clockwise_BlackFade_Func1(struct Task *task)
{
    u32 i;

    InitTransitionStructVars();
    ScanlineEffect_Clear();

    sTransitionStructPtr->WININ = 0;
    sTransitionStructPtr->WINOUT = WINOUT_WIN01_ALL;
    sTransitionStructPtr->WIN0H = -3855;
    sTransitionStructPtr->WIN0V = DISPLAY_HEIGHT;

    for (i = 0; i < 160; i++)
        gScanlineEffectRegBuffers[1][i] = WIN_RANGE(0xF3, 0xF4);

    SetVBlankCallback(VBlankCB_Phase2_Clockwise_BlackFade);
    sTransitionStructPtr->data[4] = 120;

    task->tState++;
    return TRUE;
}

static bool8 Phase2_Clockwise_BlackFade_Func2(struct Task *task)
{
    sTransitionStructPtr->VBlank_DMA = FALSE;

    sub_814A1AC(sTransitionStructPtr->data, 120, 80, sTransitionStructPtr->data[4], -1, 1, 1);
    do
    {
        gScanlineEffectRegBuffers[0][sTransitionStructPtr->data[3]] = WIN_RANGE(0x78, sTransitionStructPtr->data[2] + 1);
    } while (!sub_814A228(sTransitionStructPtr->data, TRUE, TRUE));

    sTransitionStructPtr->data[4] += 32;
    if (sTransitionStructPtr->data[4] >= 240)
    {
        sTransitionStructPtr->data[5] = 0;
        task->tState++;
    }

    sTransitionStructPtr->VBlank_DMA++;
    return FALSE;
}

static bool8 Phase2_Clockwise_BlackFade_Func3(struct Task *task)
{
    s16 left, right;
    vu8 finished = FALSE;

    sTransitionStructPtr->VBlank_DMA = FALSE;

    sub_814A1AC(sTransitionStructPtr->data, 120, 80, 240, sTransitionStructPtr->data[5], 1, 1);

    while (TRUE)
    {
        left = 120;
        right = sTransitionStructPtr->data[2] + 1;
        if (sTransitionStructPtr->data[5] >= 80)
        {
            left = sTransitionStructPtr->data[2];
            right = 240;
        }
        gScanlineEffectRegBuffers[0][sTransitionStructPtr->data[3]] = WIN_RANGE2(left, right);
        if (finished)
            break;
        finished = sub_814A228(sTransitionStructPtr->data, TRUE, TRUE);
    }

    sTransitionStructPtr->data[5] += 16;
    if (sTransitionStructPtr->data[5] >= 160)
    {
        sTransitionStructPtr->data[4] = 240;
        task->tState++;
    }
    else
    {
        while (sTransitionStructPtr->data[3] < sTransitionStructPtr->data[5])
            gScanlineEffectRegBuffers[0][++sTransitionStructPtr->data[3]] = WIN_RANGE2(left, right);
    }

    sTransitionStructPtr->VBlank_DMA++;
    return FALSE;
}

static bool8 Phase2_Clockwise_BlackFade_Func4(struct Task *task)
{
    sTransitionStructPtr->VBlank_DMA = FALSE;

    sub_814A1AC(sTransitionStructPtr->data, 120, 80, sTransitionStructPtr->data[4], 160, 1, 1);
    do
    {
        gScanlineEffectRegBuffers[0][sTransitionStructPtr->data[3]] = (sTransitionStructPtr->data[2] << 8) | 0xF0;
    } while (!sub_814A228(sTransitionStructPtr->data, TRUE, TRUE));

    sTransitionStructPtr->data[4] -= 32;
    if (sTransitionStructPtr->data[4] <= 0)
    {
        sTransitionStructPtr->data[5] = 160;
        task->tState++;
    }

    sTransitionStructPtr->VBlank_DMA++;
    return FALSE;
}

static bool8 Phase2_Clockwise_BlackFade_Func5(struct Task *task)
{
    s16 left, right;
    u16 win0H;
    vu8 finished = FALSE;

    sTransitionStructPtr->VBlank_DMA = FALSE;

    sub_814A1AC(sTransitionStructPtr->data, 120, 80, 0, sTransitionStructPtr->data[5], 1, 1);

    while (TRUE)
    {
        left = sTransitionStructPtr->data[2];
        right = (gScanlineEffectRegBuffers[0][sTransitionStructPtr->data[3]]) & 0xFF;

        if (sTransitionStructPtr->data[5] <= 80)
        {
            left = 120;
            right = sTransitionStructPtr->data[2];
        }
        win0H = WIN_RANGE2(left, right);
        gScanlineEffectRegBuffers[0][sTransitionStructPtr->data[3]] = win0H;
        if (finished)
            break;
        finished = sub_814A228(sTransitionStructPtr->data, TRUE, TRUE);
    }

    sTransitionStructPtr->data[5] -= 16;
    if (sTransitionStructPtr->data[5] <= 0)
    {
        sTransitionStructPtr->data[4] = 0;
        task->tState++;
    }
    else
    {
        while (sTransitionStructPtr->data[3] > sTransitionStructPtr->data[5])
            gScanlineEffectRegBuffers[0][--sTransitionStructPtr->data[3]] = WIN_RANGE2(left, right);
    }
    sTransitionStructPtr->VBlank_DMA++;
    return FALSE;
}

static bool8 Phase2_Clockwise_BlackFade_Func6(struct Task *task)
{
    sTransitionStructPtr->VBlank_DMA = FALSE;

    sub_814A1AC(sTransitionStructPtr->data, 120, 80, sTransitionStructPtr->data[4], 0, 1, 1);
    do
    {
        s16 left = 120;
        s16 right = sTransitionStructPtr->data[2];

        if (sTransitionStructPtr->data[2] >= 120)
        {
            left = 0;
            right = 240;
        }
        gScanlineEffectRegBuffers[0][sTransitionStructPtr->data[3]] = WIN_RANGE2(left, right);

    } while (!sub_814A228(sTransitionStructPtr->data, TRUE, TRUE));

    sTransitionStructPtr->data[4] += 32;
    if (sTransitionStructPtr->data[2] > 120)
        task->tState++;

    sTransitionStructPtr->VBlank_DMA++;
    return FALSE;
}

static bool8 Phase2_Clockwise_BlackFade_Func7(struct Task *task)
{
    DmaStop(0);
    FadeScreenBlack();
    DestroyTask(FindTaskIdByFunc(Phase2Task_Clockwise_BlackFade));
    return FALSE;
}

static void VBlankCB_Phase2_Clockwise_BlackFade(void)
{
    DmaStop(0);
    VBlankCB_BattleTransition();
    if (sTransitionStructPtr->VBlank_DMA)
        DmaCopy16(3, gScanlineEffectRegBuffers[0], gScanlineEffectRegBuffers[1], 320);
    REG_WININ = sTransitionStructPtr->WININ;
    REG_WINOUT = sTransitionStructPtr->WINOUT;
    REG_WIN0V = sTransitionStructPtr->WIN0V;
    REG_WIN0H = gScanlineEffectRegBuffers[1][0];
    DmaSet(0, gScanlineEffectRegBuffers[1], &REG_WIN0H, ((DMA_ENABLE | DMA_START_HBLANK | DMA_REPEAT | DMA_16BIT | DMA_SRC_INC | DMA_DEST_FIXED) << 16) | 1);
}

static void Phase2Task_Ripple(u8 taskId)
{
    while (sPhase2_Ripple_Funcs[gTasks[taskId].tState](&gTasks[taskId]));
}

static bool8 Phase2_Ripple_Func1(struct Task *task)
{
    u32 i;

    InitTransitionStructVars();
    ScanlineEffect_Clear();

    for (i = 0; i < 160; i++)
        gScanlineEffectRegBuffers[1][i] = sTransitionStructPtr->field_16;

    SetVBlankCallback(VBlankCB_Phase2_Ripple);
    SetHBlankCallback(HBlankCB_Phase2_Ripple);

    EnableInterrupts(INTR_FLAG_HBLANK);

    task->tState++;
    return TRUE;
}

static bool8 Phase2_Ripple_Func2(struct Task *task)
{
    u32 i;
    s16 amplitude;
    u32 theta, frequency;

    sTransitionStructPtr->VBlank_DMA = FALSE;
    amplitude = task->tData2 >> 8;
    theta = task->tData1;
    frequency = 384;
    task->tData1 += 0x400;
    if (task->tData2 <= 0x1FFF)
        task->tData2 += 384;

    for (i = 0; i < 160; i++, theta += frequency)
    {
        s16 var = theta >> 8;
        gScanlineEffectRegBuffers[0][i] = sTransitionStructPtr->field_16 + Sin(var & 0xffff, amplitude);
    }

    if (++task->tData3 == 41)
    {
        task->tData4++;
        BeginNormalPaletteFade(PALETTES_ALL, -8, 0, 0x10, RGB_BLACK);
    }

    if (task->tData4 && !gPaletteFade.active)
        DestroyTask(FindTaskIdByFunc(Phase2Task_Ripple));

    sTransitionStructPtr->VBlank_DMA++;
    return FALSE;
}

static void VBlankCB_Phase2_Ripple(void)
{
    VBlankCB_BattleTransition();
    if (sTransitionStructPtr->VBlank_DMA)
        DmaCopy16(3, gScanlineEffectRegBuffers[0], gScanlineEffectRegBuffers[1], 320);
}

static void HBlankCB_Phase2_Ripple(void)
{
    s16 var = gScanlineEffectRegBuffers[1][REG_VCOUNT];

    REG_BG1VOFS = var;
    REG_BG2VOFS = var;
    REG_BG3VOFS = var;
}

static void Phase2Task_Wave(u8 taskId)
{
    while (sPhase2_Wave_Funcs[gTasks[taskId].tState](&gTasks[taskId]));
}

static bool8 Phase2_Wave_Func1(struct Task *task)
{
    u32 i;

    InitTransitionStructVars();
    ScanlineEffect_Clear();

    sTransitionStructPtr->WININ = WININ_WIN0_ALL;
    sTransitionStructPtr->WINOUT = 0;
    sTransitionStructPtr->WIN0H = WIN_RANGE(0, DISPLAY_WIDTH);
    sTransitionStructPtr->WIN0V = WIN_RANGE(0, DISPLAY_HEIGHT);

    for (i = 0; i < 160; i++)
        gScanlineEffectRegBuffers[1][i] = WIN_RANGE(0, 242);

    SetVBlankCallback(VBlankCB_Phase2_Wave);

    task->tState++;
    return TRUE;
}

static bool8 Phase2_Wave_Func2(struct Task *task)
{
    u32 i, theta;
    u16* winVal;
    bool32 nextFunc;

    sTransitionStructPtr->VBlank_DMA = FALSE;
    winVal = gScanlineEffectRegBuffers[0];
    theta = task->tData2;
    task->tData2 += 16;
    task->tData1 += 8;
    for (i = 0, nextFunc = TRUE; i < 160; i++, theta += 4, winVal++)
    {
        s16 left = task->tData1 + Sin(theta, 40);
        if (left < 0)
            left = 0;
        if (left > 240)
            left = 240;
        *winVal = (left << 8) | (0xF1);
        if (left < 240)
            nextFunc = FALSE;
    }
    if (nextFunc)
        task->tState++;

    sTransitionStructPtr->VBlank_DMA++;
    return FALSE;
}

static bool8 Phase2_Wave_Func3(struct Task *task)
{
    DmaStop(0);
    FadeScreenBlack();
    DestroyTask(FindTaskIdByFunc(Phase2Task_Wave));
    return FALSE;
}

static void VBlankCB_Phase2_Wave(void)
{
    DmaStop(0);
    VBlankCB_BattleTransition();
    if (sTransitionStructPtr->VBlank_DMA)
        DmaCopy16(3, gScanlineEffectRegBuffers[0], gScanlineEffectRegBuffers[1], 320);
    REG_WININ = sTransitionStructPtr->WININ;
    REG_WINOUT = sTransitionStructPtr->WINOUT;
    REG_WIN0V = sTransitionStructPtr->WIN0V;
    DmaSet(0, gScanlineEffectRegBuffers[1], &REG_WIN0H, ((DMA_ENABLE | DMA_START_HBLANK | DMA_REPEAT | DMA_SRC_INC | DMA_DEST_FIXED) << 16) | 1);
}

static void Phase2Task_AntiClockwiseSpiral(u8 taskId)
{
    while (sPhase2_AntiClockwiseSpiral_Funcs[gTasks[taskId].tState](&gTasks[taskId]));
}

static void BT_AntiClockwiseSpiral_DoUpdateFrame(s16 initRadius, s16 deltaAngleMax, u8 offsetMaybe)
{
    u8 theta = 0;
    s32 i;
    s16 amplitude1, amplitude2;
    s32 y1;
    s16 x1;
    s32 y2;
    s16 x2;

    for (i = 320; i < 960; i++)
        gScanlineEffectRegBuffers[1][i] = 120;

    for (i = 0; i < (deltaAngleMax * 16); i++, ++theta)
    {
        amplitude1 = initRadius + (theta >> 3);
        if ((theta >> 3) != ((theta + 1) >> 3))
            amplitude2 = amplitude1 + 1;
        else
            amplitude2 = amplitude1;

        y1 = 80 - Sin(theta, amplitude1);
        x1 = Cos(theta, amplitude1) + 120;
        y2 = 80 - Sin(theta + 1, amplitude2);
        x2 = Cos(theta + 1, amplitude2) + 120;

        if ((y1 < 0 && y2 < 0)
         || (y1 > 159 && y2 > 159))
            continue;

        if (y1 < 0)
            y1 = 0;
        if (y1 > 159)
            y1 = 159;
        if (x1 < 0)
            x1 = 0;
        if (x1 > 255)
            x1 = 255;
        if (y2 < 0)
            y2 = 0;
        if (y2 > 159)
            y2 = 159;
        if (x2 < 0)
            x2 = 0;
        if (x2 > 255)
            x2 = 255;

        y2 -= y1;

        if (theta >= 64 && theta < 192)
        {
            gScanlineEffectRegBuffers[1][y1 + 320] = x1;

            if (!y2)
                continue;

            x2 -= x1;
            if (x2 < -1 && x1 > 1)
                --x1;
            else if (x2 > 1 && x1 < 255)
                ++x1;

            if (y2 < 0)
                for (; y2 < 0; y2++)
                    gScanlineEffectRegBuffers[1][y1 + y2 + 320] = x1;
            else
                for (; y2 > 0; y2--)
                    gScanlineEffectRegBuffers[1][y1 + y2 + 320] = x1;
        }
        else
        {
            gScanlineEffectRegBuffers[1][y1 + 480] = x1;

            if (!y2)
                continue;

            x2 -= x1;
            if (x2 < -1 && x1 > 1)
                --x1;
            else if (x2 > 1 && x1 < 255)
                ++x1;

            if (y2 < 0)
                for (; y2 < 0; y2++)
                    gScanlineEffectRegBuffers[1][y1 + y2 + 480] = x1;
            else
                for (; y2 > 0; y2--)
                    gScanlineEffectRegBuffers[1][y1 + y2 + 480] = x1;
        }
    }

    if (!offsetMaybe || !deltaAngleMax % 4)
    {
        for (i = 0; i < 160; i++)
            gScanlineEffectRegBuffers[1][i * 2 + offsetMaybe] = gScanlineEffectRegBuffers[1][i + 320] << 8 | gScanlineEffectRegBuffers[1][i + 480];
        return;
    }

    y1 = Sin(deltaAngleMax * 16, initRadius + (deltaAngleMax << 1));

    switch (deltaAngleMax / 4)
    {
    case 0:
        if (y1 > 80)
            y1 = 80;
        for (i = y1; i > 0; i--)
        {
            sTransitionStructPtr->data[2] = x1 = ((i * gUnknown_83FA444[deltaAngleMax]) >> 8) + 120;
            if (x1 < 0 || x1 > 255)
                continue;
            sTransitionStructPtr->field_14 = 400 - i;
            sTransitionStructPtr->data[10] = gScanlineEffectRegBuffers[1][400 - i];
            if (gScanlineEffectRegBuffers[1][560 - i] < x1)
                gScanlineEffectRegBuffers[1][560 - i] = 120;
            else if (gScanlineEffectRegBuffers[1][400 - i] < x1)
                gScanlineEffectRegBuffers[1][400 - i] = x1;
        }
        break;
    case 1:
        if (y1 > 80)
            y1 = 80;
        for (i = y1; i > 0; i--)
        {
            sTransitionStructPtr->data[2] = x1 = ((i * gUnknown_83FA444[deltaAngleMax]) >> 8) + 120;
            if (x1 < 0 || x1 > 255)
                continue;
            sTransitionStructPtr->field_14 = 400 - i;
            sTransitionStructPtr->data[10] = gScanlineEffectRegBuffers[1][400 - i];
            if (gScanlineEffectRegBuffers[1][400 - i] < x1)
                gScanlineEffectRegBuffers[1][400 - i] = x1;
        }
        break;
    case 2:
        if (y1 < -79)
            y1 = -79;
        for (i = y1; i <= 0; i++)
        {
            sTransitionStructPtr->data[2] = x1 = ((i * gUnknown_83FA444[deltaAngleMax]) >> 8) + 120;
            if (x1 < 0 || x1 > 255)
                continue;
            sTransitionStructPtr->field_14 = 560 - i;
            sTransitionStructPtr->data[10] = gScanlineEffectRegBuffers[1][560 - i];
            if (gScanlineEffectRegBuffers[1][400 - i] >= x1)
                gScanlineEffectRegBuffers[1][400 - i] = 120;
            else if (gScanlineEffectRegBuffers[1][560 - i] > x1)
                gScanlineEffectRegBuffers[1][560 - i] = x1;
        }
        break;
    case 3:
        if (y1 < -79)
            y1 = -79;
        for (i = y1; i <= 0; i++)
        {
            sTransitionStructPtr->data[2] = x1 = ((i * gUnknown_83FA444[deltaAngleMax]) >> 8) + 120;
            if (x1 < 0 || x1 > 255)
                continue;
            sTransitionStructPtr->field_14 = 560 - i;
            sTransitionStructPtr->data[10] = gScanlineEffectRegBuffers[1][560 - i];
            if (gScanlineEffectRegBuffers[1][560 - i] > x1)
                gScanlineEffectRegBuffers[1][560 - i] = x1;
        }
    default:
        break;
    }

    for (i = 0; i < 160; i++)
        gScanlineEffectRegBuffers[1][i * 2 + offsetMaybe] = (gScanlineEffectRegBuffers[1][i + 320] << 8) | gScanlineEffectRegBuffers[1][i + 480];
}

static bool8 Phase2_AntiClockwiseSpiral_Func1(struct Task *task)
{
    InitTransitionStructVars();
    ScanlineEffect_Clear();
    sTransitionStructPtr->WININ = 0;
    sTransitionStructPtr->WINOUT = WININ_WIN0_ALL;
    sTransitionStructPtr->WIN0H = WIN_RANGE(0x78, 0x78);
    sTransitionStructPtr->WIN0V = WIN_RANGE(0x30, 0x70);
    sTransitionStructPtr->field_C = WIN_RANGE(0x10, 0x90);
    sTransitionStructPtr->field_20 = 0;
    DmaCopy16(3, gScanlineEffectRegBuffers[1], gScanlineEffectRegBuffers[0], 640);
    SetVBlankCallback(VBlankCB_AntiClockwiseSpiral);
    ++task->tState;
    task->data[1] = 0;
    task->data[2] = 0;
    return FALSE;
}

static bool8 Phase2_AntiClockwiseSpiral_Func2(struct Task *task)
{
    s16 win_top, win_bottom;

    BT_AntiClockwiseSpiral_DoUpdateFrame(task->data[2], task->data[1], 1);
    sTransitionStructPtr->VBlank_DMA |= TRUE;
    if (++task->data[1] == 17)
    {
        BT_AntiClockwiseSpiral_DoUpdateFrame(task->data[2], 16, 0);
        win_top = 48 - task->data[2];
        if (win_top < 0)
            win_top = 0;
        win_bottom = task->data[2] + 112;
        if (win_bottom > 255)
            win_bottom = 255;
        sTransitionStructPtr->WIN0V = win_top | win_bottom; // UB: win_top should be shifted
        task->data[2] += 32;
        task->data[1] = 0;
        BT_AntiClockwiseSpiral_DoUpdateFrame(task->data[2], 0, 1);
        win_top = 48 - task->data[2];
        if (win_top < 0)
            win_top = 0;
        win_bottom = task->data[2] + 112;
        if (win_bottom > 255)
            win_bottom = 255;
        sTransitionStructPtr->field_C = win_top | win_bottom; // UB: win_top should be shifted
        sTransitionStructPtr->VBlank_DMA |= TRUE;
        if (task->data[2] > 159)
        {
            sTransitionStructPtr->field_20 = 1;
            FadeScreenBlack();
        }
    }
    return FALSE;
}

static void VBlankCB_AntiClockwiseSpiral(void)
{
    DmaStop(0);
    VBlankCB_BattleTransition();
    if (sTransitionStructPtr->field_20)
        DestroyTask(FindTaskIdByFunc(Phase2Task_AntiClockwiseSpiral));
    else
    {
        if (sTransitionStructPtr->VBlank_DMA)
        {
            DmaCopy16(3, gScanlineEffectRegBuffers[1], gScanlineEffectRegBuffers[0], 640);
            sTransitionStructPtr->VBlank_DMA = FALSE;
        }
        SetGpuReg(REG_OFFSET_WININ, sTransitionStructPtr->WININ);
        SetGpuReg(REG_OFFSET_WINOUT, sTransitionStructPtr->WINOUT);
        SetGpuReg(REG_OFFSET_WIN0V, sTransitionStructPtr->WIN0V);
        SetGpuReg(REG_OFFSET_WIN1V, sTransitionStructPtr->field_C);
        SetGpuReg(REG_OFFSET_WIN0H, gScanlineEffectRegBuffers[0][0]);
        SetGpuReg(REG_OFFSET_WIN1H, gScanlineEffectRegBuffers[0][1]);
        DmaSet(0, gScanlineEffectRegBuffers[0], &REG_WIN0H, ((DMA_ENABLE | DMA_START_HBLANK | DMA_REPEAT | DMA_32BIT | DMA_SRC_INC | DMA_DEST_FIXED) << 16) | 1);
    }
}

static void Phase2Task_Sidney(u8 taskId)
{
    gTasks[taskId].tMugshotId = MUGSHOT_LORELEI;
    Phase2Task_MugShotTransition(taskId);
}

static void Phase2Task_Phoebe(u8 taskId)
{
    gTasks[taskId].tMugshotId = MUGSHOT_BRUNO;
    Phase2Task_MugShotTransition(taskId);
}

static void Phase2Task_Glacia(u8 taskId)
{
    gTasks[taskId].tMugshotId = MUGSHOT_AGATHA;
    Phase2Task_MugShotTransition(taskId);
}

static void Phase2Task_Drake(u8 taskId)
{
    gTasks[taskId].tMugshotId = MUGSHOT_LANCE;
    Phase2Task_MugShotTransition(taskId);
}

static void Phase2Task_Champion(u8 taskId)
{
    gTasks[taskId].tMugshotId = MUGSHOT_CHAMPION;
    Phase2Task_MugShotTransition(taskId);
}

static void Phase2Task_MugShotTransition(u8 taskId)
{
    while (sPhase2_Mugshot_Funcs[gTasks[taskId].tState](&gTasks[taskId]));
}

static bool8 Phase2_Mugshot_Func1(struct Task *task)
{
    u32 i;

    InitTransitionStructVars();
    ScanlineEffect_Clear();
    Mugshots_CreateOpponentPlayerSprites(task);
    task->tData1 = 0;
    task->tData2 = 1;
    task->tData3 = 239;
    sTransitionStructPtr->WININ = WININ_WIN0_ALL;
    sTransitionStructPtr->WINOUT = WINOUT_WIN01_BG1 | WINOUT_WIN01_BG2 | WINOUT_WIN01_BG3 | WINOUT_WIN01_OBJ | WINOUT_WIN01_CLR;
    sTransitionStructPtr->WIN0V = WIN_RANGE(0, DISPLAY_HEIGHT);

    for (i = 0; i < 160; i++)
        gScanlineEffectRegBuffers[1][i] = WIN_RANGE(0xF0, 0xF1);

    SetVBlankCallback(VBlankCB0_Phase2_Mugshots);

    task->tState++;
    return FALSE;
}

static bool8 Phase2_Mugshot_Func2(struct Task *task)
{
    s32 i, j;
    u16 *tilemap, *tileset;
    const u16 *mugshotsMap;

    mugshotsMap = sMugshotsTilemap;
    GetBg0TilesDst(&tilemap, &tileset);
    CpuSet(sEliteFour_Tileset, tileset, 0xF0);
    LoadPalette(sOpponentMugshotsPals[task->tMugshotId], 0xF0, 0x20);
    LoadPalette(sPlayerMugshotsPals[gSaveBlock2Ptr->playerGender], 0xFA, 0xC);

    for (i = 0; i < 20; i++)
        for (j = 0; j < 32; j++, mugshotsMap++)
            SOME_VRAM_STORE(tilemap, i, j, *mugshotsMap | 0xF000);

    EnableInterrupts(INTR_FLAG_HBLANK);

    SetHBlankCallback(HBlankCB_Phase2_Mugshots);
    task->tState++;
    return FALSE;
}

static bool8 Phase2_Mugshot_Func3(struct Task *task)
{
    u32 i, r5;
    u16* toStore;
    s16 value;
    s32 mergedValue;

    sTransitionStructPtr->VBlank_DMA = FALSE;

    toStore = gScanlineEffectRegBuffers[0];
    r5 = task->tData1;
    task->tData1 += 0x10;

    for (i = 0; i < 80; i++, toStore++, r5 += 0x10)
    {
        value = task->tData2 + Sin(r5, 0x10);
        if (value < 0)
            value = 1;
        if (value > 0xF0)
            value = 0xF0;
        *toStore = value;
    }
    for (; i < 160; i++, toStore++, r5 += 0x10)
    {
        value = task->tData3 - Sin(r5, 0x10);
        if (value < 0)
            value = 0;
        if (value > 0xEF)
            value = 0xEF;
        *toStore = (value << 8) | (0xF0);
    }

    task->tData2 += 8;
    task->tData3 -= 8;
    if (task->tData2 > 0xF0)
        task->tData2 = 0xF0;
    if (task->tData3 < 0)
        task->tData3 = 0;
    mergedValue = *(s32*)(&task->tData2);
    if (mergedValue == 0xF0)
        task->tState++;

    sTransitionStructPtr->BG0HOFS_1 -= 8;
    sTransitionStructPtr->BG0HOFS_2 += 8;
    sTransitionStructPtr->VBlank_DMA++;
    return FALSE;
}

static bool8 Phase2_Mugshot_Func4(struct Task *task)
{
    u32 i;
    u16* toStore;

    sTransitionStructPtr->VBlank_DMA = FALSE;

    for (i = 0, toStore = gScanlineEffectRegBuffers[0]; i < 160; i++, toStore++)
        *toStore = 0xF0;

    task->tState++;
    task->tData1 = 0;
    task->tData2 = 0;
    task->tData3 = 0;
    sTransitionStructPtr->BG0HOFS_1 -= 8;
    sTransitionStructPtr->BG0HOFS_2 += 8;

    SetTrainerPicSlideTable(task->tOpponentSpriteId, 0);
    SetTrainerPicSlideTable(task->tPlayerSpriteId, 1);
    IncrementTrainerPicState(task->tOpponentSpriteId);

    PlaySE(SE_MUGSHOT);

    sTransitionStructPtr->VBlank_DMA++;
    return FALSE;
}

static bool8 Phase2_Mugshot_Func5(struct Task *task)
{
    sTransitionStructPtr->BG0HOFS_1 -= 8;
    sTransitionStructPtr->BG0HOFS_2 += 8;
    if (IsTrainerPicSlideDone(task->tOpponentSpriteId))
    {
        task->tState++;
        IncrementTrainerPicState(task->tPlayerSpriteId);
    }
    return FALSE;
}

static bool8 Phase2_Mugshot_Func6(struct Task *task)
{
    sTransitionStructPtr->BG0HOFS_1 -= 8;
    sTransitionStructPtr->BG0HOFS_2 += 8;
    if (IsTrainerPicSlideDone(task->tPlayerSpriteId))
    {
        sTransitionStructPtr->VBlank_DMA = FALSE;
        SetVBlankCallback(NULL);
        DmaStop(0);
        memset(gScanlineEffectRegBuffers[0], 0, 0x140);
        memset(gScanlineEffectRegBuffers[1], 0, 0x140);
        SetGpuReg(REG_OFFSET_WIN0H, DISPLAY_WIDTH);
        SetGpuReg(REG_OFFSET_BLDY, 0);
        task->tState++;
        task->tData3 = 0;
        task->tData4 = 0;
        sTransitionStructPtr->BLDCNT = BLDCNT_TGT1_ALL | BLDCNT_EFFECT_LIGHTEN;
        SetVBlankCallback(VBlankCB1_Phase2_Mugshots);
    }
    return FALSE;
}

static bool8 Phase2_Mugshot_Func7(struct Task *task)
{
    bool32 r6;

    sTransitionStructPtr->VBlank_DMA = FALSE;
    r6 = TRUE;
    sTransitionStructPtr->BG0HOFS_1 -= 8;
    sTransitionStructPtr->BG0HOFS_2 += 8;

    if (task->tData4 < 0x50)
        task->tData4 += 2;
    if (task->tData4 > 0x50)
        task->tData4 = 0x50;

    if (++task->tData3 & 1)
    {
        s32 i;
        for (i = 0, r6 = FALSE; i <= task->tData4; i++)
        {
            s16 index1 = 0x50 - i;
            s16 index2 = 0x50 + i;
            if (gScanlineEffectRegBuffers[0][index1] <= 15)
            {
                r6 = TRUE;
                gScanlineEffectRegBuffers[0][index1]++;
            }
            if (gScanlineEffectRegBuffers[0][index2] <= 15)
            {
                r6 = TRUE;
                gScanlineEffectRegBuffers[0][index2]++;
            }
        }
    }

    if (task->tData4 == 0x50 && !r6)
        task->tState++;

    sTransitionStructPtr->VBlank_DMA++;
    return FALSE;
}

static bool8 Phase2_Mugshot_Func8(struct Task *task)
{
    sTransitionStructPtr->VBlank_DMA = FALSE;
    BlendPalettes(PALETTES_ALL, 0x10, RGB_WHITE);
    sTransitionStructPtr->BLDCNT = 0xFF;
    task->tData3 = 0;

    task->tState++;
    return TRUE;
}

static bool8 Phase2_Mugshot_Func9(struct Task *task)
{
    sTransitionStructPtr->VBlank_DMA = FALSE;

    task->tData3++;
    memset(gScanlineEffectRegBuffers[0], task->tData3, 0x140);
    if (task->tData3 > 15)
        task->tState++;

    sTransitionStructPtr->VBlank_DMA++;
    return FALSE;
}

static bool8 Phase2_Mugshot_Func10(struct Task *task)
{
    DmaStop(0);
    FadeScreenBlack();
    DestroyTask(FindTaskIdByFunc(task->func));
    return FALSE;
}

static void VBlankCB0_Phase2_Mugshots(void)
{
    DmaStop(0);
    VBlankCB_BattleTransition();
    if (sTransitionStructPtr->VBlank_DMA != 0)
        DmaCopy16(3, gScanlineEffectRegBuffers[0], gScanlineEffectRegBuffers[1], 320);
    REG_BG0VOFS = sTransitionStructPtr->BG0VOFS;
    REG_WININ = sTransitionStructPtr->WININ;
    REG_WINOUT = sTransitionStructPtr->WINOUT;
    REG_WIN0V = sTransitionStructPtr->WIN0V;
    DmaSet(0, gScanlineEffectRegBuffers[1], &REG_WIN0H, ((DMA_ENABLE | DMA_START_HBLANK | DMA_REPEAT | DMA_SRC_INC | DMA_DEST_FIXED) << 16) | 1);
}

static void VBlankCB1_Phase2_Mugshots(void)
{
    DmaStop(0);
    VBlankCB_BattleTransition();
    if (sTransitionStructPtr->VBlank_DMA != 0)
        DmaCopy16(3, gScanlineEffectRegBuffers[0], gScanlineEffectRegBuffers[1], 320);
    REG_BLDCNT = sTransitionStructPtr->BLDCNT;
    DmaSet(0, gScanlineEffectRegBuffers[1], &REG_BLDY, ((DMA_ENABLE | DMA_START_HBLANK | DMA_REPEAT | DMA_SRC_INC | DMA_DEST_FIXED) << 16) | 1);
}

static void HBlankCB_Phase2_Mugshots(void)
{
    if (REG_VCOUNT < 80)
        REG_BG0HOFS = sTransitionStructPtr->BG0HOFS_1;
    else
        REG_BG0HOFS = sTransitionStructPtr->BG0HOFS_2;
}

// data fields for player/opponent sprites in mugshots
#define sState          data[0]
#define sOffsetX        data[1]
#define sOffsetX2       data[2]
#define sDone           data[6]
#define sSlideTableId   data[7]

static void Mugshots_CreateOpponentPlayerSprites(struct Task *task)
{
    struct Sprite *opponentSprite, *playerSprite;

    s16 mugshotId = task->tMugshotId;
    task->tOpponentSpriteId = CreateTrainerSprite(sMugshotsTrainerPicIDsTable[mugshotId],
                                                     sMugshotsOpponentCoords[mugshotId][0] - 32,
                                                     sMugshotsOpponentCoords[mugshotId][1] + 42,
                                                     0, gDecompressionBuffer);
    task->tPlayerSpriteId = CreateTrainerSprite(PlayerGenderToFrontTrainerPicId(gSaveBlock2Ptr->playerGender, gSaveBlock2Ptr->playerOutfit), DISPLAY_WIDTH + 32, 106, 0, gDecompressionBuffer);

    opponentSprite = &gSprites[task->tOpponentSpriteId];
    playerSprite = &gSprites[task->tPlayerSpriteId];

    opponentSprite->callback = SpriteCb_TrainerPic;
    playerSprite->callback = SpriteCb_TrainerPic;

    opponentSprite->oam.affineMode = ST_OAM_AFFINE_DOUBLE;
    playerSprite->oam.affineMode = ST_OAM_AFFINE_DOUBLE;

    opponentSprite->oam.matrixNum = AllocOamMatrix();
    playerSprite->oam.matrixNum = AllocOamMatrix();

    opponentSprite->oam.shape = SPRITE_SHAPE(64x32);
    playerSprite->oam.shape = SPRITE_SHAPE(64x32);

    opponentSprite->oam.size = SPRITE_SIZE(64x32);
    playerSprite->oam.size = SPRITE_SIZE(64x32);

    CalcCenterToCornerVec(opponentSprite, SPRITE_SHAPE(64x32), SPRITE_SIZE(64x32), ST_OAM_AFFINE_DOUBLE);
    CalcCenterToCornerVec(playerSprite, SPRITE_SHAPE(64x32), SPRITE_SIZE(64x32), ST_OAM_AFFINE_DOUBLE);

    SetOamMatrixRotationScaling(opponentSprite->oam.matrixNum, 0x200, 0x200, 0);
    SetOamMatrixRotationScaling(playerSprite->oam.matrixNum, -512, 512, 0);
}

static void SpriteCb_TrainerPic(struct Sprite *sprite)
{
    while (sTrainerPicSpriteCbs[sprite->sState](sprite));
}

static bool8 TrainerPicCb_Nothing(struct Sprite *sprite)
{
    return FALSE;
}

static bool8 TrainerPicCb_SetSlideOffsets(struct Sprite *sprite)
{
    s16 offfsets1[ARRAY_COUNT(sTrainerPicSlideOffsets1)];
    s16 offfsets2[ARRAY_COUNT(sTrainerPicSlideOffsets2)];

    memcpy(offfsets1, sTrainerPicSlideOffsets1, sizeof(sTrainerPicSlideOffsets1));
    memcpy(offfsets2, sTrainerPicSlideOffsets2, sizeof(sTrainerPicSlideOffsets2));

    sprite->sState++;
    sprite->sOffsetX = offfsets1[sprite->sSlideTableId];
    sprite->sOffsetX2 = offfsets2[sprite->sSlideTableId];
    return TRUE;
}

// fast slide to around middle screen
static bool8 TrainerPicCb_Slide1(struct Sprite *sprite)
{
    sprite->x += sprite->sOffsetX;
    if (sprite->sSlideTableId && sprite->x < 133)
        sprite->sState++;
    else if (!sprite->sSlideTableId && sprite->x > 103)
        sprite->sState++;
    return FALSE;
}

// slower but accelerating slide
static bool8 TrainerPicCb_Slide2(struct Sprite *sprite)
{
    sprite->sOffsetX += sprite->sOffsetX2;
    sprite->x += sprite->sOffsetX;
    if (!sprite->sOffsetX)
    {
        sprite->sState++;
        sprite->sOffsetX2 = -sprite->sOffsetX2;
        sprite->sDone = TRUE;
    }
    return FALSE;
}

static void SetTrainerPicSlideTable(s16 spriteId, s16 arrId)
{
    gSprites[spriteId].sSlideTableId = arrId;
}

static void IncrementTrainerPicState(s16 spriteId)
{
    gSprites[spriteId].sState++;
}

static s16 IsTrainerPicSlideDone(s16 spriteId)
{
    return gSprites[spriteId].sDone;
}

#undef sState
#undef sOffsetX
#undef sOffsetX2
#undef sDone
#undef sSlideTableId

static void Phase2Task_Slice(u8 taskId)
{
    while (sPhase2_Slice_Funcs[gTasks[taskId].tState](&gTasks[taskId]));
}

static bool8 Phase2_Slice_Func1(struct Task *task)
{
    u32 i;

    InitTransitionStructVars();
    ScanlineEffect_Clear();
    task->tData2 = 256;
    task->tData3 = 1;
    sTransitionStructPtr->WININ = WININ_WIN0_ALL;
    sTransitionStructPtr->WINOUT = 0;
    sTransitionStructPtr->WIN0V = WIN_RANGE(0, DISPLAY_HEIGHT);
    sTransitionStructPtr->VBlank_DMA = FALSE;

    for (i = 0; i < 160; i++)
    {
        gScanlineEffectRegBuffers[1][i] = sTransitionStructPtr->field_14;
        gScanlineEffectRegBuffers[1][160 + i] = 0xF0;
    }

    EnableInterrupts(INTR_FLAG_HBLANK);
    SetGpuRegBits(REG_OFFSET_DISPSTAT, DISPSTAT_HBLANK_INTR);

    SetVBlankCallback(VBlankCB_Phase2_Slice);
    SetHBlankCallback(HBlankCB_Phase2_Slice);

    task->tState++;
    return TRUE;
}

static bool8 Phase2_Slice_Func2(struct Task *task)
{
    u32 i;

    sTransitionStructPtr->VBlank_DMA = FALSE;

    task->tData1 += (task->tData2 >> 8);
    if (task->tData1 > 0xF0)
        task->tData1 = 0xF0;
    if (task->tData2 <= 0xFFF)
        task->tData2 += task->tData3;
    if (task->tData3 < 128)
        task->tData3 <<= 1; // multiplying by two

    for (i = 0; i < 160; i++)
    {
        u16 *storeLoc1 = &gScanlineEffectRegBuffers[0][i];
        u16 *storeLoc2 = &gScanlineEffectRegBuffers[0][i + 160];
        if (i & 1)
        {
            *storeLoc1 = sTransitionStructPtr->field_14 + task->tData1;
            *storeLoc2 = 0xF0 - task->tData1;
        }
        else
        {
            *storeLoc1 = sTransitionStructPtr->field_14 - task->tData1;
            *storeLoc2 = WIN_RANGE(task->tData1, 0xF1);
        }
    }

    if (task->tData1 > 0xEF)
        task->tState++;

    sTransitionStructPtr->VBlank_DMA++;
    return FALSE;
}

static bool8 Phase2_Slice_Func3(struct Task *task)
{
    DmaStop(0);
    FadeScreenBlack();
    DestroyTask(FindTaskIdByFunc(Phase2Task_Slice));
    return FALSE;
}

static void VBlankCB_Phase2_Slice(void)
{
    DmaStop(0);
    VBlankCB_BattleTransition();
    REG_WININ = sTransitionStructPtr->WININ;
    REG_WINOUT = sTransitionStructPtr->WINOUT;
    REG_WIN0V = sTransitionStructPtr->WIN0V;
    if (sTransitionStructPtr->VBlank_DMA)
        DmaCopy16(3, gScanlineEffectRegBuffers[0], gScanlineEffectRegBuffers[1], 640);
    DmaSet(0, &gScanlineEffectRegBuffers[1][160], &REG_WIN0H, ((DMA_ENABLE | DMA_START_HBLANK | DMA_REPEAT | DMA_16BIT | DMA_SRC_INC | DMA_DEST_FIXED) << 16) | 1);
}

static void HBlankCB_Phase2_Slice(void)
{
    if (REG_VCOUNT < 160)
    {
        s16 var = gScanlineEffectRegBuffers[1][REG_VCOUNT];
        REG_BG1HOFS = var;
        REG_BG2HOFS = var;
        REG_BG3HOFS = var;
    }
}

static void Phase2Task_BlackHole(u8 taskId)
{
    while (sPhase2_BlackHole_Funcs[gTasks[taskId].tState](&gTasks[taskId]));
}

static bool8 Phase2_BlackHole_Func1(struct Task *task)
{
    s32 i;

    InitTransitionStructVars();
    ScanlineEffect_Clear();

    sTransitionStructPtr->WININ = 0;
    sTransitionStructPtr->WINOUT = WINOUT_WIN01_ALL;
    sTransitionStructPtr->WIN0H = DISPLAY_WIDTH;
    sTransitionStructPtr->WIN0V = DISPLAY_HEIGHT;

    for (i = 0; i < 0xA0; i++)
        gScanlineEffectRegBuffers[1][i] = 0;

    SetVBlankCallback(VBlankCB1_Phase2_BigPokeball);

    task->tState++;
    task->tData1 = 1;
    task->tData2 = 0x100;
    task->tFuncState = 0;

    return FALSE;
}

static bool8 Phase2_BlackHole_Func2(struct Task *task)
{
    sTransitionStructPtr->VBlank_DMA = FALSE;
    if (!task->tFuncState)
    {
        task->tFuncState++;
        task->tData1 = 0x30;
        task->tData6 = 0;
    }
    task->tData1 += gUnknown_085C8C80[task->tData6];
    task->tData6 = (task->tData6 + 1) % 2;
    sub_814A014(gScanlineEffectRegBuffers[0], 0x78, 0x50, task->tData1);
    if (task->tData1 < 9)
    {
        task->tState++;
        task->tFuncState = 0;
    }

    sTransitionStructPtr->VBlank_DMA++;
    return FALSE;
}

static bool8 Phase2_BlackHole_Func3(struct Task *task)
{
    if (task->tFuncState == 1)
    {
        DmaStop(0);
        SetVBlankCallback(NULL);
        DestroyTask(FindTaskIdByFunc(task->func));
    }
    else
    {
        sTransitionStructPtr->VBlank_DMA = FALSE;
        if (task->tData2 < 0x400)
            task->tData2 += 0x80;
        if (task->tData1 < 0xA0)
            task->tData1 += (task->tData2 >> 8);
        if (task->tData1 > 0xA0)
            task->tData1 = 0xA0;
        sub_814A014(gScanlineEffectRegBuffers[0], 0x78, 0x50, task->tData1);
        if (task->tData1 == 0xA0)
        {
            task->tFuncState = 1;
            FadeScreenBlack();
        }
        else
            sTransitionStructPtr->VBlank_DMA++;
    }
    return FALSE;
}

static void Phase2Task_RectangularSpiral(u8 taskId)
{
    while (sPhase2_RectangularSpiral_Funcs[gTasks[taskId].tState](&gTasks[taskId]));
}

static bool8 Phase2_RectangularSpiral_Func1(struct Task *task)
{
    u16 *tilemap, *tileset;

    GetBg0TilesDst(&tilemap, &tileset);
    CpuCopy16(sShrinkingBoxTileset, tileset, 0x20);
    CpuCopy16(sShrinkingBoxTileset + 0x70, tileset + 0x20, 0x20);
    CpuFill16(0xF000, tilemap, 0x800);
    LoadPalette(sFieldEffectPal_Pokeball, 0xF0, 0x20);

    task->tData3 = 1;
    task->tState++;

    sRectangularSpiralTransition[0].field_0 = 0;
    sRectangularSpiralTransition[0].field_2 = -1;
    sRectangularSpiralTransition[0].field_4 = 1;
    sRectangularSpiralTransition[0].field_6 = 308;
    sRectangularSpiralTransition[0].field_8 = 0;

    sRectangularSpiralTransition[1].field_0 = 0;
    sRectangularSpiralTransition[1].field_2 = -1;
    sRectangularSpiralTransition[1].field_4 = 1;
    sRectangularSpiralTransition[1].field_6 = 308;
    sRectangularSpiralTransition[1].field_8 = 0;

    sRectangularSpiralTransition[2].field_0 = 0;
    sRectangularSpiralTransition[2].field_2 = -3;
    sRectangularSpiralTransition[2].field_4 = 1;
    sRectangularSpiralTransition[2].field_6 = 307;
    sRectangularSpiralTransition[2].field_8 = 0;

    sRectangularSpiralTransition[3].field_0 = 0;
    sRectangularSpiralTransition[3].field_2 = -3;
    sRectangularSpiralTransition[3].field_4 = 1;
    sRectangularSpiralTransition[3].field_6 = 307;
    sRectangularSpiralTransition[3].field_8 = 0;
    return FALSE;
}

static bool8 Phase2_RectangularSpiral_Func2(struct Task *task)
{
    u16 *tilemap, *tileset;
    u32 i, j;
    bool32 done = TRUE;

    GetBg0TilesDst(&tilemap, &tileset);

    for (i = 0; i < 2; i++)
    {
        for (j = 0; j < ARRAY_COUNT(sRectangularSpiralTransition); j++)
        {
            s16 var = 0, var2 = 0;
            s32 var3 = 0;

            if (sub_8149048(gUnknown_085C8D38[j / 2], &sRectangularSpiralTransition[j]))
            {
                done = FALSE;
                var = sRectangularSpiralTransition[j].field_2;
                if ((j % 2) == 1)
                    var = 0x27D - var;

                var2 = var % 32;
                var3 = var / 32;

                SOME_VRAM_STORE(tilemap, var3, var2, 0xF002);
            }
        }
    }

    if (done)
        task->tState++;
    return FALSE;
}

static bool8 Phase2_RectangularSpiral_Func3(struct Task *task)
{
    DmaStop(0);
    FadeScreenBlack();
    DestroyTask(FindTaskIdByFunc(task->func));
    return FALSE;
}

static bool16 sub_8149048(const s16 * const *arg0, struct StructRectangularSpiral *arg1)
{
    const s16 *array = arg0[arg1->field_0];
    if (array[arg1->field_4] == -1)
        return FALSE;

    switch (array[0])
    {
    case 1:
        arg1->field_2 += 0x1;
        break;
    case 2:
        arg1->field_2 -= 0x1;
        break;
    case 3:
        arg1->field_2 -= 0x20;
        break;
    case 4:
        arg1->field_2 += 0x20;
        break;
    }

    if (arg1->field_2 > 0x27F || array[arg1->field_4] == -1)
        return FALSE;

    if (!arg1->field_8 && array[arg1->field_4] == -2)
    {
        arg1->field_8 = 1;
        arg1->field_4 = 1;
        arg1->field_2 = arg1->field_6;
        arg1->field_0 = 4;
    }

    if (arg1->field_2 == array[arg1->field_4])
    {
        (arg1->field_0)++;
        if (arg1->field_8 == 1)
        {
            if (arg1->field_0 > 7)
            {
                (arg1->field_4)++;
                (arg1->field_0) = 4;
            }
        }
        else
        {
            if (arg1->field_0 > 3)
            {
                (arg1->field_4)++;
                (arg1->field_0) = 0;
            }
        }
    }

    return TRUE;
}

static void Phase2Task_WhiteFade(u8 taskId)
{
    while (sPhase2_WhiteFade_Funcs[gTasks[taskId].tState](&gTasks[taskId]));
}

static bool8 Phase2_WhiteFade_Func1(struct Task *task)
{
    u32 i;

    InitTransitionStructVars();
    ScanlineEffect_Clear();

    sTransitionStructPtr->BLDCNT = BLDCNT_TGT1_ALL | BLDCNT_EFFECT_LIGHTEN;
    sTransitionStructPtr->BLDY = 0;
    sTransitionStructPtr->WININ = WININ_WIN0_BG1 | WININ_WIN0_BG2 | WININ_WIN0_BG3 | WININ_WIN0_OBJ;
    sTransitionStructPtr->WINOUT = WINOUT_WIN01_ALL;
    sTransitionStructPtr->WIN0V = WIN_RANGE(0, DISPLAY_HEIGHT);

    for (i = 0; i < 160; i++)
    {
        gScanlineEffectRegBuffers[1][i] = 0;
        gScanlineEffectRegBuffers[1][i + 160] = 0xF0;
    }

    EnableInterrupts(INTR_FLAG_HBLANK);
    SetHBlankCallback(HBlankCB_Phase2_WhiteFade);
    SetVBlankCallback(VBlankCB0_Phase2_WhiteFade);

    task->tState++;
    return FALSE;
}

static bool8 Phase2_WhiteFade_Func2(struct Task *task)
{
    s32 i, posY;
    s16 arr1[ARRAY_COUNT(sUnknown_085C8DA0)];
    struct Sprite *sprite;

    memcpy(arr1, sUnknown_085C8DA0, sizeof(sUnknown_085C8DA0));
    for (i = 0, posY = 0; i <= ARRAY_COUNT(sUnknown_085C8DA0); i++, posY += 0x1B)
    {
        sprite = &gSprites[CreateInvisibleSprite(sub_8149864)];
        sprite->x = 0xF0;
        sprite->y = posY;
        sprite->data[5] = arr1[i];
    }
    sprite->data[6]++;

    task->tState++;
    return FALSE;
}

static bool8 Phase2_WhiteFade_Func3(struct Task *task)
{
    sTransitionStructPtr->VBlank_DMA = 0;
    if (sTransitionStructPtr->field_20 >= ARRAY_COUNT(sUnknown_085C8DA0))
    {
        BlendPalettes(PALETTES_ALL, 0x10, RGB_WHITE);
        task->tState++;
    }
    return FALSE;
}

static bool8 Phase2_WhiteFade_Func4(struct Task *task)
{
    sTransitionStructPtr->VBlank_DMA = FALSE;

    DmaStop(0);
    SetVBlankCallback(0);
    SetHBlankCallback(0);

    sTransitionStructPtr->WIN0H = DISPLAY_WIDTH;
    sTransitionStructPtr->BLDY = 0;
    sTransitionStructPtr->BLDCNT = BLDCNT_TGT1_ALL | BLDCNT_EFFECT_DARKEN;
    sTransitionStructPtr->WININ = WININ_WIN0_ALL;
    sTransitionStructPtr->field_20 = 0;

    SetVBlankCallback(VBlankCB1_Phase2_WhiteFade);

    task->tState++;
    return FALSE;
}

static bool8 Phase2_WhiteFade_Func5(struct Task *task)
{
   sTransitionStructPtr->field_20 += 480;
   sTransitionStructPtr->BLDY = sTransitionStructPtr->field_20 >> 8;
   if (++sTransitionStructPtr->BLDY > 16)
   {
       FadeScreenBlack();
       DestroyTask(FindTaskIdByFunc(Phase2Task_WhiteFade));
   }
   return FALSE;
}

static void VBlankCB0_Phase2_WhiteFade(void)
{
    DmaStop(0);
    VBlankCB_BattleTransition();
    REG_BLDCNT = sTransitionStructPtr->BLDCNT;
    REG_WININ = sTransitionStructPtr->WININ;
    REG_WINOUT = sTransitionStructPtr->WINOUT;
    REG_WIN0V = sTransitionStructPtr->WIN0V;
    if (sTransitionStructPtr->VBlank_DMA)
        DmaCopy16(3, gScanlineEffectRegBuffers[0], gScanlineEffectRegBuffers[1], 640);
    DmaSet(0, &gScanlineEffectRegBuffers[1][160], &REG_WIN0H, ((DMA_ENABLE | DMA_START_HBLANK | DMA_REPEAT | DMA_16BIT | DMA_SRC_INC | DMA_DEST_FIXED) << 16) | 1);
}

static void VBlankCB1_Phase2_WhiteFade(void)
{
    VBlankCB_BattleTransition();
    REG_BLDY = sTransitionStructPtr->BLDY;
    REG_BLDCNT = sTransitionStructPtr->BLDCNT;
    REG_WININ = sTransitionStructPtr->WININ;
    REG_WINOUT = sTransitionStructPtr->WINOUT;
    REG_WIN0H = sTransitionStructPtr->WIN0H;
    REG_WIN0V = sTransitionStructPtr->WIN0V;
}

static void HBlankCB_Phase2_WhiteFade(void)
{
    REG_BLDY = gScanlineEffectRegBuffers[1][REG_VCOUNT];
}

static void sub_8149864(struct Sprite *sprite)
{
    if (sprite->data[5])
    {
        sprite->data[5]--;
        if (sprite->data[6])
            sTransitionStructPtr->VBlank_DMA = TRUE;
    }
    else
    {
        u32 i;
        u16* bldY = &gScanlineEffectRegBuffers[0][sprite->y];
        u16* win0H = &gScanlineEffectRegBuffers[0][sprite->y + 160];
        u32 stripeWidth = sprite->data[6] ? 0x19 : 0x1B;

        for (i = 0; i < stripeWidth; i++)
        {
            bldY[i] = sprite->data[0] >> 8;
            win0H[i] = (u8)(sprite->x);
        }
        if (!sprite->x && sprite->data[0] == 0x1000)
            sprite->data[1] = 1;
        sprite->x -= 24;
        sprite->data[0] += 192;

        if (sprite->x < 0)
            sprite->x = 0;
        if (sprite->data[0] > 0x1000)
            sprite->data[0] = 0x1000;

        if (sprite->data[6])
            sTransitionStructPtr->VBlank_DMA = TRUE;

        if (sprite->data[1])
        {
            if (!sprite->data[6] || sTransitionStructPtr->field_20 > 4)
            {
                sTransitionStructPtr->field_20++;
                DestroySprite(sprite);
            }
        }
    }
}

static void Phase2Task_GridSquares(u8 taskId)
{
    while (sPhase2_GridSquares_Funcs[gTasks[taskId].tState](&gTasks[taskId]));
}

static bool8 Phase2_GridSquares_Func1(struct Task *task)
{
    u16 *tilemap, *tileset;

    GetBg0TilesDst(&tilemap, &tileset);
    CpuSet(sShrinkingBoxTileset, tileset, 0x20);
    CpuFill16(0xF000, tilemap, 0x800);
    LoadPalette(sFieldEffectPal_Pokeball, 0xF0, 0x20);

    task->tState++;
    return FALSE;
}

static bool8 Phase2_GridSquares_Func2(struct Task *task)
{
    u16* tileset;

    if (!task->tData1)
    {
        GetBg0TilemapDst(&tileset);
        task->tData1 = 3;
        task->tData2++;
        CpuSet(sShrinkingBoxTileset + (task->tData2 * 8), tileset, 0x20);
        if (task->tData2 > 0xD)
        {
            task->tState++;
            task->tData1 = 16;
        }
    }

    task->tData1--;
    return FALSE;
}

static bool8 Phase2_GridSquares_Func3(struct Task *task)
{
    if (--task->tData1 == 0)
    {
        FadeScreenBlack();
        DestroyTask(FindTaskIdByFunc(Phase2Task_GridSquares));
    }
    return FALSE;
}

static void Phase2Task_Shards(u8 taskId)
{
    while (sPhase2_Shards_Funcs[gTasks[taskId].tState](&gTasks[taskId]));
}

static bool8 Phase2_Shards_Func1(struct Task *task)
{
    u32 i;

    InitTransitionStructVars();
    ScanlineEffect_Clear();

    sTransitionStructPtr->WININ = WININ_WIN0_ALL;
    sTransitionStructPtr->WINOUT = 0;
    sTransitionStructPtr->WIN0V = WIN_RANGE(0, DISPLAY_HEIGHT);
    for (i = 0; i < 160; i++)
        gScanlineEffectRegBuffers[0][i] = WIN_RANGE(0, 0xF0);
    CpuSet(gScanlineEffectRegBuffers[0], gScanlineEffectRegBuffers[1], 0xA0);
    SetVBlankCallback(VBlankCB_Phase2_Shards);

    task->tState++;
    return TRUE;
}

static bool8 Phase2_Shards_Func2(struct Task *task)
{
    sub_814A1AC(sTransitionStructPtr->data,
                sUnknown_085C8DD0[task->tData1][0],
                sUnknown_085C8DD0[task->tData1][1],
                sUnknown_085C8DD0[task->tData1][2],
                sUnknown_085C8DD0[task->tData1][3],
                1, 1);
    task->tData2 = sUnknown_085C8DD0[task->tData1][4];
    task->tState++;
    return TRUE;
}

static bool8 Phase2_Shards_Func3(struct Task *task)
{
    s32 i;
    bool32 nextFunc;

    sTransitionStructPtr->VBlank_DMA = 0;

    for (i = 0, nextFunc = FALSE; i < 16; i++)
    {
        s16 left = gScanlineEffectRegBuffers[0][sTransitionStructPtr->data[3]] >> 8;
        s16 right = gScanlineEffectRegBuffers[0][sTransitionStructPtr->data[3]] & 0xFF;
        if (task->tData2)
        {
            if (right > sTransitionStructPtr->data[2])
                right = sTransitionStructPtr->data[2];
            if (right <= left)
                right = left;
        }
        else
        {
            if (left < sTransitionStructPtr->data[2])
                left = sTransitionStructPtr->data[2];
            if (left > right)
                left = right;
        }
        gScanlineEffectRegBuffers[0][sTransitionStructPtr->data[3]] = WIN_RANGE2(left, right);
        if (nextFunc)
        {
            task->tState++;
            break;
        }
        else
            nextFunc = sub_814A228(sTransitionStructPtr->data, TRUE, TRUE);
    }

    sTransitionStructPtr->VBlank_DMA++;
    return FALSE;
}

static bool8 Phase2_Shards_Func4(struct Task *task)
{
    if (++task->tData1 < 7)
    {
        task->tState++;
        task->tData3 = sUnknown_085C8E16[task->tData1 - 1];
        return TRUE;
    }
    else
    {
        DmaStop(0);
        FadeScreenBlack();
        DestroyTask(FindTaskIdByFunc(Phase2Task_Shards));
        return FALSE;
    }
}

static bool8 Phase2_Shards_Func5(struct Task *task)
{
    if (--task->tData3 == 0)
    {
        task->tState = 1;
        return TRUE;
    }
    return FALSE;
}

static void VBlankCB_Phase2_Shards(void)
{
    DmaStop(0);
    VBlankCB_BattleTransition();
    if (sTransitionStructPtr->VBlank_DMA)
        DmaCopy16(3, gScanlineEffectRegBuffers[0], gScanlineEffectRegBuffers[1], 320);
    REG_WININ = sTransitionStructPtr->WININ;
    REG_WINOUT = sTransitionStructPtr->WINOUT;
    REG_WIN0V = sTransitionStructPtr->WIN0V;
    REG_WIN0H = gScanlineEffectRegBuffers[1][0];
    DmaSet(0, gScanlineEffectRegBuffers[1], &REG_WIN0H, ((DMA_ENABLE | DMA_START_HBLANK | DMA_REPEAT | DMA_16BIT | DMA_SRC_INC | DMA_DEST_FIXED) << 16) | 1);
}

// sub-task for phase2
#undef tData1
#undef tData2
#undef tData3
#undef tData4
#undef tData5
#undef tData6
#undef tFuncState
#undef tFrames
#undef tOpponentSpriteId
#undef tPlayerSpriteId
#undef tMugshotId

// sub-task for sub-task phase
#define tData1      data[1]
#define tData2      data[2]
#define tData3      data[3]
#define tData4      data[4]
#define tData5      data[5]
#define tData6      data[6]
#define tData7      data[7]

static void CreatePhase1Task(s16 a0, s16 a1, s16 a2, s16 a3, s16 a4)
{
    u8 taskId = CreateTask(TransitionPhase1_Task_RunFuncs, 3);
    gTasks[taskId].tData1 = a0;
    gTasks[taskId].tData2 = a1;
    gTasks[taskId].tData3 = a2;
    gTasks[taskId].tData4 = a3;
    gTasks[taskId].tData5 = a4;
    gTasks[taskId].tData6 = a0;
}

static bool8 IsPhase1Done(void)
{
    if (FindTaskIdByFunc(TransitionPhase1_Task_RunFuncs) == TASK_NONE)
        return TRUE;
    return FALSE;
}

void TransitionPhase1_Task_RunFuncs(u8 taskId)
{
    while (sPhase1_TransitionAll_Funcs[gTasks[taskId].tState](&gTasks[taskId]));
}

static bool8 Phase1_TransitionAll_Func1(struct Task *task)
{
    if (!task->tData6 || --task->tData6 == 0)
    {
        task->tData6 = task->tData1;
        task->tData7 += task->tData4;
        if (task->tData7 > 16)
            task->tData7 = 16;
        BlendPalettes(PALETTES_ALL, task->tData7, 0x2D6B);
    }
    if (task->tData7 > 15)
    {
        task->tState++;
        task->tData6 = task->tData2;
    }
    return FALSE;
}

static bool8 Phase1_TransitionAll_Func2(struct Task *task)
{
    if (!task->tData6 || --task->tData6 == 0)
    {
        task->tData6 = task->tData2;
        task->tData7 -= task->tData5;
        if (task->tData7 < 0)
            task->tData7 = 0;
        BlendPalettes(PALETTES_ALL, task->tData7, 0x2D6B);
    }
    if (!task->tData7)
    {
        if (--task->tData3)
        {
            task->tData6 = task->tData1;
            task->tState = 0;
        }
        else
            DestroyTask(FindTaskIdByFunc(TransitionPhase1_Task_RunFuncs));
    }
    return FALSE;
}

#undef tData1
#undef tData2
#undef tData3
#undef tData4
#undef tData5
#undef tData6
#undef tData7

static void InitTransitionStructVars(void)
{
    memset(sTransitionStructPtr, 0, sizeof(*sTransitionStructPtr));
    GetCameraOffsetWithPan(&sTransitionStructPtr->field_14, &sTransitionStructPtr->field_16);
}

static void VBlankCB_BattleTransition(void)
{
    LoadOam();
    ProcessSpriteCopyRequests();
    TransferPlttBuffer();
}

static void GetBg0TilemapDst(u16 **tileset)
{
    u16 charBase = REG_BG0CNT >> 2;
    charBase <<= 0xE;
    *tileset = (u16*)(VRAM + charBase);
}

void GetBg0TilesDst(u16 **tilemap, u16 **tileset)
{
    u16 screenBase = REG_BG0CNT >> 8;
    u16 charBase = REG_BG0CNT >> 2;

    screenBase <<= 0xB;
    charBase <<= 0xE;

    *tilemap = (u16*)(VRAM + screenBase);
    *tileset = (u16*)(VRAM + charBase);
}

static void FadeScreenBlack(void)
{
    BlendPalettes(PALETTES_ALL, 16, RGB_BLACK);
}

static void sub_8149F98(s16 *array, s16 sinAdd, s16 index, s16 indexIncrementer, s16 amplitude, s16 arrSize)
{
    u32 i;

    for (i = 0; arrSize > 0; arrSize--, i++, index += indexIncrementer)
        array[i] = sinAdd + Sin(0xFF & index, amplitude);
}

static void sub_814A014(u16 *array, s16 a1, s16 a2, s16 a3)
{
    s32 i;

    memset(array, 0xA, 160 * sizeof(s16));
    for (i = 0; i < 64; i++)
    {
        s16 sinResult, cosResult;
        s16 toStoreOrr, r2, r3, toStore, r7, r8;

        sinResult = Sin(i, a3);
        cosResult = Cos(i, a3);

        toStoreOrr = a1 - sinResult;
        toStore = a1 + sinResult;
        r7 = a2 - cosResult;
        r8 = a2 + cosResult;

        if (toStoreOrr < 0)
            toStoreOrr = 0;
        if (toStore > 0xF0)
            toStore = 0xF0;
        if (r7 < 0)
            r7 = 0;
        if (r8 > 0x9F)
            r8 = 0x9F;

        toStore |= (toStoreOrr << 8);
        array[r7] = toStore;
        array[r8] = toStore;

        cosResult = Cos(i + 1, a3);
        r3 = a2 - cosResult;
        r2 = a2 + cosResult;

        if (r3 < 0)
            r3 = 0;
        if (r2 > 0x9F)
            r2 = 0x9F;

        while (r7 > r3)
            array[--r7] = toStore;
        while (r7 < r3)
            array[++r7] = toStore;

        while (r8 > r2)
            array[--r8] = toStore;
        while (r8 < r2)
            array[++r8] = toStore;
    }
}

static void sub_814A1AC(s16 *data, s16 a1, s16 a2, s16 a3, s16 a4, s16 a5, s16 a6)
{
    data[0] = a1;
    data[1] = a2;
    data[2] = a1;
    data[3] = a2;
    data[4] = a3;
    data[5] = a4;
    data[6] = a5;
    data[7] = a6;
    data[8] = a3 - a1;
    if (data[8] < 0)
    {
        data[8] = -data[8];
        data[6] = -a5;
    }
    data[9] = a4 - a2;
    if (data[9] < 0)
    {
        data[9] = -data[9];
        data[7] = -a6;
    }
    data[10] = 0;
}

static bool8 sub_814A228(s16 *data, bool8 a1, bool8 a2)
{
    u8 var;

    if (data[8] > data[9])
    {
        data[2] += data[6];
        data[10] += data[9];
        if (data[10] > data[8])
        {
            data[3] += data[7];
            data[10] -= data[8];
        }
    }
    else
    {
        data[3] += data[7];
        data[10] += data[8];
        if (data[10] > data[9])
        {
            data[2] += data[6];
            data[10] -= data[9];
        }
    }
    var = 0;
    if ((data[6] > 0 && data[2] >= data[4]) || (data[6] < 0 && data[2] <= data[4]))
    {
        var++;
        if (a1)
            data[2] = data[4];
    }
    if ((data[7] > 0 && data[3] >= data[5]) || (data[7] < 0 && data[3] <= data[5]))
    {
        var++;
        if (a2)
            data[3] = data[5];
    }

    if (var == 2)
        return TRUE;
    return FALSE;
}

// sub-task for phase2 of a couple of new transitions
#define tData1      data[1]
#define tData2      data[2]
#define tData3      data[3]
#define tData4      data[4]
#define tData5      data[5]
#define tData6      data[6]
#define tData7      data[7]

static bool8 Phase2_FrontierLogoWiggle_Func1(struct Task *task)
{
    u16 *tilemap, *tileset;

    sub_814669C(task);
    GetBg0TilesDst(&tilemap, &tileset);
    CpuFill16(0, tilemap, 0x800);
    LZ77UnCompVram(sFrontierLogo_Tileset, tileset);
    LoadPalette(sFrontierLogo_Palette, 0xF0, 0x20);

    task->tState++;
    return FALSE;
}

static bool8 Phase2_FrontierLogoWiggle_Func2(struct Task *task)
{
    u16 *tilemap, *tileset;

    GetBg0TilesDst(&tilemap, &tileset);
    LZ77UnCompVram(sFrontierLogo_Tilemap, tilemap);
    sub_8149F98(gScanlineEffectRegBuffers[0], 0, task->tData4, 0x84, task->tData5, 160);

    task->tState++;
    return TRUE;
}

static void Phase2Task_FrontierLogoWiggle(u8 taskId)
{
    while (sPhase2_FrontierLogoWiggle_Funcs[gTasks[taskId].tState](&gTasks[taskId]));
}

static void Phase2Task_FrontierLogoWave(u8 taskId)
{
    while (sPhase2_FrontierLogoWave_Funcs[gTasks[taskId].tState](&gTasks[taskId]));
}

static bool8 Phase2_FrontierLogoWave_Func1(struct Task *task)
{
    u16 *tilemap, *tileset;

    InitTransitionStructVars();
    ScanlineEffect_Clear();
    ClearGpuRegBits(REG_OFFSET_DISPCNT, DISPCNT_WIN0_ON | DISPCNT_WIN1_ON);
    task->tData2 = 0x2000;
    task->tData1 = 0x7FFF;
    task->tData5 = 0;
    task->tData6 = 16;
    task->tData7 = 2560;
    sTransitionStructPtr->BLDCNT = BLDCNT_TGT1_BG0 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL;
    sTransitionStructPtr->BLDALPHA = BLDALPHA_BLEND(task->tData5, task->tData6);
    REG_BLDCNT = sTransitionStructPtr->BLDCNT;
    REG_BLDALPHA = sTransitionStructPtr->BLDALPHA;
    GetBg0TilesDst(&tilemap, &tileset);
    CpuFill16(0, tilemap, 0x800);
    LZ77UnCompVram(sFrontierLogo_Tileset, tileset);
    LoadPalette(sFrontierLogo_Palette, 0xF0, 0x20);
    sTransitionStructPtr->field_16 = 0;

    task->tState++;
    return FALSE;
}

static bool8 Phase2_FrontierLogoWave_Func2(struct Task *task)
{
    u16 *tilemap, *tileset;

    GetBg0TilesDst(&tilemap, &tileset);
    LZ77UnCompVram(sFrontierLogo_Tilemap, tilemap);

    task->tState++;
    return TRUE;
}

static bool8 Phase2_FrontierLogoWave_Func3(struct Task *task)
{
    u32 i;

    for (i = 0; i < 160; i++)
        gScanlineEffectRegBuffers[1][i] = sTransitionStructPtr->field_16;

    SetVBlankCallback(VBlankCB_Phase2_30);
    SetHBlankCallback(HBlankCB_Phase2_30);
    EnableInterrupts(INTR_FLAG_HBLANK);

    task->tState++;
    return TRUE;
}

static bool8 Phase2_FrontierLogoWave_Func4(struct Task *task)
{
    u32 i;
    u16 var6, amplitude, var8;

    sTransitionStructPtr->VBlank_DMA = FALSE;

    amplitude = task->tData2 >> 8;
    var6 = task->tData1;
    var8 = 384;

    task->tData1 = var6 - task->tData7;

    if (task->tData3 >= 70)
    {
        if (task->tData2 - 384 >= 0)
            task->tData2 -= 384;
        else
            task->tData2 = 0;
    }

    if (task->tData3 >= 0 && !task->tData3 % 3)
    {
        if (task->tData5 < 16)
            task->tData5++;
        else if (task->tData6 > 0)
            task->tData6--;

        sTransitionStructPtr->BLDALPHA = BLDALPHA_BLEND(task->tData5, task->tData6);
    }

    for (i = 0; i < 160; i++, var6 += var8)
    {
        s16 index = var6 / 256;
        gScanlineEffectRegBuffers[0][i] = sTransitionStructPtr->field_16 + Sin(index & 0xff, amplitude);
    }

    if (++task->tData3 == 101)
    {
        task->tData4++;
        BeginNormalPaletteFade(PALETTES_ALL, 0, 0, 0x10, RGB_BLACK);
    }

    if (task->tData4 != 0 && !gPaletteFade.active)
        DestroyTask(FindTaskIdByFunc(Phase2Task_FrontierLogoWave));

    task->tData7 -= 17;
    sTransitionStructPtr->VBlank_DMA++;
    return FALSE;
}

static void VBlankCB_Phase2_30(void)
{
    VBlankCB_BattleTransition();
    REG_BLDCNT = sTransitionStructPtr->BLDCNT;
    REG_BLDALPHA = sTransitionStructPtr->BLDALPHA;

    if (sTransitionStructPtr->VBlank_DMA)
        DmaCopy16(3, gScanlineEffectRegBuffers[0], gScanlineEffectRegBuffers[1], 320);
}

static void HBlankCB_Phase2_30(void)
{
    u16 var = gScanlineEffectRegBuffers[1][REG_VCOUNT];

    REG_BG0VOFS = var;
}

static void Phase2Task_FrontierSquares(u8 taskId)
{
    while (sPhase2_FrontierSquares_Funcs[gTasks[taskId].tState](&gTasks[taskId]));
}

static void Phase2Task_FrontierSquaresSpiral(u8 taskId)
{
    while (sPhase2_FrontierSquaresSpiral_Funcs[gTasks[taskId].tState](&gTasks[taskId]));
}

static void Phase2Task_FrontierSquaresScroll(u8 taskId)
{
    while (sPhase2_FrontierSquaresScroll_Funcs[gTasks[taskId].tState](&gTasks[taskId]));
}

static bool8 Phase2_FrontierSquares_Func1(struct Task *task)
{
    u16 *tilemap, *tileset;

    GetBg0TilesDst(&tilemap, &tileset);
    LZ77UnCompVram(sFrontierSquares_FilledBg_Tileset, tileset);

    FillBgTilemapBufferRect_Palette0(0, 0, 0, 0, 0x20, 0x20);
    FillBgTilemapBufferRect(0, 1, 0, 0, 1, 0x20, 0xF);
    FillBgTilemapBufferRect(0, 1, 0x1D, 0, 1, 0x20, 0xF);
    CopyBgTilemapBufferToVram(0);
    LoadPalette(sFrontierSquares_Palette, 0xF0, 0x20);

    task->tData2 = 1;
    task->tData3 = 0;
    task->tData4 = 0;
    task->tData7 = 10;

    task->tState++;
    return FALSE;
}

static bool8 Phase2_FrontierSquares_Func2(struct Task *task)
{
    CopyRectToBgTilemapBufferRect(0, sFrontierSquares_Tilemap, 0, 0, 4, 4, task->tData2, task->tData3, 4, 4, 0xF, 0, 0);
    CopyBgTilemapBufferToVram(0);

    task->tData2 += 4;
    if (++task->tData4 == 7)
    {
        task->tData2 = 1;
        task->tData3 += 4;
        task->tData4 = 0;
        if (task->tData3 > 19)
            task->tState++;
    }
    return FALSE;
}

static bool8 Phase2_FrontierSquares_Func3(struct Task *task)
{
    u32 i;
    u16 *tilemap, *tileset;

    GetBg0TilesDst(&tilemap, &tileset);
    if (task->tData6++ >= task->tData7)
    {
        switch (task->tData5)
        {
        case 0:
            for (i = 250; i < 255; i++)
            {
                gPlttBufferUnfaded[i] = 0;
                gPlttBufferFaded[i] = 0;
            }
            break;
        case 1:
            BlendPalettes(PALETTES_ALL & ~(0x8000), 0x10, 0);
            LZ77UnCompVram(sFrontierSquares_EmptyBg_Tileset, tileset);
            break;
        case 2:
            LZ77UnCompVram(sFrontierSquares_Shrink1_Tileset, tileset);
            break;
        case 3:
            LZ77UnCompVram(sFrontierSquares_Shrink2_Tileset, tileset);
            break;
        default:
            FillBgTilemapBufferRect_Palette0(0, 1, 0, 0, 0x20, 0x20);
            CopyBgTilemapBufferToVram(0);
            task->tState++;
            return FALSE;
        }
        task->tData6 = 0;
        task->tData5++;
    }
    return FALSE;
}

static bool8 Phase2_FrontierSquaresSpiral_Func1(struct Task *task)
{
    u16 *tilemap, *tileset;

    GetBg0TilesDst(&tilemap, &tileset);
    LZ77UnCompVram(sFrontierSquares_FilledBg_Tileset, tileset);

    FillBgTilemapBufferRect_Palette0(0, 0, 0, 0, 0x20, 0x20);
    FillBgTilemapBufferRect(0, 1, 0, 0, 1, 0x20, 0xF);
    FillBgTilemapBufferRect(0, 1, 0x1D, 0, 1, 0x20, 0xF);
    CopyBgTilemapBufferToVram(0);
    LoadPalette(sFrontierSquares_Palette, 0xE0, 0x20);
    LoadPalette(sFrontierSquares_Palette, 0xF0, 0x20);
    BlendPalette(0xE0, 0x10, 8, 0);

    task->tData2 = 34;
    task->tData3 = 0;

    task->tState++;
    return FALSE;
}

static bool8 Phase2_FrontierSquaresSpiral_Func2(struct Task *task)
{
    u8 var = gUnknown_085C9A30[task->tData2];
    u8 varMod = var % 7;
    u8 varDiv = var / 7;
    CopyRectToBgTilemapBufferRect(0, &sFrontierSquares_Tilemap, 0, 0, 4, 4, 4 * varMod + 1, 4 * varDiv, 4, 4, 0xF, 0, 0);
    CopyBgTilemapBufferToVram(0);

    if (--task->tData2 < 0)
        task->tState++;
    return FALSE;
}

static bool8 Phase2_FrontierSquaresSpiral_Func3(struct Task *task)
{
    BlendPalette(0xE0, 0x10, 3, 0);
    BlendPalettes(PALETTES_ALL & ~(0x8000 | 0x4000), 0x10, 0);

    task->tData2 = 0;
    task->tData3 = 0;

    task->tState++;
    return FALSE;
}

static bool8 Phase2_FrontierSquaresSpiral_Func4(struct Task *task)
{
    if ((task->tData3 ^= 1))
    {
        CopyRectToBgTilemapBufferRect(
        0,
        sFrontierSquares_Tilemap,
        0,
        0,
        4,
        4,
        4 * (gUnknown_085C9A30[task->tData2] % 7) + 1,
        4 * (gUnknown_085C9A30[task->tData2] / 7),
        4,
        4,
        0xE,
        0,
        0);
    }
    else
    {
        if (task->tData2 > 0)
        {
            FillBgTilemapBufferRect(
            0,
            1,
            4 * (gUnknown_085C9A30[task->tData2 - 1] % 7) + 1,
            4 * (gUnknown_085C9A30[task->tData2 - 1] / 7),
            4,
            4,
            0xF);
        }

        task->tData2++;
    }

    if (task->tData2 > 34)
        task->tState++;

    CopyBgTilemapBufferToVram(0);
    return FALSE;
}

static bool8 Phase2_FrontierSquares_End(struct Task *task)
{
    FillBgTilemapBufferRect_Palette0(0, 1, 0, 0, 0x20, 0x20);
    CopyBgTilemapBufferToVram(0);
    BlendPalettes(PALETTES_ALL, 0x10, 0);
    DestroyTask(FindTaskIdByFunc(task->func));
    return FALSE;
}

// sub task for phase2 32
#define tSub32_X_delta  data[0]
#define tSub32_Y_delta  data[1]
#define tSub32_Bool     data[2]

static void sub_814ABE4(u8 taskId)
{
    if (!(gTasks[taskId].tSub32_Bool ^= 1))
    {
        SetGpuReg(REG_OFFSET_BG0VOFS, gBattle_BG0_X);
        SetGpuReg(REG_OFFSET_BG0HOFS, gBattle_BG0_Y);
        gBattle_BG0_X += gTasks[taskId].tSub32_X_delta;
        gBattle_BG0_Y += gTasks[taskId].tSub32_Y_delta;
    }
}

static bool8 Phase2_FrontierSquaresScroll_Func1(struct Task *task)
{
    u8 taskId = 0;
    u16 *tilemap, *tileset;

    GetBg0TilesDst(&tilemap, &tileset);
    LZ77UnCompVram(sFrontierSquares_FilledBg_Tileset, tileset);
    FillBgTilemapBufferRect_Palette0(0, 0, 0, 0, 0x20, 0x20);
    CopyBgTilemapBufferToVram(0);
    LoadPalette(sFrontierSquares_Palette, 0xF0, 0x20);

    gBattle_BG0_X = 0;
    gBattle_BG0_Y = 0;
    SetGpuReg(REG_OFFSET_BG0VOFS, gBattle_BG0_X);
    SetGpuReg(REG_OFFSET_BG0HOFS, gBattle_BG0_Y);

    task->tData2 = 0;
    taskId = CreateTask(sub_814ABE4, 1);
    switch (Random() % 4)
    {
    case 0:
        gTasks[taskId].tSub32_X_delta = 1;
        gTasks[taskId].tSub32_Y_delta = 1;
        break;
    case 1:
        gTasks[taskId].tSub32_X_delta = -1;
        gTasks[taskId].tSub32_Y_delta = -1;
        break;
    case 2:
        gTasks[taskId].tSub32_X_delta = 1;
        gTasks[taskId].tSub32_Y_delta = -1;
        break;
    default:
        gTasks[taskId].tSub32_X_delta = -1;
        gTasks[taskId].tSub32_Y_delta = 1;
        break;
    }

    task->tState++;
    return FALSE;
}

static bool8 Phase2_FrontierSquaresScroll_Func2(struct Task *task)
{
    u8 var = gUnknown_085C9A53[task->tData2];
    u8 varDiv = var / 8;
    u8 varAnd = var & 7;

    CopyRectToBgTilemapBufferRect(
    0,
    &sFrontierSquares_Tilemap,
    0,
    0,
    4,
    4,
    4 * varDiv + 1,
    4 * varAnd,
    4,
    4,
    0xF,
    0,
    0);
    CopyBgTilemapBufferToVram(0);

    if (++task->tData2 > 63)
        task->tState++;
    return 0;
}

static bool8 Phase2_FrontierSquaresScroll_Func3(struct Task *task)
{
    BlendPalettes(PALETTES_ALL & ~(0x8000), 0x10, 0);

    task->tData2 = 0;

    task->tState++;
    return FALSE;
}

static bool8 Phase2_FrontierSquaresScroll_Func4(struct Task *task)
{
    u8 var = gUnknown_085C9A53[task->tData2];
    u8 varDiv = var / 8;
    u8 varAnd = var & 7;

    FillBgTilemapBufferRect(0, 1, 4 * varDiv + 1, 4 * varAnd, 4, 4, 0xF);
    CopyBgTilemapBufferToVram(0);

    if (++task->tData2 > 63)
    {
        DestroyTask(FindTaskIdByFunc(sub_814ABE4));
        task->tState++;
    }
    return FALSE;
}

#undef tSub32_X_delta
#undef tSub32_Y_delta
#undef tSub32_Bool

static bool8 Phase2_FrontierSquaresScroll_Func5(struct Task *task)
{
    gBattle_BG0_X = 0;
    gBattle_BG0_Y = 0;
    SetGpuReg(REG_OFFSET_BG0VOFS, 0);
    SetGpuReg(REG_OFFSET_BG0HOFS, gBattle_BG0_Y);

    FillBgTilemapBufferRect_Palette0(0, 1, 0, 0, 0x20, 0x20);
    CopyBgTilemapBufferToVram(0);
    BlendPalettes(PALETTES_ALL, 0x10, 0);

    DestroyTask(FindTaskIdByFunc(task->func));
    return FALSE;
}

#undef tData1
#undef tData2
#undef tData3
#undef tData4
#undef tData5
#undef tData6
#undef tData7
