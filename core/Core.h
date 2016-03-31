#pragma once

#include "CoreReource.h"


namespace ginkgo
{
	class IWorld;
#ifdef COMP_DLL_CORE
	class Core
	{
	private:
		static volatile long entityIDBase;
		

		thread* coreThread;
		thread* physicsThread;
		thread* eventThread;

		unsigned long long startTick;

		volatile bool running;
		volatile float tickTime;
		IWorld* world;
		
		condition_variable eventConditionVar;
		condition_variable physicsConditionVar;

		std::mutex physicsLock;
		std::mutex eventLock;

	public:
		static Core core;
		Core();
		Core(const Core& copy);

		void coreFunc();
		void physicsFunc();

		void lockPhysics();
		void unlockPhysics();

		void setTickTime(float time);
		float getTickTime() const;

		float getEngineTime() const;

		IWorld* getWorld() const;

		static long generateID();
		static void startCore();
		static void stopCore();
		
	};
#endif
	DECLSPEC_CORE float getEngineTime();
	DECLSPEC_CORE void setTickTime(float time);
	DECLSPEC_CORE float getTickTime();
	DECLSPEC_CORE void startCore();
	DECLSPEC_CORE void stopCore();
	DECLSPEC_CORE IWorld* getWorld();

	DECLSPEC_CORE void lockPhysics();
	DECLSPEC_CORE void unlockPhysics();
}