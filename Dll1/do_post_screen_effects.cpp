#include "hooks.h"
#include "globals.h"
#include "visuals.h"
#include "entity.h"
#include "chams.h"
#include "interfaces.h"

void draw_chams_on_shot() {
	if (!g->v.visuals.enable)
		return;

	if (!g->v.visuals.hit_model)
		return;

	auto ctx = interfaces->material_system->GetRenderContext();

	if (!ctx)
		return;

	for (int x = 0; x < 65; x++) {
		auto& i = visuals->skeleton_info[x];
		for (int s = 0; s < i.size(); s++) {
			auto skelet = i[s];

			float expire_time = interfaces->global_vars->curtime - skelet->time;
			if (expire_time > 2.f)
				skelet->alpha -= 1.f;

			skelet->alpha = std::clamp(skelet->alpha, 0.f, 255.f);

			if (skelet->alpha <= 0)
				i.erase(i.begin() + s);

			chams->material[2]->SetMaterialVarFlag(MATERIAL_VAR_IGNOREZ, true);

			auto var3 = chams->material[2]->FindVar("$envmaptint", nullptr);
			var3->set_vec_value(skelet->clr.r() / 255.f, skelet->clr.g() / 255.f, skelet->clr.b() / 255.f);

			auto var2 = chams->material[2]->FindVar("$color", nullptr);
			var2->set_vec_value(skelet->clr.r() / 255.f, skelet->clr.g() / 255.f, skelet->clr.b() / 255.f);

			auto var5 = chams->material[2]->FindVar("$alpha", nullptr);
			var5->set_float_value(skelet->alpha / 255.f);

			interfaces->model_render->ForcedMaterialOverride(chams->material[2]);
			interfaces->model_render->DrawModelExecute(ctx, skelet->state, skelet->render_info, skelet->bones);
			interfaces->model_render->ForcedMaterialOverride(nullptr);
		}
	}
}

void __fastcall hooks::hooked_do_post_screen_effects(void* ecx, void* edx, c_view_setup* setup)
{
	static auto do_post_screen_effects_original = hooks::post_screen_effects_hook.get_original<do_post_screen_effects_fn>();

	if (interfaces->engine->is_connected() && interfaces->engine->is_in_game()) {

		draw_chams_on_shot();

		for (auto i = 0; i < interfaces->glow_manager->size; i++) //-V807
		{
			if (interfaces->glow_manager->m_GlowObjectDefinitions[i].IsUnused())
				continue;

			auto object = &interfaces->glow_manager->m_GlowObjectDefinitions[i];
			auto entity = object->m_pEntity;

			if (!entity)
				continue;

			auto client_class = entity->GetClientClass();

			if (!client_class)
				continue;

			if (entity->is_player())
			{
				auto e = (c_entity*)entity;
				auto should_glow = false;

				if (!e || !g->local || !e->is_player() || !e->is_alive() || e->is_dormant())
					continue;

				if (e == g->local) {
					if (g->v.visuals.glow_local &&
						interfaces->input->m_fCameraInThirdPerson) {
						object->m_vGlowcolor_t = vec3(
							g->v.visuals.color_glow_local[0],
							g->v.visuals.color_glow_local[1],
							g->v.visuals.color_glow_local[2]
						);
						object->m_flGlowAlpha = g->v.visuals.color_glow_local[3];
						object->m_bRenderWhenOccluded = true;
						object->m_bRenderWhenUnoccluded = false;
						object->m_bFullBloomRender = false;
					}
					else continue;
				}
				else if (e->get_team() == g->local->get_team()) {
					if (g->v.visuals.glow_team) {
						object->m_vGlowcolor_t = vec3(
							g->v.visuals.color_glow_team[0],
							g->v.visuals.color_glow_team[1],
							g->v.visuals.color_glow_team[2]
						);
						object->m_flGlowAlpha = g->v.visuals.color_glow_team[3];
						object->m_bRenderWhenOccluded = true;
						object->m_bRenderWhenUnoccluded = false;
						object->m_bFullBloomRender = false;
					}
					else continue;
				}
				else
				{
					if (g->v.visuals.glow_enemy) {
						object->m_vGlowcolor_t = vec3(
							g->v.visuals.color_glow_enemy[0],
							g->v.visuals.color_glow_enemy[1],
							g->v.visuals.color_glow_enemy[2]
						);
						object->m_flGlowAlpha = g->v.visuals.color_glow_enemy[3];
						object->m_bRenderWhenOccluded = true;
						object->m_bRenderWhenUnoccluded = false;
						object->m_bFullBloomRender = false;
					}
					else continue;
				}

				/*	auto type = 0;

					if (e == g->local)
						type = 1;
					else if (e->get_team() == g->local->get_team())
						type = 2;*/

						/*	float color[4];

							if (g->gui.visuals.glow_enemy && type == 0)
							{
								should_glow = true;

								color[0] = g->gui.visuals.color_glow_enemy[0];
								color[1] = g->gui.visuals.color_glow_enemy[1];
								color[2] = g->gui.visuals.color_glow_enemy[2];
								color[3] = g->gui.visuals.color_glow_enemy[3];
							}
							else if (g->gui.visuals.glow_team && type == 1)
							{
								should_glow = true;

								color[0] = g->gui.visuals.color_glow_team[0];
								color[1] = g->gui.visuals.color_glow_team[1];
								color[2] = g->gui.visuals.color_glow_team[2];
								color[3] = g->gui.visuals.color_glow_team[3];
							}
							else if (g->gui.visuals.glow_local && type == 2)
							{
								should_glow = true;

								color[0] = g->gui.visuals.color_glow_local[0];
								color[1] = g->gui.visuals.color_glow_local[1];
								color[2] = g->gui.visuals.color_glow_local[2];
								color[3] = g->gui.visuals.color_glow_local[3];
							}

							if (!should_glow)
								continue;

							object->m_vGlowcolor_t = vec3(
								color[0],
								color[1],
								color[2]
							);
							object->m_flGlowAlpha = color[3];
							object->m_bRenderWhenOccluded = true;
							object->m_bRenderWhenUnoccluded = false;
							object->m_bFullBloomRender = false;*/
			}
		}
	}

	do_post_screen_effects_original(ecx, setup);
}