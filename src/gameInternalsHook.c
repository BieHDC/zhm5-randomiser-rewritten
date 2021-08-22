#ifndef GAMEINTERNALHOOK_C
#define GAMEINTERNALHOOK_C

// Game Version
typedef enum {
	gvUNK,
	gvH2DX11,
	gvH2DX12,
	gvH1DX11,
	gvH1DX12
} gameversion;

gameversion getGameVersion(void) {
	SPAM("Get Game Version");
	void* image_base = (void*)0x140000000;
	IMAGE_DOS_HEADER dos_header = *(IMAGE_DOS_HEADER*)image_base;
	IMAGE_NT_HEADERS nt_header = *(IMAGE_NT_HEADERS*)((uintptr_t)image_base + dos_header.e_lfanew);

	SPAM("Header at %p and timestamp is %lX", (void*)&nt_header, (long unsigned)nt_header.FileHeader.TimeDateStamp);

	int timestamp = nt_header.FileHeader.TimeDateStamp;
	switch (timestamp) {
		// Hitman 2
		case 0x5EE9D095: //pre last update
		case 0x5F8D56D3: //last update
			return gvH2DX12;
		case 0x5EE9D065: //pre last update
		case 0x5F8D57CA: //last update
			return gvH2DX11;

		// Hitman 2016
		case 0x5F8ED8B9: //latest version
			return gvH1DX11;
		case 0x5F8ED8D0: //latest version
			return gvH1DX12;
		default:
			ERR("Unknown Game Version: %X", timestamp);
			return gvUNK;

	} //switch
	return gvUNK;
}


// Game Function Pointers
typedef struct {
	// Used Functions
	void* pPushItem;

	// Hooked Functions
	void* pPushWorldInventoryDetour;
	void* pPushNPCInventoryDetour;
	void* pPushHeroInventoryDetour;
	void* pPushStashInventoryDetour;
	void** pZEntitySceneContext_LoadScene;

	// 2016 only
	void* pPushSharedNpcWorld;
	void* pPushWorldInventoryDetour2;
} gamefunctions;
static gamefunctions gfn = {0};

gameversion getGameFnPointers(void) {
	gameversion version = getGameVersion();
	switch (version) {
	case gvH2DX12:
		INFO("Game is Hitman 2 DX12");
		gfn.pPushItem = (void*)(0x140C24650);
		gfn.pPushNPCInventoryDetour = (void*)(0x140C24BD0);
		gfn.pPushWorldInventoryDetour = (void*)(0x140C24581);
		gfn.pPushHeroInventoryDetour = (void*)(0x1405D7217);
		gfn.pPushStashInventoryDetour = (void*)(0x14059039A);
		gfn.pZEntitySceneContext_LoadScene = (void**)(0x1416AEE68);
		return version;
	case gvH2DX11:
		INFO("Game is Hitman 2 DX11");
		gfn.pPushItem = (void*)(0x140C24AF0);
		gfn.pPushNPCInventoryDetour = (void*)(0x140C25070);
		gfn.pPushWorldInventoryDetour = (void*)(0x140C24A21);
		gfn.pPushHeroInventoryDetour = (void*)(0x1405D78F7);
		gfn.pPushStashInventoryDetour = (void*)(0x140590A7A);
		gfn.pZEntitySceneContext_LoadScene = (void**)(0x141693D70);
		return version;
	case gvH1DX11:
		INFO("Game is Hitman 2016 DX11");
		gfn.pPushItem = (void*)(0x140a70b50);
		gfn.pPushHeroInventoryDetour = (void*)(0x14095d174);
		gfn.pPushStashInventoryDetour = (void*)(0x14037281b);
		gfn.pZEntitySceneContext_LoadScene = (void**)(0x141397928);

		gfn.pPushSharedNpcWorld = (void*)(0x140a70ec0);
		gfn.pPushNPCInventoryDetour = (void*)(0x1401b82ad);
		gfn.pPushWorldInventoryDetour = (void*)(0x140a708e0);
		gfn.pPushWorldInventoryDetour2 = (void*)(0x140a709a6);
		return version;
	case gvH1DX12:
		INFO("Game is Hitman 2016 DX12");
		gfn.pPushItem = (void*)(0x140a71ea0);
		gfn.pPushHeroInventoryDetour = (void*)(0x14095e704);
		gfn.pPushStashInventoryDetour = (void*)(0x14037319b);
		gfn.pZEntitySceneContext_LoadScene = (void**)(0x14139e988);

		gfn.pPushSharedNpcWorld = (void*)(0x140a72210);
		gfn.pPushNPCInventoryDetour = (void*)(0x1401b79bd);
		gfn.pPushWorldInventoryDetour = (void*)(0x140a71c30);
		gfn.pPushWorldInventoryDetour2 = (void*)(0x140a71cf6);
		return version;
	default:
		return gvUNK;
	} //switch
	return gvUNK;
}


// Hookering Logic (Better not touch)
static void* trampoline_memory_base = NULL;

//the functions replaced are all directly on a call with 5 bytes size fn,
//so hooking anything else will cause issues!
void detourCall(void* hook_call_addr, const void* hook_function) {
	SPAM("Detour call %p %p", hook_call_addr, hook_function);
	if(trampoline_memory_base == NULL) {
		SPAM("Allocating Trampoline");
		trampoline_memory_base = VirtualAlloc((LPVOID)0x17F000000, 0x200, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
		SPAM("Trampoline allocated at %p", trampoline_memory_base);
	}

	#define TRAMPONLINE_SIZE (unsigned int)17
	void* trampoline_addr = trampoline_memory_base;
	while (*(char*)trampoline_addr == 0x48) //What?
		trampoline_addr = (void*)((long long)trampoline_addr + TRAMPONLINE_SIZE);

	//Build hook jmp
	int jmp_operand = (int)((long long)trampoline_addr - ((long long)hook_call_addr + 5));

	unsigned char hook_bytes[5] = { //hmm
		0xE9, 0x00, 0x00, 0x00, 0x00 // jmp rip+XXXXXXXX
	};
	*(int*)(hook_bytes + 1) = jmp_operand;

	DWORD oldProtection;
	VirtualProtect(hook_call_addr, 0x05, PAGE_EXECUTE_READWRITE, &oldProtection);
	memcpy_s(hook_call_addr, sizeof(hook_bytes), hook_bytes, sizeof(hook_bytes));
	VirtualProtect(hook_call_addr, 0x05, oldProtection, &oldProtection);

	//Build trampoline call
	unsigned char trampoline_bytes[TRAMPONLINE_SIZE] = {
		0x48, 0xB8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //mov rax, XXXXXXXXXXXXXXXX
		0xFF, 0xD0,													//call rax
		0xE9, 0x00, 0x00, 0x00, 0x00								//jmp rip+XXXXXXXX
	};

	*(long long*)(trampoline_bytes + 2) = (long long)hook_function;
	int return_jmp_operand = ((long long)hook_call_addr + 5) - ((long long)trampoline_addr + 17);
	*(int*)(trampoline_bytes + 13) = return_jmp_operand;
	memcpy_s(trampoline_addr, sizeof(trampoline_bytes), trampoline_bytes, sizeof(trampoline_bytes));
	SPAM("Hooked");
}

void detourVFTCall(void** vft_entry_addr, void* hook_function, void** original_fn_ptr) {
	SPAM("Detour vftcall %p %p %p", vft_entry_addr, hook_function, original_fn_ptr);
	*original_fn_ptr = *vft_entry_addr;

	DWORD oldProtection;
	VirtualProtect(vft_entry_addr, sizeof(void*), PAGE_EXECUTE_READWRITE, &oldProtection);
	*vft_entry_addr = hook_function;
	VirtualProtect(vft_entry_addr, sizeof(void*), oldProtection, &oldProtection);
	SPAM("Hooked");
}

#if 0
// Might be useful in the future
BOOL nopper(uint8_t* address, size_t size) {
	SPAM("Nopping %p with size %llu", address, size);
	DWORD oldProtection;
	BOOL res = 0;
	res = VirtualProtect(address, size, PAGE_EXECUTE_READWRITE, &oldProtection);
	if (!res)
		return res;
	//DEBUG
	uint8_t* debugaddress = address-3;
	SPAM("Oldbytes:");
	printf("\t");
	for (size_t i = 0; i < size+6; i++) {
		printf("%02hhX ", *(debugaddress+i));
	}
	printf("\n");
	//DEBUG

	for (size_t i = 0; i < size; i++) {
		*(address+i) = 0x90; //NOP
	}

	//DEBUG
	SPAM("Newbytes:");
	printf("\t");
	for (size_t i = 0; i < size+6; i++) {
		printf("%02hhX ", *(debugaddress+i));
	}
	printf("\n");
	//DEBUG
	res = VirtualProtect(address, size, oldProtection, &oldProtection);
	fflush(stdout);
	return res;
}

BOOL patchbytes(uint8_t* address, uint8_t* newbytes, size_t size) {
	SPAM("Patching %p with size %llu", address, size);
	DWORD oldProtection;
	BOOL res = 0;
	res = VirtualProtect(address, size, PAGE_EXECUTE_READWRITE, &oldProtection);
	if (!res)
		return res;
	//DEBUG
	uint8_t* debugaddress = address-3;
	SPAM("Oldbytes:");
	printf("\t");
	for (size_t i = 0; i < size+6; i++) {
		printf("%02hhX ", *(debugaddress+i));
	}
	printf("\n");
	//DEBUG

	for (size_t i = 0; i < size; i++) {
		*(address+i) = *(newbytes+i);
	}

	//DEBUG
	SPAM("Newbytes:");
	printf("\t");
	for (size_t i = 0; i < size+6; i++) {
		printf("%02hhX ", *(debugaddress+i));
	}
	printf("\n");
	//DEBUG
	res = VirtualProtect(address, size, oldProtection, &oldProtection);
	fflush(stdout);
	return res;
}
#endif
#if 0
// Might be useful again at some point
// You can use this to read random pointers "safely"
#include <setjmp.h>
static jmp_buf context;
long WINAPI myexcfilter(struct _EXCEPTION_POINTERS* ignored) {
	longjmp(context, 1);
}

void printzstringguidw(void* p, char* name) {
	LPTOP_LEVEL_EXCEPTION_FILTER prev = SetUnhandledExceptionFilter(myexcfilter);
	if (setjmp(context) == 0) {
		printf("%s: %p\n\tZstr = >%.*s<\n\t", name, p, ((ZString*)p)->len, ((ZString*)p)->chars);
		printf_guid(*(GUID*)p);
	}
	else
    {
    	printf("\rFAIL:\t%s: %llx\n", name, p);
    }
    SetUnhandledExceptionFilter(prev);

}
#define printzstringguid(p) printzstringguidw(p, #p)
#endif


#if defined(HITMAN2)
typedef __int64(__fastcall *tPushItem)(void*, const GUID*, 
										ZString*, void*, 
										__int64, uint8_t, 
										__int64*, void*, 
										void*, char);

#define DEF_pushItem(inventory) 																	\
static int64_t __fastcall pushItem##inventory (														\
					void* worldInventory, const GUID* repoID, 										\
					ZString* a3, void* a4, int64_t a5, uint8_t a6, int64_t* a7, 					\
					void* a8, void* a9, char a10) {													\
	SPAM("pushItem for >%s< called", #inventory);													\
	/* We ignore sniper maps because it just makes no sense */										\
	if (currentmap == LOCATION_SNIPER || currentmap == LOCATION_SKIPME)								\
		return ((tPushItem)gfn.pPushItem)(worldInventory, repoID, a3, a4, a5, a6, a7, a8, a9, a10);	\
																									\
	const GUID* id;																					\
	GETRANDOMITEM(id, inventory, repoID);															\
	return ((tPushItem)gfn.pPushItem)(worldInventory, id, a3, a4, a5, a6, a7, a8, a9, a10);			\
};
DEF_pushItem(world); //pushItemworld
DEF_pushItem(npc); //pushItemnpc
DEF_pushItem(hero); //pushItemhero
DEF_pushItem(stash); //pushItemstash
#endif


#if defined(HITMAN2016)
typedef __int64(__fastcall *tPushItem16)(void*, const GUID*,
										void*, void*,
										void*, void*,
										void*, void*,
										char);

#define DEF_pushItem16(inventory) 																	\
static int64_t __fastcall pushItem16##inventory (													\
					void* worldInventory, const GUID* repoID, 										\
					void* a3, void* a4, void* a5, void* a6, void* a7, 								\
					void* a8, char a10) {															\
	SPAM("pushItem for >%s< called", #inventory);													\
	if (currentmap == LOCATION_SNIPER || currentmap == LOCATION_SKIPME)								\
		return ((tPushItem16)gfn.pPushItem)(worldInventory, repoID, a3, a4, a5, a6, a7, a8, a10);	\
																									\
	const GUID* id;																					\
	GETRANDOMITEM(id, inventory, repoID);															\
	return ((tPushItem16)gfn.pPushItem)(worldInventory, id, a3, a4, a5, a6, a7, a8, a10);			\
};
DEF_pushItem16(hero); //pushItem16hero
DEF_pushItem16(stash); //pushItem16stash

typedef uint64_t(__fastcall *pushItemNpcWorldShared)(long long p1,const GUID* guid,
														void* p3, uint64_t p4,
														void* p5, void* p6);

#define DEF_pushItem16_2(inventory) 															\
static uint64_t __fastcall pushItem16##inventory(long long p1,const GUID* repoID,				\
													void* p3, uint64_t p4,						\
													void* p5, void* p6) {						\
	SPAM("pushItem for >%s< called", #inventory);												\
	if (currentmap == LOCATION_SNIPER || currentmap == LOCATION_SKIPME)							\
		return ((pushItemNpcWorldShared)gfn.pPushSharedNpcWorld)(p1, repoID, p3, p4, p5, p6);	\
																								\
	const GUID* id;																				\
	GETRANDOMITEM(id, inventory, repoID);														\
	return ((pushItemNpcWorldShared)gfn.pPushSharedNpcWorld)(p1, id, p3, p4, p5, p6);		\
}
DEF_pushItem16_2(npc); //pushItem16npc
DEF_pushItem16_2(world); //pushItem16world
#endif


// tLoadScene
typedef uint64_t(__fastcall *tLoadScene)(void* this_, SSceneInitParameters* scene_init_params);

uint64_t __fastcall LoadSceneDetour(void* this_, SSceneInitParameters* scene_init_params) {
	SPAM("SSceneLoadObserver detour %p %p", this_, scene_init_params);
	#ifdef DEBUG
	SSceneInitParametersPrint(scene_init_params);
	#endif
	currentmap = SSceneInitParametersGetLocation(scene_init_params);

	// Pick a random number if not specified in the config file
	if (config.RNGSeed == 0) {
		struct timeval t;
		mingw_gettimeofday(&t, NULL);
		DWORD randomseed = (t.tv_usec * t.tv_sec * rand());
		srand(randomseed);
		INFO("Random Seed is: %lu", randomseed);
	}
	else {
		// We reset the seed to initial value every time if specified in the config file
		srand(config.RNGSeed);
	}

	return ((tLoadScene)gfn.pZEntitySceneContext_LoadScene)(this_, scene_init_params);
}


int hookGameFunctions(void) {
	SPAM("Get Game Functions");
	gameversion version = getGameFnPointers();

	SPAM("Hook Game Functions");
	#if defined(HITMAN2)
	if (version == gvH2DX11 || version == gvH2DX12) {
		detourCall(gfn.pPushWorldInventoryDetour, pushItemworld);
		detourCall(gfn.pPushNPCInventoryDetour, pushItemnpc);
		detourCall(gfn.pPushHeroInventoryDetour, pushItemhero);
		detourCall(gfn.pPushStashInventoryDetour, pushItemstash);
	}
	else if (version == gvH1DX11 || version == gvH1DX12) {
		MessageBoxA(NULL, "Wrong DLL, use dinput8.dll.2016", "", 0);
	}

	#elif defined(HITMAN2016)
	if (version == gvH1DX11 || version == gvH1DX12) {
		detourCall(gfn.pPushHeroInventoryDetour, pushItem16hero);
		detourCall(gfn.pPushStashInventoryDetour, pushItem16stash);

		detourCall(gfn.pPushNPCInventoryDetour, pushItem16npc);
		detourCall(gfn.pPushWorldInventoryDetour, pushItem16world);
		detourCall(gfn.pPushWorldInventoryDetour2, pushItem16world);
	}
	else if (version == gvH2DX11 || version == gvH2DX12) {
		MessageBoxA(NULL, "Wrong DLL, use dinput8.dll.2", "", 0);
	}
	#endif

	else {
		MessageBoxA(NULL, "Unknown Game Version, disabling mod", "", 0);
		return 0;
	}

	detourVFTCall(gfn.pZEntitySceneContext_LoadScene, &LoadSceneDetour, (void**)&gfn.pZEntitySceneContext_LoadScene);
	SPAM("Game Functions hooked!");
	return 1;
};

#endif
