#include "engine_prediction.h"
#include "interfaces.h"
#include "globals.h"
#include "entity.h"
#include "misc.h"
#include "cmd.h"
#include "md5.h"

c_engine_prediction* engine_prediction = new c_engine_prediction();

void c_engine_prediction::init()
{
	if (prediction_data.stage != stage_t::start)
		return;

	prediction_data.curtime = interfaces->global_vars->curtime;
	prediction_data.frametime = interfaces->global_vars->frametime;

	interfaces->global_vars->curtime = ticks_to_time(g->local->get_tickbase());
	interfaces->global_vars->frametime = interfaces->prediction->EnginePaused ? 0.0f : interfaces->global_vars->intervalpertick;

	prediction_data.stage = stage_t::work;
}

void c_engine_prediction::start()
{
	if (prediction_data.stage != stage_t::work)
		return;

	if (interfaces->client_state->iDeltaTick > 0)
		interfaces->prediction->update(interfaces->client_state->iDeltaTick,
			true, interfaces->client_state->nLastCommandAck, 
			interfaces->client_state->nLastOutgoingCommand + interfaces->client_state->iChokedCommands);

	if (!prediction_data.prediction_random_seed)
		prediction_data.prediction_random_seed = *reinterpret_cast <int**>(utils->find_pattern("client.dll", "A3 ? ? ? ? 66 0F 6E 86") + 0x1);

	*prediction_data.prediction_random_seed = MD5_PseudoRandom(g->cmd->command_number) & INT_MAX;

	if (!prediction_data.prediction_player)
		prediction_data.prediction_player = *reinterpret_cast <int**> (utils->find_pattern("client.dll", "89 35 ? ? ? ? F3 0F 10 48") + 0x2);

	*prediction_data.prediction_player = reinterpret_cast <int> (g->local);

	interfaces->game_movement->start_track_prediction_errors(g->local);
	interfaces->move_helper->set_host(g->local);

	c_move_data move_data;
	memset(&move_data, 0, sizeof(c_move_data));

	interfaces->prediction->setup_move(g->local, g->cmd, interfaces->move_helper, &move_data);
	interfaces->game_movement->process_movement(g->local, &move_data);
	interfaces->prediction->finish_move(g->local, g->cmd, &move_data);

	interfaces->game_movement->finish_track_prediction_errors(g->local);
	interfaces->move_helper->set_host(nullptr);

	prediction_data.stage = stage_t::finish;
}

void c_engine_prediction::finish()
{
	if (prediction_data.stage != stage_t::finish)
		return;

	*prediction_data.prediction_random_seed = -1;
	*prediction_data.prediction_player = 0;

	interfaces->global_vars->curtime = prediction_data.curtime;
	interfaces->global_vars->frametime = prediction_data.frametime;
}