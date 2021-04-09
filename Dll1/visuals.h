#pragma once
#include "vector3.h"
#include "entity.h"
#include <map>
#include <array>

class c_visuals
{
private:
	struct box_t
	{
		int x, y, w, h;
		box_t() {
			x = -1;
			y = -1;
			w = -1;
			h = -1;
		}
	};

	bool get_box(c_entity* pPlayer, box_t& box);
	void on_player(c_entity* entity);
	void on_weapon(); // позже
public:
	struct skeleton_info_t
	{
		ModelRenderInfo_t render_info;
		DrawModelState_t state;

		vec3 parent[128];
		vec3 child[128];
		matrix bones[128];
		matrix cur_bones;
		color clr;
		float time;
		float alpha;

		skeleton_info_t() {
			memset(bones, 0, sizeof(bones));
			clr = color::White;
			time = 0.f;
			alpha = 255.f;
			memset(parent, 0, sizeof(parent));
			memset(child, 0, sizeof(child));
		}
	};

	std::vector< skeleton_info_t* > skeleton_info[65];
	c_visuals() {
		
	}
	void main();
};
extern c_visuals* visuals;