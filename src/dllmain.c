// Generic Includes
#include <windows.h>
#include <stdio.h>
#include <time.h>


//#define DEBUG
#ifdef DEBUG
#warning DEBUG MODE ON
#endif

#include "utils.h"
#include "types.h"
#include "globals.h"
#include "config.c"
#include "gameInternalTypes.c"
#if defined(HITMAN2)
	#include "resources/REPO_hitman2/Repository2.c"
	#include "strategy2.c" // The file you are likely interested in
#elif defined(HITMAN2016)
	#include "resources/REPO_hitman2016/Repository2016.c"
	#include "strategy2016.c" // The file you are likely interested in
#else
	#error PLEASE DEFINE A HITMAN VERSION (HITMAN2,HITMAN2016)
#endif
#include "randomiser.c"
#include "gameInternalsHook.c"


typedef HRESULT(__stdcall *DIRECTINPUT8CREATE)(HINSTANCE, DWORD, REFIID, LPVOID*, LPUNKNOWN);
DIRECTINPUT8CREATE fpDirectInput8Create;
__declspec(dllexport) HRESULT __stdcall DirectInput8Create(HINSTANCE hinst, DWORD dwVersion, REFIID riidltf, LPVOID* ppvOut, LPUNKNOWN punkOuter) {
	if (!fpDirectInput8Create) {
		char syspath[PATH_MAX];
		GetSystemDirectoryA(syspath, PATH_MAX);
		cstrappend(syspath, "\\dinput8.dll");
		HMODULE hMod = LoadLibraryA(syspath);
		if (hMod) {
			fpDirectInput8Create = (DIRECTINPUT8CREATE)GetProcAddress(hMod, "DirectInput8Create");
			INFO("fpDirectInput8Create: %p", (void*)fpDirectInput8Create);
		}
		else { 
			MessageBoxA(NULL, "Failed to load original dinput8.dll", "", 0);
			return E_OUTOFMEMORY; //=DIERR_OUTOFMEMORY //Lets at least not crash
		}
	}
	return fpDirectInput8Create(hinst, dwVersion, riidltf, ppvOut, punkOuter);
}

/* TODO
	Ingame GUI...?
	Lots of fun things to add.
*/

BOOL APIENTRY DllMain(HMODULE hModule, DWORD reason, LPVOID lpReserved) {
	switch (reason) {
	case DLL_PROCESS_ATTACH: {
		INFO("Randomiser Starting");
		if (hookGameFunctions()) {
			loadConfig();
			initialiseStrategies();
			INFO("Everything loaded!");
		}
		else {
			ERR("Failed to start the randomiser");
		}
    	return TRUE;
	} break;
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
    	return TRUE;
    } //switch

    return TRUE;
}