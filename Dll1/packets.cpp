#include "hooks.h"
#include "globals.h"
#include "entity.h"
#include "interfaces.h"

void __fastcall hooks::hooked_packet_start(void* ecx, void* edx, int incoming, int outgoing)
{
	static auto packet_start_original = hooks::packet_start_hook.get_original<packet_start_fn>();

    if (!g->local || !g->local->is_alive())
        packet_start_original(ecx, incoming, outgoing);

    if (g->commands.empty())
        packet_start_original(ecx, incoming, outgoing);

    for (auto it = g->commands.rbegin(); it != g->commands.rend(); ++it)
    {
        if (!it->is_outgoing)
            continue;

        if (it->command_number == outgoing || outgoing > it->command_number && (!it->is_used || it->previous_command_number == outgoing))
        {
            it->previous_command_number = outgoing;
            it->is_used = true;
            packet_start_original(ecx, incoming, outgoing);
            break;
        }
    }

    auto result = false;

    for (auto it = g->commands.begin(); it != g->commands.end();)
    {
        if (outgoing == it->command_number || outgoing == it->previous_command_number)
            result = true;

        if (outgoing > it->command_number && outgoing > it->previous_command_number)
            it = g->commands.erase(it);
        else
            ++it;
    }

    if (!result)
        packet_start_original(ecx, incoming, outgoing);
}

void __fastcall hooks::hooked_packet_end(void* ecx, void* edx)
{
	static auto packet_end_original = hooks::packet_end_hook.get_original<packet_end_fn>();

	if (!g->local || !g->local->is_alive())
	{
		g->data.clear();
		return packet_end_original(ecx);
	}

	if (*(int*)((uintptr_t)ecx + 0x164) == *(int*)((uintptr_t)ecx + 0x16C))
	{
		auto ack_cmd = *(int*)((uintptr_t)ecx + 0x4D2C);
		auto correct = std::find_if(g->data.begin(), g->data.end(),
			[&ack_cmd](const correction_data& other_data)
			{
				return other_data.command_number == ack_cmd;
			}
		);

		auto netchannel = interfaces->engine->get_net_channel_info();

		if (netchannel && correct != g->data.end())
		{
			if (g->last_velocity_modifier > g->local->get_velocity_modifier() + 0.1f)
			{
				auto weapon = g->weapon;

				if (!weapon || weapon->get_item_definition_index() != weapon_revolver && !weapon->is_grenade())
				{
					for (auto& number : g->choked_number)
					{
						auto cmd = &interfaces->input->m_pCommands[number % MULTIPLAYER_BACKUP];
						auto verified = &interfaces->input->m_pVerifiedCommands[number % MULTIPLAYER_BACKUP];

						if (cmd->buttons & (in_attack | in_attack2))
						{
							cmd->buttons &= ~in_attack;

							verified->m_cmd = *cmd;
							verified->m_crc = cmd->get_checksum();
						}
					}
				}
			}

			g->last_velocity_modifier = g->local->get_velocity_modifier();
		}
	}

	packet_end_original(ecx);
}