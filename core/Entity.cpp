#include "IComponent.h"
#include "Entity.h"
#include "Core.h"
#include "IPhysicsComponent.h"
#include "IWorld.h"
#include "IRenderComponent.h"

//	Created by the master and nobody else + loser nerd

namespace ginkgo 
{
	Entity::Entity(const vec3& pos, const vec3& rot, const vec3& vel, const vec3& accel)
	{
		position = pos;
		rotation = rot;
		velocity = vel;
		acceleration = accel;
		gravityEnabled = true;

		physics = nullptr;

		entityID = Core::generateID();
	}

	const vec3& Entity::getAcceleration() const
	{
		return acceleration;
	}

	const vec3& Entity::getVelocity() const
	{
		return velocity;
	}

	quat Entity::getRotationQ() const
	{
		return quat(rotation);
	}

	vec3 const& Entity::getRotation() const
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

	void Entity::setRotationQ(const quat& rot)
	{
		rotation = glm::eulerAngles(rot);
	}

	void Entity::setRotation(const vec3& rot)
	{
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
		// Notify components of tick
		for (IComponent* component : componentList)
		{
			component->onTick(elapsedTime);
		}
		finalMove.finalPos = position + (velocity * elapsedTime);
		finalMove.finalVel = velocity + ((acceleration + (gravityEnabled ? getWorld()->getGravity() : vec3())) * elapsedTime);
		finalMove.finalAng = rotation + (angularVelocity * elapsedTime);
		finalMove.finalAngVel = angularVelocity;
	}

	void Entity::endTick(float elapsedTime)
	{
		for (IComponent* component : componentList)
		{
			component->onTickEnd(elapsedTime);
		}

		position = finalMove.finalPos;
		velocity = finalMove.finalVel;
		rotation = finalMove.finalAng;
		angularVelocity = finalMove.finalAngVel;
	}

	long Entity::getEntityID() const
	{
		return entityID;
	}

	EntityType Entity::getEntityType() const
	{
		if (physics != nullptr)
		{
			return physicsComponent;
		}
		return entity;
	}
	
	void Entity::setGravityEnabled(bool enabled)
	{
		gravityEnabled = enabled;
	}

	void Entity::addComponent(IComponent* component)
	{
		if (component->type() == PHYSICS_COMPONENT)
		{

		}
		componentList.emplace_back(component);
	}

	IPhysicsComponent* Entity::getPhysics()
	{
		return physics;
	}

	Entity::~Entity()
	{
		for (IComponent* c : componentList)
		{
			c->onDetach();
		}
	}

	IEntity* entityFactory(const vec3& pos, const vec3& rot, const vec3& vel, const vec3& accel)
	{
		return new Entity(pos, rot, vel, accel);
	}




	//--

	IEntity::~IEntity() {}
}