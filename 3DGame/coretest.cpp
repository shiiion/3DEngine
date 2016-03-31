
#include <Core.h>
#include <IWorld.h>
#include <IEntity.h>

using namespace ginkgo;

int main()
{
	startCore();
	lockPhysics();
	IEntity* newEnt = entityFactory(glm::vec3(), glm::vec3(), glm::vec3(1, 0, 0));
	getWorld()->addEntity(newEnt);
	unlockPhysics();
	while (true)
	{
		printf("%f\n", getWorld()->getEntity(1)->getPosition().x);
		std::this_thread::sleep_for(std::chrono::milliseconds(50));
	}
}