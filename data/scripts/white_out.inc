EventScript_AfterWhiteOutHeal::
	lockall
	textcolor RED
	message gText_FirstShouldRestoreMonsHealth
	call EventScript_PkmnCenterNurse_TakeAndHealPkmn
	call_if_unset FLAG_BADGE01_GET, EventScript_AfterWhiteOutHealMsgPreBrock
	call_if_set FLAG_BADGE01_GET, EventScript_AfterWhiteOutHealMsg
	applymovement VAR_LAST_TALKED, Movement_PkmnCenterNurse_Bow
	waitmovement 0
	goto EventScript_AfterWhiteOutEnd

EventScript_AfterWhiteOutHealMsgPreBrock::
	message gText_MonsHealedShouldBuyPotions
	return

EventScript_AfterWhiteOutHealMsg::
	message gText_MonsHealed
	return

EventScript_AfterWhiteOutMomHeal::
	lockall
	applymovement LOCALID_MOM, Common_Movement_WalkInPlaceFastestDown
	waitmovement 0
	textcolor RED
	message gText_HadQuiteAnExperienceTakeRest
	call Common_EventScript_OutOfCenterPartyHeal
	message gText_MomExplainHPGetPotions
EventScript_AfterWhiteOutEnd::
	fadedefaultbgm
	releaseall
	end
