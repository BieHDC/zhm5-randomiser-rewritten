#ifndef CONFIG_C
#define CONFIG_C

#define iniStringBufSize 32

#define LOAD_INI_ENTRY(name, cat, default_) 						\
	name = GetPrivateProfileIntA(cat, #name, default_, fullpath); 	\
	INFO("%s=%lu", #name, name);

#define LOAD_INI_STRING_ENTRY(name, cat, default_) 												\
	char strbuf_##name[iniStringBufSize]; 														\
	GetPrivateProfileStringA(cat, #name, default_, strbuf_##name, iniStringBufSize, fullpath); 	\
	char* name = cstrdup(strbuf_##name); 														\
	INFO("%s=%s", #name, name);

void loadConfig() {
	char* basepath = getBasePath();
	if (!basepath) {
		ERR("Basepath Error, loading defaults");
		goto load_defaults1;
	} else {
		SPAM("Basepath is >%s<", basepath);
	}

	char* fullpath = 0;
	cstrcat(&fullpath, basepath, (const char*)&"ZHM5Randomizer.ini");
	if (!fullpath) {
		ERR("Fullpath Error, loading defaults");
		goto load_defaults2;
	} else {
		SPAM("Fullpath is >%s<", fullpath);
	}

	INFO("Loading Config %s", fullpath);

	//Randomisation Strategy
	LOAD_INI_STRING_ENTRY(worldInventoryRandomizer, "ZHM5Randomizer", "FUN+");
	config.worldInventoryRandomizer = getStrategyFromString(worldInventoryRandomizer,
		&config.worldInventoryIsExtended);
	printStrategyFromEnum(config.worldInventoryRandomizer);

	LOAD_INI_STRING_ENTRY(npcInventoryRandomizer, "ZHM5Randomizer", "DEFAULT");
	config.npcInventoryRandomizer = getStrategyFromString(npcInventoryRandomizer,
		&config.npcInventoryIsExtended);
	printStrategyFromEnum(config.npcInventoryRandomizer);

	LOAD_INI_STRING_ENTRY(heroInventoryRandomizer, "ZHM5Randomizer", "DEFAULT");
	config.heroInventoryRandomizer = getStrategyFromString(heroInventoryRandomizer,
		&config.heroInventoryIsExtended);
	printStrategyFromEnum(config.heroInventoryRandomizer);

	LOAD_INI_STRING_ENTRY(stashInventoryRandomizer, "ZHM5Randomizer", "FUN");
	config.stashInventoryRandomizer = getStrategyFromString(stashInventoryRandomizer,
		&config.stashInventoryIsExtended);
	printStrategyFromEnum(config.stashInventoryRandomizer);

	INFO("Extended flags W:%i N:%i H:%i S:%i", config.worldInventoryIsExtended, config.npcInventoryIsExtended,
		config.heroInventoryIsExtended, config.stashInventoryIsExtended);

	DWORD RNGSeed;
	LOAD_INI_ENTRY(RNGSeed,	"ZHM5Randomizer", 0);
	config.RNGSeed = RNGSeed;

	free(fullpath);
	free(basepath);
	return;

	load_defaults2:
	free(basepath);
	load_defaults1:
	config.worldInventoryRandomizer = ES_FUN;
	config.npcInventoryRandomizer = ES_DEFAULT;
	config.heroInventoryRandomizer = ES_DEFAULT;
	config.stashInventoryRandomizer = ES_FUN;
	config.RNGSeed = 0;
}


#endif // CONFIG_C