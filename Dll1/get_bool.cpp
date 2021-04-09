#include "hooks.h"
#include "interfaces.h"

bool __fastcall hooks::hooked_sv_cheats_get_bool(void* ecx, void* edx)
{
	static auto sv_cheats_get_bool_original = hooks::sv_cheats_hook.get_original<sv_cheats_get_bool_fn>();
	return sv_cheats_get_bool_original(ecx);
}