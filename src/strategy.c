#ifndef STRATEGY_C
#define STRATEGY_C

// You can use this function/section to set up certain things for your strategies
// For example, i use it to get certain item's GUID for ES_{EASY/MEDIUM/HARD}

// NPC HARDNESS GUIDS
const GUID ES_EASY_GRANADE = {0x042fae7b,0xfe9e,0x4a83,0xac,0x7b,0x5c,0x91,0x4a,0x71,0xb2,0xca}; //(Grenade) Flash
const GUID ES_MEDIUM_GRANADE = {0x3f9cf03f,0xb84f,0x4419,0xb8,0x31,0x47,0x04,0xcf,0xf9,0x77,0x5c}; //Fragmentation Grenade
const GUID ES_HARD_WEAPON = {0x15291f69,0x88d0,0x4a8f,0xb3,0x1b,0x71,0x60,0x5b,0xa5,0xff,0x38}; //Striker
const GUID ES_HARD_GRANADE = {0xc82fefa7,0xfebe,0x46c8,0x90,0xec,0xc9,0x45,0xfb,0xef,0x0c,0xb4}; //Octane Booster

// No Heavy Pistol GUIDs
const GUID STRAT_NOHEAVYPISTOL_BLACKLIST_1 = {0xbbd802e2,0x7b21,0x46d8,0x8f,0x59,0x9d,0xbb,0x45,0x2f,0xf9,0x95}; //Firearms Hero Pistol Magnum Base
const GUID STRAT_NOHEAVYPISTOL_BLACKLIST_2 = {0x15291f69,0x88d0,0x4a8f,0xb3,0x1b,0x71,0x60,0x5b,0xa5,0xff,0x38}; //Firearms_Hero_Pistol_magnum_GOTY_Striker
const GUID STRAT_NOHEAVYPISTOL_BLACKLIST_3 = {0x77ecaad6,0x652f,0x480d,0xb3,0x65,0xcd,0xf9,0x08,0x20,0xa5,0xec}; //(Pistol) El Matador
const GUID STRAT_NOHEAVYPISTOL_BLACKLIST_4 = {0xc44d3dc4,0x5e2f,0x403d,0x9f,0x69,0x40,0x11,0x1f,0x36,0x43,0xdb}; //(Pistol) The Wall Piecer


void initialiseStrategies(void) {
	return;
}

/*MODES:
	World:
	DISABLED
	DEFAULT - In Class Random
	DEFAULT+ - Same as DEFAULT, but also replaces questitems (breaking mission stories)
	FUN - Prioritises fun items: explosives, tools, poison
	FUN+ - Same as FUN, but also replaces questitems (breaking mission stories)

	NPC:
	DISABLED
	DEFAULT - In Class Random
	EASY - Random low powered pistol, flash granade
	MEDIUM - Random assaultrifle or smg, fragmentation granade
	HARD - All guards have striker (basically one hit kill), Octane Booster

	Hero:
	DISABLED
	DEFAULT - In Class Random
	FUN - Prioritises fun items: explosives, tools, poison
		(might cause you to spawn with a suspicious item)

	Stash:
	DISABLED
	DEFAULT - In Class Random
	FUN - Prioritises fun items: explosives, tools, poison


	Generally, adding a "+" to the strategy enables extended mode, which means
	that it will also replace "quest items", breaking some mission stories etc..


	There are also 4 custom modes filled with examples:
	ES_CUSTOM1 fixme make implementation
	ES_CUSTOM2 fixme make implementation
	ES_CUSTOM3 fixme make implementation
	ES_CUSTOM4 fixme make implementation
*/

// Strategies
const GUID const* strat_disabled(void) {
	INFO("Strat Disabled");
	return NULL;
}

const GUID const* strat_randomfromclass(e_class class) {
	GETPTRTOCLASSARRAY(class)
			size_t randitem = rand() % itemarrlen;
			INFO("Returning %s->%s", itemtypename, itemrepo[randitem].name);
			return itemrepo[randitem].guid;
	GETPTRTOCLASSARRAYEND

	ERR("Couldnt get random from class somehow (invalid class ptr?)");
	return NULL;
}

const GUID const* strat_noheavypistol(void) {
	GETPTRTOCLASSARRAY(c_pistol)
			size_t randitem = 0;
			while (randitem = rand() % itemarrlen) {
				if ((IsEqualGUID(&STRAT_NOHEAVYPISTOL_BLACKLIST_1, itemrepo[randitem].guid)) ||
					(IsEqualGUID(&STRAT_NOHEAVYPISTOL_BLACKLIST_2, itemrepo[randitem].guid)) ||
					(IsEqualGUID(&STRAT_NOHEAVYPISTOL_BLACKLIST_3, itemrepo[randitem].guid)) ||
					(IsEqualGUID(&STRAT_NOHEAVYPISTOL_BLACKLIST_4, itemrepo[randitem].guid))) {
						SPAM("HEAVY PISTOL, rerolling");
					}
				else {
					break;
				}
			}
			INFO("Returning %s->%s", itemtypename, itemrepo[randitem].name);
			return itemrepo[randitem].guid;
	GETPTRTOCLASSARRAYEND

	ERR("Couldnt get random from class somehow (invalid class ptr?)");
	return NULL;
}

const GUID const* strat_assault_smg(void) {
	size_t rndclass = rand() % 2;
	if (rndclass == 0) {
		return strat_randomfromclass(c_assaultrifle);
	} 
	else {
		return strat_randomfromclass(c_smg);
	}

	return NULL;
}

const GUID const* strat_randomfun(void) {
	size_t rndclass = rand() % (allitemreposARRLEN-3);
	//-3 ignores questitem, invalid and ignored

	// if we have boring class, reroll 2 times max
	for (size_t i = 0; i < 2; i++) {
		switch (air[rndclass].class_enum) {
			case c_melee: 
			case c_pistol: 
			case c_sniperrifle:
			case c_assaultrifle: 
			case c_shotgun: 
			case c_suitcase: 
			case c_distraction: 
			case c_smg: 
				rndclass = rand() % (allitemreposARRLEN-3);
				break;
			default:
				goto happy;
				break;
		}
	}
	happy:;//fixme this might be bugged now //why?

	size_t rnditemfromclass = rand() % air[rndclass].len;
	SPAM("Returning %s->%s", air[rndclass].name, air[rndclass].itemrepo[rnditemfromclass].name);
	return air[rndclass].itemrepo[rnditemfromclass].guid;
}


// Caller Handler
const GUID* getRandomItem_world(itemrepoinfo* class) {
	switch (cini.worldInventoryRandomizer) {
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
			ERR("Mode not implemented for world");
			printStrategyFromEnum(cini.worldInventoryRandomizer);
	}
	return NULL;
}

//guards dont like snipers,shotgun; but like pistol,assaultrifle,smg; buggy on others
const GUID* getRandomItem_npc(itemrepoinfo* class) {
	switch (cini.npcInventoryRandomizer) {
		case ES_DISABLED:
			return strat_disabled();
		case ES_DEFAULT:
			return strat_randomfromclass(class->class_enum);
		case ES_EASY:
			if (class->class_enum == c_explosives)
				return &ES_EASY_GRANADE;
			else
				return strat_noheavypistol();
		case ES_MEDIUM:
			if (class->class_enum == c_explosives)
				return &ES_MEDIUM_GRANADE;
			else
				return strat_assault_smg();
		case ES_HARD:
			if (class->class_enum == c_explosives)
				return &ES_HARD_GRANADE;
			else
				return &ES_HARD_WEAPON;
		case ES_CUSTOM1:
		case ES_CUSTOM2:
		case ES_CUSTOM3:
		case ES_CUSTOM4:
		default:
			ERR("Mode not implemented for npc");
			printStrategyFromEnum(cini.npcInventoryRandomizer);
	}
	return NULL;
}

const GUID* getRandomItem_hero(itemrepoinfo* class) {
	switch (cini.heroInventoryRandomizer) {
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
			printStrategyFromEnum(cini.heroInventoryRandomizer);
	}
	return NULL;
}

const GUID* getRandomItem_stash(itemrepoinfo* class) {
	switch (cini.stashInventoryRandomizer) {
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
			ERR("Mode not implemented for stash");
			printStrategyFromEnum(cini.stashInventoryRandomizer);
	}
	return NULL;
}

#endif