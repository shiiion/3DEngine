
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

	ICollisionMesh* moving3 = createCollisionMesh(0.5, 0.5, 0.5, glm::vec3(1, 0, 0),
		glm::vec3(0, 1, 0),
		glm::vec3(0, 0, 1));

	ICollisionMesh* moving4 = createCollisionMesh(0.5, 0.5, 0.5, glm::vec3(1, 0, 0),
		glm::vec3(0, 1, 0),
		glm::vec3(0, 0, 1));

	Material super;
	super.reboundFraction = 1;
	
	IEntity* newEnt = entityFactory(glm::vec3(0, 1.5, 0), glm::vec3(), glm::vec3(0, 0, 0));
	IEntity* newEnt2 = entityFactory(glm::vec3(0, 0, 0), glm::vec3(), glm::vec3(0, 0, 0));
	IEntity* newEnt3 = entityFactory(glm::vec3(0, 3.5, 0), glm::vec3(), glm::vec3(0, 0, 0));
	IEntity* newEnt4 = entityFactory(glm::vec3(0, 6.5, 0), glm::vec3(), glm::vec3(0, 0, 0));

	newEnt->setPhysics(physicsObjectFactory(newEnt, moving, CTYPE_WORLDDYNAMIC, 1, super, true, true));
	newEnt2->setPhysics(physicsObjectFactory(newEnt2, moving2, CTYPE_WORLDSTATIC, 1, super, true, true));
	newEnt3->setPhysics(physicsObjectFactory(newEnt3, moving3, CTYPE_WORLDDYNAMIC, 1, super, true, true));
	newEnt4->setPhysics(physicsObjectFactory(newEnt4, moving4, CTYPE_WORLDDYNAMIC, 1, super, true, true));

	getWorld()->addEntity(newEnt);
	getWorld()->addEntity(newEnt2);
	getWorld()->addEntity(newEnt3);
	getWorld()->addEntity(newEnt4);
	int iter = 0;
	while (true)
	{
		tickCore();

		glm::vec3 a = getWorld()->getEntity(1)->getPosition();
		glm::vec3 b = getWorld()->getEntity(3)->getPosition();
		glm::vec3 c = getWorld()->getEntity(4)->getPosition();
		printf("1: %f, %f, %f | 2: %f, %f, %f | 3: %f, %f, %f\n", a.x, a.y, a.z, b.x, b.y, b.z, c.x, c.y, c.z);
		iter++;
		sleepTickTime();
		
	}
}