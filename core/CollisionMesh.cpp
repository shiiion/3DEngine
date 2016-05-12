#include "CollisionMesh.h"
#include "IPhysicsObject.h"
#include "ISurface.h"
#include <glm/gtx/rotate_vector.hpp>
#include "Core.h"
#include "IWorld.h"

namespace ginkgo
{
	CollisionMesh::CollisionMesh(float l, float w, float h, glm::vec3 const& lAxis, glm::vec3 const& wAxis, glm::vec3 const& hAxis)
	{
		owner = nullptr;
		extents[0] = w;
		extents[1] = l;
		extents[2] = h;

		axes[0] = wAxis;
		axes[1] = lAxis;
		axes[2] = hAxis;
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
				if(testAxis(glm::normalize(glm::cross(axes[a], other.getAxis(b))), other, deltaTime))
				{
					return newSeparatingAxis = true;
				}
			}
		}
		if (newSeparatingAxis)
		{
			newSeparatingAxis = false;
			getLastSeparatingAxis(other, deltaTime);
			if (lastSeparatingAxisType == 1)
			{
				printf("%f", deltaTime);
			}
			else
			{

				printf("zzzzzzz", deltaTime);
			}
			generateCollisionInfo(other, getCollisionTime(lastSeparatingAxis, other, deltaTime));
		}
		return false;
	}

	void CollisionMesh::getLastSeparatingAxis(ICollisionMesh const& other, float deltaTime)
	{
		float longestTime = getCollisionTime(axes[0], other, deltaTime);
		float ct;
		lastSeparatingAxis = axes[0];
		lastSeparatingAxisType = AXIS_THIS_1;

		if (glm::dot(lastSeparatingAxis, other.getLastMove().centerStart - lastMove.centerStart) < 0)
		{
			intersectSide = -1;
		}
		else
		{
			intersectSide = 1;
		}
		for (int a = 0; a < 3; a++)
		{
			if (longestTime < (ct = getCollisionTime(axes[a], other, deltaTime)))
			{
				longestTime = ct;
				lastSeparatingAxis = axes[a];
				lastSeparatingAxisType = AXIS_THIS_1 + a;
				if (glm::dot(lastSeparatingAxis, other.getLastMove().centerStart - lastMove.centerStart) < 0)
				{
					intersectSide = -1;
				}
				else
				{
					intersectSide = 1;
				}
			}
			if (longestTime < (ct = getCollisionTime(other.getAxis(a), other, deltaTime)))
			{
				longestTime = ct;
				lastSeparatingAxis = other.getAxis(a);
				lastSeparatingAxisType = AXIS_OTHER_1 + a;
				if (glm::dot(lastSeparatingAxis, other.getLastMove().centerStart - lastMove.centerStart) < 0)
				{
					intersectSide = -1;
				}
				else
				{
					intersectSide = 1;
				}
			}
			for (int b = 0; b < 3; b++)
			{
				if (longestTime < (ct = getCollisionTime(glm::normalize(glm::cross(axes[a], other.getAxis(b))), other, deltaTime)))
				{
					longestTime = ct;
					lastSeparatingAxis = glm::normalize(glm::cross(axes[a], other.getAxis(b)));
					lastSeparatingAxisType = AXIS_CROSS(a, b);
					if (glm::dot(lastSeparatingAxis, other.getLastMove().centerStart - lastMove.centerStart) < 0)
					{
						intersectSide = -1;
					}
					else
					{
						intersectSide = 1;
					}
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

		float sigma = glm::dot(axisNorm, centerDiff);


		if (sigma < 0)
		{
			return (r + glm::dot(axisNorm, centerDiff)) / (-glm::dot(axisNorm, velDiff));;
		}
		if (sigma > 0)
		{
			return (r - glm::dot(axisNorm, centerDiff)) / (glm::dot(axisNorm, velDiff));;
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

	void CollisionMesh::this_lastSeparatingAxis(ICollisionMesh const& other, float intersectTime)
	{
		CollisionInfo collision(*this, other);

		collision.collisionType = AGAINST_FACE_COLLISION;
		collision.collisionTime = intersectTime;
		bool dotProductEqualsZero = false;
		int index = 0;
		switch (lastSeparatingAxisType)
		{
		case AXIS_THIS_1:
			index = 0;
			break;
		case AXIS_THIS_2:
			index = 1;
			break;
		case AXIS_THIS_3:
			index = 2;
			break;
		}
		glm::vec3 centerDiff = (other.getLastMove().centerStart - lastMove.centerStart) + (other.getLastMove().velStart - lastMove.velEnd) * intersectTime;
		for (int a = 0; (a < 3) && (!dotProductEqualsZero); a++)
		{
			for (int b = 0; b < 3; b++)
			{
				if (glm::dot(axes[a], other.getAxis(b)) == 0)
				{
					dotProductEqualsZero = true;
					break;
				}
			}
		}
		if (dotProductEqualsZero)
		{
			
			float extentSum = 0;
			for (int a = 0; a < 3; a++)
			{
				extentSum += glm::abs(glm::dot(axes[a], other.getAxis(0))) * extents[a];
			}
			float one = -glm::dot(other.getAxis(0), centerDiff) - extentSum;

			extentSum = 0;

			for (int a = 0; a < 3; a++)
			{
				extentSum += glm::abs(glm::dot(axes[a], other.getAxis(1))) * extents[a];
			}
			float two = -glm::dot(other.getAxis(1), centerDiff) - extentSum;
			extentSum = 0;

			for (int a = 0; a < 3; a++)
			{
				extentSum += glm::abs(glm::dot(axes[a], other.getAxis(2))) * extents[a];
			}
			float three = -glm::dot(other.getAxis(2), centerDiff) - extentSum;

			
		}
		else
		{

		}
	}

	CollisionInfo CollisionMesh::generateCollisionInfo(ICollisionMesh const& other, float intersectTime)
	{
		//TODO: generate surface of other which this collided with
		//if this axis is last sep., it is this face against 
		//if other axis is last sep., it is other face against
		switch (lastSeparatingAxisType)
		{
		case AXIS_THIS_1:
		case AXIS_THIS_2:
		case AXIS_THIS_3:
			this_lastSeparatingAxis(other, intersectTime);
			break;
		case AXIS_OTHER_1:
		case AXIS_OTHER_2:
		case AXIS_OTHER_3:
			break;
		case AXIS_1X1:
			break;
		case AXIS_1X2:
			break;
		case AXIS_1X3:
			break;
		case AXIS_2X1:
			break;
		case AXIS_2X2:
			break;
		case AXIS_2X3:
			break;
		case AXIS_3X1:
			break;
		case AXIS_3X2:
			break;
		case AXIS_3X3:
			break;
		}
		return CollisionInfo(*this, other);
	}

	ICollisionMesh* createCollisionMesh(float l, float w, float h, glm::vec3 const& lAxis, glm::vec3 const& wAxis, glm::vec3 const& hAxis)
	{
		return new CollisionMesh(l, w, h, lAxis, wAxis, hAxis);
	}
}