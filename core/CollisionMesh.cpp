#include "CollisionMesh.h"
#include "IPhysicsObject.h"
#include "ISurface.h"
#include <glm/gtx/rotate_vector.hpp>
#include "Core.h"
#include "IWorld.h"

namespace ginkgo
{
	CollisionMesh::CollisionMesh(float l, float w, float h)
	{
		owner = nullptr;
		halfLength = l;
		halfWidth = w;
		halfHeight = h;

		widthAxis = glm::vec3(1, 0, 0);
		heightAxis = glm::vec3(0, 1, 0);
		lengthAxis = glm::vec3(0, 0, 1);
	}

	glm::vec3 const* CollisionMesh::getBoundingVertices() const
	{
		return nullptr;
	}

	MoveInfo const& CollisionMesh::getLastMove() const
	{
		return lastMove;
	}

	void CollisionMesh::finalizeMove()
	{
		center = owner->getPosition();

	}

	void CollisionMesh::generateVertexPath(float deltaTime)
	{
		//TODO: rotations
		if (owner == nullptr)
			return;
		lastMove.centerStart = owner->getPosition();
		lastMove.centerEnd = owner->getPosition() + owner->getVelocity() * deltaTime;
		lastMove.velStart = owner->getVelocity();
		lastMove.velEnd = lastMove.velStart + (lastMove.accel * deltaTime) - (getWorld()->getGravity() * deltaTime);
		lastMove.accel = owner->getAcceleration();
	}

	void CollisionMesh::setOwner(IPhysicsObject const* owner)
	{
		this->owner = owner;
		center = owner->getPosition();
	}

	bool CollisionMesh::testCollision(ICollisionMesh const& other, float deltaTime)
	{
		//15 tests

		if (testAxis(widthAxis, other, deltaTime))
			return true;
		if (testAxis(heightAxis, other, deltaTime))
			return true;
		if (testAxis(lengthAxis, other, deltaTime))
			return true;
		if (testAxis(other.getWidthAxis(), other, deltaTime))
			return true;
		if (testAxis(other.getHeightAxis(), other, deltaTime))
			return true;
		if (testAxis(other.getLengthAxis(), other, deltaTime))
			return true;
		if (testAxis(glm::cross(widthAxis, other.getWidthAxis()), other, deltaTime))
			return true;
		if (testAxis(glm::cross(widthAxis, other.getLengthAxis()), other, deltaTime))
			return true;
		if (testAxis(glm::cross(widthAxis, other.getHeightAxis()), other, deltaTime))
			return true;
		if (testAxis(glm::cross(lengthAxis, other.getWidthAxis()), other, deltaTime))
			return true;
		if (testAxis(glm::cross(lengthAxis, other.getLengthAxis()), other, deltaTime))
			return true;
		if (testAxis(glm::cross(lengthAxis, other.getHeightAxis()), other, deltaTime))
			return true;
		if (testAxis(glm::cross(heightAxis, other.getWidthAxis()), other, deltaTime))
			return true;
		if (testAxis(glm::cross(heightAxis, other.getLengthAxis()), other, deltaTime))
			return true;
		if (testAxis(glm::cross(heightAxis, other.getHeightAxis()), other, deltaTime))
			return true;
		return false;
	}

//	implementation based off of http://www.geometrictools.com/Documentation/DynamicCollisionDetection.pdf
//	axisNorm = L
//	centerDiff = D
//	Ai = axes of this
//	Bi = axes of other

	bool CollisionMesh::testAxis(glm::vec3 const& axisNorm, ICollisionMesh const& other, float deltaTime)
	{
		glm::vec3 centerDiff = (other.getLastMove().centerStart - lastMove.centerStart);
		glm::vec3 velDiff = (other.getLastMove().centerStart - lastMove.velStart);
		float proj = glm::dot(axisNorm, centerDiff);
		float projTime = glm::dot(axisNorm, centerDiff + (velDiff * deltaTime));

		float projThisBox;
		float projOtherBox;
		float otherW = other.getWidthExtent(), otherL = other.getLengthExtent(), otherH = other.getHeightExtent();

		projThisBox = (halfWidth * glm::sign(glm::dot(axisNorm, widthAxis)) * glm::dot(axisNorm, widthAxis)) +
			(halfHeight * glm::sign(glm::dot(axisNorm, heightAxis)) * glm::dot(axisNorm, heightAxis)) +
			(halfLength * glm::sign(glm::dot(axisNorm, lengthAxis)) * glm::dot(axisNorm, lengthAxis));
		projOtherBox = (otherW * glm::sign(glm::dot(axisNorm, other.getWidthAxis())) * glm::dot(axisNorm, other.getWidthAxis())) +
			(otherH * glm::sign(glm::dot(axisNorm, other.getHeightAxis())) * glm::dot(axisNorm, other.getHeightAxis())) +
			(otherL * glm::sign(glm::dot(axisNorm, other.getLengthAxis())) * glm::dot(axisNorm, other.getLengthAxis()));
		float r = projThisBox + projOtherBox;

		if (proj > r)
		{
			if (projTime > r)
			{
				return true;
			}
		}
		if (proj < -r)
		{
			if (projTime < -r)
			{
				return true;
			}
		}
		return false;
	}

	float CollisionMesh::getCollisionTime(ICollisionMesh const& other, float deltaTime) const
	{

		glm::vec3 centerDiff = (other.getLastMove().centerStart - lastMove.centerStart);
		glm::vec3 velDiff = (other.getLastMove().centerStart - lastMove.velStart);
		glm::vec3 const& axisNorm = widthAxis;
		float projThisBox, projOtherBox;
		float otherW = other.getWidthExtent(), otherL = other.getLengthExtent(), otherH = other.getHeightExtent();

		projThisBox = (halfWidth * glm::sign(glm::dot(axisNorm, widthAxis)) * glm::dot(axisNorm, widthAxis)) +
			(halfHeight * glm::sign(glm::dot(axisNorm, heightAxis)) * glm::dot(axisNorm, heightAxis)) +
			(halfLength * glm::sign(glm::dot(axisNorm, lengthAxis)) * glm::dot(axisNorm, lengthAxis));
		projOtherBox = (otherW * glm::sign(glm::dot(axisNorm, other.getWidthAxis())) * glm::dot(axisNorm, other.getWidthAxis())) +
			(otherH * glm::sign(glm::dot(axisNorm, other.getHeightAxis())) * glm::dot(axisNorm, other.getHeightAxis())) +
			(otherL * glm::sign(glm::dot(axisNorm, other.getLengthAxis())) * glm::dot(axisNorm, other.getLengthAxis()));
		float r = projThisBox + projOtherBox;

		float negVal = (r + glm::dot(axisNorm, centerDiff)) / (-glm::dot(axisNorm, velDiff));
		float posVal = (r - glm::dot(axisNorm, centerDiff)) / (glm::dot(axisNorm, velDiff));

		if (negVal == negVal && negVal != -INFINITY && negVal != INFINITY && negVal > 0 && negVal < deltaTime)
		{
			return negVal;
		}
		if (posVal == posVal && posVal != -INFINITY && posVal != INFINITY && posVal > 0 && posVal < deltaTime)
		{
			return posVal;
		}
		//uh oh
		return 0;
	}

	glm::vec3 const& CollisionMesh::getWidthAxis() const
	{
		return widthAxis;
	}

	glm::vec3 const& CollisionMesh::getHeightAxis() const
	{
		return heightAxis;
	}

	glm::vec3 const& CollisionMesh::getLengthAxis() const
	{
		return lengthAxis;
	}

	float CollisionMesh::getLengthExtent() const
	{
		return halfLength;
	}

	float CollisionMesh::getWidthExtent() const
	{
		return halfWidth;
	}

	float CollisionMesh::getHeightExtent() const
	{
		return halfHeight;
	}

	glm::vec3 const& CollisionMesh::getCenter() const
	{
		return center;
	}

	void CollisionMesh::generateCollisionInfo(ICollisionMesh const& other, float intersectTime)
	{
		//TODO: generate surface of other which this collided with
		//possible implementation: project 8 points of other and this onto velocity vector, 4 nearest projected points
		//of other on the velocity vector are the 4 points which make up the collision face
		//generate face out of 4 points?
	//	ISurface* surface = createSurface()
	}

	ICollisionMesh* createCollisionMesh(float l, float w, float h)
	{
		return new CollisionMesh(l, w, h);
	}
}