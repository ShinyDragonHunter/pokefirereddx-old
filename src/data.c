#include "global.h"
#include "malloc.h"
#include "battle.h"
#include "data.h"
#include "graphics.h"
#include "constants/items.h"
#include "constants/moves.h"
#include "constants/trainers.h"
#include "constants/battle_ai.h"

const u16 gMinigameDigits_Pal[] = INCBIN_U16("graphics/link/minigame_digits.gbapal");
const u32 gMinigameDigits_Gfx[] = INCBIN_U32("graphics/link/minigame_digits.4bpp.lz");

#define BATTLER_OFFSET(i) (gHeap + 0x8000 + MON_PIC_SIZE * (i))

const struct SpriteFrameImage gBattlerPicTable_PlayerLeft[] =
{
    BATTLER_OFFSET(0), MON_PIC_SIZE,
    BATTLER_OFFSET(1), MON_PIC_SIZE,
    BATTLER_OFFSET(2), MON_PIC_SIZE,
    BATTLER_OFFSET(3), MON_PIC_SIZE,
};

const struct SpriteFrameImage gBattlerPicTable_OpponentLeft[] =
{
    BATTLER_OFFSET(4), MON_PIC_SIZE,
    BATTLER_OFFSET(5), MON_PIC_SIZE,
    BATTLER_OFFSET(6), MON_PIC_SIZE,
    BATTLER_OFFSET(7), MON_PIC_SIZE,
};

const struct SpriteFrameImage gBattlerPicTable_PlayerRight[] =
{
    BATTLER_OFFSET(8),  MON_PIC_SIZE,
    BATTLER_OFFSET(9),  MON_PIC_SIZE,
    BATTLER_OFFSET(10), MON_PIC_SIZE,
    BATTLER_OFFSET(11), MON_PIC_SIZE,
};

const struct SpriteFrameImage gBattlerPicTable_OpponentRight[] =
{
    BATTLER_OFFSET(12), MON_PIC_SIZE,
    BATTLER_OFFSET(13), MON_PIC_SIZE,
    BATTLER_OFFSET(14), MON_PIC_SIZE,
    BATTLER_OFFSET(15), MON_PIC_SIZE,
};

const struct SpriteFrameImage gTrainerBackPicTable_Red[] =
{
    gTrainerBackPicRed, 0x0800,
    gTrainerBackPicRed + 0x0800, 0x0800,
    gTrainerBackPicRed + 0x1000, 0x0800,
    gTrainerBackPicRed + 0x1800, 0x0800,
    gTrainerBackPicRed + 0x2000, 0x0800,
};

const struct SpriteFrameImage gTrainerBackPicTable_RedDX[] =
{
    gTrainerBackPicRedDX, 0x0800,
    gTrainerBackPicRedDX + 0x0800, 0x0800,
    gTrainerBackPicRedDX + 0x1000, 0x0800,
    gTrainerBackPicRedDX + 0x1800, 0x0800,
    gTrainerBackPicRedDX + 0x2000, 0x0800,
};

const struct SpriteFrameImage gTrainerBackPicTable_RedClassic[] =
{
    gTrainerBackPicRedClassic, 0x0800,
    gTrainerBackPicRedClassic + 0x0800, 0x0800,
    gTrainerBackPicRedClassic + 0x1000, 0x0800,
    gTrainerBackPicRedClassic + 0x1800, 0x0800,
    gTrainerBackPicRedClassic + 0x2000, 0x0800,
};

const struct SpriteFrameImage gTrainerBackPicTable_RedAlola[] =
{
    gTrainerBackPicRedAlola, 0x0800,
    gTrainerBackPicRedAlola + 0x0800, 0x0800,
    gTrainerBackPicRedAlola + 0x1000, 0x0800,
    gTrainerBackPicRedAlola + 0x1800, 0x0800,
    gTrainerBackPicRedAlola + 0x2000, 0x0800,
};

const struct SpriteFrameImage gTrainerBackPicTable_RedSygnaSuit[] =
{
    gTrainerBackPicRedSygnaSuit, 0x0800,
    gTrainerBackPicRedSygnaSuit + 0x0800, 0x0800,
    gTrainerBackPicRedSygnaSuit + 0x1000, 0x0800,
    gTrainerBackPicRedSygnaSuit + 0x1800, 0x0800,
    gTrainerBackPicRedSygnaSuit + 0x2000, 0x0800,
};

const struct SpriteFrameImage gTrainerBackPicTable_RedTeamRocket[] =
{
    gTrainerBackPicRedTeamRocket, 0x0800,
    gTrainerBackPicRedTeamRocket + 0x0800, 0x0800,
    gTrainerBackPicRedTeamRocket + 0x1000, 0x0800,
    gTrainerBackPicRedTeamRocket + 0x1800, 0x0800,
    gTrainerBackPicRedTeamRocket + 0x2000, 0x0800,
};

const struct SpriteFrameImage gTrainerBackPicTable_RedTeamAqua[] =
{
    gTrainerBackPicRedTeamAqua, 0x0800,
    gTrainerBackPicRedTeamAqua + 0x0800, 0x0800,
    gTrainerBackPicRedTeamAqua + 0x1000, 0x0800,
    gTrainerBackPicRedTeamAqua + 0x1800, 0x0800,
    gTrainerBackPicRedTeamAqua + 0x2000, 0x0800,
};

const struct SpriteFrameImage gTrainerBackPicTable_RedTeamMagma[] =
{
    gTrainerBackPicRedTeamMagma, 0x0800,
    gTrainerBackPicRedTeamMagma + 0x0800, 0x0800,
    gTrainerBackPicRedTeamMagma + 0x1000, 0x0800,
    gTrainerBackPicRedTeamMagma + 0x1800, 0x0800,
    gTrainerBackPicRedTeamMagma + 0x2000, 0x0800,
};

const struct SpriteFrameImage gTrainerBackPicTable_Leaf[] =
{
    gTrainerBackPicLeaf, 0x0800,
    gTrainerBackPicLeaf + 0x0800, 0x0800,
    gTrainerBackPicLeaf + 0x1000, 0x0800,
    gTrainerBackPicLeaf + 0x1800, 0x0800,
    gTrainerBackPicLeaf + 0x2000, 0x0800,
};

const struct SpriteFrameImage gTrainerBackPicTable_LeafDX[] =
{
    gTrainerBackPicLeafDX, 0x0800,
    gTrainerBackPicLeafDX + 0x0800, 0x0800,
    gTrainerBackPicLeafDX + 0x1000, 0x0800,
    gTrainerBackPicLeafDX + 0x1800, 0x0800,
    gTrainerBackPicLeafDX + 0x2000, 0x0800,
};

const struct SpriteFrameImage gTrainerBackPicTable_LeafClassic[] =
{
    gTrainerBackPicLeafClassic, 0x0800,
    gTrainerBackPicLeafClassic + 0x0800, 0x0800,
    gTrainerBackPicLeafClassic + 0x1000, 0x0800,
    gTrainerBackPicLeafClassic + 0x1800, 0x0800,
    gTrainerBackPicLeafClassic + 0x2000, 0x0800,
};

const struct SpriteFrameImage gTrainerBackPicTable_LeafAlola[] =
{
    gTrainerBackPicLeafAlola, 0x0800,
    gTrainerBackPicLeafAlola + 0x0800, 0x0800,
    gTrainerBackPicLeafAlola + 0x1000, 0x0800,
    gTrainerBackPicLeafAlola + 0x1800, 0x0800,
    gTrainerBackPicLeafAlola + 0x2000, 0x0800,
};

const struct SpriteFrameImage gTrainerBackPicTable_LeafSygnaSuit[] =
{
    gTrainerBackPicLeafSygnaSuit, 0x0800,
    gTrainerBackPicLeafSygnaSuit + 0x0800, 0x0800,
    gTrainerBackPicLeafSygnaSuit + 0x1000, 0x0800,
    gTrainerBackPicLeafSygnaSuit + 0x1800, 0x0800,
    gTrainerBackPicLeafSygnaSuit + 0x2000, 0x0800,
};

const struct SpriteFrameImage gTrainerBackPicTable_LeafTeamRocket[] =
{
    gTrainerBackPicLeafTeamRocket, 0x0800,
    gTrainerBackPicLeafTeamRocket + 0x0800, 0x0800,
    gTrainerBackPicLeafTeamRocket + 0x1000, 0x0800,
    gTrainerBackPicLeafTeamRocket + 0x1800, 0x0800,
    gTrainerBackPicLeafTeamRocket + 0x2000, 0x0800,
};

const struct SpriteFrameImage gTrainerBackPicTable_LeafTeamAqua[] =
{
    gTrainerBackPicLeafTeamAqua, 0x0800,
    gTrainerBackPicLeafTeamAqua + 0x0800, 0x0800,
    gTrainerBackPicLeafTeamAqua + 0x1000, 0x0800,
    gTrainerBackPicLeafTeamAqua + 0x1800, 0x0800,
    gTrainerBackPicLeafTeamAqua + 0x2000, 0x0800,
};

const struct SpriteFrameImage gTrainerBackPicTable_LeafTeamMagma[] =
{
    gTrainerBackPicLeafTeamMagma, 0x0800,
    gTrainerBackPicLeafTeamMagma + 0x0800, 0x0800,
    gTrainerBackPicLeafTeamMagma + 0x1000, 0x0800,
    gTrainerBackPicLeafTeamMagma + 0x1800, 0x0800,
    gTrainerBackPicLeafTeamMagma + 0x2000, 0x0800,
};

const struct SpriteFrameImage gTrainerBackPicTable_PokeDude[] =
{
    gTrainerBackPicPokeDude, 0x0800,
    gTrainerBackPicPokeDude + 0x0800, 0x0800,
    gTrainerBackPicPokeDude + 0x1000, 0x0800,
    gTrainerBackPicPokeDude + 0x1800, 0x0800,
};

const struct SpriteFrameImage gTrainerBackPicTable_OldMan[] =
{
    gTrainerBackPicOldMan, 0x0800,
    gTrainerBackPicOldMan + 0x0800, 0x0800,
    gTrainerBackPicOldMan + 0x1000, 0x0800,
    gTrainerBackPicOldMan + 0x1800, 0x0800,
};

const struct SpriteFrameImage gTrainerBackPicTable_Gold[] =
{
    gTrainerBackPicGold, 0x0800,
    gTrainerBackPicGold + 0x0800, 0x0800,
    gTrainerBackPicGold + 0x1000, 0x0800,
    gTrainerBackPicGold + 0x1800, 0x0800,
    gTrainerBackPicGold + 0x2000, 0x0800,
};

const struct SpriteFrameImage gTrainerBackPicTable_Kris[] =
{
    gTrainerBackPicKris, 0x0800,
    gTrainerBackPicKris + 0x0800, 0x0800,
    gTrainerBackPicKris + 0x1000, 0x0800,
    gTrainerBackPicKris + 0x1800, 0x0800,
    gTrainerBackPicKris + 0x2000, 0x0800,
};

const struct SpriteFrameImage gTrainerBackPicTable_EBrendan[] =
{
    gTrainerBackPicEBrendan, 0x0800,
    gTrainerBackPicEBrendan + 0x0800, 0x0800,
    gTrainerBackPicEBrendan + 0x1000, 0x0800,
    gTrainerBackPicEBrendan + 0x1800, 0x0800,
};

const struct SpriteFrameImage gTrainerBackPicTable_EMay[] =
{
    gTrainerBackPicEMay, 0x0800,
    gTrainerBackPicEMay + 0x0800, 0x0800,
    gTrainerBackPicEMay + 0x1000, 0x0800,
    gTrainerBackPicEMay + 0x1800, 0x0800,
};

const struct SpriteFrameImage gTrainerBackPicTable_RedOriginal[] =
{
    gTrainerBackPicRed, 0x0800,
    gTrainerBackPicRed + 0x0800, 0x0800,
    gTrainerBackPicRedOriginal, 0x0800,
    gTrainerBackPicRed + 0x1800, 0x0800,
    gTrainerBackPicRedOriginal + 0x0800, 0x0800,
};

const struct SpriteFrameImage gTrainerBackPicTable_LeafOriginal[] =
{
    gTrainerBackPicLeafOriginal, 0x0800,
    gTrainerBackPicLeafOriginal + 0x0800, 0x0800,
    gTrainerBackPicLeafOriginal + 0x1000, 0x0800,
    gTrainerBackPicLeafOriginal + 0x1800, 0x0800,
    gTrainerBackPicLeafOriginal + 0x2000, 0x0800,
};

const struct SpriteFrameImage gTrainerBackPicTable_RSBrendan[] =
{
    gTrainerBackPicRSBrendan, 0x0800,
    gTrainerBackPicRSBrendan + 0x0800, 0x0800,
    gTrainerBackPicRSBrendan + 0x1000, 0x0800,
    gTrainerBackPicRSBrendan + 0x1800, 0x0800,
};

const struct SpriteFrameImage gTrainerBackPicTable_RSMay[] =
{
    gTrainerBackPicRSMay, 0x0800,
    gTrainerBackPicRSMay + 0x0800, 0x0800,
    gTrainerBackPicRSMay + 0x1000, 0x0800,
    gTrainerBackPicRSMay + 0x1800, 0x0800,
};

static const union AnimCmd sAnim_GeneralFrame0[] =
{
    ANIMCMD_FRAME(0, 0),
    ANIMCMD_END,
};

static const union AnimCmd sAnim_GeneralFrame3[] =
{
    ANIMCMD_FRAME(3, 0),
    ANIMCMD_END,
};

// Many of these affine anims seem to go unused, and
// instead SetSpriteRotScale is used to manipulate
// the battler sprites directly (for instance, in AnimTask_SwitchOutShrinkMon).
// Those with explicit indexes are referenced elsewhere.

static const union AffineAnimCmd sAffineAnim_Battler_Normal[] =
{
    AFFINEANIMCMD_FRAME(0x100, 0x100, 0, 0),
    AFFINEANIMCMD_END,
};

static const union AffineAnimCmd sAffineAnim_Battler_Flipped[] =
{
    AFFINEANIMCMD_FRAME(-0x100, 0x100, 0, 0),
    AFFINEANIMCMD_END,
};

static const union AffineAnimCmd sAffineAnim_Battler_Emerge[] =
{
    AFFINEANIMCMD_FRAME(0x28, 0x28, 0, 0),
    AFFINEANIMCMD_FRAME(0x12, 0x12, 0, 12),
    AFFINEANIMCMD_END,
};

static const union AffineAnimCmd sAffineAnim_Battler_Return[] =
{
    AFFINEANIMCMD_FRAME( -0x2,  -0x2, 0, 18),
    AFFINEANIMCMD_FRAME(-0x10, -0x10, 0, 15),
    AFFINEANIMCMD_END,
};

static const union AffineAnimCmd sAffineAnim_Battler_HorizontalSquishLoop[] =
{
    AFFINEANIMCMD_FRAME(0xA0, 0x100, 0, 0),
    AFFINEANIMCMD_FRAME( 0x4,   0x0, 0, 8),
    AFFINEANIMCMD_FRAME(-0x4,   0x0, 0, 8),
    AFFINEANIMCMD_JUMP(1),
};

static const union AffineAnimCmd sAffineAnim_Battler_Grow[] =
{
    AFFINEANIMCMD_FRAME(0x2, 0x2, 0, 20),
    AFFINEANIMCMD_END,
};

static const union AffineAnimCmd sAffineAnim_Battler_Shrink[] =
{
    AFFINEANIMCMD_FRAME(-0x2, -0x2, 0, 20),
    AFFINEANIMCMD_END,
};

static const union AffineAnimCmd sAffineAnim_Battler_BigToSmall[] =
{
    AFFINEANIMCMD_FRAME(0x100, 0x100, 0, 0),
    AFFINEANIMCMD_FRAME(-0x10, -0x10, 0, 9),
    AFFINEANIMCMD_END,
};

static const union AffineAnimCmd sAffineAnim_Battler_GrowLarge[] =
{
    AFFINEANIMCMD_FRAME(0x4, 0x4, 0, 63),
    AFFINEANIMCMD_END,
};

static const union AffineAnimCmd sAffineAnim_Battler_TipRight[] =
{
    AFFINEANIMCMD_FRAME(0x0, 0x0, -3, 5),
    AFFINEANIMCMD_FRAME(0x0, 0x0,  3, 5),
    AFFINEANIMCMD_END,
};

const union AffineAnimCmd *const gAffineAnims_BattleSpritePlayerSide[] =
{
    [BATTLER_AFFINE_NORMAL] = sAffineAnim_Battler_Normal,
    [BATTLER_AFFINE_EMERGE] = sAffineAnim_Battler_Emerge,
    [BATTLER_AFFINE_RETURN] = sAffineAnim_Battler_Return,
    sAffineAnim_Battler_HorizontalSquishLoop,
    sAffineAnim_Battler_Grow,
    sAffineAnim_Battler_Shrink,
    sAffineAnim_Battler_GrowLarge,
    sAffineAnim_Battler_TipRight,
    sAffineAnim_Battler_BigToSmall,
};

static const union AffineAnimCmd sAffineAnim_Battler_SpinShrink[] =
{
    AFFINEANIMCMD_FRAME(-0x4, -0x4, 4, 63),
    AFFINEANIMCMD_END,
};

static const union AffineAnimCmd sAffineAnim_Battler_TipLeft[] =
{
    AFFINEANIMCMD_FRAME(0x0, 0x0,  3, 5),
    AFFINEANIMCMD_FRAME(0x0, 0x0, -3, 5),
    AFFINEANIMCMD_END,
};

static const union AffineAnimCmd sAffineAnim_Battler_RotateUpAndBack[] =
{
    AFFINEANIMCMD_FRAME(0x0, 0x0, -5, 20),
    AFFINEANIMCMD_FRAME(0x0, 0x0,  0, 20),
    AFFINEANIMCMD_FRAME(0x0, 0x0,  5, 20),
    AFFINEANIMCMD_END,
};

static const union AffineAnimCmd sAffineAnim_Battler_Spin[] =
{
    AFFINEANIMCMD_FRAME(0x0, 0x0, 9, 110),
    AFFINEANIMCMD_END,
};

const union AffineAnimCmd *const gAffineAnims_BattleSpriteOpponentSide[] =
{
    [BATTLER_AFFINE_NORMAL] = sAffineAnim_Battler_Normal,
    [BATTLER_AFFINE_EMERGE] = sAffineAnim_Battler_Emerge,
    [BATTLER_AFFINE_RETURN] = sAffineAnim_Battler_Return,
    sAffineAnim_Battler_HorizontalSquishLoop,
    sAffineAnim_Battler_Grow,
    sAffineAnim_Battler_Shrink,
    sAffineAnim_Battler_SpinShrink,
    sAffineAnim_Battler_TipLeft,
    sAffineAnim_Battler_RotateUpAndBack,
    sAffineAnim_Battler_BigToSmall,
    sAffineAnim_Battler_Spin,
};

static const union AnimCmd sAnim_MonPic_1[] =
{
    ANIMCMD_FRAME(1, 0),
    ANIMCMD_END,
};

static const union AnimCmd sAnim_MonPic_2[] =
{
    ANIMCMD_FRAME(2, 0),
    ANIMCMD_END,
};

const union AnimCmd *const gAnims_MonPic[] =
{
    sAnim_GeneralFrame0,
    sAnim_MonPic_1,
    sAnim_MonPic_2,
    sAnim_GeneralFrame3,
};

#define SPECIES_SPRITE(species, sprite) [SPECIES_##species] = {sprite, MON_PIC_SIZE, SPECIES_##species}
#define SPECIES_PAL(species, pal) [SPECIES_##species] = {pal, SPECIES_##species}
#define SPECIES_SHINY_PAL(species, pal) [SPECIES_##species] = {pal, SPECIES_##species + SPECIES_SHINY_TAG}

#include "data/pokemon_graphics/front_pic_coordinates.h"
#include "data/pokemon_graphics/front_pic_table.h"
#include "data/pokemon_graphics/back_pic_coordinates.h"
#include "data/pokemon_graphics/back_pic_table.h"
#include "data/pokemon_graphics/palette_table.h"
#include "data/pokemon_graphics/shiny_palette_table.h"

#include "data/trainer_graphics/front_pic_anims.h"
#include "data/trainer_graphics/front_pic_tables.h"
#include "data/trainer_graphics/back_pic_anims.h"
#include "data/trainer_graphics/back_pic_tables.h"

#include "data/pokemon_graphics/enemy_mon_elevation.h"
#include "data/pokemon_graphics/front_pic_anims.h"

const bool8 SpeciesHasGenderDifference[SPECIES_COUNT] =
{
    [SPECIES_VENUSAUR] = TRUE,
    [SPECIES_BUTTERFREE] = TRUE,
    [SPECIES_RATTATA] = TRUE,
    [SPECIES_RATICATE] = TRUE,
    [SPECIES_PIKACHU] = TRUE,
    [SPECIES_RAICHU] = TRUE,
    [SPECIES_ZUBAT] = TRUE,
    [SPECIES_GOLBAT] = TRUE,
    [SPECIES_GLOOM] = TRUE,
    [SPECIES_VILEPLUME] = TRUE,
    [SPECIES_KADABRA] = TRUE,
    [SPECIES_ALAKAZAM] = TRUE,
    [SPECIES_DODUO] = TRUE,
    [SPECIES_DODRIO] = TRUE,
    [SPECIES_HYPNO] = TRUE,
    [SPECIES_RHYHORN] = TRUE,
    [SPECIES_RHYDON] = TRUE,
    [SPECIES_GOLDEEN] = TRUE,
    [SPECIES_SEAKING] = TRUE,
    [SPECIES_SCYTHER] = TRUE,
    [SPECIES_MAGIKARP] = TRUE,
    [SPECIES_GYARADOS] = TRUE,
    [SPECIES_EEVEE] = TRUE,
    [SPECIES_MEGANIUM] = TRUE,
    [SPECIES_LEDYBA] = TRUE,
    [SPECIES_LEDIAN] = TRUE,
    [SPECIES_SUDOWOODO] = TRUE,
    [SPECIES_POLITOED] = TRUE,
    [SPECIES_AIPOM] = TRUE,
    [SPECIES_WOOPER] = TRUE,
    [SPECIES_QUAGSIRE] = TRUE,
    [SPECIES_MURKROW] = TRUE,
    [SPECIES_WOBBUFFET] = TRUE,
    [SPECIES_GIRAFARIG] = TRUE,
    [SPECIES_GLIGAR] = TRUE,
    [SPECIES_STEELIX] = TRUE,
    [SPECIES_SCIZOR] = TRUE,
    [SPECIES_HERACROSS] = TRUE,
    [SPECIES_SNEASEL] = TRUE,
    [SPECIES_URSARING] = TRUE,
    [SPECIES_PILOSWINE] = TRUE,
    [SPECIES_OCTILLERY] = TRUE,
    [SPECIES_HOUNDOOM] = TRUE,
    [SPECIES_DONPHAN] = TRUE,
    [SPECIES_TORCHIC] = TRUE,
    [SPECIES_COMBUSKEN] = TRUE,
    [SPECIES_BLAZIKEN] = TRUE,
    [SPECIES_BEAUTIFLY] = TRUE,
    [SPECIES_DUSTOX] = TRUE,
    [SPECIES_LUDICOLO] = TRUE,
    [SPECIES_NUZLEAF] = TRUE,
    [SPECIES_SHIFTRY] = TRUE,
    [SPECIES_MILOTIC] = TRUE,
    [SPECIES_NUMEL] = TRUE,
    [SPECIES_CAMERUPT] = TRUE,
    [SPECIES_CACTURNE] = TRUE,
    [SPECIES_MEDITITE] = TRUE,
    [SPECIES_MEDICHAM] = TRUE,
    [SPECIES_ROSELIA] = TRUE,
    [SPECIES_GULPIN] = TRUE,
    [SPECIES_SWALOT] = TRUE,
    [SPECIES_RELICANTH] = TRUE,
};

#include "data/trainer_parties.h"
#include "data/text/trainer_class_names.h"
#include "data/trainers.h"
#include "data/text/japanese_species_names.h"
#include "data/text/species_names.h"
#include "data/text/german_species_names.h"
#include "data/text/move_names.h"
