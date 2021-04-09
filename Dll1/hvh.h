#pragma once
#include "interfaces.h"
#include "globals.h"
#include "hooks.h"
#include "entity.h"
#include "studio.h"

namespace hvh
{
	class c_antiaim
	{
	private:
		void pitch(bool is_legit_aa);
		void yaw(bool is_legit_aa);
	public:
		void antiaim_main();
	};
	extern c_antiaim* antiaim;

	class aim_vector {
	public:
		vec3 vec;
		int hitbox;
		float damage;
		aim_vector(vec3 p, int hitbox_, float damage_ = 0.0f)
		{
			vec = p; hitbox = hitbox_; damage = damage_;
		}
	};

	class c_aimbot
	{
	private:
		bool hitchance(vec3& angle, c_entity* ent);
		vec3 get_point(c_entity* target);
		void fire();
		bool ready_to_fire();
		bool is_visible(c_entity* e, vec3 p);
		void add_multipoints(int hitbox, matrix bones[128], std::vector< aim_vector >& points, c_entity* target);

	public:
		void aimbot_main();

	};
	extern c_aimbot* aimbot;

	class aim_record
	{
	public :
		bool have_to_shot_body;
		int  scan_mode;
		int hitbox;
		float estimated_damage;
		int hitboxes_to_scan_count;
		bool bot;
		bool is_cheater;
	};

	class aim_info 
	{
		public :
			aim_record get_record(c_entity* e);

		private:
			aim_record record_manifold[65];
	};
	extern aim_info* aimbot_data;

	struct FireBulletData
	{
		FireBulletData(const vec3& eye_pos)
			: src(eye_pos)
		{
		}

		vec3 src;
		trace_t enter_trace;
		vec3 direction;
		trace_filter filter;
		float trace_length;
		float trace_length_remaining;
		float current_damage;
		int penetrate_count;
	};

	class c_penetration
	{
	private:


	public:
		float can_hit(const vec3& point);

	};
	extern c_penetration* penetration;
}