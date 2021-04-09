#pragma once
#include <stdio.h>
#include <memory>
#include <stdexcept>
#include <d3d9.h>
#include <d3dx9.h>
#include "minhook/minhook.h"
#include "client.h"

#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")

typedef DWORD** PPDWORD;

// credits to: https://github.com/rollraw/qo0-base

class detour_hook
{
public:
	detour_hook() = default;

	explicit detour_hook(void* pFunction, void* pDetour)
		: pBaseFn(pFunction), pReplaceFn(pDetour) { }

	/* first hook setup */
	bool create(void* pFunction, void* pDetour)
	{
		pBaseFn = pFunction;

		if (pBaseFn == nullptr)
			return false;

		pReplaceFn = pDetour;

		if (pReplaceFn == nullptr)
			return false;

		MH_STATUS status = MH_CreateHook(pBaseFn, pReplaceFn, &pOriginalFn);

		if (status != MH_OK)
			return false;

		if (!this->replace())
			return false;

		return true;
	}

	/* replace function to our modified */
	bool replace()
	{
		// check is hook be created
		if (pBaseFn == nullptr)
			return false;

		// check is function not already hooked
		if (bIsHooked)
			return false;

		MH_STATUS status = MH_EnableHook(pBaseFn);

		if (status != MH_OK)
			return false;

		// switch hook state
		bIsHooked = true;
		return true;
	}

	/* remove hook fully */
	bool remove()
	{
		// restore it at first
		if (!this->restore())
			return false;

		MH_STATUS status = MH_RemoveHook(pBaseFn);

		if (status != MH_OK)
			return false;

		return true;
	}

	/* replace swaped modified function back to original */
	bool restore()
	{
		// check is function hooked
		if (!bIsHooked)
			return false;

		MH_STATUS status = MH_DisableHook(pBaseFn);

		if (status != MH_OK)
			return false;

		// switch hook state
		bIsHooked = false;
		return true;
	}

	/* get original function pointer (not a call!) */
	template <typename Fn>
	Fn get_original()
	{
		return static_cast<Fn>(pOriginalFn);
	}

	/* get hook state */
	inline bool is_hooked()
	{
		return bIsHooked;
	}

private:
	// Values
	/* hook state */
	bool bIsHooked = false;
	/* function base address */
	void* pBaseFn = nullptr;
	/* in future that is being modified and replace original */
	void* pReplaceFn = nullptr;
	/* saved function to get available restore hook later */
	void* pOriginalFn = nullptr;
};

class IMatRenderContext;
struct DrawModelState_t;
struct ModelRenderInfo_t;
class c_usercmd;
class matrix;
class vec3;
class c_entity;
class c_studio_hdr;
class Quaternion;
class bf_write;
class c_view_setup;
class c_move_helper;
class c_animstate;

typedef int(__thiscall* list_leaves_in_box_fn)(void*, const vec3&, const vec3&, unsigned short*, int);
typedef void(__thiscall* packet_start_fn)(void*, int, int);
typedef void(__thiscall* packet_end_fn)(void*);
typedef float(__thiscall* get_viewmodel_fov_fn)(void*);
typedef void(__thiscall* update_clientside_animation_fn)(c_entity*);
typedef void(__thiscall* do_extra_bone_processing_fn)(c_entity*, c_studio_hdr*, vec3*, Quaternion*, const matrix&, uint8_t*, void*);
typedef void(__thiscall* modify_eye_pos_fn)(c_animstate*, const vec3&);
typedef void(__thiscall* standard_blending_rules_fn)(c_entity*, c_studio_hdr*, vec3*, Quaternion*, float, int);
typedef void(__thiscall* calc_viewmodel_bob_fn)(c_entity*, const vec3&);
typedef bool(__thiscall* in_prediction_fn)(void*);
typedef bool(__thiscall* sv_cheats_get_bool_fn)(void*);
typedef void(__thiscall* override_view_fn)(void*, c_view_setup*);
typedef void(__thiscall* do_post_screen_effects_fn)(void*, c_view_setup*);
typedef void(__thiscall* run_command_fn)(void*, c_entity*, c_usercmd*, c_move_helper*);
typedef void(__thiscall* do_procedural_foot_plant_fn)(void*, void*, void*, void*, void*);
typedef void(__thiscall* check_for_sequence_change_fn)(void*, void*, int, bool, bool);
typedef void(__thiscall* draw_model_execute_fn)(void*, IMatRenderContext*, const DrawModelState_t&, const ModelRenderInfo_t&, matrix*);
typedef void(__thiscall* paint_traverse_fn)(void*, unsigned int, bool, bool);
typedef bool(__thiscall* create_move_fn)(void*, float, c_usercmd*);
typedef bool(__thiscall* write_user_cmd_delta_to_buffer_fn)(void*, int, bf_write*, int, int, bool);
typedef void(__thiscall* frame_stage_notify_fn)(void*, frame_stage);
typedef long(__stdcall* reset_fn) (IDirect3DDevice9*, D3DPRESENT_PARAMETERS*);
typedef long(__stdcall* present_fn) (IDirect3DDevice9*, const RECT*, const RECT*, HWND, const RGNDATA*);

namespace hooks
{
	extern detour_hook reset_hook;
	extern detour_hook present_hook;
	extern detour_hook paint_traverse_hook;
	extern detour_hook create_move_hook;
	extern detour_hook frame_stage_hook;
	extern detour_hook draw_model_execute_hook;
	extern detour_hook packet_start_hook;
	extern detour_hook packet_end_hook;
	extern detour_hook write_user_hook;
	extern detour_hook override_view_hook;
	extern detour_hook get_viewmodel_hook;
	extern detour_hook post_screen_effects_hook;
	extern detour_hook run_command_hook;
	extern detour_hook prediction_hook;
	extern detour_hook sv_cheats_hook;
	extern detour_hook list_leaves_in_box_hook;

	extern do_procedural_foot_plant_fn do_procedural_foot_plant_original;
	extern modify_eye_pos_fn modify_eye_pos_original;
	extern calc_viewmodel_bob_fn calc_viewmodel_bob_original;
	extern standard_blending_rules_fn standard_blending_rules_original;
	extern update_clientside_animation_fn update_clientside_animation_original;
	extern do_extra_bone_processing_fn do_extra_bone_processing_original;
	extern check_for_sequence_change_fn check_for_sequence_change_original;

	extern HRESULT __stdcall hooked_reset(IDirect3DDevice9* device, D3DPRESENT_PARAMETERS* params);
	extern HRESULT __stdcall hooked_present(IDirect3DDevice9* device,
        const RECT* source_rect, const RECT* dest_rect, HWND window_override, const RGNDATA* dirty_region);
	extern LRESULT __stdcall hooked_wnd_proc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	extern void __fastcall hooked_paint_traverse(void* ecx, void* edx, unsigned int vguiPanel, bool forceRepaint, bool allowForce);
	extern bool __fastcall hooked_create_move(void* ecx, void* edx, float input_sample_frametime, c_usercmd* cmd);
	extern void __fastcall hooked_frame_stage_notify(void* ecx, void* edx, frame_stage stage);
	extern void __stdcall hooked_draw_model_execute(IMatRenderContext* ctx, const DrawModelState_t& state, const ModelRenderInfo_t& info, matrix* bone_to_world);
	extern void __fastcall hooked_check_for_sequence_change(void* ecx, void* edx, void* hdr, int cur_sequence, bool force_new_sequence, bool interpolate);
	extern void __fastcall hooked_do_procedural_foot_plant(void* ecx, void* edx, void* boneToWorld, void* pLeftFootChain, void* pRightFootChain, void* pos);
	extern void __fastcall hooked_do_extra_bone_processing(c_entity* ecx, uint32_t, c_studio_hdr* hdr, vec3* pos, Quaternion* q, const matrix& mat, uint8_t* bone_computed, void* context);
	extern void __fastcall hooked_standard_blending_rules(c_entity* ecx, void* edx, c_studio_hdr* hdr, vec3* pos, Quaternion* q, float curTime, int boneMask);
	extern void __fastcall hooked_modify_eye_pos(c_animstate* ecx, void* edx, vec3& position);
	extern void __fastcall hooked_calc_viewmodel_bob(c_entity* ecx, void* edx, vec3& position);
	extern void __fastcall hooked_update_clientside_animation(c_entity* ecx, void* edx);
	extern void __fastcall hooked_packet_start(void* ecx, void* edx, int incoming, int outgoing);
	extern void __fastcall hooked_packet_end(void* ecx, void* edx);
	extern bool __fastcall hooked_write_user_cmd_delta_to_buffer(void* ecx, void* edx, int slot, bf_write* buf, int from, int to, bool is_new_command);
	extern void __fastcall hooked_override_view(void* ecx, void* edx, c_view_setup* setup);
	extern float __stdcall hooked_get_viewmodel_fov();
	extern void __fastcall hooked_do_post_screen_effects(void* ecx, void* edx, c_view_setup* setup);
	extern void __fastcall hooked_run_command(void* ecx, void* edx, c_entity* player, c_usercmd* m_pcmd, c_move_helper* move_helper);
	extern bool __fastcall hooked_in_prediction(void* ecx, void* edx);
	extern bool __fastcall hooked_sv_cheats_get_bool(void* ecx, void* edx);
	extern int __fastcall hooked_list_leaves_in_box(void* ecx, void* edx, vec3& mins, vec3& maxs, unsigned short* list, int list_max);

	extern bool is_hooked();
	extern void init();
	extern void unload();
};