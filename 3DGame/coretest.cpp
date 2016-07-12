
#include <Core.h>
#include <IWorld.h>
#include <IEntity.h>
#include <IPhysicsObject.h>
#include <ICollisionMesh.h>
#include <glm/gtx/rotate_vector.hpp>

using namespace ginkgo;

int main()
{
	startCore();
	ginkgo::setTickTime(1.f / 60.f);
	//ICollisionMesh* moving = createCollisionMesh(1, 1, 1, glm::rotateZ(glm::rotateX(glm::vec3(1, 0, 0), PI / 4.f), PI / 4.f),
	//	glm::rotateZ(glm::vec3(0, 1, 0), PI / 4.f),
	//	glm::rotateZ(glm::vec3(0, 0, 1), PI / 4.f));

	//ICollisionMesh* still = createCollisionMesh(1, 1, 1, glm::rotateX(glm::vec3(1, 0, 0), PI / 4.f), 
	//		glm::rotateX(glm::vec3(0, 1, 0), PI / 4.f), 
	//		glm::rotateX(glm::vec3(0, 0, 1), PI / 4.f));
	ICollisionMesh* moving = createCollisionMesh(0.5, 0.5, 0.5, glm::vec3(1, 0, 0),
		glm::vec3(0, 1, 0),
		glm::vec3(0, 0, 1));

	ICollisionMesh* moving2 = createCollisionMesh(0.5, 0.5, 0.5, glm::vec3(1, 0, 0),
		glm::vec3(0, 1, 0),
		glm::vec3(0, 0, 1));
	Material super;
	super.reboundFraction = 0.2;
	IPhysicsObject* newEnt = physicsObjectFactory(moving, CTYPE_WORLDDYNAMIC, 1, super, nullptr, glm::vec3(0, 5, 0), true, true, glm::vec3(1, 1, 1), glm::vec3(0, 0, 0), glm::vec3(0.1, 0, 0));
	IPhysicsObject* newEnt2 = physicsObjectFactory(moving2, CTYPE_WORLDDYNAMIC, 1, super, nullptr, glm::vec3(0, 0, 0), true, true, glm::vec3(1, 1, 1), glm::vec3(0, 0, 0), glm::vec3(0, 2, 0));
	getWorld()->addEntity(newEnt);
	getWorld()->addEntity(newEnt2);
	while (true)
	{
		tickCore();

		glm::vec3 a = getWorld()->getEntity(1)->getPosition();
		glm::vec3 b = getWorld()->getEntity(2)->getPosition();
		printf("1: %f, %f, %f | 2: %f, %f, %f\n", a.x, a.y, a.z, b.x, b.y, b.z);
		sleepTickTime();
	}
}