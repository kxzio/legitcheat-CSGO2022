#pragma once
#include <d3d9.h>
#include <d3dx9.h>
#include <vector>
#include "vector2.h"
#include "vector3.h"
#include "surface.h"
#include "imgui/imgui.h"
#include "imgui/imgui_internal.h"

#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")

enum FontCenteringFlags
{
	HFONT_CENTERED_NONE = (1 << 0),
	HFONT_CENTERED_X = (1 << 1),
	HFONT_CENTERED_Y = (1 << 2)
};

enum GradientType
{
	GRADIENT_HORIZONTAL,
	GRADIENT_VERTICAL
};

struct CUSTOMVERTEX2 {
	FLOAT x, y, z;
	FLOAT rhw;
	DWORD color;
};

namespace fonts {
	extern vgui::HFont main;
	extern vgui::HFont misc;
}

class c_render
{
private:
	float alpha_factor = 1.0f;
public:
	void init_d3d(IDirect3DDevice9* device);
	void init_fonts();
	float get_alphafactor();
	void set_alphafactor(float factor);
	void rect(int x, int y, int w, int h, color color);
	void rect_filled(int x, int y, int w, int h, color color);
	void gradient(int x, int y, int w, int h, color first, color second, GradientType type);
	void rounded_box(int x, int y, int w, int h, int points, int radius, color color);
	void grid(int x, int y, int w, int h, color first, color second, color third, GradientType type);
	void circle(int x, int y, int points, int radius, color color);
	void circle_filled(int x, int y, int points, int radius, color color);
	void triangle(vec2 point_one, vec2 point_two, vec2 point_three, color color);
	void line(int x, int y, int x2, int y2, color color);
	void text(vgui::HFont font, int x, int y, color color, DWORD flags, const char* msg, ...);
	void wtext(vgui::HFont font, int x, int y, color color, DWORD flags, wchar_t* msg);
	int text_width(vgui::HFont font, const char* msg, ...);
	void DrawFilled3DBox(const vec3& origin, int width, int height, color outline, color filling);
	RECT viewport();
};

extern c_render* render;