#ifndef GAMEINTERNALTYPES_C
#define GAMEINTERNALTYPES_C
//The RPC run-time library allocates memory for the string returned in the StringUuid parameter.
//The application is responsible for calling RpcStringFree to deallocate that memory.
// needs "-lrpcrt4 "
#if 0
unsigned char* guidtostring(GUID id) {
	unsigned char* guidstring = 0;
	UuidToStringA((GUID*)&id, &guidstring);
	return guidstring;
}

GUID stringtoguid(unsigned char* guidstring) {
	GUID id = {0};
	UuidFromStringA(guidstring, &id);
	return id;
}
#endif

//This is a Glacier 2 Engine type, don't touch!
typedef struct {
	uint32_t len;
	uint32_t unk;
	char* chars;
} ZString;
//sizeof(ZString) == 0x10

//This is a Glacier 2 Engine type, don't touch!
//Comment from OP:
//There is something fucked up going on with this structure,
//it seems like TArray<ZString> employs some sort of short array
//optimisation, where a TArray<ZString> with a single entry will turn into 
//an inlined ZString. That's how the TArray in SSceneInitParams behaves at least.
//This is fine if you use the source code but reading such structures from
//memory is a bit cumbersome so please excuse the sketchy code and UB in the code below.
//Data members should only be accessed after checking if the struct is 
//using the short array optimisation. This can be done by calling isShort();
typedef struct {
	union {
		struct {
			ZString* start_;
			ZString* end_;
			ZString* last_;
		};
		ZString t;
	};
} TArray;

int32_t TaisShort(TArray* ta) {
	if (ta->start_ == NULL)
		return false; //empty array
#if 0
	ZString str;
	memcpy_s(&str, sizeof(ZString), ta, sizeof(ZString));

	if (str.len > 0xFFFF) //assuming strings aren't longer than 0xFFFF;
		return false;
	return true;
#else // Yes, this works too, which is nice, keeping the original just in case
	uint32_t len = *(uint32_t*)ta; //len is first param
	if (len > 0xFFFF) //assuming strings aren't longer than 0xFFFF;
		return false;
	return true;
#endif
}

ZString* TaGetIndex(TArray* ta, size_t i) {
	if(TaisShort(ta))
		return &ta->t;
	return &ta->start_[i];
}


size_t Tasize(TArray* ta) {
	if (TaisShort(ta))
		return 1;
	return ta->end_ - ta->start_;
}
//sizeof(TArray) == 0x18;

// We pass out as ptr because we add to it later again
// Dont tamper with it unless you know what you are doing!
void simplezstringhash(size_t* hash, ZString* zstr) {
	for (uint32_t i = 0; i < zstr->len; i++) {
		*hash += (size_t)zstr->chars[i];
	}
}

typedef struct {
	ZString m_SceneResource;
	TArray m_aAdditionalBrickResources;
} SSceneInitParameters;

void SSceneInitParametersPrint(SSceneInitParameters* sip) {
	INFO("SSceneInitParameter: %p", sip);
	INFO("\tm_SceneResource: >%.*s<", sip->m_SceneResource.len, sip->m_SceneResource.chars);
	for (size_t i = 0; i < Tasize(&(sip->m_aAdditionalBrickResources)); i++) {
		ZString* tmp = TaGetIndex(&sip->m_aAdditionalBrickResources, i);
		INFO("\t\tm_aAdditionalBrickResources: >%.*s<", tmp->len, tmp->chars);
	}
}

// Map Detection
// > assembly:/_PRO/Scenes/missions/**mapname**/<<<nonconsistentafterhere>>> <
// non spec for loading and main menu > assembly:/_PRO/Scenes/**name**/**codename** <
typedef struct {
	char mapname[12];
	char codename[13]; //maybe we have use for it later
	location loc; //prefilled with "defaults" unless we detect a difficulty later
} mn;
#define SZMAPNAMES 29
mn mapnames[SZMAPNAMES] = {
	// Misc
	//{{"Frontend"},		{"Boot"}},		// Skip in the randomiser
	//{{"Frontend"},		{"MainMenu"}},	// Skip in the randomiser
	// Old Maps
	{{"TheFacility"},	{"polarbear"},	NONE},								// ICA Facility Map
	{{"paris"},			{"peacock"},	LOCATION_PEACOCK_PROFESSIONAL},		// Paris Main Mission
	{{"paris"},			{"parisnoel"},	LOCATION_PEACOCK_PROFESSIONAL},		// Paris Holiday Hoarders
	{{"CoastalTown"},	{"octopus"},	LOCATION_OCTOPUS_PROFESSIONAL},		// Sapienza Main Mission
	{{"CoastalTown"},	{"mamba"},		LOCATION_OCTOPUS_PROFESSIONAL},		// Landslide
	{{"CoastalTown"},	{"ebola"},		LOCATION_OCTOPUS_PROFESSIONAL},		// The Author
	{{"CoastalTown"},	{"copperhead"},	LOCATION_OCTOPUS_PROFESSIONAL},		// The Icon
	{{"marrakesh"},		{"spider"},		LOCATION_SPIDER_PROFESSIONAL},		// Marrakesh Main
	{{"marrakesh"},		{"python"},		LOCATION_SPIDER_PROFESSIONAL},		// A House Built On Sand
	{{"bangkok"},		{"tiger"},		LOCATION_TIGER_PROFESSIONAL},		// Bangkok Main Mission
	{{"bangkok"},		{"zika"},		LOCATION_TIGER_PROFESSIONAL},		// The Source
	{{"colorado_2"},	{"spider"},		LOCATION_BULL_PROFESSIONAL},		// Colorado Main Mission
	{{"colorado_2"},	{"rabies"},		LOCATION_SNIPER},					// The "Snipermission" we need to skip!
	{{"hokkaido"},		{"snowcrane"},	LOCATION_SNOWCRANE_PROFESSIONAL},	// Hokkaido Main Mission
	{{"hokkaido"},		{"mamushi"},	LOCATION_SNOWCRANE_PROFESSIONAL},	// Hokkaido Special Assignment
	// New Maps
	{{"sheep"},			{"newzealand"},	LOCATION_SHEEP_PROFESSIONAL},		// New Zealand Main Mission
	{{"miami"},			{"flamingo"},	LOCATION_FLAMINGO_PROFESSIONAL},	// Miami Main Mission
	{{"colombia"},		{"hippo"},		LOCATION_HIPPO_PROFESSIONAL},		// Colombia Main Mission
	{{"colombia"},		{"anaconda"},	LOCATION_HIPPO_PROFESSIONAL},		// Colombia Special Assignment
	{{"mumbai"},		{"mongoose"},	LOCATION_MONGOOSE_PROFESSIONAL},	// Mumbai Main Mission
	{{"mumbai"},		{"kingcobra"},	LOCATION_MONGOOSE_PROFESSIONAL},	// Mumbai Special Assignment
	{{"skunk"},			{"vermont"},	LOCATION_SKUNK_PROFESSIONAL},		// USA Main Mission
	{{"vermont"},		{"gartnersnake"},	LOCATION_SKUNK_PROFESSIONAL},	// USA Special Assignment
	{{"theark"},		{"magpie"},		LOCATION_MAGPIE_PROFESSIONAL},		// Island Sgail Main Mission
	{{"greedy"},		{"raccoon"},	LOCATION_RACCOON_PROFESSIONAL},		// New York Main Mission
	{{"opulent"},		{"stingray"},	LOCATION_STINGRAY_PROFESSIONAL},	// Haven Island Main Mission
	// Special Maps
	{{"hawk"},			{""},			LOCATION_SNIPER},		// Himmelstein Sniper
	{{"salty"},			{"seagull"},	LOCATION_SNIPER},		// Hantu Port Sniper
	{{"caged"},			{"falcon"},		LOCATION_SNIPER},		// Russia Sniper
};

typedef struct {
	size_t hash;
	location loc;
} thm;
#define HASHMAPLEN 29
thm hashmap[HASHMAPLEN] = {
	{0x00000000000035C9, LOCATION_PEACOCK_CASUAL},
	{0x000000000000376C, LOCATION_PEACOCK_MASTER},
	{0x0000000000003685, LOCATION_OCTOPUS_CASUAL},
	{0x0000000000002E09, LOCATION_OCTOPUS_MASTER},
	{0x00000000000039B1, LOCATION_SPIDER_CASUAL},
	{0x00000000000038E1, LOCATION_SPIDER_MASTER},
	{0x0000000000003636, LOCATION_TIGER_CASUAL},
	{0x0000000000003546, LOCATION_TIGER_MASTER},
	{0x0000000000003842, LOCATION_BULL_CASUAL},
	{0x00000000000033A0, LOCATION_BULL_MASTER},
	{0x0000000000002E5C, LOCATION_SNIPER},
	{0x0000000000003AC7, LOCATION_SNOWCRANE_CASUAL},
	{0x0000000000003A97, LOCATION_SNOWCRANE_MASTER},
	{0x00000000000048D5, LOCATION_SHEEP_CASUAL},
	{0x00000000000048C2, LOCATION_SHEEP_MASTER},
	{0x0000000000004A49, LOCATION_FLAMINGO_CASUAL},
	{0x0000000000004A36, LOCATION_FLAMINGO_MASTER},
	{0x0000000000004B8C, LOCATION_HIPPO_CASUAL},
	{0x0000000000004B79, LOCATION_HIPPO_MASTER},
	{0x0000000000004C2F, LOCATION_MONGOOSE_CASUAL},
	{0x0000000000004C1C, LOCATION_MONGOOSE_MASTER},
	{0x0000000000004841, LOCATION_SKUNK_CASUAL},
	{0x000000000000482E, LOCATION_SKUNK_MASTER},
	{0x0000000000004A68, LOCATION_MAGPIE_CASUAL},
	{0x0000000000004A55, LOCATION_MAGPIE_MASTER},
	{0x0000000000005DBE, LOCATION_RACCOON_CASUAL},
	{0x0000000000005DAB, LOCATION_RACCOON_MASTER},
	{0x00000000000062FB, LOCATION_STINGRAY_CASUAL},
	{0x00000000000062E8, LOCATION_STINGRAY_MASTER}
};

size_t lenofmapname(char* map) {
	for (size_t i = 0;;i++) {
		if (map[i] == '/')
			return i;
	}
	return 0;
}

location findMapName(ZString* zstr) {
	// assembly:/_PRO/scenes/missions/**mapname**/<<<nonconsistentafterhere>>>
	char* mapname = &(zstr->chars[31]);
	size_t lenmapname = lenofmapname(mapname);
	SPAM("Mapname: >%.*s< Len: %llu", (int)lenmapname, mapname, lenmapname);

	for (size_t i = 0; i < SZMAPNAMES; i++) {
		if (strlen((char*)&mapnames[i].mapname) != lenmapname)
			continue;
		if (strncasecmp((char*)&mapnames[i].mapname, mapname, lenmapname) == 0) {
			SPAM("Map is %s", (char*)&mapnames[i].mapname);
			return mapnames[i].loc;
		}

	}
	return NONE;
}

int32_t isFrontend(ZString* zstr) {
	// assembly:/_PRO/Scenes/**name**/**codename**
	char* mapname = &(zstr->chars[22]); //offset of above string
	char front[] = "Frontend";
	if (strncasecmp((char*)&front, mapname, strlen((char*)&front)) == 0)
		return true;
	else
		return false;
}

location SSceneInitParametersGetLocation(SSceneInitParameters* sip) {
	if (isFrontend(&(sip->m_SceneResource)))
		return LOCATION_SKIPME;

	//if hash not found, we default to mapname::professional
	location emap = findMapName(&(sip->m_SceneResource));
	INFO("Found Map %s", scenarioNames[emap]);

	size_t hash = 0;
	simplezstringhash(&hash, &(sip->m_SceneResource));
	for (size_t i = 0; i < Tasize(&(sip->m_aAdditionalBrickResources)); i++) {
		ZString* tmp = TaGetIndex(&sip->m_aAdditionalBrickResources, i);
		// We need to ignore "6core" and "8core" here
		if (strnstr(tmp->chars, "6core", tmp->len) ||
			strnstr(tmp->chars, "8core", tmp->len)) {
			continue;
		}
		simplezstringhash(&hash, tmp);
	}
	INFO("Hash is >%i< >%p<", (int)hash, (void*)hash);

	// Check if we have casual or master
	for (size_t i = 0; i < HASHMAPLEN; i++) {
		if (hashmap[i].hash == hash) {
			emap = hashmap[i].loc;
			INFO("Found specific map %s", scenarioNames[emap]);
			break;
		}
	}
	INFO("Final Map load: %s", scenarioNames[emap]);

	return emap;
}

#endif