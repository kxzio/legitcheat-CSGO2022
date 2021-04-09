#include "hooks.h"
#include "interfaces.h"
#include "entity.h"

int __fastcall hooks::hooked_list_leaves_in_box(void* ecx, void* edx, vec3& mins, vec3& maxs, unsigned short* list, int list_max)
{
	static auto list_leaves_in_box_original = hooks::list_leaves_in_box_hook.get_original<list_leaves_in_box_fn>();

	if (*(uint32_t*)_ReturnAddress() != 0x14244489)
		return list_leaves_in_box_original(ecx, mins, maxs, list, list_max);

	auto info = *(RenderableInfo_t**)((uintptr_t)_AddressOfReturnAddress() + 0x14);

	if (!info)
		return list_leaves_in_box_original(ecx, mins, maxs, list, list_max);

	if (!info->m_pRenderable)
		return list_leaves_in_box_original(ecx, mins, maxs, list, list_max);

	auto e = info->m_pRenderable->GetIClientUnknown()->GetBaseEntity();

	if (!e || !e->is_player())
		return list_leaves_in_box_original(ecx, mins, maxs, list, list_max);

	info->m_Flags &= ~0x100;
	info->m_Flags2 |= 0xC0;

	return list_leaves_in_box_original(ecx, vec3(-32768.0f, -32768.0f, -32768.0f), vec3(32768.0f, 32768.0f, 32768.0f), list, list_max);
}