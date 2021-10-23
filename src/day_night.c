#include "global.h"
#include "day_night.h"
#include "decompress.h"
#include "event_data.h"
#include "field_tasks.h"
#include "field_weather.h"
#include "overworld.h"
#include "palette.h"
#include "rtc.h"
#include "strings.h"
#include "string_util.h"
#include "constants/day_night.h"
#include "constants/layouts.h"
#include "constants/rgb.h"

EWRAM_DATA u16 gPlttBufferPreDN[PLTT_BUFFER_SIZE] = {0};
EWRAM_DATA struct PaletteOverride *gPaletteOverrides[4] = {NULL};

static EWRAM_DATA struct
{
    bool8 initialized:1;
    bool8 retintPhase:1;
    u8 timeOfDay;
    u16 prevTintPeriod; // tint period associated with currently drawn palettes
    u16 currTintPeriod; // tint period associated with currRGBTint
    u16 currRGBTint[3];
} sDNSystemControl = {0};

static const u16 sTimeOfDayTints[][3] =
{
    [0] = {TINT_NIGHT},
    [1] = {TINT_NIGHT},
    [2] = {TINT_NIGHT},
    [3] = {TINT_NIGHT},
    [4] = {Q_8_8(0.6), Q_8_8(0.65), Q_8_8(1.0)},
    [5] = {TINT_MORNING},
    [6] = {TINT_MORNING},
    [7] = {TINT_MORNING},
    [8] = {Q_8_8(0.9), Q_8_8(0.85), Q_8_8(1.0)},
    [9] = {Q_8_8(1.0), Q_8_8(0.9), Q_8_8(1.0)},
    [10] = {TINT_DAY},
    [11] = {TINT_DAY},
    [12] = {TINT_DAY},
    [13] = {TINT_DAY},
    [14] = {TINT_DAY},
    [15] = {TINT_DAY},
    [16] = {TINT_DAY},
    [17] = {Q_8_8(1.0), Q_8_8(0.98), Q_8_8(0.9)},
    [18] = {Q_8_8(0.9), Q_8_8(0.7), Q_8_8(0.67)},
    [19] = {Q_8_8(0.75), Q_8_8(0.66), Q_8_8(0.77)},
    [20] = {Q_8_8(0.7), Q_8_8(0.63), Q_8_8(0.82)},
    [21] = {TINT_NIGHT},
    [22] = {TINT_NIGHT},
    [23] = {TINT_NIGHT},
};

static const u8 *const sDayOfWeekTable[] = 
{
    [DAY_SUNDAY] = gText_Sunday,
    [DAY_MONDAY] = gText_Monday,
    [DAY_TUESDAY] = gText_Tuesday,
    [DAY_WEDNESDAY] = gText_Wednesday,
    [DAY_THURSDAY] = gText_Thursday,
    [DAY_FRIDAY] = gText_Friday,
    [DAY_SATURDAY] = gText_Saturday
};

u8 GetCurrentTimeOfDay(void)
{
    if (gLocalTime.hours < HOUR_MORNING)
        return TIME_NIGHT;
    else if (gLocalTime.hours < HOUR_DAY)
        return TIME_MORNING;
    else if (gLocalTime.hours < HOUR_NIGHT)
        return TIME_DAY;
    return TIME_NIGHT;
}

const u8 *GetDayOfWeekString(u8 dayOfWeek)
{
    return sDayOfWeekTable[dayOfWeek];
}

void CopyDayOfWeekStringToVar1(void)
{
    if (gSpecialVar_0x8004 <= DAY_SATURDAY)
        StringCopy(gStringVar1, sDayOfWeekTable[gSpecialVar_0x8004]);
    else
        StringCopy(gStringVar1, gText_None);
}

void CopyCurrentDayOfWeekStringToVar1(void)
{
    RtcCalcLocalTime();
    if (gLocalTime.dayOfWeek <= DAY_SATURDAY)
        StringCopy(gStringVar1, sDayOfWeekTable[gLocalTime.dayOfWeek]);
    else
        StringCopy(gStringVar1, gText_None);
}

static void LoadPaletteOverrides(void)
{
    u32 i, j;
    const u16* src;
    u16* dest;
    s8 hour = (gMapHeader.useNightTint) ? HOUR_MIDNIGHT_0 : gLocalTime.hours;

    for (i = 0; i < ARRAY_COUNT(gPaletteOverrides); i++)
    {
        const struct PaletteOverride *curr = gPaletteOverrides[i];

        if (curr)
        {
            while (curr->slot != PALOVER_LIST_TERM && curr->palette)
            {
                if ((curr->startHour < curr->endHour && hour >= curr->startHour && hour < curr->endHour)
                 || (curr->startHour > curr->endHour && (hour >= curr->startHour
                 || hour < curr->endHour)))
                {
                    for (j = 0, src = curr->palette, dest = gPlttBufferUnfaded + (curr->slot * 16); j < 16; j++, src++, dest++)
                    {
                        if (*src != RGB_BLACK)
                            *dest = *src;
                    }
                }
                curr++;
            }
        }
    }
}

static bool8 ShouldTintOverworld(void)
{
    if (IsMapTypeOutdoors(gMapHeader.mapType))
        return TRUE;

    // more conditions?
    return FALSE;
}

void TintPaletteForDayNight(u16 offset, u16 size)
{
    s8 hour, nextHour;
    u8 hourPhase;
    u16 period;

    if (ShouldTintOverworld())
    {
        RtcCalcLocalTimeFast();
        if (gMapHeader.useNightTint)
        {
            hour = HOUR_MIDNIGHT_0;
            hourPhase = 0;
        }
        else
        {
            hour = gLocalTime.hours;
            hourPhase = gLocalTime.minutes / MINUTES_PER_TINT_PERIOD;
        }

        period = (hour * TINT_PERIODS_PER_HOUR) + hourPhase;
        if (!sDNSystemControl.initialized || sDNSystemControl.currTintPeriod != period)
        {
            sDNSystemControl.initialized = TRUE;
            sDNSystemControl.currTintPeriod = period;
            nextHour = (hour + 1) % 24;
            LerpColors(sDNSystemControl.currRGBTint, sTimeOfDayTints[hour], sTimeOfDayTints[nextHour], hourPhase);
        }
        TintPalette_CustomToneWithCopy(gPlttBufferPreDN + offset, gPlttBufferUnfaded + offset, size / 2, sDNSystemControl.currRGBTint[0], sDNSystemControl.currRGBTint[1], sDNSystemControl.currRGBTint[2], FALSE);
    }
    else
        CpuCopy16(gPlttBufferPreDN + offset, gPlttBufferUnfaded + offset, size);
    LoadPaletteOverrides();
}

void CheckClockForImmediateTimeEvents(void)
{
    if (!sDNSystemControl.retintPhase && ShouldTintOverworld())
        RtcCalcLocalTimeFast();
}

void ProcessImmediateTimeEvents(void)
{
    s8 hour, nextHour;
    u8 hourPhase;
    u16 period;
    u8 timeOfDay = GetCurrentTimeOfDay();

    if (ShouldTintOverworld())
    {
        if (sDNSystemControl.retintPhase)
        {
            sDNSystemControl.retintPhase = FALSE;
            TintPalette_CustomToneWithCopy(gPlttBufferPreDN + (BG_PLTT_SIZE / 2), gPlttBufferUnfaded + (BG_PLTT_SIZE / 2), OBJ_PLTT_SIZE / 2, sDNSystemControl.currRGBTint[0], sDNSystemControl.currRGBTint[1], sDNSystemControl.currRGBTint[2], TRUE);
            LoadPaletteOverrides();

            if (gWeatherPtr->palProcessingState != WEATHER_PAL_STATE_SCREEN_FADING_IN
             && gWeatherPtr->palProcessingState != WEATHER_PAL_STATE_SCREEN_FADING_OUT)
                CpuCopy16(gPlttBufferUnfaded, gPlttBufferFaded, PLTT_SIZE);
        }
        else
        {
            if (gMapHeader.useNightTint)
            {
                hour = HOUR_MIDNIGHT_0;
                hourPhase = 0;
            }
            else
            {
                hour = gLocalTime.hours;
                hourPhase = gLocalTime.minutes / MINUTES_PER_TINT_PERIOD;
            }

            period = (hour * TINT_PERIODS_PER_HOUR) + hourPhase;
            if (!sDNSystemControl.initialized
             || sDNSystemControl.prevTintPeriod != period)
            {
                sDNSystemControl.initialized = TRUE;
                sDNSystemControl.prevTintPeriod = sDNSystemControl.currTintPeriod = period;
                nextHour = (hour + 1) % 24;
                LerpColors(sDNSystemControl.currRGBTint, sTimeOfDayTints[hour], sTimeOfDayTints[nextHour], hourPhase);
                TintPalette_CustomToneWithCopy(gPlttBufferPreDN, gPlttBufferUnfaded, BG_PLTT_SIZE / 2, sDNSystemControl.currRGBTint[0], sDNSystemControl.currRGBTint[1], sDNSystemControl.currRGBTint[2], TRUE);
                sDNSystemControl.retintPhase = TRUE;
            }
        }
    }

    if (sDNSystemControl.timeOfDay != timeOfDay)
    {
        sDNSystemControl.timeOfDay = timeOfDay;
        ForceTimeBasedEvents(); // for misc events that should run on time of day boundaries
    }
}
