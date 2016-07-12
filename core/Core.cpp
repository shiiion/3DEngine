#include "Core.h"
#include "IWorld.h"
#include "IEntity.h"
#include "World.h"
#include <Windows.h>
#include "IPhysicsObject.h"
#include "IAbstractInputSystem.h"

namespace ginkgo
{
	long Core::entityIDBase = 1;
	Core Core::core;

	long Core::generateID()
	{
		return entityIDBase++;
	}

	void Core::startCore()
	{
		core.running = true;
	}

	void Core::stopCore()
	{
		core.running = false;
	}

	Core::Core()
	{
		running = false;
		tickTime = (1.f / 60.f);
		startTick = GetTickCount64();
		world = new World(1.0f);
		lastTickTime = getEngineTime();
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

	void Core::coreTick()
	{
		if (running)
		{
			float elapsedTime = 0.016;//getEngineTime() - lastTickTime;
			lastTickTime = elapsedTime + lastTickTime;

			processInput();
			physicsTick(elapsedTime);
		}
	}

	void Core::processInput()
	{
		for (IAbstractInputSystem* input : inputSystemList)
		{
			input->checkInput();
		}

		for (IAbstractInputSystem* input : inputSystemList)
		{
			input->runInput();
		}
	}

	void Core::physicsTick(float elapsedTime)
	{

		const vector<IEntity*>& entityList = world->getEntityList();

		for (IEntity* e : entityList)
		{
			e->tick(elapsedTime);
		}

		const vector<IPhysicsObject*>& physicsObjects = (const vector<IPhysicsObject*>&)world->getEntitiesByType(physicsObject);
		vector<IPhysicsObject*> colliders;
		for (IPhysicsObject* p : physicsObjects)
		{
			if (p->getCollisionType() == CTYPE_WORLDSTATIC)
			{
				continue;
			}
			world->getEntityTree().retrieveCollisions(colliders, p);
			for (IPhysicsObject* collider : physicsObjects)
			{
				if (collider->getEntityID() != p->getEntityID() && !p->CollisionAlreadyExists(collider))
				{
					p->checkCollision(elapsedTime, collider);
				}
			}
		}

		for (IPhysicsObject* p : physicsObjects)
		{
			p->resolveCollisions(elapsedTime);
		}
	}

	IWorld* Core::getWorld() const
	{
		return world;
	}

	void Core::sleep()
	{
		std::this_thread::sleep_for(std::chrono::milliseconds((long long)(tickTime * 1000.f)));
	}

	void Core::registerInputSystem(IAbstractInputSystem* input, ICharacter* controller)
	{
		input->setOwner(controller);
		core.inputSystemList.emplace_back(input);
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

	void tickCore()
	{
		Core::core.coreTick();
	}

	void sleepTickTime()
	{
		Core::core.sleep();
	}

	void registerInputSystem(IAbstractInputSystem* input, ICharacter* controller)
	{
		Core::registerInputSystem(input, controller);
	}
}