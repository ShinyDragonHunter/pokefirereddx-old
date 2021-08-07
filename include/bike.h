#ifndef GUARD_BIKE_H
#define GUARD_BIKE_H

// TODO: Do the constants make sense in FRLG? 

// Player speeds
enum
{
    SPEED_STANDING,
    SPEED_NORMAL,
    SPEED_FAST,
    SPEED_FASTER,
    SPEED_FASTEST
};

enum
{
    BIKE_TRANS_FACE_DIRECTION,
    BIKE_TRANS_TURNING,
    BIKE_TRANS_MOVING,
    BIKE_TRANS_DOWNHILL,
    BIKE_TRANS_UPHILL
};

enum
{
    BIKE_STATE_NORMAL,
    BIKE_STATE_TURNING,
    BIKE_STATE_SLOPE
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
