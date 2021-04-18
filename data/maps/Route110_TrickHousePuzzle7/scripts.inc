Route110_TrickHousePuzzle7_MapScripts:: @ 826E091
	map_script MAP_SCRIPT_ON_TRANSITION, Route110_TrickHousePuzzle7_OnTransition
	map_script MAP_SCRIPT_ON_FRAME_TABLE, Route110_TrickHousePuzzle7_OnFrame
	.byte 0

Route110_TrickHousePuzzle7_OnTransition: @ 826E198
	compare VAR_TRICK_HOUSE_PUZZLE_7_STATE_2, 1
	goto_if_eq Route110_TrickHousePuzzle7_EventScript_TeleportedTransition
	clearflag FLAG_TRICK_HOUSE_PUZZLE_7_SWITCH_1
	clearflag FLAG_TRICK_HOUSE_PUZZLE_7_SWITCH_2
	clearflag FLAG_TRICK_HOUSE_PUZZLE_7_SWITCH_3
	clearflag FLAG_TRICK_HOUSE_PUZZLE_7_SWITCH_4
	clearflag FLAG_TRICK_HOUSE_PUZZLE_7_SWITCH_5
Route110_TrickHousePuzzle7_EventScript_TeleportedTransition:: @ 826E1B3
	end

Route110_TrickHousePuzzle7_OnFrame: @ 826E1C0
	map_script_2 VAR_TRICK_HOUSE_PUZZLE_7_STATE_2, 1, Route110_TrickHousePuzzle7_EventScript_ClearState2
	.2byte 0

Route110_TrickHousePuzzle7_EventScript_ClearState2:: @ 826E1CA
	setvar VAR_TRICK_HOUSE_PUZZLE_7_STATE_2, 0
	end

Route110_TrickHousePuzzle7_EventScript_Scroll:: @ 826E1D0
	lockall
	compare VAR_TRICK_HOUSE_PUZZLE_7_STATE, 0
	goto_if_eq Route110_TrickHousePuzzle7_EventScript_FoundScroll
	goto Route110_TrickHousePuzzle_EventScript_ReadScrollAgain
	end

Route110_TrickHousePuzzle7_EventScript_FoundScroll:: @ 826E1E2
	setvar VAR_TRICK_HOUSE_PUZZLE_7_STATE, 1
	goto Route110_TrickHousePuzzle_EventScript_FoundScroll
	end

@ From here is where the new, actually used puzzle scripts begin
Route110_TrickHousePuzzle7_EventScript_YellowButton:: @ 826E31B
	playse SE_SWITCH
	waitse
	playse SE_REPEL
	initrotatingtilepuzzle TRUE
	moverotatingtileobjects 0
	waitmovement 0
	turnrotatingtileobjects
	waitmovement 0
	freerotatingtilepuzzle
	end

Route110_TrickHousePuzzle7_EventScript_BlueButton:: @ 826E331
	playse SE_SWITCH
	waitse
	playse SE_REPEL
	initrotatingtilepuzzle TRUE
	moverotatingtileobjects 1
	waitmovement 0
	turnrotatingtileobjects
	waitmovement 0
	freerotatingtilepuzzle
	end

Route110_TrickHousePuzzle7_EventScript_GreenButton:: @ 826E347
	playse SE_SWITCH
	waitse
	playse SE_REPEL
	initrotatingtilepuzzle TRUE
	moverotatingtileobjects 2
	waitmovement 0
	turnrotatingtileobjects
	waitmovement 0
	freerotatingtilepuzzle
	end

Route110_TrickHousePuzzle7_EventScript_PurpleButton:: @ 826E35D
	playse SE_SWITCH
	waitse
	playse SE_REPEL
	initrotatingtilepuzzle TRUE
	moverotatingtileobjects 3
	waitmovement 0
	turnrotatingtileobjects
	waitmovement 0
	freerotatingtilepuzzle
	end

Route110_TrickHousePuzzle7_EventScript_Joshua:: @ 826E389
	trainerbattle_single TRAINER_JOSHUA, Route110_TrickHousePuzzle7_Text_JoshuaIntro, Route110_TrickHousePuzzle7_Text_JoshuaDefeat
	msgbox Route110_TrickHousePuzzle7_Text_JoshuaPostBattle, MSGBOX_AUTOCLOSE
	end

Route110_TrickHousePuzzle7_EventScript_Patricia:: @ 826E3A0
	trainerbattle_single TRAINER_PATRICIA, Route110_TrickHousePuzzle7_Text_PatriciaIntro, Route110_TrickHousePuzzle7_Text_PatriciaDefeat
	msgbox Route110_TrickHousePuzzle7_Text_PatriciaPostBattle, MSGBOX_AUTOCLOSE
	end

Route110_TrickHousePuzzle7_EventScript_Alexis:: @ 826E3B7
	trainerbattle_single TRAINER_ALEXIS, Route110_TrickHousePuzzle7_Text_AlexisIntro, Route110_TrickHousePuzzle7_Text_AlexisDefeat
	msgbox Route110_TrickHousePuzzle7_Text_AlexisPostBattle, MSGBOX_AUTOCLOSE
	end

Route110_TrickHousePuzzle7_EventScript_Mariela:: @ 826E3CE
	trainerbattle_single TRAINER_MARIELA, Route110_TrickHousePuzzle7_Text_MarielaIntro, Route110_TrickHousePuzzle7_Text_MarielaDefeat
	msgbox Route110_TrickHousePuzzle7_Text_MarielaPostBattle, MSGBOX_AUTOCLOSE
	end

Route110_TrickHousePuzzle7_EventScript_Alvaro:: @ 826E3E5
	trainerbattle_single TRAINER_ALVARO, Route110_TrickHousePuzzle7_Text_AlvaroIntro, Route110_TrickHousePuzzle7_Text_AlvaroDefeat
	msgbox Route110_TrickHousePuzzle7_Text_AlvaroPostBattle, MSGBOX_AUTOCLOSE
	end

Route110_TrickHousePuzzle7_EventScript_Everett:: @ 826E3FC
	trainerbattle_single TRAINER_EVERETT, Route110_TrickHousePuzzle7_Text_EverettIntro, Route110_TrickHousePuzzle7_Text_EverettDefeat
	msgbox Route110_TrickHousePuzzle7_Text_EverettPostBattle, MSGBOX_AUTOCLOSE
	end

Route110_TrickHousePuzzle7_EventScript_WroteSecretCodeLockOpened:: @ 826E413
	.string "{PLAYER} wrote down the secret code\n"
	.string "on the door.\p"
	.string "“TRICK MASTER is huggable.”\n"
	.string "… … … … … … … …\p"
	.string "The lock clicked open!$"

Route110_TrickHousePuzzle7_Text_JoshuaIntro: @ 826E481
	.string "The TRICK MASTER always vanishes\n"
	.string "like smoke. How does he do it?$"

Route110_TrickHousePuzzle7_Text_JoshuaDefeat: @ 826E4C1
	.string "Aiyeeeh! You're much too strong!\n"
	.string "How do you do it?$"

Route110_TrickHousePuzzle7_Text_JoshuaPostBattle: @ 826E4F4
	.string "I wish I could appear and disappear as\n"
	.string "if I were smoke, too.$"

Route110_TrickHousePuzzle7_Text_PatriciaIntro: @ 826E531
	.string "Going around the same spot…\n"
	.string "It begets ill fortune…$"

Route110_TrickHousePuzzle7_Text_PatriciaDefeat: @ 826E564
	.string "Defeated!\n"
	.string "It's a bad sign…$"

Route110_TrickHousePuzzle7_Text_PatriciaPostBattle: @ 826E57F
	.string "I've circled the same spot over ten\n"
	.string "times now… It's ill fortune…$"

Route110_TrickHousePuzzle7_Text_AlexisIntro: @ 826E5C0
	.string "Whoever wins will get through here\n"
	.string "first. That's the feeling I get.$"

Route110_TrickHousePuzzle7_Text_AlexisDefeat: @ 826E604
	.string "Oh!\n"
	.string "Well, go ahead, then!$"

Route110_TrickHousePuzzle7_Text_AlexisPostBattle: @ 826E61E
	.string "You're solving all the puzzles in the\n"
	.string "TRICK HOUSE. That's the feeling I get.$"

Route110_TrickHousePuzzle7_Text_MarielaIntro: @ 826E66B
	.string "Nufufufu, here at last!\n"
	.string "Let's get right with it!$"

Route110_TrickHousePuzzle7_Text_MarielaDefeat: @ 826E69C
	.string "You're so casual about winning!$"

Route110_TrickHousePuzzle7_Text_MarielaPostBattle: @ 826E6BC
	.string "Humph! I'm not upset!\n"
	.string "Not me!$"

Route110_TrickHousePuzzle7_Text_AlvaroIntro: @ 826E6DA
	.string "I ever so closely watched you coming!$"

Route110_TrickHousePuzzle7_Text_AlvaroDefeat: @ 826E700
	.string "This outcome I didn't see coming…$"

Route110_TrickHousePuzzle7_Text_AlvaroPostBattle: @ 826E722
	.string "Well, anyway, we both picked a weird\n"
	.string "place to get acquainted.\p"
	.string "As one weirdo to another,\n"
	.string "let's do our best!$"

Route110_TrickHousePuzzle7_Text_EverettIntro: @ 826E78D
	.string "It's awfully cramped in here…$"

Route110_TrickHousePuzzle7_Text_EverettDefeat: @ 826E7AB
	.string "Oh, yes, strong you are.$"

Route110_TrickHousePuzzle7_Text_EverettPostBattle: @ 826E7C4
	.string "I was hoping to switch places with you\n"
	.string "when I beat you, but…$"
