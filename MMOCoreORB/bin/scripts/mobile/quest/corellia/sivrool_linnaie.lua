sivrool_linnaie = Creature:new {
	objectName = "",
	socialGroup = "",
	pvpFaction = "",
	faction = "",
	level = 17,
	chanceHit = 0.320000,
	damageMin = 160,
	damageMax = 170,
	baseXp = 1257,
	baseHAM = 3500,
	baseHAMmax = 4300,
	armor = 0,
	resists = {0,0,0,0,0,0,0,-1,-1},
	meatType = "",
	meatAmount = 0,
	hideType = "",
	hideAmount = 0,
	boneType = "",
	boneAmount = 0,
	milk = 0,
	tamingChance = 0.000000,
	ferocity = 0,
	pvpBitmask = ATTACKABLE,
	creatureBitmask = PACK,
	optionsBitmask = 136,
	diet = HERBIVORE,

	templates = {"object/mobile/dressed_eisley_officer_bothan_male_01.iff"},
	lootGroups = {},
	weapons = {},
	conversationTemplate = "ging_darjeek_mission_target_convotemplate",
	attacks = brawlermaster
}

CreatureTemplates:addCreatureTemplate(sivrool_linnaie, "sivrool_linnaie")
