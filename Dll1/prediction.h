#pragma once
#include "vector3.h"
#include "utils.h"
#include "cmd.h"

class c_entity;

class c_move_helper
{
	virtual void UnknownVirtual() = 0;
public:
	virtual void set_host(c_entity* host) = 0;
private:
	virtual void pad00() = 0;
	virtual void pad01() = 0;
public:
	virtual void process_impacts() = 0;
};

class c_move_data {
public:
	bool    m_bFirstRunOfFunctions : 1;
	bool    m_bGameCodeMovedPlayer : 1;
	int     m_nPlayerHandle;        // edict index on server, client entity handle on client=
	int     m_nImpulseCommand;      // Impulse command issued.
	vec3  m_vecViewAngles;        // Command view angles (local space)
	vec3  m_vecAbsViewAngles;     // Command view angles (world space)
	int     m_nButtons;             // Attack buttons.
	int     m_nOldButtons;          // From host_client->oldbuttons;
	float   m_flForwardMove;
	float   m_flSideMove;
	float   m_flUpMove;
	float   m_flMaxSpeed;
	float   m_flClientMaxSpeed;
	vec3  m_vecVelocity;          // edict::velocity        // Current movement direction.
	vec3  m_vecAngles;            // edict::angles
	vec3  m_vecOldAngles;
	float   m_outStepHeight;        // how much you climbed this move
	vec3  m_outWishVel;           // This is where you tried 
	vec3  m_outJumpVel;           // This is your jump velocity
	vec3  m_vecConstraintCenter;
	float   m_flConstraintRadius;
	float   m_flConstraintWidth;
	float   m_flConstraintSpeedFactor;
	float   m_flUnknown[5];
	vec3  m_vecAbsOrigin;
};

class c_game_movement
{
public:
	virtual			~c_game_movement(void) {}

	virtual void	process_movement(c_entity* pPlayer, c_move_data* pMove) = 0;
	virtual void	reset(void) = 0;
	virtual void	start_track_prediction_errors(c_entity* pPlayer) = 0;
	virtual void	finish_track_prediction_errors(c_entity* pPlayer) = 0;
	virtual void	diff_print(char const* fmt, ...) = 0;

	virtual vec3 const& get_player_mins(bool ducked) const = 0;
	virtual vec3 const& get_player_maxs(bool ducked) const = 0;
	virtual vec3 const& get_player_view_offset(bool ducked) const = 0;

	virtual bool			is_moving_player_stuck(void) const = 0;
	virtual c_entity*		get_moving_player(void) const = 0;
	virtual void			ublock_pusher(c_entity* pPlayer, c_entity* pPusher) = 0;

	virtual void    setup_movement_bounds(c_move_data* pMove) = 0;
};

class c_prediction
{
public:
	void update(int start_frame, bool valid_frame, int inc_ack, int out_cmd)
	{
		typedef void(__thiscall* oUpdate)(void*, int, bool, int, int);
		return getvfunc<oUpdate>(this, 3)(this, start_frame, valid_frame, inc_ack, out_cmd);
	}

	void check_moving_ground(c_entity* player, double frametime)
	{
		typedef void(__thiscall* oCheckMovingGround)(void*, c_entity*, double);
		return getvfunc<oCheckMovingGround>(this, 18)(this, player, frametime);
	}

	void run_command(c_entity* player, c_entity* ucmd, c_move_helper* moveHelper)
	{
		typedef void(__thiscall* oRunCommand)(void*, c_entity*, c_entity*, c_move_helper*);
		return getvfunc<oRunCommand>(this, 19)(this, player, ucmd, moveHelper);
	}

	void setup_move(c_entity* player, c_usercmd* ucmd, c_move_helper* moveHelper, void* pMoveData)
	{
		typedef void(__thiscall* oSetupMove)(void*, c_entity*, c_usercmd*, c_move_helper*, void*);
		return getvfunc<oSetupMove>(this, 20)(this, player, ucmd, moveHelper, pMoveData);
	}

	void finish_move(c_entity* player, c_usercmd* ucmd, void* pMoveData)
	{
		typedef void(__thiscall* oFinishMove)(void*, c_entity*, c_usercmd*, void*);
		return getvfunc<oFinishMove>(this, 21)(this, player, ucmd, pMoveData);
	}

	void set_local_view_angles(vec3& Angles)
	{
		typedef void(__thiscall* Fn)(void*, vec3&);
		return getvfunc<Fn>(this, 13)(this, Angles);
	}

	char pad00[8]; 			
	bool InPrediction;		
	char pad01[1];			
	bool EnginePaused;		
	char pad02[13];			
	bool IsFirstTimePredicted;
};