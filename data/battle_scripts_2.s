#include "constants/battle.h"
#include "constants/battle_script_commands.h"
#include "constants/battle_anim.h"
#include "constants/battle_string_ids.h"
#include "constants/items.h"
#include "constants/songs.h"
#include "constants/game_stat.h"
	.include "asm/macros.inc"
	.include "asm/macros/battle_script.inc"
	.include "constants/constants.inc"

	.section script_data, "aw", %progbits

	.align 2
gBattlescriptsForBallThrow::
	.4byte BattleScript_BallThrow        @ BALL_NONE
	.4byte BattleScript_BallThrow        @ BALL_MASTER
	.4byte BattleScript_BallThrow        @ BALL_ULTRA
	.4byte BattleScript_BallThrow        @ BALL_GREAT
	.4byte BattleScript_BallThrow        @ BALL_POKE
	.4byte BattleScript_SafariBallThrow  @ BALL_SAFARI
	.4byte BattleScript_BallThrow        @ BALL_NET
	.4byte BattleScript_BallThrow        @ BALL_DIVE
	.4byte BattleScript_BallThrow        @ BALL_NEST
	.4byte BattleScript_BallThrow        @ BALL_REPEAT
	.4byte BattleScript_BallThrow        @ BALL_TIMER
	.4byte BattleScript_BallThrow        @ BALL_LUXURY
	.4byte BattleScript_BallThrow        @ BALL_PREMIER
	.4byte BattleScript_BallThrow        @ BALL_LEVEL
	.4byte BattleScript_BallThrow        @ BALL_LURE
	.4byte BattleScript_BallThrow        @ BALL_MOON
	.4byte BattleScript_BallThrow        @ BALL_FRIEND
	.4byte BattleScript_BallThrow        @ BALL_FAST
	.4byte BattleScript_BallThrow        @ BALL_HEAVY
	.4byte BattleScript_BallThrow        @ BALL_LOVE
	.4byte BattleScript_BallThrow        @ BALL_PARK

	.align 2
gBattlescriptsForUsingItem::
	.4byte BattleScript_PlayerUsesItem
	.4byte BattleScript_OpponentUsesHealItem        @ AI_ITEM_FULL_RESTORE
	.4byte BattleScript_OpponentUsesHealItem        @ AI_ITEM_HEAL_HP
	.4byte BattleScript_OpponentUsesStatusCureItem  @ AI_ITEM_CURE_CONDITION
	.4byte BattleScript_OpponentUsesXItem           @ AI_ITEM_X_STAT
	.4byte BattleScript_OpponentUsesGuardSpec       @ AI_ITEM_GUARD_SPEC

	.align 2
gBattlescriptsForRunningByItem::
	.4byte BattleScript_RunByUsingItem
	.4byte BattleScript_ActionUsePokeFlute

	.align 2
gBattlescriptsForSafariActions::
	.4byte BattleScript_ActionWatchesCarefully
	.4byte BattleScript_ActionThrowRock
	.4byte BattleScript_ActionThrowBait
	.4byte BattleScript_ActionWallyThrow

BattleScript_BallThrow::
	jumpifword CMP_COMMON_BITS, gBattleTypeFlags, BATTLE_TYPE_WALLY_TUTORIAL, BattleScript_BallThrowByWally
	printstring STRINGID_PLAYERUSEDITEM
	handleballthrow

BattleScript_BallThrowByWally::
	printstring STRINGID_WALLYUSEDITEM
	handleballthrow

BattleScript_SafariBallThrow::
	printstring STRINGID_PLAYERUSEDITEM
	updatestatusicon BS_ATTACKER
	handleballthrow

BattleScript_SuccessBallThrow::
	jumpifhalfword CMP_EQUAL, gLastUsedItem, ITEM_SAFARI_BALL, BattleScript_PrintCaughtMonInfo
	incrementgamestat GAME_STAT_POKEMON_CAPTURES
BattleScript_PrintCaughtMonInfo::
	printstring STRINGID_GOTCHAPKMNCAUGHT
	trysetcaughtmondexflags BattleScript_TryNicknameCaughtMon
	printstring STRINGID_PKMNDATAADDEDTODEX
	waitstate
	setbyte gBattleCommunication, 0
	displaydexinfo
BattleScript_TryNicknameCaughtMon::
	printstring STRINGID_GIVENICKNAMECAPTURED
	waitstate
	setbyte gBattleCommunication, 0
	trygivecaughtmonnick BattleScript_GiveCaughtMonEnd
	givecaughtmon
	printfromtable gCaughtMonStringIds
	waitmessage B_WAIT_TIME_LONG
	goto BattleScript_SuccessBallThrowEnd
BattleScript_GiveCaughtMonEnd::
	givecaughtmon
BattleScript_SuccessBallThrowEnd::
	setbyte gBattleOutcome, B_OUTCOME_CAUGHT
	finishturn

BattleScript_WallyBallThrow::
	printstring STRINGID_GOTCHAPKMNCAUGHT2
	setbyte gBattleOutcome, B_OUTCOME_CAUGHT
	finishturn

BattleScript_ShakeBallThrow::
	printfromtable gBallEscapeStringIds
	waitmessage B_WAIT_TIME_LONG
	jumpifword CMP_NO_COMMON_BITS, gBattleTypeFlags, BATTLE_TYPE_SAFARI, BattleScript_ShakeBallThrowEnd
	jumpifbyte CMP_NOT_EQUAL, gNumSafariBalls, 0, BattleScript_ShakeBallThrowEnd
	printstring STRINGID_OUTOFSAFARIBALLS
	waitmessage B_WAIT_TIME_LONG
	setbyte gBattleOutcome, B_OUTCOME_NO_SAFARI_BALLS
BattleScript_ShakeBallThrowEnd::
	finishaction

BattleScript_TrainerBallBlock::
	waitmessage B_WAIT_TIME_LONG
	printstring STRINGID_TRAINERBLOCKEDBALL
	waitmessage B_WAIT_TIME_LONG
	printstring STRINGID_DONTBEATHIEF
	waitmessage B_WAIT_TIME_LONG
	finishaction

BattleScript_PlayerUsesItem::
	moveendcase MOVEEND_MIRROR_MOVE
	end

BattleScript_OpponentUsesHealItem::
	printstring STRINGID_EMPTYSTRING3
	pause B_WAIT_TIME_MED
	playse SE_USE_ITEM
	printstring STRINGID_TRAINER1USEDITEM
	waitmessage B_WAIT_TIME_LONG
	useitemonopponent
	orword gHitMarker, HITMARKER_IGNORE_SUBSTITUTE
	healthbarupdate BS_ATTACKER
	datahpupdate BS_ATTACKER
	printstring STRINGID_PKMNSITEMRESTOREDHEALTH
	waitmessage B_WAIT_TIME_LONG
	updatestatusicon BS_ATTACKER
	moveendcase MOVEEND_MIRROR_MOVE
	finishaction

BattleScript_OpponentUsesStatusCureItem::
	printstring STRINGID_EMPTYSTRING3
	pause B_WAIT_TIME_MED
	playse SE_USE_ITEM
	printstring STRINGID_TRAINER1USEDITEM
	waitmessage B_WAIT_TIME_LONG
	useitemonopponent
	printfromtable gTrainerItemCuredStatusStringIds
	waitmessage B_WAIT_TIME_LONG
	updatestatusicon BS_ATTACKER
	moveendcase MOVEEND_MIRROR_MOVE
	finishaction

BattleScript_OpponentUsesXItem::
	printstring STRINGID_EMPTYSTRING3
	pause B_WAIT_TIME_MED
	playse SE_USE_ITEM
	printstring STRINGID_TRAINER1USEDITEM
	waitmessage B_WAIT_TIME_LONG
	useitemonopponent
	printfromtable gStatUpStringIds
	waitmessage B_WAIT_TIME_LONG
	moveendcase MOVEEND_MIRROR_MOVE
	finishaction

BattleScript_OpponentUsesGuardSpec::
	printstring STRINGID_EMPTYSTRING3
	pause B_WAIT_TIME_MED
	playse SE_USE_ITEM
	printstring STRINGID_TRAINER1USEDITEM
	waitmessage B_WAIT_TIME_LONG
	useitemonopponent
	printfromtable gMistUsedStringIds
	waitmessage B_WAIT_TIME_LONG
	moveendcase MOVEEND_MIRROR_MOVE
	finishaction

BattleScript_RunByUsingItem:
	playse SE_FLEE
	setbyte gBattleOutcome, B_OUTCOME_RAN
	finishturn

BattleScript_ActionUsePokeFlute:
	checkpokeflute BS_ATTACKER
	jumpifbyte CMP_EQUAL, cMULTISTRING_CHOOSER, 1, BattleScript_PokeFluteWakeUp
	printstring STRINGID_POKEFLUTECATCHY
	waitmessage B_WAIT_TIME_LONG
	goto BattleScript_PokeFluteEnd

BattleScript_PokeFluteWakeUp:
	printstring STRINGID_POKEFLUTE
	waitmessage B_WAIT_TIME_LONG
	fanfare MUS_RG_POKE_FLUTE
	waitfanfare BS_ATTACKER
	printstring STRINGID_MONHEARINGFLUTEAWOKE
	waitmessage B_WAIT_TIME_LONG
	updatestatusicon BS_PLAYER2
	waitstate
BattleScript_PokeFluteEnd:
	finishaction

BattleScript_ActionWatchesCarefully:
	printfromtable gSafariPokeblockResultStringIds
	waitmessage B_WAIT_TIME_LONG
	playanimation BS_OPPONENT1, B_ANIM_SAFARI_REACTION, NULL
	end2

BattleScript_ActionThrowRock:
	printstring STRINGID_THREWROCK
	waitmessage B_WAIT_TIME_LONG
	playanimation BS_ATTACKER, B_ANIM_ROCK_THROW, NULL
	end2

BattleScript_ActionThrowBait:
	printstring STRINGID_THREWBAIT
	waitmessage B_WAIT_TIME_LONG
	playanimation BS_ATTACKER, B_ANIM_BAIT_THROW, NULL
	end2

BattleScript_ActionWallyThrow:
	printstring STRINGID_RETURNMON
	waitmessage B_WAIT_TIME_LONG
	returnatktoball
	waitstate
	trainerslidein BS_TARGET
	waitstate
	printstring STRINGID_YOUTHROWABALLNOWRIGHT
	waitmessage B_WAIT_TIME_LONG
	end2
