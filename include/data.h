#ifndef GUARD_DATA_H
#define GUARD_DATA_H

#include "constants/moves.h"

#define SPECIES_SHINY_TAG 500

enum {
    BATTLER_AFFINE_NORMAL,
    BATTLER_AFFINE_EMERGE,
    BATTLER_AFFINE_RETURN,
};

struct MonCoords
{
    // This would use a bitfield, but some function
    // uses it as a u8 and casting won't match.
    u8 size; // u8 width:4, height:4;
    u8 y_offset;
};

struct TrainerMonNoItemDefaultMoves
{
    u16 iv;
    u8 lvl;
    u16 species;
};

struct TrainerMonItemDefaultMoves
{
    u16 iv;
    u8 lvl;
    u16 species;
    u16 heldItem;
};

struct TrainerMonNoItemCustomMoves
{
    u16 iv;
    u8 lvl;
    u16 species;
    u16 moves[MAX_MON_MOVES];
};

struct TrainerMonItemCustomMoves
{
    u16 iv;
    u8 lvl;
    u16 species;
    u16 heldItem;
    u16 moves[MAX_MON_MOVES];
};

union TrainerMonPtr
{
    const struct TrainerMonNoItemDefaultMoves *NoItemDefaultMoves;
    const struct TrainerMonNoItemCustomMoves *NoItemCustomMoves;
    const struct TrainerMonItemDefaultMoves *ItemDefaultMoves;
    const struct TrainerMonItemCustomMoves *ItemCustomMoves;
};

struct Trainer
{
    /*0x00*/ u8 partyFlags;
    /*0x01*/ u8 trainerClass;
    /*0x02*/ u8 encounterMusic_gender; // last bit is gender
    /*0x03*/ u8 trainerPic;
    /*0x04*/ u8 trainerName[12];
    /*0x10*/ u16 items[4];
    /*0x18*/ bool8 doubleBattle;
    /*0x1C*/ u32 aiFlags;
    /*0x20*/ u8 partySize;
    /*0x24*/ union TrainerMonPtr party;
};

#define TRAINER_ENCOUNTER_MUSIC(trainer)((gTrainers[trainer].encounterMusic_gender & 0x7F))

extern const u16 gMinigameDigits_Pal[];
extern const u32 gMinigameDigits_Gfx[];

extern const struct SpriteFrameImage gBattlerPicTable_PlayerLeft[];
extern const struct SpriteFrameImage gBattlerPicTable_OpponentLeft[];
extern const struct SpriteFrameImage gBattlerPicTable_PlayerRight[];
extern const struct SpriteFrameImage gBattlerPicTable_OpponentRight[];
extern const struct SpriteFrameImage gTrainerBackPicTable_Red[];
extern const struct SpriteFrameImage gTrainerBackPicTable_RedDX[];
extern const struct SpriteFrameImage gTrainerBackPicTable_RedClassic[];
extern const struct SpriteFrameImage gTrainerBackPicTable_RedAlola[];
extern const struct SpriteFrameImage gTrainerBackPicTable_RedSygnaSuit[];
extern const struct SpriteFrameImage gTrainerBackPicTable_RedTeamRocket[];
extern const struct SpriteFrameImage gTrainerBackPicTable_RedTeamAqua[];
extern const struct SpriteFrameImage gTrainerBackPicTable_RedTeamMagma[];
extern const struct SpriteFrameImage gTrainerBackPicTable_Leaf[];
extern const struct SpriteFrameImage gTrainerBackPicTable_LeafDX[];
extern const struct SpriteFrameImage gTrainerBackPicTable_LeafClassic[];
extern const struct SpriteFrameImage gTrainerBackPicTable_LeafAlola[];
extern const struct SpriteFrameImage gTrainerBackPicTable_LeafSygnaSuit[];
extern const struct SpriteFrameImage gTrainerBackPicTable_LeafTeamRocket[];
extern const struct SpriteFrameImage gTrainerBackPicTable_LeafTeamAqua[];
extern const struct SpriteFrameImage gTrainerBackPicTable_LeafTeamMagma[];
extern const struct SpriteFrameImage gTrainerBackPicTable_PokeDude[];
extern const struct SpriteFrameImage gTrainerBackPicTable_OldMan[];
extern const struct SpriteFrameImage gTrainerBackPicTable_Gold[];
extern const struct SpriteFrameImage gTrainerBackPicTable_Kris[];
extern const struct SpriteFrameImage gTrainerBackPicTable_EBrendan[];
extern const struct SpriteFrameImage gTrainerBackPicTable_EMay[];
extern const struct SpriteFrameImage gTrainerBackPicTable_RedOriginal[];
extern const struct SpriteFrameImage gTrainerBackPicTable_LeafOriginal[];
extern const struct SpriteFrameImage gTrainerBackPicTable_RSBrendan[];
extern const struct SpriteFrameImage gTrainerBackPicTable_RSMay[];

extern const union AffineAnimCmd *const gAffineAnims_BattleSpritePlayerSide[];
extern const union AffineAnimCmd *const gAffineAnims_BattleSpriteOpponentSide[];
extern const union AffineAnimCmd *const gAffineAnims_BattleSpriteContest[];

extern const union AnimCmd *const gAnims_MonPic[]; // Castform's front pic animation uses this
extern const struct MonCoords gMonFrontPicCoords[];
extern const struct CompressedSpriteSheet gMonFrontPicTable[];
extern const struct CompressedSpriteSheet gFemaleMonFrontPicTable[];
extern const struct MonCoords gMonBackPicCoords[];
extern const struct CompressedSpriteSheet gMonBackPicTable[];
extern const struct CompressedSpriteSheet gFemaleMonBackPicTable[];
extern const struct SpritePalette gMonPaletteTable[];
extern const struct SpritePalette gMonShinyPaletteTable[];
extern const union AnimCmd *const *const gTrainerFrontAnimsPtrTable[];
extern const union AnimCmd *const gAnims_NONE[]; // Defined here because gTrainerFrontAnimsPtrTable uses it
extern const struct MonCoords gTrainerFrontPicCoords[];
extern const struct CompressedSpriteSheet gTrainerFrontPicTable[];
extern const struct SpritePalette gTrainerFrontPicPaletteTable[];
extern const union AnimCmd *const *const gTrainerBackAnimsPtrTable[];
extern const struct MonCoords gTrainerBackPicCoords[];
extern const struct SpritePalette gTrainerBackPicPaletteTable[];

extern const u8 gEnemyMonElevation[SPECIES_COUNT];
extern const bool8 SpeciesHasGenderDifference[SPECIES_COUNT];

extern const union AnimCmd *const *const gMonFrontAnimsPtrTable[];

extern const struct Trainer gTrainers[];
extern const u8 gTrainerClassNames[][13];
extern const u8 gJapaneseSpeciesNames[][JAPANESE_NAME_LENGTH + 1];
extern const u8 gSpeciesNames[][POKEMON_NAME_LENGTH + 1];
extern const u8 gGermanSpeciesNames[][POKEMON_NAME_LENGTH + 1];
extern const u8 gMoveNames[MOVES_COUNT][MOVE_NAME_LENGTH + 1];

#endif // GUARD_DATA_H
