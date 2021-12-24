#ifndef GUARD_DEBUG_H
#define GUARD_DEBUG_H

#include "constants/debug.h"

extern const u16 gBgColor[];

struct DebugModifyArrows
{
    u8 arrowSpriteId[2];
    u16 minValue;
    u16 maxValue;
    int currValue;
    u8 currentDigit;
    u8 maxDigits;
    u8 charDigits[MODIFY_DIGITS_MAX];
    void *modifiedValPtr;
    u8 typeOfVal;
    u8 currValueForm;
};

void CB2_StartSoundCheckMenu(void);

void CB2_Debug_Pokemon(void);

void PadString(const u8 *src, u8 *dst);
u32 CharDigitsToValue(u8 *charDigits, u8 maxDigits);
void ValueToCharDigits(u8 *charDigits, u32 newValue, u8 maxDigits);
bool32 TryMoveDigit(struct DebugModifyArrows *, bool32 moveUp);

void CB2_BattleDebugMenu(void);

#endif // GUARD_DEBUG_H
