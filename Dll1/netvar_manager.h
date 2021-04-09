#pragma once
#include <memory>
#include <vector>
#include <sstream>
#include "client_class.h"
#include "client.h"

namespace offsets
{
	extern uintptr_t m_fFlags;
	extern uintptr_t m_iHealth;
	extern uintptr_t m_lifeState;
	extern uintptr_t m_vecVelocity;
	extern uintptr_t m_bIsScoped;
	extern uintptr_t m_hActiveWeapon;
	extern uintptr_t m_nTickBase;
	extern uintptr_t m_aimPunchAngle;
	extern uintptr_t m_aimPunchAngleVel;
	extern uintptr_t m_viewPunchAngle;
	extern uintptr_t m_bClientSideAnimation;
	extern uintptr_t m_flPoseParameter;
	extern uintptr_t m_vecOrigin;
	extern uintptr_t m_angEyeAngles;
	extern uintptr_t m_bGunGameImmunity;
	extern uintptr_t m_bHasHeavyArmor;
	extern uintptr_t m_flSimulationTime;
	extern uintptr_t m_flVelocityModifier;
	extern uintptr_t m_fLastShotTime;
	extern uintptr_t m_iItemDefinitionIndex;
	extern uintptr_t m_flNextPrimaryAttack;
	extern uintptr_t m_flNextSecondaryAttack;
	extern uintptr_t m_flPostponeFireReadyTime;
	extern uintptr_t m_hViewModel;
	extern uintptr_t m_nViewModelIndex;
	extern uintptr_t m_nSequence;
	extern uintptr_t m_nAnimationParity;
	extern uintptr_t m_hWeapon;
	extern uintptr_t deadflag;
	extern uintptr_t m_iTeamNum;
	extern uintptr_t m_vecViewOffset;
	extern uintptr_t m_CollisionGroup;
	extern uintptr_t m_bHasHelmet;
	extern uintptr_t m_ArmorValue;
	extern uintptr_t m_iClip1;
	extern uintptr_t m_bSpotted;
	extern uintptr_t m_nForceBone;
	extern uintptr_t m_nHitboxSet;
	extern uintptr_t m_iDefaultFOV; //"DT_BasePlayer", "m_iDefaultFOV");
	extern uintptr_t m_iFOV;// "DT_BasePlayer", "m_iFOV");
	extern uintptr_t m_iShotsFired;
}

class c_netvar_manager
{
public:
	void init();
	void parse_offsets();
	int get_offset(const char* tableName, const char* propName);
	bool hook_prop(const char* tableName, const char* propName, recv_var_proxy_fn fun);
private:
	int get_prop(const char* tableName, const char* propName, recv_prop** prop = 0);
	int get_prop(recv_table* recvTable, const char* propName, recv_prop** prop = 0);
	recv_table* get_table(const char* tableName);
	std::vector< recv_table* > m_tables;
};

extern c_netvar_manager* netvar_manager;