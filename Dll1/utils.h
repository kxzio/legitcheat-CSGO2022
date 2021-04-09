#pragma once
#include <memory>
#include <iostream>
#include <Windows.h>
#include <Psapi.h>
#include <iomanip>

#pragma comment( lib, "psapi.lib" )

template <typename T = void*>
constexpr T getvfunc(void* thisptr, std::size_t nIndex)
{
	return (*static_cast<T**>(thisptr))[nIndex];
}

struct datamap_t;

class c_utils
{
public:
	unsigned int find_in_data_map(datamap_t* map, const char* name);
	uintptr_t find_pattern(const char* szModule, const char* szSignature);
};

extern c_utils* utils;