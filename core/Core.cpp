#include "Core.h"
#include "IWorld.h"
#include "IEntity.h"
#include "World.h"
#include <Windows.h>
#include "IPhysicsObject.h"

namespace ginkgo
{
	volatile long Core::entityIDBase = 1;
	Core Core::core;
	volatile bool doWakePhysics = false;

	long Core::generateID()
	{
		return entityIDBase++;
	}

	void Core::startCore()
	{
		core.running = true;
		core.coreThread = new thread(&Core::coreFunc, &core);
		core.physicsThread = new thread(&Core::physicsFunc, &core);
		//TODO: add event thread
	}

	void Core::stopCore()
	{
		core.running = false;
		core.coreThread->join();
		core.physicsThread->join();
		core.eventThread->join();
	}

	Core::Core()
	{
		running = false;
		tickTime = (1.f / 61.f);
		startTick = GetTickCount64();
		world = new World(1.0f);
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
			{
				std::lock_guard<std::mutex> lck(physicsLock);
				doWakePhysics = true;
			}
			physicsConditionVar.notify_one();
			{
				std::lock_guard<std::mutex> lck(physicsLock);
			}
			std::this_thread::sleep_for(std::chrono::milliseconds((int)(tickTime * 1000.f)));
		}
	}

	void Core::physicsFunc()
	{
		float tickEnd = getEngineTime(), elapsedTime;
		std::unique_lock<std::mutex> lck(physicsLock);
		while (running)
		{
			physicsConditionVar.wait(lck, [] { return doWakePhysics; });
			doWakePhysics = false;

			const vector<IEntity*>& entityList = world->getEntityList();
			doWakePhysics = false;
			elapsedTime = getEngineTime() - tickEnd;

			for (IEntity* e : entityList)
			{
				e->tick(elapsedTime);
			}

			const vector<IPhysicsObject*>& physicsObjects = (const vector<IPhysicsObject*>&)world->getEntitiesByType(physicsObject);

			for (IPhysicsObject* p : physicsObjects)
			{
				p->checkCollisions(elapsedTime);
			}

			for (IPhysicsObject* p : physicsObjects)
			{
				p->resolveCollisions(elapsedTime);
			}

			elapsedTime = getEngineTime() - tickEnd;
			tickEnd = getEngineTime();
		}
	}

	IWorld* Core::getWorld() const
	{
		return world;
	}

	void Core::lockPhysics()
	{
		physicsLock.lock();
	}

	void Core::unlockPhysics()
	{
		physicsLock.unlock();
	}

//~~~~~~~~~~~~~~~~~~~~~~~

	float getEngineTime()
	{
		return Core::core.getEngineTime();
	}

	void setTickTime(float time)
	{
		Core::core.setTickTime(time);
	}

	float getTickTime()
	{
		return Core::core.getTickTime();
	}

	void startCore()
	{
		Core::core.startCore();
	}

	void stopCore()
	{
		Core::core.stopCore();
	}

	IWorld* getWorld()
	{
		return Core::core.getWorld();
	}

	void lockPhysics()
	{
		Core::core.lockPhysics();
	}

	void unlockPhysics()
	{
		Core::core.unlockPhysics();
	}
}