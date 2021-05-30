#ifndef GUARD_DAY_NIGHT_H
#define GUARD_DAY_NIGHT_H

#define PALOVER_LIST_TERM 0xFF

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
u8 GetTimeOfDay(s8 hours);
void CheckClockForImmediateTimeEvents(void);
void ProcessImmediateTimeEvents(void);
extern const u16 sTimeOfDayTints[][3];
bool8 ShouldTintOverworld(void);
void TintPaletteForDayNight(u16 offset, u16 size);
const u8 *GetDayOfWeekString(u8 timeOfDay);

#endif // GUARD_DAY_NIGHT_H
