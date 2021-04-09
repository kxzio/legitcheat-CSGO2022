#include "hooks.h"
#include "globals.h"
#include "render.h"
#include "visuals.h"
#include "misc.h"
#include "interfaces.h"
#include "imgui/imgui.h"
#include "imgui/imgui_freetype.h"
#include "imgui/imgui_impl_dx9.h"
#include "imgui/imgui_impl_win32.h"

HRESULT __stdcall hooks::hooked_reset(IDirect3DDevice9* device, D3DPRESENT_PARAMETERS* params)
{
	static auto reset_original = hooks::reset_hook.get_original<reset_fn>();

	if (g->unload)
		return reset_original(device, params);

	ImGui_ImplDX9_InvalidateDeviceObjects();

	long hr = reset_original(device, params);
	if (hr >= 0) {
		ImGui_ImplDX9_CreateDeviceObjects();
	}
	return hr;
}

HRESULT __stdcall hooks::hooked_present(IDirect3DDevice9* device,
	const RECT* source_rect, const RECT* dest_rect, HWND window_override, const RGNDATA* dirty_region)
{
	static auto present_original = hooks::present_hook.get_original<present_fn>();

	if (g->unload)
		return present_original(device, source_rect, dest_rect, window_override, dirty_region);

	IDirect3DStateBlock9* stateBlock = nullptr;
	IDirect3DVertexDeclaration9* vertDec = nullptr;

	device->GetVertexDeclaration(&vertDec);
	device->CreateStateBlock(D3DSBT_PIXELSTATE, &stateBlock);

	if (!g->init_d3d)
	{
		ImGui::CreateContext();
		ImGui_ImplWin32_Init(g->window);
		ImGui_ImplDX9_Init(device);

		render->init_d3d(device);

		ImFontConfig cfg;
		ImGuiIO& io = ImGui::GetIO();
		g->fonts.menu = io.Fonts->AddFontFromFileTTF("C:/windows/fonts/segoeui.ttf", 16, &cfg, io.Fonts->GetGlyphRangesCyrillic());
		g->fonts.main = io.Fonts->AddFontFromFileTTF("C:/windows/fonts/tahoma.ttf", 14, &cfg, io.Fonts->GetGlyphRangesCyrillic());
		g->fonts.misc = io.Fonts->AddFontFromFileTTF("C:/windows/fonts/verdanab.ttf", 12, &cfg, io.FontsAdvanced->GetGlyphRangesCyrillic());

		ImGuiFreeType::BuildFontAtlas(io.Fonts, ImGuiFreeType::RasterizerFlags::LightHinting);

		ImGui_ImplDX9_CreateDeviceObjects();
		g->init_d3d = true;
	}

	static void* dwReturnAddress = _ReturnAddress();

	if (dwReturnAddress == _ReturnAddress())
	{
		ImGui_ImplDX9_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();

		misc->do_menu();

		ImGui::EndFrame();
		ImGui::Render();
		ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
	}

	stateBlock->Apply();
	stateBlock->Release();
	device->SetVertexDeclaration(vertDec);

	return present_original(device, source_rect, dest_rect, window_override, dirty_region);
}