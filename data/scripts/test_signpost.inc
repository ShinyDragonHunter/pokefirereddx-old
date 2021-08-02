Text_ThisIsATestSignpostMsg::
	.string "This is a test message.\n"
	.string "This is a signpost.$"

EventScript_TestSignpostMsg::
	msgbox Text_ThisIsATestSignpostMsg, MSGBOX_SIGN
	end

Text_ThisIsATestObjectEventMsg::
	.string "This is a test message.\n"
	.string "This is an object event.$"

EventScript_TestObjectEventMsg::
	textcolor DARK_GRAY
	msgbox Text_ThisIsATestObjectEventMsg, MSGBOX_NPC
	end
