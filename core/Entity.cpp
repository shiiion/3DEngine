#include "IComponent.h"
#include "Entity.h"
#include "Core.h"
#include "IPhysicsComponent.h"
#include "IWorld.h"
#include "IRenderComponent.h"

//	Created by the master and nobody else + loser nerd

namespace ginkgo 
{
	Entity::Entity(const vec3& pos, const quat& rot, const vec3& vel, const vec3& accel)
	{
		position = pos;
		rotation = rot;
		velocity = vel;
		acceleration = accel;
		gravityEnabled = true;

		entityID = Core::generateID();
		renderComponent = nullptr;
	}

	const vec3& Entity::getAcceleration() const
	{
		return acceleration;
	}

	const vec3& Entity::getVelocity() const
	{
		return velocity;
	}

	const quat& Entity::getRotation() const
	{
		return rotation;
	}

	const vec3& Entity::getPosition() const
	{
		return position;
	}

	bool Entity::isGravityEnabled() const
	{
		return gravityEnabled;
	}

	void Entity::setAcceleration(const vec3& accel)
	{
		acceleration = accel;
	}

	void Entity::setVelocity(const vec3& vel)
	{
		velocity = vel;
	}

	void Entity::setRotation(const quat& rot)
	{
		if (physicsComponent != nullptr)
		{
			physicsComponent->setRotation(rot);
		}
		if (renderComponent != nullptr)
		{
			renderComponent->setRotation(rot);
		}
		rotation = rot;
	}

	void Entity::setPosition(const vec3& pos)
	{
		position = pos;
	}

	void Entity::setEntityID(long ID)
	{
		entityID = ID;
	}

	void Entity::beginTick(float elapsedTime)
	{
		for (IComponent* component : componentList)
		{
			component->onTick(elapsedTime);
		}
		if (physicsComponent != nullptr)
		{
			//THE COST OF PREMATURE OPTIMIZATION IS EXPENSIVE
			//if (physicsComponent->getCollisionType() == CTYPE_WORLDSTATIC)
			//{
			//	return;
			//}
			physicsComponent->onTick(elapsedTime);
		}
		position += velocity * elapsedTime;
		velocity += acceleration * elapsedTime + (gravityEnabled ? (getWorld()->getGravity() * elapsedTime) : vec3(0, 0, 0));
	}

	void Entity::endTick(float elapsedTime)
	{
		if (physicsComponent != nullptr)
		{
			position = physicsComponent->getMoveResult().finalPos;
			velocity = physicsComponent->getMoveResult().finalVel;
		}
		for (IComponent* component : componentList)
		{
			component->onTickEnd(elapsedTime);
		}
	}

	long Entity::getEntityID() const
	{
		return entityID;
	}

	EntityType Entity::getEntityType() const
	{
		if (physicsComponent == nullptr)
		{
			return entity;
		}
		return PhysicsComponent;
	}
	
	void Entity::setGravityEnabled(bool enabled)
	{
		gravityEnabled = enabled;
	}

	void Entity::setPhysics(IPhysicsComponent* component)
	{
		physicsComponent = component;
	}

	void Entity::setRenderable(IRenderComponent* component)
	{
		//remove existing render component from component list
		for (int a = 0; a < componentList.size(); a++)
		{
			if (componentList[a] == static_cast<IComponent*>(renderComponent))
			{
				componentList.erase(componentList.begin() + a);
			}
		}
		renderComponent = component;
		componentList.emplace_back(renderComponent);
	}

	void Entity::addComponent(IComponent* component)
	{
		componentList.emplace_back(component);
	}

	Entity::~Entity()
	{
		for (IComponent* c : componentList)
		{
			c->onDetach();
		}
	}

	IEntity* entityFactory(const vec3& pos, const quat& rot, const vec3& vel, const vec3& accel)
	{
		return new Entity(pos, rot, vel, accel);
	}




	//--

	IEntity::~IEntity() {}
}