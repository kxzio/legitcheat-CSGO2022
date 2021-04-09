#pragma once
#include <iostream>
#include "panel.h"
#include "client.h"
#include "cvar.h"
#include "entity_list.h"
#include "engine.h"
#include "debug_overlay.h"
#include "model_render.h"
#include "material_system.h"
#include "client_state.h"
#include "prediction.h"
#include "input.h"
#include "surface.h"
#include "events.h"

template< typename T >
T* get_interfaces(const char* strModule, const char* strInterface)
{
	typedef T* (*CreateInterfaceFn)(const char* szName, int iReturn);
	CreateInterfaceFn CreateInterface = (CreateInterfaceFn)(GetProcAddress(GetModuleHandleA(strModule), "CreateInterface"));
	return CreateInterface(strInterface, 0);
}

class c_glow_manager;
class trace;

class c_interfaces
{
public:
	c_panel* panel;
	c_base_client_dll* client;
	c_client_mode* client_mode;
	i_cvar* cvar;
	c_entity_list* entity_list;
	c_engine* engine;
	c_debug_overlay* debug_overlay;
	c_model_render* model_render;
	c_material_system* material_system;
	c_client_state* client_state;
	c_prediction* prediction;
	c_game_movement* game_movement;
	c_move_helper* move_helper;
	c_global_vars_base* global_vars;
	c_input* input;
	c_surface* surface;
	c_model_info* model_info;
	trace* engine_trace;
	c_engine_sound* engine_sound;
	c_glow_manager* glow_manager;
	c_event_manager* event_manager;
	c_mdl_cache* mdl_cache;
	IPhysicsSurfaceProps* surface_props;

	c_interfaces() 
	{
		panel = nullptr;
		client = nullptr;
		client_mode = nullptr;
		cvar = nullptr;
		entity_list = nullptr;
		engine = nullptr;
		debug_overlay = nullptr;
		model_render = nullptr;
		material_system = nullptr;
		client_state = nullptr;
		prediction = nullptr;
		game_movement = nullptr;
		move_helper = nullptr;
		global_vars = nullptr;
		input = nullptr;
		surface = nullptr;
		model_info = nullptr;
		engine_trace = nullptr;
		engine_sound = nullptr;
		glow_manager = nullptr;
		event_manager = nullptr;
		mdl_cache = nullptr;
		surface_props = nullptr;
	}

	void init();
};
extern c_interfaces* interfaces;