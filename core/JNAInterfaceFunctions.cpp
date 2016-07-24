#include "JNAInterfaceFunctions.h"

#ifdef _WIN64

#include "IEntity.h"
#include "ICollisionMesh.h"
#include "IPhysicsObject.h"
#include <glm/gtx/rotate_vector.hpp>
#include "CollisionMesh.h"

namespace ginkgo
{
	IEntity* createAddEntity(float x, float y, float z, float xr, float yr, float zr, float xv, float yv, float zv, float xe, float ye, float ze, float rebound, int dynamic, float mass, float friction)
	{
		glm::vec3 xAxis = glm::rotateX(glm::rotateY(glm::rotateZ(glm::vec3(1, 0, 0), xr), yr), zr);
		glm::vec3 yAxis = glm::rotateX(glm::rotateY(glm::rotateZ(glm::vec3(0, 1, 0), xr), yr), zr);
		glm::vec3 zAxis = glm::rotateX(glm::rotateY(glm::rotateZ(glm::vec3(0, 0, 1), xr), yr), zr);

		ICollisionMesh* pcm = createCollisionMesh(xe, ye, ze, xAxis, yAxis, zAxis);

		IEntity* newEnt = entityFactory(glm::vec3(x, y, z), glm::vec3(xr, yr, zr), glm::vec3(xv, yv, zv));

		int dyn = (dynamic ? CTYPE_WORLDDYNAMIC : CTYPE_WORLDSTATIC);

		Material mat;
		mat.reboundFraction = rebound;
		mat.friction = friction;

		newEnt->setPhysics(physicsObjectFactory(newEnt, pcm, dyn, mass, mat));
		getWorld()->addEntity(newEnt);
		return newEnt;
	}

	float getEntityLoc(IEntity* entity, int ind)
	{
		switch (ind)
		{
		case 0:
			return entity->getPosition().x;
		case 1:
			return entity->getPosition().y;
		case 2:
			return entity->getPosition().z;
		default:
			return 0;
		}
	}

	float getAxis(IEntity* entity, int ind, int ind2)
	{
		glm::vec3 axis;

		switch (ind)
		{
		case 0:
			axis = ((CollisionMesh*)entity->getPhysics()->getCollisionMesh())->getAxis(0) * ((CollisionMesh*)entity->getPhysics()->getCollisionMesh())->getExtent(0);
			break;
		case 1:
			axis = ((CollisionMesh*)entity->getPhysics()->getCollisionMesh())->getAxis(1) * ((CollisionMesh*)entity->getPhysics()->getCollisionMesh())->getExtent(1);
			break;
		case 2:
			axis = ((CollisionMesh*)entity->getPhysics()->getCollisionMesh())->getAxis(2) * ((CollisionMesh*)entity->getPhysics()->getCollisionMesh())->getExtent(2);
			break;
		default:
			return 0;
		}

		switch (ind2)
		{
		case 0:
			return axis.x;
		case 1:
			return axis.y;
		case 2:
			return axis.z;
		default:
			return 0;
		}
	}

	void setVel(IEntity* entity, float x, float y, float z)
	{
		entity->setVelocity(glm::vec3(x, y, z));
	}
}

#endif