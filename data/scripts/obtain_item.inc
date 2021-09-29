Std_ObtainItem::
	textcolor DARK_GRAY
	additem VAR_0x8000, VAR_0x8001
	copyvar VAR_0x8007, VAR_RESULT
	call EventScript_ObtainItemMessage
	copyvar VAR_TEXT_COLOR, VAR_PREV_TEXT_COLOR
	return

EventScript_ObtainItemMessage::
	bufferitemnameplural 1, VAR_0x8000, VAR_0x8001
	checkitemtype VAR_0x8000
	call EventScript_BufferPocketNameAndTryFanfare
	compare VAR_0x8007, 1
	call_if_eq EventScript_ObtainedItem
	compare VAR_0x8007, 0
	call_if_eq EventScript_NoRoomForItem
	return

EventScript_BufferPocketNameAndTryFanfare::
	switch VAR_RESULT
	case POCKET_ITEMS, EventScript_BufferItemsPocket
	case POCKET_KEY_ITEMS, EventScript_BufferKeyItemsPocket
	case POCKET_POKE_BALLS, EventScript_BufferPokeballsPocket
	case POCKET_TM_HM, EventScript_BufferTMHMsPocket
	case POCKET_BERRIES, EventScript_BufferBerriesPocket
	end

EventScript_BufferItemsPocket::
	bufferstdstring 2, STDSTRING_ITEMS
	compare VAR_0x8007, 1
	call_if_eq EventScript_PlayFanfareObtainedItem
	return

EventScript_BufferKeyItemsPocket::
	bufferstdstring 2, STDSTRING_KEYITEMS
	compare VAR_0x8007, 1
	call_if_eq EventScript_PlayFanfareObtainedKeyItem
	return

EventScript_BufferPokeballsPocket::
	bufferstdstring 2, STDSTRING_POKEBALLS
	compare VAR_0x8007, 1
	call_if_eq EventScript_PlayFanfareObtainedItem
	return

EventScript_BufferTMHMsPocket::
	bufferstdstring 2, STDSTRING_TMHMS
	compare VAR_0x8007, 1
	call_if_eq EventScript_PlayFanfareObtainedTMHM
	return

EventScript_BufferBerriesPocket::
	bufferstdstring 2, STDSTRING_BERRIES
	compare VAR_0x8007, 1
	call_if_eq EventScript_PlayFanfareObtainedBerry
	return

EventScript_ObtainedItem::
    compare VAR_0x8001, 1
	goto_if_eq EventScript_ObtainedItemMessage
	buffernumberstring 0, VAR_0x8001
	message gText_ObtainedTheItems
	goto_if_eq EventScript_ContinueObtainedItem
EventScript_ObtainedItemMessage:
	message gText_ObtainedTheItem
EventScript_ContinueObtainedItem:
	waitfanfare
	msgbox gText_PutItemInPocket, MSGBOX_DEFAULT
	setvar VAR_RESULT, 1
	return

EventScript_NoRoomForItem::
	setvar VAR_RESULT, 0
	return

EventScript_PlayFanfareObtainedKeyItem::
	playfanfare MUS_RG_OBTAIN_KEY_ITEM
	return

EventScript_PlayFanfareObtainedItem::
	playfanfare MUS_OBTAIN_ITEM
	return

EventScript_PlayFanfareObtainedTMHM::
	playfanfare MUS_OBTAIN_TMHM
	return

EventScript_PlayFanfareObtainedBerry::
	playfanfare MUS_OBTAIN_BERRY
	return

Std_FindItem::
	lock
	faceplayer
	waitse
	copyvar VAR_0x8004, VAR_0x8000
	copyvar VAR_0x8005, VAR_0x8001
	checkitemspace VAR_0x8000, VAR_0x8001
	copyvar VAR_0x8007, VAR_RESULT
	bufferitemnameplural 1, VAR_0x8000, VAR_0x8001
	checkitemtype VAR_0x8000
	call EventScript_BufferPocketNameAndTryFanfare
	compare VAR_0x8007, 1
	call_if_eq EventScript_PickUpItem
	compare VAR_0x8007, 0
	call_if_eq EventScript_NoRoomToPickUpItem
	release
	return

EventScript_PickUpItem::
	removeobject VAR_LAST_TALKED
	additem VAR_0x8004, VAR_0x8005
	specialvar VAR_RESULT, BufferTMHMMoveName
	copyvar VAR_0x8008, VAR_RESULT
	compare VAR_0x8008, 1
	call_if_eq EventScript_FoundTMHM
	compare VAR_0x8008, 0
	call_if_eq EventScript_FoundItem
	waitfanfare
	waitmessage
	bufferitemnameplural 1, VAR_0x8004, VAR_0x8005
	pyramid_inchallenge
	compare VAR_RESULT, 1
	goto_if_eq EventScript_PutBattlePyramidItemInBag
	msgbox gText_PutItemInPocket, MSGBOX_DEFAULT
	return

EventScript_PutBattlePyramidItemInBag::
	msgbox gText_PlayerPutItemInBag, MSGBOX_DEFAULT
	return

EventScript_FoundTMHM::
	bufferitemnameplural 0, VAR_0x8004, VAR_0x8005
	message gText_PlayerFoundOneTMHM
	return

EventScript_FoundItem::
	compare VAR_0x8001, 1
	goto_if_eq EventScript_FoundItemMessage
	buffernumberstring 0, VAR_0x8001
	message gText_PlayerFoundItems
	return
EventScript_FoundItemMessage::
	message gText_PlayerFoundOneItem
	return

EventScript_NoRoomToPickUpItem::
	msgbox gText_ObtainedTheItem, MSGBOX_DEFAULT
	msgbox gText_TooBadBagIsFull, MSGBOX_DEFAULT
	setvar VAR_RESULT, 0
	return

EventScript_HiddenItemScript::
	lockall
	waitse
	additem VAR_0x8005
	copyvar VAR_0x8007, VAR_RESULT
	bufferitemnameplural 1, VAR_0x8005, 1
	checkitemtype VAR_0x8005
	call EventScript_BufferPocketNameAndTryFanfare
	compare VAR_0x8007, 1
	goto_if_eq EventScript_PickUpHiddenItem
	compare VAR_0x8007, 0
	goto_if_eq EventScript_NoRoomForHiddenItem

EventScript_PickUpHiddenItem::
	copyvar VAR_0x8008, VAR_0x8004
	copyvar VAR_0x8004, VAR_0x8005
	specialvar VAR_RESULT, BufferTMHMMoveName
	compare VAR_RESULT, 1
	goto_if_eq EventScript_FoundHiddenTMHM
	compare VAR_RESULT, 0
	goto_if_eq EventScript_FoundHiddenItem

EventScript_FoundHiddenTMHM::
	bufferitemnameplural 0, VAR_0x8004, 1
	message gText_PlayerFoundOneTMHM
	goto EventScript_PutHiddenItemInPocket

EventScript_FoundHiddenItem::
	message gText_PlayerFoundOneItem
	goto EventScript_PutHiddenItemInPocket

EventScript_PutHiddenItemInPocket::
	waitmessage
	waitfanfare
	bufferitemnameplural 1, VAR_0x8004, 1
	copyvar VAR_0x8004, VAR_0x8008
	msgbox gText_PutItemInPocket, MSGBOX_DEFAULT
	special TryPutTreasureInvestigatorsOnAir
	special SetHiddenItemFlag
	releaseall
	end

EventScript_NoRoomForHiddenItem::
	msgbox gText_PlayerFoundOneItem, MSGBOX_DEFAULT
	msgbox gText_TooBadBagIsFull, MSGBOX_DEFAULT
	setvar VAR_RESULT, 0
	releaseall
	end
