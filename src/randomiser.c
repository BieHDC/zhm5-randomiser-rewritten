#ifndef RANDOMISER_C
#define RANDOMISER_C

// returns printf-able name
// returns item class ptr in class
const char noname[] = "UNKNOWN";
const char* const itemFindInformation(const GUID* guid, itemrepoinfo** iteminfo, int32_t dir) {
	LOOPALLITEMS
			if (IsEqualGUID(guid, item->guid)) {
				*iteminfo = currentclass;
				if (dir == 0)
					SPAM("Found %s->%s", currentclass->name, item->name);
				else
					SPAM("Returning %s->%s", currentclass->name, item->name);
				return item->name;
			}
	LOOPALLITEMSEND
	*iteminfo = NULL;
	return (const char* const)&noname;
}

#ifdef DEBUG
#define itemFindInformationOpt(g,i,d) itemFindInformation(g,i,d)
#else
#define itemFindInformationOpt(g,i,d)
#endif


int itemIsSkipped(const GUID* id, int ext) {
	if (!ext) {
		for (size_t i = 0; i < questitemARRLEN; i++) {
			if (IsEqualGUID(id, questitem_guid_name[i].guid)) {
				SPAM("Skipping Questitem!");
				return 1;
			}
		}
	}

	return 0;
}

#define GETRANDOMITEM(id, inventory, repoID) 									\
	itemrepoinfo* class = NULL;													\
	const char* itemname = itemFindInformation(repoID, &class, 0);				\
	if (!class) {																\
		INFO("Unknown item, ignoring...");										\
		printf_guid(*repoID); /* Ignored item, but you can double check */		\
		(void)itemname;															\
		id = repoID;															\
	} 																			\
	else {																		\
		/*if extended mode is off, we ignore questitems and dont replace them*/	\
		if (itemIsSkipped(repoID, config. inventory## InventoryIsExtended)) {	\
			id = repoID;														\
		}																		\
		else {																	\
			id = getRandomItem_##inventory(class);	/*strategy.c*/				\
			if (!id) {															\
				id = repoID;													\
			}																	\
			itemFindInformationOpt(id, &class, 1);								\
		}																		\
	} 																			\

#endif