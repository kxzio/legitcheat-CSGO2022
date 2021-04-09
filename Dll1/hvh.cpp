#include "hvh.h"
#include "imgui/imgui.h"
#include "engine_prediction.h"
#include "math.h"
#include <thread>

#define SLOBYTE(x)   (*((int8_t*)&(x)))

#define DAMAGE_NO		0
#define DAMAGE_EVENTS_ONLY	1	
#define DAMAGE_YES		2
#define DAMAGE_AIM		3

#define	surf_light		0x0001		// value will hold the light strength
#define	surf_sky2d		0x0002		// don't draw, indicates we should skylight + draw 2d sky but not draw the 3d skybox
#define	surf_sky		0x0004		// don't draw, but add to skybox
#define	surf_warp		0x0008		// turbulent water warp
#define	surf_trans		0x0010
#define surf_noportal	0x0020	// the surface can not have a portal placed on it
#define	surf_trigger	0x0040	// fixme: this is an xbox hack to work around elimination of trigger surfaces, which breaks occluders
#define	surf_nodraw		0x0080	// don't bother referencing the texture
#define	surf_hint		0x0100	// make a primary bsp splitter
#define	surf_skip		0x0200	// completely ignore, allowing non-closed brushes
#define surf_nolight	0x0400	// don't calculate light
#define surf_bumplight	0x0800	// calculate three lightmaps for the surface for bumpmapping
#define surf_noshadows	0x1000	// don't receive shadows
#define surf_nodecals	0x2000	// don't receive decals
#define surf_nopaint	0x2000	// the surface can not have paint placed on it
#define surf_nochop		0x4000	// don't subdivide patches on this surface
#define surf_hitbox		0x8000	// surface is part of a hitbox

#define char_tex_antlion		'a'
#define char_tex_bloodyflesh	'b'
#define	char_tex_concrete		'c'
#define char_tex_dirt			'd'
#define char_tex_eggshell		'e' ///< the egg sacs in the tunnels in ep2.
#define char_tex_flesh			'f'
#define char_tex_grate			'g'
#define char_tex_alienflesh		'h'
#define char_tex_clip			'i'
//#define char_tex_unused		'j'
#define char_tex_snow			'k'
#define char_tex_plastic		'l'
#define char_tex_metal			'm'
#define char_tex_sand			'n'
#define char_tex_foliage		'o'
#define char_tex_computer		'p'
//#define char_tex_unused		'q'
#define char_tex_reflective		'r'
#define char_tex_slosh			's'
#define char_tex_tile			't'
#define char_tex_cardboard		'u'
#define char_tex_vent			'v'
#define char_tex_wood			'w'
//#define char_tex_unused		'x' ///< do not use - "fake" materials use this (ladders, wading, clips, etc)
#define char_tex_glass			'y'
#define char_tex_warpshield		'z' ///< wierd-looking jello effect for advisor shield.

enum contents : uint32_t {

	contents_empty = 0,
	contents_solid = 0x1,
	contents_window = 0x2,
	contents_aux = 0x4,
	contents_grate = 0x8,
	contents_slime = 0x10,
	contents_water = 0x20,
	contents_blocklos = 0x40,
	contents_opaque = 0x80,
	last_visible_contents = contents_opaque,
	all_visible_contents = (last_visible_contents | (last_visible_contents - 1)),
	contents_testfogvolume = 0x100,
	contents_unused = 0x200,
	contents_blocklight = 0x400,
	contents_team1 = 0x800,
	contents_team2 = 0x1000,
	contents_ignore_nodraw_opaque = 0x2000,
	contents_moveable = 0x4000,
	contents_areaportal = 0x8000,
	contents_playerclip = 0x10000,
	contents_monsterclip = 0x20000,
	contents_current_0 = 0x40000,
	contents_current_90 = 0x80000,
	contents_current_180 = 0x100000,
	contents_current_270 = 0x200000,
	contents_current_up = 0x400000,
	contents_current_down = 0x800000,
	contents_origin = 0x1000000,
	contents_monster = 0x2000000,
	contents_debris = 0x4000000,
	contents_detail = 0x8000000,
	contents_translucent = 0x10000000,
	contents_ladder = 0x20000000,
	contents_hitbox = 0x40000000

};

enum mask : uint32_t {
	mask_all = (0xffffffff),
	mask_solid = (contents_solid | contents_moveable | contents_window | contents_monster | contents_grate),
	mask_playersolid = (contents_solid | contents_moveable | contents_playerclip | contents_window | contents_monster | contents_grate),
	mask_npcsolid = (contents_solid | contents_moveable | contents_monsterclip | contents_window | contents_monster | contents_grate),
	mask_npcfluid = (contents_solid | contents_moveable | contents_monsterclip | contents_window | contents_monster),
	mask_water = (contents_water | contents_moveable | contents_slime),
	mask_opaque = (contents_solid | contents_moveable | contents_opaque),
	mask_opaque_and_npcs = (mask_opaque | contents_monster),
	mask_blocklos = (contents_solid | contents_moveable | contents_blocklos),
	mask_blocklos_and_npcs = (mask_blocklos | contents_monster),
	mask_visible = (mask_opaque | contents_ignore_nodraw_opaque),
	mask_visible_and_npcs = (mask_opaque_and_npcs | contents_ignore_nodraw_opaque),
	mask_shot = (contents_solid | contents_moveable | contents_monster | contents_window | contents_debris | contents_hitbox),
	mask_shot_brushonly = (contents_solid | contents_moveable | contents_window | contents_debris),
	mask_shot_hull = (contents_solid | contents_moveable | contents_monster | contents_window | contents_debris | contents_grate),
	mask_shot_portal = (contents_solid | contents_moveable | contents_window | contents_monster),
	mask_solid_brushonly = (contents_solid | contents_moveable | contents_window | contents_grate),
	mask_playersolid_brushonly = (contents_solid | contents_moveable | contents_window | contents_playerclip | contents_grate),
	mask_npcsolid_brushonly = (contents_solid | contents_moveable | contents_window | contents_monsterclip | contents_grate),
	mask_npcworldstatic = (contents_solid | contents_window | contents_monsterclip | contents_grate),
	mask_npcworldstatic_fluid = (contents_solid | contents_window | contents_monsterclip),
	mask_splitareaportal = (contents_water | contents_slime),
	mask_current = (contents_current_0 | contents_current_90 | contents_current_180 | contents_current_270 | contents_current_up | contents_current_down),
	mask_deadsolid = (contents_solid | contents_playerclip | contents_window | contents_grate)
};

enum TextureCharacters {
	CHAR_TEX_ANTLION = 'A',
	CHAR_TEX_BLOODYFLESH = 'B',
	CHAR_TEX_CONCRETE = 'C',
	CHAR_TEX_DIRT = 'D',
	CHAR_TEX_EGGSHELL = 'E',
	CHAR_TEX_FLESH = 'F',
	CHAR_TEX_GRATE = 'G',
	CHAR_TEX_ALIENFLESH = 'H',
	CHAR_TEX_CLIP = 'I',
	CHAR_TEX_PLASTIC = 'L',
	CHAR_TEX_METAL = 'M',
	CHAR_TEX_SAND = 'N',
	CHAR_TEX_FOLIAGE = 'O',
	CHAR_TEX_COMPUTER = 'P',
	CHAR_TEX_SLOSH = 'S',
	CHAR_TEX_TILE = 'T',
	CHAR_TEX_CARDBOARD = 'U',
	CHAR_TEX_VENT = 'V',
	CHAR_TEX_WOOD = 'W',
	CHAR_TEX_GLASS = 'Y',
	CHAR_TEX_WARPSHIELD = 'Z',
};

enum ragebot_hitboxes : int
{
	HITBOX_HEAD,
	HITBOX_NECK,
	HITBOX_PELVIS,
	HITBOX_BODY,
	HITBOX_THORAX,
	HITBOX_CHEST,
	HITBOX_UPPER_CHEST,
	HITBOX_RIGHT_THIGH,
	HITBOX_LEFT_THIGH,
	HITBOX_LEFT_CALF,
	HITBOX_RIGHT_CALF,
	HITBOX_LEFT_FOOT,
	HITBOX_RIGHT_FOOT,
	HITBOX_LEFT_HAND,
	HITBOX_RIGHT_HAND,
	HITBOX_LEFT_UPPER_ARM,
	HITBOX_LEFT_FOREARM,
	HITBOX_RIGHT_UPPER_ARM,
	HITBOX_RIGHT_FOREARM,
	HITBOX_MAX
};

namespace hvh
{

	enum sides_e { iright, ileft, itop, ibottom };

	c_antiaim* antiaim = new c_antiaim();	

	vec3 point = vec3(0, 0, 0);

	vec3 aim_punch = vec3(0, 0, 0);

	vec3 aim_angle = vec3(0, 0, 0);

	void c_antiaim::pitch(bool is_legit_aa) {
		if (is_legit_aa) 
			return;

		switch (g->v.anti_aim.pitch) {
		case 1: // emotion
			g->cmd->viewangles.x = 89.f;
			break;
		case 2: // minimal
			g->cmd->viewangles.x = 89.f - (g->local->get_velocity().length_2d() / 100.f);
			break;
		case 3: // up
			g->cmd->viewangles.x = -89.f;
			break;
		case 4: // zero
			g->cmd->viewangles.x = 0.f;
			break;
		}
	}
	void c_antiaim::yaw(bool is_legit_aa) {
		if (is_legit_aa) 
			g->cmd->viewangles.y += 0.f;
		else {
			switch (g->v.anti_aim.yaw) {
			case 1: // backward
				g->cmd->viewangles.y += 180.f;
				break;
			case 2: // forward
				g->cmd->viewangles.y += 0.f;
				break;
			case 3: {// sideways 
				static bool fl = false;
				g->cmd->viewangles.y += fl ? 90.f : -90.f;
				if (g->send_packet)
					fl = !fl;
			}
			break;
			}

			if (g->v.anti_aim.jitter) {
				static bool fl = false;
				g->cmd->viewangles.y += fl ? g->v.anti_aim.jitter_amount : -g->v.anti_aim.jitter_amount;
				if (g->send_packet)
					fl = !fl;
			}
		}

		bool side_flip = ImGui::IsActiveHotkey(g->v.anti_aim.flip_key);
		float amount =  is_legit_aa ? 30.f : side_flip ? 60.f - g->v.anti_aim.right_amount : 60.f - g->v.anti_aim.left_amount;

		if (g->v.anti_aim.desync > 0.f) {
			if (g->local->get_velocity().length_2d() <= 0.f || std::fabs(g->local->get_velocity().z) <= 100.f)
				g->cmd->sidemove += g->cmd->command_number % 2 ? 1.01f : -1.01f;
		}

		switch (g->v.anti_aim.desync) {
		case 1: // static
		{
			if (!g->send_packet) {
				g->cmd->viewangles.y += amount - 60.f * (side_flip ? 1 : -1);
			}
			else
				g->cmd->viewangles.y += amount * (side_flip ? 1 : -1);
		}
		break;
		case 2: {// spin 

			static int j = 1;
			static int x = 0;

			if (!g->send_packet) {
				g->cmd->viewangles.y += std::clamp(x, -60, 60);
			}

			//https://www.cyberforum.ru/cpp-beginners/thread1518804.html
			// да я настолько тупой, и что?
			if (j) {
				if (x == 60) {
					j = 0;
					x -= 10;
				}
				else x += 10;
			}
			else {
				if (x == -60) {
					j = 1;
					x += 10;
				}
				else x -= 10;
			}
		}
		break;
		}
	}
	void c_antiaim::antiaim_main() {
		if (!g->v.anti_aim.enable)
			return;

		if (g->cmd->buttons & in_attack
			|| g->weapon->is_grenade()
			|| g->weapon->is_knife())
			return;

		bool on_use = g->cmd->buttons & in_use;

		if (g->v.anti_aim.anti_aim_on_use) {
			pitch(on_use);
			yaw(on_use);
		}
		else
		{
			if (g->cmd->buttons & in_use)
				return;

			pitch(false);
			yaw(false);
		}
	}

	c_aimbot* aimbot = new c_aimbot();

	static int t_backup { 0 };
	static int t_since_shot { 0 };
	int time_since_shot()
	{
		t_since_shot = g->local->get_simulation_time() - t_backup;
		if (g->cmd->buttons & in_attack)
		{
			t_backup = g->local->get_simulation_time();
			t_since_shot = 0;
		}
		return t_since_shot;
	}

	void c_aimbot::aimbot_main() {

		if (!g->v.aimbot.enable) return;
			
		if (!g->local->is_alive()) return;

		if (interfaces->engine->is_connected() && interfaces->engine->is_in_game())
			fire();

	}

	enum emode { read, write };

	void do_rcs(vec3& ang)
	{
		auto weapon = g->weapon;

		static vec3 prev_punch = { 0.0f, 0.0f, 0.0f };

		const int id = g->weapon->get_item_definition_index();

		if (id == weapon_awp || id == weapon_ssg08 || id == weapon_deagle ||
			id == weapon_usp_silencer || id == weapon_hkp2000 || 
			id == weapon_fiveseven || id == weapon_glock)
		{
			prev_punch = vec3(0, 0, 0);
			return;
		}

		c_cvar* recoil_cvar = interfaces->cvar->find_var("weapon_recoil_scale");

		aim_punch = g->local->get_punch_angle() * recoil_cvar->get_float();

		// rcs in %
		aim_punch *= (g->v.aimbot.rcs_val / 200.f);

		//distance modifi
		vec3 angle_point = math::calculate_angle(g->local->get_eye_position(), ang);
		const float dist = angle_point.dist_to(g->cmd->viewangles);
		float delta = (1.f - dist / 300.f);
		std::clamp(delta, 0.f, 1.f);
		//apply
		aim_punch *= delta;

		auto rcs = aim_punch;

		ang -= rcs;
	}
	void c_aimbot::fire()
	{
		//global aiming data:::
		c_entity* bst_e = nullptr;

		vec3 bst_vec = vec3(0, 0, -1);

		vec3 bst_ang = vec3(0, 0, -1);

		//get closest target/vec
		float fov_limit = 180.f;

		for (int i { }; i < 64; i++)
		{
			c_entity* e = interfaces->entity_list->get_client_entity(i);

			if (!e || !e->is_alive() || e->is_dormant() || e->has_gun_game_immunity())
				continue;

			if (!e->is_player())
				continue;

			if (e->get_team() == g->local->get_team())
				continue;

			if (e == g->local)
				continue;

			vec3 eye_pos = g->local->get_eye_position();

			vec3 world_point = get_point(e);

			//rebuild 3d point to angle 
			vec3 angle_point = math::calculate_angle(eye_pos, world_point);

			const float flfov = math::get_fov(g->cmd->viewangles, angle_point);

			//&& is_visible(e, world_point)

			if (flfov < fov_limit && is_visible(e, world_point))
			{
				bst_e = e;
				bst_ang = angle_point;
				bst_vec = world_point;
				fov_limit = flfov;
			}
		}

		//valid check
		if (!bst_e) 
			return;

		if (bst_vec == vec3(0, 0, -1) || bst_ang == vec3(0, 0, -1))
			return;

		if (!bst_vec.is_valid() || !bst_ang.is_valid())
			return;

		bool const valid_shot = ready_to_fire();

		//default
		vec3 real_aim_angle   = bst_ang;
		//silent
		vec3 silent_aim_angle = bst_ang;

		//ignore
		bool silent_shot = false;
		//have to set viewangle (engine)
		bool real_shot   = false;

		bool b_found = false;

		static bool delay_shot;

		const int t_shot = time_since_shot();
		
		//if weapon is ready to be used
		if (valid_shot)
		{
			//clicked
			if (GetAsyncKeyState(VK_LBUTTON))
			{	
				if (math::get_fov(g->cmd->viewangles, bst_ang) < g->v.aimbot.fov)
				{
					b_found = true;

					silent_shot = false;

					//engine set viewangles
					real_shot = true;
					//default aimbot

					//trace
					trace_t tr;

					ray_t ray;

					trace_filter filter;

					vec3 end;

					vec3 start;

					math::angle_vectors(g->cmd->viewangles + aim_punch, end);

					start = g->local->get_eye_position() ;
					end = start + (end * 56756.f);

					filter.skip = g->local;
					auto endpos = end;
					ray.initialize(g->local->get_eye_position(), endpos);

					interfaces->engine_trace->trace_ray(ray, MASK_SHOT | CONTENTS_GRATE, &filter, &tr);
					auto hit_ent = tr.entity == bst_e;

					if (!hit_ent)
					{
						delay_shot = true;
						//lock in attack button
						g->cmd->buttons &= ~in_attack;
									
					}

					//smooth ::

					auto smooth_value = g->v.aimbot.aim_smooth;

					static auto recoil_scale = interfaces->cvar->find_var("weapon_recoil_scale")->get_float();

					vec3 delta_vec = bst_ang - (g->cmd->viewangles + aim_punch * 1.5);

					math::normalize_angles(delta_vec);

					smooth_value = (interfaces->global_vars->intervalpertick * 64.0f) * smooth_value;
					
					delta_vec /= smooth_value;

					real_aim_angle = g->cmd->viewangles + delta_vec;

					math::normalize_angles(real_aim_angle);

				}
				if (math::get_fov(g->cmd->viewangles, bst_ang) < g->v.aimbot.silent_fov)
				{
					silent_shot = true;
					//max chance = 100, min = 0
					silent_aim_angle = bst_ang;
				}
			}

			//
			if (real_shot)//set
			{
				do_rcs(real_aim_angle);
				g->cmd->viewangles = real_aim_angle;
				interfaces->engine->set_view_angles(real_aim_angle);
			}

			//set new angle, if need
			if (silent_shot)
			{
				g->cmd->viewangles = silent_aim_angle;
			}

			//normalize
			math::normalize_angle(g->cmd->viewangles);
			math::normalize_angle(real_aim_angle);
		}


	}

	bool c_aimbot::is_visible(c_entity* e, vec3 p) {

		trace_t tr;
		ray_t ray;
		trace_filter filter;

		filter.skip = g->local;
		auto endpos = p;
		ray.initialize(g->local->get_eye_position(), endpos);

		interfaces->engine_trace->trace_ray(ray, MASK_SHOT | CONTENTS_GRATE, &filter, &tr);
		auto result = tr.entity == e || tr.flFraction > 0.97f;

		if (result)
			return true;

		return result;
	}

	bool c_aimbot::hitchance(vec3& angle, c_entity* ent) {

		int traces_hit = 0;
		auto local = g->local;
		if (!local)
			return false;

		vec3 forward, right, up;
		const vec3 eye_position = local->get_eye_position();
		math::angle_vectors(angle + local->get_view_punch_angle() * 2.f, &forward, &right, &up); 
		// maybe add an option to not account for punch.

		auto weapon = local->get_weapon();

		if (!weapon)
			return false;

		float weapon_spread = weapon->get_spread();
		float weapon_cone = weapon->get_inaccuracy();

		const auto get_bullet_location = [&](int seed) {

			math::random_seed(seed);

			float a = math::random_float(0.f, 1.f);
			float b = math::random_float(0.f, 2.f * M_PI);
			float c = math::random_float(0.f, 1.f);
			float d = math::random_float(0.f, 2.f * M_PI);

			const float generated_spread = a * weapon_spread;
			const float generated_cone = c * weapon_cone;

			const vec3 spread = vec3(
				std::cos(b) * generated_spread + std::cos(d) * generated_cone,
				std::sin(b) * generated_spread + std::sin(d) * generated_cone,
				0.f
			);

			return vec3(forward + right * -spread.x + up * -spread.y).normalized();
		};

		for (int i = 1; i <= 256; i++) {

			vec3 spread_angle;
			vec3 bullet_end;

			math::vector_angles(get_bullet_location(i), spread_angle);
			math::angle_vectors(angle - (spread_angle - angle), bullet_end);

			trace_t trace;
			ray_t ray;
			ray.initialize(eye_position, eye_position + bullet_end * weapon->get_weapon_data()->flRange);

			interfaces->engine_trace->clip_ray_to_entity(ray, MASK_SHOT, ent, &trace);

			if (trace.entity == ent)
				++traces_hit;

			if (traces_hit >= static_cast<int>(g->v.aimbot.hitchance * 2.56f))
				return true;
		}

		return false;
	}

	vec3 c_aimbot::get_point(c_entity* target)
	{
		vec3 best_point;

		//whole points data
		std::vector < aim_vector > estimated_points;

		//store once
		target->invalidate_bone_cache();

		//setup current bones
		matrix bones[128];
		target->setup_bones(bones, 128, 256, interfaces->global_vars->curtime);

		vec3 eyepos = g->local->get_eye_position();

		//ignore limbs
		bool ignore_entity_extra_limbs = false;


		//add points
		if (g->v.aimbot.hitscan[g->v.aimbot.head])
		{
			//add head to scan vector if we dont have to aim only body
			estimated_points.push_back(aim_vector(target->get_bone_position(bones, hitbox_head), hitbox_head));
			add_multipoints(hitbox_head, bones, estimated_points, target);
			
		}
		if (g->v.aimbot.hitscan[g->v.aimbot.neck])
		{
			estimated_points.push_back(aim_vector(target->get_bone_position(bones, hitbox_neck), hitbox_neck));
			add_multipoints(hitbox_neck, bones, estimated_points, target);
			
		}
		if (g->v.aimbot.hitscan[g->v.aimbot.lowebody])
		{
			estimated_points.push_back(aim_vector(target->get_bone_position(bones, hitbox_lower_chest), hitbox_lower_chest));
			estimated_points.push_back(aim_vector(target->get_bone_position(bones, hitbox_chest), hitbox_chest));
			add_multipoints(hitbox_neck, bones, estimated_points, target);
			add_multipoints(hitbox_chest, bones, estimated_points, target);	
		}
		if (g->v.aimbot.hitscan[g->v.aimbot.stomach])
		{
			estimated_points.push_back(aim_vector(target->get_bone_position(bones, hitbox_stomach), hitbox_stomach));
			add_multipoints (hitbox_stomach, bones, estimated_points, target);
		}
		if (g->v.aimbot.hitscan[g->v.aimbot.upperbody])
		{
			estimated_points.push_back (aim_vector(target->get_bone_position(bones, hitbox_upper_chest), hitbox_upper_chest));
			add_multipoints (hitbox_upper_chest, bones, estimated_points, target);
		}
		if (g->v.aimbot.hitscan[g->v.aimbot.legs])
		{			
			estimated_points.push_back(aim_vector(target->get_bone_position(bones, hitbox_left_calf), hitbox_left_calf));
			estimated_points.push_back(aim_vector(target->get_bone_position(bones, hitbox_right_calf), hitbox_right_calf));
			estimated_points.push_back(aim_vector(target->get_bone_position(bones, hitbox_right_foot), hitbox_right_foot));
			estimated_points.push_back(aim_vector(target->get_bone_position(bones, hitbox_left_foot), hitbox_left_foot));
			add_multipoints(hitbox_left_calf, bones, estimated_points, target);
			add_multipoints(hitbox_right_calf, bones, estimated_points, target);
			add_multipoints(hitbox_right_foot, bones, estimated_points, target);
			add_multipoints(hitbox_left_foot, bones, estimated_points, target);	
		}

		//sort
		float worst_distance = FLT_MAX;
		for (auto i = 0; i < estimated_points.size(); i++)
		{
			//rebuild
			vec3 angle_point = math::calculate_angle(g->local->get_eye_position(), estimated_points[i].vec);
			const float dist = angle_point.dist_to(g->cmd->viewangles);
			if (dist < worst_distance)
			{
				//set new best record
				best_point = estimated_points[i].vec;
				worst_distance = dist;
				//set new best value		
			}
		}

		//filter points

		return best_point;

	}
	bool c_aimbot::ready_to_fire() {

		int shift = 0; // make it shift tickbase

		auto weapon = g->local->get_weapon();

		if (!weapon)
			return false;

		float server_time = ticks_to_time(g->local->get_tickbase() - shift);

		if (weapon->get_next_attack() > server_time)
			return false;

		if (weapon->get_next_secondary_attack() > server_time)
			return false;

		if (weapon->get_next_primary_attack() > server_time)
			return false;

		if (weapon->get_item_definition_index() == weapon_revolver && weapon->post_pone_fire_ready_time() > server_time)
			return false;

		return true;
	}
	void c_aimbot::add_multipoints(int hitbox, matrix bones[128], std::vector< aim_vector > & points, c_entity* target)
	{
		auto mdl = interfaces->model_info->get_studio_model(target->GetModel());
		auto set = mdl->pHitboxSet(target->get_hitbox_set());
		auto hbx = set->pHitbox(hitbox);

		if (!hbx) return;

		vec3 mins, maxs;
		math::vector_transform(hbx->bbmin, bones[hbx->bone], mins);
		math::vector_transform(hbx->bbmax, bones[hbx->bone], maxs);

		vec3 center = (mins + maxs) * 0.5f;
		vec3 angle = math::calculate_angle(center, g->local->get_eye_position());

		vec3 forward;

		math::angle_vectors(angle, forward);

		vec3 right  = forward.cross(vec3(0, 0, 1));
		vec3 left   = vec3(-right.x, -right.y, right.z);
		vec3 top    = vec3(0, 0, 1);
		vec3 bottom = vec3(0, 0, -1);

		float adjusted_radius = 0.f;

		//scale sides mode :
		bool sides [4] { false, false, false, false };

		switch (hitbox)
		{
			case hitbox_head: 
			{
				adjusted_radius = hbx->radius * double(g->v.aimbot.hitbox_scale_head / 100.f);
				sides[sides_e::iright]	 = true; /*right scale */ {
					sides[sides_e::ileft] = true; /*left scale */
					sides[sides_e::itop ] = true; /*top scale */
				}
			}
			break;
			case hitbox_neck:
			{
				adjusted_radius = hbx->radius * double(g->v.aimbot.hitbox_scale_body / 100.f);
				sides[sides_e::iright]	 = true; /*right scale */ {
					sides[sides_e::ileft] = true; /*left scale */
				}
			}
			break;
			case hitbox_stomach:
			{
				adjusted_radius = hbx->radius * double(g->v.aimbot.hitbox_scale_body / 100.f);
				sides[sides_e::iright] = true; /*right scale */ {
					sides[sides_e::ileft   ] = true; /*left scale */
					sides[sides_e::ibottom ] = true; /*top scale */
				}
			}
			break;
			case hitbox_upper_chest:
			{
				adjusted_radius = hbx->radius * double(g->v.aimbot.hitbox_scale_body / 100.f);
				sides[sides_e::iright]    = true; /*right scale */ {
					sides[sides_e::ileft] = true; /*left scale */
					sides[sides_e::itop ] = true; /*top scale */
				}
			}
			break;
			case hitbox_pelvis:
			{
				adjusted_radius = hbx->radius * double(g->v.aimbot.hitbox_scale_body / 100.f);
				sides[sides_e::iright]	   = true; /*right scale */ {
					sides[sides_e::ileft  ] = true; /*left scale */
					sides[sides_e::ibottom] = true; /*top scale */
				}
			}
			break;
			case hitbox_left_calf  :
			case hitbox_right_calf :
			case hitbox_right_foot :
			case hitbox_left_foot  :
			{
				adjusted_radius = hbx->radius * double(g->v.aimbot.hitbox_scale_legs / 100.f);
				sides[sides_e::iright] = true; /*right scale */ {
					sides[sides_e::ibottom] = true; /*top scale */
				}
			}
			break;
		}

		if (sides[sides_e::iright]) //right side of scaled point
			points.push_back(aim_vector({ center + right * (adjusted_radius) }, hitbox));
		if (sides[sides_e::ileft]) //left side of scaled point
			points.push_back(aim_vector({ center + left * (adjusted_radius) }, hitbox));
		if (sides[sides_e::itop]) //top side of scaled point
			points.push_back(aim_vector({ center + top * (adjusted_radius) }, hitbox));
		if (sides[sides_e::ibottom]) //bottom side of scaled point
			points.push_back(aim_vector({ center + bottom * (adjusted_radius) }, hitbox));
	}

}