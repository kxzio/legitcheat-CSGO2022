#pragma once
#include <string>
#include <memory>
#include "utils.h"
class KeyValues;
#define VirtualFn( cast )typedef cast( __thiscall* OriginalFn )
class IGameEvent
{
public:
	std::string GetName(void)
	{
		VirtualFn(const char*)(PVOID);
		return std::string(getvfunc< OriginalFn >(this, 1)(this));
	}
	bool GetBool(std::string strKeyName, bool bDefaultValue = false)
	{
		VirtualFn(bool)(PVOID, const char*, bool);
		const char* c = strKeyName.c_str();
		return getvfunc< OriginalFn >(this, 5)(this, c, bDefaultValue);
	}
	int GetInt(std::string strKeyName, int iDefaultValue = 0)
	{
		VirtualFn(int)(PVOID, const char*, int);
		const char* c = strKeyName.c_str();
		return getvfunc< OriginalFn >(this, 6)(this, c, iDefaultValue);
	}
	float GetFloat(std::string strKeyName, float iDefaultValue = 0.f)
	{
		VirtualFn(float)(PVOID, const char*, float);
		const char* c = strKeyName.c_str();
		return getvfunc< OriginalFn >(this, 8)(this, c, iDefaultValue);
	}
	const char* GetString(const char* keyname = nullptr, const char* default_value = "")
	{
		VirtualFn(const char*)(PVOID, const char*, const char*);
		return getvfunc< OriginalFn >(this, 9)(this, keyname, default_value);
	}
};

class IGameEventListener2
{
public:
	virtual ~IGameEventListener2(void) {}
	virtual void FireGameEvent(IGameEvent* pGameEvent) = 0;
	virtual int GetEventDebugID(void)
	{
		return iDebug;
	}

public:
	int	iDebug;
};

class c_event_manager
{
public:
	virtual ~c_event_manager(void)
	{
	};
	virtual int LoadEventsFromFile(const char* filename) = 0;
	virtual void Reset() = 0;
	virtual bool AddListener(IGameEventListener2* listener, const char* name, bool bServerSide) = 0;
	virtual bool FindListener(IGameEventListener2* listener, const char* name) = 0;
	virtual void RemoveListener(IGameEventListener2* listener) = 0;
	virtual void AddListenerGlobal(IGameEventListener2* listener, bool bIsServerSide) = 0;
	virtual IGameEvent* zzz(const char* name, bool bForce = false, int* pCookie = nullptr) = 0;
	virtual bool FireEvent(IGameEvent* pEvent, bool bDontBroadcast = false) = 0;
	virtual bool FireEventClientSide(IGameEvent* pEvent) = 0;
	virtual IGameEvent* DuplicateEvent(IGameEvent* pEvent) = 0;
	virtual void FreeEvent(IGameEvent* pEvent) = 0;
	virtual KeyValues* GetEventDataTypes(IGameEvent* pEvent) = 0;
};