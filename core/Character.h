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
		vec3 position;
		vec3 rotation;
		vec3 velocity;
		vec3 acceleration;
		bool gravityEnabled;
		float airSpeedFactor;

		IRenderComponent* renderComponent;
		IPhysicsObject* physicsComponent;

		vector<int> movementStateList;
		IAbstractInputSystem* inputSystem;

		vector<IComponent*> componentList;

		int movementState;

		int movementCtlFlags;

	public:
		Character(const vec3& pos, const vec3& rot = vec3(), const vec3& vel = vec3(), const vec3& accel = vec3());

		void beginTick(float elapsedTime) override;
		void endTick(float elapsedTime) override;

		const vec3& getPosition() const override { return position; }
		const vec3& getVelocity() const override { return velocity; }
		const vec3& getAcceleration() const override { return acceleration; }
		const vec3& getRotation() const override { return rotation; }
		bool isGravityEnabled() const override { return gravityEnabled; }

		long getEntityID() const override { return entityID; }
		int getMovementState() const override { return this->movementState; }

		IRenderComponent* getRenderable() override
		{
			return const_cast<IRenderComponent*>(static_cast<Character const*>(this)->getRenderable());
		}
		IRenderComponent const* getRenderable() const override
		{
			return renderComponent;
		}
		IPhysicsObject* getPhysics() override
		{
			return const_cast<IPhysicsObject*>(static_cast<Character const*>(this)->getPhysics());
		}
		IPhysicsObject const* getPhysics() const override
		{
			return physicsComponent;
		}

		void setPosition(const vec3& pos) override { position = pos; }
		void setVelocity(const vec3& vel) override { velocity = vel; }
		void setAcceleration(const vec3& acc) override { acceleration = acc; }
		void addAcceleration(const vec3& acc) override { acceleration += acc; }
		void setRotation(const vec3& ang) override { rotation = ang; }
		void setEntityID(long ID) override { entityID = ID; }
		void setGravityEnabled(bool enabled) override { gravityEnabled = enabled; }
		void setMovementState(int newState) override { this->movementState = newState; }
		void setRenderable(IRenderComponent* component) override { renderComponent = component; }
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
		IAbstractInputSystem* getInputSystem() override { return const_cast<IAbstractInputSystem*>(static_cast<Character const*>(this)->getInputSystem()); }
		IAbstractInputSystem const* getInputSystem() const override { return inputSystem; }

		vector<int> const& getMovementStates() const override { return movementStateList; }
		void addMovementState(int movementStateID)
		{
			//TODO: see this
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
	void walkingEnabled(ICharacter& character, float elapsedTime);
	void walkingDisabled(ICharacter& character, float elapsedTime);
}