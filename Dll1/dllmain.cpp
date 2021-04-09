#include <Windows.h>
#include <thread>
#include <chrono>
#include "globals.h"
#include "utils.h"
#include "hooks.h"
#include "interfaces.h"
#include "render.h"
#include "config.h"
#include "netvar_manager.h"
#include "event_manager.h"

using namespace std::literals::chrono_literals;

#define sleep(ms) std::this_thread::sleep_for(ms)

void init_cheat()
{
    while (!(g->window = FindWindowA("Valve001", NULL)))
        sleep(200ms);

    while (!GetModuleHandleA("serverbrowser.dll"))
        sleep(200ms);

    config.setup_elements();
    interfaces->init();
    netvar_manager->init();
    netvar_manager->parse_offsets();
    event_override->register_self();
    hooks::init();

    while (!g->unload) {
        sleep(1s);
    }

    hooks::unload();
    FreeLibraryAndExitThread(g->module, 0);
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        DisableThreadLibraryCalls(hModule);
        CreateThread(0, 0, (LPTHREAD_START_ROUTINE)init_cheat, 0, 0, 0);
        g->module = hModule;
        break;
    }
    return TRUE;
}

