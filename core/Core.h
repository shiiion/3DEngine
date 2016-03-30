#pragma once

#include "CoreReource.h"


#ifdef COMP_DLL
namespace ginkgo
{
	class IWorld;
	class Core
	{
	private:
		static volatile long entityIDBase;
		
		static Core core;

		long startTick;

		volatile bool running;
		volatile float tickTime;
		IWorld* world;
		
		condition_variable renderConditionVar;
		condition_variable eventConditionVar;

	public:
		Core();

		void coreThread();

		void setTickTime(float time);
		float getTickTime() const;

		float getEngineTime() const;

		static long generateID();
	};
}
#endif