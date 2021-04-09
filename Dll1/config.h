#pragma once
#include <string>
#include "json/json.h"
#include <sstream>
#include <ostream>
#include <fstream>
#include <ShlObj_core.h>
#include "imgui/imgui.h"

typedef Json::Value json;

class c_config
{
private:
	void save_bind(json& j, std::string name, ImGuiBind_t* b) {
		if (b == nullptr)
			return;

		j[name]["key"] = b->Key;
		j[name]["type"] = b->Type;
	}

	void save_float(json& j, std::string name, float v)
	{
		j[name] = v;
	}

	void save_int(json& j, std::string name, int v)
	{
		j[name] = v;
	}

	void save_bool(json& j, std::string name, bool v)
	{
		j[name] = v;
	}

	void save_clr(json& j, std::string name, float* v)
	{
		j[name]["r"] = v[0];
		j[name]["g"] = v[1];
		j[name]["b"] = v[2];
		j[name]["a"] = v[3];
	}

	void load_bind(const json& j, std::string name, ImGuiBind_t* b)
	{
		if (b == nullptr)
			return;

		b->Key = j[name]["key"].asInt();
		b->Type = j[name]["type"].asInt();
	}

	void load_clr(const json& j, std::string name, float* v)
	{
		if (j[name].empty())
			return;

		v[0] = j[name]["r"].asFloat();
		v[1] = j[name]["g"].asFloat();
		v[2] = j[name]["b"].asFloat();
		v[3] = j[name]["a"].asFloat();
	}

	void load_int(const json& j, std::string name, int& v)
	{
		if (j[name].empty())
			return;

		v = j[name].asInt();
	}

	void load_bool(const json& j, std::string name, bool& v)
	{
		if (j[name].empty())
			return;

		v = j[name].asBool();
	}

	void load_float(const json& j, std::string name, float& v)
	{
		if (j[name].empty())
			return;

		v = j[name].asFloat();
	}

public:
	void setup_elements();
	void create(const std::string& config_name);
	void erase_cfg(const std::string& config_name);
	void save(const std::string& config_name);
	void load(const std::string& config_name);
};
extern c_config config;