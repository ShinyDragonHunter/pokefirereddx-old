#ifndef GUARD_CONSTANTS_DAY_NIGHT_H
#define GUARD_CONSTANTS_DAY_NIGHT_H

#define PALOVER_LIST_TERM 0xFF

#define TINT_MORNING Q_8_8(0.7), Q_8_8(0.7), Q_8_8(0.9)
#define TINT_DAY Q_8_8(1.0), Q_8_8(1.0), Q_8_8(1.0)
#define TINT_NIGHT Q_8_8(0.6), Q_8_8(0.6), Q_8_8(0.92)

#define HOUR_MORNING 4
#define HOUR_DAY 10
#define HOUR_NIGHT 20

// not real times of day but useful for specifying override times
#define HOUR_AFTERNOON 18
#define HOUR_MIDNIGHT_0 0
#define HOUR_MIDNIGHT_24 24

#define TIME_MORNING 0
#define TIME_DAY 1
#define TIME_NIGHT 2
#define TIMES_OF_DAY_COUNT 3

#define DAY_SUNDAY 0
#define DAY_MONDAY 1
#define DAY_TUESDAY 2
#define DAY_WEDNESDAY 3
#define DAY_THURSDAY 4
#define DAY_FRIDAY 5
#define DAY_SATURDAY 6

#define TINT_PERIODS_PER_HOUR 60
#define MINUTES_PER_TINT_PERIOD (60 / TINT_PERIODS_PER_HOUR)
#define TINT_PERIODS_COUNT (24 * TINT_PERIODS_PER_HOUR)

#endif  // GUARD_CONSTANTS_DAY_NIGHT_H
