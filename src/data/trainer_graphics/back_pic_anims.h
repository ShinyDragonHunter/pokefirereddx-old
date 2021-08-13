static const union AnimCmd gAnimCmd_5Frames[] =
{
    ANIMCMD_FRAME(1, 20),
    ANIMCMD_FRAME(2, 6),
    ANIMCMD_FRAME(3, 6),
    ANIMCMD_FRAME(4, 24),
    ANIMCMD_FRAME(0, 1),
    ANIMCMD_END,
};

static const union AnimCmd gAnimCmd_4Frames[] =
{
    ANIMCMD_FRAME(1, 24),
    ANIMCMD_FRAME(2, 9),
    ANIMCMD_FRAME(3, 24),
    ANIMCMD_FRAME(0, 9),
    ANIMCMD_END,
};

static const union AnimCmd *const sBackAnims_5Frames[] =
{
    sAnim_GeneralFrame0,
    gAnimCmd_5Frames,
};

static const union AnimCmd *const sBackAnims_4Frames[] =
{
    sAnim_GeneralFrame0,
    gAnimCmd_4Frames,
};

const union AnimCmd *const *const gTrainerBackAnimsPtrTable[] =
{
    [TRAINER_BACK_PIC_RED] = sBackAnims_5Frames,
    [TRAINER_BACK_PIC_RED_EXTRA_OUTFIT] = sBackAnims_5Frames,
    [TRAINER_BACK_PIC_RED_CLASSIC_OUTFIT] = sBackAnims_5Frames,
    [TRAINER_BACK_PIC_LEAF] = sBackAnims_5Frames,
    [TRAINER_BACK_PIC_POKE_DUDE] = sBackAnims_4Frames,
    [TRAINER_BACK_PIC_OLD_MAN] = sBackAnims_4Frames,
    [TRAINER_BACK_PIC_GOLD] = sBackAnims_5Frames,
    [TRAINER_BACK_PIC_KRIS] = sBackAnims_5Frames,
    [TRAINER_BACK_PIC_H_BRENDAN] = sBackAnims_4Frames,
    [TRAINER_BACK_PIC_H_MAY] = sBackAnims_4Frames,
    [TRAINER_BACK_PIC_E_BRENDAN] = sBackAnims_4Frames,
    [TRAINER_BACK_PIC_E_MAY] = sBackAnims_4Frames,
    [TRAINER_BACK_PIC_RS_BRENDAN] = sBackAnims_4Frames,
    [TRAINER_BACK_PIC_RS_MAY] = sBackAnims_4Frames,
};
