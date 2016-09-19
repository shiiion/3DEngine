
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
	ICollisionMesh* moving = createCollisionMesh(1, 1, 1, glm::vec3(1, 0, 0),
		glm::vec3(0, 1, 0),
		glm::vec3(0, 0, 1));

	ICollisionMesh* still = createCollisionMesh(100, 1, 100, glm::vec3(1, 0, 0),
		glm::vec3(0, 1, 0),
		glm::vec3(0, 0, 1));
	Material super;
	super.reboundFraction = 0;
	super.friction = 1;
	IEntity* newEnt1 = entityFactory(glm::vec3(0, 5, 0), glm::vec3(), glm::vec3(0, 0, 0));
	newEnt1->setPhysics(physicsObjectFactory(newEnt1, moving, CTYPE_WORLDDYNAMIC, 1, super, true, true));

	getWorld()->addEntity(newEnt1);

	IEntity* newEnt2 = entityFactory(glm::vec3(0, -4, 0), glm::vec3(), glm::vec3(0, 0, 0));
	newEnt2->setPhysics(physicsObjectFactory(newEnt2, still, CTYPE_WORLDSTATIC, 1, super, true, true));

	getWorld()->addEntity(newEnt2);
	//for (int a = -2; a < -1; a++)
	//{
	//	for (int b = -2; b < -1; b++)
	//	{

	//		ICollisionMesh* moving = createCollisionMesh(0.5f, 0.5f, 0.5f, glm::vec3(1, 0, 0),
	//			glm::vec3(0, 1, 0),
	//			glm::vec3(0, 0, 1));

	//		

	//		IEntity* newEnt = entityFactory(glm::vec3(a * 1.1f, 1.1f, b * 1.1f), glm::vec3(), glm::vec3(1, 0, 0));

	//		newEnt->setPhysics(physicsObjectFactory(newEnt, moving, CTYPE_WORLDDYNAMIC, 1, super, true, true));

	//		getWorld()->addEntity(newEnt);
	//	}
	//}
	int iter = 0;
	while (true)
	{
		glm::vec3 b = getWorld()->getEntity(1)->getPosition();
		printf("position: %f, %f, %f\n", b.x, b.y, b.z);
		tickCore(1);

		iter++;
		sleepTickTime();
		
	}
}