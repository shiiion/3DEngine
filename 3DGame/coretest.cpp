
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
	ICollisionMesh* still = createCollisionMesh(100, 0.5f, 100, glm::vec3(1, 0, 0),
		glm::vec3(0, 1, 0),
		glm::vec3(0, 0, 1));
	Material super;
	super.reboundFraction = 0;
	super.friction = 1;
	IEntity* newEnt1 = entityFactory(glm::vec3(0, 0, 0), glm::vec3(), glm::vec3(0, 0, 0));
	newEnt1->setPhysics(physicsObjectFactory(newEnt1, still, CTYPE_WORLDSTATIC, 1, super, true, true));

	getWorld()->addEntity(newEnt1);
	for (int a = -2; a < -1; a++)
	{
		for (int b = -2; b < -1; b++)
		{

			ICollisionMesh* moving = createCollisionMesh(0.5f, 0.5f, 0.5f, glm::vec3(1, 0, 0),
				glm::vec3(0, 1, 0),
				glm::vec3(0, 0, 1));

			

			IEntity* newEnt = entityFactory(glm::vec3(a * 1.1f, 1.1f, b * 1.1f), glm::vec3(), glm::vec3(1, 0, 0));

			newEnt->setPhysics(physicsObjectFactory(newEnt, moving, CTYPE_WORLDDYNAMIC, 1, super, true, true));

			getWorld()->addEntity(newEnt);
		}
	}
	int iter = 0;
	while (true)
	{
		tickCore(1);

		glm::vec3 b = getWorld()->getEntity(2)->getVelocity();
		glm::vec3 a = getWorld()->getEntity(2)->getPosition();
		printf("1: %f, %f, %f | 2: %f, %f, %f\n", a.x, a.y, a.z, b.x, b.y, b.z);
		iter++;
		sleepTickTime();
		
	}
}