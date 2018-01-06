#include "Core.h"
#include "IWorld.h"
#include "IEntity.h"
#include "World.h"
#include <Windows.h>
#include "IPhysicsComponent.h"
#include "IAbstractInputSystem.h"
#include "PhysicsComponent.h"
#include "CollisionMesh.h"
#include "InputWrapper.h"
#include <GLFW\glfw3.h>

#include <vector>
#include "MovementStateCallbackManager.h"
#include "Character.h"

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

	void Core::setupInput(GLFWwindow* window)
	{
		registerCallbacks(window);
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
		world = new World(-9.8f);
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

	void Core::coreTick(float timeScale)
	{
		if (running)
		{
			float elapsedTime = 0.016f * timeScale;//(getEngineTime() - lastTickTime) * timeScale; 
			lastTickTime = getEngineTime();

			processInput();
			physicsTick(elapsedTime);
			//update(elapsedTime); + render
		}
	}

	void Core::processInput()
	{
		glfwPollEvents();
		
		for (IAbstractInputSystem* input : inputSystemList)
		{
			input->runInput();
		}
	}

	void Core::physicsTick(float elapsedTime)
	{
		const vector<IEntity*>& entityList = world->getEntityList();

		vector<IPhysicsComponent*> PhysicsComponents;

 		for (IEntity* e : entityList)
		{
			//do world movement thing here
			//world->function(); -- callback(characterInstance, elapsedTime);
			
			e->beginTick(elapsedTime);
			if (e->getEntityType() >= PhysicsComponent)
			{
				PhysicsComponents.emplace_back(e->getPhysics());
			}
			world->updateOctreeIndex(e);
		}
		//world->recalculateTree();
		world->preCollisionTest();

		vector<IPhysicsComponent*> colliders;

		//COLLISION DETECTION
		for (IPhysicsComponent* p : PhysicsComponents)
		{
			if (p->getCollisionType() == CTYPE_WORLDSTATIC || !p->doesCollide())
			{
				continue;
			}

			world->getEntityTree().retrieveCollisions(colliders, p);

			for (IPhysicsComponent* collider : colliders)
			{
				if (!collider->doesCollide())
				{
					continue;
				}
				//OPTIMIZATION: check existing tests (even if there was no result)
				if (collider->getParent()->getEntityID() != p->getParent()->getEntityID())
				{
					if (!world->collisionExists(p, collider))
					{
						p->checkCollision(elapsedTime, collider);
					}
				}
			}
			colliders.clear();
		}

		//update all characters' movement states
		//world->otherfunction() -- for(all available movement states) if(callback(characterInstance)) change characterInstance's movementState
		//if all callbacks fail, default to 0 (freemove)

		world->resolveCollisions(16);


		for (IEntity* e : entityList)
		{
			e->endTick(elapsedTime);
		}
		world->checkMovementStates(elapsedTime);
		world->doMovementStates(elapsedTime);

		world->clearCollisionCache();
	}

	IWorld* Core::getWorld() const
	{
		return world;
	}

	void Core::sleep()
	{
		long long sleeptime = (long long)(tickTime * 1000.f);
		sleeptime = max(sleeptime - (long long)((getEngineTime() - lastTickTime) * 1000.f), 1);
		std::this_thread::sleep_for(std::chrono::milliseconds(sleeptime));
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

	void tickCore(float ts)
	{
		Core::core.coreTick(ts);
	}

	void sleepTickTime()
	{
		Core::core.sleep();
	}

	void registerInputSystem(IAbstractInputSystem* input, ICharacter* controller)
	{
		Core::registerInputSystem(input, controller);
	}

	vector<IAbstractInputSystem*> const& getAllInputSystems()
	{
		return Core::core.getInputSystemList();
	}

	int registerMovementState(const std::string& name, const CheckIfMovementState& CheckMovementState, const DoOnMovementState& OnMovementState, const OnMovementStateEnabled& OnStateEnabled, const OnMovementStateDisabled& OnStateDisabled)
	{
		IWorld* world = getWorld();
		return world->registerMovementState(name, CheckMovementState, OnMovementState, OnStateEnabled, OnStateDisabled);
	}

	int getMovementState(const std::string & name)
	{
		IWorld* world = getWorld();
		return world->getMovementState(name);
	}

	void setupInput(GLFWwindow* window)
	{
		Core::core.setupInput(window);
	}
}