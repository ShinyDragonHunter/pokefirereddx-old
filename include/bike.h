#ifndef GUARD_BIKE_H
#define GUARD_BIKE_H

// Player speeds
enum
{
    SPEED_STANDING,
    SPEED_NORMAL,
    SPEED_FAST,
    SPEED_FASTER,
    SPEED_FASTEST,
};

// Acro bike transitions
enum
{
    ACRO_TRANS_FACE_DIRECTION,
    ACRO_TRANS_TURN_DIRECTION,
    ACRO_TRANS_MOVING,
    ACRO_TRANS_NORMAL_TO_WHEELIE,
    ACRO_TRANS_WHEELIE_TO_NORMAL,
};

// Acro bike states
enum
{
    ACRO_STATE_NORMAL,
    ACRO_STATE_TURNING,
    ACRO_STATE_WHEELIE_STANDING,
};

// Exported RAM declarations
extern bool8 gUnusedBikeCameraAheadPanback;

// Exported ROM declarations
void MovePlayerOnBike(u8 direction, u16 newKeys, u16 heldKeys);
bool8 IsBikingDisallowedByPlayer(void);
void GetOnOffBike(u8 transitionFlags);
void BikeClearState(void);
s16 GetPlayerSpeed(void);
bool32 IsRunningDisallowed(u8 metatile);

#endif // GUARD_BIKE_H
