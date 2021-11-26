#define TRAINER_BACK_PAL(trainerPic, pal) [TRAINER_BACK_PIC_##trainerPic] = {pal, TRAINER_BACK_PIC_##trainerPic}

const struct SpritePalette gTrainerBackPicPaletteTable[] =
{
    TRAINER_BACK_PAL(RED_ORIGINAL, gTrainerBackPicPalette_RedLeafOriginal),
    TRAINER_BACK_PAL(LEAF_ORIGINAL, gTrainerBackPicPalette_RedLeafOriginal),
    TRAINER_BACK_PAL(POKE_DUDE, gTrainerBackPicPalette_PokeDude),
    TRAINER_BACK_PAL(OLD_MAN, gTrainerBackPicPalette_OldMan),
    TRAINER_BACK_PAL(E_BRENDAN, gTrainerPalette_EBrendan),
    TRAINER_BACK_PAL(E_MAY, gTrainerPalette_EMay),
    TRAINER_BACK_PAL(RS_BRENDAN, gTrainerPalette_RSBrendan),
    TRAINER_BACK_PAL(RS_MAY, gTrainerPalette_RSMay),
    TRAINER_BACK_PAL(WALLY, gTrainerPalette_Wally),
    TRAINER_BACK_PAL(STEVEN, gTrainerPalette_Steven),

    TRAINER_BACK_PAL(H_BRENDAN, gTrainerPalette_HBrendan),
    TRAINER_BACK_PAL(H_MAY, gTrainerPalette_HMay),
    TRAINER_BACK_PAL(GOLD, gTrainerPalette_Gold),
    TRAINER_BACK_PAL(KRIS, gTrainerBackPicPalette_Kris),
    TRAINER_BACK_PAL(LANCE, gTrainerPalette_EliteFourLance),

    TRAINER_BACK_PAL(RED, gTrainerBackPicPalette_RedLeaf),
    TRAINER_BACK_PAL(LEAF, gTrainerBackPicPalette_RedLeaf),
    TRAINER_BACK_PAL(RED_DX, gTrainerBackPicPalette_RedLeafDX),
    TRAINER_BACK_PAL(LEAF_DX, gTrainerBackPicPalette_RedLeafDX),
    TRAINER_BACK_PAL(RED_CLASSIC, gTrainerBackPicPalette_RedLeafDX),
    TRAINER_BACK_PAL(LEAF_CLASSIC, gTrainerBackPicPalette_RedLeafDX),
    TRAINER_BACK_PAL(RED_ALOLA, gTrainerBackPicPalette_RedLeafAlola),
    TRAINER_BACK_PAL(LEAF_ALOLA, gTrainerBackPicPalette_RedLeafAlola),
    TRAINER_BACK_PAL(RED_SYGNA_SUIT, gTrainerBackPicPalette_RedLeafSygnaSuit),
    TRAINER_BACK_PAL(LEAF_SYGNA_SUIT, gTrainerBackPicPalette_RedLeafSygnaSuit),
    TRAINER_BACK_PAL(RED_TEAM_ROCKET, gTrainerBackPicPalette_RedLeafTeamRocket),
    TRAINER_BACK_PAL(LEAF_TEAM_ROCKET, gTrainerBackPicPalette_RedLeafTeamRocket),
    TRAINER_BACK_PAL(RED_TEAM_AQUA, gTrainerBackPicPalette_RedLeafTeamAqua),
    TRAINER_BACK_PAL(LEAF_TEAM_AQUA, gTrainerBackPicPalette_RedLeafTeamAqua),
    TRAINER_BACK_PAL(RED_TEAM_MAGMA, gTrainerBackPicPalette_RedLeafTeamMagma),
    TRAINER_BACK_PAL(LEAF_TEAM_MAGMA, gTrainerBackPicPalette_RedLeafTeamMagma),
};
