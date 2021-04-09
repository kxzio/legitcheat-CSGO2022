#include "hooks.h"
#include "interfaces.h"
#include "key_values.h"
#include "studio.h"
#include "chams.h"
#include "globals.h"
#include <fstream>

void __stdcall hooks::hooked_draw_model_execute(IMatRenderContext* ctx, 
	const DrawModelState_t& state, const ModelRenderInfo_t& info, matrix* bone_to_world)
{
	static auto draw_model_execute_original = hooks::draw_model_execute_hook.get_original<draw_model_execute_fn>();

	auto name = info.pModel->name;

	bool is_weapon = strstr(name, "weapons/v_") && !strstr(name, "arms");
	bool is_arms = strstr(name, "arms");
	bool is_sleeve = strstr(name, "sleeve");

	static bool fl = false;
	if (!fl)
	{
		std::ofstream("csgo\\materials\\glow_a.vmt") << R"#("VertexLitGeneric" {
			"$additive" "1"
			"$envmap" "models/effects/cube_white"
			"$envmaptint" "[0.1 1 0.1]"
			"$envmapfresnel" "1"
			"$envmapfresnelminmaxexp" "[0 1 2]"
			"$alpha" "1"
			"$halfambert" "1"
		})#";
		std::ofstream("csgo\\materials\\glow_b.vmt") << R"#("VertexLitGeneric" {
			"$additive" "1"
			"#ignorez" "1"
			"$envmap" "models/effects/cube_white"
			"$envmaptint" "[0.1 1 0.1]"
			"$envmapfresnel" "1"
			"$envmapfresnelminmaxexp" "[0 1 2]"
			"$alpha" "1"
			"$halfambert" "1"
		})#";
		std::ofstream("csgo\\materials\\glow_c.vmt") << R"#("VertexLitGeneric" {
			"$additive" "1"
			"#ignorez" "1"
			"$envmap" "models/effects/cube_white"
			"$envmaptint" "[0.1 1 0.1]"
			"$envmapfresnel" "1"
			"$envmapfresnelminmaxexp" "[0 1 2]"
			"$alpha" "0.6"
			"$halfambert" "0"
		})#";
		fl = true;
	}

	chams->material[0] = interfaces->material_system->FindMaterial("glow_a", TEXTURE_GROUP_OTHER);
	chams->material[1] = interfaces->material_system->FindMaterial("glow_b", TEXTURE_GROUP_OTHER);
	chams->material[2] = interfaces->material_system->FindMaterial("glow_c", TEXTURE_GROUP_OTHER);

	if (interfaces->model_render->IsForcedMaterialOverride() && !is_weapon && !is_arms && !is_sleeve)
		return draw_model_execute_original(interfaces->model_render, ctx, state, info, bone_to_world);

	chams->draw(interfaces->model_render, ctx, state, info, bone_to_world);
	draw_model_execute_original(interfaces->model_render, ctx, state, info, bone_to_world);
	interfaces->model_render->ForcedMaterialOverride(nullptr);
}