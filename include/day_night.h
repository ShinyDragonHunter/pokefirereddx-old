#ifndef GUARD_DAY_NIGHT_H
#define GUARD_DAY_NIGHT_H

struct PaletteOverride
{
    u8 slot;
    u8 startHour;
    u8 endHour;
    void *palette;
};

extern EWRAM_DATA u16 gPlttBufferPreDN[];
extern EWRAM_DATA struct PaletteOverride *gPaletteOverrides[];

u8 GetCurrentTimeOfDay(void);
void CheckClockForImmediateTimeEvents(void);
void ProcessImmediateTimeEvents(void);
void TintPaletteForDayNight(u16 offset, u16 size);
const u8 *GetDayOfWeekString(u8 dayOfWeek);

#endif // GUARD_DAY_NIGHT_H
