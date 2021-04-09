#pragma once
#include "vector3.h"
#include "utils.h"

class c_net_channel_info;

struct player_info_t {
	int64_t __pad0;
	union {
		int64_t xuid;
		struct {
			int xuidlow;
			int xuidhigh;
		};
	};
	char name[128];
	int userid;
	char guid[33];
	unsigned int friendsid;
	char friendsname[128];
	bool fakeplayer;
	bool ishltv;
	unsigned int customfiles[4];
	unsigned char filesdownloaded;
};

class c_engine {
public:
	c_net_channel_info* get_net_channel_info() {
		using original_fn = c_net_channel_info * (__thiscall*)(c_engine*);
		return getvfunc<original_fn>(this, 78)(this);
	}

	void* get_bsp_tree_query() {
		using original_fn = void* (__thiscall*)(c_engine*);
		return getvfunc<original_fn>(this, 43)(this);
	}

	int get_local_player() {
		using original_fn = int(__thiscall*)(c_engine*);
		return getvfunc<original_fn>(this, 12)(this);
	}

	int get_player_for_user_id(int user_id) {
		using original_fn = int(__thiscall*)(PVOID, int);
		return getvfunc<original_fn>(this, 9)(this, user_id);
	}

	void get_player_info(int index, player_info_t* info) {
		using original_fn = void(__thiscall*)(c_engine*, int, player_info_t*);
		return getvfunc<original_fn>(this, 8)(this, index, info);
	}

	void get_screen_size(int& width, int& height) {
		using original_fn = void(__thiscall*)(c_engine*, int&, int&);
		return getvfunc<original_fn>(this, 5)(this, width, height);
	}

	bool is_in_game() {
		using original_fn = bool(__thiscall*)(c_engine*);
		return getvfunc<original_fn>(this, 26)(this);
	}

	bool is_connected() {
		using original_fn = bool(__thiscall*)(c_engine*);
		return getvfunc<original_fn>(this, 27)(this);
	}

	void execute_cmd(const char* cmd) {
		using original_fn = void(__thiscall*)(c_engine*, const char*);
		return getvfunc<original_fn>(this, 108)(this, cmd);
	}

	void execute_cmd_unrestricted(const char* cmd, const char* flag = 0) {
		using original_fn = void(__thiscall*)(c_engine*, const char*, const char*);
		return getvfunc<original_fn>(this, 114)(this, cmd, flag);
	}

	void set_view_angles(vec3& angles) {
		using original_fn = void(__thiscall*)(c_engine*, vec3&);
		return getvfunc<original_fn>(this, 19)(this, angles);
	}

	void get_view_angles(vec3& angles) {
		return getvfunc<void(__thiscall*)(void*, vec3&)>(this, 18)(this, angles);
	}

	matrix& world_to_screen_matrix() {
		using original_fn = matrix & (__thiscall*)(c_engine*);
		return getvfunc<original_fn>(this, 37)(this);
	}

	bool is_taking_screenshot() {
		using original_fn = bool(__thiscall*)(c_engine*);
		return getvfunc<original_fn>(this, 92)(this);
	}

	const char* get_level_name() {
		using original_fn = const char* (__thiscall*)(c_engine*);
		return getvfunc<original_fn>(this, 53)(this);
	}
};

struct SurfacePhysicsParams_t {
	float friction;
	float elasticity;
	float density;
	float thickness;
	float dampening;
};


struct SurfaceAudioParams_t {
	float reflectivity; // like elasticity, but how much sound should be reflected by this surface
	float hardnessFactor; // like elasticity, but only affects impact sound choices
	float roughnessFactor; // like friction, but only affects scrape sound choices   
	float roughThreshold; // surface roughness > this causes "rough" scrapes, < this causes "smooth" scrapes
	float hardThreshold; // surface hardness > this causes "hard" impacts, < this causes "soft" impacts
	float hardVelocityThreshold; // collision velocity > this causes "hard" impacts, < this causes "soft" impacts   
	float highPitchOcclusion;
	//a value betweeen 0 and 100 where 0 is not occluded at all and 100 is silent (except for any additional reflected sound)
	float midPitchOcclusion;
	float lowPitchOcclusion;
};

struct SurfaceSoundNames_t {
	unsigned short walkstepleft;
	unsigned short walkstepright;
	unsigned short runstepleft;
	unsigned short runstepright;
	unsigned short impactsoft;
	unsigned short impacthard;
	unsigned short scrapesmooth;
	unsigned short scraperough;
	unsigned short bulletimpact;
	unsigned short rolling;
	unsigned short breaksound;
	unsigned short strainsound;
};

struct SurfaceSoundHandles_t {
	short walkstepleft;
	short walkstepright;
	short runstepleft;
	short runstepright;
	short impactsoft;
	short impacthard;
	short scrapesmooth;
	short scraperough;
	short bulletimpact;
	short rolling;
	short breaksound;
	short strainsound;
};

struct SurfaceGameProps_t {
	float maxspeedfactor;
	float jumpfactor;
	float penetrationmodifier;
	float damagemodifier;
	uint16_t material;
	uint8_t climbable;
};
struct surface_data_t {
	SurfacePhysicsParams_t physics;
	SurfaceAudioParams_t audio;
	SurfaceSoundNames_t sounds;
	SurfaceGameProps_t game;
};
class IPhysicsSurfaceProps
{
public:
	virtual ~IPhysicsSurfaceProps(void) {}

	// parses a text file containing surface prop keys
	virtual int		ParseSurfaceData(const char* pFilename, const char* pTextfile) = 0;
	// current number of entries in the database
	virtual int		SurfacePropCount(void) const = 0;

	virtual int		GetSurfaceIndex(const char* pSurfacePropName) const = 0;
	virtual void	GetPhysicsProperties(int surfaceDataIndex, float* density, float* thickness, float* friction, float* elasticity) const = 0;

	virtual surface_data_t* GetSurfaceData(int surfaceDataIndex) = 0;
	virtual const char* GetString(unsigned short stringTableIndex) const = 0;


	virtual const char* GetPropName(int surfaceDataIndex) const = 0;

	// sets the global index table for world materials
	// UNDONE: Make this per-CPhysCollide
	virtual void	SetWorldMaterialIndexTable(int* pMapArray, int mapSize) = 0;

	// NOTE: Same as GetPhysicsProperties, but maybe more convenient
	virtual void	GetPhysicsParameters(int surfaceDataIndex, SurfacePhysicsParams_t* pParamsOut) const = 0;
};
typedef void* FileNameHandle_t;
struct SndInfo_t
{
	int			m_nGuid;
	FileNameHandle_t m_filenameHandle;
	int			m_nSoundSource;
	int			m_nChannel;
	int			m_nSpeakerEntity;
	float		m_flVolume;
	float		m_flLastSpatializedVolume;
	float		m_flRadius;
	int			m_nPitch;
	vec3* m_pOrigin;
	vec3* m_pDirection;
	bool		m_bUpdatePositions;
	bool		m_bIsSentence;
	bool		m_bDryMix;
	bool		m_bSpeaker;
	bool		m_bFromServer;
};

class RadarPlayer_t
{
public:
	vec3 pos; //0x0000
	vec3 angle; //0x000C
	vec3 spotted_map_angle_related; //0x0018
	DWORD tab_related; //0x0024
	char pad_0x0028[0xC]; //0x0028
	float spotted_time; //0x0034
	float spotted_fraction; //0x0038
	float time; //0x003C
	char pad_0x0040[0x4]; //0x0040
	__int32 player_index; //0x0044
	__int32 entity_index; //0x0048
	char pad_0x004C[0x4]; //0x004C
	__int32 health; //0x0050
	char name[32]; //0x785888
	char pad_0x0074[0x75]; //0x0074
	unsigned char spotted; //0x00E9
	char pad_0x00EA[0x8A]; //0x00EA
};

class CCSGO_HudRadar
{
public:
	char pad_0x0000[0x14C];
	RadarPlayer_t radar_info[65];
};

class c_engine_sound
{
public:
	void get_active_sounds(c_utl_vector <SndInfo_t>& sndList)
	{
		using GetActiveSoundsFn = void(__thiscall*)(void*, c_utl_vector <SndInfo_t>&);
		return getvfunc <GetActiveSoundsFn>(this, 19)(this, sndList);
	}
};