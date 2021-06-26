Common_EventScript_SetupRivalOnBikeGfxId:: @ 8271EFB
	checkplayergender
	compare VAR_RESULT, FEMALE
	goto_if_eq EventScript_SetupRivalOnBikeGfxIdMale
	setvar VAR_OBJ_GFX_ID_3, OBJ_EVENT_GFX_LEAF_BIKE
	return

EventScript_SetupRivalOnBikeGfxIdMale:: @ 8271F19
	setvar VAR_OBJ_GFX_ID_3, OBJ_EVENT_GFX_RED_BIKE
	return
