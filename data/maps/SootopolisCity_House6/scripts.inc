SootopolisCity_House6_MapScripts:: @ 8226F5B
	.byte 0

SootopolisCity_House6_EventScript_Woman:: @ 8226F5C
	lock
	faceplayer
	msgbox SootopolisCity_House6_Text_TakeGoodCareOfIt, MSGBOX_DEFAULT
	release
	end

SootopolisCity_House6_Text_TakeGoodCareOfIt: @ 8227034
	.string "Take good care of it!$"
