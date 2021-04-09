#pragma once
#include <iostream>
#include "client_class.h"
#include "utils.h"
#include "vector3.h"

enum frame_stage
{
	frame_undefined = -1,
	frame_start,
	frame_net_update_start,
	frame_net_update_postdataupdate_start,
	frame_net_update_postdataupdate_end,
	frame_net_update_end,
	frame_render_start,
	frame_render_end
};

class c_global_vars_base
{
public:
    float     realtime;                     // 0x0000
    int       framecount;                   // 0x0004
    float     absoluteframetime;            // 0x0008
    float     absoluteframestarttimestddev; // 0x000C
    float     curtime;                      // 0x0010
    float     frametime;                    // 0x0014
    int       maxclients;                   // 0x0018
    int       tickcount;                    // 0x001C
    float     intervalpertick;              // 0x0020
    float     interpolation_amount;         // 0x0024
    int       simticksthisframe;            // 0x0028
    int       networkprotocol;              // 0x002C
    void*     savedata;                     // 0x0030
    bool      client;						  // 0x0031
    bool      remoteclient;                 // 0x0032
    int		  timestampnetworkingbase;	  // 0x0036
    int		  timestamprandomizewindow;	  // 0x003A
};

class c_base_client_dll {
public:
	c_client_class* get_client_classes() {
		using original_fn = c_client_class * (__thiscall*)(c_base_client_dll*);
		return getvfunc<original_fn>(this, 8)(this); /*(*(original_fn**)this)[8](this);*/
	}

	bool dispatch_user_message(int msg_type, unsigned int arg1, unsigned int length, const void* data = nullptr) {
		using original_fn = bool(__thiscall*)(void*, int, unsigned int, unsigned int, const void*);
		return getvfunc<original_fn>(this, 38)(this, msg_type, arg1, length, data); /*(*(original_fn**)this)[38](this, msg_type, arg1, length, data);*/
	}
};

class IPanel;
class c_entity;

enum class ClearFlags_t
{
    VIEW_CLEAR_COLOR = 0x1,
    VIEW_CLEAR_DEPTH = 0x2,
    VIEW_CLEAR_FULL_TARGET = 0x4,
    VIEW_NO_DRAW = 0x8,
    VIEW_CLEAR_OBEY_STENCIL = 0x10,
    VIEW_CLEAR_STENCIL = 0x20,
};

enum class MotionBlurMode_t
{
    MOTION_BLUR_DISABLE = 1,
    MOTION_BLUR_GAME = 2,
    MOTION_BLUR_SFM = 3
};

class c_view_setup
{
public:
    __int32   x;                  //0x0000 
    __int32   x_old;              //0x0004 
    __int32   y;                  //0x0008 
    __int32   y_old;              //0x000C 
    __int32   width;              //0x0010 
    __int32   width_old;          //0x0014 
    __int32   height;             //0x0018 
    __int32   height_old;         //0x001C 
    char      pad_0x0020[0x90];   //0x0020
    float     fov;                //0x00B0 
    float     viewmodel_fov;      //0x00B4 
    vec3    origin;             //0x00B8 
    vec3    angles;             //0x00C4 
    char      pad_0x00D0[0x7C];   //0x00D0

};//Size=0x014C

class c_client_mode
{
public:
    virtual             ~c_client_mode() {}
    virtual int         ClientModeCSNormal(void*) = 0;
    virtual void        Init() = 0;
    virtual void        InitViewport() = 0;
    virtual void        Shutdown() = 0;
    virtual void        Enable() = 0;
    virtual void        Disable() = 0;
    virtual void        Layout() = 0;
    virtual IPanel* GetViewport() = 0;
    virtual void* GetViewportAnimationController() = 0;
    virtual void        ProcessInput(bool bActive) = 0;
    virtual bool        ShouldDrawDetailObjects() = 0;
    virtual bool        ShouldDrawEntity(c_entity* e) = 0;
    virtual bool        ShouldDrawLocalPlayer(c_entity* pPlayer) = 0;
    virtual bool        ShouldDrawParticles() = 0;
    virtual bool        ShouldDrawFog(void) = 0;
    virtual void        OverrideView(c_view_setup* pSetup) = 0;
    virtual int         KeyInput(int down, int keynum, const char* pszCurrentBinding) = 0;
    virtual void        StartMessageMode(int iMessageModeType) = 0;
    virtual IPanel* GetMessagePanel() = 0;
    virtual void        OverrideMouseInput(float* x, float* y) = 0;
    virtual bool        CreateMove(float flInputSampleTime, void* usercmd) = 0;
    virtual void        LevelInit(const char* newmap) = 0;
    virtual void        LevelShutdown(void) = 0;
};