agrilat_swamp_conversationtemplate = ConvoTemplate:new {
	initialScreen = "agrilat_initial",
	templateType = "Lua",
	luaClassHandler = "agrilat_swamp_racetrack_convo_handler",
	screens = {}
}

agrilat_initial = ConvoScreen:new {
	id = "agrilat_initial",
	leftDialog = "@conversation/racing_agrilat:s_1e9a4eea",
	stopConversation = "false",
	options = {
		{"@conversation/racing_agrilat:s_dffdee4b","cs_jsPlumb_1_90"},
		{"@conversation/racing_agrilat:s_1736e216","cs_jsPlumb_1_29"},
		{"@conversation/racing_agrilat:s_838e4ffb","cs_jsPlumb_1_181"},
		{"@conversation/racing_agrilat:s_2492930f","cs_jsPlumb_1_207"}
	}
}
agrilat_swamp_conversationtemplate:addScreen(agrilat_initial);
cs_jsPlumb_1_29 = ConvoScreen:new {
	id = "cs_jsPlumb_1_29",
	leftDialog = "@conversation/racing_agrilat:s_f6b2a28e",
	stopConversation = "false",
	options = {
		{"@conversation/racing_agrilat:s_e460e3d3","cs_jsPlumb_1_233"},
		{"@conversation/racing_agrilat:s_fbd55d9d",""}
	}
}
agrilat_swamp_conversationtemplate:addScreen(cs_jsPlumb_1_29);
cs_jsPlumb_1_90 = ConvoScreen:new {
	id = "cs_jsPlumb_1_90",
	leftDialog = "@conversation/racing_agrilat:s_d24f3595",
	stopConversation = "false",
	options = {
		{"@conversation/racing_agrilat:s_2528fad7","cs_jsPlumb_1_116"},
		{"@conversation/racing_agrilat:s_262e8687","cs_jsPlumb_1_142"}
	}
}
agrilat_swamp_conversationtemplate:addScreen(cs_jsPlumb_1_90);
cs_jsPlumb_1_116 = ConvoScreen:new {
	id = "cs_jsPlumb_1_116",
	leftDialog = "@conversation/racing_agrilat:s_b2acc217",
	stopConversation = "true",
	options = {
	}
}
agrilat_swamp_conversationtemplate:addScreen(cs_jsPlumb_1_116);
cs_jsPlumb_1_142 = ConvoScreen:new {
	id = "cs_jsPlumb_1_142",
	leftDialog = "@conversation/racing_agrilat:s_c0918d6",
	stopConversation = "true",
	options = {
	}
}
agrilat_swamp_conversationtemplate:addScreen(cs_jsPlumb_1_142);
cs_jsPlumb_1_181 = ConvoScreen:new {
	id = "cs_jsPlumb_1_181",
	leftDialog = "@conversation/racing_agrilat:s_abd9745d",
	stopConversation = "true",
	options = {
	}
}
agrilat_swamp_conversationtemplate:addScreen(cs_jsPlumb_1_181);
cs_jsPlumb_1_207 = ConvoScreen:new {
	id = "cs_jsPlumb_1_207",
	leftDialog = "@conversation/racing_agrilat:s_abd9745d",
	stopConversation = "true",
	options = {
	}
}
agrilat_swamp_conversationtemplate:addScreen(cs_jsPlumb_1_207);
cs_jsPlumb_1_233 = ConvoScreen:new {
	id = "cs_jsPlumb_1_233",
	leftDialog = "@conversation/racing_agrilat:s_4b4fb2b7",
	stopConversation = "true",
	options = {
	}
}
agrilat_swamp_conversationtemplate:addScreen(cs_jsPlumb_1_233);
addConversationTemplate("agrilat_swamp_conversationtemplate", agrilat_swamp_conversationtemplate);