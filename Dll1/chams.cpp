#include "chams.h"
#include "key_values.h"
#include "hooks.h"
#include "model_render.h"
#include "studio.h"
#include "entity.h"
#include <fstream>

c_chams* chams = new c_chams();

void c_chams::draw(void* ctx, IMatRenderContext* r_ctx, const DrawModelState_t& state, const ModelRenderInfo_t& info, matrix* mat) {
	static auto draw_model_execute_original = hooks::draw_model_execute_hook.get_original<draw_model_execute_fn>();

	const auto ModelName = info.pModel->name;

	if (strstr(ModelName, "models/player") != nullptr)
	{
		auto entity = interfaces->entity_list->get_client_entity(info.entity_index);

		if (!entity
			|| !g->local
			|| !entity->is_player()
			|| !entity->is_alive()
			|| entity->is_dormant()
			|| entity == g->local
			|| entity->get_team() == g->local->get_team())
			return draw_model_execute_original(ctx, r_ctx, state, info, mat);

		if (g->v.visuals.chams_enemy_xqz) {

			this->material[1]->SetMaterialVarFlag(MATERIAL_VAR_IGNOREZ, true);
			auto var3 = this->material[1]->FindVar("$envmaptint", nullptr);
			var3->set_vec_value(g->v.visuals.color_chams_enemy_xqz[0],
				g->v.visuals.color_chams_enemy_xqz[1], g->v.visuals.color_chams_enemy_xqz[2]);

			interfaces->model_render->ForcedMaterialOverride(this->material[1]);

			draw_model_execute_original(ctx, r_ctx, state, info, mat);
			interfaces->model_render->ForcedMaterialOverride(nullptr);
		}
		if (g->v.visuals.chams_enemy_visible) {
			auto var = this->material[0]->FindVar("$envmaptint", nullptr);
			var->set_vec_value(g->v.visuals.color_chams_enemy_visible[0], 
				g->v.visuals.color_chams_enemy_visible[1], g->v.visuals.color_chams_enemy_visible[2]);

			auto var6 = this->material[0]->FindVar("$alpha", nullptr);
			var6->set_float_value(g->v.visuals.color_chams_enemy_visible[3]);

			interfaces->model_render->ForcedMaterialOverride(this->material[0]);
			draw_model_execute_original(ctx, r_ctx, state, info, mat);
		}
	}
}