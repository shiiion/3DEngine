#include "Core.h"
#include "IWorld.h"
#include "IEntity.h"
#include <Windows.h>

namespace ginkgo
{
	volatile long Core::entityIDBase = 1;
	Core Core::core = Core();

	long Core::generateID()
	{
		return entityIDBase++;
	}

	Core::Core()
	{
		running = false;
		tickTime = (1.f / 60.f);
		startTick = GetTickCount64();
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

	void Core::coreThread()
	{
		running = true;
		float tickStart = getEngineTime(), tickEnd = getEngineTime(), elapsedTime;
		while (running)
		{
			const vector<IEntity*>& entityList = world->getEntityList();

			for (IEntity* e : entityList)
			{
				e->tick(elapsedTime);
			}



			std::this_thread::sleep_for(std::chrono::milliseconds((int)(tickTime * 1000.f)));
		}
	}
}