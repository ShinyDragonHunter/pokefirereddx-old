MossdeepCity_House4_MapScripts:: @ 8222DD7
	.byte 0

MossdeepCity_House4_EventScript_Woman:: @ 8222DD8
	lock
	faceplayer
	msgbox MossdeepCity_House4_Text_BrotherLikesToFindBases, MSGBOX_DEFAULT
	release
	end

MossdeepCity_House4_EventScript_NinjaBoy:: @ 8222DF7
	lock
	faceplayer
	msgbox MossdeepCity_House4_Text_BrotherLikesToFindBases, MSGBOX_DEFAULT
	release
	end

MossdeepCity_House4_EventScript_Skitty:: @ 8222E1E
	lock
	faceplayer
	waitse
	playmoncry SPECIES_SKITTY, 0
	msgbox MossdeepCity_House4_Text_Skitty, MSGBOX_DEFAULT
	waitmoncry
	release
	end

MossdeepCity_House4_Text_BrotherLikesToFindBases: @ 8222E31
	.string "My little brother says he likes to go\n"
	.string "find people's SECRET BASES.$"

MossdeepCity_House4_Text_BrotherLikesToVisitBasesAndBattle: @ 8222E73
	.string "My little brother says he likes to\n"
	.string "visit people's SECRET BASES and have\l"
	.string "POKéMON battles.$"

MossdeepCity_House4_Text_Skitty: @ 8222F31
	.string "SKITTY: Miyaan?$"

