#pragma once
#include "vector2.h"
#include "app_system.h"
#include "cvar.h"
#include "imgui/imgui.h"
#include <math.h>
#include <algorithm>

namespace vgui {
	typedef unsigned long HFont;
	typedef unsigned int VPANEL;
};

enum FontFeature {
	FONT_FEATURE_ANTIALIASED_FONTS = 1,
	FONT_FEATURE_DROPSHADOW_FONTS = 2,
	FONT_FEATURE_OUTLINE_FONTS = 6,
};

enum FontDrawType {
	FONT_DRAW_DEFAULT = 0,
	FONT_DRAW_NONADDITIVE,
	FONT_DRAW_ADDITIVE,
	FONT_DRAW_TYPE_COUNT = 2,
};

enum FontFlags {
	FONTFLAG_NONE,
	FONTFLAG_ITALIC = 0x001,
	FONTFLAG_UNDERLINE = 0x002,
	FONTFLAG_STRIKEOUT = 0x004,
	FONTFLAG_SYMBOL = 0x008,
	FONTFLAG_ANTIALIAS = 0x010,
	FONTFLAG_GAUSSIANBLUR = 0x020,
	FONTFLAG_ROTARY = 0x040,
	FONTFLAG_DROPSHADOW = 0x080,
	FONTFLAG_ADDITIVE = 0x100,
	FONTFLAG_OUTLINE = 0x200,
	FONTFLAG_CUSTOM = 0x400,
	FONTFLAG_BITMAP = 0x800,
};

struct IntRect {
	int x0;
	int y0;
	int x1;
	int y1;
};

struct Vertex_t {
	Vertex_t() { }
	Vertex_t(const vec2& pos, const vec2& coord = vec2(0, 0)) {
		m_Position = pos;
		m_TexCoord = coord;
	}
	void Init(const vec2& pos, const vec2& coord = vec2(0, 0)) {
		m_Position = pos;
		m_TexCoord = coord;
	}

	vec2 m_Position;
	vec2 m_TexCoord;
};

class color
{
public:
	color()
	{
		*((int*)this) = 0;
	}

	color(int color32)
	{
		*((int*)this) = color32;
	}

	color(int _r, int _g, int _b)
	{
		set(_r, _g, _b, 255);
	}

	color(int _r, int _g, int _b, int _a)
	{
		set(_r, _g, _b, _a);
	}

	explicit color(float* rgb) : color(rgb[0] * 255.f, rgb[1] * 255.f, rgb[2] * 255.f, rgb[3] * 255.f) {}
	void set(int _r, int _g, int _b, int _a = 255)
	{
		_color[0] = std::clamp(_r, 0, 255);
		_color[1] = std::clamp(_g, 0, 255);
		_color[2] = std::clamp(_b, 0, 255);
		_color[3] = std::clamp(_a, 0, 255);
	}

	void get(int& _r, int& _g, int& _b, int& _a) const
	{
		_r = _color[0];
		_g = _color[1];
		_b = _color[2];
		_a = _color[3];
	}

	unsigned char get_raw() const
	{
		return *(unsigned char*)this;
	}

	unsigned char* get_ptr_r() {
		return &_color[0];
	}
	unsigned char* get_ptr_g() {
		return &_color[1];
	}
	unsigned char* get_ptr_b() {
		return &_color[2];
	}
	unsigned char* get_ptr_a() {
		return &_color[3];
	}


	inline int r() const
	{
		return _color[0];
	}
	inline int g() const
	{
		return _color[1];
	}
	inline int b() const
	{
		return _color[2];
	}
	inline int a() const
	{
		return _color[3];
	}

	inline void set_red(int value) {
		_color[0] = value;
	}
	inline void set_green(int value) {
		_color[1] = value;
	}
	inline void set_blue(int value) {
		_color[2] = value;
	}
	inline void set_alpha(int value) {
		_color[3] = value;
	}
	unsigned char& operator[](int index)
	{
		return _color[index];
	}

	const unsigned char& operator[](int index) const
	{
		return _color[index];
	}

	bool operator ==(const color& rhs) const
	{
		return rhs.r() == this->r()
			&& rhs.g() == this->g()
			&& rhs.b() == this->b()
			&& rhs.a() == this->a();
	}

	bool operator !=(const color& rhs) const
	{
		return !(operator==(rhs));
	}

	color& operator=(const color& rhs)
	{
		color clr = rhs;
		set_u32(clr.u32());
		return *this;
	}
	void set_u32(unsigned long color32)
	{
		*(unsigned long*)_color = color32;
	}
	ImU32 u32()
	{
		return ((_color[3] & 0xff) << 24) + ((_color[2] & 0xff) << 16) + ((_color[1] & 0xff) << 8)
			+ (_color[0] & 0xff);
	}
	double get_hue()
	{
		double r = _color[0] / 255.f;
		double g = _color[1] / 255.f;
		double b = _color[2] / 255.f;

		double mx = max(r, max(g, b));
		double mn = min(r, min(g, b));
		if (mx == mn)
			return 0.f;

		double delta = mx - mn;

		double hue = 0.f;
		if (mx == r)
			hue = (g - b) / delta;
		else if (mx == g)
			hue = 2.f + (b - r) / delta;
		else
			hue = 4.f + (r - g) / delta;

		hue *= 60.f;
		if (hue < 0.f)
			hue += 360.f;

		return hue / 360.f;
	}

	double get_saturation()
	{
		double r = _color[0] / 255.f;
		double g = _color[1] / 255.f;
		double b = _color[2] / 255.f;

		double mx = max(r, max(g, b));
		double mn = min(r, min(g, b));

		double delta = mx - mn;

		if (mx == 0.f)
			return delta;

		return delta / mx;
	}

	double get_brightness()
	{
		double r = _color[0] / 255.f;
		double g = _color[1] / 255.f;
		double b = _color[2] / 255.f;

		return max(r, max(g, b));
	}

	static color hsb(float hue, float saturation, float brightness)
	{
		auto clamp = [](float value, float min, float max) {
			if (value > max) return max;
			else if (value < min) return min;
			return value;
		};
		hue = clamp(hue, 0.f, 1.f);
		saturation = clamp(saturation, 0.f, 1.f);
		brightness = clamp(brightness, 0.f, 1.f);

		float h = (hue == 1.f) ? 0.f : (hue * 6.f);
		float f = h - static_cast<int>(h);
		float p = brightness * (1.f - saturation);
		float q = brightness * (1.f - saturation * f);
		float t = brightness * (1.f - (saturation * (1.f - f)));

		if (h < 1.f)
			return color((int)(brightness * 255), (int)(t * 255), (int)(p * 255));
		else if (h < 2.f)
			return color((int)(q * 255), (int)(brightness * 255), (int)(p * 255));
		else if (h < 3.f)
			return color((int)(p * 255), (int)(brightness * 255), (int)(t * 255));
		else if (h < 4)
			return color((int)(p * 255), (int)(q * 255), (int)(brightness * 255));
		else if (h < 5)
			return color((int)(t * 255), (int)(p * 255), (int)(brightness * 255));
		else
			return color((int)(brightness * 255), (int)(p * 255), (int)(q * 255));
	}
	color transition(const color& other, float strength) {
		return color(
			(other.r() + r()) / (2.f - strength),
			(other.g() + g()) / (2.f - strength),
			(other.b() + b()) / (2.f - strength));
	}
	color manage_alpha(int alpha) {
		return color((*this)[0], (*this)[1], (*this)[2], std::clamp(alpha, 0, 255));
	}
	color increase(int value, bool consider_alpha = false) {
		return color((*this)[0] + value, (*this)[1] + value, (*this)[2] + value, (*this)[3] + consider_alpha * value);
	}
	static color Black;
	static color White;
	static color Red;
	static color Green;
	static color Blue;
	static color Yellow;
	static color Pink;
	static color LightBlue;
private:
	unsigned char _color[4];
};

//-----------------------------------------------------------------------------
// Purpose: Wraps contextless windows system functions
//-----------------------------------------------------------------------------
class c_surface : public c_app_system {
public:
	virtual void          run_frame() = 0;
	virtual vgui::VPANEL  get_embedded_panel() = 0;
	virtual void          set_embedded_panel(vgui::VPANEL pPanel) = 0;
	virtual void          push_make_current(vgui::VPANEL panel, bool useInsets) = 0;
	virtual void          pop_make_current(vgui::VPANEL panel) = 0;
	virtual void          draw_set_color(int r, int g, int b, int a) = 0;
	virtual void          draw_set_color(color col) = 0;
	virtual void          draw_filled_rect(int x0, int y0, int x1, int y1) = 0;
	virtual void          draw_filled_rect_array(IntRect* pRects, int numRects) = 0;
	virtual void          draw_outlined_rect(int x0, int y0, int x1, int y1) = 0;
	virtual void          draw_line(int x0, int y0, int x1, int y1) = 0;
	virtual void          draw_poly_line(int* px, int* py, int numPoints) = 0;
	virtual void          draw_sent_apparent_depth(float f) = 0;
	virtual void          draw_clear_apparent_depth(void) = 0;
	virtual void          draw_set_text_font(vgui::HFont font) = 0;
	virtual void          draw_set_text_color(int r, int g, int b, int a) = 0;
	virtual void          draw_set_text_color(color col) = 0;
	virtual void          draw_set_text_pos(int x, int y) = 0;
	virtual void          draw_get_text_pos(int& x, int& y) = 0;
	virtual void          draw_print_text(const wchar_t* text, int textLen, FontDrawType drawType = FontDrawType::FONT_DRAW_DEFAULT) = 0;
	virtual void          draw_unicode_char(wchar_t wch, FontDrawType drawType = FontDrawType::FONT_DRAW_DEFAULT) = 0;
	virtual void          draw_flush_text() = 0;
	virtual void*		  create_html_window(void* events, vgui::VPANEL context) = 0;
	virtual void          paint_html_window(void* htmlwin) = 0;
	virtual void          delete_html_window(void* htmlwin) = 0;
	virtual int           draw_get_texture_id(char const* filename) = 0;
	virtual bool          draw_get_texture_file(int id, char* filename, int maxlen) = 0;
	virtual void          draw_set_texture_file(int id, const char* filename, int hardwareFilter, bool forceReload) = 0;
	virtual void          draw_set_texture_rgba(int id, const unsigned char* rgba, int wide, int tall) = 0;
	virtual void          draw_set_texture(int id) = 0;
	virtual void          delete_texture_by_id(int id) = 0;
	virtual void          draw_set_texture_size(int id, int& wide, int& tall) = 0;
	virtual void          draw_textured_rect(int x0, int y0, int x1, int y1) = 0;
	virtual bool          is_texture_id_valid(int id) = 0;
	virtual int           create_new_texture_id(bool procedural = false) = 0;
	virtual void          get_screen_size(int& wide, int& tall) = 0;
	virtual void          set_as_top_most(vgui::VPANEL panel, bool state) = 0;
	virtual void          bring_to_font(vgui::VPANEL panel) = 0;
	virtual void          set_foreground_window(vgui::VPANEL panel) = 0;
	virtual void          set_panel_visible(vgui::VPANEL panel, bool state) = 0;
	virtual void          set_minimized(vgui::VPANEL panel, bool state) = 0;
	virtual bool          is_minimized(vgui::VPANEL panel) = 0;
	virtual void          flash_window(vgui::VPANEL panel, bool state) = 0;
	virtual void          set_title(vgui::VPANEL panel, const wchar_t* title) = 0;
	virtual void          set_as_tool_bar(vgui::VPANEL panel, bool state) = 0;
	virtual void          create_popup(vgui::VPANEL panel, bool minimised, bool showTaskbarIcon = true, bool disabled = false, bool mouseInput = true, bool kbInput = true) = 0;
	virtual void          swap_buffers(vgui::VPANEL panel) = 0;
	virtual void          invalidate(vgui::VPANEL panel) = 0;
	virtual void          set_cursor(unsigned long cursor) = 0;
	virtual bool          is_cursor_visible() = 0;
	virtual void          apply_changes() = 0;
	virtual bool          is_within(int x, int y) = 0;
	virtual bool          has_focus() = 0;
	virtual bool          supports_feature(int /*SurfaceFeature_t*/ feature) = 0;
	virtual void          restrict_panel_to_single(vgui::VPANEL panel, bool bForceAllowNonModalSurface = false) = 0;
	virtual void          set_modal_panel(vgui::VPANEL) = 0;
	virtual vgui::VPANEL  get_modal_panel() = 0;
	virtual void          unlock_cursor() = 0;
	virtual void          lock_cursor() = 0;
	virtual void          set_translate_extended_keys(bool state) = 0;
	virtual vgui::VPANEL  get_topmost_popup() = 0;
	virtual void          set_top_level_focus(vgui::VPANEL panel) = 0;
	virtual vgui::HFont   font_create() = 0;
	virtual bool          set_font_glyph_set(vgui::HFont font, const char* windowsFontName, int tall, int weight, int blur, int scanlines, int flags, int nRangeMin = 0, int nRangeMax = 0) = 0;
	virtual bool          add_custom_font_file(const char* fontFileName) = 0;
	virtual int           get_font_tall(vgui::HFont font) = 0;
	virtual int           get_font_ascent(vgui::HFont font, wchar_t wch) = 0;
	virtual bool          is_font_additive(vgui::HFont font) = 0;
	virtual void          get_char_wide(vgui::HFont font, int ch, int& a, int& b, int& c) = 0;
	virtual int           get_character_width(vgui::HFont font, int ch) = 0;
	virtual void          get_text_size(vgui::HFont font, const wchar_t* text, int& wide, int& tall) = 0;
	virtual vgui::VPANEL  get_notify_panel() = 0;
	virtual void          set_notify_icon(vgui::VPANEL context, unsigned long icon, vgui::VPANEL panelToReceiveMessages, const char* text) = 0;
	virtual void          play_sound(const char* fileName) = 0;
	virtual int           get_popup_count() = 0;
	virtual vgui::VPANEL  get_popup(int index) = 0;
	virtual bool          should_paint_child_panel(vgui::VPANEL childPanel) = 0;
	virtual bool          recreate_context(vgui::VPANEL panel) = 0;
	virtual void          add_panel(vgui::VPANEL panel) = 0;
	virtual void          release_panel(vgui::VPANEL panel) = 0;
	virtual void          move_popup_to_front(vgui::VPANEL panel) = 0;
	virtual void          move_popup_to_back(vgui::VPANEL panel) = 0;
	virtual void          solve_traverse(vgui::VPANEL panel, bool forceApplySchemeSettings = false) = 0;
	virtual void          paint_traverse(vgui::VPANEL panel) = 0;
	virtual void          enable_mouse_capture(vgui::VPANEL panel, bool state) = 0;
	virtual void          get_workspace_bounds(int& x, int& y, int& wide, int& tall) = 0;
	virtual void          get_abs_window_bounds(int& x, int& y, int& wide, int& tall) = 0;
	virtual void          get_proportional_base(int& width, int& height) = 0;
	virtual void          calc_mouse_vis() = 0;
	virtual bool          net_kbi_input() = 0;
	virtual bool          has_cursor_pos_funcs() = 0;
	virtual void          surface_get_cursor_pos(int& x, int& y) = 0;
	virtual void          surface_set_cursor_pos(int x, int y) = 0;
	virtual void          draw_textured_line(const Vertex_t& a, const Vertex_t& b) = 0;
	virtual void          draw_outlined_circle(int x, int y, int radius, int segments) = 0;
	virtual void          add_text_poly_line(const Vertex_t* p, int n) = 0;
	virtual void          add_text_sub_rect(int x0, int y0, int x1, int y1, float texs0, float text0, float texs1, float text1) = 0;
	virtual void          add_text_poly(int n, Vertex_t* pVertice, bool bClipVertices = true) = 0;
};

class c_glow_manager
{
public:

	class GlowObjectDefinition_t
	{
	public:

		class c_entity* m_pEntity;
		union
		{
			vec3 m_vGlowcolor_t;           //0x0004
			struct
			{
				float   m_flRed;           //0x0004
				float   m_flGreen;         //0x0008
				float   m_flBlue;          //0x000C
			};
		};
		float m_flGlowAlpha;
		uint8_t pad_0014[4];
		float m_flSomeFloatThingy;
		uint8_t pad_001C[4];
		float m_flAnotherFloat;
		bool m_bRenderWhenOccluded;
		bool m_bRenderWhenUnoccluded;
		bool m_bFullBloomRender;
		uint8_t pad_0027[5];
		int32_t m_bPulsatingChams;
		int32_t m_nSplitScreenSlot;
		int32_t m_nNextFreeSlot;

		bool IsUnused() const { return m_nNextFreeSlot != GlowObjectDefinition_t::ENTRY_IN_USE; }

		static const int END_OF_FREE_LIST = -1;
		static const int ENTRY_IN_USE = -2;
	};

	GlowObjectDefinition_t* m_GlowObjectDefinitions;
	int max_size;
	int pad;
	int size;
	GlowObjectDefinition_t* m_Unknown;
	int	currentObjects;
};