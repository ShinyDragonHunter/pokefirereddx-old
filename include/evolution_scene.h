#ifndef GUARD_EVOLUTION_SCENE_H
#define GUARD_EVOLUTION_SCENE_H

void BeginEvolutionScene(struct Pokemon* mon, u16 postEvoSpecies, bool8 canStopEvo, u8 partyID, u8 postEvoForm);
void EvolutionScene(struct Pokemon* mon, u16 postEvoSpecies, bool8 canStopEvo, u8 partyID, u8 postEvoForm);
void TradeEvolutionScene(struct Pokemon* mon, u16 postEvoSpecies, u8 preEvoSpriteID, u8 partyID, u8 postEvoForm);

extern void (*gCB2_AfterEvolution)(void);

#endif // GUARD_EVOLUTION_SCENE_H
