#include "render.h"
#include "globals.h"
#include "math.h"
#include "interfaces.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_dx9.h"
#include "imgui/imgui_impl_win32.h"
#include <mutex>

namespace fonts {
	vgui::HFont main;
	vgui::HFont misc;
}

c_render* render = new c_render();

void c_render::init_d3d(IDirect3DDevice9* device)
{
	ImColor mainColor = ImColor(54, 54, 54, 255);
	ImColor bodyColor = ImColor(54, 54, 54, 255);
	ImColor fontColor = ImColor(255, 255, 255, 255);

	ImVec4 mainColorHovered = ImVec4(mainColor.Value.x + 0.1f, mainColor.Value.y + 0.1f, mainColor.Value.z + 0.1f, mainColor.Value.w);
	ImVec4 mainColorActive = ImVec4(mainColor.Value.x + 0.2f, mainColor.Value.y + 0.2f, mainColor.Value.z + 0.2f, mainColor.Value.w);
	ImVec4 menubarColor = ImVec4(bodyColor.Value.x, bodyColor.Value.y, bodyColor.Value.z, bodyColor.Value.w - 0.8f);
	ImVec4 frameBgColor = ImVec4(bodyColor.Value.x, bodyColor.Value.y, bodyColor.Value.z, bodyColor.Value.w + .1f);
	ImVec4 tooltipBgColor = ImVec4(bodyColor.Value.x, bodyColor.Value.y, bodyColor.Value.z, bodyColor.Value.w + .05f);

	ImGuiStyle& style = ImGui::GetStyle();
	style.WindowRounding = 0.f;
	style.FrameRounding = 0.f;
	style.ChildRounding = 0.f;

	style.Colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f); //white
	style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.85f, 0.85f, 0.85f, 0.85f); //main quarter
	style.Colors[ImGuiCol_WindowBg] = ImVec4(0.15f, 0.15f, 0.15f, 0.70f); //main quarter
	style.Colors[ImGuiCol_Border] = ImVec4(0.14f, 0.16f, 0.19f, 0.60f); //main border
	style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f); //dark
	style.Colors[ImGuiCol_FrameBg] = ImVec4(0.07f, 0.07f, 0.07f, 1.00f); //main bg
	style.Colors[ImGuiCol_FrameBgHovered] = ImColor(230, 122, 183); //main colored
	style.Colors[ImGuiCol_FrameBgActive] = ImColor(230, 122, 183); //main colored
	style.Colors[ImGuiCol_TitleBg] = ImVec4(0.11f, 0.11f, 0.11f, 1.00f); //main bg
	style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.20f, 0.24f, 0.28f, 0.75f); //collapsed
	style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.07f, 0.07f, 0.07f, 1.00f); //main bg
	style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.11f, 0.11f, 0.11f, 0.70f); //main bg
	style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.07f, 0.07f, 0.07f, 1.00f); //main bg
	style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f); //main half
	style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.50f, 0.50f, 0.50f, 0.70f); //main half
	style.Colors[ImGuiCol_ScrollbarGrabActive] = ImColor(230, 122, 183); //main colored
	style.Colors[ImGuiCol_CheckMark] = ImColor(230, 122, 183); //main colored
	style.Colors[ImGuiCol_TabHovered] = ImColor(230, 122, 183); //main colored
	style.Colors[ImGuiCol_TabActive] = ImColor(230, 122, 183); //main colored
	style.Colors[ImGuiCol_Tab] = ImColor(196, 104, 156); //main colored
	style.Colors[ImGuiCol_TabUnfocused] = ImColor(196, 104, 156); //main colored
	style.Colors[ImGuiCol_TabUnfocusedActive] = ImColor(196, 104, 156); //main colored
	style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f); //main half
	style.Colors[ImGuiCol_SliderGrabActive] = ImColor(230, 122, 183); //main colored
	style.Colors[ImGuiCol_Button] = ImVec4(0.20f, 0.20f, 0.20f, 1.00f); //main
	style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.30f, 0.30f, 0.30f, 1.00f); //main
	style.Colors[ImGuiCol_ButtonActive] = ImColor(230, 122, 183); //main colored
	style.Colors[ImGuiCol_Header] = ImColor(230, 122, 183); //main colored
	style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.20f, 0.20f, 0.20f, 1.00f); //main
	style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.30f, 0.30f, 0.30f, 1.00f); //main
	style.Colors[ImGuiCol_ResizeGrip] = ImVec4(0.90f, 0.90f, 0.90f, 0.75f); //main white
	style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(1.00f, 1.00f, 1.00f, 0.60f); //main white
	style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(1.00f, 1.00f, 1.00f, 0.90f); //main white
	style.Colors[ImGuiCol_PlotLines] = ImColor(230, 122, 183); //main colored
	style.Colors[ImGuiCol_PlotLinesHovered] = ImColor(230, 122, 183, 150); //main colored
	style.Colors[ImGuiCol_PlotHistogram] = ImColor(230, 122, 183); //main colored
	style.Colors[ImGuiCol_PlotHistogramHovered] = ImColor(230, 122, 183, 150); //main colored
	style.Colors[ImGuiCol_TextSelectedBg] = ImColor(230, 122, 183); //main colored
	style.Colors[ImGuiCol_PopupBg] = ImVec4(0.07f, 0.07f, 0.07f, 0.70f); //main bg
	style.Colors[ImGuiCol_ModalWindowDarkening] = ImVec4(0.20f, 0.24f, 0.28f, 0.60f); //collapsed

	device->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE);
	device->SetRenderState(D3DRS_LIGHTING, false);
	device->SetRenderState(D3DRS_FOGENABLE, false);
	device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	device->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);

	device->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);
	device->SetRenderState(D3DRS_SCISSORTESTENABLE, true);
	device->SetRenderState(D3DRS_ZWRITEENABLE, false);
	device->SetRenderState(D3DRS_STENCILENABLE, false);

	device->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
	device->SetRenderState(D3DRS_ALPHATESTENABLE, false);
	device->SetRenderState(D3DRS_SEPARATEALPHABLENDENABLE, true);
	device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	device->SetRenderState(D3DRS_SRCBLENDALPHA, D3DBLEND_INVDESTALPHA);
	device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	device->SetRenderState(D3DRS_DESTBLENDALPHA, D3DBLEND_ONE);

	device->SetRenderState(D3DRS_SRGBWRITEENABLE, false);
	device->SetRenderState(D3DRS_COLORWRITEENABLE, D3DCOLORWRITEENABLE_RED | D3DCOLORWRITEENABLE_GREEN | D3DCOLORWRITEENABLE_BLUE | D3DCOLORWRITEENABLE_ALPHA);

	device->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
	device->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	device->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
	device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	device->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	device->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);

	device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
}

void c_render::init_fonts() {
	auto should_reload = []() {
		static int old_width, old_height;
		static int width, height;

		interfaces->engine->get_screen_size(width, height);

		if (width != old_width || height != old_height)
		{
			old_width = width;
			old_height = height;

			return true;
		}

		return false;
	};

	static auto create_font = [](const char* name, int size, int weight, DWORD flags) -> vgui::HFont
	{
		auto font = interfaces->surface->font_create();
		interfaces->surface->set_font_glyph_set(font, name, size, weight, NULL, NULL, flags);

		return font;
	};

	if (should_reload()) {
		fonts::main = create_font("Verdana", 12, 400, FONTFLAG_ANTIALIAS | FONTFLAG_DROPSHADOW);
		fonts::misc = create_font("Visitor TT1 BRK", 9, 400, FONTFLAG_OUTLINE);
	}
}

void c_render::set_alphafactor(float factor)
{
	alpha_factor = factor;
}

float c_render::get_alphafactor()
{
	return alpha_factor;
}

void c_render::rect(int x, int y, int w, int h, color color)
{
	if (!interfaces->surface)
		return;

	color.set_alpha(static_cast<int>(color.a() * alpha_factor));

	interfaces->surface->draw_set_color(color);
	interfaces->surface->draw_outlined_rect(x, y, w + x, h + y);
}

void c_render::rect_filled(int x, int y, int w, int h, color color)
{
	if (!interfaces->surface)
		return;

	color.set_alpha(static_cast<int>(color.a() * alpha_factor));

	interfaces->surface->draw_set_color(color);
	interfaces->surface->draw_filled_rect(x, y, x + w, y + h);
}

void c_render::gradient(int x, int y, int w, int h, color first, color second, GradientType type)
{
	if (!interfaces->surface)
		return;

	first.set_alpha(static_cast<int>(first.a() * alpha_factor));
	second.set_alpha(static_cast<int>(second.a() * alpha_factor));

	auto filled_rect_fade = [&](bool reversed, float alpha) {
		using Fn = void(__thiscall*)(VOID*, int, int, int, int, unsigned int, unsigned int, bool);
		getvfunc< Fn >(interfaces->surface, 123) (
			interfaces->surface, x, y, x + w, y + h,
			reversed ? alpha : 0,
			reversed ? 0 : alpha,
			type == GRADIENT_HORIZONTAL);
	};

	static auto blend = [](const color& first, const color& second, float t) -> color {
		return color(
			first.r() + t * (second.r() - first.r()),
			first.g() + t * (second.g() - first.g()),
			first.b() + t * (second.b() - first.b()),
			first.a() + t * (second.a() - first.a()));
	};

	if (first.a() == 255 || second.a() == 255) {
		interfaces->surface->draw_set_color(blend(first, second, 0.5f));
		interfaces->surface->draw_filled_rect(x, y, x + w, y + h);
	}

	interfaces->surface->draw_set_color(first);
	filled_rect_fade(true, first.a());

	interfaces->surface->draw_set_color(second);
	filled_rect_fade(false, second.a());
}

void c_render::rounded_box(int x, int y, int w, int h, int points, int radius, color color)
{
	if (!interfaces->surface)
		return;

	color.set_alpha(static_cast<int>(color.a() * alpha_factor));

	Vertex_t* round = new Vertex_t[4 * points]; //-V121

	for (int i = 0; i < 4; i++) {
		int _x = x + ((i < 2) ? (w - radius) : radius);
		int _y = y + ((i % 3) ? (h - radius) : radius);

		float a = 90.f * i;

		for (int j = 0; j < points; j++) {
			float _a = DEG2RAD(a + (j / (float)(points - 1)) * 90.f);

			round[(i * points) + j] = Vertex_t(vec2(_x + radius * sin(_a), _y - radius * cos(_a)));
		}
	}

	auto surface = interfaces->surface;

	static int Texture = surface->create_new_texture_id(true);
	unsigned char buffer[4] = { 255, 255, 255, 255 };

	surface->draw_set_texture_rgba(Texture, buffer, 1, 1);
	surface->draw_set_color(color);
	surface->draw_set_texture(Texture);

	surface->add_text_poly(4 * points, round);
}

void c_render::grid(int x, int y, int w, int h, color first, color second, color third, GradientType type)
{
	if (!interfaces->surface)
		return;

	first.set_alpha(static_cast<int>(first.a() * alpha_factor));
	second.set_alpha(static_cast<int>(second.a() * alpha_factor));
	third.set_alpha(static_cast<int>(third.a() * alpha_factor));

	auto filled_rect_fade = [&](bool reversed, float alpha) {
		using Fn = void(__thiscall*)(VOID*, int, int, int, int, unsigned int, unsigned int, bool);
		getvfunc< Fn >(interfaces->surface, 123) (
			interfaces->surface, x, y, x + w, y + h,
			reversed ? alpha : 0,
			reversed ? 0 : alpha,
			type == GRADIENT_HORIZONTAL);
	};

	static auto blend = [](const color& first, const color& second, const color& third, float t) -> color {
		return color(
			first.r() + t * (second.r() - third.r()),
			first.g() + t * (second.g() - third.g()),
			first.b() + t * (second.b() - third.b()),
			first.a() + t * (second.a() - third.a()));
	};

	if (first.a() == 255 || second.a() == 255) {
		interfaces->surface->draw_set_text_color(blend(first, second, third, 0.5f));
		interfaces->surface->draw_filled_rect(x, y, x + w, y + h);
	}

	interfaces->surface->draw_set_text_color(first); //-V807
	filled_rect_fade(true, first.a());

	interfaces->surface->draw_set_text_color(second);
	filled_rect_fade(false, second.a());

	interfaces->surface->draw_set_text_color(third);
	filled_rect_fade(false, third.a());
}

void c_render::circle(int x, int y, int points, int radius, color color)
{
	if (!interfaces->surface)
		return;

	color.set_alpha(static_cast<int>(color.a() * alpha_factor));

	static bool once = true;

	static std::vector<float> temppointsx;
	static std::vector<float> temppointsy;

	if (once)
	{
		float step = (float)DirectX::XM_PI * 2.0f / points;
		for (float a = 0; a < (DirectX::XM_PI * 2.0f); a += step) //-V1034
		{
			temppointsx.push_back(cosf(a));
			temppointsy.push_back(sinf(a));
		}
		once = false;
	}

	std::vector<int> pointsx;
	std::vector<int> pointsy;
	std::vector<Vertex_t> vertices;

	for (int i = 0; i < temppointsx.size(); i++)
	{
		float eeks = radius * temppointsx[i] + x;
		float why = radius * temppointsy[i] + y;
		pointsx.push_back(eeks);
		pointsy.push_back(why);

		vertices.emplace_back(Vertex_t(vec2(eeks, why)));
	}

	interfaces->surface->draw_set_text_color(color);
	interfaces->surface->draw_poly_line(pointsx.data(), pointsy.data(), points); // only if you want en extra outline
}

void c_render::circle_filled(int x, int y, int points, int radius, color color)
{
	if (!interfaces->surface)
		return;

	color.set_alpha(static_cast<int>(color.a() * alpha_factor));

	static bool once = true;

	static std::vector<float> temppointsx;
	static std::vector<float> temppointsy;

	if (once)
	{
		float step = (float)DirectX::XM_PI * 2.0f / points;
		for (float a = 0; a < (DirectX::XM_PI * 2.0f); a += step) //-V1034
		{
			temppointsx.push_back(cosf(a));
			temppointsy.push_back(sinf(a));
		}
		once = false;
	}

	std::vector<int> pointsx;
	std::vector<int> pointsy;
	std::vector<Vertex_t> vertices;

	for (int i = 0; i < temppointsx.size(); i++)
	{
		float eeks = radius * temppointsx[i] + x;
		float why = radius * temppointsy[i] + y;
		pointsx.push_back(eeks);
		pointsy.push_back(why);

		vertices.emplace_back(Vertex_t(vec2(eeks, why)));
	}

	interfaces->surface->draw_set_text_color(color);
	interfaces->surface->add_text_poly(points, vertices.data());
}

void c_render::triangle(vec2 point_one, vec2 point_two, vec2 point_three, color color)
{
	if (!interfaces->surface)
		return;

	color.set_alpha(static_cast<int>(color.a() * alpha_factor));

	Vertex_t verts[3] = {
		Vertex_t(point_one),
		Vertex_t(point_two),
		Vertex_t(point_three)
	};

	auto surface = interfaces->surface;

	static int texture = surface->create_new_texture_id(true);
	unsigned char buffer[4] = { 255, 255, 255, 255 };

	surface->draw_set_texture_rgba(texture, buffer, 1, 1);
	surface->draw_set_color(color);
	surface->draw_set_texture(texture);

	surface->add_text_poly(3, verts);
}

void c_render::line(int x, int y, int x2, int y2, color color)
{
	if (!interfaces->surface)
		return;

	color.set_alpha(static_cast<int>(color.a() * alpha_factor));

	interfaces->surface->draw_set_color(color);
	interfaces->surface->draw_line(x, y, x2, y2);
}

void c_render::text(vgui::HFont font, int x, int y, color color, DWORD flags, const char* msg, ...)
{
	if (!interfaces->surface)
		return;

	color.set_alpha(static_cast<int>(color.a() * alpha_factor));

	va_list va_alist;
	char buffer[1024];
	va_start(va_alist, msg);
	_vsnprintf(buffer, sizeof(buffer), msg, va_alist);
	va_end(va_alist);
	wchar_t wbuf[1024];

	MultiByteToWideChar(CP_UTF8, 0, buffer, 256, wbuf, 256);

	auto surface = interfaces->surface;

	int width, height;
	surface->get_text_size(font, wbuf, width, height);

	if (!(flags & HFONT_CENTERED_NONE))
	{
		if (flags & HFONT_CENTERED_X)
			x -= width * 0.5f;

		if (flags & HFONT_CENTERED_Y)
			y -= height * 0.5f;
	}

	surface->draw_set_text_font(font);
	surface->draw_set_text_color(color);
	surface->draw_set_text_pos(x, y);
	surface->draw_print_text(wbuf, wcslen(wbuf));
}

void c_render::wtext(vgui::HFont font, int x, int y, color color, DWORD flags, wchar_t* msg)
{
	if (!interfaces->surface)
		return;

	color.set_alpha(static_cast<int>(color.a() * alpha_factor));

	wchar_t buffer[256];
	va_list args;

	va_start(args, msg);
	wvsprintfW(buffer, msg, args);
	va_end(args);

	auto surface = interfaces->surface;

	int width, height;
	surface->get_text_size(font, buffer, width, height);

	if (!(flags & HFONT_CENTERED_NONE)) {
		if (flags & HFONT_CENTERED_X)
			x -= width * 0.5f;

		if (flags & HFONT_CENTERED_Y)
			y -= height * 0.5f;
	}

	surface->draw_set_text_font(font);
	surface->draw_set_text_color(color);
	surface->draw_set_text_pos(x, y);
	surface->draw_print_text(buffer, wcslen(buffer));
}

int c_render::text_width(vgui::HFont font, const char* msg, ...)
{
	if (!interfaces->surface)
		return 0.0f;

	va_list va_alist;
	char buffer[1024];
	va_start(va_alist, msg);
	_vsnprintf(buffer, sizeof(buffer), msg, va_alist);
	va_end(va_alist);
	wchar_t wbuf[1024];

	MultiByteToWideChar(CP_UTF8, 0, buffer, 256, wbuf, 256);

	int width, height;
	interfaces->surface->get_text_size(font, wbuf, width, height);

	return width;
}

void c_render::DrawFilled3DBox(const vec3& origin, int width, int height, color outline, color filling)
{
	if (!interfaces->surface)
		return;

	float difw = float(width / 2);
	float difh = float(height / 2);

	vec3 boxVectors[8] =
	{
		vec3(origin.x - difw, origin.y - difh, origin.z - difw),
		vec3(origin.x - difw, origin.y - difh, origin.z + difw),
		vec3(origin.x + difw, origin.y - difh, origin.z + difw),
		vec3(origin.x + difw, origin.y - difh, origin.z - difw),
		vec3(origin.x - difw, origin.y + difh, origin.z - difw),
		vec3(origin.x - difw, origin.y + difh, origin.z + difw),
		vec3(origin.x + difw, origin.y + difh, origin.z + difw),
		vec3(origin.x + difw, origin.y + difh, origin.z - difw),
	};

	static vec3 vec0, vec1, vec2, vec33,
		vec4, vec5, vec6, vec7;

	if (math::world_to_screen(boxVectors[0], vec0) &&
		math::world_to_screen(boxVectors[1], vec1) &&
		math::world_to_screen(boxVectors[2], vec2) &&
		math::world_to_screen(boxVectors[3], vec33) &&
		math::world_to_screen(boxVectors[4], vec4) &&
		math::world_to_screen(boxVectors[5], vec5) &&
		math::world_to_screen(boxVectors[6], vec6) &&
		math::world_to_screen(boxVectors[7], vec7))
	{
		vec3 lines[12][2];
		lines[0][0] = vec0;
		lines[0][1] = vec1;
		lines[1][0] = vec1;
		lines[1][1] = vec2;
		lines[2][0] = vec2;
		lines[2][1] = vec33;
		lines[3][0] = vec33;
		lines[3][1] = vec0;

		lines[4][0] = vec4;
		lines[4][1] = vec5;
		lines[5][0] = vec5;
		lines[5][1] = vec6;
		lines[6][0] = vec6;
		lines[6][1] = vec7;
		lines[7][0] = vec7;
		lines[7][1] = vec4;

		lines[8][0] = vec0;
		lines[8][1] = vec4;

		lines[9][0] = vec1;
		lines[9][1] = vec5;

		lines[10][0] = vec2;
		lines[10][1] = vec6;

		lines[11][0] = vec33;
		lines[11][1] = vec7;

		for (int i = 0; i < 12; i++)
			line(lines[i][0].x, lines[i][0].y, lines[i][1].x, lines[i][1].y, outline);
	}
}

void colorConvertHSVtoRGB(float h, float s, float v, float& out_r, float& out_g, float& out_b)
{
	if (!s) //-V550
	{
		out_r = out_g = out_b = v;
		return;
	}

	h = fmod(h, 1.0f) / (60.0f / 360.0f);

	auto i = (int)h;

	auto f = h - (float)i;
	auto p = v * (1.0f - s);
	auto q = v * (1.0f - s * f);
	auto t = v * (1.0f - s * (1.0f - f));

	switch (i)
	{
	case 0:
		out_r = v;
		out_g = t;
		out_b = p;
		break;
	case 1:
		out_r = q;
		out_g = v;
		out_b = p;
		break;
	case 2:
		out_r = p;
		out_g = v;
		out_b = t;
		break;
	case 3:
		out_r = p;
		out_g = q;
		out_b = v;
		break;
	case 4:
		out_r = t;
		out_g = p;
		out_b = v;
		break;
	default:
		out_r = v;
		out_g = p;
		out_b = q;
		break;
	}
}

RECT c_render::viewport()
{
	RECT viewport = { 0, 0, 0, 0 };

	static int width, height;
	interfaces->engine->get_screen_size(width, height);
	viewport.right = width;
	viewport.bottom = height;

	return viewport;
}