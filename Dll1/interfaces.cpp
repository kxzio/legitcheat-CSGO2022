#include "interfaces.h"

c_interfaces* interfaces = new c_interfaces();

void c_interfaces::init()
{
	if (!panel)
		panel = get_interfaces<c_panel>("vgui2.dll", "VGUI_Panel009");

	if (!client)
		client = get_interfaces<c_base_client_dll>("client.dll", "VClient018");

	if (!client_mode)
		client_mode = **reinterpret_cast<c_client_mode***>((*reinterpret_cast<uintptr_t**>(client))[10] + 5);

	if (!cvar)
		cvar = get_interfaces<i_cvar>("vstdlib.dll", "VEngineCvar007");

	if (!entity_list)
		entity_list = get_interfaces<c_entity_list>("client.dll", "VClientEntityList003");

	if (!engine)
		engine = get_interfaces<c_engine>("engine.dll", "VEngineClient014");

	if (!surface_props)
		surface_props = get_interfaces<IPhysicsSurfaceProps>("vphysics.dll", "VPhysicsSurfaceProps001");

	if (!debug_overlay)
		debug_overlay = get_interfaces<c_debug_overlay>("engine.dll", "VDebugOverlay004");

	if (!model_render)
		model_render = get_interfaces<c_model_render>("engine.dll", "VEngineModel016");

	if (!material_system)
		material_system = get_interfaces<c_material_system>("materialsystem.dll", "VMaterialSystem080");

	if (!client_state)
		client_state = **reinterpret_cast<c_client_state***> ((*reinterpret_cast<uintptr_t**>(engine))[12] + 0x10);

	if (!prediction)
		prediction = get_interfaces<c_prediction>("client.dll", "VClientPrediction001");

	if (!game_movement)
		game_movement = get_interfaces<c_game_movement>("client.dll", "GameMovement001");

	if (!move_helper)
		move_helper = **reinterpret_cast<c_move_helper***>(utils->find_pattern("client.dll", "8B 0D ? ? ? ? 8B 45 ? 51 8B D4 89 02 8B 01") + 2);

	if (!global_vars)
		global_vars = **reinterpret_cast<c_global_vars_base***>((*reinterpret_cast<DWORD**>(client))[0] + 0x1F);

	if (!input)
		input = *reinterpret_cast<c_input**>(utils->find_pattern("client.dll", "B9 ? ? ? ? F3 0F 11 04 24 FF 50 10") + 1);

	if (!surface)
		surface = get_interfaces<c_surface>("vguimatsurface.dll", "VGUI_Surface031");

	if (!model_info)
		model_info = get_interfaces<c_model_info>("engine.dll", "VModelInfoClient004");

	if (!engine_trace)
		engine_trace = get_interfaces<trace>("engine.dll", "EngineTraceClient004");

	if (!engine_sound)
		engine_sound = get_interfaces<c_engine_sound>("engine.dll", "IEngineSoundClient003");

	if (!glow_manager)
		glow_manager = *reinterpret_cast<c_glow_manager**>(utils->find_pattern("client.dll", "0F 11 05 ? ? ? ? 83 C8 01") + 0x3);

	if (!event_manager)
		event_manager = get_interfaces<c_event_manager>("engine.dll", "GAMEEVENTSMANAGER002");


}