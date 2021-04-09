#include "visuals.h"
#include "entity.h"
#include "globals.h"
#include "interfaces.h"
#include "studio.h"
#include "render.h"
#include "math.h"
#include <algorithm>
#include "hvh.h"

color color::Black(0, 0, 0);
color color::White(255, 255, 255);
color color::Red(255, 0, 0);
color color::Green(0, 255, 0);
color color::Blue(0, 0, 255);
color color::Yellow(255, 255, 0);
color color::Pink(255, 0, 255);
color color::LightBlue(0, 255, 255);

c_visuals* visuals = new c_visuals();

bool c_visuals::get_box(c_entity* pPlayer, c_visuals::box_t& box)
{
	vec3 origin{}, mins{}, maxs{};
	vec3 bottom{}, top{};

	origin = pPlayer->get_vec_origin();

	// get hitbox bounds.
	pPlayer->compute_hitbox_surrounding_box(&mins, &maxs);

	// correct x and y coordinates.
	mins = { origin.x, origin.y, mins.z };
	maxs = { origin.x, origin.y, maxs.z + 8.f };

	if (!math::world_to_screen(mins, bottom) || !math::world_to_screen(maxs, top))
		return false;

	box.h = bottom.y - top.y;
	box.w = box.h / 2.f;
	box.x = bottom.x - (box.w / 2.f);
	box.y = bottom.y - box.h;
	return true;
}

void c_visuals::on_player(c_entity* entity) {
	if (entity->is_dormant())
		return;

	static auto sanitize = [](char* name) -> std::string
	{
		std::string tmp(name);

		if (tmp.length() > 20)
		{
			tmp.erase(20, tmp.length() - 20);
			tmp.append("...");
		}

		return tmp;
	};

	box_t box{};
	if (!get_box(entity, box))
		return;

	player_info_t info{};
	interfaces->engine->get_player_info(entity->EntIndex(), &info);

	std::string name = sanitize(info.name);
	int health = entity->get_health();
	health = std::clamp(health, 0, 100);

	int armor = entity->get_armor();
	bool helmet = entity->has_helmet();

	c_weapon* weapon = nullptr;
	if (!weapon)
		weapon = entity->get_weapon();

	c_weapon_info* weapon_info = nullptr;
	if (!weapon_info)
		weapon_info = weapon->get_weapon_data();

	std::string weapon_name = weapon_info->szWeaponName;
	weapon_name.replace(0, 7, "");

	int ammo = -1;
	int ammo_max = -1;
	if (weapon->is_grenade() || weapon->is_knife()) {
		ammo = ammo_max = 0;
	}
	else {
		ammo = weapon->get_ammo();
		ammo_max = weapon_info->iMaxClip1;
	}

	auto hdr = interfaces->model_info->get_studio_model(entity->GetModel());
	if (!hdr)
		return;

	auto upper_direction = entity->get_bone_position(entity->get_bone_cache().base(), 7) - entity->get_bone_position(entity->get_bone_cache().base(), 6);
	auto breast_bone = entity->get_bone_position(entity->get_bone_cache().base(), 6) + upper_direction * 0.5f;

	for (auto j = 0; j < hdr->numbones; j++)
	{
		auto bone = hdr->pBone(j);

		if (!bone)
			continue;

		if (bone->parent == -1)
			continue;

		if (!(bone->flags & BONE_USED_BY_HITBOX))
			continue;

		auto child = entity->get_bone_position(entity->get_bone_cache().base(), j);
		auto parent = entity->get_bone_position(entity->get_bone_cache().base(), bone->parent);

		auto delta_child = child - breast_bone;
		auto delta_parent = parent - breast_bone;

		if (delta_parent.length() < 9.0f && delta_child.length() < 9.0f)
			parent = breast_bone;

		if (j == 5)
			child = breast_bone;

		if (fabs(delta_child.z) < 5.0f && delta_parent.length() < 5.0f && delta_child.length() < 5.0f || j == 6)
			continue;

		if (g->v.visuals.skeleton) {
			vec3 vec_child, vec_parent;
			if (!math::world_to_screen(child, vec_child, false)
				|| !math::world_to_screen(parent, vec_parent, false))
				continue;

			render->line(vec_parent.x, vec_parent.y, vec_child.x, vec_child.y,
				color(g->v.visuals.color_skeleton[0] * 255.f,
					g->v.visuals.color_skeleton[1] * 255.f,
					g->v.visuals.color_skeleton[2] * 255.f,
					g->v.visuals.color_skeleton[3] * 255.f));
		}
	}

	if (g->v.visuals.name) 
		render->text(fonts::main, box.x + box.w / 2.f, box.y - 13.f,
			color(g->v.visuals.color_name[0] * 255.f,
				g->v.visuals.color_name[1] * 255.f,
				g->v.visuals.color_name[2] * 255.f,
				g->v.visuals.color_name[3] * 255.f), HFONT_CENTERED_X, name.c_str());

	if (g->v.visuals.weapon)
		render->text(fonts::misc, box.x + box.w / 2.f, box.y + box.h + 2,
			color(g->v.visuals.color_weapon[0] * 255.f,
				g->v.visuals.color_weapon[1] * 255.f,
				g->v.visuals.color_weapon[2] * 255.f,
				g->v.visuals.color_weapon[3] * 255.f), HFONT_CENTERED_X, weapon_name.c_str());

	if (g->v.visuals.flags) {
		int off_y = 0;

		if (armor > 0) {
			render->text(fonts::misc, box.x + box.w + 3, (box.y - 2),
				color::White, 0, helmet ? "hk" : "k");
			off_y += 9;
		}
		if (ammo > 0)
		{
			std::string s = "ammo: " + std::to_string(ammo);
			render->text(fonts::misc, box.x + box.w + 3, (box.y - 2) + off_y,
				color::White, 0, s.c_str());
			off_y += 9;
		}
		if (health > 0 && health <= 50) {
			render->text(fonts::misc, box.x + box.w + 3, (box.y - 2) + off_y,
				color::Red, 0, "lethal");
			off_y += 9;
		}
	}

	if (g->v.visuals.box) {
		render->rect(box.x + 1, box.y + 1, box.w - 2, box.h - 2, color(0, 0, 0, g->v.visuals.color_box[3] * 255.f));
		render->rect(box.x - 1, box.y - 1, box.w + 2, box.h + 2, color(0, 0, 0, g->v.visuals.color_box[3] * 255.f));
		render->rect(box.x, box.y, box.w, box.h,
			color(g->v.visuals.color_box[0] * 255.f,
				g->v.visuals.color_box[1] * 255.f,
				g->v.visuals.color_box[2] * 255.f,
				g->v.visuals.color_box[3] * 255.f));
	}

	if (g->v.visuals.health_bar) {
		int health_height = box.h - (box.h * health) / 100;
		render->rect_filled(box.x - 6, box.y - 1, 4, box.h + 2, color(0, 0, 0, g->v.visuals.color_hp[3] * 255.f));
		render->rect_filled(box.x - 5, box.y + health_height, 2, box.h - health_height,
			color(g->v.visuals.color_hp[0] * 255.f,
				g->v.visuals.color_hp[1] * 255.f,
				g->v.visuals.color_hp[2] * 255.f,
				g->v.visuals.color_hp[3] * 255.f));
	}
}

void c_visuals::main() {
	if (!g->v.visuals.enable)
		return;

	if (!g->can_connect)
		return;

	// https://github.com/danielkrupinski/Osiris/blob/master/Osiris/GameData.cpp#L34
	static int lastFrame;
	if (lastFrame == interfaces->global_vars->framecount)
		return;

	lastFrame = interfaces->global_vars->framecount;

	if (g->is_local_alive) {
		if (g->v.visuals.remove_scope && g->is_local_scoped) {
			render->line(g->screen.w / 2, 0, g->screen.w / 2, g->screen.h, color::Black);
			render->line(0, g->screen.h / 2, g->screen.w, g->screen.h / 2, color::Black);
		}
	}


	for (int i = 0; i < interfaces->entity_list->get_highest_ent_index(); i++) {
		c_entity* entity = interfaces->entity_list->get_client_entity(i);
		if (entity == nullptr)
			continue;

		if (entity->is_player()) {
			if (entity == g->local || entity->get_team() == g->local->get_team()
				|| !entity->is_alive())
				continue;

			on_player(entity);
		}
	}

	float fov = static_cast<float>(g->local->get_fov());

	vec3 screensize = vec3(g->screen.w, g->screen.h, 0);

	vec3 center = screensize * 0.5f;

	float ratio = screensize.x / screensize.y;

	float screenFov = atanf((ratio) * (0.75f) * tan(DEG2RAD(fov * 0.5f)));

	float fov_circle = tanf(DEG2RAD(g->v.aimbot.fov)) / tanf(screenFov) * center.x;
	render->rect(g->screen.w / 2 - fov_circle, g->screen.h / 2 - fov_circle, fov_circle * 2, fov_circle * 2, color(255, 0, 0));

	float silent_fov_circle = tanf(DEG2RAD(g->v.aimbot.silent_fov)) / tanf(screenFov) * center.x;
	render->rect(g->screen.w / 2 - silent_fov_circle, g->screen.h / 2 - silent_fov_circle, silent_fov_circle * 2, silent_fov_circle * 2, color(0, 0, 255));

	if (g->v.visuals.hit_skeleton) {
		for (int x = 0; x < 65; x++) {
			auto& i = skeleton_info[x];
			for (int s = 0; s < i.size(); s++) {
				auto skelet = i[s];
				float expire_time = interfaces->global_vars->curtime - skelet->time;
				if (expire_time > 2.f)
					skelet->alpha -= 1.f;

				skelet->alpha = std::clamp(skelet->alpha, 0.f, 255.f);

				if (skelet->alpha <= 0)
					i.erase(i.begin() + s);

				for (int j = 0; j < 128; j++) {
					vec3 child, parent;
					if (math::world_to_screen(skelet->parent[j], parent)
						&& math::world_to_screen(skelet->child[j], child)) {
						render->line(parent.x, parent.y, child.x, child.y,
							skelet->clr.manage_alpha(skelet->alpha));
					}
				}
			}
		}
	}
}