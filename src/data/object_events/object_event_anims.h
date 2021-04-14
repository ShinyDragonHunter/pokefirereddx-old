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

static const union AnimCmd sAnim_BunnyHoppyBackWheelSouth[] =
{
    ANIMCMD_FRAME(9, 4),
    ANIMCMD_FRAME(10, 4),
    ANIMCMD_END,
};

static const union AnimCmd sAnim_BunnyHoppyBackWheelNorth[] =
{
    ANIMCMD_FRAME(13, 4),
    ANIMCMD_FRAME(14, 4),
    ANIMCMD_END,
};

static const union AnimCmd sAnim_BunnyHoppyBackWheelWest[] =
{
    ANIMCMD_FRAME(17, 4),
    ANIMCMD_FRAME(18, 4),
    ANIMCMD_END,
};

static const union AnimCmd sAnim_BunnyHoppyBackWheelEast[] =
{
    ANIMCMD_FRAME(17, 4, .hFlip = TRUE),
    ANIMCMD_FRAME(18, 4, .hFlip = TRUE),
    ANIMCMD_END,
};

static const union AnimCmd sAnim_BunnyHoppyFrontWheelSouth[] =
{
    ANIMCMD_FRAME(11, 4),
    ANIMCMD_FRAME(12, 4),
    ANIMCMD_END,
};

static const union AnimCmd sAnim_BunnyHoppyFrontWheelNorth[] =
{
    ANIMCMD_FRAME(15, 4),
    ANIMCMD_FRAME(16, 4),
    ANIMCMD_END,
};

static const union AnimCmd sAnim_BunnyHoppyFrontWheelWest[] =
{
    ANIMCMD_FRAME(19, 4),
    ANIMCMD_FRAME(20, 4),
    ANIMCMD_END,
};

static const union AnimCmd sAnim_BunnyHoppyFrontWheelEast[] =
{
    ANIMCMD_FRAME(19, 4, .hFlip = TRUE),
    ANIMCMD_FRAME(20, 4, .hFlip = TRUE),
    ANIMCMD_END,
};

static const union AnimCmd sAnim_StandingWheelieBackWheelSouth[] =
{
    ANIMCMD_FRAME(9, 4),
    ANIMCMD_FRAME(0, 4),
    ANIMCMD_END,
};

static const union AnimCmd sAnim_StandingWheelieBackWheelNorth[] =
{
    ANIMCMD_FRAME(13, 4),
    ANIMCMD_FRAME(1, 4),
    ANIMCMD_END,
};

static const union AnimCmd sAnim_StandingWheelieBackWheelWest[] =
{
    ANIMCMD_FRAME(17, 4),
    ANIMCMD_FRAME(2, 4),
    ANIMCMD_END,
};

static const union AnimCmd sAnim_StandingWheelieBackWheelEast[] =
{
    ANIMCMD_FRAME(17, 4, .hFlip = TRUE),
    ANIMCMD_FRAME(2, 4, .hFlip = TRUE),
    ANIMCMD_END,
};

static const union AnimCmd sAnim_StandingWheelieFrontWheelSouth[] =
{
    ANIMCMD_FRAME(11, 4),
    ANIMCMD_FRAME(0, 4),
    ANIMCMD_END,
};

static const union AnimCmd sAnim_StandingWheelieFrontWheelNorth[] =
{
    ANIMCMD_FRAME(15, 4),
    ANIMCMD_FRAME(1, 4),
    ANIMCMD_END,
};

static const union AnimCmd sAnim_StandingWheelieFrontWheelWest[] =
{
    ANIMCMD_FRAME(19, 4),
    ANIMCMD_FRAME(2, 4),
    ANIMCMD_END,
};

static const union AnimCmd sAnim_StandingWheelieFrontWheelEast[] =
{
    ANIMCMD_FRAME(19, 4, .hFlip = TRUE),
    ANIMCMD_FRAME(2, 4, .hFlip = TRUE),
    ANIMCMD_END,
};

static const union AnimCmd sAnim_MovingWheelieSouth[] =
{
    ANIMCMD_FRAME(21, 4),
    ANIMCMD_FRAME(10, 4),
    ANIMCMD_FRAME(22, 4),
    ANIMCMD_FRAME(10, 4),
    ANIMCMD_JUMP(0),
};

static const union AnimCmd sAnim_MovingWheelieNorth[] =
{
    ANIMCMD_FRAME(23, 4),
    ANIMCMD_FRAME(14, 4),
    ANIMCMD_FRAME(24, 4),
    ANIMCMD_FRAME(14, 4),
    ANIMCMD_JUMP(0),
};

static const union AnimCmd sAnim_MovingWheelieWest[] =
{
    ANIMCMD_FRAME(25, 4),
    ANIMCMD_FRAME(18, 4),
    ANIMCMD_FRAME(26, 4),
    ANIMCMD_FRAME(18, 4),
    ANIMCMD_JUMP(0),
};

static const union AnimCmd sAnim_MovingWheelieEast[] =
{
    ANIMCMD_FRAME(25, 4, .hFlip = TRUE),
    ANIMCMD_FRAME(18, 4, .hFlip = TRUE),
    ANIMCMD_FRAME(26, 4, .hFlip = TRUE),
    ANIMCMD_FRAME(18, 4, .hFlip = TRUE),
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

static const union AffineAnimCmd sAffineAnim_KyogreGroudon_GoSouthStart[] =
{
    AFFINEANIMCMD_FRAME(0x0, 0x0, 1, 1),
    AFFINEANIMCMD_FRAME(0x0, 0x0, 0, 1),
    AFFINEANIMCMD_FRAME(0x0, 0x0, 0, 1),
    AFFINEANIMCMD_FRAME(0x0, 0x0, 0, 1),
    AFFINEANIMCMD_LOOP(7),
    AFFINEANIMCMD_JUMP(0),
};

static const union AffineAnimCmd sAffineAnim_KyogreGroudon_GoSouth[] =
{
    AFFINEANIMCMD_FRAME(0x0, 0x0, -1, 1),
    AFFINEANIMCMD_FRAME(0x0, 0x0, 0, 1),
    AFFINEANIMCMD_LOOP(15),
    AFFINEANIMCMD_FRAME(0x0, 0x0, 1, 1),
    AFFINEANIMCMD_FRAME(0x0, 0x0, 0, 1),
    AFFINEANIMCMD_LOOP(15),
    AFFINEANIMCMD_JUMP(0),
};

static const union AffineAnimCmd sAffineAnim_KyogreGroudon_AttackEast[] =
{
    AFFINEANIMCMD_FRAME(0x100, 0x100, 10, 0),
    AFFINEANIMCMD_END,
};

static const union AffineAnimCmd sAffineAnim_KyogreGroudon_DipEast[] =
{
    AFFINEANIMCMD_FRAME(0x0, 0x0, -1, 1),
    AFFINEANIMCMD_LOOP(8),
    AFFINEANIMCMD_END,
};

static const union AffineAnimCmd sAffineAnim_KyogreGroudon_DipWest[] =
{
    AFFINEANIMCMD_FRAME(0x0, 0x0, 1, 1),
    AFFINEANIMCMD_LOOP(8),
    AFFINEANIMCMD_END,
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

static const union AnimCmd *const sAnimTable_GroudonSide[] = {
    sAnim_FaceSouth,
    sAnim_FaceNorth,
    sAnim_FaceWest,
    sAnim_FaceEast,
    sAnim_GoSouth,
    sAnim_GoEast,
    sAnim_GoWest,
    sAnim_GoEast,
    sAnim_GoFastSouth,
    sAnim_GoFastEast,
    sAnim_GoFastWest,
    sAnim_GoFastEast,
    sAnim_GoFasterSouth,
    sAnim_GoFasterEast,
    sAnim_GoFasterWest,
    sAnim_GoFasterEast,
    sAnim_GoFastestSouth,
    sAnim_GoFastestEast,
    sAnim_GoFastestWest,
    sAnim_GoFastestEast,
};

// Though they correspond to facing/walking movements, Rayquaza doesn't have
// equivalent images aside from flying up. Its other frames aside from the 'normal' 
// frame are for the sequence where it awakens on Sky Pillar.
// The corresponding facing/walking movements are commented alongside
static const union AnimCmd *const sAnimTable_Rayquaza[] = {
    sAnim_FaceSouth,                // Face South
    sAnim_FaceSouth,                // Face North
    sAnim_FaceSouth,                // Face West
    sAnim_GetOnOffSurfBlobSouth,    // Face East
    sAnim_FaceSouth,                // Go South
    sAnim_GetOnOffSurfBlobNorth,    // Go North
    sAnim_FaceWest,                 // Go West
    sAnim_GetOnOffSurfBlobSouth,    // Go East
    sAnim_FaceSouth,                // Go fast South
    sAnim_GetOnOffSurfBlobNorth,    // Go fast North
    sAnim_FaceNorth,                // Go fast West
    sAnim_GetOnOffSurfBlobSouth,    // Go fast East
    sAnim_FaceSouth,                // Go faster South
    sAnim_GetOnOffSurfBlobNorth,    // Go faster North
    sAnim_FaceWest,                 // Go faster West
    sAnim_GetOnOffSurfBlobSouth,    // Go faster East
    sAnim_FaceSouth,                // Go fastest South
    sAnim_GetOnOffSurfBlobNorth,    // Go fastest North
    sAnim_FaceWest,                 // Go fastest West
    sAnim_GetOnOffSurfBlobSouth,    // Go fastest East
};

static const union AnimCmd *const sAnimTable_AcroBike[] = {
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
    sAnim_BunnyHoppyBackWheelSouth,
    sAnim_BunnyHoppyBackWheelNorth,
    sAnim_BunnyHoppyBackWheelWest,
    sAnim_BunnyHoppyBackWheelEast,
    sAnim_BunnyHoppyFrontWheelSouth,
    sAnim_BunnyHoppyFrontWheelNorth,
    sAnim_BunnyHoppyFrontWheelWest,
    sAnim_BunnyHoppyFrontWheelEast,
    sAnim_StandingWheelieBackWheelSouth,
    sAnim_StandingWheelieBackWheelNorth,
    sAnim_StandingWheelieBackWheelWest,
    sAnim_StandingWheelieBackWheelEast,
    sAnim_StandingWheelieFrontWheelSouth,
    sAnim_StandingWheelieFrontWheelNorth,
    sAnim_StandingWheelieFrontWheelWest,
    sAnim_StandingWheelieFrontWheelEast,
    sAnim_MovingWheelieSouth,
    sAnim_MovingWheelieNorth,
    sAnim_MovingWheelieWest,
    sAnim_MovingWheelieEast,
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

static const union AffineAnimCmd *const sAffineAnimTable_KyogreGroudon[] = {
    sAffineAnim_KyogreGroudon_GoSouthStart, // Used by Kyogre/Groudon when awakened
    sAffineAnim_KyogreGroudon_GoSouth,      // Used by Kyogre/Groudon when awakened
    sAffineAnim_KyogreGroudon_AttackEast,   // Used by Kyogre during Sootopolis fight
    sAffineAnim_KyogreGroudon_AttackEast,   // Unused
    sAffineAnim_KyogreGroudon_DipWest,      // Unused
    sAffineAnim_KyogreGroudon_DipEast,      // Unused
};

// For animations with alternating steps
static const struct StepAnimTable sStepAnimTables[] = {
    {
        .anims = sAnimTable_Standard,
        .animPos = {1, 3, 0, 2},
    },
    {
        .anims = sAnimTable_AcroBike,
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
