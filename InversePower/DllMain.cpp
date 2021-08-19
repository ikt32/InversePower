#include "script.h"
#include "Constants.hpp"
#include "Util/Logger.hpp"
#include <inc/main.h>

BOOL APIENTRY DllMain(HMODULE hInstance, DWORD reason, LPVOID lpReserved) {
    Log::SetFile(".\\InversePower.log");
    switch (reason) {
    case DLL_PROCESS_ATTACH:
        Log::Clear();
        Log::Write(INFO, "InversePower {}", Constants::DisplayVersion);
        scriptRegister(hInstance, ScriptMain);
        break;
    case DLL_PROCESS_DETACH:
        scriptUnregister(hInstance);
        break;
    }
    return TRUE;
}
