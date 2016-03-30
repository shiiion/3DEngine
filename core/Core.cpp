#include "Core.h"
#include "IWorld.h"
#include "IEntity.h"
#include <Windows.h>

namespace ginkgo
{
	static bool doWakePhysics = false;
	volatile long Core::entityIDBase = 1;
	Core Core::core;

	long Core::generateID()
	{
		return entityIDBase++;
	}

	void Core::startCore()
	{
		core.running = true;
		core.coreThread = new thread(&Core::coreFunc, core);
		core.physicsThread = new thread(&Core::physicsFunc, core);
	}

	Core::Core()
	{
		running = false;
		tickTime = (1.f / 60.f);
		startTick = GetTickCount64();
	}

	Core::Core(const Core& copy)
	{
		startTick = copy.startTick;
		running = copy.running;
		tickTime = copy.tickTime;
		world = copy.world;
	}

	float Core::getTickTime() const
	{
		return tickTime;
	}

	void Core::setTickTime(float time)
	{
		tickTime = time;
	}

	float Core::getEngineTime() const
	{
		return (float)(GetTickCount64() - startTick) / 1000.f;
	}

	void Core::coreFunc()
	{
		running = true;
		while (running)
		{
			doWakePhysics = true;
			physicsConditionVar.notify_one();
			physicsLock.lock();

			std::this_thread::sleep_for(std::chrono::milliseconds((int)(tickTime * 1000.f)));
		}
	}

	void Core::physicsFunc()
	{
		float tickEnd = getEngineTime(), elapsedTime;

		while (running)
		{
			const vector<IEntity*>& entityList = world->getEntityList();
			physicsConditionVar.wait(physicsLock, [] { return doWakePhysics; });
			doWakePhysics = false;
			elapsedTime = getEngineTime() - tickEnd;

			for (IEntity* e : entityList)
			{
				e->tick(elapsedTime);
			}

			//!!COLLISION CODE GOES HERE!!

			elapsedTime = getEngineTime() - tickEnd;
			tickEnd = getEngineTime();
		}
	}
}