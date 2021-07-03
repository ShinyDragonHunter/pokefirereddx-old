Common_EventScript_SetupRivalOnBikeGfxId:: @ 8271EFB
	checkplayergender
	compare VAR_RESULT, MALE
	goto_if_eq EventScript_SetupRivalOnBikeGfxIdFemale
	goto_if_ne EventScript_SetupRivalOnBikeGfxIdMale
	end

EventScript_SetupRivalOnBikeGfxIdFemale:: @ 8271F19
	setvar VAR_OBJ_GFX_ID_3, OBJ_EVENT_GFX_LEAF_BIKE
	return

EventScript_SetupRivalOnBikeGfxIdMale:: @ 8271F19
	setvar VAR_OBJ_GFX_ID_3, OBJ_EVENT_GFX_RED_BIKE
	return
