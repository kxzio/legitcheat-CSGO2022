#pragma once
#include <Windows.h>
#include <deque>
#include <vector>
#include "imgui/imgui.h"
#include "vector3.h"
#include "render.h"

class c_usercmd;
class c_entity;
class c_weapon;

struct command
{
	bool is_outgoing = false;
	bool is_used = false;
	int previous_command_number = 0;
	int command_number = 0;
};

struct correction_data
{
	int command_number = 0;
	int tickcount = 0;
	int choked_commands = 0;
};

struct globals
{
	struct menu
	{
		struct
		{
			bool enable;
			float fov;
			float silent_fov;
			int hitchance;
			float aim_smooth;
			int silent_chance;

			float hitbox_scale_head;
			float hitbox_scale_body;
			float hitbox_scale_legs;
			bool rcs;
			int rcs_val;
			bool hitscan[10];
			enum hitscan_menu {
				head, neck, lowebody, upperbody, chest, stomach, legs
			};

		}aimbot;
		struct
		{
			bool enable;
			bool anti_aim_on_use;
			int pitch;
			int yaw;
			int desync;
			float left_amount;
			float right_amount;
			ImGuiBind_t* flip_key = new ImGuiBind_t();
			bool jitter;
			float jitter_amount;
			bool freestanding;
		}anti_aim;
		struct
		{
			bool fake_lag;
			int amount;
			ImGuiBind_t* fakeduck = new ImGuiBind_t();
			bool meme_walk;
			bool auto_jump;
			bool air_strafe;
			bool hit_marker;
			float aspect_ratio;
			float fov;
			float viewmodel_fov;
		}misc;
		struct
		{
			bool enable;
			bool remove_scope;
			bool name;
			bool box;
			bool weapon;
			bool health_bar;
			bool skeleton;
			bool flags;

			bool hit_skeleton;
			bool hit_model;

			bool glow_enemy;
			bool glow_team;
			bool glow_local;

			float color_glow_enemy[4];
			float color_glow_team[4];
			float color_glow_local[4];

			float color_box[4];
			float color_name[4];
			float color_weapon[4];
			float color_hp[4];
			float color_skeleton[4];

			bool chams_enemy_visible;
			bool chams_enemy_xqz;

			float color_chams_enemy_visible[4];
			float color_chams_enemy_xqz[4];
		}visuals;
		int cfg_index = 0;
		char cfg_name[256];

		menu()
		{
			memset(cfg_name, 0, sizeof(cfg_name));
		}
	}v;

	struct
	{
		ImFont* menu;
		ImFont* main;
		ImFont* misc;
	}fonts;

	bool can_connect;
	bool is_local_alive;
	bool is_local_scoped;
	bool unload;
	bool init_d3d;
	bool gui_opened;
	bool send_packet;
	bool in_createmove;
	bool fake_ducking;
	bool should_update;

	bool key_pressed[256];
	bool key_down[256];
	int key_pressedticks[256];

	float last_velocity_modifier;

	float view_matrix[4][4];

	WNDPROC old_window;
	HWND window;
	HMODULE module;

	c_usercmd* cmd;
	c_entity* local;
	c_weapon* weapon;

	struct screen_t
	{
		int w, h;
	}screen;

	std::vector <int> choked_number;
	std::deque <command> commands;
	std::deque <correction_data> data;

	vec3 real_ang;
	vec3 fake_ang;

	int last_penetrated_count = 4;
	int last_hitgroup = -1;
	bool force_hitbox_penetration_accuracy = false;

	globals() {
		can_connect = false;
		is_local_alive = false;
		is_local_scoped = false;
		unload = false;
		init_d3d = false;
		fake_ducking = false;
		in_createmove = false;
		send_packet = true;
		should_update = true;
		gui_opened = true;

		last_velocity_modifier = -1.f;

		module = NULL;
		window = NULL;
		old_window = NULL;

		cmd = nullptr;
		local = nullptr;
		weapon = nullptr;

		memset(key_down, 0, sizeof(key_down));
		memset(key_pressed, 0, sizeof(key_pressed));
		memset(key_pressedticks, 0, sizeof(key_pressedticks));
		memset(view_matrix, 0, sizeof(view_matrix));

		commands = {};
		data = {};
		choked_number = {};

		real_ang = vec3();
		fake_ang = vec3();

	}
	~globals() {

	}
};

extern globals* g;