#ifndef GUARD_BATTLE_TRANSITION_H
#define GUARD_BATTLE_TRANSITION_H

void TestBattleTransition(u8 transitionId);
void BattleTransition_StartOnField(u8 transitionId);
void BattleTransition_Start(u8 transitionId);
bool8 IsBattleTransitionDone(void);
bool8 FldEff_Pokeball(void);
void TransitionPhase1_Task_RunFuncs(u8 taskId);
void GetBg0TilesDst(u16 **tilemap, u16 **tileset);

extern const struct SpritePalette gSpritePalette_Pokeball;

enum // TRANSITION_MUGSHOT
{
    MUGSHOT_LORELEI,
    MUGSHOT_BRUNO,
    MUGSHOT_AGATHA,
    MUGSHOT_LANCE,
    MUGSHOT_CHAMPION,
    MUGSHOTS_COUNT
};

// credits for the names go to Dyskinesia, Tetrable and Farore
// names are naturally subject to change

#define B_TRANSITION_BLUR                                      0
#define B_TRANSITION_SWIRL                                     1
#define B_TRANSITION_SHUFFLE                                   2
#define B_TRANSITION_BIG_POKEBALL                              3
#define B_TRANSITION_POKEBALLS_TRAIL                           4
#define B_TRANSITION_CLOCKWISE_BLACKFADE                       5
#define B_TRANSITION_RIPPLE                                    6
#define B_TRANSITION_WAVE                                      7
#define B_TRANSITION_SLICE                                     8
#define B_TRANSITION_WHITEFADE                                 9
#define B_TRANSITION_GRID_SQUARES                              10
#define B_TRANSITION_SHARDS                                    11
#define B_TRANSITION_LORELEI                                   12
#define B_TRANSITION_BRUNO                                     13
#define B_TRANSITION_AGATHA                                    14
#define B_TRANSITION_LANCE                                     15
#define B_TRANSITION_CHAMPION                                  16
// added in Emerald
#define B_TRANSITION_MAGMA                                     17
#define B_TRANSITION_RAYQUAZA                                  18
#define B_TRANSITION_SHRED_SPLIT                               19
#define B_TRANSITION_BLACKHOLE1                                20
#define B_TRANSITION_BLACKHOLE2                                21
#define B_TRANSITION_RECTANGULAR_SPIRAL                        22
#define B_TRANSITION_FRONTIER_LOGO_WIGGLE                      23
#define B_TRANSITION_FRONTIER_LOGO_WAVE                        24
#define B_TRANSITION_FRONTIER_SQUARES                          25
#define B_TRANSITION_FRONTIER_SQUARES_SCROLL                   26
#define B_TRANSITION_FRONTIER_SQUARES_SPIRAL                   27
#define B_TRANSITION_FRONTIER_CIRCLES_MEET                     28
#define B_TRANSITION_FRONTIER_CIRCLES_CROSS                    29
#define B_TRANSITION_FRONTIER_CIRCLES_ASYMMETRIC_SPIRAL        30
#define B_TRANSITION_FRONTIER_CIRCLES_SYMMETRIC_SPIRAL         31
#define B_TRANSITION_FRONTIER_CIRCLES_MEET_IN_SEQ              32
#define B_TRANSITION_FRONTIER_CIRCLES_CROSS_IN_SEQ             33
#define B_TRANSITION_FRONTIER_CIRCLES_ASYMMETRIC_SPIRAL_IN_SEQ 34
#define B_TRANSITION_FRONTIER_CIRCLES_SYMMETRIC_SPIRAL_IN_SEQ  35
#define B_TRANSITION_COUNT                                     36

#endif // GUARD_BATTLE_TRANSITION_H
