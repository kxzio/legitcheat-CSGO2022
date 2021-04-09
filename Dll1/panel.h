#pragma once
#include "utils.h"

class c_panel
{
public:
	const char* get_name(int iIndex)
	{
		typedef const char* (__thiscall* OriginalFn)(void*, int);
		return getvfunc< OriginalFn >(this, 36)(this, iIndex);
	}
	void set_mouse_input_enabled(unsigned int panel, bool state)
	{
		return getvfunc<void(__thiscall*)(PVOID, int, bool)>(this, 32)(this, panel, state);
	}
};
