--Generated by SWGEmu Conversation Editor
rk_record_keeper_jabbaConversationTemplate = ConvoTemplate:new {
	initialScreen = "init",
	templateType = "Lua",
	luaClassHandler = "jabba_park_record_keeper_convo_handler",
	screens = {}
}
record_keeper_init = ConvoScreen:new {
	id = "init",
	leftDialog = "",
	stopConversation = "false",
	options = {
	}
}
rk_record_keeper_jabbaConversationTemplate:addScreen(record_keeper_init);

cs_jsPlumb_1_161 = ConvoScreen:new {
	id = "cs_jsPlumb_1_161",
	leftDialog = "@conversation/theme_park_record_keeper_jabba:s_f3139d0",
	stopConversation = "true",
	options = {
	}
}
rk_record_keeper_jabbaConversationTemplate:addScreen(cs_jsPlumb_1_161);
cs_jsPlumb_1_165 = ConvoScreen:new {
	id = "continue",
	leftDialog = "@conversation/theme_park_record_keeper_jabba:s_faf7c09a",
	stopConversation = "true",
	options = {
	}
}
rk_record_keeper_jabbaConversationTemplate:addScreen(cs_jsPlumb_1_165);
cs_jsPlumb_1_171 = ConvoScreen:new {
	id = "cs_jsPlumb_1_171",
	leftDialog = "@conversation/theme_park_record_keeper_jabba:s_c7528c3b",
	stopConversation = "false",
	options = {
		{"@conversation/theme_park_record_keeper_jabba:s_866cf013","cs_jsPlumb_1_205"}
	}
}
rk_record_keeper_jabbaConversationTemplate:addScreen(cs_jsPlumb_1_171);
cs_jsPlumb_1_173 = ConvoScreen:new {
	id = "cs_jsPlumb_1_173",
	leftDialog = "@conversation/theme_park_record_keeper_jabba:s_c1ff278a",
	stopConversation = "false",
	options = {
		{"@conversation/theme_park_record_keeper_jabba:s_2e18346d","cs_jsPlumb_1_161"},
		{"@conversation/theme_park_record_keeper_jabba:s_9ef848f9","cs_jsPlumb_1_219"}
	}
}
rk_record_keeper_jabbaConversationTemplate:addScreen(cs_jsPlumb_1_173);
cs_jsPlumb_1_177 = ConvoScreen:new {
	id = "cs_jsPlumb_1_177",
	leftDialog = "@conversation/theme_park_record_keeper_jabba:s_badb4859",
	stopConversation = "false",
	options = {
		{"@conversation/theme_park_record_keeper_jabba:s_f7565bbd","continue"},
		{"@conversation/theme_park_record_keeper_jabba:s_ebce22a9","cs_jsPlumb_1_187"}
	}
}
rk_record_keeper_jabbaConversationTemplate:addScreen(cs_jsPlumb_1_177);
cs_jsPlumb_1_179 = ConvoScreen:new {
	id = "cs_jsPlumb_1_179",
	leftDialog = "@conversation/theme_park_record_keeper_jabba:s_b193a14c",
	stopConversation = "false",
	options = {
		{"@conversation/theme_park_record_keeper_jabba:s_4fd35e09","reset"},
		{"@conversation/theme_park_record_keeper_jabba:s_93e226b7","cs_jsPlumb_1_502"}
	}
}
rk_record_keeper_jabbaConversationTemplate:addScreen(cs_jsPlumb_1_179);
cs_jsPlumb_1_181 = ConvoScreen:new {
	id = "completed",
	leftDialog = "@conversation/theme_park_record_keeper_jabba:s_adccd297",
	stopConversation = "true",
	options = {
	}
}
rk_record_keeper_jabbaConversationTemplate:addScreen(cs_jsPlumb_1_181);
cs_jsPlumb_1_187 = ConvoScreen:new {
	id = "cs_jsPlumb_1_187",
	leftDialog = "@conversation/theme_park_record_keeper_jabba:s_960270ff",
	stopConversation = "true",
	options = {
	}
}
rk_record_keeper_jabbaConversationTemplate:addScreen(cs_jsPlumb_1_187);
cs_jsPlumb_1_193 = ConvoScreen:new {
	id = "cs_jsPlumb_1_193",
	leftDialog = "@conversation/theme_park_record_keeper_jabba:s_741d0966",
	stopConversation = "false",
	options = {
		{"@conversation/theme_park_record_keeper_jabba:s_e3c4a3d5","cs_jsPlumb_1_177"},
		{"@conversation/theme_park_record_keeper_jabba:s_ff20b1f1","cs_jsPlumb_1_517"},
		{"@conversation/theme_park_record_keeper_jabba:s_710595f4","cs_jsPlumb_1_629"}
	}
}
rk_record_keeper_jabbaConversationTemplate:addScreen(cs_jsPlumb_1_193);
cs_jsPlumb_1_195 = ConvoScreen:new {
	id = "cs_jsPlumb_1_195",
	leftDialog = "@conversation/theme_park_record_keeper_jabba:s_72907ccb",
	stopConversation = "false",
	options = {
		{"@conversation/theme_park_record_keeper_jabba:s_ff20b1f1","cs_jsPlumb_1_179"},
		{"@conversation/theme_park_record_keeper_jabba:s_6e88ecc1","cs_jsPlumb_1_201"},
		{"@conversation/theme_park_record_keeper_jabba:s_710595f4","cs_jsPlumb_1_629"}
	}
}
rk_record_keeper_jabbaConversationTemplate:addScreen(cs_jsPlumb_1_195);
cs_jsPlumb_1_201 = ConvoScreen:new {
	id = "cs_jsPlumb_1_201",
	leftDialog = "@conversation/theme_park_record_keeper_jabba:s_6d63d30d",
	stopConversation = "true",
	options = {
	}
}
rk_record_keeper_jabbaConversationTemplate:addScreen(cs_jsPlumb_1_201);
cs_jsPlumb_1_203 = ConvoScreen:new {
	id = "reset",
	leftDialog = "@conversation/theme_park_record_keeper_jabba:s_630c8b2",
	stopConversation = "true",
	options = {
	}
}
rk_record_keeper_jabbaConversationTemplate:addScreen(cs_jsPlumb_1_203);
cs_jsPlumb_1_205 = ConvoScreen:new {
	id = "cs_jsPlumb_1_205",
	leftDialog = "@conversation/theme_park_record_keeper_jabba:s_5cc1a230",
	stopConversation = "false",
	options = {
		{"@conversation/theme_park_record_keeper_jabba:s_3cf75f2d","cs_jsPlumb_1_173"}
	}
}
rk_record_keeper_jabbaConversationTemplate:addScreen(cs_jsPlumb_1_205);
cs_jsPlumb_1_211 = ConvoScreen:new {
	id = "start",
	leftDialog = "@conversation/theme_park_record_keeper_jabba:s_3be84186",
	stopConversation = "false",
	options = {
		{"@conversation/theme_park_record_keeper_jabba:s_f51b07ff","cs_jsPlumb_1_215"}
	}
}
rk_record_keeper_jabbaConversationTemplate:addScreen(cs_jsPlumb_1_211);
cs_jsPlumb_1_213 = ConvoScreen:new {
	id = "not_started",
	leftDialog = "@conversation/theme_park_record_keeper_jabba:s_3ae9c534",
	stopConversation = "true",
	options = {
	}
}
rk_record_keeper_jabbaConversationTemplate:addScreen(cs_jsPlumb_1_213);
cs_jsPlumb_1_215 = ConvoScreen:new {
	id = "cs_jsPlumb_1_215",
	leftDialog = "@conversation/theme_park_record_keeper_jabba:s_34f17e66",
	stopConversation = "false",
	options = {
		{"@conversation/theme_park_record_keeper_jabba:s_edb238bc","cs_jsPlumb_1_171"}
	}
}
rk_record_keeper_jabbaConversationTemplate:addScreen(cs_jsPlumb_1_215);
cs_jsPlumb_1_219 = ConvoScreen:new {
	id = "cs_jsPlumb_1_219",
	leftDialog = "@conversation/theme_park_record_keeper_jabba:s_5e8caf64",
	stopConversation = "false",
	options = {
		{"@conversation/theme_park_record_keeper_jabba:s_14693cf1","cs_jsPlumb_1_193"},
		{"@conversation/theme_park_record_keeper_jabba:s_a76f22bf","cs_jsPlumb_1_195"},
		{"@conversation/theme_park_record_keeper_jabba:s_bfd7fb6d","cs_jsPlumb_1_313"}
	}
}
rk_record_keeper_jabbaConversationTemplate:addScreen(cs_jsPlumb_1_219);
cs_jsPlumb_1_313 = ConvoScreen:new {
	id = "cs_jsPlumb_1_313",
	leftDialog = "@conversation/theme_park_record_keeper_jabba:s_555b76aa",
	stopConversation = "true",
	options = {
	}
}
rk_record_keeper_jabbaConversationTemplate:addScreen(cs_jsPlumb_1_313);
cs_jsPlumb_1_502 = ConvoScreen:new {
	id = "cs_jsPlumb_1_502",
	leftDialog = "@conversation/theme_park_record_keeper_jabba:s_6d63d30d",
	stopConversation = "true",
	options = {
	}
}
rk_record_keeper_jabbaConversationTemplate:addScreen(cs_jsPlumb_1_502);
cs_jsPlumb_1_517 = ConvoScreen:new {
	id = "cs_jsPlumb_1_517",
	leftDialog = "@conversation/theme_park_record_keeper_jabba:s_e11b3d7c",
	stopConversation = "false",
	options = {
		{"@conversation/theme_park_record_keeper_jabba:s_4fd35e09","reset"},
		{"@conversation/theme_park_record_keeper_jabba:s_93e226b7","cs_jsPlumb_1_502"}
	}
}
rk_record_keeper_jabbaConversationTemplate:addScreen(cs_jsPlumb_1_517);
cs_jsPlumb_1_629 = ConvoScreen:new {
	id = "cs_jsPlumb_1_629",
	leftDialog = "@conversation/theme_park_record_keeper_jabba:s_555b76aa",
	stopConversation = "true",
	options = {
	}
}
rk_record_keeper_jabbaConversationTemplate:addScreen(cs_jsPlumb_1_629);
addConversationTemplate("rk_record_keeper_jabbaConversationTemplate", rk_record_keeper_jabbaConversationTemplate);
