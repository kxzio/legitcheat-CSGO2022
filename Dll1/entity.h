#pragma once
#include "globals.h"
#include "utils.h"
#include "interfaces.h"
#include "datamap.h"
#include "studio.h"

#define time_to_ticks(t) ((int)(0.5f + (float)(t) / interfaces->global_vars->intervalpertick))
#define ticks_to_time(t) (interfaces->global_vars->intervalpertick * (t))

#define fl_onground (1 << 0)
#define fl_ducking (1 << 1)
#define fl_waterjump (1 << 2)
#define fl_ontrain (1 << 3)
#define fl_inrain (1 << 4)
#define fl_frozen (1 << 5)
#define fl_atcontrols (1 << 6)
#define fl_client (1 << 7)
#define fl_fakeclient (1 << 8)
#define fl_inwater (1 << 9)
#define fl_fly (1 << 10)
#define fl_swim (1 << 11)
#define fl_conveyor (1 << 12)
#define fl_npc (1 << 13)
#define fl_godmode (1 << 14)
#define fl_notarget (1 << 15)
#define fl_aimtarget (1 << 16)
#define fl_partialground (1 << 17)
#define fl_staticprop (1 << 18)
#define fl_graphed (1 << 19)
#define fl_grenade (1 << 20)
#define fl_stepmovement (1 << 21)
#define fl_donttouch (1 << 22)
#define fl_basevelocity (1 << 23)
#define fl_worldbrush (1 << 24)
#define fl_object (1 << 25)
#define fl_killme (1 << 26)
#define fl_onfire (1 << 27)
#define fl_dissolving (1 << 28)
#define fl_transragdoll (1 << 29)
#define fl_unblockable_by_player (1 << 30)

enum movetype_t {
	movetype_none,
	movetype_isometric,
	movetype_walk,
	movetype_step,
	movetype_fly,
	movetype_flygravity,
	movetype_vphysics,
	movetype_push,
	movetype_noclip,
	movetype_ladder,
	movetype_observer,
	movetype_custom,
	movetype_last = movetype_custom,
	movetype_max_bits = 4,
	max_movetype
};

class IClientUnknown;
class ClientClass;
class bf_read;

#define NUM_ENT_ENTRY_BITS         (11 + 2)
#define NUM_ENT_ENTRIES            (1 << NUM_ENT_ENTRY_BITS)
#define INVALID_EHANDLE_INDEX       0xFFFFFFFF
#define NUM_SERIAL_NUM_BITS        16 // (32 - NUM_ENT_ENTRY_BITS)
#define NUM_SERIAL_NUM_SHIFT_BITS (32 - NUM_SERIAL_NUM_BITS)
#define ENT_ENTRY_MASK             (( 1 << NUM_SERIAL_NUM_BITS) - 1)

class CBaseHandle;

class IHandleEntity {
public:
	virtual ~IHandleEntity() {}
	virtual void SetRefEHandle(const CBaseHandle& handle) = 0;
	virtual const CBaseHandle& GetRefEHandle() const = 0;
};

class ICollideable
{
public:
	virtual void pad0();
	virtual const vec3& OBBMin() const;
	virtual const vec3& OBBMax() const;
};

class IClientUnknown : public IHandleEntity
{
public:
	virtual ICollideable* GetCollideable() = 0;
	virtual IClientNetworkable* GetClientNetworkable() = 0;
	virtual IClientRenderable* GetClientRenderable() = 0;
	virtual IClientEntity* GetIClientEntity() = 0;
	virtual c_entity* GetBaseEntity() = 0;
	virtual IClientThinkable* GetClientThinkable() = 0;
	//virtual IClientModelRenderable*  GetClientModelRenderable() = 0;
	virtual IClientAlphaProperty* GetClientAlphaProperty() = 0;
};

class IClientRenderable
{
public:
	virtual IClientUnknown* GetIClientUnknown() = 0;
	virtual vec3 const& GetRenderOrigin(void) = 0;
	virtual vec3 const& GetRenderAngles(void) = 0;
	virtual bool                       ShouldDraw(void) = 0;
	virtual int                        GetRenderFlags(void) = 0; // ERENDERFLAGS_xxx
	virtual void                       Unused(void) const {}
	virtual ClientShadowHandle_t       GetShadowHandle() const = 0;
	virtual ClientRenderHandle_t& RenderHandle() = 0;
	virtual const model_t* GetModel() const = 0;
	virtual int                        DrawModel(int flags, const int /*RenderableInstance_t*/& instance) = 0;
	virtual int                        GetBody() = 0;
	virtual void                       GetColorModulation(float* color) = 0;
	virtual bool                       LODTest() = 0;
	virtual bool                       SetupBones(matrix* pBoneToWorldOut, int nMaxBones, int boneMask, float currentTime) = 0;
	virtual void                       SetupWeights(const matrix* pBoneToWorld, int nFlexWeightCount, float* pFlexWeights, float* pFlexDelayedWeights) = 0;
	virtual void                       DoAnimationEvents(void) = 0;
	virtual void* GetPVSNotifyInterface() = 0;
	virtual void                       GetRenderBounds(vec3& mins, vec3& maxs) = 0;
	virtual void                       GetRenderBoundsWorldspace(vec3& mins, vec3& maxs) = 0;
	virtual void                       GetShadowRenderBounds(vec3& mins, vec3& maxs, int /*ShadowType_t*/ shadowType) = 0;
	virtual bool                       ShouldReceiveProjectedTextures(int flags) = 0;
	virtual bool                       GetShadowCastDistance(float* pDist, int /*ShadowType_t*/ shadowType) const = 0;
	virtual bool                       GetShadowCastDirection(vec3* pDirection, int /*ShadowType_t*/ shadowType) const = 0;
	virtual bool                       IsShadowDirty() = 0;
	virtual void                       MarkShadowDirty(bool bDirty) = 0;
	virtual IClientRenderable* GetShadowParent() = 0;
	virtual IClientRenderable* FirstShadowChild() = 0;
	virtual IClientRenderable* NextShadowPeer() = 0;
	virtual int /*ShadowType_t*/       ShadowCastType() = 0;
	virtual void                       CreateModelInstance() = 0;
	virtual ModelInstanceHandle_t      GetModelInstance() = 0;
	virtual const matrix& RenderableToWorldTransform() = 0;
	virtual int                        LookupAttachment(const char* pAttachmentName) = 0;
	virtual  bool                      GetAttachment(int number, vec3& origin, vec3& angles) = 0;
	virtual bool                       GetAttachment(int number, matrix& matrix) = 0;
	virtual float* GetRenderClipPlane(void) = 0;
	virtual int                        GetSkin() = 0;
	virtual void                       OnThreadedDrawSetup() = 0;
	virtual bool                       UsesFlexDelayedWeights() = 0;
	virtual void                       RecordToolMessage() = 0;
	virtual bool                       ShouldDrawForSplitScreenUser(int nSlot) = 0;
	virtual uint8_t                    OverrideAlphaModulation(uint8_t nAlpha) = 0;
	virtual uint8_t                    OverrideShadowAlphaModulation(uint8_t nAlpha) = 0;
};

#pragma region MASKS

#define   DISPSURF_FLAG_SURFACE           (1<<0)
#define   DISPSURF_FLAG_WALKABLE          (1<<1)
#define   DISPSURF_FLAG_BUILDABLE         (1<<2)
#define   DISPSURF_FLAG_SURFPROP1         (1<<3)
#define   DISPSURF_FLAG_SURFPROP2         (1<<4)

#define   CONTENTS_EMPTY                0

#define   CONTENTS_SOLID                0x1       
#define   CONTENTS_WINDOW               0x2
#define   CONTENTS_AUX                  0x4
#define   CONTENTS_GRATE                0x8
#define   CONTENTS_SLIME                0x10
#define   CONTENTS_WATER                0x20
#define   CONTENTS_BLOCKLOS             0x40 
#define   CONTENTS_OPAQUE               0x80 
#define   LAST_VISIBLE_CONTENTS         CONTENTS_OPAQUE

#define   ALL_VISIBLE_CONTENTS            (LAST_VISIBLE_CONTENTS | (LAST_VISIBLE_CONTENTS-1))

#define   CONTENTS_TESTFOGVOLUME        0x100
#define   CONTENTS_UNUSED               0x200     
#define   CONTENTS_BLOCKLIGHT           0x400
#define   CONTENTS_TEAM1                0x800 
#define   CONTENTS_TEAM2                0x1000 
#define   CONTENTS_IGNORE_NODRAW_OPAQUE 0x2000
#define   CONTENTS_MOVEABLE             0x4000
#define   CONTENTS_AREAPORTAL           0x8000
#define   CONTENTS_PLAYERCLIP           0x10000
#define   CONTENTS_MONSTERCLIP          0x20000
#define   CONTENTS_CURRENT_0            0x40000
#define   CONTENTS_CURRENT_90           0x80000
#define   CONTENTS_CURRENT_180          0x100000
#define   CONTENTS_CURRENT_270          0x200000
#define   CONTENTS_CURRENT_UP           0x400000
#define   CONTENTS_CURRENT_DOWN         0x800000

#define   CONTENTS_ORIGIN               0x1000000 

#define   CONTENTS_MONSTER              0x2000000 
#define   CONTENTS_DEBRIS               0x4000000
#define   CONTENTS_DETAIL               0x8000000 
#define   CONTENTS_TRANSLUCENT          0x10000000
#define   CONTENTS_LADDER               0x20000000
#define   CONTENTS_HITBOX               0x40000000

#define   SURF_LIGHT                    0x0001 
#define   SURF_SKY2D                    0x0002 
#define   SURF_SKY                      0x0004 
#define   SURF_WARP                     0x0008 
#define   SURF_TRANS                    0x0010
#define   SURF_NOPORTAL                 0x0020 
#define   SURF_TRIGGER                  0x0040 
#define   SURF_NODRAW                   0x0080 

#define   SURF_HINT                     0x0100 

#define   SURF_SKIP                     0x0200   
#define   SURF_NOLIGHT                  0x0400   
#define   SURF_BUMPLIGHT                0x0800   
#define   SURF_NOSHADOWS                0x1000   
#define   SURF_NODECALS                 0x2000   
#define   SURF_NOPAINT                  SURF_NODECALS
#define   SURF_NOCHOP                   0x4000   
#define   SURF_HITBOX                   0x8000   

// -----------------------------------------------------
// spatial content masks - used for spatial queries (traceline,etc.)
// -----------------------------------------------------
#define   MASK_ALL                      (0xFFFFFFFF)
#define   MASK_SOLID                    (CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_WINDOW|CONTENTS_MONSTER|CONTENTS_GRATE)
#define   MASK_PLAYERSOLID              (CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_PLAYERCLIP|CONTENTS_WINDOW|CONTENTS_MONSTER|CONTENTS_GRATE)
#define   MASK_NPCSOLID                 (CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_MONSTERCLIP|CONTENTS_WINDOW|CONTENTS_MONSTER|CONTENTS_GRATE)
#define   MASK_NPCFLUID                 (CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_MONSTERCLIP|CONTENTS_WINDOW|CONTENTS_MONSTER)
#define   MASK_WATER                    (CONTENTS_WATER|CONTENTS_MOVEABLE|CONTENTS_SLIME)
#define   MASK_OPAQUE                   (CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_OPAQUE)
#define   MASK_OPAQUE_AND_NPCS          (MASK_OPAQUE|CONTENTS_MONSTER)
#define   MASK_BLOCKLOS                 (CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_BLOCKLOS)
#define   MASK_BLOCKLOS_AND_NPCS        (MASK_BLOCKLOS|CONTENTS_MONSTER)
#define   MASK_VISIBLE                  (MASK_OPAQUE|CONTENTS_IGNORE_NODRAW_OPAQUE)
#define   MASK_VISIBLE_AND_NPCS         (MASK_OPAQUE_AND_NPCS|CONTENTS_IGNORE_NODRAW_OPAQUE)
#define   MASK_SHOT                     (CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_MONSTER|CONTENTS_WINDOW|CONTENTS_DEBRIS|CONTENTS_HITBOX)
#define   MASK_SHOT_BRUSHONLY           (CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_WINDOW|CONTENTS_DEBRIS)
#define   MASK_SHOT_HULL                (CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_MONSTER|CONTENTS_WINDOW|CONTENTS_DEBRIS|CONTENTS_GRATE)
#define   MASK_SHOT_PORTAL              (CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_WINDOW|CONTENTS_MONSTER)
#define   MASK_SOLID_BRUSHONLY          (CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_WINDOW|CONTENTS_GRATE)
#define   MASK_PLAYERSOLID_BRUSHONLY    (CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_WINDOW|CONTENTS_PLAYERCLIP|CONTENTS_GRATE)
#define   MASK_NPCSOLID_BRUSHONLY       (CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_WINDOW|CONTENTS_MONSTERCLIP|CONTENTS_GRATE)
#define   MASK_NPCWORLDSTATIC           (CONTENTS_SOLID|CONTENTS_WINDOW|CONTENTS_MONSTERCLIP|CONTENTS_GRATE)
#define   MASK_NPCWORLDSTATIC_FLUID     (CONTENTS_SOLID|CONTENTS_WINDOW|CONTENTS_MONSTERCLIP)
#define   MASK_SPLITAREAPORTAL          (CONTENTS_WATER|CONTENTS_SLIME)
#define   MASK_CURRENT                  (CONTENTS_CURRENT_0|CONTENTS_CURRENT_90|CONTENTS_CURRENT_180|CONTENTS_CURRENT_270|CONTENTS_CURRENT_UP|CONTENTS_CURRENT_DOWN)
#define   MASK_DEADSOLID                (CONTENTS_SOLID|CONTENTS_PLAYERCLIP|CONTENTS_WINDOW|CONTENTS_GRATE)
#pragma endregion

class IHandleEntity;
struct Ray_t;
class CGameTrace;
class IClientNetworkable
{
public:
	virtual IClientUnknown* GetIClientUnknown() = 0;
	virtual void             Release() = 0;
	virtual c_client_class* GetClientClass() = 0;
	virtual void             NotifyShouldTransmit(int state) = 0;
	virtual void             OnPreDataChanged(int updateType) = 0;
	virtual void             OnDataChanged(int updateType) = 0;
	virtual void             PreDataUpdate(int updateType) = 0;
	virtual void             PostDataUpdate(int updateType) = 0;
	virtual void             __unkn(void) = 0;
	virtual bool             IsDormant(void) = 0;
	virtual int              EntIndex(void) const = 0;
	virtual void             ReceiveMessage(int classID, bf_read& msg) = 0;
	virtual void* GetDataTableBasePtr() = 0;
	virtual void             SetDestroyedOnRecreateEntities(void) = 0;
};

class IClientUnknown;
class CClientThinkHandlePtr;
typedef CClientThinkHandlePtr* ClientThinkHandle_t;

class IClientThinkable
{
public:
	virtual IClientUnknown* GetIClientUnknown() = 0;
	virtual void                ClientThink() = 0;
	virtual ClientThinkHandle_t GetThinkHandle() = 0;
	virtual void                SetThinkHandle(ClientThinkHandle_t hThink) = 0;
	virtual void                Release() = 0;
};

class IClientEntity : public IClientUnknown, public IClientRenderable, public IClientNetworkable, public IClientThinkable
{
public:
	// Delete yourself.
	virtual void			Release(void) = 0;

	// Network origin + angles
	virtual const vec3& GetAbsOrigin(void) const = 0;
	virtual const vec3& GetAbsAngles(void) const = 0;

};

//typedef CGameTrace trace_t;
class ICollideable;
class ITraceListData;
class CPhysCollide;
struct ray_t {
	vector_aligned m_start; // starting point, centered within the extents
	vector_aligned m_delta; // direction + length of the ray
	vector_aligned m_start_offset; // Add this to m_Start to get the actual ray start
	vector_aligned m_extents; // Describes an axis aligned box extruded along a ray
	const matrix* m_world_axis_transform;
	//const matrix_t *m_pWorldAxisTransform;
	bool m_is_ray; // are the extents zero?
	bool m_is_swept; // is delta != 0?

	void initialize(const vec3& start, const vec3& end) {
		m_delta = end - start;

		m_is_swept = (m_delta.length_sqr() != 0);

		m_extents.x = m_extents.y = m_extents.z = 0.0f;
		m_is_ray = true;

		m_start_offset.x = m_start_offset.y = m_start_offset.z = 0.0f;

		m_start = start;
	}

	void initialize(vec3& vecStart, vec3& vecEnd, vec3 min, vec3 max) {
		m_delta = vecEnd - vecStart;

		m_is_swept = (m_delta.length_sqr() != 0);

		m_extents.x = (max.x - min.x);
		m_extents.y = (max.y - min.y);
		m_extents.z = (max.z - min.z);
		m_is_ray = false;

		m_start_offset.x = m_start_offset.y = m_start_offset.z = 0.0f;

		m_start = vecStart + ((max + min) * 0.5f);
	}

private:
};

struct csurface_t {
	const char* name;
	short surfaceProps;
	unsigned short flags;
};

struct cplane_t {
	vec3 normal;
	float m_dist;
	BYTE m_type;
	BYTE m_sign_bits;
	BYTE m_pad[2];
};

struct trace_t {
	vec3 start;
	vec3 end;
	cplane_t plane;
	float flFraction;
	int contents;
	unsigned short dispFlags;
	bool allsolid;
	bool startSolid;
	float fractionLeftSolid;
	csurface_t surface;
	int hitGroup;
	short physicsBone;
	c_entity* entity;
	int hitbox;

	bool did_hit() const {
		return flFraction < 1.f;
	}

	bool did_hit_world() const {
		return false;
	}

	bool did_hit_non_world_entity() const {
		return entity != NULL && !did_hit_world();
	}
};

enum TraceType_t {
	TRACE_EVERYTHING = 0,
	TRACE_WORLD_ONLY, // NOTE: This does *not* test static props!!!
	TRACE_ENTITIES_ONLY, // NOTE: This version will *not* test static props
	TRACE_EVERYTHING_FILTER_PROPS, // NOTE: This version will pass the IHandleEntity for props through the filter, unlike all other filters
};

class i_trace_filter {
public:
	virtual bool ShouldHitEntity(void* pEntity, int contentsMask) = 0;
	virtual TraceType_t GetTraceType() const = 0;
};

class trace_filter : public i_trace_filter {
public:
	bool ShouldHitEntity(void* pEntityHandle, int contentsMask) {
		return (pEntityHandle != skip);
	}

	TraceType_t GetTraceType() const {
		return TRACE_EVERYTHING;
	}

	void* skip;
};

class trace_filter_one_entity : public i_trace_filter {
public:
	bool ShouldHitEntity(void* pEntityHandle, int contentsMask) {
		return (pEntityHandle == pEntity);
	}

	TraceType_t GetTraceType() const {
		return TRACE_EVERYTHING;
	}

	void* pEntity;
};

class trace_filter_one_entity2 : public i_trace_filter {
public:
	bool ShouldHitEntity(void* pEntityHandle, int contentsMask) {
		return (pEntityHandle == pEntity);
	}

	TraceType_t GetTraceType() const {
		return TRACE_ENTITIES_ONLY;
	}

	void* pEntity;
};

class trace_filter_skip_two_entities : public i_trace_filter {
public:
	trace_filter_skip_two_entities(void* pPassEnt1, void* pPassEnt2) {
		passentity1 = pPassEnt1;
		passentity2 = pPassEnt2;
	}

	virtual bool ShouldHitEntity(void* pEntityHandle, int contentsMask) {
		return !(pEntityHandle == passentity1 || pEntityHandle == passentity2);
	}

	virtual TraceType_t GetTraceType() const {
		return TRACE_EVERYTHING;
	}

	void* passentity1;
	void* passentity2;
};

class trace_filter_skip_one_entity : public i_trace_filter {
public:
	trace_filter_skip_one_entity(void* pPassEnt1) {
		passentity1 = pPassEnt1;
	}

	virtual bool ShouldHitEntity(void* pEntityHandle, int contentsMask) {
		return !(pEntityHandle == passentity1);
	}

	virtual TraceType_t GetTraceType() const {
		return TRACE_EVERYTHING;
	}

	void* passentity1;
};

class trace_entity : public i_trace_filter {
public:
	bool ShouldHitEntity(void* pEntityHandle, int contentsMask) {
		return !(pEntityHandle == pSkip1);
	}

	TraceType_t GetTraceType() const {
		return TRACE_ENTITIES_ONLY;
	}

	void* pSkip1;
};

class trace_world_only : public i_trace_filter {
public:
	bool ShouldHitEntity(void* pEntityHandle, int contentsMask) {
		return false;
	}

	TraceType_t GetTraceType() const {
		return TRACE_EVERYTHING;
	}

	void* pSkip1;
};

class collideable_t;
class trace {
public:
	virtual int get_point_contents(const vec3& pos, int mask = MASK_ALL, IHandleEntity** ent = nullptr) = 0;
	virtual int get_point_contents_world(const vec3& pos, int mask = MASK_ALL) = 0;
	virtual int get_point_contents_collideable(collideable_t* collide, const vec3& pos) = 0;
	virtual void clip_ray_to_entity(const ray_t& ray, unsigned int mask, c_entity* ent, trace_t* trace) = 0;
	virtual void clip_ray_to_collideable(const ray_t& ray, unsigned int mask, collideable_t* collide, trace_t* trace) = 0;
	virtual void trace_ray(const ray_t& ray, unsigned int mask, i_trace_filter* filter, trace_t* trace) = 0;
};

class CBaseHandle { //-V690
public:
	CBaseHandle();
	CBaseHandle(const CBaseHandle& other);
	CBaseHandle(unsigned long value);
	CBaseHandle(int iEntry, int iSerialNumber);

	void Init(int iEntry, int iSerialNumber);
	void Term();

	// Even if this returns true, Get() still can return return a non-null value.
	// This just tells if the handle has been initted with any values.
	bool IsValid() const;

	int GetEntryIndex() const;
	int GetSerialNumber() const;

	int ToInt() const;
	bool operator !=(const CBaseHandle& other) const;
	bool operator ==(const CBaseHandle& other) const;
	bool operator ==(const IHandleEntity* e) const;
	bool operator !=(const IHandleEntity* e) const;
	bool operator <(const CBaseHandle& other) const;
	bool operator <(const IHandleEntity* e) const;

	// Assign a value to the handle.
	const CBaseHandle& operator=(const IHandleEntity* pEntity);
	const CBaseHandle& Set(const IHandleEntity* pEntity);

	// Use this to dereference the handle.
	// Note: this is implemented in game code (ehandle.h)
	inline IHandleEntity* Get() const {
		uint32_t handle = uint32_t(this);
		return (IHandleEntity*)interfaces->entity_list->get_client_entity_handle(handle);
	}

protected:
	// The low NUM_SERIAL_BITS hold the index. If this value is less than MAX_EDICTS, then the entity is networkable.
	// The high NUM_SERIAL_NUM_BITS bits are the serial number.
	unsigned long	m_Index;
};

inline CBaseHandle::CBaseHandle() {
	m_Index = INVALID_EHANDLE_INDEX;
}

inline CBaseHandle::CBaseHandle(const CBaseHandle& other) {
	m_Index = other.m_Index;
}

inline CBaseHandle::CBaseHandle(unsigned long value) {
	m_Index = value;
}

inline CBaseHandle::CBaseHandle(int iEntry, int iSerialNumber) {
	Init(iEntry, iSerialNumber);
}

inline void CBaseHandle::Init(int iEntry, int iSerialNumber) {
	m_Index = iEntry | (iSerialNumber << NUM_ENT_ENTRY_BITS);
}

inline void CBaseHandle::Term() {
	m_Index = INVALID_EHANDLE_INDEX;
}

inline bool CBaseHandle::IsValid() const {
	return m_Index != INVALID_EHANDLE_INDEX;
}

inline int CBaseHandle::GetEntryIndex() const {
	return m_Index & ENT_ENTRY_MASK;
}

inline int CBaseHandle::GetSerialNumber() const {
	return m_Index >> NUM_ENT_ENTRY_BITS;
}

inline int CBaseHandle::ToInt() const {
	return (int)m_Index;
}

inline bool CBaseHandle::operator !=(const CBaseHandle& other) const {
	return m_Index != other.m_Index;
}

inline bool CBaseHandle::operator ==(const CBaseHandle& other) const {
	return m_Index == other.m_Index;
}

inline bool CBaseHandle::operator ==(const IHandleEntity* e) const {
	return Get() == e;
}

inline bool CBaseHandle::operator !=(const IHandleEntity* e) const {
	return Get() != e;
}

inline bool CBaseHandle::operator <(const CBaseHandle& other) const {
	return m_Index < other.m_Index;
}

inline bool CBaseHandle::operator <(const IHandleEntity* pEntity) const {
	unsigned long otherIndex = (pEntity) ? pEntity->GetRefEHandle().m_Index : INVALID_EHANDLE_INDEX;
	return m_Index < otherIndex;
}

inline const CBaseHandle& CBaseHandle::operator=(const IHandleEntity* pEntity) {
	return Set(pEntity);
}

inline const CBaseHandle& CBaseHandle::Set(const IHandleEntity* pEntity) {
	if (pEntity) {
		*this = pEntity->GetRefEHandle();
	}
	else {
		m_Index = INVALID_EHANDLE_INDEX;
	}

	return *this;
}

// -------------------------------------------------------------------------------------------------- //
// CHandle.
// -------------------------------------------------------------------------------------------------- //
template< class T >
class CHandle : public CBaseHandle {
public:

	CHandle();
	CHandle(int iEntry, int iSerialNumber);
	CHandle(const CBaseHandle& handle);
	CHandle(T* pVal);

	// The index should have come from a call to ToInt(). If it hasn't, you're in trouble.
	static CHandle<T> FromIndex(int index);

	T* Get() const;
	void	Set(const T* pVal);

	operator T* ();
	operator T* () const;

	bool	operator !() const;
	bool	operator==(T* val) const;
	bool	operator!=(T* val) const;
	const CBaseHandle& operator=(const T* val);

	T* operator->() const;
};


// ----------------------------------------------------------------------- //
// Inlines.
// ----------------------------------------------------------------------- //

template<class T>
CHandle<T>::CHandle() { }


template<class T>
CHandle<T>::CHandle(int iEntry, int iSerialNumber) {
	Init(iEntry, iSerialNumber);
}


template<class T>
CHandle<T>::CHandle(const CBaseHandle& handle)
	: CBaseHandle(handle) { }


template<class T>
CHandle<T>::CHandle(T* pObj) {
	Term();
	Set(pObj);
}


template<class T>
inline CHandle<T> CHandle<T>::FromIndex(int index) {
	CHandle<T> ret;
	ret.m_Index = index;
	return ret;
}


template<class T>
inline T* CHandle<T>::Get() const {
	return (T*)CBaseHandle::Get();
}

template<class T>
inline CHandle<T>::operator T* () {
	return Get();
}

template<class T>
inline CHandle<T>::operator T* () const {
	return Get();
}


template<class T>
inline bool CHandle<T>::operator !() const {
	return !Get();
}

template<class T>
inline bool CHandle<T>::operator==(T* val) const {
	return Get() == val;
}

template<class T>
inline bool CHandle<T>::operator!=(T* val) const {
	return Get() != val;
}

template<class T>
void CHandle<T>::Set(const T* pVal) {
	CBaseHandle::Set(reinterpret_cast<const IHandleEntity*>(pVal));
}

template<class T>
inline const CBaseHandle& CHandle<T>::operator=(const T* val) {
	Set(val);
	return *this;
}

template<class T>
T* CHandle<T>::operator -> () const {
	return Get();
}

class c_animstate
{
public:
	char pad[3];
	char bUnknown; //0x4
	char pad2[87];
	void* m_pLastBoneSetupWeapon; //0x5C
	void* m_pBaseEntity; //0x60
	void* m_pActiveWeapon; //0x64
	void* m_pLastActiveWeapon; //0x68
	float m_flLastClientSideAnimationUpdateTime; //0x6C
	int m_iLastClientSideAnimationUpdateFramecount; //0x70
	float m_flUpdateTimeDelta; //0x74
	float m_flEyeYaw; //0x78
	float m_flPitch; //0x7C
	float m_flGoalFeetYaw; //0x80
	float m_flCurrentFeetYaw; //0x84
	float m_flCurrentTorsoYaw; //0x88
	float m_flUnknownVelocityLean; //0x8C //changes when moving/jumping/hitting ground
	float m_flLeanAmount; //0x90
	char pad4[4]; //NaN
	float m_flFeetCycle; //0x98 0 to 1
	float m_flFeetYawRate; //0x9C 0 to 1
	float m_fUnknown2;
	float m_fDuckAmount; //0xA4
	float m_fLandingDuckAdditiveSomething; //0xA8
	float m_fUnknown3; //0xAC
	vec3 m_vOrigin; //0xB0, 0xB4, 0xB8
	vec3 m_vLastOrigin; //0xBC, 0xC0, 0xC4
	float m_vVelocityX; //0xC8
	float m_vVelocityY; //0xCC
	char pad5[4];
	float m_flUnknownFloat1; //0xD4 Affected by movement and direction
	char pad6[8];
	float m_flUnknownFloat2; //0xE0 //from -1 to 1 when moving and affected by direction
	float m_flUnknownFloat3; //0xE4 //from -1 to 1 when moving and affected by direction
	float m_unknown; //0xE8
	float m_velocity; //0xEC
	float flUpVelocity; //0xF0
	float m_flSpeedNormalized; //0xF4 //from 0 to 1
	float m_flFeetSpeedForwardsOrSideWays; //0xF8 //from 0 to 2. something  is 1 when walking, 2.something when running, 0.653 when crouch walking
	float m_flFeetSpeedUnknownForwardOrSideways; //0xFC //from 0 to 3. something
	float m_flTimeSinceStartedMoving; //0x100
	float m_flTimeSinceStoppedMoving; //0x104
	bool m_bOnGround; //0x108
	bool m_bInHitGroundAnimation; //0x109
	char pad7[10];
	float m_flLastOriginZ; //0x114
	float m_flHeadHeightOrOffsetFromHittingGroundAnimation; //0x118 from 0 to 1, is 1 when standing
	float m_flStopToFullRunningFraction; //0x11C from 0 to 1, doesnt change when walking or crouching, only running
	char pad8[4]; //NaN
	float m_flMovingFraction; //0x124 affected while jumping and running, or when just jumping, 0 to 1
	char pad9[4]; //NaN
	float m_flUnknown3;
	char pad10[528];

	float& time_since_in_air()
	{
		return *(float*)((uintptr_t)this + 0x110);
	}

	float& yaw_desync_adjustment()
	{
		return *(float*)((uintptr_t)this + 0x334);
	}

	float& foot_yaw()
	{
		return *(float*)((uintptr_t)this + 0x80);
	}
};

class c_weapon;

struct c_weapon_info
{
public:
	char pad_0000[4]; //0x0000
	char* ConsoleName; //0x0004
	char pad_0008[12]; //0x0008
	int iMaxClip1; //0x0014
	char pad_0018[12]; //0x0018
	int iMaxClip2; //0x0024
	char pad_0028[4]; //0x0028
	char* szWorldModel; //0x002C
	char* szViewModel; //0x0030
	char* szDropedModel; //0x0034
	char pad_0038[4]; //0x0038
	char* N00000984; //0x003C
	char pad_0040[56]; //0x0040
	char* szEmptySound; //0x0078
	char pad_007C[4]; //0x007C
	char* szBulletType; //0x0080
	char pad_0084[4]; //0x0084
	char* szHudName; //0x0088
	char* szWeaponName; //0x008C
	char pad_0090[60]; //0x0090
	int WeaponType; //0x00CC
	int iWeaponPrice; //0x00D0
	int iKillAward; //0x00D4
	char* szAnimationPrefex; //0x00D8
	float flCycleTime; //0x00DC
	float flCycleTimeAlt; //0x00E0
	float flTimeToIdle; //0x00E4
	float flIdleInterval; //0x00E8
	bool bFullAuto; //0x00EC
	char pad_00ED[3]; //0x00ED
	int iDamage; //0x00F0
	float flArmorRatio; //0x00F4
	int iBullets; //0x00F8
	float flPenetration; //0x00FC
	float flFlinchVelocityModifierLarge; //0x0100
	float flFlinchVelocityModifierSmall; //0x0104
	float flRange; //0x0108
	float flRangeModifier; //0x010C
	char pad_0110[28]; //0x0110
	int iCrosshairMinDistance; //0x012C
	float flMaxPlayerSpeed; //0x0130
	float flMaxPlayerSpeedAlt; //0x0134
	char pad_0138[4]; //0x0138
	float flSpread; //0x013C
	float flSpreadAlt; //0x0140
	float flInaccuracyCrouch; //0x0144
	float flInaccuracyCrouchAlt; //0x0148
	float flInaccuracyStand; //0x014C
	float flInaccuracyStandAlt; //0x0150
	float flInaccuracyJumpIntial; //0x0154
	float flInaccaurcyJumpApex;
	float flInaccuracyJump; //0x0158
	float flInaccuracyJumpAlt; //0x015C
	float flInaccuracyLand; //0x0160
	float flInaccuracyLandAlt; //0x0164
	float flInaccuracyLadder; //0x0168
	float flInaccuracyLadderAlt; //0x016C
	float flInaccuracyFire; //0x0170
	float flInaccuracyFireAlt; //0x0174
	float flInaccuracyMove; //0x0178
	float flInaccuracyMoveAlt; //0x017C
	float flInaccuracyReload; //0x0180
	int iRecoilSeed; //0x0184
	float flRecoilAngle; //0x0188
	float flRecoilAngleAlt; //0x018C
	float flRecoilVariance; //0x0190
	float flRecoilAngleVarianceAlt; //0x0194
	float flRecoilMagnitude; //0x0198
	float flRecoilMagnitudeAlt; //0x019C
	float flRecoilMagnatiudeVeriance; //0x01A0
	float flRecoilMagnatiudeVerianceAlt; //0x01A4
	float flRecoveryTimeCrouch; //0x01A8
	float flRecoveryTimeStand; //0x01AC
	float flRecoveryTimeCrouchFinal; //0x01B0
	float flRecoveryTimeStandFinal; //0x01B4
	int iRecoveryTransititionStartBullet; //0x01B8
	int iRecoveryTransititionEndBullet; //0x01BC
	bool bUnzoomAfterShot; //0x01C0
	char pad_01C1[31]; //0x01C1
	char* szWeaponClass; //0x01E0
	char pad_01E4[56]; //0x01E4
	float flInaccuracyPitchShift; //0x021C
	float flInaccuracySoundThreshold; //0x0220
	float flBotAudibleRange; //0x0224
	char pad_0228[12]; //0x0228
	bool bHasBurstMode; //0x0234
};

class c_viewmodel;

class c_animlayer
{
public:
	bool m_bClientBlend;		 //0x0000
	float m_flBlendIn;			 //0x0004
	void* m_pStudioHdr;			 //0x0008
	int m_nDispatchSequence;     //0x000C
	int m_nDispatchSequence_2;   //0x0010
	uint32_t m_nOrder;           //0x0014
	uint32_t m_nSequence;        //0x0018
	float_t m_flPrevCycle;       //0x001C
	float_t m_flWeight;          //0x0020
	float_t m_flWeightDeltaRate; //0x0024
	float_t m_flPlaybackRate;    //0x0028
	float_t m_flCycle;           //0x002C
	void* m_pOwner;              //0x0030
	char pad_0038[4];            //0x0034
};


class c_entity : public IClientEntity
{
public:
	int get_health();
	int get_hitbox_set();
	int get_armor();
	int& get_flags();
	c_utl_vector<matrix>& get_bone_cache();
	int get_life_state();
	int get_fov();
	bool is_alive();
	int fov();
	int default_fov();
	bool has_helmet();
	int& m_take_damage();
	bool compute_hitbox_surrounding_box(vec3* mins, vec3* maxs);
	int get_move_type();
	vec3& get_velocity();
	bool is_player();
    bool is_scoped();
    c_animstate* get_animstate();
    c_weapon* get_weapon();
	c_viewmodel* get_viewmodel();
    datamap_t* get_pred_desc_map();
	int get_sequence();
    datamap_t* get_data_desc_map();
    int& get_tickbase();
    int& get_effects();
    int& get_e_flags();
    vec3& get_punch_angle();
	int& shots_fired();
	vec3 get_bone_position(matrix* mat, int i);
	void* animating();
	model_t* get_model();
	studiohdr_t* get_studio_model();
	mstudiohitboxset_t* hitbox_set_2();
	mstudiobbox_t* get_hitbox(int nHitbox);
	vec3 get_bone_position(int nHitbox);
	void invalidate_bone_cache();
	vec3 get_eye_position();
    vec3& get_punch_angle_vel();
    vec3& get_view_punch_angle();
    bool& get_clientside_animation();
    void update_clientside_animation();
    void create_anim_state(c_animstate* state);
    void reset_anim_state(c_animstate* state);
    void update_anim_state(c_animstate* state, const vec3& ang);
    void set_abs_origin(const vec3& origin);
	int& get_model_bone_counter();
	int& get_model_recent_bone_counter();
    void set_abs_angles(const vec3& ang);
    std::array<float, 24>& get_pose_parameter();
    vec3& get_vec_origin();
	vec3& get_view_offset();
	const matrix& m_rgflCoordinateFrame();
    vec3& get_eye_angles();
    vec3& get_abs_velocity();
	int get_team();
    bool is_dormant();
	c_animlayer* get_anim_overlays();
	c_animlayer* get_anim_overlay(int index);
	float get_spawn_time();
    bool has_gun_game_immunity();
	bool has_heavy_armor();
	bool& has_spotted();
    bool setup_bones(matrix* mat, int max_bones, int bone_mask, float cur_time);
    float& get_simulation_time();
	float& get_old_simulation_time();
    float& get_velocity_modifier();
    float& get_last_shot_time();
};

enum weapons
{
	weapon_none,
	weapon_deagle,
	weapon_elite,
	weapon_fiveseven,
	weapon_glock,
	weapon_ak47 = 7,
	weapon_aug,
	weapon_awp,
	weapon_famas,
	weapon_g3sg1,
	weapon_galilar = 13,
	weapon_m249,
	weapon_m4a1 = 16,
	weapon_mac10,
	weapon_p90 = 19,
	weapon_zone_repulsor,
	weapon_mp5sd = 23,
	weapon_ump45,
	weapon_xm1014,
	weapon_bizon,
	weapon_mag7,
	weapon_negev,
	weapon_sawedoff,
	weapon_tec9,
	weapon_taser,
	weapon_hkp2000,
	weapon_mp7,
	weapon_mp9,
	weapon_nova,
	weapon_p250,
	weapon_shield,
	weapon_scar20,
	weapon_sg553,
	weapon_ssg08,
	weapon_knifegg,
	weapon_knife,
	weapon_flashbang,
	weapon_hegrenade,
	weapon_smokegrenade,
	weapon_molotov,
	weapon_decoy,
	weapon_incgrenade,
	weapon_c4,
	weapon_healthshot = 57,
	weapon_knife_t = 59,
	weapon_m4a1_silencer,
	weapon_usp_silencer,
	weapon_cz75a = 63,
	weapon_revolver,
	weapon_tagrenade = 68,
	weapon_fists,
	weapon_breachcharge,
	weapon_tablet = 72,
	weapon_melee = 74,
	weapon_axe,
	weapon_hammer,
	weapon_spanner = 78,
	weapon_knife_ghost = 80,
	weapon_firebomb,
	weapon_diversion,
	weapon_frag_grenade,
	weapon_snowball,
	weapon_bumpmine,
	weapon_knife_bayonet = 500,
	weapon_knife_css = 503,
	weapon_knife_flip = 505,
	weapon_knife_gut,
	weapon_knife_karambit,
	weapon_knife_m9_bayonet,
	weapon_knife_tactical,
	weapon_knife_falchion = 512,
	weapon_knife_survival_bowie = 514,
	weapon_knife_butterfly,
	weapon_knife_push,
	weapon_knife_cord,
	weapon_knife_canis,
	weapon_knife_ursus,
	weapon_knife_gypsy_jackknife,
	weapon_knife_outdoor,
	weapon_knife_stiletto,
	weapon_knife_widowmaker,
	weapon_knife_skeleton = 525,
	glove_studded_bloodhound = 5027,
	glove_t_side = 5028,
	glove_ct_side = 5029,
	glove_sporty = 5030,
	glove_slick = 5031,
	glove_leather_wrap = 5032,
	glove_motorcycle = 5033,
	glove_specialist = 5034,
	glove_hydra = 5035
};

class c_weapon
{
public:
    float get_spread();
    float get_inaccuracy();
    float get_next_attack();
    float get_next_primary_attack();
	float post_pone_fire_ready_time();
    float get_next_secondary_attack();
    void update_accuracy_penalty();
	int get_ammo();
    c_weapon_info* get_weapon_data();
	bool is_knife();
	bool is_grenade();
	short get_item_definition_index();
};

class c_viewmodel : public c_entity
{
public:
	int get_viewmodel_index();
	int get_animation_party();
	float& get_cycle();
	float& get_animtime();
	CHandle<c_weapon> get_weapon_handle();
};