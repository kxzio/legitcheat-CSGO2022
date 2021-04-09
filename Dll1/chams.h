#pragma once
#include "interfaces.h"

class IMaterial;
class c_chams
{
public:
	IMaterial* material[3];
	void draw(void*, IMatRenderContext*, const DrawModelState_t&, const ModelRenderInfo_t&, matrix*);
};

extern c_chams* chams;