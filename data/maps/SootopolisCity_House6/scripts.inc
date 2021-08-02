SootopolisCity_House6_MapScripts::
	.byte 0

SootopolisCity_House6_EventScript_Woman::
	lock
	faceplayer
	msgbox SootopolisCity_House6_Text_TakeGoodCareOfIt, MSGBOX_DEFAULT
	release
	end

SootopolisCity_House6_Text_TakeGoodCareOfIt:
	.string "Take good care of it!$"
