#include "global.h"
#include "battle.h"
#include "battle_anim.h"
#include "battle_bg.h"
#include "bg.h"
#include "data.h"
#include "debug.h"
#include "decompress.h"
#include "field_weather.h"
#include "gpu_regs.h"
#include "item.h"
#include "list_menu.h"
#include "m4a.h"
#include "main.h"
#include "malloc.h"
#include "menu.h"
#include "overworld.h"
#include "palette.h"
#include "palette_util.h"
#include "pokemon_animation.h"
#include "pokemon_debug.h"
#include "pokemon_icon.h"
#include "reset_rtc_screen.h"
#include "scanline_effect.h"
#include "script.h"
#include "script_pokemon_util.h"
#include "sound.h"
#include "string_util.h"
#include "strings.h"
#include "task.h"
#include "text_window.h"
#include "trainer_pokemon_sprites.h"
#include "constants/items.h"
#include "constants/rgb.h"
#include "constants/songs.h"

// Credits: Gamer2020, AsparagusEduardo, TheXaman, ShinyDragonHunter

const u16 gBgColor[] = {RGB_WHITE};

static struct PokemonDebugMenu *GetStructPtr(u8 taskId)
{
    u8 *taskDataPtr = (u8*)(&gTasks[taskId].data[0]);

    return (struct PokemonDebugMenu*)(T1_READ_PTR(taskDataPtr));
}

struct PokemonDebugMenu
{
    u16 currentMonId;
    u8 currentMonWindowId;
    u8 currentMonForm;
    u8 frontSpriteId;
    u8 backSpriteId;
    u8 iconSpriteId;
    bool8 isShiny;
    bool8 isFemale;
    struct DebugModifyArrows modifyArrows;
    struct PokemonDebugOptionArrows optionArrows;
    u8 animIdBack;
    u8 animIdFront;
    u8 battleBgType;
    u8 battleTerrain;
    bool8 inSubmenu;
    u8 submenuYpos;
    u8 timeOfDay;
};

// Bg templates
static const struct BgTemplate sBgTemplates[] =
{
    {
        .bg = 0,
        .charBaseIndex = 0,
        .mapBaseIndex = 24,
        .screenSize = 2,
        .paletteMode = 0,
        .priority = 0,
        .baseTile = 0
    },
    {
        .bg = 1,
        .charBaseIndex = 1,
        .mapBaseIndex = 28,
        .screenSize = 2,
        .paletteMode = 0,
        .priority = 0,
        .baseTile = 0
    },
    {
        .bg = 2,
        .charBaseIndex = 1,
        .mapBaseIndex = 30,
        .screenSize = 1,
        .paletteMode = 0,
        .priority = 1,
        .baseTile = 0
    },
   {
        .bg = 3,
        .charBaseIndex = 2,
        .mapBaseIndex = 26,
        .screenSize = 1,
        .paletteMode = 0,
        .priority = 3,
        .baseTile = 0
    },
};

// Window templates
static const struct WindowTemplate sPokemonDebugWindowTemplate[] =
{
    [WIN_NAME_NUMBERS] = {
        .bg = 0,
        .tilemapLeft = 15,
        .tilemapTop = 12,
        .width = 15,
        .height = 2,
        .paletteNum = 0xF,
        .baseBlock = 1
    },
    [WIN_INSTRUCTIONS] = {
        .bg = 0,
        .tilemapLeft = 0,
        .tilemapTop = 0,
        .width = 15,
        .height = 4,
        .paletteNum = 0xF,
        .baseBlock = 31
    },
    [WIN_BOTTOM_LEFT] = {
        .bg = 0,
        .tilemapLeft = 1,
        .tilemapTop = 14,
        .width = 5,
        .height = 6,
        .paletteNum = 0xF,
        .baseBlock = 91
    },
    [WIN_BOTTOM_RIGHT] = {
        .bg = 0,
        .tilemapLeft = 6,
        .tilemapTop = 14,
        .width = 25,
        .height = 6,
        .paletteNum = 0xF,
        .baseBlock = 121
    },
    DUMMY_WIN_TEMPLATE,
};


// Lookup tables
static const u8 sBackAnimNames[][24] =
{
    [BACK_ANIM_NONE]                    = _("NONE"),
    [BACK_ANIM_H_VIBRATE]               = _("H VIBRATE"),
    [BACK_ANIM_H_SLIDE]                 = _("H SLIDE"),
    [BACK_ANIM_H_SPRING]                = _("H SPRING"),
    [BACK_ANIM_H_SPRING_REPEATED]       = _("H SPRING REPEATED"),
    [BACK_ANIM_SHRINK_GROW]             = _("SHRINK GROW"),
    [BACK_ANIM_GROW]                    = _("GROW"),
    [BACK_ANIM_CIRCLE_COUNTERCLOCKWISE] = _("CIRCLE COUNTERCLOCKWISE"),
    [BACK_ANIM_H_SHAKE]                 = _("H SHAKE"),
    [BACK_ANIM_V_SHAKE]                 = _("V SHAKE"),
    [BACK_ANIM_V_SHAKE_H_SLIDE]         = _("V SHAKE H SLIDE"),
    [BACK_ANIM_V_STRETCH]               = _("V STRETCH"),
    [BACK_ANIM_H_STRETCH]               = _("H STRETCH"),
    [BACK_ANIM_GROW_STUTTER]            = _("GROW STUTTER"),
    [BACK_ANIM_V_SHAKE_LOW]             = _("V SHAKE LOW"),
    [BACK_ANIM_TRIANGLE_DOWN]           = _("TRIANGLE DOWN"),
    [BACK_ANIM_CONCAVE_ARC_LARGE]       = _("CONCAVE ARC LARGE"),
    [BACK_ANIM_CONVEX_DOUBLE_ARC]       = _("CONVEX DOUBLE ARC"),
    [BACK_ANIM_CONCAVE_ARC_SMALL]       = _("CONCAVE ARC SMALL"),
    [BACK_ANIM_DIP_RIGHT_SIDE]          = _("DIP RIGHT SIDE"),
    [BACK_ANIM_SHRINK_GROW_VIBRATE]     = _("SHRINK GROW VIBRATE"),
    [BACK_ANIM_JOLT_RIGHT]              = _("JOLT RIGHT"),
    [BACK_ANIM_SHAKE_FLASH_YELLOW]      = _("SHAKE FLASH YELLOW"),
    [BACK_ANIM_SHAKE_GLOW_RED]          = _("SHAKE GLOW RED"),
    [BACK_ANIM_SHAKE_GLOW_GREEN]        = _("SHAKE GLOW GREEN"),
    [BACK_ANIM_SHAKE_GLOW_BLUE]         = _("SHAKE GLOW BLUE"),
    [BACK_ANIM_SHAKE_GLOW_PURPLE]       = _("SHAKE GLOW PURPLE"),
};

static const u8 sFrontAnimNames[][34] =
{
    [ANIM_NONE]                              = _("NONE"),
    [ANIM_V_SQUISH_AND_BOUNCE]               = _("V SQUISH AND BOUNCE"),
    [ANIM_CIRCULAR_STRETCH_TWICE]            = _("CIRCULAR STRETCH TWICE"),
    [ANIM_H_VIBRATE]                         = _("H VIBRATE"),
    [ANIM_H_SLIDE]                           = _("H SLIDE"),
    [ANIM_V_SLIDE]                           = _("V SLIDE"),
    [ANIM_BOUNCE_ROTATE_TO_SIDES]            = _("BOUNCE ROTATE TO SIDES"),
    [ANIM_V_JUMPS_H_JUMPS]                   = _("V JUMPS H JUMPS"),
    [ANIM_ROTATE_TO_SIDES]                   = _("ROTATE TO SIDES"),
    [ANIM_ROTATE_TO_SIDES_TWICE]             = _("ROTATE TO SIDES TWICE"),
    [ANIM_GROW_VIBRATE]                      = _("GROW VIBRATE"),
    [ANIM_ZIGZAG_FAST]                       = _("ZIGZAG FAST"),
    [ANIM_SWING_CONCAVE]                     = _("SWING CONCAVE"),
    [ANIM_SWING_CONCAVE_FAST]                = _("SWING CONCAVE FAST"),
    [ANIM_SWING_CONVEX]                      = _("SWING CONVEX"),
    [ANIM_SWING_CONVEX_FAST]                 = _("SWING CONVEX FAST"),
    [ANIM_H_SHAKE]                           = _("H SHAKE"),
    [ANIM_V_SHAKE]                           = _("V SHAKE"),
    [ANIM_CIRCULAR_VIBRATE]                  = _("CIRCULAR VIBRATE"),
    [ANIM_TWIST]                             = _("TWIST"),
    [ANIM_SHRINK_GROW]                       = _("SHRINK GROW"),
    [ANIM_CIRCLE_C_CLOCKWISE]                = _("CIRCLE C CLOCKWISE"),
    [ANIM_GLOW_BLACK]                        = _("GLOW BLACK"),
    [ANIM_H_STRETCH]                         = _("H STRETCH"),
    [ANIM_V_STRETCH]                         = _("V STRETCH"),
    [ANIM_RISING_WOBBLE]                     = _("RISING WOBBLE"),
    [ANIM_V_SHAKE_TWICE]                     = _("V SHAKE TWICE"),
    [ANIM_TIP_MOVE_FORWARD]                  = _("TIP MOVE FORWARD"),
    [ANIM_H_PIVOT]                           = _("H PIVOT"),
    [ANIM_V_SLIDE_WOBBLE]                    = _("V SLIDE WOBBLE"),
    [ANIM_H_SLIDE_WOBBLE]                    = _("H SLIDE WOBBLE"),
    [ANIM_V_JUMPS_BIG]                       = _("V JUMPS BIG"),
    [ANIM_SPIN_LONG]                         = _("SPIN LONG"),
    [ANIM_GLOW_ORANGE]                       = _("GLOW ORANGE"),
    [ANIM_GLOW_RED]                          = _("GLOW RED"),
    [ANIM_GLOW_BLUE]                         = _("GLOW BLUE"),
    [ANIM_GLOW_YELLOW]                       = _("GLOW YELLOW"),
    [ANIM_GLOW_PURPLE]                       = _("GLOW PURPLE"),
    [ANIM_BACK_AND_LUNGE]                    = _("BACK AND LUNGE"),
    [ANIM_BACK_FLIP]                         = _("BACK FLIP"),
    [ANIM_FLICKER]                           = _("FLICKER"),
    [ANIM_BACK_FLIP_BIG]                     = _("BACK FLIP BIG"),
    [ANIM_FRONT_FLIP]                        = _("FRONT FLIP"),
    [ANIM_TUMBLING_FRONT_FLIP]               = _("TUMBLING FRONT FLIP"),
    [ANIM_FIGURE_8]                          = _("FIGURE 8"),
    [ANIM_FLASH_YELLOW]                      = _("FLASH YELLOW"),
    [ANIM_SWING_CONCAVE_FAST_SHORT]          = _("SWING CONCAVE FAST SHORT"),
    [ANIM_SWING_CONVEX_FAST_SHORT]           = _("SWING CONVEX FAST SHORT"),
    [ANIM_ROTATE_UP_SLAM_DOWN]               = _("ROTATE UP SLAM DOWN"),
    [ANIM_DEEP_V_SQUISH_AND_BOUNCE]          = _("DEEP V SQUISH AND BOUNCE"),
    [ANIM_H_JUMPS]                           = _("H JUMPS"),
    [ANIM_H_JUMPS_V_STRETCH]                 = _("H JUMPS V STRETCH"),
    [ANIM_ROTATE_TO_SIDES_FAST]              = _("ROTATE TO SIDES FAST"),
    [ANIM_ROTATE_UP_TO_SIDES]                = _("ROTATE UP TO SIDES"),
    [ANIM_FLICKER_INCREASING]                = _("FLICKER INCREASING"),
    [ANIM_TIP_HOP_FORWARD]                   = _("TIP HOP FORWARD"),
    [ANIM_PIVOT_SHAKE]                       = _("PIVOT SHAKE"),
    [ANIM_TIP_AND_SHAKE]                     = _("TIP AND SHAKE"),
    [ANIM_VIBRATE_TO_CORNERS]                = _("VIBRATE TO CORNERS"),
    [ANIM_GROW_IN_STAGES]                    = _("GROW IN STAGES"),
    [ANIM_V_SPRING]                          = _("V SPRING"),
    [ANIM_V_REPEATED_SPRING]                 = _("V REPEATED SPRING"),
    [ANIM_SPRING_RISING]                     = _("SPRING RISING"),
    [ANIM_H_SPRING]                          = _("H SPRING"),
    [ANIM_H_REPEATED_SPRING_SLOW]            = _("H REPEATED SPRING SLOW"),
    [ANIM_H_SLIDE_SHRINK]                    = _("H SLIDE SHRINK"),
    [ANIM_LUNGE_GROW]                        = _("LUNGE GROW"),
    [ANIM_CIRCLE_INTO_BG]                    = _("CIRCLE INTO BG"),
    [ANIM_RAPID_H_HOPS]                      = _("RAPID H HOPS"),
    [ANIM_FOUR_PETAL]                        = _("FOUR PETAL"),
    [ANIM_V_SQUISH_AND_BOUNCE_SLOW]          = _("V SQUISH AND BOUNCE SLOW"),
    [ANIM_H_SLIDE_SLOW]                      = _("H SLIDE SLOW"),
    [ANIM_V_SLIDE_SLOW]                      = _("V SLIDE SLOW"),
    [ANIM_BOUNCE_ROTATE_TO_SIDES_SMALL]      = _("BOUNCE ROTATE TO SIDES SMALL"),
    [ANIM_BOUNCE_ROTATE_TO_SIDES_SLOW]       = _("BOUNCE ROTATE TO SIDES SLOW"),
    [ANIM_BOUNCE_ROTATE_TO_SIDES_SMALL_SLOW] = _("BOUNCE ROTATE TO SIDES SMALL SLOW"),
    [ANIM_ZIGZAG_SLOW]                       = _("ZIGZAG SLOW"),
    [ANIM_H_SHAKE_SLOW]                      = _("H SHAKE SLOW"),
    [ANIM_V_SHAKE_SLOW]                      = _("V SHAKE SLOW"),
    [ANIM_TWIST_TWICE]                       = _("TWIST TWICE"),
    [ANIM_CIRCLE_C_CLOCKWISE_SLOW]           = _("CIRCLE C CLOCKWISE SLOW"),
    [ANIM_V_SHAKE_TWICE_SLOW]                = _("V SHAKE TWICE SLOW"),
    [ANIM_V_SLIDE_WOBBLE_SMALL]              = _("V SLIDE WOBBLE SMALL"),
    [ANIM_V_JUMPS_SMALL]                     = _("V JUMPS SMALL"),
    [ANIM_SPIN]                              = _("SPIN"),
    [ANIM_TUMBLING_FRONT_FLIP_TWICE]         = _("TUMBLING FRONT FLIP TWICE"),
    [ANIM_DEEP_V_SQUISH_AND_BOUNCE_TWICE]    = _("DEEP V SQUISH AND BOUNCE TWICE"),
    [ANIM_H_JUMPS_V_STRETCH_TWICE]           = _("H JUMPS V STRETCH TWICE"),
    [ANIM_V_SHAKE_BACK]                      = _("V SHAKE BACK"),
    [ANIM_V_SHAKE_BACK_SLOW]                 = _("V SHAKE BACK SLOW"),
    [ANIM_V_SHAKE_H_SLIDE_SLOW]              = _("V SHAKE H SLIDE SLOW"),
    [ANIM_V_STRETCH_BOTH_ENDS_SLOW]          = _("V STRETCH BOTH ENDS SLOW"),
    [ANIM_H_STRETCH_FAR_SLOW]                = _("H STRETCH FAR SLOW"),
    [ANIM_V_SHAKE_LOW_TWICE]                 = _("V SHAKE LOW TWICE"),
    [ANIM_H_SHAKE_FAST]                      = _("H SHAKE FAST"),
    [ANIM_H_SLIDE_FAST]                      = _("H SLIDE FAST"),
    [ANIM_H_VIBRATE_FAST]                    = _("H VIBRATE FAST"),
    [ANIM_H_VIBRATE_FASTEST]                 = _("H VIBRATE FASTEST"),
    [ANIM_V_SHAKE_BACK_FAST]                 = _("V SHAKE BACK FAST"),
    [ANIM_V_SHAKE_LOW_TWICE_SLOW]            = _("V SHAKE LOW TWICE SLOW"),
    [ANIM_V_SHAKE_LOW_TWICE_FAST]            = _("V SHAKE LOW TWICE FAST"),
    [ANIM_CIRCLE_C_CLOCKWISE_LONG]           = _("CIRCLE C CLOCKWISE LONG"),
    [ANIM_GROW_STUTTER_SLOW]                 = _("GROW STUTTER SLOW"),
    [ANIM_V_SHAKE_H_SLIDE]                   = _("V SHAKE H SLIDE"),
    [ANIM_V_SHAKE_H_SLIDE_FAST]              = _("V SHAKE H SLIDE FAST"),
    [ANIM_TRIANGLE_DOWN_SLOW]                = _("TRIANGLE DOWN SLOW"),
    [ANIM_TRIANGLE_DOWN]                     = _("TRIANGLE DOWN"),
    [ANIM_TRIANGLE_DOWN_TWICE]               = _("TRIANGLE DOWN TWICE"),
    [ANIM_GROW]                              = _("GROW"),
    [ANIM_GROW_TWICE]                        = _("GROW TWICE"),
    [ANIM_H_SPRING_FAST]                     = _("H SPRING FAST"),
    [ANIM_H_SPRING_SLOW]                     = _("H SPRING SLOW"),
    [ANIM_H_REPEATED_SPRING_FAST]            = _("H REPEATED SPRING FAST"),
    [ANIM_H_REPEATED_SPRING]                 = _("H REPEATED SPRING"),
    [ANIM_SHRINK_GROW_FAST]                  = _("SHRINK GROW FAST"),
    [ANIM_SHRINK_GROW_SLOW]                  = _("SHRINK GROW SLOW"),
    [ANIM_V_STRETCH_BOTH_ENDS]               = _("V STRETCH BOTH ENDS"),
    [ANIM_V_STRETCH_BOTH_ENDS_TWICE]         = _("V STRETCH BOTH ENDS TWICE"),
    [ANIM_H_STRETCH_FAR_TWICE]               = _("H STRETCH FAR TWICE"),
    [ANIM_H_STRETCH_FAR]                     = _("H STRETCH FAR"),
    [ANIM_GROW_STUTTER_TWICE]                = _("GROW STUTTER TWICE"),
    [ANIM_GROW_STUTTER]                      = _("GROW STUTTER"),
    [ANIM_CONCAVE_ARC_LARGE_SLOW]            = _("CONCAVE ARC LARGE SLOW"),
    [ANIM_CONCAVE_ARC_LARGE]                 = _("CONCAVE ARC LARGE"),
    [ANIM_CONCAVE_ARC_LARGE_TWICE]           = _("CONCAVE ARC LARGE TWICE"),
    [ANIM_CONVEX_DOUBLE_ARC_SLOW]            = _("CONVEX DOUBLE ARC SLOW"),
    [ANIM_CONVEX_DOUBLE_ARC]                 = _("CONVEX DOUBLE ARC"),
    [ANIM_CONVEX_DOUBLE_ARC_TWICE]           = _("CONVEX DOUBLE ARC TWICE"),
    [ANIM_CONCAVE_ARC_SMALL_SLOW]            = _("CONCAVE ARC SMALL SLOW"),
    [ANIM_CONCAVE_ARC_SMALL]                 = _("CONCAVE ARC SMALL"),
    [ANIM_CONCAVE_ARC_SMALL_TWICE]           = _("CONCAVE ARC SMALL TWICE"),
    [ANIM_H_DIP]                             = _("H DIP"),
    [ANIM_H_DIP_FAST]                        = _("H DIP FAST"),
    [ANIM_H_DIP_TWICE]                       = _("H DIP TWICE"),
    [ANIM_SHRINK_GROW_VIBRATE_FAST]          = _("SHRINK GROW VIBRATE FAST"),
    [ANIM_SHRINK_GROW_VIBRATE]               = _("SHRINK GROW VIBRATE"),
    [ANIM_SHRINK_GROW_VIBRATE_SLOW]          = _("SHRINK GROW VIBRATE SLOW"),
    [ANIM_JOLT_RIGHT_FAST]                   = _("JOLT RIGHT FAST"),
    [ANIM_JOLT_RIGHT]                        = _("JOLT RIGHT"),
    [ANIM_JOLT_RIGHT_SLOW]                   = _("JOLT RIGHT SLOW"),
    [ANIM_SHAKE_FLASH_YELLOW_FAST]           = _("SHAKE FLASH YELLOW FAST"),
    [ANIM_SHAKE_FLASH_YELLOW]                = _("SHAKE FLASH YELLOW"),
    [ANIM_SHAKE_FLASH_YELLOW_SLOW]           = _("SHAKE FLASH YELLOW SLOW"),
    [ANIM_SHAKE_GLOW_RED_FAST]               = _("SHAKE GLOW RED FAST"),
    [ANIM_SHAKE_GLOW_RED]                    = _("SHAKE GLOW RED"),
    [ANIM_SHAKE_GLOW_RED_SLOW]               = _("SHAKE GLOW RED SLOW"),
    [ANIM_SHAKE_GLOW_GREEN_FAST]             = _("SHAKE GLOW GREEN FAST"),
    [ANIM_SHAKE_GLOW_GREEN]                  = _("SHAKE GLOW GREEN"),
    [ANIM_SHAKE_GLOW_GREEN_SLOW]             = _("SHAKE GLOW GREEN SLOW"),
    [ANIM_SHAKE_GLOW_BLUE_FAST]              = _("SHAKE GLOW BLUE FAST"),
    [ANIM_SHAKE_GLOW_BLUE]                   = _("SHAKE GLOW BLUE"),
    [ANIM_SHAKE_GLOW_BLUE_SLOW]              = _("SHAKE GLOW BLUE SLOW"),
    [ANIM_SHAKE_GLOW_PURPLE_FAST]            = _("SHAKE GLOW PURPLE FAST"),
    [ANIM_SHAKE_GLOW_PURPLE]                 = _("SHAKE GLOW PURPLE"),
    [ANIM_SHAKE_GLOW_PURPLE_SLOW]            = _("SHAKE GLOW PURPLE SLOW"),
};

static const u8 sBattleBackgroundNames[][32] = 
{
    [MAP_BATTLE_SCENE_NORMAL]   = _("NORMAL                        "),
    [MAP_BATTLE_SCENE_GYM]      = _("GYM                           "),
    [MAP_BATTLE_SCENE_INDOOR_1] = _("INDOOR 1                      "),
    [MAP_BATTLE_SCENE_INDOOR_2] = _("INDOOR 2                      "),
    [MAP_BATTLE_SCENE_LORELEI]  = _("LORELEI                       "),
    [MAP_BATTLE_SCENE_BRUNO]    = _("BRUNO                         "),
    [MAP_BATTLE_SCENE_AGATHA]   = _("AGATHA                        "),
    [MAP_BATTLE_SCENE_LANCE]    = _("LANCE                         "),
    [MAP_BATTLE_SCENE_LINK]     = _("LINK                          "),
    [MAP_BATTLE_SCENE_CHAMPION] = _("CHAMPION                      "),
};

static const u8 sText_GrassMorning[]     = _("NORMAL - GRASS - MORN         ");
static const u8 sText_GrassDay[]         = _("NORMAL - GRASS - DAY          ");
static const u8 sText_GrassNight[]       = _("NORMAL - GRASS - NITE         ");
static const u8 sText_LongGrassMorning[] = _("NORMAL - LONG GRASS - MORN");
static const u8 sText_LongGrassDay[]     = _("NORMAL - LONG GRASS - DAY     ");
static const u8 sText_LongGrassNight[]   = _("NORMAL - LONG GRASS - NITE");
static const u8 sText_SandMorning[]      = _("NORMAL - LONG GRASS - MORN");
static const u8 sText_SandDay[]          = _("NORMAL - LONG GRASS - DAY     ");
static const u8 sText_SandNight[]        = _("NORMAL - LONG GRASS - NITE");
static const u8 sText_Underwater[]       = _("NORMAL - UNDERWATER           ");
static const u8 sText_WaterMorning[]     = _("NORMAL - WATER - MORN         ");
static const u8 sText_WaterDay[]         = _("NORMAL - WATER - DAY          ");
static const u8 sText_WaterNight[]       = _("NORMAL - WATER - NITE         ");
static const u8 sText_PondMorning[]      = _("NORMAL - POND - MORN          ");
static const u8 sText_PondDay[]          = _("NORMAL - POND - DAY           ");
static const u8 sText_PondNight[]        = _("NORMAL - POND - NITE          ");
static const u8 sText_MountainMorning[]  = _("NORMAL - MOUNTAIN - MORN      ");
static const u8 sText_MountainDay[]      = _("NORMAL - MOUNTAIN - DAY       ");
static const u8 sText_MountainNight[]    = _("NORMAL - MOUNTAIN - NITE      ");
static const u8 sText_Cave[]             = _("NORMAL - CAVE                 ");
static const u8 sText_Building[]         = _("NORMAL - BUILDING             ");
static const u8 sText_PlainMorning[]     = _("NORMAL - PLAIN - MORN         ");
static const u8 sText_PlainDay[]         = _("NORMAL - PLAIN - DAY          ");
static const u8 sText_PlainNight[]       = _("NORMAL - PLAIN - NITE         ");


static const u8 *const sBattleBackgroundTerrainNames[][3] = 
{
    [BATTLE_TERRAIN_GRASS]      = {sText_GrassMorning,     sText_GrassDay,     sText_GrassNight},
    [BATTLE_TERRAIN_LONG_GRASS] = {sText_LongGrassMorning, sText_LongGrassDay, sText_LongGrassNight},
    [BATTLE_TERRAIN_SAND]       = {sText_SandMorning,      sText_SandDay,      sText_SandNight},
    [BATTLE_TERRAIN_UNDERWATER] = {sText_Underwater,       sText_Underwater,   sText_Underwater},
    [BATTLE_TERRAIN_WATER]      = {sText_WaterMorning,     sText_WaterDay,     sText_WaterNight},
    [BATTLE_TERRAIN_POND]       = {sText_PondMorning,      sText_PondDay,      sText_PondNight},
    [BATTLE_TERRAIN_MOUNTAIN]   = {sText_MountainMorning,  sText_MountainDay,  sText_MountainNight},
    [BATTLE_TERRAIN_CAVE]       = {sText_Cave,             sText_Cave,         sText_Cave},
    [BATTLE_TERRAIN_BUILDING]   = {sText_Building,         sText_Building,     sText_Building},
    [BATTLE_TERRAIN_PLAIN]      = {sText_PlainMorning,     sText_PlainDay,     sText_PlainNight},
};

// Forward declarations
static void PrintDigitChars(struct PokemonDebugMenu *data);
static void SetUpModifyArrows(struct PokemonDebugMenu *data);
static void CB2_DebugRunner(void);
static void ResetBGsDebugMenu(u16);
static void HandleInputPokemonDebug(u8);
static void ReloadPokemonSprites(struct PokemonDebugMenu *data);
static void ExitPokemonDebug(u8);

// Text handling functions
void PadString(const u8 *src, u8 *dst)
{
    u32 i;

    for (i = 0; i < 17 && src[i] != EOS; i++)
        dst[i] = src[i];

    for (; i < 17; i++)
        dst[i] = CHAR_SPACE;

    dst[i] = EOS;
}

static void PrintInstructionsOnWindow(struct PokemonDebugMenu *data)
{
    u8 textBottomForms[] = _("BACK:\nFRONT:\nBG:\nFORMS:$");
    u8 textBottom[] = _("BACK:\nFRONT:\nBG:$");
    u16 species = data->modifyArrows.currValue;
    
    // Instructions window
    FillWindowPixelBuffer(WIN_INSTRUCTIONS, 0x11);
    if (data->inSubmenu)
    {
        u8 textInstructionsInSubmenuGender[] = _("{START_BUTTON} Shiny {SELECT_BUTTON} Gender\n{B_BUTTON} Back$");
        u8 textInstructionsInSubmenu[] = _("{START_BUTTON} Shiny {B_BUTTON} Back$");

        if ((gBaseStats[species].flags & FLAG_GENDER_DIFFERENCE))
            AddTextPrinterParameterized(WIN_INSTRUCTIONS, 4, textInstructionsInSubmenuGender, 2, 0, 0, NULL);
        else
            AddTextPrinterParameterized(WIN_INSTRUCTIONS, 4, textInstructionsInSubmenu, 2, 0, 0, NULL);
    }
    else
    {
        u8 textInstructionsGender[] = _("{START_BUTTON} Shiny {SELECT_BUTTON} Gender\n{B_BUTTON} Exit {A_BUTTON} Submenu$");
        u8 textInstructions[] = _("{START_BUTTON} Shiny {B_BUTTON} Exit\n{A_BUTTON} Submenu$");

        if ((gBaseStats[species].flags & FLAG_GENDER_DIFFERENCE))
            AddTextPrinterParameterized(WIN_INSTRUCTIONS, 4, textInstructionsGender, 2, 0, 0, NULL);
        else
            AddTextPrinterParameterized(WIN_INSTRUCTIONS, 4, textInstructions, 2, 0, 0, NULL);
    }
    CopyWindowToVram(WIN_INSTRUCTIONS, 3);

    // Bottom left text
    if (data->currentMonId == SPECIES_ARBOK || data->currentMonId == SPECIES_UNOWN || gBaseStats[data->currentMonId].forms[0])
        AddTextPrinterParameterized(WIN_BOTTOM_LEFT, 4, textBottomForms, 1, 0, 0, NULL);
    else
    {
        FillWindowPixelBuffer(WIN_BOTTOM_LEFT, PIXEL_FILL(0));
        AddTextPrinterParameterized(WIN_BOTTOM_LEFT, 4, textBottom, 1, 0, 0, NULL);
    }
}

static void VBlankCB(void)
{
    LoadOam();
    ProcessSpriteCopyRequests();
    TransferPlttBuffer();
}

static void SetStructPtr(u8 taskId, void *ptr)
{
    u32 structPtr = (u32)(ptr);
    u8 *taskDataPtr = (u8*)(&gTasks[taskId].data[0]);

    taskDataPtr[0] = structPtr >> 0;
    taskDataPtr[1] = structPtr >> 8;
    taskDataPtr[2] = structPtr >> 16;
    taskDataPtr[3] = structPtr >> 24;
}

// Digit and arrow functions
static void PrintDigitChars(struct PokemonDebugMenu *data)
{
    s32 i;
    u16 species = data->modifyArrows.currValue;
    u8 text[MODIFY_DIGITS_MAX + POKEMON_NAME_LENGTH + 4];

    for (i = 0; i < data->modifyArrows.maxDigits; i++)
        text[i] = data->modifyArrows.charDigits[i];
    
    text[i++] = CHAR_SPACE;
    text[i++] = CHAR_HYPHEN;

    text[i++] = CHAR_SPACE;

    if ((gBaseStats[species].flags & FLAG_GENDER_DIFFERENCE))
        text[i++] = CHAR_MALE + data->isFemale;

    StringCopy(&text[i], gSpeciesNames[species]);

    FillWindowPixelBuffer(WIN_NAME_NUMBERS, 0x11);
    AddTextPrinterParameterized(WIN_NAME_NUMBERS, 2, text, 6, 1, 0, NULL);
}

u32 CharDigitsToValue(u8 *charDigits, u8 maxDigits)
{
    s32 i;
    u8 id = 0;
    u32 newValue = 0;
    u8 valueDigits[MODIFY_DIGITS_MAX];

    for (i = 0; i < MODIFY_DIGITS_MAX; i++)
        valueDigits[i] = charDigits[i] - CHAR_0;

    if (maxDigits >= MODIFY_DIGITS_MAX)
        newValue += valueDigits[id++] * 1000;
    if (maxDigits >= MODIFY_DIGITS_MAX - 1)
        newValue += valueDigits[id++] * 100;
    if (maxDigits >= MODIFY_DIGITS_MAX - 2)
        newValue += valueDigits[id++] * 10;
    if (maxDigits >= MODIFY_DIGITS_MAX - 3)
        newValue += valueDigits[id++];

    return newValue;
}

void ValueToCharDigits(u8 *charDigits, u32 newValue, u8 maxDigits)
{
    s32 i;
    u8 valueDigits[MODIFY_DIGITS_MAX];
    u8 id = 0;

    if (maxDigits >= MODIFY_DIGITS_MAX)
        valueDigits[id++] = newValue / 1000;
    if (maxDigits >= MODIFY_DIGITS_MAX - 1)
        valueDigits[id++] = (newValue % 1000) / 100;
    if (maxDigits >= MODIFY_DIGITS_MAX - 2)
        valueDigits[id++] = (newValue % 100) / 10;
    if (maxDigits >= MODIFY_DIGITS_MAX - 3)
        valueDigits[id++] = newValue % 10;

    for (i = 0; i < MODIFY_DIGITS_MAX; i++)
        charDigits[i] = valueDigits[i] + CHAR_0;
}

static void SetArrowInvisibility(struct PokemonDebugMenu *data)
{
    bool8 invisible = data->inSubmenu;
    gSprites[data->modifyArrows.arrowSpriteId[0]].invisible = invisible;
    gSprites[data->modifyArrows.arrowSpriteId[1]].invisible = invisible;
    gSprites[data->optionArrows.arrowSpriteId[0]].invisible = !invisible;
}

static void SetUpModifyArrows(struct PokemonDebugMenu *data)
{
    LoadSpritePalette(&gSpritePalette_Arrow);
    data->modifyArrows.arrowSpriteId[0] = CreateSprite(&gSpriteTemplate_Arrow, MODIFY_DIGITS_ARROW_X, MODIFY_DIGITS_ARROW1_Y, 0);
    data->modifyArrows.arrowSpriteId[1] = CreateSprite(&gSpriteTemplate_Arrow, MODIFY_DIGITS_ARROW_X, MODIFY_DIGITS_ARROW2_Y, 0);
    gSprites[data->modifyArrows.arrowSpriteId[1]].animNum = 1;

    data->modifyArrows.minValue = SPECIES_BULBASAUR;
    data->modifyArrows.maxValue = NUM_SPECIES - 1;
    data->modifyArrows.maxDigits = 3;
    data->modifyArrows.modifiedValPtr = &data->currentMonId;
    data->modifyArrows.currValue = data->currentMonId;
    data->modifyArrows.currValueForm = data->currentMonForm;

    data->modifyArrows.currentDigit = 0;
    ValueToCharDigits(data->modifyArrows.charDigits, data->modifyArrows.currValue, data->modifyArrows.maxDigits);
}

static void SetUpOptionArrows(struct PokemonDebugMenu *data)
{
    LoadSpritePalette(&gSpritePalette_Arrow);
    data->optionArrows.arrowSpriteId[0] = CreateSprite(&gSpriteTemplate_Arrow, OPTIONS_ARROW_1_X, OPTIONS_ARROW_Y, 0);
    gSprites[data->optionArrows.arrowSpriteId[0]].animNum = 2;

    data->optionArrows.currentDigit = 0;

    gSprites[data->optionArrows.arrowSpriteId[0]].invisible = TRUE;
}

bool32 TryMoveDigit(struct DebugModifyArrows *modArrows, bool32 moveUp)
{
    s32 i;
    u8 charDigits[MODIFY_DIGITS_MAX];
    u32 newValue;

    for (i = 0; i < MODIFY_DIGITS_MAX; i++)
        charDigits[i] = modArrows->charDigits[i];

    if (moveUp)
    {
        if (charDigits[modArrows->currentDigit] == CHAR_9)
        {
            charDigits[modArrows->currentDigit] = CHAR_0;
            for (i = modArrows->currentDigit - 1; i >= 0; i--)
            {
                if (charDigits[i] == CHAR_9)
                    charDigits[i] = CHAR_0;
                else
                {
                    charDigits[i]++;
                    break;
                }
            }
        }
        else
            charDigits[modArrows->currentDigit]++;
    }
    else
    {
        if (charDigits[modArrows->currentDigit] == CHAR_0)
        {
            charDigits[modArrows->currentDigit] = CHAR_9;
            
            for (i = modArrows->currentDigit - 1; i >= 0; i--)
            {
                if (charDigits[i] == CHAR_0)
                    charDigits[i] = CHAR_9;
                else
                {
                    charDigits[i]--;
                    break;
                }
            }
        }
        else
            charDigits[modArrows->currentDigit]--;
    }

    newValue = CharDigitsToValue(charDigits, modArrows->maxDigits);
    if (newValue > modArrows->maxValue || newValue < modArrows->minValue)
    {
        return FALSE;
    }
    else
    {
        modArrows->currValue = newValue;
        for (i = 0; i < MODIFY_DIGITS_MAX; i++)
             modArrows->charDigits[i] = charDigits[i];
        return TRUE;
    }
}

// Sprite functions
static const struct SpritePalette *GetMonSpritePalStruct_Debug(u16 species, bool32 isShiny)
{
    if (isShiny)
        return &gMonShinyPaletteTable[species];
    return &gMonPaletteTable[species];
}

static void LoadSpecialPokePic_Debug(const struct CompressedSpriteSheet *src, void *dest, s32 species, bool32 isFrontPic, bool32 isFemale, u8 form)
{
    if (species == SPECIES_UNOWN)
    {
        s32 variantSpecies = species;

        if (form)
	        variantSpecies = NUM_SPECIES + form;

        if (isFrontPic)
            LZ77UnCompWram(gMonFrontPicTable[variantSpecies].data, dest);
        else
            LZ77UnCompWram(gMonBackPicTable[variantSpecies].data, dest);
    }
    else if (species == SPECIES_ARBOK && isFrontPic && form)
        LZ77UnCompWram(gMonFrontPicTable[SPECIES_ARBOK_KANTO].data, dest);
    else if ((gBaseStats[species].flags & FLAG_GENDER_DIFFERENCE) && isFemale)
    {
        if (isFrontPic)
            LZ77UnCompWram(gFemaleMonFrontPicTable[species].data, dest);
        else
            LZ77UnCompWram(gFemaleMonBackPicTable[species].data, dest);
    }
    else
        LZ77UnCompWram(src->data, dest);
    if (species == SPECIES_SPINDA && isFrontPic)
    {
        DrawSpindaSpotsToFrame(0, dest, FALSE);
        DrawSpindaSpotsToFrame(0, dest, TRUE);
    }
}

static void HandleLoadSpecialPokePic_Debug(const struct CompressedSpriteSheet *src, void *dest, s32 species, bool32 isFemale, u8 form)
{
    bool32 isFrontPic = FALSE; // backPic

    if (src == &gMonFrontPicTable[species] || src == &gFemaleMonFrontPicTable[species])
        isFrontPic = TRUE; // frontPic

    LoadSpecialPokePic_Debug(src, dest, species, isFrontPic, isFemale, form);
}

static void BattleLoadMonSpriteGfx_Debug(u16 species, bool32 isShiny)
{
    const void *paletteData = gMonPaletteTable[species].data;

    if (isShiny)
        paletteData = gMonShinyPaletteTable[species].data;

    LoadPalette(paletteData, 0x100 + 5 * 16, 32);
}

// Battle background functions
static void LoadBattleBg(u8 battleBgType, u8 battleTerrain, u8 timeOfDay)
{
    switch (battleBgType)
    {
    case MAP_BATTLE_SCENE_NORMAL:
        LZDecompressVram(gBattleTerrainTable[battleTerrain].tileset, (void*)(BG_CHAR_ADDR(2)));
        LZDecompressVram(gBattleTerrainTable[battleTerrain].tilemap, (void*)(BG_SCREEN_ADDR(26)));
        LoadCompressedPalette(gBattleTerrainTable[battleTerrain].palette[timeOfDay], 0x20, 0x60);
        break;
    default:
        LZDecompressVram(gBattleTerrainTiles_Building, (void*)(BG_CHAR_ADDR(2)));
        LZDecompressVram(gBattleTerrainTilemap_Building, (void*)(BG_SCREEN_ADDR(26)));
        LoadCompressedPalette(gBattleTerrainTable[BATTLE_TERRAIN_PLAIN + battleBgType].palette[0], 0x20, 0x60);
        break;
    }
}

static void PrintBattleBgName(u8 taskId)
{
    struct PokemonDebugMenu *data = GetStructPtr(taskId);
    u8 text[32 + 1];

    if (data->battleBgType)
        StringCopy(text, sBattleBackgroundNames[data->battleBgType]);
    else
        StringCopy(text, sBattleBackgroundTerrainNames[data->battleTerrain][data->timeOfDay]);
    AddTextPrinterParameterized(WIN_BOTTOM_RIGHT, 4, text, 0, 24, 0, NULL);
}

static void UpdateBattleBg(u8 taskId, bool32 increment)
{
    struct PokemonDebugMenu *data = GetStructPtr(taskId);

    if (!data->battleBgType)
    {
        bool32 noTimeOfDay = FALSE;

        if (data->battleTerrain == BATTLE_TERRAIN_UNDERWATER
         || data->battleTerrain == BATTLE_TERRAIN_CAVE
         || data->battleTerrain == BATTLE_TERRAIN_BUILDING)
            noTimeOfDay = TRUE;

        if (increment)
        {
            if (data->battleTerrain == BATTLE_TERRAIN_PLAIN)
            {
                if (data->timeOfDay == 2)
                {
                    data->battleBgType += 1;
                    data->timeOfDay = 0;
                }
                else
                    data->timeOfDay += 1;
            }
            else if (noTimeOfDay || data->timeOfDay >= 2)
            {
                data->battleTerrain += 1;
                data->timeOfDay = 0;
            }
            else
                data->timeOfDay += 1;
        }
        else
        {
            if (!data->battleTerrain)
            {
                if (data->timeOfDay)
                    data->timeOfDay -= 1;
                else
                {
                    data->battleBgType = MAP_BATTLE_SCENE_CHAMPION;
                    data->timeOfDay = 0;
                }
            }
            else if (noTimeOfDay || !data->timeOfDay)
            {
                data->battleTerrain -= 1;
                data->timeOfDay = 2;
            }
            else
                data->timeOfDay -= 1;
        }
    }
    else if (data->battleBgType == MAP_BATTLE_SCENE_GYM)
    {
        if (increment)
            data->battleBgType += 1;
        else
        {
            data->battleBgType = MAP_BATTLE_SCENE_NORMAL;
            data->battleTerrain = BATTLE_TERRAIN_PLAIN;
            data->timeOfDay = 2;
        }
    }
    else if (data->battleBgType == MAP_BATTLE_SCENE_CHAMPION)
    {
        if (increment)
        {
            data->battleBgType = MAP_BATTLE_SCENE_NORMAL;
            data->battleTerrain = BATTLE_TERRAIN_GRASS;
            data->timeOfDay = 0;
        }
        else
            data->battleBgType -= 1;
    }
    else
    {
        if (increment)
            data->battleBgType += 1;
        else 
            data->battleBgType -= 1;
    }

    PrintBattleBgName(taskId);

    LoadBattleBg(data->battleBgType, data->battleTerrain, data->timeOfDay);
}

// Main functions
static void UpdateMonAnimNames(u8 taskId)
{
    struct PokemonDebugMenu *data = GetStructPtr(taskId);
    u8 frontAnim = data->animIdFront;
    u8 backAnim = data->animIdBack;
    u8 text[34];
    u8 textL[] = _("{L_BUTTON}");
    u8 textR[] = _("{R_BUTTON}");

    FillWindowPixelBuffer(WIN_BOTTOM_RIGHT, PIXEL_FILL(0));

    // Back
    StringCopy(text, sBackAnimNames[backAnim]);
    AddTextPrinterParameterized(WIN_BOTTOM_RIGHT, 4, textL, 0, 0, 0, NULL);
    AddTextPrinterParameterized(WIN_BOTTOM_RIGHT, 4, text, 20, 0, 0, NULL);
    // Front
    StringCopy(text, sFrontAnimNames[frontAnim]);
    AddTextPrinterParameterized(WIN_BOTTOM_RIGHT, 4, textR, 0, 12, 0, NULL);
    AddTextPrinterParameterized(WIN_BOTTOM_RIGHT, 4, text, 20, 12, 0, NULL);

    PrintBattleBgName(taskId);
}

static void ResetPokemonDebugWindows(void)
{
    u32 i;

    FreeAllWindowBuffers();
    InitWindows(sPokemonDebugWindowTemplate);

    for (i = 0; i < WIN_END + 1; i++)
    {
        FillWindowPixelBuffer(i, PIXEL_FILL(0));
        PutWindowTilemap(i);
        CopyWindowToVram(i, 3);
    }
}

void CB2_Debug_Pokemon(void)
{
    u8 taskId;
    const struct SpritePalette *palette;
    struct PokemonDebugMenu *data;
    u16 species;

    switch (gMain.state)
    {
        default:
            SetVBlankCallback(NULL);
            FreeMonSpritesGfx();
            ResetBGsDebugMenu(0);
            DmaFillLarge16(3, 0, (u8 *)VRAM, VRAM_SIZE, 0x1000)
            DmaClear32(3, OAM, OAM_SIZE);
            DmaClear16(3, PLTT, PLTT_SIZE);
            gMain.state = 1;
            break;
        case 1:
            ScanlineEffect_Stop();
            ResetTasks();
            ResetSpriteData();
            ResetPaletteFadeControl();
            FreeAllSpritePalettes();
            gReservedSpritePaletteCount = 8;
            ResetAllPicSprites();
            BlendPalettes(PALETTES_ALL, 16, RGB_BLACK);
            LoadPalette(GetTextWindowPalette(0), 15 * 16, 0x40);

            FillBgTilemapBufferRect(0, 0, 0, 0, 32, 20, 15);
            InitBgsFromTemplates(0, sBgTemplates, ARRAY_COUNT(sBgTemplates));
            LoadBattleBg(MAP_BATTLE_SCENE_NORMAL, BATTLE_TERRAIN_GRASS, 0);
            
            gMain.state++;
            break;
        case 2:
            ResetPokemonDebugWindows();
            gMain.state++;
            break;
        case 3:
            AllocateMonSpritesGfx();

            LoadPalette(gBgColor, 0, 2);
            LoadMonIconPalette(SPECIES_BULBASAUR);

            SetGpuReg(REG_OFFSET_DISPCNT, DISPCNT_OBJ_ON | DISPCNT_OBJ_1D_MAP);
            ShowBg(0);
            ShowBg(1);
            ShowBg(2);
            ShowBg(3);

            // input task handler
            taskId = CreateTask(HandleInputPokemonDebug, 0);

            data = AllocZeroed(sizeof(struct PokemonDebugMenu));
            SetStructPtr(taskId, data);

            data->currentMonId = SPECIES_BULBASAUR;
            species = data->currentMonId;

            // Print instructions
            PrintInstructionsOnWindow(data);

            // Front
            HandleLoadSpecialPokePic_Debug(&gMonFrontPicTable[species], gMonSpritesGfxPtr->sprites.ptr[1], species, FALSE, 0);
            BattleLoadMonSpriteGfx_Debug(species, FALSE);
            SetMultiuseSpriteTemplateToPokemon(species, 1, 0);
            data->frontSpriteId = CreateSprite(&gMultiuseSpriteTemplate, DEBUG_MON_X, DEBUG_MON_Y + gMonFrontPicCoords[species].y_offset, 0);
            gSprites[data->frontSpriteId].oam.paletteNum = 5;
            gSprites[data->frontSpriteId].callback = SpriteCallbackDummy;
            gSprites[data->frontSpriteId].oam.priority = 0;

            // Back
            HandleLoadSpecialPokePic_Debug(&gMonBackPicTable[species], gMonSpritesGfxPtr->sprites.ptr[2], species, FALSE, 0);
            palette = GetMonSpritePalStruct_Debug(species, FALSE);
            LoadSpritePalette(palette);
            SetMultiuseSpriteTemplateToPokemon(species, 2, 0);
            gMultiuseSpriteTemplate.paletteTag = palette->tag;
            data->backSpriteId = CreateSprite(&gMultiuseSpriteTemplate, DEBUG_MON_BACK_X, DEBUG_MON_BACK_Y + gMonBackPicCoords[species].y_offset, 0);
            gSprites[data->backSpriteId].callback = SpriteCallbackDummy;
            gSprites[data->backSpriteId].oam.priority = 0;

            // Icon sprite
            data->iconSpriteId = CreateMonIconNoPersonality(species, SpriteCB_MonIcon, DEBUG_ICON_X, DEBUG_ICON_Y, 4);
            gSprites[data->iconSpriteId].oam.priority = 0;

            // Modify Arrows
            SetUpModifyArrows(data);
            PrintDigitChars(data);

            // Option Arrows
            SetUpOptionArrows(data);

            // Anim names
            data->animIdBack = gBaseStats[species].backPicAnim;
            data->animIdFront = gBaseStats[species].frontPicAnim;
            UpdateMonAnimNames(taskId);

            // BattleBg Name
            PrintBattleBgName(taskId);

            gMain.state++;
            break;
        case 4:
            EnableInterrupts(1);
            SetVBlankCallback(VBlankCB);
            SetMainCallback2(CB2_DebugRunner);
            m4aMPlayVolumeControl(&gMPlayInfo_BGM, 0xFFFF, 0x80);
            break;
    }
}

static void CB2_DebugRunner(void)
{
    RunTasks();
    AnimateSprites();
    BuildOamBuffer();
    UpdatePaletteFade();
}

static void ResetBGsDebugMenu(u16 a)
{
    if (!(a & DISPCNT_BG0_ON))
    {
        ClearGpuRegBits(0, DISPCNT_BG0_ON);
        SetGpuReg(REG_OFFSET_BG0CNT, 0);
        SetGpuReg(REG_OFFSET_BG0HOFS, 0);
        SetGpuReg(REG_OFFSET_BG0VOFS, 0);
    }
    if (!(a & DISPCNT_BG1_ON))
    {
        ClearGpuRegBits(0, DISPCNT_BG1_ON);
        SetGpuReg(REG_OFFSET_BG1CNT, 0);
        SetGpuReg(REG_OFFSET_BG1HOFS, 0);
        SetGpuReg(REG_OFFSET_BG1VOFS, 0);
    }
    if (!(a & DISPCNT_BG2_ON))
    {
        ClearGpuRegBits(0, DISPCNT_BG2_ON);
        SetGpuReg(REG_OFFSET_BG2CNT, 0);
        SetGpuReg(REG_OFFSET_BG2HOFS, 0);
        SetGpuReg(REG_OFFSET_BG2VOFS, 0);
    }
    if (!(a & DISPCNT_BG3_ON))
    {
        ClearGpuRegBits(0, DISPCNT_BG3_ON);
        SetGpuReg(REG_OFFSET_BG3CNT, 0);
        SetGpuReg(REG_OFFSET_BG3HOFS, 0);
        SetGpuReg(REG_OFFSET_BG3VOFS, 0);
    }
    if (!(a & DISPCNT_OBJ_ON))
    {
        ClearGpuRegBits(0, DISPCNT_OBJ_ON);
        ResetSpriteData();
        FreeAllSpritePalettes();
        gReservedSpritePaletteCount = 8;
    }
}

static void UpdateSubmenuOptionValue(u8 taskId, bool32 increment)
{
    struct PokemonDebugMenu *data = GetStructPtr(taskId);
    u8 option = data->submenuYpos;

    switch (option)
    {
    case 0:
        if (increment)
        {
            if (data->animIdBack >= BACK_ANIM_SHAKE_GLOW_PURPLE)
                data->animIdBack = BACK_ANIM_NONE;
            else
                data->animIdBack += 1;
        }
        else
        {
            if (data->animIdBack)
                data->animIdBack -= 1;
            else
                data->animIdBack = BACK_ANIM_SHAKE_GLOW_PURPLE;
        }
        UpdateMonAnimNames(taskId);
        break;
    case 1:
        if (increment)
        {
            if (data->animIdFront >= ANIM_SHAKE_GLOW_PURPLE_SLOW)
                data->animIdFront = ANIM_NONE;
            else
                data->animIdFront += 1;
        }
        else
        {
            if (data->animIdFront)
                data->animIdFront -= 1;
            else
                data->animIdFront = ANIM_SHAKE_GLOW_PURPLE_SLOW;
        }
        UpdateMonAnimNames(taskId);
        break;
    case 2:
        UpdateBattleBg(taskId, increment);
        break;
    case 3:
        if (data->currentMonId == SPECIES_ARBOK)
        {
            struct DebugModifyArrows *modArrows = &data->modifyArrows;

            data->currentMonForm = !data->currentMonForm;
            modArrows->currValue = data->currentMonId;
            *(u16*)(data->modifyArrows.modifiedValPtr) = data->modifyArrows.currValue;
            ReloadPokemonSprites(data);
            PlaySE(SE_DEX_SCROLL);
            while (!(gMain.intrCheck & INTR_FLAG_VBLANK));
        }
        if (data->currentMonId == SPECIES_UNOWN)
        {
            struct DebugModifyArrows *modArrows = &data->modifyArrows;
            if (increment)
            {
                if (data->currentMonForm < NUM_UNOWN_FORMS - 1)
                    data->currentMonForm += 1;
                else
                    data->currentMonForm = 0;
            }
            else
            {
                if (data->currentMonForm)
                    data->currentMonForm -= 1;
                else
                    data->currentMonForm = NUM_UNOWN_FORMS - 1;
            }
            modArrows->currValue = data->currentMonId;

            *(u16*)(data->modifyArrows.modifiedValPtr) = data->modifyArrows.currValue;
            ReloadPokemonSprites(data);
            PlaySE(SE_DEX_SCROLL);
            while (!(gMain.intrCheck & INTR_FLAG_VBLANK));
        }
        else if (gBaseStats[data->currentMonId].forms[0])
        {
            struct DebugModifyArrows *modArrows = &data->modifyArrows;
            u8 form = GetForm(data->currentMonId);

            if (increment)
            {
                if (gBaseStats[data->currentMonId].forms[form + 1])
                    modArrows->currValue = GetFormSpecies(data->currentMonId, form + 1);
                else
                    modArrows->currValue = gBaseStats[data->currentMonId].forms[0];
            }
            else
            {
                if (data->currentMonId != gBaseStats[data->currentMonId].forms[0])
                    modArrows->currValue = GetFormSpecies(data->currentMonId, form - 1);
                else
                    modArrows->currValue = gBaseStats[data->currentMonId].forms[gBaseStats[data->currentMonId].formCount];
            }

            *(u16*)(data->modifyArrows.modifiedValPtr) = data->modifyArrows.currValue;
            ReloadPokemonSprites(data);
            data->animIdBack = gBaseStats[modArrows->currValue].backPicAnim;
            data->animIdFront = gBaseStats[modArrows->currValue].frontPicAnim;
            UpdateMonAnimNames(taskId);
            PlaySE(SE_DEX_SCROLL);
            while (!(gMain.intrCheck & INTR_FLAG_VBLANK));
        }
    default:
        break;
    }
}

static void HandleInputPokemonDebug(u8 taskId)
{
    struct PokemonDebugMenu *data = GetStructPtr(taskId);
    struct Sprite *FrontSprite = &gSprites[data->frontSpriteId];
    struct Sprite *BackSprite = &gSprites[data->backSpriteId];

    if (JOY_NEW(L_BUTTON) && (BackSprite->callback == SpriteCallbackDummy))
    {
        PlayCryInternal(data->currentMonId, 0, 120, 10, 0);
        LaunchAnimationTaskForBackSprite(BackSprite, data->animIdBack);
    }
    if (JOY_NEW(R_BUTTON) && (FrontSprite->callback == SpriteCallbackDummy))
    {
        PlayCryInternal(data->currentMonId, 0, 120, 10, 0);
        if (data->currentMonId != SPECIES_CASTFORM)
            StartSpriteAnim(FrontSprite, 1);
        LaunchAnimationTaskForFrontSprite(FrontSprite, data->animIdFront);
    }
    if (JOY_NEW(START_BUTTON))
    {
        data->isShiny = !data->isShiny;
        if (data->isShiny)
            PlaySE(SE_SHINY);
        ReloadPokemonSprites(data);
    }
    if (JOY_NEW(SELECT_BUTTON) && (gBaseStats[data->modifyArrows.currValue].flags & FLAG_GENDER_DIFFERENCE))
    {
        data->isFemale = !data->isFemale;
        PrintDigitChars(data);
        *(u16*)(data->modifyArrows.modifiedValPtr) = data->modifyArrows.currValue;
        ReloadPokemonSprites(data);
        PlaySE(SE_DEX_PAGE);
        while (!(gMain.intrCheck & INTR_FLAG_VBLANK));
    }
    if (data->inSubmenu) // Submenu
    {
        if (JOY_NEW(B_BUTTON))
        {
            data->inSubmenu = FALSE;
            if (data->submenuYpos == 3)
            {
                data->submenuYpos = 2;
                data->optionArrows.currentDigit = data->submenuYpos;
                gSprites[data->optionArrows.arrowSpriteId[0]].y = OPTIONS_ARROW_Y + data->optionArrows.currentDigit * 12;
            }
            SetArrowInvisibility(data);
            PrintInstructionsOnWindow(data);
        }
        else if (JOY_NEW(DPAD_DOWN))
        {
            data->submenuYpos += 1;
            if (data->submenuYpos >= 3)
            {
                if ((data->currentMonId != SPECIES_ARBOK) || (data->currentMonId != SPECIES_UNOWN) || (!gBaseStats[data->currentMonId].formCount) || (data->submenuYpos >= 4))
                    data->submenuYpos = 0;
            }
            data->optionArrows.currentDigit = data->submenuYpos;
            gSprites[data->optionArrows.arrowSpriteId[0]].y = OPTIONS_ARROW_Y + data->optionArrows.currentDigit * 12;
        }
        else if (JOY_NEW(DPAD_UP))
        {
            if (data->submenuYpos)
                data->submenuYpos -= 1;
            else
            {
                if ((data->currentMonId == SPECIES_ARBOK) || (data->currentMonId == SPECIES_UNOWN) || (gBaseStats[data->currentMonId].formCount))
                    data->submenuYpos = 3;
                else
                    data->submenuYpos = 2;
            }

            data->optionArrows.currentDigit = data->submenuYpos;
            gSprites[data->optionArrows.arrowSpriteId[0]].y = OPTIONS_ARROW_Y + data->optionArrows.currentDigit * 12;
        }
        else if (JOY_NEW(DPAD_LEFT))
            UpdateSubmenuOptionValue(taskId, FALSE);
        else if (JOY_NEW(DPAD_RIGHT))
            UpdateSubmenuOptionValue(taskId, TRUE);
    }
    else
    {
        if (JOY_NEW(A_BUTTON))
        {
            data->inSubmenu = TRUE;
            SetArrowInvisibility(data);
            PrintInstructionsOnWindow(data);
        }
        else if (JOY_NEW(B_BUTTON))
        {
            BeginNormalPaletteFade(PALETTES_ALL, 0, 0, 16, RGB_BLACK);
            gTasks[taskId].func = ExitPokemonDebug;
            PlaySE(SE_PC_OFF);
        }
        else if ((JOY_NEW(DPAD_DOWN) && TryMoveDigit(&data->modifyArrows, FALSE))
         || (JOY_NEW(DPAD_UP) && TryMoveDigit(&data->modifyArrows, TRUE)))
        {
            data->isFemale = FALSE;
            data->currentMonForm = 0;
            PrintDigitChars(data);
            *(u16*)(data->modifyArrows.modifiedValPtr) = data->modifyArrows.currValue;
            ReloadPokemonSprites(data);
            data->animIdBack = gBaseStats[data->currentMonId].backPicAnim;
            data->animIdFront = gBaseStats[data->currentMonId].frontPicAnim;
            UpdateMonAnimNames(taskId);
            PlaySE(SE_DEX_SCROLL);
            while (!(gMain.intrCheck & INTR_FLAG_VBLANK));
        }
        else if (JOY_NEW(DPAD_LEFT))
        {
            if (data->modifyArrows.currentDigit)
            {
                data->modifyArrows.currentDigit--;
                gSprites[data->modifyArrows.arrowSpriteId[0]].x2 -= 6;
                gSprites[data->modifyArrows.arrowSpriteId[1]].x2 -= 6;
            }
        }
        else if (JOY_NEW(DPAD_RIGHT))
        {
            if (data->modifyArrows.currentDigit != (data->modifyArrows.maxDigits - 1))
            {
                data->modifyArrows.currentDigit++;
                gSprites[data->modifyArrows.arrowSpriteId[0]].x2 += 6;
                gSprites[data->modifyArrows.arrowSpriteId[1]].x2 += 6;
            }
        }
    }
}

static void ReloadPokemonSprites(struct PokemonDebugMenu *data)
{
    const struct SpritePalette *palette;
    u16 species = data->currentMonId;
    u8 form = data->currentMonForm;
    u16 coordSpecies = species;

    DestroySprite(&gSprites[data->frontSpriteId]);
    DestroySprite(&gSprites[data->backSpriteId]);
    DestroySprite(&gSprites[data->iconSpriteId]);

    FreeMonSpritesGfx();
    ResetSpriteData();
    ResetPaletteFadeControl();
    FreeAllSpritePalettes();
    ResetAllPicSprites();
    FreeMonIconPalettes();

    AllocateMonSpritesGfx();

    if (form)
    {
        if (species == SPECIES_UNOWN)
        {
            // The other Unowns are separate from Unown A.
            coordSpecies = NUM_SPECIES + form;
        }
    }

    LoadMonIconPalette(coordSpecies);

    // Update instructions
    PrintInstructionsOnWindow(data);

    // Front
    HandleLoadSpecialPokePic_Debug(&gMonFrontPicTable[species], gMonSpritesGfxPtr->sprites.ptr[1], species, data->isFemale, form);
    BattleLoadMonSpriteGfx_Debug(coordSpecies, data->isShiny);
    SetMultiuseSpriteTemplateToPokemon(species, 1, 0);
    data->frontSpriteId = CreateSprite(&gMultiuseSpriteTemplate, DEBUG_MON_X, DEBUG_MON_Y + gMonFrontPicCoords[coordSpecies].y_offset, 0);
    gSprites[data->frontSpriteId].oam.paletteNum = 5;
    gSprites[data->frontSpriteId].callback = SpriteCallbackDummy;
    gSprites[data->frontSpriteId].oam.priority = 0;

    // Back
    HandleLoadSpecialPokePic_Debug(&gMonBackPicTable[species], gMonSpritesGfxPtr->sprites.ptr[2], species, data->isFemale, form);
    palette = GetMonSpritePalStruct_Debug(coordSpecies, data->isShiny);
    LoadSpritePalette(palette);
    SetMultiuseSpriteTemplateToPokemon(species, 2, 0);
    gMultiuseSpriteTemplate.paletteTag = palette->tag;
    data->backSpriteId = CreateSprite(&gMultiuseSpriteTemplate, DEBUG_MON_BACK_X, DEBUG_MON_BACK_Y + gMonBackPicCoords[coordSpecies].y_offset, 0);
    gSprites[data->backSpriteId].callback = SpriteCallbackDummy;
    gSprites[data->backSpriteId].oam.priority = 0;

    // Icon sprite
    data->iconSpriteId = CreateMonIconNoPersonality(coordSpecies, SpriteCB_MonIcon, DEBUG_ICON_X, DEBUG_ICON_Y, 4);
    gSprites[data->iconSpriteId].oam.priority = 0;

    // Modify arrows
    LoadSpritePalette(&gSpritePalette_Arrow);
    data->modifyArrows.arrowSpriteId[0] = CreateSprite(&gSpriteTemplate_Arrow, MODIFY_DIGITS_ARROW_X + (data->modifyArrows.currentDigit * 6), MODIFY_DIGITS_ARROW1_Y, 0);
    data->modifyArrows.arrowSpriteId[1] = CreateSprite(&gSpriteTemplate_Arrow, MODIFY_DIGITS_ARROW_X + (data->modifyArrows.currentDigit * 6), MODIFY_DIGITS_ARROW2_Y, 0);
    gSprites[data->modifyArrows.arrowSpriteId[1]].animNum = 1;

    // Option Arrows
    LoadSpritePalette(&gSpritePalette_Arrow);
    data->optionArrows.arrowSpriteId[0] = CreateSprite(&gSpriteTemplate_Arrow, OPTIONS_ARROW_1_X, OPTIONS_ARROW_Y + data->optionArrows.currentDigit * 12, 0);
    gSprites[data->optionArrows.arrowSpriteId[0]].animNum = 2;

    // Arrow invisibility
    SetArrowInvisibility(data);
}

static void ExitPokemonDebug(u8 taskId)
{
    if (!gPaletteFade.active)
    {
        struct PokemonDebugMenu *data = GetStructPtr(taskId);
        Free(data);
        FreeMonSpritesGfx();
        DestroyTask(taskId);
        SetMainCallback2(CB2_ReturnToFieldWithOpenMenu);
        m4aMPlayVolumeControl(&gMPlayInfo_BGM, 0xFFFF, 0x100);
    }
}
