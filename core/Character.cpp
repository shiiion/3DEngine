#include "Character.h"
#include "IPhysicsComponent.h"
#include "IComponent.h"
#include "UserInputSystem.h"
#include <GLFW\glfw3.h>

namespace ginkgo
{
	Character::Character(const vec3& pos, const vec3& rot, const vec3& vel, const vec3& accel)
	{
		position = pos;
		rotation = rot;
		velocity = vel;
		acceleration = accel;
		airSpeedFactor = 1;
		gravityEnabled = true;

		physics = nullptr;

		entityID = Core::generateID();

		//freemove
		movementStateList.emplace_back(0);
		//walking
		movementStateList.emplace_back(1);
		movementState = 0;
		
		inputSystem = createUserInputSystem();

		inputSystem->addCommand(CommandSetReset(1, [](IAbstractInputSystem* inputSystem, int outputCode, bool set)
		{
			if (set)
			{
				inputSystem->getOwner()->setMovementControlFlag(FWD_MOVE);
			}
			else
			{
				inputSystem->getOwner()->resetMovementControlFlag(FWD_MOVE);
			}
		}));
		inputSystem->addCommand(CommandSetReset(2, [](IAbstractInputSystem* inputSystem, int outputCode, bool set)
		{
			if (set)
			{
				inputSystem->getOwner()->setMovementControlFlag(BACK_MOVE);
			}
			else
			{
				inputSystem->getOwner()->resetMovementControlFlag(BACK_MOVE);
			}
		}));
		inputSystem->addCommand(CommandSetReset(3, [](IAbstractInputSystem* inputSystem, int outputCode, bool set)
		{
			if (set)
			{
				inputSystem->getOwner()->setMovementControlFlag(LEFT_MOVE);
			}
			else
			{
				inputSystem->getOwner()->resetMovementControlFlag(LEFT_MOVE);
			}
		}));
		inputSystem->addCommand(CommandSetReset(4, [](IAbstractInputSystem* inputSystem, int outputCode, bool set)
		{
			if (set)
			{
				inputSystem->getOwner()->setMovementControlFlag(RIGHT_MOVE);
			}
			else
			{
				inputSystem->getOwner()->resetMovementControlFlag(RIGHT_MOVE);
			}
		}));

		inputSystem->bindInputCode(GLFW_KEY_W, 1);
		inputSystem->bindInputCode(GLFW_KEY_S, 2);
		inputSystem->bindInputCode(GLFW_KEY_A, 3);
		inputSystem->bindInputCode(GLFW_KEY_D, 4);

		registerInputSystem(inputSystem, this);
	}

	void Character::beginTick(float elapsedTime)
	{
		for (IComponent* component : componentList)
		{
			component->onTick(elapsedTime);
		}
		finalMove.finalPos = position + (velocity * elapsedTime);
		finalMove.finalVel = velocity + ((acceleration + (gravityEnabled ? getWorld()->getGravity() : vec3())) * elapsedTime);
		finalMove.finalAng = rotation + (angularVelocity * elapsedTime);
		finalMove.finalAngVel = angularVelocity;
	}

	void Character::endTick(float elapsedTime)
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

	void Character::addComponent(IComponent* component)
	{
		componentList.emplace_back(component);
	}

	Character::~Character()
	{
		for (IComponent* c : componentList)
		{
			c->onDetach();
		}
	}

	ICharacter* characterFactory(const vec3& pos, const vec3& rot, const vec3& vel, const vec3& accel)
	{
		return new Character(pos, rot, vel, accel);
	}


	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// HELPER FUNCTIONS~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


	//void applyMovement(ICharacter& character, float elapsedTime)
	//{
	//	//TODO: CHANGE THIS!! MOVE IN ROTATION DIRECTION (project character direction vector onto XZ plane)
	//	//TODO: walking velocity
	//	//TODO: maket all of these functions private member functions
	//	if (character.getMovementControlFlags() & FWD_MOVE)
	//	{
	//		character.setVelocity(character.getVelocity() + vec3(0, 0, 1) * elapsedTime);
	//	}
	//	if (character.getMovementControlFlags() & BACK_MOVE)
	//	{
	//		character.setVelocity(character.getVelocity() + vec3(0, 0, -1) * elapsedTime);
	//	}
	//	if (character.getMovementControlFlags() & LEFT_MOVE)
	//	{
	//		character.setVelocity(character.getVelocity() + vec3(-1, 0, 0) * elapsedTime);
	//	}
	//	if (character.getMovementControlFlags() & RIGHT_MOVE)
	//	{
	//		character.setVelocity(character.getVelocity() + vec3(1, 0, 0) * elapsedTime);
	//	}
	//}

	/*void resolveFreemove(ICharacter& character, float elapsedTime)
	{
		applyMovement(character, elapsedTime);
	}*/

	/*bool isWalkableNormal(vec3 const& normal)
	{
		if (normal.y <= 0)
		{
			return false;
		}
		vec3 normalized = glm::normalize(normal);
		vec3 vertical(0, -1, 0);

		float angBetween = glm::acos(glm::dot(-normalized, vertical));
		return (angBetween <= 45);
	}*/

	/*vec3 const* findBestWalkableNormal(std::forward_list<SurfaceData> const& normalList, bool& found)
	{
		auto listIter = normalList.begin();
		while (listIter != normalList.end() && !(found = isWalkableNormal(listIter->surfaceNormal))) listIter++;

		if (found)
		{
			return &listIter->surfaceNormal;
		}
		return nullptr;
	}*/

	//void resolveWalking(ICharacter& character, float elapsedTime)
	//{
	//	applyMovement(character, elapsedTime);
	//	bool found;
	//	vec3 const* normalPtr = findBestWalkableNormal(character.getPhysics()->getCollisionNormalList(), found);//TODO: change this to most recent viable collision normal
	//	if (normalPtr == nullptr)
	//	{
	//		return;
	//	}
	//	vec3 const& collisionNormal = *normalPtr;
	//	vec3 velocityProjectionOnNormal = (collisionNormal * glm::dot(character.getVelocity(), collisionNormal) / pow(length(collisionNormal), 2));//should work
	//																																					//checks if slope is away or towards the player's movement, projects velocity on slope
	//	if (glm::length(character.getVelocity() + collisionNormal) > glm::length(character.getVelocity()))
	//	{
	//		character.setVelocity(character.getVelocity() - velocityProjectionOnNormal);//TODO: only project movement velocity
	//	}
	//	else
	//	{
	//		character.setVelocity(character.getVelocity() + velocityProjectionOnNormal);
	//	}
	//}

	//bool checkWalking(ICharacter const& character, float elapsedTime)
	//{
	//	if (character.getPhysics()->getNumCollisions())//some boolean for collision
	//	{
	//		//TODO: default to newest walkable normal!!
	//		bool found;
	//		findBestWalkableNormal(character.getPhysics()->getCollisionNormalList(), found);//TODO find collision normal
	//		return found;
	//	}
	//	return false;
	//} 


	//void walkingEnabled(ICharacter& character, float elapsedTime)
	//{
	//	character.setGravityEnabled(false);
	//}

	//void walkingDisabled(ICharacter& character, float elapsedTime)
	//{
	//	character.setGravityEnabled(true);
	//}

	ICharacter::~ICharacter() {}
}