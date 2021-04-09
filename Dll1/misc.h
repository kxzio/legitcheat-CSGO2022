#pragma once
#include <iostream>
#include <memory>
#include <array>
#include "client.h"
#include "entity.h"

class c_weapon;

struct anim_info_t
{
	float gfy = -1;
	c_animlayer layers[15];
	std::array<float, 24> poses;
};
extern anim_info_t anim_info;

struct viewmodel_info
{
	c_weapon* weapon = nullptr;

	int viewmodel_index = 0;
	int sequence = 0;
	int animation_parity = 0;

	float cycle = 0.0f;
	float animation_time = 0.0f;
};

extern viewmodel_info* viewmodel_data;

class c_misc
{
public:
	void rebuild_mouse_delta();
	void bunnyhop();
	void auto_strafe();
	void slide_walk(bool extra);
	void fake_lag();
	void fake_duck();
	void record_viewmodel_values();
	void set_viewmodel_values(frame_stage stage);
	void do_animfix();
	void fix_movement();
	void do_menu();
};

extern c_misc* misc;