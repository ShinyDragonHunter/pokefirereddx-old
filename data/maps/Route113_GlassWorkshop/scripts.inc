Route113_GlassWorkshop_MapScripts:: @ 826ED03
	map_script MAP_SCRIPT_ON_TRANSITION, Route113_GlassWorkshop_OnTransition
	.byte 0

Route113_GlassWorkshop_OnTransition: @ 826ED09
	setflag FLAG_LANDMARK_GLASS_WORKSHOP
	compare VAR_GLASS_WORKSHOP_STATE, 1
	call_if_eq Route113_GlassWorkshop_EventScript_ReenterWorkshopAfterSootSack
	end

Route113_GlassWorkshop_EventScript_ReenterWorkshopAfterSootSack:: @ 826ED18
	setvar VAR_GLASS_WORKSHOP_STATE, 2
	return

Route113_GlassWorkshop_EventScript_NinjaBoy:: @ 826F194
	msgbox Route113_GlassWorkshop_Text_FunToBlowGlassFlute, MSGBOX_NPC
	end

Route113_GlassWorkshop_Text_FunToBlowGlassFlute: @ 826F7EC
	.string "It's fun to blow a glass flute while\n"
	.string "my boss is talking.\p"
	.string "Huff-huff! Puff-puff!$"

