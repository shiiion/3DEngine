
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
	ginkgo::setTickTime(1.f / 120.f);
	lockPhysics();
#undef PI

#define PI 2.88f
	ICollisionMesh* moving = createCollisionMesh(1, 1, 1, glm::rotateZ(glm::rotateX(glm::vec3(1, 0, 0), PI / 4.f), PI / 4.f),
		glm::rotateZ(glm::rotateX(glm::vec3(0, 1, 0), PI / 4.f), PI / 4.f),
		glm::rotateZ(glm::rotateX(glm::vec3(0, 0, 1), PI / 4.f), PI / 4.f));
#undef PI
#define PI 1.f
	ICollisionMesh* still = createCollisionMesh(1, 1, 1, glm::rotateX(glm::vec3(1, 0, 0), PI / 4.f), 
			glm::rotateX(glm::vec3(0, 1, 0), PI / 4.f), 
			glm::rotateX(glm::vec3(0, 0, 1), PI / 4.f));
	
	IPhysicsObject* newEnt = physicsObjectFactory(moving, CTYPE_WORLDDYNAMIC, 5, Material(), nullptr, glm::vec3(0, -4.4f, 0), true, true, glm::vec3(1, 1, 1), glm::vec3(0, 0, 0), glm::vec3(0, 1.f, 0));
	IPhysicsObject* newEnt2 = physicsObjectFactory(still, CTYPE_WORLDSTATIC, 5, Material(), nullptr, glm::vec3(), true, true);
	getWorld()->addEntity(newEnt);
	getWorld()->addEntity(newEnt2);
	unlockPhysics();
	while (true)
	{
		printf("%f, %f, %f\n", getWorld()->getEntity(1)->getPosition().x, getWorld()->getEntity(1)->getPosition().y, getWorld()->getEntity(1)->getPosition().z);
		std::this_thread::sleep_for(std::chrono::milliseconds(16));
	}
}