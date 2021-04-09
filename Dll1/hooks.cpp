#include "hooks.h"
#include "utils.h"
#include "globals.h"
#include "interfaces.h"
#include "event_manager.h"
#include <detours.h>
#include "detour/detours.h"

detour_hook hooks::reset_hook;
detour_hook hooks::present_hook;
detour_hook hooks::paint_traverse_hook;
detour_hook hooks::create_move_hook;
detour_hook hooks::frame_stage_hook;
detour_hook hooks::draw_model_execute_hook;
detour_hook hooks::packet_start_hook;
detour_hook hooks::packet_end_hook;
detour_hook hooks::write_user_hook;
detour_hook hooks::override_view_hook;
detour_hook hooks::get_viewmodel_hook;
detour_hook hooks::post_screen_effects_hook;
detour_hook hooks::run_command_hook;
detour_hook hooks::prediction_hook;
detour_hook hooks::sv_cheats_hook;
detour_hook hooks::list_leaves_in_box_hook;

do_procedural_foot_plant_fn hooks::do_procedural_foot_plant_original;
modify_eye_pos_fn hooks::modify_eye_pos_original;
calc_viewmodel_bob_fn hooks::calc_viewmodel_bob_original;
standard_blending_rules_fn hooks::standard_blending_rules_original;
update_clientside_animation_fn hooks::update_clientside_animation_original;
do_extra_bone_processing_fn hooks::do_extra_bone_processing_original;
check_for_sequence_change_fn hooks::check_for_sequence_change_original;

bool hooks::is_hooked()
{
	if (MH_Initialize() != MH_OK)
		return false;

	static uintptr_t direct_device = NULL;
	if (!direct_device)
		direct_device = **reinterpret_cast<uintptr_t**>(utils->find_pattern("shaderapidx9.dll", "A1 ? ? ? ? 50 8B 08 FF 51 0C") + 1);

	static DWORD* do_procedural_foot_plant = nullptr;
	if (!do_procedural_foot_plant)
		do_procedural_foot_plant = reinterpret_cast<DWORD*>(utils->find_pattern("client.dll", "55 8B EC 83 E4 F0 83 EC 78 56 8B F1 57 8B 56 60"));

	static DWORD* modify_eye_pos = nullptr;
	if (!modify_eye_pos)
		modify_eye_pos = reinterpret_cast<DWORD*>(utils->find_pattern("client.dll", "55 8B EC 83 E4 F8 83 EC 5C 53 8B D9 56 57 83"));

	static DWORD* calc_viewmodel_bob = nullptr;
	if (!calc_viewmodel_bob)
		calc_viewmodel_bob = reinterpret_cast<DWORD*>(utils->find_pattern("client.dll", "55 8B EC A1 ? ? ? ? 83 EC 10 56 8B F1 B9"));

	static DWORD* standard_blending_rules = nullptr;
	if (!standard_blending_rules)
		standard_blending_rules = reinterpret_cast<DWORD*>(utils->find_pattern("client.dll", "55 8B EC 83 E4 F0 B8 ? ? ? ? E8 ? ? ? ? 56 8B 75 08 57 8B F9 85 F6"));

	static DWORD* update_clientside_animation = nullptr;
	if (!update_clientside_animation)
		update_clientside_animation = reinterpret_cast<DWORD*>(utils->find_pattern("client.dll", "55 8B EC 51 56 8B F1 80 BE ? ? 00 00 00 74 36 8B 06 FF 90 ? ? 00 00"));

	static DWORD* do_extra_bone_processing = nullptr;
	if (!do_extra_bone_processing)
		do_extra_bone_processing = reinterpret_cast<DWORD*>(utils->find_pattern("client.dll", "55 8B EC 83 E4 F8 81 EC ? ? ? ? 53 56 8B F1 57 89 74 24 1C"));

	static DWORD* check_for_sequence_change = nullptr;
	if (!check_for_sequence_change)
		check_for_sequence_change = reinterpret_cast<DWORD*>(utils->find_pattern("client.dll", "55 8B EC 51 53 8B 5D 08 56 8B F1 57 85"));

	static c_cvar* sv_cheats = nullptr;
	if (!sv_cheats)
		sv_cheats = interfaces->cvar->find_var("sv_cheats");

	calc_viewmodel_bob_original = 
		reinterpret_cast<calc_viewmodel_bob_fn>(DetourFunction(
			reinterpret_cast<PBYTE>(calc_viewmodel_bob), reinterpret_cast<PBYTE>(hooked_calc_viewmodel_bob)));

	do_procedural_foot_plant_original = 
		reinterpret_cast<do_procedural_foot_plant_fn>(DetourFunction(
			reinterpret_cast<PBYTE>(do_procedural_foot_plant), reinterpret_cast<PBYTE>(hooked_do_procedural_foot_plant)));

	modify_eye_pos_original = 
		reinterpret_cast<modify_eye_pos_fn>(DetourFunction(
			reinterpret_cast<PBYTE>(modify_eye_pos), reinterpret_cast<PBYTE>(hooked_modify_eye_pos)));

	standard_blending_rules_original =
		reinterpret_cast<standard_blending_rules_fn>(DetourFunction(
			reinterpret_cast<PBYTE>(standard_blending_rules), reinterpret_cast<PBYTE>(hooked_standard_blending_rules)));

	update_clientside_animation_original =
		reinterpret_cast<update_clientside_animation_fn>(DetourFunction(
			reinterpret_cast<PBYTE>(update_clientside_animation), reinterpret_cast<PBYTE>(hooked_update_clientside_animation)));

	do_extra_bone_processing_original =
		reinterpret_cast<do_extra_bone_processing_fn>(DetourFunction(
			reinterpret_cast<PBYTE>(do_extra_bone_processing), reinterpret_cast<PBYTE>(hooked_do_extra_bone_processing)));

	check_for_sequence_change_original =
		reinterpret_cast<check_for_sequence_change_fn>(DetourFunction(
			reinterpret_cast<PBYTE>(check_for_sequence_change), reinterpret_cast<PBYTE>(hooked_check_for_sequence_change)));

	if (!check_for_sequence_change_original)
		return false;

	if (!do_extra_bone_processing_original)
		return false;

	if (!standard_blending_rules_original)
		return false;

	if (!calc_viewmodel_bob_original)
		return false;

	if (!do_procedural_foot_plant_original)
		return false;

	if (!modify_eye_pos_original)
		return false;

	if (!hooks::reset_hook.create(getvfunc(reinterpret_cast<void*>(direct_device), 16), &hooked_reset))
		return false;

	if (!hooks::present_hook.create(getvfunc(reinterpret_cast<void*>(direct_device), 17), &hooked_present))
		return false;

	if (!hooks::paint_traverse_hook.create(getvfunc(interfaces->panel, 41), &hooked_paint_traverse))
		return false;

	if (!hooks::create_move_hook.create(getvfunc(interfaces->client_mode, 24), &hooked_create_move))
		return false;

	if (!hooks::frame_stage_hook.create(getvfunc(interfaces->client, 37), &hooked_frame_stage_notify))
		return false;

	if (!hooks::draw_model_execute_hook.create(getvfunc(interfaces->model_render, 21), &hooked_draw_model_execute))
		return false;

	if (!hooks::packet_start_hook.create(getvfunc(
		reinterpret_cast<c_client_state*>(reinterpret_cast<uint32_t>(interfaces->client_state) + 0x8), 5), &hooked_packet_start))
		return false;

	if (!hooks::packet_end_hook.create(getvfunc(
		reinterpret_cast<c_client_state*>(reinterpret_cast<uint32_t>(interfaces->client_state) + 0x8), 6), &hooked_packet_end))
		return false;

	if (!hooks::write_user_hook.create(getvfunc(interfaces->client, 24), &hooked_write_user_cmd_delta_to_buffer))
		return false;

	if (!hooks::override_view_hook.create(getvfunc(interfaces->client_mode, 18), &hooked_override_view))
		return false;

	if (!hooks::get_viewmodel_hook.create(getvfunc(interfaces->client_mode, 35), &hooked_get_viewmodel_fov))
		return false;

	if (!hooks::post_screen_effects_hook.create(getvfunc(interfaces->client_mode, 44), &hooked_do_post_screen_effects))
		return false;

	if (!hooks::run_command_hook.create(getvfunc(interfaces->prediction, 19), &hooked_run_command))
		return false;

	if (!hooks::prediction_hook.create(getvfunc(interfaces->prediction, 14), &hooked_in_prediction))
		return false;

	if (!hooks::sv_cheats_hook.create(getvfunc(sv_cheats, 13), &hooked_sv_cheats_get_bool))
		return false;

	if (!hooks::list_leaves_in_box_hook.create(getvfunc(interfaces->engine->get_bsp_tree_query(), 6), &hooked_list_leaves_in_box))
		return false;

	return true;
}

void hooks::init()
{
	if (is_hooked()) {
		if (g->window)
			g->old_window = reinterpret_cast<WNDPROC>(SetWindowLongPtr(g->window, GWL_WNDPROC, reinterpret_cast<LONG_PTR>(hooked_wnd_proc)));
	}
	else exit(0);
}

void hooks::unload()
{
	reset_hook.remove();
	present_hook.remove();
	paint_traverse_hook.remove();
	frame_stage_hook.remove();
	create_move_hook.remove();
	draw_model_execute_hook.remove();
	packet_start_hook.remove();
	packet_end_hook.remove();
	write_user_hook.remove();
	override_view_hook.remove();
	get_viewmodel_hook.remove();
	post_screen_effects_hook.remove();
	run_command_hook.remove();
	prediction_hook.remove();
	sv_cheats_hook.remove();
	list_leaves_in_box_hook.remove();

	MH_Uninitialize();

	DetourRemove(reinterpret_cast<PBYTE>(do_extra_bone_processing_original), reinterpret_cast<PBYTE>(hooked_do_extra_bone_processing));
	DetourRemove(reinterpret_cast<PBYTE>(do_procedural_foot_plant_original), reinterpret_cast<PBYTE>(hooked_do_procedural_foot_plant));
	DetourRemove(reinterpret_cast<PBYTE>(modify_eye_pos_original), reinterpret_cast<PBYTE>(hooked_modify_eye_pos));
	DetourRemove(reinterpret_cast<PBYTE>(calc_viewmodel_bob_original), reinterpret_cast<PBYTE>(hooked_calc_viewmodel_bob));
	DetourRemove(reinterpret_cast<PBYTE>(standard_blending_rules_original), reinterpret_cast<PBYTE>(hooked_standard_blending_rules));
	DetourRemove(reinterpret_cast<PBYTE>(update_clientside_animation_original), reinterpret_cast<PBYTE>(hooked_update_clientside_animation));
	DetourRemove(reinterpret_cast<PBYTE>(check_for_sequence_change_original), reinterpret_cast<PBYTE>(hooked_check_for_sequence_change));

	SetWindowLongPtr(g->window, GWL_WNDPROC, reinterpret_cast<LONG_PTR>(g->old_window));

	for (auto listener : event_manager_list)
		interfaces->event_manager->RemoveListener(listener);
}
