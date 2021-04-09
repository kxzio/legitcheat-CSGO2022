#pragma once
#include <memory>

class c_entity;
class c_entity_list
{
public:
	virtual void Function0() = 0;
	virtual void Function1() = 0;
	virtual void Function2() = 0;

	virtual c_entity* get_client_entity(int iIndex) = 0;
	virtual c_entity* get_client_entity_handle(uint32_t hHandle) = 0;
	virtual int number_of_entities(bool bIncludeNonNetworkable) = 0;
	virtual int get_highest_ent_index() = 0;
};