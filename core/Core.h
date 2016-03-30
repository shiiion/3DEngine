#pragma once

#include "CoreReource.h"

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
