EventScript_StartTrainerApproach::
	selectapproachingtrainer
	lockfortrainer
EventScript_TrainerApproach::
	special PlayTrainerEncounterMusic
	special DoTrainerApproach
	waitstate
	goto EventScript_ShowTrainerIntroMsg

EventScript_TryDoNormalTrainerBattle::
	lock
	faceplayer
	applymovement VAR_LAST_TALKED, Movement_RevealTrainer
	waitmovement 0
	specialvar VAR_RESULT, GetTrainerFlag
	compare VAR_RESULT, FALSE
	goto_if_ne EventScript_NoNormalTrainerBattle
	special PlayTrainerEncounterMusic
	special SetTrainerFacingDirection
	goto EventScript_ShowTrainerIntroMsg

EventScript_NoNormalTrainerBattle::
	gotopostbattlescript

EventScript_TryDoDoubleTrainerBattle::
	lock
	faceplayer
	call EventScript_RevealTrainer
	specialvar VAR_RESULT, GetTrainerFlag
	compare VAR_RESULT, FALSE
	goto_if_ne EventScript_NoNormalTrainerBattle
	special HasEnoughMonsForDoubleBattle
	compare VAR_RESULT, PLAYER_HAS_TWO_USABLE_MONS
	goto_if_ne EventScript_NotEnoughMonsForDoubleBattle
	special PlayTrainerEncounterMusic
	special SetTrainerFacingDirection
	goto EventScript_ShowTrainerIntroMsg

EventScript_NotEnoughMonsForDoubleBattle::
	special ShowTrainerCantBattleSpeech
	waitmessage
	waitbuttonpress
	release
	end

EventScript_DoNoIntroTrainerBattle::
	applymovement VAR_LAST_TALKED, Movement_RevealTrainer
	waitmovement 0
	special PlayTrainerEncounterMusic
	trainerbattlebegin
	goto EventScript_NoNormalTrainerBattle

EventScript_TryDoRematchBattle::
	call EventScript_RevealTrainer
	specialvar VAR_RESULT, IsTrainerReadyForRematch
	compare VAR_RESULT, FALSE
	goto_if_eq EventScript_NoNormalTrainerBattle
	special PlayTrainerEncounterMusic
	special SetTrainerFacingDirection
	special ShowTrainerIntroSpeech
	waitmessage
	waitbuttonpress
	special BattleSetup_StartRematchBattle
	waitstate
	releaseall
	end

EventScript_TryDoDoubleRematchBattle::
	specialvar VAR_RESULT, IsTrainerReadyForRematch
	compare VAR_RESULT, FALSE
	goto_if_eq EventScript_NoNormalTrainerBattle
	special HasEnoughMonsForDoubleBattle
	compare VAR_RESULT, PLAYER_HAS_TWO_USABLE_MONS
	goto_if_ne EventScript_NotEnoughMonsForDoubleRematchBattle
	special PlayTrainerEncounterMusic
	special SetTrainerFacingDirection
	special ShowTrainerIntroSpeech
	waitmessage
	waitbuttonpress
	special BattleSetup_StartRematchBattle
	waitstate
	releaseall
	end

EventScript_NotEnoughMonsForDoubleRematchBattle::
	special ShowTrainerCantBattleSpeech
	waitmessage
	waitbuttonpress
	release
	end

EventScript_RevealTrainer::
	applymovement VAR_LAST_TALKED, Movement_RevealTrainer
	waitmovement 0
	return

Movement_RevealTrainer:
	reveal_trainer
	step_end

EventScript_ShowTrainerIntroMsg::
	special ShowTrainerIntroSpeech
	waitmessage
	waitbuttonpress
	special TryPrepareSecondApproachingTrainer
	compare VAR_RESULT, TRUE
	goto_if_eq EventScript_TrainerApproach
	goto EventScript_DoTrainerBattle

EventScript_DoTrainerBattle::
	trainerbattlebegin
	gotobeatenscript
	releaseall
	end

Std_MsgboxAutoclose::
	message 0x0
	waitmessage
	waitbuttonpress
	release
	return
