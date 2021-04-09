#include "hooks.h"
#include "globals.h"
#include "interfaces.h"
#include "visuals.h"

void __fastcall hooks::hooked_paint_traverse(void* ecx, void* edx, unsigned int vguiPanel, bool forceRepaint, bool allowForce)
{
	static auto paint_traverse_original = hooks::paint_traverse_hook.get_original<paint_traverse_fn>();

	if (g->unload) {
		interfaces->panel->set_mouse_input_enabled(vguiPanel, 0);
		return paint_traverse_original(ecx, vguiPanel, forceRepaint, allowForce);
	}

	auto panel_name = interfaces->panel->get_name(vguiPanel);
	if (g->v.visuals.remove_scope && !strcmp(panel_name, "HudZoom"))
		return;

	if (!strcmp(panel_name, "FocusOverlayPanel"))
		interfaces->panel->set_mouse_input_enabled(vguiPanel, g->gui_opened);

	paint_traverse_original(ecx, vguiPanel, forceRepaint, allowForce);

	if (!strcmp(panel_name, "MatSystemTopPanel")) {
		render->init_fonts();

		static c_cvar* aspect_ratio = interfaces->cvar->find_var("r_aspectratio");
		aspect_ratio->m_fnChangeCallbacks.m_Size = 0;
		aspect_ratio->set_value(g->v.misc.aspect_ratio);

		interfaces->engine->get_screen_size(g->screen.w, g->screen.h);

		visuals->main();
	}
}