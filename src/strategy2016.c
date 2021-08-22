#ifndef STRATEGY2016_C
#define STRATEGY2016_C

// Not much done here, 2016 has much less items, so no really fun strategy yet.

// Just in case a future very complex strategy needs any of them
void initialiseStrategies(void) {
	return;
}

// Strategies
const GUID* const strat_disabled(void) {
	SPAM("Strat Disabled");
	return NULL;
}

const GUID* const strat_randomfromclass(e_class class) {
	GETPTRTOCLASSARRAY(class)
			size_t randitem = rand() % itemarrlen;
			return itemrepo[randitem].guid;
	GETPTRTOCLASSARRAYEND

	ERR("Couldnt get random from class somehow (invalid class ptr?)");
	return NULL;
}

const GUID* const strat_randomfun(void) {
	size_t rndclass = rand() % (allitemreposARRLEN-2);
	//-2 ignores questitem and invalid

	// if we have boring class, reroll 2 times max
	for (size_t i = 0; i < 2; i++) {
		switch (air[rndclass].class_enum) {
			case c_melee:
			case c_pistol:
			case c_sniperrifle:
			case c_assaultrifle:
			case c_shotgun:
			case c_distraction:
			case c_smg:
				rndclass = rand() % (allitemreposARRLEN-2);
				break;
			default:
				goto happy;
				break;
		}
	}
	happy:;

	size_t rnditemfromclass = rand() % air[rndclass].len;
	return air[rndclass].itemrepo[rnditemfromclass].guid;
}

static size_t index[allitemreposARRLEN] = {0};
const GUID* const strat_seq(itemrepoinfo* class) {
	GETPTRTOCLASSARRAY(class->class_enum)
			size_t itemindex = index[iter];
			index[iter]++;
			if (index[iter] >= itemarrlen)
				index[iter] = 0;
			return itemrepo[itemindex].guid;
	GETPTRTOCLASSARRAYEND

	ERR("Couldnt get random from class somehow (invalid class ptr?)");
	return NULL;
}


// Caller Handlers
const GUID* const getRandomItem_world(itemrepoinfo* class) {
	switch (config.worldInventoryRandomizer) {
		case ES_DISABLED:
			return strat_disabled();
		case ES_DEFAULT:
			return strat_randomfromclass(class->class_enum);
		case ES_FUN:
			return strat_randomfun();
		case ES_CUSTOM1:
			return strat_seq(class);
		case ES_CUSTOM2:
		case ES_CUSTOM3:
		case ES_CUSTOM4:
		default:
			ERR("Mode not implemented for world");
			printStrategyFromEnum(config.worldInventoryRandomizer);
	} //switch
	return NULL;
}

//Guards dont like snipers,shotgun; but like pistol,assaultrifle,smg; buggy on others
const GUID* const getRandomItem_npc(itemrepoinfo* class) {
	switch (config.npcInventoryRandomizer) {
		case ES_DISABLED:
			return strat_disabled();
		case ES_DEFAULT:
			return strat_randomfromclass(class->class_enum);
		case ES_EASY:
		case ES_MEDIUM:
		case ES_HARD:
			return strat_randomfun();
		case ES_CUSTOM1:
			return strat_seq(class);
		case ES_CUSTOM2:
		case ES_CUSTOM3:
		case ES_CUSTOM4:
		default:
			ERR("Mode not implemented for npc");
			printStrategyFromEnum(config.npcInventoryRandomizer);
	} //switch
	return NULL;
}

const GUID* const getRandomItem_hero(itemrepoinfo* class) {
	switch (config.heroInventoryRandomizer) {
		case ES_DISABLED:
			return strat_disabled();
		case ES_DEFAULT:
			return strat_randomfromclass(class->class_enum);
		case ES_FUN:
			return strat_randomfun();
		case ES_CUSTOM1:
		case ES_CUSTOM2:
		case ES_CUSTOM3:
		case ES_CUSTOM4:
		default:
			ERR("Mode not implemented for hero");
			printStrategyFromEnum(config.heroInventoryRandomizer);
	} //switch
	return NULL;
}

const GUID* const getRandomItem_stash(itemrepoinfo* class) {
	switch (config.stashInventoryRandomizer) {
		case ES_DISABLED:
			return strat_disabled();
		case ES_DEFAULT:
			return strat_randomfromclass(class->class_enum);
		case ES_FUN:
			return strat_randomfun();
		case ES_CUSTOM1:
			return strat_seq(class);
		case ES_CUSTOM2:
		case ES_CUSTOM3:
		case ES_CUSTOM4:
		default:
			ERR("Mode not implemented for stash");
			printStrategyFromEnum(config.stashInventoryRandomizer);
	} //switch
	return NULL;
}

#endif