#ifndef STRATEGY_C
#define STRATEGY_C

// You can use this function/section to set up certain things for your strategies
// For example, i use it to get certain item's GUID for ES_{EASY/MEDIUM/HARD} and more

// NPC HARDNESS GUIDS
const GUID ES_EASY_GRANADE	 = {0x042fae7b,0xfe9e,0x4a83,0xac,0x7b,0x5c,0x91,0x4a,0x71,0xb2,0xca}; //(Grenade) Flash
const GUID ES_MEDIUM_GRANADE = {0x3f9cf03f,0xb84f,0x4419,0xb8,0x31,0x47,0x04,0xcf,0xf9,0x77,0x5c}; //Fragmentation Grenade
const GUID ES_HARD_WEAPON 	 = {0x15291f69,0x88d0,0x4a8f,0xb3,0x1b,0x71,0x60,0x5b,0xa5,0xff,0x38}; //Striker
const GUID ES_HARD_GRANADE	 = {0xc82fefa7,0xfebe,0x46c8,0x90,0xec,0xc9,0x45,0xfb,0xef,0x0c,0xb4}; //Octane Booster

// No Heavy Pistol GUIDs
const GUID STRAT_NOHEAVYPISTOL_BLACKLIST_1 = {0xbbd802e2,0x7b21,0x46d8,0x8f,0x59,0x9d,0xbb,0x45,0x2f,0xf9,0x95}; //Firearms Hero Pistol Magnum Base
const GUID STRAT_NOHEAVYPISTOL_BLACKLIST_2 = {0x15291f69,0x88d0,0x4a8f,0xb3,0x1b,0x71,0x60,0x5b,0xa5,0xff,0x38}; //Firearms_Hero_Pistol_magnum_GOTY_Striker
const GUID STRAT_NOHEAVYPISTOL_BLACKLIST_3 = {0x77ecaad6,0x652f,0x480d,0xb3,0x65,0xcd,0xf9,0x08,0x20,0xa5,0xec}; //(Pistol) El Matador
const GUID STRAT_NOHEAVYPISTOL_BLACKLIST_4 = {0xc44d3dc4,0x5e2f,0x403d,0x9f,0x69,0x40,0x11,0x1f,0x36,0x43,0xdb}; //(Pistol) The Wall Piecer

// Troll/ES_CUSTOM1 GUIDs
const GUID ES_CUSTOM1_RAKE 		= {0x81654161,0x7711,0x4985,0x80,0x56,0x86,0x51,0xa3,0x81,0xd3,0xca}; //Rake
const GUID ES_CUSTOM1_BANANNA 	= {0x903d273c,0xc750,0x441d,0x91,0x6a,0x31,0x55,0x7f,0xea,0x33,0x82}; //Banana

// ES_CUSTOM3 GUID
const GUID SLOWSUITCASE = {0x8493fe56,0x8303,0x4285,0x94,0xc1,0x06,0xf9,0x6c,0x60,0x4d,0xba}; //(Container) ICA Briefcase Slow MK II

// ES_CUSTOM4 GUID
const GUID NULLGUID =  {0x00000000,0x0000,0x0000,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};


// Just in case a future very complex strategy needs any of them
void initialiseStrategies(void) {
	return;
}

void preMapLoadTrigger(void) {
	return;
}

void postMapLoadTrigger(void) {
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
	that it will also replace "quest items", breaking some mission stories etc..,
	however items needed to finish a mission should stick (clues, datacore, ...).
	Usually only makes sense for World and NPC, since Stash and Hero dont have
	such items.


	There are also 4 custom modes filled with examples:
	ES_CUSTOM1 - Troll Items - Accident Pacification Items, Banana and Rake for now, suggest more!
	ES_CUSTOM2 - Sequential - Loops through all items in a class sequentually
	ES_CUSTOM3 - All Suitcase - Replaces every item with a Suitcase, the fun one of course
	ES_CUSTOM4 - No Items - You can disarm NPCs with that, or the whole map...

	To add more mode slots, see types.h:115,129,149
	And dont forget to add them to "getRandomItem_##inventory"!

	Returning a NULLptr from a strategy results in the actually requested item being spawned,
	see strat_disabled, which is useful if you want to only replace certain items or classes.
	If you want to "delete" an Item, return "&NULLGUID" as seen in ES_CUSTOM4.
*/

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

const GUID* const strat_noheavypistol(void) {
	GETPTRTOCLASSARRAY(c_pistol)
			size_t randitem = 0;
			while ((randitem = rand() % itemarrlen)) {
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
			return itemrepo[randitem].guid;
	GETPTRTOCLASSARRAYEND

	ERR("Couldnt get random from class somehow (invalid class ptr?)");
	return NULL;
}

const GUID* const strat_assault_smg(void) {
	size_t rndclass = rand() % 2;
	if (rndclass == 0) {
		return strat_randomfromclass(c_assaultrifle);
	} 
	else {
		return strat_randomfromclass(c_smg);
	}

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
			case c_suitcase: 
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

const GUID* const strat_troll(void) {
	size_t rndclass = rand() % 2;
	if (rndclass == 0) {
		return &ES_CUSTOM1_RAKE;
	} 
	else {
		return &ES_CUSTOM1_BANANNA;
	}

	return NULL;
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
	switch (cini.worldInventoryRandomizer) {
		case ES_DISABLED:
			return strat_disabled();
		case ES_DEFAULT:
			return strat_randomfromclass(class->class_enum);
		case ES_FUN:
			return strat_randomfun();
		case ES_CUSTOM1:
			return strat_troll();
		case ES_CUSTOM2:
			return strat_seq(class);
		case ES_CUSTOM3:
			return &SLOWSUITCASE;
		case ES_CUSTOM4:
			return &NULLGUID;
		default:
			ERR("Mode not implemented for world");
			printStrategyFromEnum(cini.worldInventoryRandomizer);
	} //switch
	return NULL;
}

//Guards dont like snipers,shotgun; but like pistol,assaultrifle,smg; buggy on others
const GUID* const getRandomItem_npc(itemrepoinfo* class) {
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
			INFO("Troll items on NPCs might be a bad idea!");
			return strat_troll();
		case ES_CUSTOM2:
			return strat_seq(class);
		case ES_CUSTOM3:
			INFO("All suitcase is going to bug NPCs!");
			return &SLOWSUITCASE;
		case ES_CUSTOM4:
			return &NULLGUID;
		default:
			ERR("Mode not implemented for npc");
			printStrategyFromEnum(cini.npcInventoryRandomizer);
	} //switch
	return NULL;
}

const GUID* const getRandomItem_hero(itemrepoinfo* class) {
	switch (cini.heroInventoryRandomizer) {
		case ES_DISABLED:
			return strat_disabled();
		case ES_DEFAULT:
			return strat_randomfromclass(class->class_enum);
		case ES_FUN:
			return strat_randomfun();
		case ES_CUSTOM1:
			return strat_troll();
		case ES_CUSTOM2:
			return strat_seq(class);
		case ES_CUSTOM3:
			return &SLOWSUITCASE;
		case ES_CUSTOM4:
			return &NULLGUID;
		default:
			ERR("Mode not implemented for hero");
			printStrategyFromEnum(cini.heroInventoryRandomizer);
	} //switch
	return NULL;
}

const GUID* const getRandomItem_stash(itemrepoinfo* class) {
	switch (cini.stashInventoryRandomizer) {
		case ES_DISABLED:
			return strat_disabled();
		case ES_DEFAULT:
			return strat_randomfromclass(class->class_enum);
		case ES_FUN:
			return strat_randomfun();
		case ES_CUSTOM1:
			return strat_troll();
		case ES_CUSTOM2:
			return strat_seq(class);
		case ES_CUSTOM3:
			return &SLOWSUITCASE;
		case ES_CUSTOM4:
			return &NULLGUID;
		default:
			ERR("Mode not implemented for stash");
			printStrategyFromEnum(cini.stashInventoryRandomizer);
	} //switch
	return NULL;
}

#endif