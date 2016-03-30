#pragma once

#include "CoreReource.h"


#ifdef COMP_DLL
namespace ginkgo
{
	class IWorld;
	class Core
	{
	private:
		static long entityIDBase;
		
		static Core core;



		float tickTime;
		IWorld* world;
		
	public:
		void coreThread();



		static long generateID();
	};
}
#endif