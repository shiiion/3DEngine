#pragma once

#ifdef _WIN64
#include "CoreReource.h"
#include "Core.h"

namespace ginkgo
{
	DECLSPEC_CORE IEntity* createAddEntity(float x, float y, float z, float xr, float yr, float zr, float xv, float yv, float zv, float xe, float ye, float ze, float rebound, int dynamic, float mass);
	
	//0 = x, 1 = y, 2 = z
	DECLSPEC_CORE float getEntityLoc(IEntity* entity, int ind);

	//0 = x, 1 = y, 2 = z
	DECLSPEC_CORE float getAxis(IEntity* entity, int ind, int ind2);
}
#endif