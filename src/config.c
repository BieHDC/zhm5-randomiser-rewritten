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
	cini.worldInventoryRandomizer = getStrategyFromString(worldInventoryRandomizer, 
		&cini.worldInventoryIsExtended);
	printStrategyFromEnum(cini.worldInventoryRandomizer);

	LOAD_INI_STRING_ENTRY(npcInventoryRandomizer, "ZHM5Randomizer", "DEFAULT");
	cini.npcInventoryRandomizer = getStrategyFromString(npcInventoryRandomizer, 
		&cini.npcInventoryIsExtended);
	printStrategyFromEnum(cini.npcInventoryRandomizer);

	LOAD_INI_STRING_ENTRY(heroInventoryRandomizer, "ZHM5Randomizer", "DEFAULT");
	cini.heroInventoryRandomizer = getStrategyFromString(heroInventoryRandomizer, 
		&cini.heroInventoryIsExtended);
	printStrategyFromEnum(cini.heroInventoryRandomizer);

	LOAD_INI_STRING_ENTRY(stashInventoryRandomizer, "ZHM5Randomizer", "FUN");
	cini.stashInventoryRandomizer = getStrategyFromString(stashInventoryRandomizer, 
		&cini.stashInventoryIsExtended);
	printStrategyFromEnum(cini.stashInventoryRandomizer);

	INFO("Extended flags W:%i N:%i H:%i S:%i", cini.worldInventoryIsExtended, cini.npcInventoryIsExtended, 
		cini.heroInventoryIsExtended, cini.stashInventoryIsExtended);

	DWORD RNGSeed;
	LOAD_INI_ENTRY(RNGSeed,	"ZHM5Randomizer", 0);
	cini.RNGSeed = RNGSeed;

	free(fullpath);
	free(basepath);
	return;

	load_defaults2:
	free(basepath);
	load_defaults1:
	cini.worldInventoryRandomizer = ES_FUN;
	cini.npcInventoryRandomizer = ES_DEFAULT;
	cini.heroInventoryRandomizer = ES_DEFAULT;
	cini.stashInventoryRandomizer = ES_FUN;
	cini.RNGSeed = 0;
}


#endif // CONFIG_C