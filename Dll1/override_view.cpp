#include "hooks.h"
#include "globals.h"
#include "entity.h"
#include "interfaces.h"

void __fastcall hooks::hooked_override_view(void* ecx, void* edx, c_view_setup* setup) {
	static auto override_view_original = hooks::override_view_hook.get_original<override_view_fn>();

	if (g->local && g->local->is_alive()) {
		if (g->v.misc.fov > 0.f)
			setup->fov = g->v.misc.fov;

		if (g->fake_ducking)
			setup->origin.z = g->local->GetAbsOrigin().z + interfaces->game_movement->get_player_view_offset(false).z;
	}
		
	override_view_original(ecx, setup);
}

float __stdcall hooks::hooked_get_viewmodel_fov()
{
	static auto get_viewmodel_fov_original = hooks::get_viewmodel_hook.get_original<get_viewmodel_fov_fn>();

	return g->v.misc.viewmodel_fov > 0.f ? g->v.misc.viewmodel_fov : get_viewmodel_fov_original(interfaces->client_mode);
}