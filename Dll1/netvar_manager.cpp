#include "netvar_manager.h"
#include "interfaces.h"

c_netvar_manager* netvar_manager = new c_netvar_manager();

namespace offsets
{
	uintptr_t m_fFlags;
	uintptr_t m_iHealth;
	uintptr_t m_lifeState;
	uintptr_t m_vecVelocity;
	uintptr_t m_bIsScoped;
	uintptr_t m_hActiveWeapon;
	uintptr_t m_nTickBase;
	uintptr_t m_aimPunchAngle;
	uintptr_t m_aimPunchAngleVel;
	uintptr_t m_viewPunchAngle;
	uintptr_t m_bClientSideAnimation;
	uintptr_t m_flPoseParameter;
	uintptr_t m_vecOrigin;
	uintptr_t m_angEyeAngles;
	uintptr_t m_iTeamNum;
	uintptr_t m_bGunGameImmunity;
	uintptr_t m_bHasHeavyArmor;
	uintptr_t m_flSimulationTime;
	uintptr_t m_flVelocityModifier;
	uintptr_t m_fLastShotTime;
	uintptr_t m_iItemDefinitionIndex;
	uintptr_t m_flNextPrimaryAttack;
	uintptr_t m_flPostponeFireReadyTime;
	uintptr_t m_flNextSecondaryAttack;
	uintptr_t m_hViewModel;
	uintptr_t m_nViewModelIndex;
	uintptr_t m_nSequence;
	uintptr_t m_nAnimationParity;
	uintptr_t m_hWeapon;
	uintptr_t m_CachedBoneData;
	uintptr_t m_vecViewOffset;
	uintptr_t deadflag;
	uintptr_t m_CollisionGroup;
	uintptr_t m_bHasHelmet;
	uintptr_t m_ArmorValue;
	uintptr_t m_iClip1;
	uintptr_t m_bSpotted;
	uintptr_t m_nForceBone;
	uintptr_t m_nHitboxSet;
	uintptr_t m_iDefaultFOV; //"DT_BasePlayer", "m_iDefaultFOV");
	uintptr_t m_iFOV;// "DT_BasePlayer", "m_iFOV");
	uintptr_t m_iShotsFired;
}

void c_netvar_manager::init()
{
	m_tables.clear();

	c_client_class* clientClass = interfaces->client->get_client_classes();
	if (!clientClass)
		return;

	while (clientClass)
	{
		recv_table* recvTable = clientClass->recvtable_ptr;
		m_tables.push_back(recvTable);

		clientClass = clientClass->next_ptr;
	}
}

void c_netvar_manager::parse_offsets()
{
	offsets::m_fFlags = get_offset("DT_CSPlayer", "m_fFlags");
	offsets::m_iHealth = get_offset("DT_CSPlayer", "m_iHealth");
	offsets::m_lifeState = get_offset("DT_CSPlayer", "m_lifeState");
	offsets::m_bHasHeavyArmor = get_offset("DT_CSPlayer", "m_bHasHeavyArmor");
	offsets::m_vecVelocity = get_offset("DT_BasePlayer", "m_vecVelocity[0]");
	offsets::m_bIsScoped = get_offset("DT_CSPlayer", "m_bIsScoped");
	offsets::m_hActiveWeapon = get_offset("DT_BasePlayer", "m_hActiveWeapon");
	offsets::m_nTickBase = get_offset("DT_BasePlayer", "m_nTickBase");
	offsets::m_aimPunchAngle = get_offset("DT_CSPlayer", "m_aimPunchAngle");
	offsets::m_aimPunchAngleVel = get_offset("DT_CSPlayer", "m_aimPunchAngleVel");
	offsets::m_bClientSideAnimation = get_offset("DT_BaseAnimating", "m_bClientSideAnimation");
	offsets::m_flPoseParameter = get_offset("DT_BaseAnimating", "m_flPoseParameter");
	offsets::m_nSequence = get_offset("DT_BaseAnimating", "m_nSequence");
	offsets::m_vecOrigin = get_offset("DT_BaseEntity", "m_vecOrigin");
	offsets::m_angEyeAngles = get_offset("DT_CSPlayer", "m_angEyeAngles[0]");
	offsets::m_bGunGameImmunity = get_offset("DT_CSPlayer", "m_bGunGameImmunity");
	offsets::m_flSimulationTime = get_offset("DT_CSPlayer", "m_flSimulationTime");
	offsets::m_flVelocityModifier = get_offset("DT_CSPlayer", "m_flVelocityModifier");
	offsets::m_fLastShotTime = get_offset("DT_WeaponCSBase", "m_fLastShotTime");
	offsets::m_iItemDefinitionIndex = get_offset("DT_BaseAttributableItem", "m_iItemDefinitionIndex");
	offsets::m_hViewModel = get_offset("DT_BasePlayer", "m_hViewModel[0]");
	offsets::m_nViewModelIndex = get_offset("DT_BaseViewModel", "m_nViewModelIndex");
	offsets::m_nAnimationParity = get_offset("DT_BaseViewModel", "m_nAnimationParity");
	offsets::m_hWeapon = get_offset("DT_BaseViewModel", "m_hWeapon");
	offsets::m_iTeamNum = get_offset("DT_BaseEntity", "m_iTeamNum");
	offsets::m_vecViewOffset = get_offset("DT_BasePlayer", "m_vecViewOffset[0]");
	offsets::m_CollisionGroup = get_offset("DT_BaseEntity", "m_CollisionGroup");
	offsets::m_bHasHelmet = get_offset("DT_CSPlayer", "m_bHasHelmet");
	offsets::m_ArmorValue = get_offset("DT_CSPlayer", "m_ArmorValue");
	offsets::m_bSpotted = get_offset("DT_BaseEntity", "m_bSpotted");
	offsets::m_iClip1 = get_offset("DT_BaseCombatWeapon", "m_iClip1");
	offsets::m_nForceBone = get_offset("DT_BaseAnimating", "m_nForceBone");
	offsets::m_nHitboxSet = get_offset("DT_BaseAnimating", "m_nHitboxSet");
	offsets::m_flPostponeFireReadyTime = get_offset("DT_BaseCombatWeapon", "m_flPostponeFireReadyTime");
	offsets::deadflag = 0x31D4;
	offsets::m_iDefaultFOV = get_offset("DT_BasePlayer", "m_iDefaultFOV");
	offsets::m_iFOV = get_offset("DT_BasePlayer", "m_iFOV");
	offsets::m_iShotsFired = get_offset("DT_CSPlayer", "m_iShotsFired");
}
int c_netvar_manager::get_offset(const char* tableName, const char* propName)
{
	int offset = get_prop(tableName, propName);
	if (!offset)
	{
		return 0;
	}
	return offset;
}

bool c_netvar_manager::hook_prop(const char* tableName, const char* propName, recv_var_proxy_fn fun)
{
	recv_prop* recvProp = 0;
	get_prop(tableName, propName, &recvProp);
	if (!recvProp)
		return false;

	recvProp->proxy_fn = fun;

	return true;
}

int c_netvar_manager::get_prop(const char* tableName, const char* propName, recv_prop** prop)
{
	recv_table* recvTable = get_table(tableName);
	if (!recvTable)
		return 0;

	int offset = get_prop(recvTable, propName, prop);
	if (!offset)
		return 0;

	return offset;
}

int c_netvar_manager::get_prop(recv_table* recvTable, const char* propName, recv_prop** prop)
{
	int extraOffset = 0;
	for (int i = 0; i < recvTable->props_count; ++i)
	{
		recv_prop* recvProp = &recvTable->props[i];
		recv_table* child = recvProp->data_table;

		if (child && (child->props > 0))
		{
			int tmp = get_prop(child, propName, prop);
			if (tmp)
				extraOffset += (recvProp->offset + tmp);
		}

		if (_stricmp(recvProp->prop_name, propName))
			continue;

		if (prop)
			*prop = recvProp;

		return (recvProp->offset + extraOffset);
	}

	return extraOffset;
}

recv_table* c_netvar_manager::get_table(const char* tableName)
{
	if (m_tables.empty())
		return 0;

	for (auto table : m_tables)
	{
		if (!table)
			continue;

		if (_stricmp(table->table_name, tableName) == 0)
			return table;
	}

	return 0;
}