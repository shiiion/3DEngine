
#include <Core.h>
#include <IWorld.h>
#include <IEntity.h>

using namespace ginkgo;

int main()
{
	startCore();
	ginkgo::setTickTime(1.f / 120.f);
	lockPhysics();
	IEntity* newEnt = entityFactory(glm::vec3(), glm::vec3(), glm::vec3(1, 0, 0));
	getWorld()->addEntity(newEnt);
	unlockPhysics();
	while (true)
	{
		lockPhysics();
		printf("%f\n", getWorld()->getEntity(1)->getPosition().x);
		unlockPhysics();
		std::this_thread::sleep_for(std::chrono::milliseconds(16));
	}
}