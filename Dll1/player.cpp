#include "hooks.h"
#include "cmd.h"
#include "globals.h"
#include "interfaces.h"
#include "misc.h"
#include "entity.h"
#include <memory>

void __fastcall hooks::hooked_check_for_sequence_change(
	void* ecx, void* edx, void* hdr, int cur_sequence, bool force_new_sequence, bool interpolate) {
	check_for_sequence_change_original(ecx, hdr, cur_sequence, force_new_sequence, false);
}

void __fastcall hooks::hooked_do_procedural_foot_plant(void* ecx, 
	void* edx, void* boneToWorld, void* pLeftFootChain, void* pRightFootChain, void* pos) 
{
	//do_procedural_foot_plant_original(ecx, boneToWorld, pLeftFootChain, pRightFootChain, pos);
	return;
}

void __fastcall hooks::hooked_do_extra_bone_processing(c_entity* ecx, uint32_t, c_studio_hdr* hdr, vec3* pos, Quaternion* q,
	const matrix& mat, uint8_t* bone_computed, void* context)
{
	do_extra_bone_processing_original(ecx, hdr, pos, q, mat, bone_computed, context);
}

void __fastcall hooks::hooked_standard_blending_rules(c_entity* ecx,
	void* edx, c_studio_hdr* hdr, vec3* pos, Quaternion* q, float curTime, int boneMask)
{
	if (!ecx || !ecx->is_player())
		return standard_blending_rules_original(ecx, hdr, pos, q, curTime, boneMask);

	int old_effects = ecx->get_effects();
	ecx->get_effects() |= 8;
	standard_blending_rules_original(ecx, hdr, pos, q, curTime, boneMask);
	ecx->get_effects() = old_effects;
}

void __fastcall hooks::hooked_modify_eye_pos(c_animstate* ecx, void* edx, vec3& position)
{
	if (!g->local || !g->local->is_alive())
		return modify_eye_pos_original(ecx, position);

	if ((c_entity*)ecx->m_pBaseEntity == g->local)
		return;
	else
		modify_eye_pos_original(ecx, position);
}

void __fastcall hooks::hooked_calc_viewmodel_bob(c_entity* ecx, void* edx, vec3& position)
{
	if (g->local) {
		if (ecx != g->local || !g->local->is_alive())
			calc_viewmodel_bob_original(ecx, position);
	}
	else
		calc_viewmodel_bob_original(ecx, position);
}

void __fastcall hooks::hooked_update_clientside_animation(c_entity* ecx, void* edx)
{
	if (!ecx || !g->local || !g->local->is_alive())
	{
		ecx->get_clientside_animation() = true;
		update_clientside_animation_original(ecx);
		return;
	}

	if (ecx == g->local)
	{
		if (!g->should_update) {
			std::array<float, 24> poses = g->local->get_pose_parameter();

			g->local->get_pose_parameter() = anim_info.poses;
			g->local->set_abs_angles(vec3(0.f, anim_info.gfy, 0.f));

			auto backup_value = *(uint8_t*)((uintptr_t)g->local + 0x274);
			*(uint8_t*)((uintptr_t)g->local + 0x274) = 0;
			g->local->setup_bones(nullptr, -1, 0x7FF00, interfaces->global_vars->curtime);
			*(uint8_t*)((uintptr_t)g->local + 0x274) = backup_value;

			g->local->get_pose_parameter() = poses;
		}
		else
			update_clientside_animation_original(ecx);
	}
	else
		update_clientside_animation_original(ecx);
}