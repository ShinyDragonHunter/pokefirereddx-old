MossdeepCity_House1_MapScripts::
	.byte 0

MossdeepCity_House1_EventScript_BlackBelt::
	lock
	faceplayer
	msgbox MossdeepCity_House1_Text_ItLikesXPokeblocks, MSGBOX_DEFAULT
	release
	end

MossdeepCity_House1_EventScript_Woman::
	msgbox MossdeepCity_House1_Text_HusbandCanTellPokeblockMonLikes, MSGBOX_NPC
	end

MossdeepCity_House1_Text_ItLikesXPokeblocks:
	.string "It likes {POKEBLOCK}S,\n"
	.string "doesn't it?\p"
	.string "No, I'm positive of it! It definitely\n"
	.string "likes {POKEBLOCK}S!$"

MossdeepCity_House1_Text_HusbandCanTellPokeblockMonLikes:
	.string "My husband can tell what kind of\n"
	.string "{POKEBLOCK}S a POKéMON likes at a glance.$"

