#include "hooks.h"
#include "globals.h"
#include "entity.h"
#include "misc.h"
#include "interfaces.h"

void __fastcall hooks::hooked_run_command(void* ecx, void* edx, c_entity* player, c_usercmd* m_pcmd, c_move_helper* move_helper)
{
	static auto run_command_original = hooks::run_command_hook.get_original<run_command_fn>();

	if (!player || !g->local || player != g->local)
		return run_command_original(ecx, player, m_pcmd, move_helper);

	if (g->local->is_alive()) {

		auto backup_velocity_modifier = player->get_velocity_modifier();

		player->get_velocity_modifier() = g->last_velocity_modifier;
		run_command_original(ecx, player, m_pcmd, move_helper);

		if (!g->in_createmove)
			player->get_velocity_modifier() = backup_velocity_modifier;

		misc->record_viewmodel_values();
	}
	else
		run_command_original(ecx, player, m_pcmd, move_helper);
}

bool __fastcall hooks::hooked_in_prediction(void* ecx, void* edx)
{
	static auto in_prediction_original = hooks::prediction_hook.get_original<in_prediction_fn>();
	return in_prediction_original(ecx);
}