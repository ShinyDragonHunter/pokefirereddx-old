EventScript_RepelWoreOff::
	lockall
	checkitem VAR_REPEL_LAST_USED, 1
	compare VAR_RESULT, TRUE
	goto_if_eq EventScript_RepelUseAnother
	msgbox Text_RepelWoreOff, MSGBOX_SIGN
	end

EventScript_RepelUseAnother:
	msgbox Text_RepelUseAnother, MSGBOX_YESNO
	compare VAR_RESULT, YES
	goto_if_eq EventScript_UseNewRepel
EventScript_RepelEnd:
	releaseall
	end

EventScript_UseNewRepel:
	bufferitemname 0, VAR_REPEL_LAST_USED
	playse SE_REPEL
	msgbox Text_UsedNewRepel, MSGBOX_SIGN
	removeitem VAR_REPEL_LAST_USED, 1
	waitse
	callnative HandleUseExpiredRepel
	goto EventScript_RepelEnd

Text_RepelWoreOff::
	.string "REPEL's effect wore off…$"

Text_RepelUseAnother::
	.string "REPEL's effect wore off…\n"
	.string "Use another?$"

Text_UsedNewRepel::
	.string "{PLAYER} used the\n"
	.string "{STR_VAR_1}.$"
