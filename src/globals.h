#ifndef GLOBALS_H
#define GLOBALS_H

typedef struct {
	e_strategy worldInventoryRandomizer;
	int32_t worldInventoryIsExtended;
	e_strategy npcInventoryRandomizer;
	int32_t npcInventoryIsExtended;
	e_strategy heroInventoryRandomizer;
	int32_t heroInventoryIsExtended;
	e_strategy stashInventoryRandomizer;
	int32_t stashInventoryIsExtended;
	DWORD RNGSeed;
} configINI;

configINI config = {0};

location currentmap = LOCATION_SKIPME;

#endif //GLOBALS_H