EventScript_FieldPoison::
	lockall
	special TryFieldPoisonWhiteOut
	waitstate
	compare VAR_RESULT, FLDPSN_WHITEOUT
	goto_if_eq EventScript_FieldWhiteOut
	compare VAR_RESULT, FLDPSN_FRONTIER_WHITEOUT
	goto_if_eq EventScript_FrontierFieldWhiteOut
	releaseall
	end

EventScript_FieldWhiteOut::
	checkmoney 1, 0
	compare VAR_RESULT, FALSE
	goto_if_eq EventScript_FieldWhiteOutNoMoney
	compare VAR_RESULT, TRUE
	goto_if_eq EventScript_FieldWhiteOutHasMoney
	end

EventScript_FieldWhiteOutNoMoney::
	message gText_PlayerWhitedOut
	waitmessage
	waitbuttonpress
	goto EventScript_FieldWhiteOutFade
	end

EventScript_FieldWhiteOutHasMoney::
	special OverworldWhiteOutGetMoneyLoss
	msgbox gText_PlayerWhitedOutLostMoney
	waitmessage
	waitbuttonpress
	goto EventScript_FieldWhiteOutFade
	end

EventScript_FrontierFieldWhiteOut::
	message gText_PlayerWhitedOut
	waitmessage
	waitbuttonpress
	pike_inchallenge
	compare VAR_RESULT, TRUE
	goto_if_eq BattleFrontier_BattlePike_EventScript_Retire
	pyramid_inchallenge
	compare VAR_RESULT, 0
	goto_if_ne BattleFrontier_BattlePyramid_EventScript_WarpToLobbyLost
	trainerhill_inchallenge
	compare VAR_RESULT, TRUE
	goto_if_eq TrainerHill_1F_EventScript_Lost

EventScript_FieldWhiteOutFade::
	special Script_FadeOutMapMusic
	waitstate
	fadescreen FADE_TO_BLACK
	special SetCB2WhiteOut
	waitstate
	end
