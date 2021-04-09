#include "hooks.h"
#include "globals.h"
#include "interfaces.h"

bool __fastcall hooks::hooked_write_user_cmd_delta_to_buffer(void* ecx, void* edx, int slot, bf_write* buf, int from, int to, bool is_new_command)
{
	static auto write_user_cmd_delta_to_buffer_original = hooks::write_user_hook.get_original<write_user_cmd_delta_to_buffer_fn>();
	return write_user_cmd_delta_to_buffer_original(ecx, slot, buf, from, to, is_new_command);
}