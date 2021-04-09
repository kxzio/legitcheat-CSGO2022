#pragma once

#include "utils.h"
#include "vector2.h"
#include "vector3.h"
#include "model_cache.h"

#define VIRTUAL(function_name, index, type, ...) \
auto function_name { \
return getvfunc <type> (this, index)(this, __VA_ARGS__); \
};

enum PreviewImageRetVal_t
{
    MATERIAL_PREVIEW_IMAGE_BAD = 0,
    MATERIAL_PREVIEW_IMAGE_OK,
    MATERIAL_NO_PREVIEW_IMAGE,
};

enum MaterialVarFlags_t
{
    MATERIAL_VAR_DEBUG = (1 << 0),
    MATERIAL_VAR_NO_DEBUG_OVERRIDE = (1 << 1),
    MATERIAL_VAR_NO_DRAW = (1 << 2),
    MATERIAL_VAR_USE_IN_FILLRATE_MODE = (1 << 3),
    MATERIAL_VAR_VERTEXCOLOR = (1 << 4),
    MATERIAL_VAR_VERTEXALPHA = (1 << 5),
    MATERIAL_VAR_SELFILLUM = (1 << 6),
    MATERIAL_VAR_ADDITIVE = (1 << 7),
    MATERIAL_VAR_ALPHATEST = (1 << 8),
    MATERIAL_VAR_ZNEARER = (1 << 10),
    MATERIAL_VAR_MODEL = (1 << 11),
    MATERIAL_VAR_FLAT = (1 << 12),
    MATERIAL_VAR_NOCULL = (1 << 13),
    MATERIAL_VAR_NOFOG = (1 << 14),
    MATERIAL_VAR_IGNOREZ = (1 << 15),
    MATERIAL_VAR_DECAL = (1 << 16),
    MATERIAL_VAR_ENVMAPSPHERE = (1 << 17), // OBSOLETE
    MATERIAL_VAR_UNUSED = (1 << 18), // UNUSED
    MATERIAL_VAR_ENVMAPCAMERASPACE = (1 << 19), // OBSOLETE
    MATERIAL_VAR_BASEALPHAENVMAPMASK = (1 << 20),
    MATERIAL_VAR_TRANSLUCENT = (1 << 21),
    MATERIAL_VAR_NORMALMAPALPHAENVMAPMASK = (1 << 22),
    MATERIAL_VAR_NEEDS_SOFTWARE_SKINNING = (1 << 23), // OBSOLETE
    MATERIAL_VAR_OPAQUETEXTURE = (1 << 24),
    MATERIAL_VAR_ENVMAPMODE = (1 << 25), // OBSOLETE
    MATERIAL_VAR_SUPPRESS_DECALS = (1 << 26),
    MATERIAL_VAR_HALFLAMBERT = (1 << 27),
    MATERIAL_VAR_WIREFRAME = (1 << 28),
    MATERIAL_VAR_ALLOWALPHATOCOVERAGE = (1 << 29),
    MATERIAL_VAR_ALPHA_MODIFIED_BY_PROXY = (1 << 30),
    MATERIAL_VAR_VERTEXFOG = (1 << 31),
};
class IMaterial;
struct studioloddata_t
{
    // not needed - this is really the same as studiohwdata_t.m_NumStudioMeshes
    //int					m_NumMeshes; 
    void* m_pMeshData; // there are studiohwdata_t.m_NumStudioMeshes of these.
    float				m_SwitchPoint;
    // one of these for each lod since we can switch to simpler materials on lower lods.
    int					numMaterials;
    IMaterial** ppMaterials; /* will have studiohdr_t.numtextures elements allocated */
                                       // hack - this needs to go away.
    int* pMaterialFlags; /* will have studiohdr_t.numtextures elements allocated */

                                         // For decals on hardware morphing, we must actually do hardware skinning
                                         // For this to work, we have to hope that the total # of bones used by
                                         // hw flexed verts is < than the max possible for the dx level we're running under
    int* m_pHWMorphDecalBoneRemap;
    int					m_nDecalBoneCount;
};
struct studiohwdata_t
{
    int					m_RootLOD;	// calced and clamped, nonzero for lod culling
    int					m_NumLODs;
    studioloddata_t* m_pLODs;
    int					m_NumStudioMeshes;

    inline float LODMetric(float unitSphereSize) const { return (unitSphereSize != 0.0f) ? (100.0f / unitSphereSize) : 0.0f; }
    inline int GetLODForMetric(float lodMetric) const
    {
        if (!m_NumLODs)
            return 0;

        // shadow lod is specified on the last lod with a negative switch
        // never consider shadow lod as viable candidate
        int numLODs = (m_pLODs[m_NumLODs - 1].m_SwitchPoint < 0.0f) ? m_NumLODs - 1 : m_NumLODs;

        for (int i = m_RootLOD; i < numLODs - 1; i++)
        {
            if (m_pLODs[i + 1].m_SwitchPoint > lodMetric)
                return i;
        }

        return numLODs - 1;
    }
};

typedef unsigned short ModelInstanceHandle_t;
typedef void* LightCacheHandle_t;
typedef void* StudioDecalHandle_t;
typedef int OverrideType_t;
typedef int ImageFormat;
typedef int VertexFormat_t;
typedef int MaterialPropertyTypes_t;

class IClientUnknown;
class IClientEntity;
class ICollideable;
class IClientThinkable;
class IClientAlphaProperty;
class IClientNetworkable;
class IClientRenderable;
class IMaterial;
class CStudioHdr;
class IMatRenderContext;
class DataCacheHandle_t;
class ITexture;
class KeyValues;
struct model_t;
struct mstudioanimdesc_t;
struct mstudioseqdesc_t;
struct Ray_t;
struct DrawModelInfo_t;
struct studiohwdata_t;
struct MaterialLightingState_t;
struct ClientShadowHandle_t;
struct ClientRenderHandle_t;
struct ColorMeshInfo_t;

class CBaseHandle;

class IHandleEntity;

class IClientRenderable;

struct RenderableInfo_t
{
    IClientRenderable* m_pRenderable;
    void* m_pAlphaProperty;
    int m_EnumCount;
    int m_nRenderFrame;
    unsigned short m_FirstShadow;
    unsigned short m_LeafList;
    short m_Area;
    uint16_t m_Flags;
    uint16_t m_Flags2;
    vec3 m_vecBloatedAbsMins;
    vec3 m_vecBloatedAbsMaxs;
    vec3 m_vecAbsMins;
    vec3 m_vecAbsMaxs;
    int pad;
};

struct DrawModelState_t
{
    studiohdr_t* m_pStudioHdr;
    studiohwdata_t* m_pStudioHWData;
    IClientRenderable* m_pRenderable;
    const matrix* m_pModelToWorld;
    StudioDecalHandle_t     m_decals;
    int                     m_drawFlags;
    int                     m_lod;
};

struct StaticPropRenderInfo_t
{
    const matrix* pModelToWorld;
    const model_t* pModel;
    IClientRenderable* pRenderable;
    vec3* pLightingOrigin;
    short                   skin;
    ModelInstanceHandle_t   instance;
};

struct ModelRenderInfo_t
{
    vec3                  origin;
    vec3                  angles;
    char pad[0x4];
    IClientRenderable* pRenderable;
    const model_t* pModel;
    const matrix* pModelToWorld;
    const matrix* pLightingOffset;
    const vec3* pLightingOrigin;
    int                     flags;
    int                     entity_index;
    int                     skin;
    int                     body;
    int                     hitboxset;
    ModelInstanceHandle_t   instance;

    ModelRenderInfo_t() //-V730
    {
        pModelToWorld = NULL;
        pLightingOffset = NULL;
        pLightingOrigin = NULL;
    }
};

struct LightingQuery_t
{
    vec3                  m_LightingOrigin;
    ModelInstanceHandle_t   m_InstanceHandle;
    bool                    m_bAmbientBoost;
};

struct StaticLightingQuery_t : public LightingQuery_t
{
    IClientRenderable* m_pRenderable;
};

class IMaterialVar
{
public:
    VIRTUAL(set_float_value(float val), 4, void(__thiscall*)(void*, float), val);
    VIRTUAL(set_int_value(int val), 5, void(__thiscall*)(void*, int), val);
    VIRTUAL(set_string_value(const char* val), 6, void(__thiscall*)(void*, const char*), val);
    VIRTUAL(set_vec_value(float x, float y, float z), 11, void(__thiscall*)(void*, float, float, float), x, y, z);
};

class c_material
{
public:
    virtual const char* GetName() const = 0;
    virtual const char* GetTextureGroupName() const = 0;
    virtual PreviewImageRetVal_t    GetPreviewImageProperties(int* width, int* height, ImageFormat* imageFormat, bool* isTranslucent) const = 0;
    virtual PreviewImageRetVal_t    GetPreviewImage(unsigned char* data, int width, int height, ImageFormat imageFormat) const = 0;
    virtual int                     GetMappingWidth() = 0;
    virtual int                     GetMappingHeight() = 0;
    virtual int                     GetNumAnimationFrames() = 0;
    virtual bool                    InMaterialPage(void) = 0;
    virtual    void                 GetMaterialOffset(float* pOffset) = 0;
    virtual void                    GetMaterialScale(float* pScale) = 0;
    virtual IMaterial* GetMaterialPage(void) = 0;
    virtual IMaterialVar* FindVar(const char* varName, bool* found, bool complain = true) = 0;
    virtual void                    IncrementReferenceCount(void) = 0;
    virtual void                    DecrementReferenceCount(void) = 0;
    inline void                     AddRef() { IncrementReferenceCount(); }
    inline void                     Release() { DecrementReferenceCount(); }
    virtual int                     GetEnumerationID(void) const = 0;
    virtual void                    GetLowResColorSample(float s, float t, float* color) const = 0;
    virtual void                    RecomputeStateSnapshots() = 0;
    virtual bool                    IsTranslucent() = 0;
    virtual bool                    IsAlphaTested() = 0;
    virtual bool                    IsVertexLit() = 0;
    virtual VertexFormat_t          GetVertexFormat() const = 0;
    virtual bool                    HasProxy(void) const = 0;
    virtual bool                    UsesEnvCubemap(void) = 0;
    virtual bool                    NeedsTangentSpace(void) = 0;
    virtual bool                    NeedsPowerOfTwoFrameBufferTexture(bool bCheckSpecificToThisFrame = true) = 0;
    virtual bool                    NeedsFullFrameBufferTexture(bool bCheckSpecificToThisFrame = true) = 0;
    virtual bool                    NeedsSoftwareSkinning(void) = 0;
    virtual void                    AlphaModulate(float alpha) = 0;
    virtual void                    ColorModulate(float r, float g, float b) = 0;
    virtual void                    SetMaterialVarFlag(MaterialVarFlags_t flag, bool on) = 0;
    virtual bool                    GetMaterialVarFlag(MaterialVarFlags_t flag) const = 0;
    virtual void                    GetReflectivity(vec3& reflect) = 0;
    virtual bool                    GetPropertyFlag(MaterialPropertyTypes_t type) = 0;
    virtual bool                    IsTwoSided() = 0;
    virtual void                    SetShader(const char* pShaderName) = 0;
    virtual int                     GetNumPasses(void) = 0;
    virtual int                     GetTextureMemoryBytes(void) = 0;
    virtual void                    Refresh() = 0;
    virtual bool                    NeedsLightmapBlendAlpha(void) = 0;
    virtual bool                    NeedsSoftwareLighting(void) = 0;
    virtual int                     ShaderParamCount() const = 0;
    virtual IMaterialVar** GetShaderParams(void) = 0;
    virtual bool                    IsErrorMaterial() const = 0;
    virtual void                    Unused() = 0;
    virtual float                   GetAlphaModulation() = 0;
    virtual void                    GetColorModulation(float* r, float* g, float* b) = 0;
    virtual bool                    IsTranslucentUnderModulation(float fAlphaModulation = 1.0f) const = 0;
    virtual IMaterialVar* FindVarFast(char const* pVarName, unsigned int* pToken) = 0;
    virtual void                    SetShaderAndParams(KeyValues* pKeyValues) = 0;
    virtual const char* GetShaderName() const = 0;
    virtual void                    DeleteIfUnreferenced() = 0;
    virtual bool                    IsSpriteCard() = 0;
    virtual void                    CallBindProxy(void* proxyData) = 0;
    virtual void                    RefreshPreservingMaterialVars() = 0;
    virtual bool                    WasReloadedFromWhitelist() = 0;
    virtual bool                    SetTempExcluded(bool bSet, int nExcludedDimensionLimit) = 0;
    virtual int                     GetReferenceCount() const = 0;
};

class c_model_info
{
public:
    inline void* get_model(int Index) {
        return getvfunc<void* (__thiscall*)(void*, int)>(this, 1)(this, Index);
    }
    int	get_model_index(const char* name)
    {
        typedef int(__thiscall* oGetModelName)(PVOID, const char*);
        return getvfunc< oGetModelName >(this, 2)(this, name);
    }
    const char* get_model_name(const model_t* mod)
    {
        typedef const char* (__thiscall* oGetModelName)(PVOID, const model_t*);
        return getvfunc< oGetModelName >(this, 3)(this, mod);
    }
    inline const char* get_model_name(const void* Model) {
        return getvfunc<const char* (__thiscall*)(void*, const void*)>(this, 3)(this, Model);
    }
    studiohdr_t* get_studio_model(const model_t* mod)
    {
        typedef studiohdr_t* (__stdcall* oGetStudiomodel)(const model_t*);
        return getvfunc< oGetStudiomodel >(this, 32)(mod);
    }
};
class iclient_renderable {
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
    virtual void* /*IPVSNotify*/       GetPVSNotifyInterface() = 0;
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
    virtual   bool                     GetAttachment(int number, vec3& origin, vec3& angles) = 0;
    virtual bool                       GetAttachment(int number, matrix& matrix) = 0;
    virtual float* GetRenderClipPlane(void) = 0;
    virtual int                        GetSkin() = 0;
    virtual void                       OnThreadedDrawSetup() = 0;
    virtual bool                       UsesFlexDelayedWeights() = 0;
    virtual void                       RecordToolMessage() = 0;
    virtual bool                       ShouldDrawForSplitScreenUser(int nSlot) = 0;
    virtual uint8_t                      OverrideAlphaModulation(uint8_t nAlpha) = 0;
    virtual uint8_t                      OverrideShadowAlphaModulation(uint8_t nAlpha) = 0;
};

class c_model_render
{
public:
    virtual int                     DrawModel(int flags, IClientRenderable* pRenderable, ModelInstanceHandle_t instance, int entity_index, const model_t* model, vec3 const& origin, vec3 const& angles, int skin, int body, int hitboxset, const matrix* modelToWorld = NULL, const matrix* pLightingOffset = NULL) = 0;
    virtual void                    ForcedMaterialOverride(IMaterial* newMaterial, OverrideType_t nOverrideType = 0, int nOverrides = 0) = 0;
    virtual bool                    IsForcedMaterialOverride(void) = 0;
    virtual void                    SetViewTarget(const CStudioHdr* pStudioHdr, int nBodyIndex, const vec3& target) = 0;
    virtual ModelInstanceHandle_t   CreateInstance(IClientRenderable* pRenderable, LightCacheHandle_t* pCache = NULL) = 0;
    virtual void                    DestroyInstance(ModelInstanceHandle_t handle) = 0;
    virtual void                    SetStaticLighting(ModelInstanceHandle_t handle, LightCacheHandle_t* pHandle) = 0;
    virtual LightCacheHandle_t      GetStaticLighting(ModelInstanceHandle_t handle) = 0;
    virtual bool                    ChangeInstance(ModelInstanceHandle_t handle, IClientRenderable* pRenderable) = 0;
    virtual void                    AddDecal(ModelInstanceHandle_t handle, Ray_t const& ray, vec3 const& decalUp, int decalIndex, int body, bool noPokeThru, int maxLODToDecal) = 0;
    virtual void                    RemoveAllDecals(ModelInstanceHandle_t handle) = 0;
    virtual bool                    ModelHasDecals(ModelInstanceHandle_t handle) = 0;
    virtual void                    RemoveAllDecalsFromAllModels() = 0;
    virtual matrix* DrawModelShadowSetup(IClientRenderable* pRenderable, int body, int skin, DrawModelInfo_t* pInfo, matrix* pCustomBoneToWorld = NULL) = 0;
    virtual void                    DrawModelShadow(IClientRenderable* pRenderable, const DrawModelInfo_t& info, matrix* pCustomBoneToWorld = NULL) = 0;
    virtual bool                    RecomputeStaticLighting(ModelInstanceHandle_t handle) = 0;
    virtual void                    ReleaseAllStaticPropColorData(void) = 0;
    virtual void                    RestoreAllStaticPropColorData(void) = 0;
    virtual int                     DrawModelEx(ModelRenderInfo_t& pInfo) = 0;
    virtual int                     DrawModelExStaticProp(ModelRenderInfo_t& pInfo) = 0;
    virtual bool                    DrawModelSetup(ModelRenderInfo_t& pInfo, DrawModelState_t* pState, matrix** ppBoneToWorldOut) = 0;
    virtual void                    DrawModelExecute(IMatRenderContext* ctx, const DrawModelState_t& state, const ModelRenderInfo_t& pInfo, matrix* pCustomBoneToWorld = NULL) = 0;
    virtual void                    SetupLighting(const vec3& vecCenter) = 0;
    virtual int                     DrawStaticPropArrayFast(StaticPropRenderInfo_t* pProps, int count, bool bShadowDepth) = 0;
    virtual void                    SuppressEngineLighting(bool bSuppress) = 0;
    virtual void                    SetupColorMeshes(int nTotalVerts) = 0;
    virtual void                    SetupLightingEx(const vec3& vecCenter, ModelInstanceHandle_t handle) = 0;
    virtual bool                    GetBrightestShadowingLightSource(const vec3& vecCenter, vec3& lightPos, vec3& lightBrightness, bool bAllowNonTaggedLights) = 0;
    virtual void                    ComputeLightingState(int nCount, const LightingQuery_t* pQuery, MaterialLightingState_t* pState, ITexture** ppEnvCubemapTexture) = 0;
    virtual void                    GetModelDecalHandles(StudioDecalHandle_t* pDecals, int nDecalStride, int nCount, const ModelInstanceHandle_t* pHandles) = 0;
    virtual void                    ComputeStaticLightingState(int nCount, const StaticLightingQuery_t* pQuery, MaterialLightingState_t* pState, MaterialLightingState_t* pDecalState, ColorMeshInfo_t** ppStaticLighting, ITexture** ppEnvCubemapTexture, DataCacheHandle_t* pColorMeshHandles) = 0;
    virtual void                    CleanupStaticLightingState(int nCount, DataCacheHandle_t* pColorMeshHandles) = 0;
};