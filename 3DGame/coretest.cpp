
#include <Core.h>
#include <IWorld.h>
#include <IEntity.h>
#include <IPhysicsObject.h>
#include <ICollisionMesh.h>

using namespace ginkgo;

int main()
{
	startCore();
	ginkgo::setTickTime(1.f / 120.f);
	lockPhysics();
	IPhysicsObject* newEnt = physicsObjectFactory(createCollisionMesh(1, 1, 1), CTYPE_WORLDDYNAMIC, 5, Material(), nullptr, glm::vec3(-3, -3, 0), true, true, glm::vec3(1, 1, 1), glm::vec3(0, 0, 0), glm::vec3(0.5f, 0.5f, 0));
	IPhysicsObject* newEnt2 = physicsObjectFactory(createCollisionMesh(1, 1, 1), CTYPE_WORLDSTATIC, 5, Material(), nullptr, glm::vec3(), true, true);
	getWorld()->addEntity(newEnt);
	getWorld()->addEntity(newEnt2);
	unlockPhysics();
	while (true)
	{
	//	printf("%f, %f, %f\n", getWorld()->getEntity(1)->getPosition().x, getWorld()->getEntity(1)->getPosition().y, getWorld()->getEntity(1)->getPosition().z);
		std::this_thread::sleep_for(std::chrono::milliseconds(16));
	}
}