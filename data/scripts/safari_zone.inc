SafariZone_EventScript_OutOfBallsMidBattle::
	setvar VAR_SAFARI_ZONE_STATE, 1
	special ExitSafariMode
	setwarp MAP_ROUTE121_SAFARI_ZONE_ENTRANCE, 255, 2, 5
	end

SafariZone_EventScript_Exit::
	setvar VAR_SAFARI_ZONE_STATE, 1
	special ExitSafariMode
	warp MAP_ROUTE121_SAFARI_ZONE_ENTRANCE, 255, 2, 5
	waitstate
	end

SafariZone_EventScript_RetirePrompt::
	lockall
	msgbox SafariZone_Text_WouldYouLikeToExit, MSGBOX_YESNO
	compare VAR_RESULT, YES
	goto_if_eq SafariZone_EventScript_Exit
	releaseall
	end

SafariZone_EventScript_TimesUp::
	lockall
	playse SE_DING_DONG
	message SafariZone_Text_TimesUp
	waitmessage
	waitbuttonpress
	releaseall
	goto SafariZone_EventScript_Exit

SafariZone_EventScript_OutOfBalls::
	lockall
	playse SE_DING_DONG
	message SafariZone_Text_OutOfBalls
	waitmessage
	waitbuttonpress
	releaseall
	goto SafariZone_EventScript_Exit

SafariZone_Text_WouldYouLikeToExit:
	.string "Would you like to exit the SAFARI\n"
	.string "ZONE right now?$"

SafariZone_Text_TimesUp:
	.string "PA: Ding-dong! Time's up!\n"
	.string "Your SAFARI Game is over.$"

SafariZone_Text_OutOfBalls:
	.string "PA: Ding-dong!\p"
	.string "You've run out of SAFARI BALLS.\n"
	.string "Your SAFARI Game is over.$"
