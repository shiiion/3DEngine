//
//#include <Core.h>
//#include <IWorld.h>
//#include <IEntity.h>
//#include <IPhysicsObject.h>
//#include <ICollisionMesh.h>
//#include <glm/gtx/rotate_vector.hpp>
//
//using namespace ginkgo;
//
//int main()
//{
//	startCore();
//	ginkgo::setTickTime(1.f / 60.f);
//	//ICollisionMesh* moving = createCollisionMesh(1, 1, 1, glm::rotateZ(glm::rotateX(glm::vec3(1, 0, 0), PI / 4.f), PI / 4.f),
//	//	glm::rotateZ(glm::vec3(0, 1, 0), PI / 4.f),
//	//	glm::rotateZ(glm::vec3(0, 0, 1), PI / 4.f));
//
//	//ICollisionMesh* still = createCollisionMesh(1, 1, 1, glm::rotateX(glm::vec3(1, 0, 0), PI / 4.f), 
//	//		glm::rotateX(glm::vec3(0, 1, 0), PI / 4.f), 
//	//		glm::rotateX(glm::vec3(0, 0, 1), PI / 4.f));
//
//	for (int a = 0; a < 90; a++)
//	{
//
//	
//		ICollisionMesh* moving = createCollisionMesh(1, 1, 1, glm::vec3(1, 0, 0),
//			glm::vec3(0, 1, 0),
//			glm::vec3(0, 0, 1));
//
//		Material super;
//		super.reboundFraction = 0;
//		super.friction = .5f;
//
//		IEntity* newEnt = entityFactory(glm::vec3(rand() * 5000, rand() * 5000, rand() * 5000), glm::vec3(), glm::vec3(0, 0, 0));
//
//		newEnt->setPhysics(physicsObjectFactory(newEnt, moving, CTYPE_WORLDDYNAMIC, 1, super, true, true));
//
//		getWorld()->addEntity(newEnt);
//	}
//	int iter = 0;
//	while (true)
//	{
//		tickCore();
//
//		glm::vec3 b = getWorld()->getEntity(3)->getVelocity();
//		glm::vec3 a = getWorld()->getEntity(3)->getPosition();
//		printf("1: %f, %f, %f | 2: %f, %f, %f\n", a.x, a.y, a.z, b.x, b.y, b.z);
//		iter++;
//		sleepTickTime();
//		
//	}
//}