// Generic Includes
#include <windows.h>
#include <stdio.h>
#include <time.h>

// Resources
#include "../resources/Repository.c"

// Useful Utilities
//#define DEBUG
#include "utils.h"

// Types
#include "types.h"

// Globals
#include "globals.h"

// Config loader
#include "config.c"

// Game Engine Types
#include "gameInternalTypes.c"

// Strategy
#include "strategy.c"

// Randomiser
#include "randomiser.c"

// Game internals
#include "gameInternalsHook.c"


typedef HRESULT(__stdcall *DIRECTINPUT8CREATE)(HINSTANCE, DWORD, REFIID, LPVOID*, LPUNKNOWN);
DIRECTINPUT8CREATE fpDirectInput8Create;
__declspec(dllexport)  HRESULT __stdcall DirectInput8Create(HINSTANCE hinst, DWORD dwVersion, REFIID riidltf, LPVOID* ppvOut, LPUNKNOWN punkOuter) {
	if (!fpDirectInput8Create) {
		char syspath[PATH_MAX];
		GetSystemDirectoryA(syspath, PATH_MAX);
		cstrappend(syspath, "\\dinput8.dll");
		HMODULE hMod = LoadLibraryA(syspath);
		if (hMod) {
			fpDirectInput8Create = (DIRECTINPUT8CREATE)GetProcAddress(hMod, "DirectInput8Create");
			INFO("fpDirectInput8Create: 0x%I64x", (uintptr_t)fpDirectInput8Create);
		}
		else { 
			MessageBoxA(NULL, "Failed to load DirectInput dll", "", 0);
			return E_OUTOFMEMORY; //=DIERR_OUTOFMEMORY //Lets at least not crash
		}
	}
	return fpDirectInput8Create(hinst, dwVersion, riidltf, ppvOut, punkOuter);
}

/* TODO
	Update bad python script
	Use Repo file from game
	Add a readme file
	add header protecters
*/

BOOL APIENTRY DllMain(HMODULE hModule, DWORD  reason, LPVOID lpReserved) {
	switch (reason) {
	case DLL_PROCESS_ATTACH:
	{
		INFO("Randomiser Starting");

		loadConfig();

		initialiseStrategies();
		hookGameFunctions();

		SPAM("Everything loaded!");
	} break;
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
    	return TRUE;
    }

    return TRUE;
}