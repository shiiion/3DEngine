#pragma once

#include "ICharacter.h"
#include "Core.h"
#include <xtgmath.h>

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
		int airSpeedFactor;

		IRenderable* renderableComponent;
		IPhysicsObject* physicsComponent;

		vector<int> movementStateList;
		IAbstractInputSystem* inputSystem;

		vector<IComponent*> componentList;

		int movementState;

	public:
		Character(const glm::vec3& pos, const glm::vec3& rot = glm::vec3(), const glm::vec3& vel = glm::vec3(), const glm::vec3& accel = glm::vec3());

		void beginTick(float elapsedTime) override;
		void endTick(float elapsedTime) override;

		const glm::vec3& getPosition() const override { return position; }
		const glm::vec3& getVelocity() const override { return velocity; }
		const glm::vec3& getAcceleration() const override { return acceleration; }
		const glm::vec3& getRotation() const override { return rotation; }

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
		void setMovementState(int newState) override { this->movementState = newState;  }
		void setRenderable(IRenderable* component) override { renderableComponent = component; }
		void setPhysics(IPhysicsObject* component) override { physicsComponent = component; }

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
		
		void checkMovementState() {
			if (isColliding)//some boolean for collision
			{
				glm::vec3 collisionNormal = ;//TODO find collision normal
				glm::vec3 xyUnitVec = glm::vec3(collisionNormal[0], collisionNormal[1], 0);
				double angle = acos(dot(collisionNormal, xyUnitVec) / (length(collisionNormal)*length(xyUnitVec)));
				//basically if the angle is negative or greater than 45, the player will slide on the slope
				if (angle <= 45 && angle >= 0)//pretty sure this is okay, you can change the angle if you want
				{
					setMovementState(1);
				}
				else
				{
					setMovementState(0);
				}
			}
			else
			{
				setMovementState(0);
			}
		}
		void setAirSpeedFactor(int factor) { airSpeedFactor = factor; }
		const void resloveMovementState() {
			if (this->movementState == 0) {
				if (isColliding)//some boolean for collision
				{
					glm::vec3 collisionNormal = ;//TODO find collision normal
					glm::vec3 xyUnitVec = glm::vec3(collisionNormal[0], collisionNormal[1], 0);
					glm::vec3 zUnitVec = glm::vec3(0, 0, -1);
					glm::vec3 slopeVec = (collisionNormal + zUnitVec);//always down the slope
					glm::vec3 velocityProjectionOnNormal = (collisionNormal * dot(velocity, collisionNormal) / pow(length(collisionNormal), 2));
					double angle = acos(dot(collisionNormal, xyUnitVec) / (length(collisionNormal)*length(xyUnitVec)));
					//TODO: add friction coefficient for sliding on slope
					if (angle < 0)
					{
						setAcceleration(glm::vec3(0, 0, -9.8));
						setVelocity(velocity *= airSpeedFactor);
					}
					else
					{
						//projects accel down slope, projects player's velocity on slope, allows for regular movement
						if (length(velocity + collisionNormal) > length(velocity)) {
							setAcceleration(slopeVec * dot(glm::vec3(0, 0, -9.8), slopeVec) / (pow(length(slopeVec), 2)));
							setVelocity(velocity - velocityProjectionOnNormal);
						}
						else {
							setAcceleration(slopeVec * dot(glm::vec3(0, 0, -9.8), slopeVec) / (pow(length(slopeVec), 2)));
							setVelocity(velocity + velocityProjectionOnNormal);
						}
					}

				}
				else
				{
					setAcceleration(glm::vec3(0, 0, -9.8));
					setVelocity(velocity *= airSpeedFactor);
				}
			} else if (this->movementState == 1) {
				glm::vec3 collisionNormal = ;//TODO find collision normal
				glm::vec3 velocityProjectionOnNormal = (collisionNormal * dot(velocity, collisionNormal) / pow(length(collisionNormal), 2));//should work
					//checks if slope is away or towards the player's movement, projects velocity on slope
					if (length(velocity + collisionNormal) > length(velocity)){
						setVelocity(velocity - velocityProjectionOnNormal);
					} else {
						setVelocity(velocity + velocityProjectionOnNormal);
					}
				
			}

		}
	};
}