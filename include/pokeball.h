#ifndef GUARD_POKEBALL_H
#define GUARD_POKEBALL_H

enum
{
    BALL_POKE,
    BALL_GREAT,
    BALL_SAFARI,
    BALL_ULTRA,
    BALL_MASTER,
    BALL_NET,
    BALL_DIVE,
    BALL_NEST,
    BALL_REPEAT,
    BALL_TIMER,
    BALL_LUXURY,
    BALL_PREMIER,
    BALL_LEVEL,
    BALL_LURE,
    BALL_MOON,
    BALL_FRIEND,
    BALL_FAST,
    BALL_HEAVY,
    BALL_LOVE,
    BALL_PARK,
    POKEBALL_COUNT
};

enum
{
    NONE,
    MASTER_BALL,
    ULTRA_BALL,
    GREAT_BALL,
    POKE_BALL,
    SAFARI_BALL,
    NET_BALL,
    DIVE_BALL,
    NEST_BALL,
    REPEAT_BALL,
    TIMER_BALL,
    LUXURY_BALL,
    PREMIER_BALL,
    LEVEL_BALL,
    LURE_BALL,
    MOON_BALL,
    FRIEND_BALL,
    FAST_BALL,
    HEAVY_BALL,
    LOVE_BALL,
    PARK_BALL
};

enum {
    BALL_AFFINE_ANIM_0,
    BALL_ROTATE_RIGHT,
    BALL_ROTATE_LEFT, 
    BALL_AFFINE_ANIM_3,
    BALL_AFFINE_ANIM_4
};

extern const struct SpriteTemplate gBallSpriteTemplates[];

#define POKEBALL_PLAYER_SENDOUT     0xFF
#define POKEBALL_OPPONENT_SENDOUT   0xFE

u8 DoPokeballSendOutAnimation(s16 pan, u8 kindOfThrow);
void CreatePokeballSpriteToReleaseMon(u8 monSpriteId, u8 battler, u8 x, u8 y, u8 oamPriority, u8 subpriortiy, u8 g, u32 h, u16 species);
u8 CreateTradePokeballSprite(u8 a, u8 b, u8 x, u8 y, u8 oamPriority, u8 subPriority, u8 g, u32 h);
void StartHealthboxSlideIn(u8 battler);
void DoHitAnimHealthboxEffect(u8 battler);
void LoadBallGfx(u8 ballId);
void FreeBallGfx(u8 ballId);

#endif // GUARD_POKEBALL_H
