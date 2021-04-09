#include "config.h"
#include "globals.h"

c_config config;

void c_config::setup_elements()
{
	g->v.anti_aim.enable = false;
	g->v.anti_aim.anti_aim_on_use = false;
	g->v.anti_aim.jitter = false;
	g->v.anti_aim.freestanding = false;

	g->v.anti_aim.pitch = 0;
	g->v.anti_aim.yaw = 0;
	g->v.anti_aim.desync = 0;

	g->v.anti_aim.left_amount = 60.f;
	g->v.anti_aim.right_amount = 60.f;
	g->v.anti_aim.jitter_amount = 0.f;

	g->v.anti_aim.flip_key->Key = 0;
	g->v.anti_aim.flip_key->Type = Toggle;

	g->v.misc.fake_lag = false;
	g->v.misc.hit_marker = false;
	g->v.misc.meme_walk = false;
	g->v.misc.auto_jump = false;
	g->v.misc.air_strafe = false;

	g->v.misc.aspect_ratio = 0.f;
	g->v.misc.fov = 0.f;
	g->v.misc.viewmodel_fov = 0.f;

	g->v.misc.amount = 1;

	g->v.misc.fakeduck->Key = 0;
	g->v.misc.fakeduck->Type = Hold;

	g->v.visuals.enable = false;
	g->v.visuals.remove_scope = false;
	g->v.visuals.name = false;
	g->v.visuals.box = false;
	g->v.visuals.weapon = false;
	g->v.visuals.health_bar = false;
	g->v.visuals.skeleton = false;

	g->v.visuals.hit_model = false;
	g->v.visuals.hit_skeleton = false;

	g->v.visuals.glow_enemy = false;
	g->v.visuals.glow_team = false;
	g->v.visuals.glow_local = false;

	g->v.visuals.chams_enemy_xqz = false;
	g->v.visuals.chams_enemy_visible = false;

	g->v.visuals.color_chams_enemy_visible[0] = 1.f;
	g->v.visuals.color_chams_enemy_visible[1] = 0.6f;
	g->v.visuals.color_chams_enemy_visible[2] = 0.2f;
	g->v.visuals.color_chams_enemy_visible[3] = 0.6f;

	g->v.visuals.color_chams_enemy_xqz[0] = 1.f;
	g->v.visuals.color_chams_enemy_xqz[1] = 0.6f;
	g->v.visuals.color_chams_enemy_xqz[2] = 0.2f;
	g->v.visuals.color_chams_enemy_xqz[3] = 0.6f;

	g->v.visuals.color_glow_enemy[0] = 1.f;
	g->v.visuals.color_glow_enemy[1] = 0.5f;
	g->v.visuals.color_glow_enemy[2] = 1.f;
	g->v.visuals.color_glow_enemy[3] = 0.6f;

	g->v.visuals.color_glow_team[0] = 1.f;
	g->v.visuals.color_glow_team[1] = 0.5f;
	g->v.visuals.color_glow_team[2] = 1.f;
	g->v.visuals.color_glow_team[3] = 0.6f;

	g->v.visuals.color_glow_local[0] = 0.f;
	g->v.visuals.color_glow_local[1] = 0.85f;
	g->v.visuals.color_glow_local[2] = 0.85f;
	g->v.visuals.color_glow_local[3] = 0.6f;

	g->v.visuals.color_box[0] = 1.f;
	g->v.visuals.color_box[1] = 0.5f;
	g->v.visuals.color_box[2] = 1.f;
	g->v.visuals.color_box[3] = 1.f;

	g->v.visuals.color_name[0] = 1.f;
	g->v.visuals.color_name[1] = 1.f;
	g->v.visuals.color_name[2] = 1.f;
	g->v.visuals.color_name[3] = 1.f;

	g->v.visuals.color_weapon[0] = 1.f;
	g->v.visuals.color_weapon[1] = 1.f;
	g->v.visuals.color_weapon[2] = 1.f;
	g->v.visuals.color_weapon[3] = 1.f;

	g->v.visuals.color_hp[0] = 0.f;
	g->v.visuals.color_hp[1] = 1.f;
	g->v.visuals.color_hp[2] = 0.f;
	g->v.visuals.color_hp[3] = 1.f;

	g->v.visuals.color_skeleton[0] = 1.f;
	g->v.visuals.color_skeleton[1] = 1.f;
	g->v.visuals.color_skeleton[2] = 1.f;
	g->v.visuals.color_skeleton[3] = 1.f;
}

void c_config::create(const std::string& config_name) {

	static TCHAR path[256];
	std::string folder, file;

	if (SUCCEEDED(SHGetFolderPath(NULL, CSIDL_PERSONAL, NULL, 0, path)))
	{
		folder = std::string(path) + "\\csgo\\";
		file = std::string(path) + "\\csgo\\" + config_name + ".json";
	}

	CreateDirectory(folder.c_str(), NULL);

	std::ofstream file_out(file);
	file_out.close();
}

void c_config::erase_cfg(const std::string& config_name) {

	static TCHAR path[256];
	std::string folder, file;

	if (SUCCEEDED(SHGetFolderPath(NULL, CSIDL_PERSONAL, NULL, 0, path)))
	{
		folder = std::string(path) + "\\csgo\\";
		file = std::string(path) + "\\csgo\\" + config_name;
	}

	CreateDirectory(folder.c_str(), NULL);

	remove(file.c_str());
}

void c_config::save(const std::string& config_name)
{
	json json_obj;

	auto& hvh = json_obj["hvh"]; {
		auto& anti_aim = hvh["anti_aim"]; {
			save_bool(anti_aim, "enable", g->v.anti_aim.enable);
			save_bool(anti_aim, "anti_aim_on_use", g->v.anti_aim.anti_aim_on_use);
			save_int(anti_aim, "pitch", g->v.anti_aim.pitch);
			save_int(anti_aim, "yaw", g->v.anti_aim.yaw);
			save_int(anti_aim, "desync", g->v.anti_aim.desync);
			save_float(anti_aim, "left_amount", g->v.anti_aim.left_amount);
			save_float(anti_aim, "right_amount", g->v.anti_aim.right_amount);
			save_bind(anti_aim, "inverter", g->v.anti_aim.flip_key);
			save_bool(anti_aim, "jitter", g->v.anti_aim.jitter);
			save_float(anti_aim, "jitter_amount", g->v.anti_aim.jitter_amount);
			save_bool(anti_aim, "freestanding", g->v.anti_aim.freestanding);
		}
	}

	auto& misc = json_obj["misc"]; {
		auto& fake_lag = misc["fake_lag"]; {
			save_bool(fake_lag, "enable", g->v.misc.fake_lag);
			save_int(fake_lag, "amount", g->v.misc.amount);
		}
		save_bind(misc, "fake_duck", g->v.misc.fakeduck);
		save_bool(misc, "meme_walk", g->v.misc.meme_walk);
		save_bool(misc, "auto_jump", g->v.misc.auto_jump);
		save_bool(misc, "air_strafe", g->v.misc.air_strafe);
		save_float(misc, "aspect_ratio", g->v.misc.aspect_ratio);
		save_float(misc, "fov", g->v.misc.fov);
		save_bool(misc, "hit_marker", g->v.misc.hit_marker);
		save_float(misc, "viewmodel_fov", g->v.misc.viewmodel_fov);
	}

	auto& visuals = json_obj["visuals"]; {
		auto& player = visuals["player"]; {
			save_bool(player, "enable", g->v.visuals.enable);
			save_bool(player, "name", g->v.visuals.name);
			save_bool(player, "box", g->v.visuals.box);
			save_bool(player, "weapon", g->v.visuals.weapon);
			save_bool(player, "health_bar", g->v.visuals.health_bar);
			save_bool(player, "skeleton", g->v.visuals.skeleton);
			save_bool(player, "hit_model", g->v.visuals.hit_model);
			save_bool(player, "hit_skeleton", g->v.visuals.hit_skeleton);
			save_bool(player, "flags", g->v.visuals.flags);

			auto& glow = player["glow"]; {
				save_bool(glow, "glow_enemy", g->v.visuals.glow_enemy);
				save_bool(glow, "glow_team", g->v.visuals.glow_team);
				save_bool(glow, "glow_local", g->v.visuals.glow_local);
			}
			auto& chams = player["chams"]; {
				save_bool(chams, "enemy_visible", g->v.visuals.chams_enemy_visible);
				save_bool(chams, "enemy_xqz", g->v.visuals.chams_enemy_xqz);
			}
		}

		auto& other = visuals["other"]; {
			save_bool(other, "remove_scope", g->v.visuals.remove_scope);
		}

		auto& colors = json_obj["colors"]; {
			save_clr(colors, "chams_enemy_visible", g->v.visuals.color_chams_enemy_visible);
			save_clr(colors, "chams_enemy_xqz", g->v.visuals.color_chams_enemy_xqz);
			save_clr(colors, "glow_enemy", g->v.visuals.color_glow_enemy);
			save_clr(colors, "glow_team", g->v.visuals.color_glow_team);
			save_clr(colors, "glow_local", g->v.visuals.color_glow_local);
			save_clr(colors, "box", g->v.visuals.color_box);
			save_clr(colors, "name", g->v.visuals.color_name);
			save_clr(colors, "weapon", g->v.visuals.color_weapon);
			save_clr(colors, "hp", g->v.visuals.color_hp);
			save_clr(colors, "skeleton", g->v.visuals.color_skeleton);
		}
	}

	static TCHAR path[256];
	std::string folder, file;

	if (SUCCEEDED(SHGetFolderPath(NULL, CSIDL_PERSONAL, NULL, 0, path)))
	{
		folder = std::string(path) + "\\csgo\\";
		file = std::string(path) + "\\csgo\\" + config_name;
	}

	CreateDirectory(folder.c_str(), NULL);

	auto str = json_obj.toStyledString();
	std::ofstream file_out(file);
	if (file_out.good())
		file_out << str;
	file_out.close();
}

void c_config::load(const std::string& config_name)
{
	static TCHAR path[256];
	std::string folder, file;

	if (SUCCEEDED(SHGetFolderPath(NULL, CSIDL_PERSONAL, NULL, 0, path)))
	{
		folder = std::string(path) + "\\csgo\\";
		file = std::string(path) + "\\csgo\\" + config_name;
	}

	CreateDirectory(folder.c_str(), NULL);

	std::ifstream input_file = std::ifstream(file);

	if (!input_file.good())
		return;

	json json_obj;
	input_file >> json_obj;

	auto& hvh = json_obj["hvh"]; {
		auto& anti_aim = hvh["anti_aim"]; {
			load_bool(anti_aim, "enable", g->v.anti_aim.enable);
			load_bool(anti_aim, "anti_aim_on_use", g->v.anti_aim.anti_aim_on_use);
			load_int(anti_aim, "pitch", g->v.anti_aim.pitch);
			load_int(anti_aim, "yaw", g->v.anti_aim.yaw);
			load_int(anti_aim, "desync", g->v.anti_aim.desync);
			load_float(anti_aim, "left_amount", g->v.anti_aim.left_amount);
			load_float(anti_aim, "right_amount", g->v.anti_aim.right_amount);
			load_bind(anti_aim, "inverter", g->v.anti_aim.flip_key);
			load_bool(anti_aim, "jitter", g->v.anti_aim.jitter);
			load_float(anti_aim, "jitter_amount", g->v.anti_aim.jitter_amount);
			load_bool(anti_aim, "freestanding", g->v.anti_aim.freestanding);
		}
	}

	auto& misc = json_obj["misc"]; {
		auto& fake_lag = misc["fake_lag"]; {
			load_bool(fake_lag, "enable", g->v.misc.fake_lag);
			load_int(fake_lag, "amount", g->v.misc.amount);
		}
		load_bind(misc, "fake_duck", g->v.misc.fakeduck);
		load_bool(misc, "meme_walk", g->v.misc.meme_walk);
		load_bool(misc, "auto_jump", g->v.misc.auto_jump);
		load_bool(misc, "air_strafe", g->v.misc.air_strafe);
		load_float(misc, "aspect_ratio", g->v.misc.aspect_ratio);
		load_float(misc, "fov", g->v.misc.fov);
		load_bool(misc, "hit_marker", g->v.misc.hit_marker);
		load_float(misc, "viewmodel_fov", g->v.misc.viewmodel_fov);
	}

	auto& visuals = json_obj["visuals"]; {
		auto& player = visuals["player"]; {
			load_bool(player, "enable", g->v.visuals.enable);
			load_bool(player, "name", g->v.visuals.name);
			load_bool(player, "box", g->v.visuals.box);
			load_bool(player, "weapon", g->v.visuals.weapon);
			load_bool(player, "health_bar", g->v.visuals.health_bar);
			load_bool(player, "skeleton", g->v.visuals.skeleton);
			load_bool(player, "hit_model", g->v.visuals.hit_model);
			load_bool(player, "hit_skeleton", g->v.visuals.hit_skeleton);
			load_bool(player, "flags", g->v.visuals.flags);

			auto& glow = player["glow"]; {
				load_bool(glow, "glow_enemy", g->v.visuals.glow_enemy);
				load_bool(glow, "glow_team", g->v.visuals.glow_team);
				load_bool(glow, "glow_local", g->v.visuals.glow_local);
			}

			auto& chams = player["chams"]; {
				load_bool(chams, "enemy_visible", g->v.visuals.chams_enemy_visible);
				load_bool(chams, "enemy_xqz", g->v.visuals.chams_enemy_xqz);
			}
		}

		auto& other = visuals["other"]; {
			load_bool(other, "remove_scope", g->v.visuals.remove_scope);
		}
		auto& colors = json_obj["colors"]; {
			load_clr(colors, "chams_enemy_visible", g->v.visuals.color_chams_enemy_visible);
			load_clr(colors, "chams_enemy_xqz", g->v.visuals.color_chams_enemy_xqz);
			load_clr(colors, "glow_enemy", g->v.visuals.color_glow_enemy);
			load_clr(colors, "glow_team", g->v.visuals.color_glow_team);
			load_clr(colors, "glow_local", g->v.visuals.color_glow_local);
			load_clr(colors, "box", g->v.visuals.color_box);
			load_clr(colors, "name", g->v.visuals.color_name);
			load_clr(colors, "weapon", g->v.visuals.color_weapon);
			load_clr(colors, "hp", g->v.visuals.color_hp);
			load_clr(colors, "skeleton", g->v.visuals.color_skeleton);
		}
	}

	input_file.close();
}
