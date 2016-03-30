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

		thread* coreThread;
		thread* physicsThread;
		thread* eventThread;

		unsigned long long startTick;

		volatile bool running;
		volatile float tickTime;
		IWorld* world;
		
		condition_variable eventConditionVar;
		condition_variable physicsConditionVar;

		std::unique_lock<std::mutex> physicsLock;
		std::unique_lock<std::mutex> eventLock;

	public:
		Core();
		Core(const Core& copy);

		void coreFunc();
		void physicsFunc();

		void setTickTime(float time);
		float getTickTime() const;

		float getEngineTime() const;

		static long generateID();
		static void startCore();
		static void stopCore();
	};
}
#endif