#pragma once
#include <iostream>
#include <memory>
#include "utils.h"
#include "surface.h"

class color;

inline int utl_memory_calc_new_alloc_count(int nAllocationCount, int nGrowSize, int nNewSize, int nBytesItem)
{
	if (nGrowSize)
		nAllocationCount = ((1 + ((nNewSize - 1) / nGrowSize)) * nGrowSize);
	else
	{
		if (!nAllocationCount)
			nAllocationCount = (31 + nBytesItem) / nBytesItem;

		while (nAllocationCount < nNewSize)
			nAllocationCount *= 2;
	}

	return nAllocationCount;
}

template< class T, class I = int >
class c_utl_memory
{
public:
	T& operator[](I i)
	{
		return m_pMemory[i];
	}

	T* base()
	{
		return m_pMemory;
	}

	int num_allocated() const
	{
		return m_nAllocationCount;
	}

	void grow(int num = 1)
	{
		if (is_allocated_external())
			return;

		int nAllocationRequested = m_nAllocationCount + num;
		int nNewAllocationCount = utl_memory_calc_new_alloc_count(m_nAllocationCount, m_nGrowSize, nAllocationRequested, sizeof(T));

		if ((int)(I)nNewAllocationCount < nAllocationRequested)
		{
			if ((int)(I)nNewAllocationCount == 0 && (int)(I)(nNewAllocationCount - 1) >= nAllocationRequested)
				--nNewAllocationCount;
			else
			{
				if ((int)(I)nAllocationRequested != nAllocationRequested)
					return;

				while ((int)(I)nNewAllocationCount < nAllocationRequested)
					nNewAllocationCount = (nNewAllocationCount + nAllocationRequested) / 2;
			}
		}

		m_nAllocationCount = nNewAllocationCount;

		if (m_pMemory)
			m_pMemory = (T*)realloc(m_pMemory, m_nAllocationCount * sizeof(T));
		else
			m_pMemory = (T*)malloc(m_nAllocationCount * sizeof(T));
	}

	bool is_allocated_external() const
	{
		return m_nGrowSize < 0;
	}

protected:
	T* m_pMemory;
	int m_nAllocationCount;
	int m_nGrowSize;
};

template <class T>
inline T* construct(T* pMemory)
{
	return ::new(pMemory) T;
}

template <class T>
inline void destruct(T* pMemory)
{
	pMemory->~T();
}

template< class T, class A = c_utl_memory<T> >
class c_utl_vector
{
	typedef A CAllocator;
public:
	CAllocator m_Memory;
	int m_Size;

	T& operator[](int i)
	{
		return m_Memory[i];
	}

	T& element(int i)
	{
		return m_Memory[i];
	}

	T* base()
	{
		return m_Memory.base();
	}

	int count() const
	{
		return m_Size;
	}

	void remove_all()
	{
		for (int i = m_Size; --i >= 0; )
			destruct(&element(i));

		m_Size = 0;
	}

	int add_to_tail()
	{
		return insert_before(m_Size);
	}

	int insert_before(int elem)
	{
		grow_vector();
		shift_elements_right(elem);
		construct(&element(elem));

		return elem;
	}

protected:
	void grow_vector(int num = 1)
	{
		if (m_Size + num > m_Memory.NumAllocated())
			m_Memory.Grow(m_Size + num - m_Memory.NumAllocated());

		m_Size += num;
		reset_dbg_info();
	}

	void shift_elements_right(int elem, int num = 1)
	{
		int numToMove = m_Size - elem - num;
		if ((numToMove > 0) && (num > 0))
			memmove(&element(elem + num), &element(elem), numToMove * sizeof(T));
	}

	T* m_pElements;

	inline void reset_dbg_info()
	{
		m_pElements = base();
	}
};

typedef void(*FnChangeCallbackV1_t)(void);
typedef void(*FnChangeCallback_t)(void* var, const char* pOldValue, float flOldValue);

struct CVValue_t
{
	char* m_pszString;
	int m_StringLength;

	// Values
	float m_fValue;
	int m_nValue;
};

class c_cvar
{
public:
	const char* get_string()
	{
		typedef const char* (__thiscall* GetString_t)(void*);
		return getvfunc<GetString_t>(this, 11)(this);
	}

	float get_float()
	{
		typedef float(__thiscall* GetFloat_t)(void*);
		return getvfunc <GetFloat_t>(this, 12)(this);
	}

	int get_int()
	{
		typedef int(__thiscall* GetInt_t)(void*);
		return getvfunc <GetInt_t>(this, 13)(this);
	}

	void set_value(const char* value)
	{
		typedef void(__thiscall* SetValue_t)(void*, const char*);
		return getvfunc<SetValue_t>(this, 14)(this, value);
	}

	void set_value(float value)
	{
		typedef void(__thiscall* SetValue_t)(void*, float);
		return getvfunc<SetValue_t>(this, 15)(this, value);
	}

	void set_value(int value)
	{
		typedef void(__thiscall* SetValue_t)(void*, int);
		return getvfunc<SetValue_t>(this, 16)(this, value);
	}

	char* get_name()
	{
		typedef char* (__thiscall* OriginalFn)(void*);
		return getvfunc<OriginalFn>(this, 5)(this);
	}

	void* virtualtable;
	c_cvar* m_pNext;
	int m_bRegistered;
	char* m_pszName;
	char* m_pszHelpString;
	int m_nFlags;
	FnChangeCallbackV1_t m_fnChangeCallbacksV1;
	c_cvar* m_pParent;
	char* m_pszDefaultValue;
	CVValue_t m_Value;
	int m_bHasMin;
	float m_fMinVal;
	int m_bHasMax;
	float m_fMaxVal;
	c_utl_vector<FnChangeCallback_t> m_fnChangeCallbacks;

	float get_value()
	{
		DWORD xored = *(DWORD*)&m_pParent->m_Value.m_fValue ^ (DWORD)this;
		return *(float*)&xored;
	}
	int get_value_new()
	{
		return (int)(m_pParent->m_Value.m_nValue ^ (DWORD)this);
	}
	bool get_bool() { return !!get_int(); }
};

struct CVarDLLIdentifier_t;

class ConCommandBase
{
public:
	PVOID pVMTBase; // 0x00

	ConCommandBase* m_pNext; // 0x04
	bool m_bRegistered; // 0x08
	const char* m_pszName; // 0x0C
	const char* m_pszHelpString; // 0x10
	int m_nFlags; // 0x14

	ConCommandBase* s_pConCommandBases; // 0x18
	void* s_pAccessor; // 0x1C
};

class i_cvar
{
public:
	template <typename... Values>
	void print_console_color(const color& Msgcolor_t, const char* szMsgFormat, Values... Parameters)
	{
		typedef void(*oConsolecolor_tPrintf)(void*, const color&, const char*, ...);
		return getvfunc<oConsolecolor_tPrintf>(this, 25)(this, Msgcolor_t, szMsgFormat, Parameters...);
	}

	void unregister_command(c_cvar* pCommandBase)
	{
		typedef c_cvar* (__thiscall* FindVarFn)(void*, c_cvar*);
		getvfunc< FindVarFn >(this, 13)(this, pCommandBase);
	}

	c_cvar* find_var(const char* var_name)
	{
		typedef c_cvar* (__thiscall* FindVarFn)(void*, const char*);
		return getvfunc< FindVarFn >(this, 16)(this, var_name);
	}

	void register_command(c_cvar* pCommandBase)
	{
		typedef c_cvar* (__thiscall* FindVarFn)(void*, c_cvar*);
		getvfunc< FindVarFn >(this, 12)(this, pCommandBase);
	}
};
