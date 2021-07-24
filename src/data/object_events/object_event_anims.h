static const union AnimCmd sAnim_FaceSouth[] =
{
    ANIMCMD_FRAME(0, 1),
    ANIMCMD_JUMP(0),
};

static const union AnimCmd sAnim_FaceNorth[] =
{
    ANIMCMD_FRAME(1, 1),
    ANIMCMD_JUMP(0),
};

static const union AnimCmd sAnim_FaceWest[] =
{
    ANIMCMD_FRAME(2, 1),
    ANIMCMD_JUMP(0),
};

static const union AnimCmd sAnim_FaceEast[] =
{
    ANIMCMD_FRAME(2, 1, .hFlip = TRUE),
    ANIMCMD_JUMP(0),
};

static const union AnimCmd sAnim_GoSouth[] =
{
    ANIMCMD_FRAME(3, 8),
    ANIMCMD_FRAME(0, 8),
    ANIMCMD_FRAME(4, 8),
    ANIMCMD_FRAME(0, 8),
    ANIMCMD_JUMP(0),
};

static const union AnimCmd sAnim_GoNorth[] =
{
    ANIMCMD_FRAME(5, 8),
    ANIMCMD_FRAME(1, 8),
    ANIMCMD_FRAME(6, 8),
    ANIMCMD_FRAME(1, 8),
    ANIMCMD_JUMP(0),
};

static const union AnimCmd sAnim_GoWest[] =
{
    ANIMCMD_FRAME(7, 8),
    ANIMCMD_FRAME(2, 8),
    ANIMCMD_FRAME(8, 8),
    ANIMCMD_FRAME(2, 8),
    ANIMCMD_JUMP(0),
};

static const union AnimCmd sAnim_GoEast[] =
{
    ANIMCMD_FRAME(7, 8, .hFlip = TRUE),
    ANIMCMD_FRAME(2, 8, .hFlip = TRUE),
    ANIMCMD_FRAME(8, 8, .hFlip = TRUE),
    ANIMCMD_FRAME(2, 8, .hFlip = TRUE),
    ANIMCMD_JUMP(0),
};

static const union AnimCmd sAnim_GoFastSouth[] =
{
    ANIMCMD_FRAME(3, 4),
    ANIMCMD_FRAME(0, 4),
    ANIMCMD_FRAME(4, 4),
    ANIMCMD_FRAME(0, 4),
    ANIMCMD_JUMP(0),
};

static const union AnimCmd sAnim_GoFastNorth[] =
{
    ANIMCMD_FRAME(5, 4),
    ANIMCMD_FRAME(1, 4),
    ANIMCMD_FRAME(6, 4),
    ANIMCMD_FRAME(1, 4),
    ANIMCMD_JUMP(0),
};

static const union AnimCmd sAnim_GoFastWest[] =
{
    ANIMCMD_FRAME(7, 4),
    ANIMCMD_FRAME(2, 4),
    ANIMCMD_FRAME(8, 4),
    ANIMCMD_FRAME(2, 4),
    ANIMCMD_JUMP(0),
};

static const union AnimCmd sAnim_GoFastEast[] =
{
    ANIMCMD_FRAME(7, 4, .hFlip = TRUE),
    ANIMCMD_FRAME(2, 4, .hFlip = TRUE),
    ANIMCMD_FRAME(8, 4, .hFlip = TRUE),
    ANIMCMD_FRAME(2, 4, .hFlip = TRUE),
    ANIMCMD_JUMP(0),
};

static const union AnimCmd sAnim_GoFasterSouth[] =
{
    ANIMCMD_FRAME(3, 2),
    ANIMCMD_FRAME(0, 2),
    ANIMCMD_FRAME(4, 2),
    ANIMCMD_FRAME(0, 2),
    ANIMCMD_JUMP(0),
};

static const union AnimCmd sAnim_GoFasterNorth[] =
{
    ANIMCMD_FRAME(5, 2),
    ANIMCMD_FRAME(1, 2),
    ANIMCMD_FRAME(6, 2),
    ANIMCMD_FRAME(1, 2),
    ANIMCMD_JUMP(0),
};

static const union AnimCmd sAnim_GoFasterWest[] =
{
    ANIMCMD_FRAME(7, 2),
    ANIMCMD_FRAME(2, 2),
    ANIMCMD_FRAME(8, 2),
    ANIMCMD_FRAME(2, 2),
    ANIMCMD_JUMP(0),
};

static const union AnimCmd sAnim_GoFasterEast[] =
{
    ANIMCMD_FRAME(7, 2, .hFlip = TRUE),
    ANIMCMD_FRAME(2, 2, .hFlip = TRUE),
    ANIMCMD_FRAME(8, 2, .hFlip = TRUE),
    ANIMCMD_FRAME(2, 2, .hFlip = TRUE),
    ANIMCMD_JUMP(0),
};

static const union AnimCmd sAnim_GoFastestSouth[] =
{
    ANIMCMD_FRAME(3, 1),
    ANIMCMD_FRAME(0, 1),
    ANIMCMD_FRAME(4, 1),
    ANIMCMD_FRAME(0, 1),
    ANIMCMD_JUMP(0),
};

static const union AnimCmd sAnim_GoFastestNorth[] =
{
    ANIMCMD_FRAME(5, 1),
    ANIMCMD_FRAME(1, 1),
    ANIMCMD_FRAME(6, 1),
    ANIMCMD_FRAME(1, 1),
    ANIMCMD_JUMP(0),
};

static const union AnimCmd sAnim_GoFastestWest[] =
{
    ANIMCMD_FRAME(7, 1),
    ANIMCMD_FRAME(2, 1),
    ANIMCMD_FRAME(8, 1),
    ANIMCMD_FRAME(2, 1),
    ANIMCMD_JUMP(0),
};

static const union AnimCmd sAnim_GoFastestEast[] =
{
    ANIMCMD_FRAME(7, 1, .hFlip = TRUE),
    ANIMCMD_FRAME(2, 1, .hFlip = TRUE),
    ANIMCMD_FRAME(8, 1, .hFlip = TRUE),
    ANIMCMD_FRAME(2, 1, .hFlip = TRUE),
    ANIMCMD_JUMP(0),
};

static const union AnimCmd sAnim_RunSouth[] =
{
    ANIMCMD_FRAME(12, 5),
    ANIMCMD_FRAME(9, 3),
    ANIMCMD_FRAME(13, 5),
    ANIMCMD_FRAME(9, 3),
    ANIMCMD_JUMP(0),
};

static const union AnimCmd sAnim_RunNorth[] =
{
    ANIMCMD_FRAME(14, 5),
    ANIMCMD_FRAME(10, 3),
    ANIMCMD_FRAME(15, 5),
    ANIMCMD_FRAME(10, 3),
    ANIMCMD_JUMP(0),
};

static const union AnimCmd sAnim_RunWest[] =
{
    ANIMCMD_FRAME(16, 5),
    ANIMCMD_FRAME(11, 3),
    ANIMCMD_FRAME(17, 5),
    ANIMCMD_FRAME(11, 3),
    ANIMCMD_JUMP(0),
};

static const union AnimCmd sAnim_RunEast[] =
{
    ANIMCMD_FRAME(16, 5, .hFlip = TRUE),
    ANIMCMD_FRAME(11, 3, .hFlip = TRUE),
    ANIMCMD_FRAME(17, 5, .hFlip = TRUE),
    ANIMCMD_FRAME(11, 3, .hFlip = TRUE),
    ANIMCMD_JUMP(0),
};

static const union AnimCmd sAnim_FieldMove[] =
{
    ANIMCMD_FRAME(0, 4),
    ANIMCMD_FRAME(1, 4),
    ANIMCMD_FRAME(2, 4),
    ANIMCMD_FRAME(3, 4),
    ANIMCMD_FRAME(4, 8),
    ANIMCMD_END,
};

const union AnimCmd sAnim_VsSeeker[] = {
    ANIMCMD_FRAME(0, 4),
    ANIMCMD_FRAME(1, 4),
    ANIMCMD_FRAME(5, 4),
    ANIMCMD_FRAME(6, 4),
    ANIMCMD_LOOP(0),
    ANIMCMD_FRAME(7, 4),
    ANIMCMD_FRAME(8, 4),
    ANIMCMD_LOOP(6),
    ANIMCMD_FRAME(6, 4),
    ANIMCMD_FRAME(1, 4),
    ANIMCMD_FRAME(0, 4),
    ANIMCMD_END,
};

static const union AnimCmd sAnim_GetOnOffSurfBlobSouth[] =
{
    ANIMCMD_FRAME(3, 1),
    ANIMCMD_JUMP(0),
};

static const union AnimCmd sAnim_GetOnOffSurfBlobNorth[] =
{
    ANIMCMD_FRAME(4, 1),
    ANIMCMD_JUMP(0),
};

static const union AnimCmd sAnim_GetOnOffSurfBlobWest[] =
{
    ANIMCMD_FRAME(5, 1),
    ANIMCMD_JUMP(0),
};

static const union AnimCmd sAnim_GetOnOffSurfBlobEast[] =
{
    ANIMCMD_FRAME(5, 1, .hFlip = TRUE),
    ANIMCMD_JUMP(0),
};

static const union AnimCmd sAnim_BerryTreeStage1[] =
{
    ANIMCMD_FRAME(1, 32),
    ANIMCMD_FRAME(2, 32),
    ANIMCMD_END,
};

static const union AnimCmd sAnim_BerryTreeStage2[] =
{
    ANIMCMD_FRAME(3, 48),
    ANIMCMD_FRAME(4, 48),
    ANIMCMD_END,
};

static const union AnimCmd sAnim_BerryTreeStage3[] =
{
    ANIMCMD_FRAME(5, 32),
    ANIMCMD_FRAME(5, 32),
    ANIMCMD_FRAME(6, 32),
    ANIMCMD_FRAME(6, 32),
    ANIMCMD_END,
};

static const union AnimCmd sAnim_BerryTreeStage4[] =
{
    ANIMCMD_FRAME(7, 48),
    ANIMCMD_FRAME(7, 48),
    ANIMCMD_FRAME(8, 48),
    ANIMCMD_FRAME(8, 48),
    ANIMCMD_END,
};

static const union AnimCmd sAnim_NurseBow[] =
{
    ANIMCMD_FRAME(0, 8),
    ANIMCMD_FRAME(9, 32),
    ANIMCMD_FRAME(0, 8),
    ANIMCMD_END,
};

static const union AnimCmd sAnim_RockBreak[] =
{
    ANIMCMD_FRAME(0, 8),
    ANIMCMD_FRAME(1, 8),
    ANIMCMD_FRAME(2, 8),
    ANIMCMD_FRAME(3, 8),
    ANIMCMD_END,
};

static const union AnimCmd sAnim_TreeCut[] =
{
    ANIMCMD_FRAME(0, 6),
    ANIMCMD_FRAME(1, 6),
    ANIMCMD_FRAME(2, 6),
    ANIMCMD_FRAME(3, 6),
    ANIMCMD_END,
};

static const union AnimCmd sAnim_TakeOutRodSouth[] =
{
    ANIMCMD_FRAME(8, 4),
    ANIMCMD_FRAME(9, 4),
    ANIMCMD_FRAME(10, 4),
    ANIMCMD_FRAME(11, 4),
    ANIMCMD_END,
};

static const union AnimCmd sAnim_TakeOutRodNorth[] =
{
    ANIMCMD_FRAME(4, 4),
    ANIMCMD_FRAME(5, 4),
    ANIMCMD_FRAME(6, 4),
    ANIMCMD_FRAME(7, 4),
    ANIMCMD_END,
};

static const union AnimCmd sAnim_TakeOutRodWest[] =
{
    ANIMCMD_FRAME(0, 4),
    ANIMCMD_FRAME(1, 4),
    ANIMCMD_FRAME(2, 4),
    ANIMCMD_FRAME(3, 4),
    ANIMCMD_END,
};

static const union AnimCmd sAnim_TakeOutRodEast[] =
{
    ANIMCMD_FRAME(0, 4, .hFlip = TRUE),
    ANIMCMD_FRAME(1, 4, .hFlip = TRUE),
    ANIMCMD_FRAME(2, 4, .hFlip = TRUE),
    ANIMCMD_FRAME(3, 4, .hFlip = TRUE),
    ANIMCMD_END,
};

static const union AnimCmd sAnim_PutAwayRodSouth[] =
{
    ANIMCMD_FRAME(11, 4),
    ANIMCMD_FRAME(10, 6),
    ANIMCMD_FRAME(9, 6),
    ANIMCMD_FRAME(8, 6),
    ANIMCMD_END,
};

static const union AnimCmd sAnim_PutAwayRodNorth[] =
{
    ANIMCMD_FRAME(7, 4),
    ANIMCMD_FRAME(6, 6),
    ANIMCMD_FRAME(5, 6),
    ANIMCMD_FRAME(4, 6),
    ANIMCMD_END,
};

static const union AnimCmd sAnim_PutAwayRodWest[] =
{
    ANIMCMD_FRAME(3, 4),
    ANIMCMD_FRAME(2, 4),
    ANIMCMD_FRAME(1, 4),
    ANIMCMD_FRAME(0, 4),
    ANIMCMD_END,
};

static const union AnimCmd sAnim_PutAwayRodEast[] =
{
    ANIMCMD_FRAME(3, 4, .hFlip = TRUE),
    ANIMCMD_FRAME(2, 4, .hFlip = TRUE),
    ANIMCMD_FRAME(1, 4, .hFlip = TRUE),
    ANIMCMD_FRAME(0, 4, .hFlip = TRUE),
    ANIMCMD_END,
};

static const union AnimCmd sAnim_HookedPokemonSouth[] =
{
    ANIMCMD_FRAME(10, 6),
    ANIMCMD_FRAME(11, 6),
    ANIMCMD_LOOP(1),
    ANIMCMD_FRAME(11, 30),
    ANIMCMD_JUMP(0),
};

static const union AnimCmd sAnim_HookedPokemonNorth[] =
{
    ANIMCMD_FRAME(6, 6),
    ANIMCMD_FRAME(7, 6),
    ANIMCMD_LOOP(1),
    ANIMCMD_FRAME(7, 30),
    ANIMCMD_JUMP(0),
};

static const union AnimCmd sAnim_HookedPokemonWest[] =
{
    ANIMCMD_FRAME(2, 6),
    ANIMCMD_FRAME(3, 6),
    ANIMCMD_LOOP(1),
    ANIMCMD_FRAME(3, 30),
    ANIMCMD_JUMP(0),
};

static const union AnimCmd sAnim_HookedPokemonEast[] =
{
    ANIMCMD_FRAME(2, 6, .hFlip = TRUE),
    ANIMCMD_FRAME(3, 6, .hFlip = TRUE),
    ANIMCMD_LOOP(1),
    ANIMCMD_FRAME(3, 30, .hFlip = TRUE),
    ANIMCMD_JUMP(0),
};

static const union AnimCmd sAnim_HoOhFlapWings[] =
{
    ANIMCMD_FRAME(0, 8),
    ANIMCMD_FRAME(1, 8),
    ANIMCMD_FRAME(0, 8),
    ANIMCMD_FRAME(1, 8),
    ANIMCMD_JUMP(0),
};

static const union AnimCmd *const sAnimTable_Inanimate[] = {
    sAnim_FaceSouth,
};

static const union AnimCmd *const sAnimTable_Standard[] = {
    sAnim_FaceSouth,
    sAnim_FaceNorth,
    sAnim_FaceWest,
    sAnim_FaceEast,
    sAnim_GoSouth,
    sAnim_GoNorth,
    sAnim_GoWest,
    sAnim_GoEast,
    sAnim_GoFastSouth,
    sAnim_GoFastNorth,
    sAnim_GoFastWest,
    sAnim_GoFastEast,
    sAnim_GoFasterSouth,
    sAnim_GoFasterNorth,
    sAnim_GoFasterWest,
    sAnim_GoFasterEast,
    sAnim_GoFastestSouth,
    sAnim_GoFastestNorth,
    sAnim_GoFastestWest,
    sAnim_GoFastestEast,
    sAnim_RunSouth,
    sAnim_RunNorth,
    sAnim_RunWest,
    sAnim_RunEast,
};

static const union AnimCmd *const sAnimTable_HoOh[] = {
    sAnim_FaceSouth,
    sAnim_FaceNorth,
    sAnim_FaceWest,
    sAnim_FaceEast,
    sAnim_HoOhFlapWings,
    sAnim_FaceSouth,
    sAnim_GoWest,
    sAnim_GoEast,
    sAnim_GoFastSouth,
    sAnim_GoFastNorth,
    sAnim_GoFastWest,
    sAnim_GoFastEast,
    sAnim_GoFasterSouth,
    sAnim_GoFasterNorth,
    sAnim_GoFasterWest,
    sAnim_GoFasterEast,
    sAnim_GoFastestSouth,
    sAnim_GoFastestNorth,
    sAnim_GoFastestWest,
    sAnim_GoFastestEast,
};

static const union AnimCmd *const sAnimTable_Surfing[] = {
    sAnim_FaceSouth,
    sAnim_FaceNorth,
    sAnim_FaceWest,
    sAnim_FaceEast,
    sAnim_FaceSouth,
    sAnim_FaceNorth,
    sAnim_FaceWest,
    sAnim_FaceEast,
    sAnim_FaceSouth,
    sAnim_FaceNorth,
    sAnim_FaceWest,
    sAnim_FaceEast,
    sAnim_FaceSouth,
    sAnim_FaceNorth,
    sAnim_FaceWest,
    sAnim_FaceEast,
    sAnim_FaceSouth,
    sAnim_FaceNorth,
    sAnim_FaceWest,
    sAnim_FaceEast,
    sAnim_GetOnOffSurfBlobSouth,
    sAnim_GetOnOffSurfBlobNorth,
    sAnim_GetOnOffSurfBlobWest,
    sAnim_GetOnOffSurfBlobEast,
};

static const union AnimCmd *const sAnimTable_Nurse[] = {
    sAnim_FaceSouth,
    sAnim_FaceNorth,
    sAnim_FaceWest,
    sAnim_FaceEast,
    sAnim_GoSouth,
    sAnim_GoNorth,
    sAnim_GoWest,
    sAnim_GoEast,
    sAnim_GoFastSouth,
    sAnim_GoFastNorth,
    sAnim_GoFastWest,
    sAnim_GoFastEast,
    sAnim_GoFasterSouth,
    sAnim_GoFasterNorth,
    sAnim_GoFasterWest,
    sAnim_GoFasterEast,
    sAnim_GoFastestSouth,
    sAnim_GoFastestNorth,
    sAnim_GoFastestWest,
    sAnim_GoFastestEast,
    sAnim_NurseBow,
};

static const union AnimCmd *const sAnimTable_FieldMove[] = {
    sAnim_FieldMove,
    sAnim_VsSeeker,
};

static const union AnimCmd *const sAnimTable_BerryTree[] = {
    sAnim_FaceSouth,
    sAnim_BerryTreeStage1,
    sAnim_BerryTreeStage2,
    sAnim_BerryTreeStage3,
    sAnim_BerryTreeStage4,
};

static const union AnimCmd *const sAnimTable_BreakableRock[] = {
    sAnim_FaceSouth,
    sAnim_RockBreak,
};

static const union AnimCmd *const sAnimTable_CuttableTree[] = {
    sAnim_FaceSouth,
    sAnim_TreeCut,
};

static const union AnimCmd *const sAnimTable_Fishing[] = {
    sAnim_TakeOutRodSouth,
    sAnim_TakeOutRodNorth,
    sAnim_TakeOutRodWest,
    sAnim_TakeOutRodEast,
    sAnim_PutAwayRodSouth,
    sAnim_PutAwayRodNorth,
    sAnim_PutAwayRodWest,
    sAnim_PutAwayRodEast,
    sAnim_HookedPokemonSouth,
    sAnim_HookedPokemonNorth,
    sAnim_HookedPokemonWest,
    sAnim_HookedPokemonEast,
};

// For animations with alternating steps
static const struct StepAnimTable sStepAnimTables[] = {
    {
        .anims = sAnimTable_Standard,
        .animPos = {1, 3, 0, 2},
    },
    {
        .anims = sAnimTable_Surfing,
        .animPos = {1, 3, 0, 2},
    },
    {
        .anims = sAnimTable_Nurse,
        .animPos = {1, 3, 0, 2},
    },
    {
        .anims = sAnimTable_Fishing,
        .animPos = {1, 3, 0, 2},
    },
    {
        NULL,
        {0, 0, 0, 0},
    },
};
