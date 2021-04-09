#include "misc.h"
#include "globals.h"
#include "vector3.h"
#include "cmd.h"
#include "config.h"
#include "cvar.h"
#include "interfaces.h"
#include "entity.h"
#include "netvar_manager.h"
#include "math.h"
#include "imgui/imgui.h"
#include <array>

viewmodel_info* viewmodel_data = new viewmodel_info();

c_misc* misc = new c_misc();

#define CheckIfNonValidNumber(x) (fpclassify(x) == FP_INFINITE || fpclassify(x) == FP_NAN || fpclassify(x) == FP_SUBNORMAL)

void c_misc::rebuild_mouse_delta()
{
	if (!g->local || !g->local->is_alive())
		return;

	static vec3 delta_viewangles{ };
	vec3 delta = g->cmd->viewangles - delta_viewangles;

	static c_cvar* sensitivity = interfaces->cvar->find_var("sensitivity");

	if (!sensitivity)
		return;

	if (delta.x != 0.f) {
		static c_cvar* m_pitch = interfaces->cvar->find_var("m_pitch");

		if (!m_pitch)
			return;

		int final_dy = static_cast<int>((delta.x / m_pitch->get_float()) / sensitivity->get_float());
		if (final_dy <= 32767) {
			if (final_dy >= -32768) {
				if (final_dy >= 1 || final_dy < 0) {
					if (final_dy <= -1 || final_dy > 0)
						final_dy = final_dy;
					else
						final_dy = -1;
				}
				else {
					final_dy = 1;
				}
			}
			else {
				final_dy = 32768;
			}
		}
		else {
			final_dy = 32767;
		}

		g->cmd->mousedy = static_cast<short>(final_dy);
	}

	if (delta.y != 0.f) {
		static c_cvar* m_yaw = interfaces->cvar->find_var("m_yaw");

		if (!m_yaw)
			return;

		int final_dx = static_cast<int>((delta.y / m_yaw->get_float()) / sensitivity->get_float());
		if (final_dx <= 32767) {
			if (final_dx >= -32768) {
				if (final_dx >= 1 || final_dx < 0) {
					if (final_dx <= -1 || final_dx > 0)
						final_dx = final_dx;
					else
						final_dx = -1;
				}
				else {
					final_dx = 1;
				}
			}
			else {
				final_dx = 32768;
			}
		}
		else {
			final_dx = 32767;
		}

		g->cmd->mousedx = static_cast<short>(final_dx);
	}

	delta_viewangles = g->cmd->viewangles;
}

void c_misc::bunnyhop()
{
	if (!g->local || !g->local->is_alive())
		return;

	if (!g->v.misc.auto_jump)
		return;

	if (!(g->local->get_flags() & fl_onground))
	{
		if (g->cmd->buttons & in_jump)
			g->cmd->buttons &= ~in_jump;
	}
}

void c_misc::auto_strafe()
{
	if (!g->local || !g->local->is_alive())
		return;

	if (!g->v.misc.air_strafe)
		return;

	static auto cl_sidespeed = interfaces->cvar->find_var("cl_sidespeed");
	auto side_speed = cl_sidespeed->get_float();

	if (!(g->local->get_flags() & fl_onground))
	{
		static auto old_yaw = 0.0f;

		auto get_velocity_degree = [](float velocity)
		{
			auto tmp = RAD2DEG(atan(30.0f / velocity));

			if (CheckIfNonValidNumber(tmp) || tmp > 90.0f)
				return 90.0f;

			else if (tmp < 0.0f)
				return 0.0f;
			else
				return tmp;
		};

		if (g->local->get_move_type() != movetype_walk)
			return;

		auto velocity = g->local->get_velocity();
		velocity.z = 0.0f;

		auto forwardmove = g->cmd->forwardmove;
		auto sidemove = g->cmd->sidemove;

		if (velocity.length_2d() < 5.0f && !forwardmove && !sidemove)
			return;

		static auto flip = false;
		flip = !flip;

		auto turn_direction_modifier = flip ? 1.0f : -1.0f;
		auto viewangles = g->cmd->viewangles;

		if (g->cmd->buttons & in_moveleft) {
			if (g->cmd->buttons & in_moveright)
				g->cmd->buttons &= ~in_moveright;
		}
		else if (g->cmd->buttons & in_moveright) {
			if (g->cmd->buttons & in_moveleft)
				g->cmd->buttons &= ~in_moveleft;
		}
		else if (g->cmd->buttons & in_forward) {
			if (g->cmd->buttons & in_back)
				g->cmd->buttons &= ~in_back;
		}
		else if (g->cmd->buttons & in_back) {
			if (g->cmd->buttons & in_forward)
				g->cmd->buttons &= ~in_forward;
		}

		if (forwardmove || sidemove)
		{
			g->cmd->forwardmove = 0.0f;
			g->cmd->sidemove = 0.0f;

			auto turn_angle = atan2(-sidemove, forwardmove);
			viewangles.y += turn_angle * 57.92f;
		}
		else if (forwardmove) //-V550
			g->cmd->forwardmove = 0.0f;

		auto strafe_angle = RAD2DEG(atan(15.0f / velocity.length_2d()));

		if (strafe_angle > 90.0f)
			strafe_angle = 90.0f;
		else if (strafe_angle < 0.0f)
			strafe_angle = 0.0f;

		auto temp = vec3(0.0f, viewangles.y - old_yaw, 0.0f);
		temp.y = math::normalize_yaw(temp.y);

		auto yaw_delta = temp.y;
		old_yaw = viewangles.y;

		auto abs_yaw_delta = fabs(yaw_delta);

		if (abs_yaw_delta <= strafe_angle || abs_yaw_delta >= 30.0f)
		{
			vec3 velocity_angles;
			math::vector_angles(velocity, velocity_angles);

			temp = vec3(0.0f, viewangles.y - velocity_angles.y, 0.0f);
			temp.y = math::normalize_yaw(temp.y);

			auto velocityangle_yawdelta = temp.y;
			auto velocity_degree = get_velocity_degree(velocity.length_2d());

			if (velocityangle_yawdelta <= velocity_degree || velocity.length_2d() <= 15.0f)
			{
				if (-velocity_degree <= velocityangle_yawdelta || velocity.length_2d() <= 15.0f)
				{
					viewangles.y += strafe_angle * turn_direction_modifier;
					g->cmd->sidemove = side_speed * turn_direction_modifier;
				}
				else
				{
					viewangles.y = velocity_angles.y - velocity_degree;
					g->cmd->sidemove = side_speed;
				}
			}
			else
			{
				viewangles.y = velocity_angles.y + velocity_degree;
				g->cmd->sidemove = -side_speed;
			}
		}
		else if (yaw_delta > 0.0f)
			g->cmd->sidemove = -side_speed;
		else if (yaw_delta < 0.0f)
			g->cmd->sidemove = side_speed;

		auto move = vec3(g->cmd->forwardmove, g->cmd->sidemove, 0.0f);
		auto speed = move.length();

		vec3 angles_move;
		math::vector_angles(move, angles_move);

		auto normalized_x = fmod(g->cmd->viewangles.x + 180.0f, 360.0f) - 180.0f;
		auto normalized_y = fmod(g->cmd->viewangles.y + 180.0f, 360.0f) - 180.0f;

		auto yaw = DEG2RAD(normalized_y - viewangles.y + angles_move.y);

		if (normalized_x >= 90.0f || normalized_x <= -90.0f 
			|| g->cmd->viewangles.x >= 90.0f && g->cmd->viewangles.x <= 200.0f
			|| g->cmd->viewangles.x <= -90.0f && g->cmd->viewangles.x <= 200.0f) //-V648
			g->cmd->forwardmove = -cos(yaw) * speed;
		else
			g->cmd->forwardmove = cos(yaw) * speed;

		g->cmd->sidemove = sin(yaw) * speed;
	}
}

void c_misc::slide_walk(bool extra) {
	if (extra) {
		if (g->cmd->forwardmove > 0.0f)
		{
			g->cmd->buttons |= in_back;
			g->cmd->buttons &= ~in_forward;
		}
		else if (g->cmd->forwardmove < 0.0f)
		{
			g->cmd->buttons |= in_forward;
			g->cmd->buttons &= ~in_back;
		}

		if (g->cmd->sidemove > 0.0f)
		{
			g->cmd->buttons |= in_moveleft;
			g->cmd->buttons &= ~in_moveright;
		}
		else if (g->cmd->sidemove < 0.0f)
		{
			g->cmd->buttons |= in_moveright;
			g->cmd->buttons &= ~in_moveleft;
		}
	}
	else {
		auto cmd = g->cmd;

		auto buttons = cmd->buttons & ~(in_moveright | in_moveleft | in_back | in_forward);

		if (cmd->forwardmove <= 0.0f)
			buttons |= in_back;
		else
			buttons |= in_forward;

		if (cmd->sidemove > 0.0f)
			goto LABEL_15;
		else if (cmd->sidemove >= 0.0f)
			goto LABEL_18;

		goto LABEL_17;

		if (cmd->forwardmove <= 0.0f) //-V779
			buttons |= in_forward;
		else
			buttons |= in_back;

		if (cmd->sidemove > 0.0f)
		{
		LABEL_17:
			buttons |= in_moveleft;
			goto LABEL_18;
		}

		if (cmd->sidemove < 0.0f)
			LABEL_15:

		buttons |= in_moveright;

	LABEL_18:
		cmd->buttons = buttons;
	}
}

void c_misc::fake_lag()
{
	if (!g->local || !g->local->is_alive())
		return;

	if (g->fake_ducking)
		return;

	if (g->v.misc.fake_lag) {
		if (g->local->get_velocity().length_2d() > 10.f)
			g->send_packet = interfaces->client_state->iChokedCommands >= g->v.misc.amount;
		else
			g->send_packet = interfaces->client_state->iChokedCommands >= 1;
	}
	else {
		if (g->v.anti_aim.enable)
			g->send_packet = interfaces->client_state->iChokedCommands >= 1;
	}
}

void c_misc::fake_duck()
{
	if (!g->local || !g->local->is_alive()) {
		g->fake_ducking = false;
		return;
	}

	if (!(g->local->get_flags() & fl_onground)) {
		g->fake_ducking = false;
		return;
	}

	if (ImGui::IsActiveHotkey(g->v.misc.fakeduck))
	{
		g->send_packet = interfaces->client_state->iChokedCommands >= 14;

		g->fake_ducking = true;

		if (interfaces->client_state->iChokedCommands > 6)
			g->cmd->buttons |= in_duck;
		else
			g->cmd->buttons &= ~in_duck;
	}
	else g->fake_ducking = false;
}

void c_misc::record_viewmodel_values()
{
	if (!g->local || !g->local->is_alive())
		return;

	auto viewmodel = g->local->get_viewmodel();

	if (viewmodel)
	{
		viewmodel_data->weapon = viewmodel->get_weapon_handle().Get();

		viewmodel_data->viewmodel_index = viewmodel->get_viewmodel_index();
		viewmodel_data->sequence = viewmodel->get_sequence();
		viewmodel_data->animation_parity = viewmodel->get_animation_party();

		viewmodel_data->cycle = viewmodel->get_cycle();
		viewmodel_data->animation_time = viewmodel->get_animtime();
	}
}

void c_misc::set_viewmodel_values(frame_stage stage)
{
	if (!g->local || !g->local->is_alive())
		return;

	if (stage == frame_net_update_postdataupdate_start)
	{
		auto viewmodel = g->local->get_viewmodel();

		if (viewmodel 
			&& viewmodel_data->weapon == viewmodel->get_weapon_handle().Get()
			&& viewmodel_data->sequence == viewmodel->get_sequence() 
			&& viewmodel_data->animation_parity == viewmodel->get_animation_party())
		{
			viewmodel->get_cycle() = viewmodel_data->cycle;
			viewmodel->get_animtime() = viewmodel_data->animation_time;
		}
	}
}

anim_info_t anim_info;

void update_viewmodel(c_entity* player)
{
	static auto update_all_viewmodel_addons = reinterpret_cast<int(__fastcall*)(void*)>(
		utils->find_pattern("client.dll",
		"55 8B EC 83 E4 ? 83 EC ? 53 8B D9 56 57 8B 03 FF 90 ? ? ? ? 8B F8 89 7C 24 ? 85 FF 0F 84 ? ? ? ? 8B 17 8B CF"));

	static auto get_viewmodel = reinterpret_cast<void* (__thiscall*)(void*, bool)>(
		utils->find_pattern("client.dll",
		"55 8B EC 8B 45 08 53 8B D9 56 8B 84 83 ? ? ? ? 83 F8 FF 74 18 0F"));

	auto v10 = getvfunc<int(__thiscall*)(c_entity*)>(player, 157)(player);
	if (!v10) return;

	auto v11 = getvfunc<int(__thiscall*)(c_entity*)>(player, 267)(player);
	if (!v11) return;

	auto viewmodel = get_viewmodel(player, *(bool*)(v11 + 0x3224));
	if (!viewmodel) return;

	update_all_viewmodel_addons(viewmodel);
}

void c_misc::do_animfix()
{
	if (!g->local || !g->local->is_alive())
		return;

	c_animstate* state = g->local->get_animstate();
	if (!state)
		return;

	if (state->m_iLastClientSideAnimationUpdateFramecount == interfaces->global_vars->framecount)
		state->m_iLastClientSideAnimationUpdateFramecount = interfaces->global_vars->framecount - 1;

	if (g->send_packet) {
		std::array<float, 24> poses = g->local->get_pose_parameter();

		state->m_flGoalFeetYaw = g->real_ang.y;
		g->local->get_clientside_animation() = true;
		g->should_update = true;
		g->local->update_anim_state(state, g->fake_ang);
		g->should_update = false;

		anim_info.gfy = state->m_flGoalFeetYaw;
		anim_info.poses = g->local->get_pose_parameter();
		g->local->get_pose_parameter() = poses;
	}
	state->m_flGoalFeetYaw = anim_info.gfy;
	update_viewmodel(g->local);
}

void c_misc::fix_movement()
{
	vec3 original;
	interfaces->engine->get_view_angles(original);

	vec3 vMove(g->cmd->forwardmove, g->cmd->sidemove, g->cmd->upmove);
	float flSpeed = sqrt(vMove.x * vMove.x + vMove.y * vMove.y), flYaw;
	vec3 vMove2;
	math::vector_angles(vMove, vMove2);
	vMove2.normalize();
	flYaw = DEG2RAD(g->cmd->viewangles.y - original.y + vMove2.y);
	g->cmd->forwardmove = cos(flYaw) * flSpeed;
	g->cmd->sidemove = sin(flYaw) * flSpeed;
	if (90 < abs(g->cmd->viewangles.x) && 180 > abs(g->cmd->viewangles.x))
		g->cmd->forwardmove *= -1;
}

std::vector<std::string> config_list = {};
typedef void(*LPSEARCHFUNC)(LPCTSTR lpszFileName);

BOOL search_files(LPCTSTR lpszFileName, LPSEARCHFUNC lpSearchFunc, BOOL bInnerFolders = FALSE)
{
	LPTSTR part;
	char tmp[MAX_PATH];
	char name[MAX_PATH];

	HANDLE hSearch = NULL;
	WIN32_FIND_DATA wfd;
	memset(&wfd, 0, sizeof(WIN32_FIND_DATA));

	if (bInnerFolders)
	{
		if (GetFullPathName(lpszFileName, MAX_PATH, tmp, &part) == 0) return FALSE;
		strcpy(name, part);
		strcpy(part, "*.*");
		wfd.dwFileAttributes = FILE_ATTRIBUTE_DIRECTORY;
		if (!((hSearch = FindFirstFile(tmp, &wfd)) == INVALID_HANDLE_VALUE))
			do
			{
				if (!strncmp(wfd.cFileName, ".", 1) || !strncmp(wfd.cFileName, "..", 2))
					continue;

				if (wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
				{
					char next[MAX_PATH];
					if (GetFullPathName(lpszFileName, MAX_PATH, next, &part) == 0) return FALSE;
					strcpy(part, wfd.cFileName);
					strcat(next, "\\");
					strcat(next, name);

					search_files(next, lpSearchFunc, TRUE);
				}
			} while (FindNextFile(hSearch, &wfd));
			FindClose(hSearch);
	}

	if ((hSearch = FindFirstFile(lpszFileName, &wfd)) == INVALID_HANDLE_VALUE)
		return TRUE;
	do
		if (!(wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
		{
			char file[MAX_PATH];
			if (GetFullPathName(lpszFileName, MAX_PATH, file, &part) == 0) return FALSE;
			strcpy(part, wfd.cFileName);

			lpSearchFunc(wfd.cFileName);
		}
	while (FindNextFile(hSearch, &wfd));
	FindClose(hSearch);
	return TRUE;
}

void read_configs(LPCTSTR lpszFileName)
{
	config_list.push_back(lpszFileName);
}

void refresh_configs()
{
	static TCHAR path[MAX_PATH];
	std::string folder, file;

	if (SUCCEEDED(SHGetFolderPath(NULL, CSIDL_PERSONAL, NULL, 0, path)))
	{
		config_list.clear();
		std::string ConfigDir = std::string(path) + "\\csgo\\*.json";
		search_files(ConfigDir.c_str(), read_configs, FALSE);
	}
}

namespace ImGui
{
	static auto vector_getter = [](void* vec, int idx, const char** out_text)
	{
		auto& vector = *static_cast<std::vector<std::string>*>(vec);
		if (idx < 0 || idx >= static_cast<int>(vector.size())) { return false; }
		*out_text = vector.at(idx).c_str();
		return true;
	};

	IMGUI_API bool ComboBoxArray(const char* label, int* currIndex, std::vector<std::string>& values)
	{
		if (values.empty()) { return false; }
		return Combo(label, currIndex, vector_getter,
			static_cast<void*>(&values), values.size());
	}

	IMGUI_API bool ListBoxCfg(const char* label, int* currIndex, std::vector<std::string>& values, int height)
	{
		if (values.empty()) { return false; }

		return ListBox(label, currIndex, vector_getter, static_cast<void*>(&values), values.size(), height);
	}
}

void c_misc::do_menu() {
	refresh_configs();
	if (!g->gui_opened)
		return;

	ImGui::SetColorEditOptions(ImGuiColorEditFlags_PickerHueWheel | ImGuiColorEditFlags_AlphaBar);
	ImGui::PushFont(g->fonts.misc);
	ImGui::SetNextWindowSize(ImVec2(500, 500), ImGuiCond_Once);
	ImGui::Begin("blackbird pre-alpha", &g->gui_opened,
		ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoSavedSettings);
	ImGui::PopFont();

	ImGui::PushFont(g->fonts.menu);
	{
		if (ImGui::BeginTabBar("cheat")) {
			if (ImGui::BeginTabItem("hvh")) {

				ImGui::Checkbox("aimbot", &g->v.aimbot.enable);

				ImGui::SliderInt("hitchance", &g->v.aimbot.hitchance, 0.f, 100.f);

				ImGui::SliderFloat("fov", &g->v.aimbot.fov, 0.f, 30.f);

				ImGui::SliderFloat("smooth", &g->v.aimbot.aim_smooth, 1.f, 100.f);

				ImGui::SliderFloat("silent fov", &g->v.aimbot.silent_fov, 0.f, 30.f);

				ImGui::SliderFloat("head scale", &g->v.aimbot.hitbox_scale_head, 0.f, 100.f);
				ImGui::SliderFloat("body scale", &g->v.aimbot.hitbox_scale_body, 0.f, 100.f);
				ImGui::SliderFloat("legs scale", &g->v.aimbot.hitbox_scale_legs, 0.f, 100.f);

				ImGui::Checkbox("rcs", &g->v.aimbot.rcs);

				ImGui::SliderInt("rcs value", &g->v.aimbot.rcs_val, 0, 100);

				ImGui::ListBoxHeader("hitboxes");
				{
					ImGui::Selectable("head", &g->v.aimbot.hitscan[g->v.aimbot.hitscan_menu::head]);
					ImGui::Selectable("neck", &g->v.aimbot.hitscan[g->v.aimbot.hitscan_menu::neck]);
					ImGui::Selectable("upperbody", &g->v.aimbot.hitscan[g->v.aimbot.hitscan_menu::upperbody]);
					ImGui::Selectable("lowerbody", &g->v.aimbot.hitscan[g->v.aimbot.hitscan_menu::lowebody]);
					ImGui::Selectable("stomach", &g->v.aimbot.hitscan[g->v.aimbot.hitscan_menu::stomach]);
					ImGui::Selectable("legs", &g->v.aimbot.hitscan[g->v.aimbot.hitscan_menu::legs]);
				}
				ImGui::ListBoxFooter();



				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("visuals")) {
				ImGui::Checkbox("toggle visuals", &g->v.visuals.enable);
				if (ImGui::BeginTabBar("visuals_tab")) {
					if (ImGui::BeginTabItem("enemy")) {
						ImGui::Checkbox("name", &g->v.visuals.name);
						ImGui::Checkbox("box", &g->v.visuals.box);
						ImGui::Checkbox("weapon", &g->v.visuals.weapon);
						ImGui::Checkbox("health bar", &g->v.visuals.health_bar);
						ImGui::Checkbox("skeleton", &g->v.visuals.skeleton);
						ImGui::Checkbox("model on hit", &g->v.visuals.hit_model);
						ImGui::Checkbox("skeleton on hit", &g->v.visuals.hit_skeleton);
						ImGui::Checkbox("chams visible", &g->v.visuals.chams_enemy_visible);
						ImGui::Checkbox("chams throw walls", &g->v.visuals.chams_enemy_xqz);
						ImGui::EndTabItem();
					}
					if (ImGui::BeginTabItem("local")) {
						ImGui::Checkbox("remove scope", &g->v.visuals.remove_scope);
						ImGui::EndTabItem();
					}
					if (ImGui::BeginTabItem("glow")) {
						ImGui::Checkbox("glow enemy", &g->v.visuals.glow_enemy);
						ImGui::Checkbox("glow team", &g->v.visuals.glow_team);
						ImGui::Checkbox("glow local", &g->v.visuals.glow_local);
						ImGui::EndTabItem();
					}
					if (ImGui::BeginTabItem("colors")) {
						ImGui::ColorEdit4("box color", g->v.visuals.color_box);
						ImGui::ColorEdit4("name color", g->v.visuals.color_name);
						ImGui::ColorEdit4("weapon color", g->v.visuals.color_weapon);
						ImGui::ColorEdit4("hp color", g->v.visuals.color_hp);
						ImGui::ColorEdit4("skeleton color", g->v.visuals.color_skeleton);
						ImGui::ColorEdit4("glow enemy color", g->v.visuals.color_glow_enemy);
						ImGui::ColorEdit4("glow team color", g->v.visuals.color_glow_team);
						ImGui::ColorEdit4("glow local color", g->v.visuals.color_glow_local);
						ImGui::ColorEdit4("chams enemy visible", g->v.visuals.color_chams_enemy_visible);
						ImGui::ColorEdit4("chams enemy throw walls", g->v.visuals.color_chams_enemy_xqz	);
						ImGui::EndTabItem();
					}
					ImGui::EndTabBar();
				}
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("misc")) {
				ImGui::Checkbox("auto jump", &g->v.misc.auto_jump);
				ImGui::Checkbox("air strafe", &g->v.misc.air_strafe);
				ImGui::Checkbox("meme walk", &g->v.misc.meme_walk);
				ImGui::Checkbox("fake lag", &g->v.misc.fake_lag);
				if (g->v.misc.fake_lag)
					ImGui::SliderInt("factor", &g->v.misc.amount, 1, 14);

				ImGui::SliderFloat("aspect ratio", &g->v.misc.aspect_ratio, 0.f, 2.f);
				ImGui::SliderFloat("world fov", &g->v.misc.fov, 0.f, 180.f);
				ImGui::SliderFloat("viewmodel fov", &g->v.misc.viewmodel_fov, 0.f, 180.f);

				ImGui::Checkbox("hit marker", &g->v.misc.hit_marker);

				ImGui::Hotkey("fake duck", g->v.misc.fakeduck, ImVec2(100, 20));
				if (ImGui::SmallButton("unload"))
					g->unload = true;
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("config")) {
				ImGui::BeginGroup();
				ImGui::InputText("##CFG", g->v.cfg_name, 128);
				ImGui::EndGroup();
				static int sel = 0;
				static int sel2 = 0;
				std::string cfg_str = "";
				std::vector<std::string> default_val = { "-" };
			
				if (config_list.size() > 0) {
					ImGui::BeginGroup();
					ImGui::ListBoxCfg("##cfg_list", &sel, config_list, 5);
					cfg_str = config_list[g->v.cfg_index];
					ImGui::EndGroup();
					g->v.cfg_index = sel;
				}
				else {
					ImGui::BeginGroup();
					ImGui::ListBoxCfg("##empty_cfg_list", &sel2, default_val, 5);
					ImGui::EndGroup();
				}
			
				ImGui::BeginGroup();
				if (config_list.size() >= 1) {
					if (ImGui::Button("save", ImVec2(50, 20)))
					{
						config.save(cfg_str);
					}
					ImGui::SameLine();
				}

				if (config_list.size() > 0) {
					if (ImGui::Button("load", ImVec2(50, 20)))
					{
						config.load(cfg_str);
					}
					ImGui::SameLine();
				}

				if (ImGui::Button("create", ImVec2(50, 20)))
				{
					std::string ConfigFileName = g->v.cfg_name;
					if (ConfigFileName.size() < 1)
					{
						ConfigFileName = "null";
					}
					config.create(ConfigFileName);
					refresh_configs();
				}

				if (config_list.size() > 0) {
					ImGui::SameLine();
					if (ImGui::Button("delete", ImVec2(50, 20)))
					{
						config.erase_cfg(cfg_str);
						refresh_configs();
					}
				}
				ImGui::SameLine();
				if (ImGui::Button("refresh", ImVec2(50, 20)))
				{
					refresh_configs();
				}
				ImGui::EndGroup();
				ImGui::EndTabItem();
			}
			ImGui::EndTabBar();
		}
	}
	ImGui::PopFont();
	ImGui::End(false);
}