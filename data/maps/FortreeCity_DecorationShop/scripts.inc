FortreeCity_DecorationShop_MapScripts:: @ 821800D
	.byte 0

FortreeCity_DecorationShop_EventScript_PokefanM:: @ 821800E
	msgbox FortreeCity_DecorationShop_Text_MerchandiseSentToPC, MSGBOX_NPC
	end

FortreeCity_DecorationShop_EventScript_Girl:: @ 8218017
	msgbox FortreeCity_DecorationShop_Text_BuyingDeskForDolls, MSGBOX_NPC
	end

FortreeCity_DecorationShop_Text_MerchandiseSentToPC: @ 8218078
	.string "Merchandise you buy here is sent to\n"
	.string "your own PC.\p"
	.string "That's fantastic! I wish they could\n"
	.string "also deliver me home like that.$"

FortreeCity_DecorationShop_Text_BuyingDeskForDolls: @ 82180ED
	.string "I'm buying a pretty desk and I'm\n"
	.string "putting my cute DOLLS on it.\p"
	.string "If I don't, when I decorate my\n"
	.string "SECRET BASE, my DOLLS will get\l"
	.string "dirty or poked with splinters.$"

