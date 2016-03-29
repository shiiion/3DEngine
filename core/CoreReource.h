#pragma once

#ifdef COMP_DLL
#define DECLSPEC __declspec(dllexport)
#else
#define DECLSPEC __declspec(dllimport)
#endif
#include <vector>


namespace ginkgo
{
	using std::vector;
	struct Material
	{
		float collisionTime;
		float friction;
	};

	typedef unsigned __int8 UBYTE;
	typedef signed __int32 INT32;
	typedef unsigned __int32 UINT32;

};