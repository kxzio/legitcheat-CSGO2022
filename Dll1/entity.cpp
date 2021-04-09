#include "entity.h"
#include "netvar_manager.h"
#include "studio.h"
#include "math.h"

int c_entity::get_health() 
{
	return *reinterpret_cast<int*>(reinterpret_cast<uintptr_t>(this) + offsets::m_iHealth);
}

int c_entity::get_hitbox_set() {
	return *reinterpret_cast<int*>(reinterpret_cast<uintptr_t>(this) + offsets::m_nHitboxSet);
}

int c_entity::get_armor()
{
	return *reinterpret_cast<int*>(reinterpret_cast<uintptr_t>(this) + offsets::m_ArmorValue);
}

int& c_entity::get_flags() 
{
	return *reinterpret_cast<int*>(reinterpret_cast<uintptr_t>(this) + offsets::m_fFlags);
}

c_utl_vector<matrix>& c_entity::get_bone_cache()
{
	static auto m_CachedBoneData = *(DWORD*)(utils->find_pattern("client.dll", "FF B7 ? ? ? ? 52") + 0x2) + 0x4;

	return *reinterpret_cast<c_utl_vector<matrix>*>(reinterpret_cast<uintptr_t>(this) + m_CachedBoneData);
}

int c_entity::get_life_state() 
{
	return *reinterpret_cast<int*>(reinterpret_cast<uintptr_t>(this) + offsets::m_lifeState);
}
int c_entity::fov()
{
	return *reinterpret_cast<int*>(reinterpret_cast<uintptr_t>(this) + offsets::m_iFOV);
}
int c_entity::default_fov()
{
	return *reinterpret_cast<int*>(reinterpret_cast<uintptr_t>(this) + offsets::m_iDefaultFOV);
}
int c_entity::get_fov() {
	if (fov() != 0)
		return fov();
	return default_fov();
}
bool c_entity::is_alive() 
{
	return get_life_state() == 0 && get_health() > 0;
}

bool c_entity::has_helmet()
{
	return* reinterpret_cast<int*>(reinterpret_cast<uintptr_t>(this) + offsets::m_bHasHelmet);
}

bool c_entity::compute_hitbox_surrounding_box(vec3* mins, vec3* maxs) {
	using compute_hitbox_surrounding_box_t = bool(__thiscall*)(void*, vec3*, vec3*);
	static auto compute_hitbox_surrounding_box = reinterpret_cast<compute_hitbox_surrounding_box_t>(utils->find_pattern("client.dll", "55 8B EC 83 E4 F8 81 EC 24 04 00 00 ? ? ? ? ? ?"));
	if (!compute_hitbox_surrounding_box)
		return false;

	return compute_hitbox_surrounding_box(this, mins, maxs);
}

int	c_entity::get_move_type()
{
	if (!this)
		return 0;

	return *reinterpret_cast<int*>(reinterpret_cast<uintptr_t>(this) + 0x25C);
}

vec3& c_entity::get_velocity() 
{
	return *reinterpret_cast<vec3*>(reinterpret_cast<uintptr_t>(this) + offsets::m_vecVelocity);
}

bool c_entity::is_player() 
{
	return this->GetClientClass()->class_id == 40;
}

bool c_entity::is_scoped()
{
	return *reinterpret_cast<bool*>(reinterpret_cast<uintptr_t>(this) + offsets::m_bIsScoped);
}

c_animstate* c_entity::get_animstate()
{
	return *reinterpret_cast<c_animstate**>(reinterpret_cast<uintptr_t>(this) + 0x3914);
}

c_weapon* c_entity::get_weapon()
{
	DWORD weapon_data = *reinterpret_cast<DWORD*>(reinterpret_cast<uintptr_t>(this) + offsets::m_hActiveWeapon);
	return reinterpret_cast<c_weapon*>(interfaces->entity_list->get_client_entity_handle(weapon_data));
}

c_viewmodel* c_entity::get_viewmodel()
{
	DWORD weapon_data = *reinterpret_cast<DWORD*>(reinterpret_cast<uintptr_t>(this) + offsets::m_hViewModel);
	return reinterpret_cast<c_viewmodel*>(interfaces->entity_list->get_client_entity_handle(weapon_data));
}

datamap_t* c_entity::get_data_desc_map()
{
	typedef datamap_t* (__thiscall* o_GetPredDescMap)(void*);
	return getvfunc<o_GetPredDescMap>(this, 15)(this);
}

datamap_t* c_entity::get_pred_desc_map()
{
	typedef datamap_t* (__thiscall* o_GetPredDescMap)(void*);
	return getvfunc<o_GetPredDescMap>(this, 17)(this);
}

int c_entity::get_sequence()
{
	return *reinterpret_cast<int*>(reinterpret_cast<uintptr_t>(this) + offsets::m_nSequence);
}

int& c_entity::get_tickbase()
{
	return *reinterpret_cast<int*>(reinterpret_cast<uintptr_t>(this) + offsets::m_nTickBase);
}

int& c_entity::get_effects()
{
	static auto m_fEffects = utils->find_in_data_map(get_pred_desc_map(), "m_fEffects");
	return *reinterpret_cast<int*>(uintptr_t(this) + m_fEffects);
}

int& c_entity::get_e_flags()
{
	static auto m_iEFlags = utils->find_in_data_map(get_pred_desc_map(), "m_iEFlags");
	return *reinterpret_cast<int*>(uintptr_t(this) + m_iEFlags);
}

vec3& c_entity::get_punch_angle()
{
	return *reinterpret_cast<vec3*>(uintptr_t(this) + offsets::m_aimPunchAngle);
}
//	NETVAR(int32_t, m_iShotsFired, "DT_CSPlayer", "m_iShotsFired");
int& c_entity::shots_fired()
{
	return *reinterpret_cast<int*>(uintptr_t(this) + offsets::m_viewPunchAngle);
}
vec3 c_entity::get_bone_position(matrix* mat, int i)
{
	if (!mat)
		return vec3(0, 0, 0);

	return vec3(mat[i][0][3], mat[i][1][3], mat[i][2][3]);
}
void* c_entity::animating() {
	return reinterpret_cast<void*>(uintptr_t(this) + 0x4);
}


vec3 c_entity::get_eye_position()
{
	return get_vec_origin() + get_view_offset();
}

void c_entity::invalidate_bone_cache()
{
	typedef void(__thiscall* originalFn)(void*);
	static auto func = (originalFn)utils->find_pattern("client.dll", "80 3D ? ? ? ? ? 74 16 A1 ? ? ? ? 48 C7 81");

	if (this && func)
		func(this);
}

vec3& c_entity::get_punch_angle_vel()
{
	return *reinterpret_cast<vec3*>(uintptr_t(this) + offsets::m_aimPunchAngleVel);
}

vec3& c_entity::get_view_punch_angle()
{
	return *reinterpret_cast<vec3*>(uintptr_t(this) + offsets::m_viewPunchAngle);
}

bool& c_entity::get_clientside_animation()
{
	return *reinterpret_cast<bool*>(uintptr_t(this) + offsets::m_bClientSideAnimation);
}

void c_entity::update_clientside_animation()
{
	typedef void(__thiscall* fn)(void*);
	getvfunc<fn>(this, 223)(this);
}

void c_entity::create_anim_state(c_animstate* state)
{
	using create_anim_state_t = void(__thiscall*)(c_animstate*, c_entity*);
	static auto create_anim_state = reinterpret_cast<create_anim_state_t>(utils->find_pattern("client.dll", "55 8B EC 56 8B F1 B9 ? ? ? ? C7 46"));
	if (!create_anim_state)
		return;

	create_anim_state(state, this);
}

void c_entity::reset_anim_state(c_animstate* state)
{
	using reset_anim_state_t = void(__thiscall*)(c_animstate*);
	static auto reset_anim_state = reinterpret_cast<reset_anim_state_t>(utils->find_pattern("client.dll", "56 6A 01 68 ? ? ? ? 8B F1"));
	if (!reset_anim_state)
		return;

	reset_anim_state(state);
}

void c_entity::update_anim_state(c_animstate* state, const vec3& ang)
{
	using Fn = void(__vectorcall*)(void*, void*, float, float, float, void*);
	static auto fn = reinterpret_cast <Fn> (utils->find_pattern("client.dll", "55 8B EC 83 E4 F8 83 EC 18 56 57 8B F9 F3 0F 11 54 24"));

	fn(state, nullptr, 0.0f, ang[1], ang[0], nullptr);
}

void c_entity::set_abs_origin(const vec3& origin)
{
	using set_abs_origin_t = void(__thiscall*)(c_entity*, const vec3&);
	static auto set_abs_origin = reinterpret_cast<set_abs_origin_t>(utils->find_pattern("client.dll", "55 8B EC 83 E4 F8 51 53 56 57 8B F1 E8"));
	if (!set_abs_origin)
		return;

	set_abs_origin(this, origin);
}

int& c_entity::get_model_bone_counter()
{
	static auto invalidateBoneCache = utils->find_pattern("client.dll", "80 3D ? ? ? ? ? 74 16 A1 ? ? ? ? 48 C7 81");
	return **(int**)(invalidateBoneCache + 10);
}

int& c_entity::get_model_recent_bone_counter() {
	return *reinterpret_cast<int*>(reinterpret_cast<uintptr_t>(this) + offsets::m_nForceBone + 0x4);
}

void c_entity::set_abs_angles(const vec3& ang)
{
	using set_abs_angles_t = void(__thiscall*)(c_entity*, const vec3&);
	static auto set_abs_angles = reinterpret_cast<set_abs_angles_t>(utils->find_pattern("client.dll", "55 8B EC 83 E4 F8 83 EC 64 53 56 57 8B F1 E8"));
	if (!set_abs_angles)
		return;

	set_abs_angles(this, ang);
}

std::array<float, 24>& c_entity::get_pose_parameter()
{	
	return *reinterpret_cast<std::array<float, 24>*>(reinterpret_cast<uintptr_t>(this) + offsets::m_flPoseParameter);
}

vec3& c_entity::get_vec_origin()
{
	return *reinterpret_cast<vec3*>(uintptr_t(this) + offsets::m_vecOrigin);
}

vec3& c_entity::get_view_offset()
{
	return *reinterpret_cast<vec3*>(uintptr_t(this) + offsets::m_vecViewOffset);
}

const matrix& c_entity::m_rgflCoordinateFrame()
{
	static auto _m_rgflCoordinateFrame = offsets::m_CollisionGroup - 0x30;
	return *reinterpret_cast<matrix*>(uintptr_t(this) + _m_rgflCoordinateFrame);
}

vec3& c_entity::get_eye_angles()
{
	return *reinterpret_cast<vec3*>(uintptr_t(this) + offsets::m_angEyeAngles);
}

vec3& c_entity::get_abs_velocity()
{
	static auto m_vecAbsVelocity = utils->find_in_data_map(get_pred_desc_map(), "m_vecAbsVelocity");
	return *reinterpret_cast<vec3*>(uintptr_t(this) + m_vecAbsVelocity);
}

int c_entity::get_team() {
	return *reinterpret_cast<int*>(uintptr_t(this) + offsets::m_iTeamNum);
}

bool c_entity::is_dormant()
{
	return IsDormant();
}

c_animlayer* c_entity::get_anim_overlays()
{
	return *reinterpret_cast<c_animlayer**>(reinterpret_cast<uintptr_t>(this) + 0x2980);
}

c_animlayer* c_entity::get_anim_overlay(int index)
{
	if (index < 15)
		return &get_anim_overlays()[index];
	else
		return nullptr;
}

float c_entity::get_spawn_time()
{
	return *reinterpret_cast<float*>(reinterpret_cast<uintptr_t>(this) + 0xA370);
}

bool c_entity::has_gun_game_immunity()
{
	return *reinterpret_cast<bool*>(reinterpret_cast<uintptr_t>(this) + offsets::m_bGunGameImmunity);
}
bool c_entity::has_heavy_armor() {
	//"DT_CSPlayer", "m_bHasHeavyArmor"
	return *reinterpret_cast<bool*>(reinterpret_cast<uintptr_t>(this) + offsets::m_bHasHeavyArmor);
}

bool& c_entity::has_spotted()
{
	return *reinterpret_cast<bool*>(reinterpret_cast<uintptr_t>(this) + offsets::m_bSpotted);
}

bool c_entity::setup_bones(matrix* mat, int max_bones, int bone_mask, float cur_time)
{
	return SetupBones(mat, max_bones, bone_mask, cur_time);
}

float& c_entity::get_simulation_time()
{
	return *reinterpret_cast<float*>(reinterpret_cast<uintptr_t>(this) + offsets::m_flSimulationTime);
}

float& c_entity::get_old_simulation_time()
{
	return *reinterpret_cast<float*>(reinterpret_cast<uintptr_t>(this) + offsets::m_flSimulationTime + 0x4);
}

float& c_entity::get_velocity_modifier()
{
	return *reinterpret_cast<float*>(reinterpret_cast<uintptr_t>(this) + offsets::m_flVelocityModifier);
}

float& c_entity::get_last_shot_time()
{
	return *reinterpret_cast<float*>(reinterpret_cast<uintptr_t>(this) + offsets::m_fLastShotTime);
}

int& c_entity::m_take_damage() {
	return *reinterpret_cast<int*>(uintptr_t(this) + 0x280);
}

float c_weapon::get_spread()
{
	if (!this)
		return 0.f;

	typedef float(__thiscall* OriginalFn)(void*);
	return getvfunc< OriginalFn >(this, 452)(this);
}

float c_weapon::get_inaccuracy()
{
	if (!this)
		return 0.f;

	typedef float(__thiscall* OriginalFn)(void*);
	return getvfunc< OriginalFn >(this, 482)(this);
}

float c_weapon::get_next_attack()
{
	return *reinterpret_cast<float*>(reinterpret_cast<uintptr_t>(this) + 0x2D70);
}

float c_weapon::get_next_primary_attack()
{
	return *reinterpret_cast<float*>(reinterpret_cast<uintptr_t>(this) + offsets::m_flNextPrimaryAttack);
}
float c_weapon::post_pone_fire_ready_time()
{
	return *reinterpret_cast<float*>(reinterpret_cast<uintptr_t>(this) + offsets::m_flPostponeFireReadyTime);
}

float c_weapon::get_next_secondary_attack()
{
	return *reinterpret_cast<float*>(reinterpret_cast<uintptr_t>(this) + offsets::m_flNextSecondaryAttack);
}

void c_weapon::update_accuracy_penalty()
{
	if (!this) 
		return;

	typedef void(__thiscall* oUpdateAccuracyPenalty)(PVOID);
	return getvfunc<oUpdateAccuracyPenalty>(this, 483)(this);
}

int c_weapon::get_ammo()
{
	return *reinterpret_cast<int*>(reinterpret_cast<uintptr_t>(this) + offsets::m_iClip1);
}

c_weapon_info* c_weapon::get_weapon_data()
{
	if (!this) 
		return nullptr;

	typedef c_weapon_info* (__thiscall* OriginalFn)(void*);
	return getvfunc<OriginalFn>(this, 460)(this);
}

bool c_weapon::is_knife()
{
	if (!this)
		return false;

	int idx = get_item_definition_index();

	return idx == weapon_knife || idx == weapon_knife_bayonet || idx == weapon_knife_butterfly || idx == weapon_knife_falchion
		|| idx == weapon_knife_flip || idx == weapon_knife_gut || idx == weapon_knife_karambit || idx == weapon_knife_m9_bayonet
		|| idx == weapon_knife_push || idx == weapon_knife_survival_bowie || idx == weapon_knife_t || idx == weapon_knife_tactical
		|| idx == weapon_knifegg || idx == weapon_knife_ghost || idx == weapon_knife_gypsy_jackknife || idx == weapon_knife_stiletto
		|| idx == weapon_knife_ursus || idx == weapon_knife_widowmaker || idx == weapon_knife_css || idx == weapon_knife_canis
		|| idx == weapon_knife_cord || idx == weapon_knife_outdoor || idx == weapon_knife_skeleton;
}

bool c_weapon::is_grenade()
{
	if (!this)
		return false;

	int idx = get_item_definition_index();

	return idx == weapon_flashbang 
		|| idx == weapon_hegrenade 
		|| idx == weapon_smokegrenade 
		|| idx == weapon_molotov 
		|| idx == weapon_decoy 
		|| idx == weapon_incgrenade;
}

short c_weapon::get_item_definition_index()
{
	return *reinterpret_cast<short*>(reinterpret_cast<uintptr_t>(this) + offsets::m_iItemDefinitionIndex);
}

int c_viewmodel::get_viewmodel_index()
{
	return *reinterpret_cast<int*>(reinterpret_cast<uintptr_t>(this) + offsets::m_nViewModelIndex);
}

int c_viewmodel::get_animation_party()
{
	return *reinterpret_cast<int*>(reinterpret_cast<uintptr_t>(this) + offsets::m_nAnimationParity);
}

float& c_viewmodel::get_cycle()
{
	static auto m_flCycle = utils->find_in_data_map(get_pred_desc_map(), "m_flCycle");
	return *reinterpret_cast<float*>(reinterpret_cast<uintptr_t>(this) + m_flCycle);
}

float& c_viewmodel::get_animtime()
{
	static auto m_flAnimTime = utils->find_in_data_map(get_pred_desc_map(), "m_flAnimTime");
	return *reinterpret_cast<float*>(reinterpret_cast<uintptr_t>(this) + m_flAnimTime);
}

CHandle<c_weapon> c_viewmodel::get_weapon_handle()
{
	return *reinterpret_cast<CHandle<c_weapon>*>(reinterpret_cast<uintptr_t>(this) + offsets::m_hWeapon);
}
