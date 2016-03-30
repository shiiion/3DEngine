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

		static thread* coreThread;
		static thread* physicsThread;
		static thread* eventThread;

		long startTick;

		volatile bool running;
		volatile float tickTime;
		IWorld* world;
		
		condition_variable eventConditionVar;
		condition_variable physicsConditionVar;

		std::unique_lock<std::mutex> physicsLock;
		std::unique_lock<std::mutex> eventLock;

	public:
		Core();

		void coreThread();
		void physicsThread();

		void setTickTime(float time);
		float getTickTime() const;

		float getEngineTime() const;

		static long generateID();
		static void startCore();
		static void stopCore();
	};
}
#endif