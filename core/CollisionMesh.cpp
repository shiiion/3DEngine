#include "CollisionMesh.h"
#include "IPhysicsObject.h"
#include "ISurface.h"
#include <glm/gtx/rotate_vector.hpp>
#include "Core.h"
#include "IWorld.h"

namespace ginkgo
{
	CollisionMesh::CollisionMesh(float w, float h, float l, glm::vec3 const& wAxis, glm::vec3 const& hAxis, glm::vec3 const& lAxis)
	{
		owner = nullptr;
		extents[0] = w;
		extents[1] = h;
		extents[2] = l;

		axes[0] = wAxis;
		axes[1] = hAxis;
		axes[2] = lAxis;
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
		}
		for (int a = 0; a < 3;a++)
		{
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

	void CollisionMesh::this_lastSeparatingAxis(ICollisionMesh const& other, float intersectTime, CollisionInfo& collisionOut)
	{
#define CIJ(i, j) (glm::dot(axes[i], other.getAxis(j)))
		collisionOut.collisionTime = intersectTime;
		int index = 0;
		bool dotProductZero = false;
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

		collisionOut.collisionNormal = axes[index] * (float)-intersectSide;
		collisionOut.lastSeparatingAxis = axes[index];

		for (int a = 0; (a < 3) && !dotProductZero; a++)
		{
			for (int b = 0; b < 3; b++)
			{
				if (CIJ(a, b) == 0.f)
				{
					dotProductZero = true;
					break;
				}
			}
		}
		//vertex-face
		if (!dotProductZero)
		{
#define YSUBJ(i, j) (-intersectSide * glm::sign(CIJ(i, j)) * other.getExtent(j))
			glm::vec3 CAI = (lastMove.centerStart + (lastMove.velStart * intersectTime));
			float x, y, z, yxt = 0, yyt = 0, yzt = 0, extentSumX = 0, extentSumY = 0, extentSumZ = 0;
			for (int a = 0; a < 3; a++)
			{
				yxt += CIJ(0, a) * YSUBJ(0, a);
				yyt += CIJ(1, a) * YSUBJ(1, a);
				yzt += CIJ(2, a) * YSUBJ(2, a);

			}
			x = glm::dot(axes[0], centerDiff) + yxt;
			y = glm::dot(axes[1], centerDiff) + yyt;
			z = glm::dot(axes[2], centerDiff) + yzt;

			collisionOut.intersectionPoint = ((x * axes[0]) + (y * axes[1]) + (z * axes[2])) + CAI;
			return;
#undef YSUBJ
		}
		//edge-face or face-face
		else
		{
			MoveInfo const& otherMove = other.getLastMove();
			glm::vec3 CAI = otherMove.centerStart + (otherMove.velStart * intersectTime);
			float sumx = 0, sumy = 0, sumz = 0, minyx, minyy, minyz, maxyx, maxyy, maxyz;
			for (int a = 0; a < 3; a++)
			{
				sumx += glm::abs(glm::dot(axes[a], other.getAxis(0))) * extents[a];
				sumy += glm::abs(glm::dot(axes[a], other.getAxis(1))) * extents[a];
				sumz += glm::abs(glm::dot(axes[a], other.getAxis(2))) * extents[a];
			}

			minyx = glm::dot(-other.getAxis(0), centerDiff) - sumx;
			minyy = glm::dot(-other.getAxis(1), centerDiff) - sumy;
			minyz = glm::dot(-other.getAxis(2), centerDiff) - sumz;
			maxyx = glm::dot(-other.getAxis(0), centerDiff) + sumx;
			maxyy = glm::dot(-other.getAxis(1), centerDiff) + sumy;
			maxyz = glm::dot(-other.getAxis(2), centerDiff) + sumz;

			float x, y, z;
			if (other.getExtent(0) <= maxyx)
			{
				x = other.getExtent(0);
			}
			else if (-other.getExtent(0) >= minyx)
			{
				x = -other.getExtent(0);
			}
			else
			{
				x = minyx;
			}
			
			if (other.getExtent(1) <= maxyy)
			{
				y = other.getExtent(1);
			}
			else if (-other.getExtent(1) >= minyy)
			{
				y = -other.getExtent(1);
			}
			else
			{
				y = minyy;
			}
			
			if (other.getExtent(2) <= maxyz)
			{
				z = other.getExtent(2);
			}
			else if (-other.getExtent(2) >= minyz)
			{
				z = -other.getExtent(2);
			}
			else
			{
				z = minyz;
			}
			collisionOut.intersectionPoint = ((x * other.getAxis(0)) + (y * other.getAxis(1)) + (z * other.getAxis(2))) + CAI;
		}
#undef CIJ
	}

	void CollisionMesh::other_lastSeparatingAxis(ICollisionMesh const& other, float intersectTime, CollisionInfo& collisionOut)
	{
#define CIJ(i, j) (glm::dot(axes[i], other.getAxis(j)))
		collisionOut.collisionTime = intersectTime;
		int index = 0;
		bool dotProductZero = false;
		switch (lastSeparatingAxisType)
		{
		case AXIS_OTHER_1:
			index = 0;
			break;
		case AXIS_OTHER_2:
			index = 1;
			break;
		case AXIS_OTHER_3:
			index = 2;
			break;
		}
		glm::vec3 centerDiff = (other.getLastMove().centerStart - lastMove.centerStart) + (other.getLastMove().velStart - lastMove.velEnd) * intersectTime;

		collisionOut.collisionNormal = other.getAxis(index) * (float)-intersectSide;
		collisionOut.lastSeparatingAxis = other.getAxis(index);

		for (int a = 0; (a < 3) && !dotProductZero; a++)
		{
			for (int b = 0; b < 3; b++)
			{
				if (CIJ(a, b) == 0.f)
				{
					dotProductZero = true;
					break;
				}
			}
		}
		//vertex-face
		if (!dotProductZero)
		{
#define YSUBJ(i, j) (-intersectSide * glm::sign(CIJ(i, j)) * other.getExtent(j))
			glm::vec3 CAI = (lastMove.centerStart + (lastMove.velStart * intersectTime));
			float x, y, z, yxt = 0, yyt = 0, yzt = 0, extentSumX = 0, extentSumY = 0, extentSumZ = 0;
			for (int a = 0; a < 3; a++)
			{
				yxt += CIJ(0, a) * YSUBJ(0, a);
				yyt += CIJ(1, a) * YSUBJ(1, a);
				yzt += CIJ(2, a) * YSUBJ(2, a);

			}
			x = glm::dot(axes[0], centerDiff) + yxt;
			y = glm::dot(axes[1], centerDiff) + yyt;
			z = glm::dot(axes[2], centerDiff) + yzt;

			collisionOut.intersectionPoint = ((x * axes[0]) + (y * axes[1]) + (z * axes[2])) + CAI;
			return;
#undef YSUBJ
		}
		//edge-face or face-face
		else
		{
			MoveInfo const& otherMove = other.getLastMove();
			glm::vec3 CAI = otherMove.centerStart + (otherMove.velStart * intersectTime);
			float sumx = 0, sumy = 0, sumz = 0, minyx, minyy, minyz, maxyx, maxyy, maxyz;
			for (int a = 0; a < 3; a++)
			{
				sumx += glm::abs(glm::dot(axes[a], other.getAxis(0))) * extents[a];
				sumy += glm::abs(glm::dot(axes[a], other.getAxis(1))) * extents[a];
				sumz += glm::abs(glm::dot(axes[a], other.getAxis(2))) * extents[a];
			}

			minyx = glm::dot(-other.getAxis(0), centerDiff) - sumx;
			minyy = glm::dot(-other.getAxis(1), centerDiff) - sumy;
			minyz = glm::dot(-other.getAxis(2), centerDiff) - sumz;
			maxyx = glm::dot(-other.getAxis(0), centerDiff) + sumx;
			maxyy = glm::dot(-other.getAxis(1), centerDiff) + sumy;
			maxyz = glm::dot(-other.getAxis(2), centerDiff) + sumz;

			float x, y, z;
			if (other.getExtent(0) <= maxyx)
			{
				x = other.getExtent(0);
			}
			else if (-other.getExtent(0) >= minyx)
			{
				x = -other.getExtent(0);
			}
			else
			{
				x = minyx;
			}

			if (other.getExtent(1) <= maxyy)
			{
				y = other.getExtent(1);
			}
			else if (-other.getExtent(1) >= minyy)
			{
				y = -other.getExtent(1);
			}
			else
			{
				y = minyy;
			}

			if (other.getExtent(2) <= maxyz)
			{
				z = other.getExtent(2);
			}
			else if (-other.getExtent(2) >= minyz)
			{
				z = -other.getExtent(2);
			}
			else
			{
				z = minyz;
			}
			collisionOut.intersectionPoint = ((x * other.getAxis(0)) + (y * other.getAxis(1)) + (z * other.getAxis(2))) + CAI;
		}
#undef CIJ
	}

	CollisionInfo CollisionMesh::generateCollisionInfo(ICollisionMesh const& other, float intersectTime)
	{
		//TODO: generate surface of other which this collided with
		//if this axis is last sep., it is this face against 
		//if other axis is last sep., it is other face against
		CollisionInfo ret(*this, other);
		switch (lastSeparatingAxisType)
		{
		//FACE V ?
		//? V FACE
		case AXIS_THIS_1:
		case AXIS_THIS_2:
		case AXIS_THIS_3:
			this_lastSeparatingAxis(other, intersectTime, ret);
			break;
		case AXIS_OTHER_1:
		case AXIS_OTHER_2:
		case AXIS_OTHER_3:
			other_lastSeparatingAxis(other, intersectTime, ret);
			break;
		//EDGE V EDGE OH SHIT
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
		return ret;
	}

	ICollisionMesh* createCollisionMesh(float w, float h, float l, glm::vec3 const& wAxis, glm::vec3 const& hAxis, glm::vec3 const& lAxis)
	{
		return new CollisionMesh(w, h, l, wAxis, hAxis, lAxis);
	}
}