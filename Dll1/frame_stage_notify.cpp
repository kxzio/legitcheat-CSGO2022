#include "hooks.h"
#include "misc.h"
#include "interfaces.h"
#include "visuals.h"
#include "netvar_manager.h"

void __fastcall hooks::hooked_frame_stage_notify(void* ecx, void* edx, frame_stage stage)
{
	static auto frame_stage_notify_original = hooks::frame_stage_hook.get_original<frame_stage_notify_fn>();

	if (interfaces->engine->is_connected() && interfaces->engine->is_in_game()) {

		if (stage == frame_render_start) {

			for (int i = 0; i < 4; i++)
				for (int j = 0; j < 4; j++)
					g->view_matrix[i][j] = interfaces->engine->world_to_screen_matrix()[i][j];

			if (g->local) {
				g->can_connect = true;
				if (g->local->is_alive())
				{
					g->is_local_alive = true;
					g->is_local_scoped = g->local->is_scoped();
				}
				else
					g->is_local_alive = false;
			}

			if (g->local && g->local->is_alive()) {
				if (interfaces->input->m_fCameraInThirdPerson)
					*reinterpret_cast<vec3*>(reinterpret_cast<uintptr_t>(g->local) + offsets::deadflag + 4) = g->fake_ang;

				static c_cvar* r_jiggle_bones = interfaces->cvar->find_var("r_jiggle_bones");
				r_jiggle_bones->m_fnChangeCallbacks.m_Size = 0;
				r_jiggle_bones->set_value(0);

				if (g->local->get_flags() & fl_onground) {
					g->local->get_animstate()->m_bOnGround = true;
					g->local->get_animstate()->m_bInHitGroundAnimation = false;
				}
			}
		}

		misc->set_viewmodel_values(stage);
	}
	else g->can_connect = false;

	frame_stage_notify_original(ecx, stage);
}