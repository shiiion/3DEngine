#pragma once

#include "ICharacter.h"
#include "IPhysicsObject.h"
#include "Core.h"

#define FWD_MOVE 0x01
#define LEFT_MOVE 0x02
#define RIGHT_MOVE 0x04
#define BACK_MOVE 0x08

namespace ginkgo
{
	class Character : public ICharacter
	{
	private:
		long entityID;

	protected:
		glm::vec3 position;
		glm::vec3 rotation;
		glm::vec3 velocity;
		glm::vec3 acceleration;
		bool gravityEnabled;
		float airSpeedFactor;

		IRenderable* renderableComponent;
		IPhysicsObject* physicsComponent;

		vector<int> movementStateList;
		IAbstractInputSystem* inputSystem;

		vector<IComponent*> componentList;

		int movementState;

		int movementCtlFlags;

	public:
		Character(const glm::vec3& pos, const glm::vec3& rot = glm::vec3(), const glm::vec3& vel = glm::vec3(), const glm::vec3& accel = glm::vec3());

		void beginTick(float elapsedTime) override;
		void endTick(float elapsedTime) override;

		const glm::vec3& getPosition() const override { return position; }
		const glm::vec3& getVelocity() const override { return velocity; }
		const glm::vec3& getAcceleration() const override { return acceleration; }
		const glm::vec3& getRotation() const override { return rotation; }
		bool isGravityEnabled() const override { return gravityEnabled; }

		long getEntityID() const override { return entityID; }
		int getMovementState() const override { return this->movementState; }

		IRenderable* getRenderable() const override { return renderableComponent; }
		IPhysicsObject* getPhysics() const override { return physicsComponent; }

		void setPosition(const glm::vec3& pos) override { position = pos; }
		void setVelocity(const glm::vec3& vel) override { velocity = vel; }
		void setAcceleration(const glm::vec3& acc) override { acceleration = acc; }
		void addAcceleration(const glm::vec3& acc) override { acceleration += acc; }
		void setRotation(const glm::vec3& ang) override { rotation = ang; }
		void setEntityID(long ID) override { entityID = ID; }
		void setGravityEnabled(bool enabled) override { gravityEnabled = enabled; }
		void setMovementState(int newState) override { this->movementState = newState; }
		void setRenderable(IRenderable* component) override { renderableComponent = component; }
		void setPhysics(IPhysicsObject* component) override 
		{ 
			physicsComponent = component;
		}

		void addComponent(IComponent* component) override;

		EntityType getEntityType() const override
		{
			if (physicsComponent == nullptr)
			{
				return entity;
			}
			return physicsObject;
		}


		void setInputSystem(IAbstractInputSystem* is) override { inputSystem = is; }
		IAbstractInputSystem* getInputSystem() const override { return inputSystem; }

		vector<int> const& getMovementStates() const override { return movementStateList; }
		void addMovementState(int movementStateID)
		{
			//if (getWorld()->movementStateExists(movementStateID))
			//{
			movementStateList.emplace_back(movementStateID);
			//}
		}



		float getAirSpeedFactor() const override { return airSpeedFactor; }
		void setAirSpeedFactor(float factor) override { airSpeedFactor = factor; }

		void setMovementControlFlag(int flag) override { movementCtlFlags |= flag; }
		void resetMovementControlFlag(int flag) override { movementCtlFlags &= (~flag); }


		int getMovementControlFlags() const override { return movementCtlFlags; }
	};


	void resolveFreemove(ICharacter& character, float elapsedTime);
	void resolveWalking(ICharacter& character, float elapsedTime);
	bool checkWalking(ICharacter const& character, float elapsedTime);
}