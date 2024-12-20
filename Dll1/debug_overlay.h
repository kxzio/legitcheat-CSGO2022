#pragma once
#include "vector3.h"
#include "utils.h"
class OverlayText_t;
class color;

class c_debug_overlay
{
public:
	virtual void add_entity_text_overlay(int ent_index, int line_offset, float duration, int r, int g, int b, int a, const char* format, ...) = 0;
	virtual void add_box_overlay(const vec3& origin, const vec3& mins, const vec3& max, vec3 const& orientation, int r, int g, int b, int a, float duration) = 0;
	virtual void add_sphere_overlay(const vec3& vOrigin, float flRadius, int nTheta, int nPhi, int r, int g, int b, int a, float flDuration) = 0;
	virtual void add_triangle_overlay(const vec3& p1, const vec3& p2, const vec3& p3, int r, int g, int b, int a, bool noDepthTest, float duration) = 0;
	virtual void add_line_overlay(const vec3& origin, const vec3& dest, int r, int g, int b, bool noDepthTest, float duration) = 0;
	virtual void add_text_overlay(const vec3& origin, float duration, const char* format, ...) = 0;
	virtual void add_text_overlay(const vec3& origin, int line_offset, float duration, const char* format, ...) = 0;
	virtual void add_screen_text_overlay(float flXPos, float flYPos, float flDuration, int r, int g, int b, int a, const char* text) = 0;
	virtual void add_swept_box_overlay(const vec3& start, const vec3& end, const vec3& mins, const vec3& max, const vec3& angles, int r, int g, int b, int a, float flDuration) = 0;
	virtual void add_grid_overlay(const vec3& origin) = 0;
	virtual void add_coord_flame_overlay(const matrix& frame, float flScale, int vColorTable[3][3] = NULL) = 0;
	virtual int screen_position(const vec3& point, vec3& screen) = 0;
	virtual int screen_position(float flXPos, float flYPos, vec3& screen) = 0;
	virtual OverlayText_t* get_first(void) = 0;
	virtual OverlayText_t* get_next(OverlayText_t* current) = 0;
	virtual void clear_dead_overlays(void) = 0;
	virtual void clear_all_overlays(void) = 0;
	virtual void add_text_overlay_rgb(const vec3& origin, int line_offset, float duration, float r, float g, float b, float alpha, const char* format, ...) = 0;
	virtual void add_text_overlay_rgb(const vec3& origin, int line_offset, float duration, int r, int g, int b, int a, const char* format, ...) = 0;
	virtual void add_line_overlay_alpha(const vec3& origin, const vec3& dest, int r, int g, int b, int a, bool noDepthTest, float duration) = 0;
	virtual void add_box_overlay_alt(const vec3& origin, const vec3& mins, const vec3& max, vec3 const& orientation, const color faceColor, const color edgeColor, float duration) = 0;
	virtual void add_line_overlay(const vec3& origin, const vec3& dest, int r, int g, int b, int a, float, float) = 0;
	virtual void purge_text_overlays() = 0;
	virtual void add_capsule_overlay(const vec3& mins, const vec3& max, float& radius, int r, int g, int b, int a, float duration, char unknown, char ignorez) = 0;

	void box_overlay(const vec3& origin, const vec3& mins, const vec3& max, vec3 const& orientation, int r, int g, int b, int a, float duration)
	{
		using Fn = void(__thiscall*)(void*, const vec3&, const vec3&, const vec3&, vec3 const&, int, int, int, int, float);
		getvfunc<Fn>(this, 1)(this, origin, mins, max, orientation, r, g, b, a, duration);
	}
};