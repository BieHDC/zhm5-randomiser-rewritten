#ifndef RANDOMISER_C
#define RANDOMISER_C

// returns printf-able name
// returns item class ptr in class
const char noname[] = "UNKNOWN";
const char const* findItemInformation(const GUID* guid, itemrepoinfo** iteminfo) {
	LOOPALLITEMS
			if (IsEqualGUID(guid, item->guid)) {
				*iteminfo = currentclass;
				return item->name;
			}
	LOOPALLITEMSEND
	return (const char const*)&noname;
}

int isSkippedItem(const GUID* id, int ext) {
	for (size_t i = 0; i < ignoredARRLEN; i++) {
		if (IsEqualGUID(id, ignored_guid_name[i].guid)) {
			SPAM("Skipping Ignored!");
			return 1;
		}
	}

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

#define GETRANDOMITEM(id, inventory, repoID) 								\
	itemrepoinfo* class = NULL;												\
	const char* itemname = findItemInformation(repoID, &class);				\
	if (!class) {															\
		SPAM("ITEM NOT FOUND IN REPO!");									\
		printf_guid(*repoID);												\
		id = repoID;														\
	} 																		\
	else {																	\
		INFO("Found %s->%s", class->name, itemname);						\
																			\
		/*if extended mode is on, we dont check for questitem*/				\
		if (isSkippedItem(repoID, cini. inventory## InventoryIsExtended)) {	\
			id = repoID;													\
		}																	\
		else {																\
			id = getRandomItem_##inventory(class);	/*strategy.c*/			\
			if (!id) {														\
				id = repoID;												\
			}																\
		}																	\
	} 																		\

#endif