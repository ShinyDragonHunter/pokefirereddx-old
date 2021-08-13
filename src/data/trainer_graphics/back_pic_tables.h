const struct MonCoords gTrainerBackPicCoords[] =
{
    [TRAINER_BACK_PIC_RED] = {.size = 8, .y_offset = 5},
    [TRAINER_BACK_PIC_RED_EXTRA_OUTFIT] = {.size = 8, .y_offset = 5},
    [TRAINER_BACK_PIC_RED_CLASSIC_OUTFIT] = {.size = 8, .y_offset = 5},
    [TRAINER_BACK_PIC_LEAF] = {.size = 8, .y_offset = 5},
    [TRAINER_BACK_PIC_POKE_DUDE] = {.size = 8, .y_offset = 4},
    [TRAINER_BACK_PIC_OLD_MAN] = {.size = 8, .y_offset = 4},
    [TRAINER_BACK_PIC_GOLD] = {.size = 8, .y_offset = 5},
    [TRAINER_BACK_PIC_KRIS] = {.size = 8, .y_offset = 5},
    [TRAINER_BACK_PIC_H_BRENDAN] = {.size = 8, .y_offset = 4},
    [TRAINER_BACK_PIC_H_MAY] = {.size = 8, .y_offset = 4},
    [TRAINER_BACK_PIC_E_BRENDAN] = {.size = 8, .y_offset = 4},
    [TRAINER_BACK_PIC_E_MAY] = {.size = 8, .y_offset = 4},
    [TRAINER_BACK_PIC_RS_BRENDAN] = {.size = 8, .y_offset = 4},
    [TRAINER_BACK_PIC_RS_MAY] = {.size = 8, .y_offset = 4},
};

#define TRAINER_BACK_PAL(trainerPic, pal) [TRAINER_BACK_PIC_##trainerPic] = {pal, TRAINER_BACK_PIC_##trainerPic}

const struct SpritePalette gTrainerBackPicPaletteTable[] =
{
    TRAINER_BACK_PAL(RED, gTrainerBackPicPalettePlayer),
    TRAINER_BACK_PAL(RED_EXTRA_OUTFIT, gTrainerBackPicPalettePlayerExtraOutfit),
    TRAINER_BACK_PAL(RED_CLASSIC_OUTFIT, gTrainerPalettePlayerClassicOutfit),
    TRAINER_BACK_PAL(LEAF, gTrainerBackPicPalettePlayer),
    TRAINER_BACK_PAL(POKE_DUDE, gTrainerBackPicPalettePokeDude),
    TRAINER_BACK_PAL(OLD_MAN, gTrainerBackPicPaletteOldMan),
    TRAINER_BACK_PAL(GOLD, gTrainerPaletteGold),
    TRAINER_BACK_PAL(KRIS, gTrainerPaletteKris),
    TRAINER_BACK_PAL(H_BRENDAN, gTrainerPaletteHBrendan),
    TRAINER_BACK_PAL(H_MAY, gTrainerPaletteHMay),
    TRAINER_BACK_PAL(E_BRENDAN, gTrainerPaletteEBrendan),
    TRAINER_BACK_PAL(E_MAY, gTrainerPaletteEMay),
    TRAINER_BACK_PAL(RS_BRENDAN, gTrainerPaletteRSBrendan),
    TRAINER_BACK_PAL(RS_MAY, gTrainerPaletteRSMay),
};
