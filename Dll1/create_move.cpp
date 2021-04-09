#include "hooks.h"
#include "cmd.h"
#include "entity.h"
#include "globals.h"
#include "interfaces.h"
#include "misc.h"
#include "engine_prediction.h"
#include "math.h"
#include "visuals.h"
#include "hvh.h"

bool __fastcall hooks::hooked_create_move(void* ecx, void* edx, float input_sample_frametime, c_usercmd* cmd) {
	static auto create_move_original = hooks::create_move_hook.get_original<create_move_fn>();

	if (!cmd || !cmd->command_number)
		return create_move_original(ecx, input_sample_frametime, cmd);

	uintptr_t* frame_ptr;
	__asm mov frame_ptr, ebp;

	g->send_packet = *(bool*)(*frame_ptr - 0x1C);

	g->cmd = cmd;
	g->local = interfaces->entity_list->get_client_entity(interfaces->engine->get_local_player());
	g->weapon = g->local->get_weapon();

	g->cmd->buttons |= in_bullrush;

	g->in_createmove = false;
	int r = math::random_int(1, 3);

	if (interfaces->engine->is_connected() && interfaces->engine->is_in_game()) {
		g->in_createmove = true;

		misc->rebuild_mouse_delta();
		misc->bunnyhop();
		misc->auto_strafe();
		misc->slide_walk(g->v.misc.meme_walk && r != 2);
		misc->fake_lag();
		misc->fake_duck();

		if (g->local && g->local->is_alive()) {
			engine_prediction->prediction_data.stage = stage_t::start;
			engine_prediction->init();
			engine_prediction->start();
			{
				hvh::antiaim->antiaim_main();
				hvh::aimbot->aimbot_main();
			}
			engine_prediction->finish();

			misc->fix_movement();

			auto& correct = g->data.emplace_front();

			correct.command_number = g->cmd->command_number;
			correct.choked_commands = interfaces->client_state->iChokedCommands + 1;
			correct.tickcount = interfaces->global_vars->tickcount;

			if (g->send_packet)
				g->choked_number.clear();
			else
				g->choked_number.emplace_back(correct.command_number);

			while (g->data.size() > (int)(2.0f / interfaces->global_vars->intervalpertick))
				g->data.pop_back();

			auto& out = g->commands.emplace_back();

			out.is_outgoing = g->send_packet;
			out.is_used = false;
			out.command_number = g->cmd->command_number;
			out.previous_command_number = 0;

			while (g->commands.size() > (int)(1.0f / interfaces->global_vars->intervalpertick))
				g->commands.pop_front();

			if (!g->send_packet)
			{
				auto net_channel = interfaces->client_state->pNetChannel;

				if (net_channel->m_nChokedPackets > 0 && !(net_channel->m_nChokedPackets % 4))
				{
					auto backup_choke = net_channel->m_nChokedPackets;
					net_channel->m_nChokedPackets = 0;

					net_channel->send_datagram();
					--net_channel->m_nOutSequenceNr;

					net_channel->m_nChokedPackets = backup_choke;
				}
			}

			if (g->send_packet)
				g->fake_ang = g->cmd->viewangles;
			else
				g->real_ang = g->cmd->viewangles;

			g->cmd->viewangles.clamp();
			misc->do_animfix();
		}
	}

	cmd = g->cmd;
	*(bool*)(*frame_ptr - 0x1C) = g->send_packet;
	g->in_createmove = false;

	return false;
}