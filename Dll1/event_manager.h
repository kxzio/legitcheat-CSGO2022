#pragma once
#include "events.h"
#include "interfaces.h"
#include <vector>

typedef void(*Fucntion)(IGameEvent* event);
extern std::vector<IGameEventListener2*> event_manager_list;
class CGameEventListener : public IGameEventListener2
{
private:
	std::string eventName;
	Fucntion Call;
	bool server;
public:
	CGameEventListener() : eventName(""), Call(nullptr), server(false)
	{
	}

	CGameEventListener(const std::string& gameEventName, const Fucntion Func, bool Server)
		: eventName(gameEventName), Call(Func), server(Server)
	{
		this->RegListener();
	}

	virtual void FireGameEvent(IGameEvent* event)
	{
		this->Call(event);
	}

	virtual int IndicateEventHandling(void)
	{
		return 0x2A;
	}

	int GetEventDebugID() override
	{
		return 0x2A;
	}

	void RegListener()
	{
		static bool first = true;
		if (!first)
			return;
		if (interfaces->event_manager->AddListener(this, this->eventName.c_str(), server))
			event_manager_list.push_back(this);
	}
};

class c_event_override
{
public:
	c_event_override();
	void register_self();
};

extern c_event_override* event_override;