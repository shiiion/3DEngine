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
		extents[0] = w;
		extents[1] = h;
		extents[2] = l;

		axes[0] = glm::vec3(1, 0, 0);
		axes[1] = glm::vec3(0, 1, 0);
		axes[2] = glm::vec3(0, 0, 1);
		newSeparatingAxis = true;
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

		for (int a = 0; a < 3; a++)
		{
			if (testAxis(axes[a], other, deltaTime) || testAxis(other.getAxis(a), other, deltaTime))
			{
				return newSeparatingAxis = true;
			}
			for (int b = 0; b < 3; b++)
			{
				if(testAxis(glm::cross(axes[a], other.getAxis(b)), other, deltaTime))
				{
					return newSeparatingAxis = true;
				}
			}
		}
		if (newSeparatingAxis)
		{
			newSeparatingAxis = false;
			getLastSeparatingAxis(other, deltaTime);
		}
		return false;
	}

	void CollisionMesh::getLastSeparatingAxis(ICollisionMesh const& other, float deltaTime)
	{
		float longestTime = getCollisionTime(axes[0], other, deltaTime);
		float ct;
		lastSeparatingAxis = axes[0];
		for (int a = 0; a < 3; a++)
		{
			if (longestTime < (ct = getCollisionTime(axes[a], other, deltaTime)))
			{
				longestTime = ct;
				lastSeparatingAxis = axes[a];
			}
			if (longestTime < (ct = getCollisionTime(other.getAxis(a), other, deltaTime)))
			{
				longestTime = ct;
				lastSeparatingAxis = axes[a];
			}
			for (int b = 0; b < 3; b++)
			{
				if (longestTime < (ct = testAxis(glm::cross(axes[a], other.getAxis(b)), other, deltaTime)))
				{
					longestTime = ct;
					lastSeparatingAxis = glm::cross(axes[a], other.getAxis(b));
				}
			}
		}
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
		float otherW = other.getExtent(0), otherL = other.getExtent(2), otherH = other.getExtent(1);

		projThisBox = (extents[0] * glm::sign(glm::dot(axisNorm, axes[0])) * glm::dot(axisNorm, axes[0])) +
			(extents[1] * glm::sign(glm::dot(axisNorm, axes[1])) * glm::dot(axisNorm, axes[1])) +
			(extents[2] * glm::sign(glm::dot(axisNorm, axes[2])) * glm::dot(axisNorm, axes[2]));
		projOtherBox = (otherW * glm::sign(glm::dot(axisNorm, other.getAxis(0))) * glm::dot(axisNorm, other.getAxis(0))) +
			(otherH * glm::sign(glm::dot(axisNorm, other.getAxis(1))) * glm::dot(axisNorm, other.getAxis(1))) +
			(otherL * glm::sign(glm::dot(axisNorm, other.getAxis(2))) * glm::dot(axisNorm, other.getAxis(2)));
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

	float CollisionMesh::getCollisionTime(const glm::vec3& axisNorm, const ICollisionMesh& other, float deltaTime) const
	{

		glm::vec3 centerDiff = (other.getLastMove().centerStart - lastMove.centerStart);
		glm::vec3 velDiff = (other.getLastMove().centerStart - lastMove.velStart);
		float proj = glm::dot(axisNorm, centerDiff);
		float projTime = glm::dot(axisNorm, centerDiff + (velDiff * deltaTime));

		float projThisBox;
		float projOtherBox;
		float otherW = other.getExtent(0), otherL = other.getExtent(2), otherH = other.getExtent(1);

		projThisBox = (extents[0] * glm::sign(glm::dot(axisNorm, axes[0])) * glm::dot(axisNorm, axes[0])) +
			(extents[1] * glm::sign(glm::dot(axisNorm, axes[1])) * glm::dot(axisNorm, axes[1])) +
			(extents[2] * glm::sign(glm::dot(axisNorm, axes[2])) * glm::dot(axisNorm, axes[2]));
		projOtherBox = (otherW * glm::sign(glm::dot(axisNorm, other.getAxis(0))) * glm::dot(axisNorm, other.getAxis(0))) +
			(otherH * glm::sign(glm::dot(axisNorm, other.getAxis(1))) * glm::dot(axisNorm, other.getAxis(1))) +
			(otherL * glm::sign(glm::dot(axisNorm, other.getAxis(2))) * glm::dot(axisNorm, other.getAxis(2)));
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

	glm::vec3 const& CollisionMesh::getAxis(int axis) const
	{
		return axes[axis];
	}

	float CollisionMesh::getExtent(int extent) const
	{
		return extents[extent];
	}

	glm::vec3 const& CollisionMesh::getCenter() const
	{
		return center;
	}

	void CollisionMesh::generateCollisionInfo(ICollisionMesh const& other, float intersectTime)
	{
		//TODO: generate surface of other which this collided with
		
	//	ISurface* surface = createSurface()
	}

	ICollisionMesh* createCollisionMesh(float l, float w, float h)
	{
		return new CollisionMesh(l, w, h);
	}
}