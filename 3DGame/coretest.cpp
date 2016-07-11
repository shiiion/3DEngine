
#include <Core.h>
#include <IWorld.h>
#include <IEntity.h>
#include <IPhysicsObject.h>
#include <ICollisionMesh.h>
#include <glm/gtx/rotate_vector.hpp>

using namespace ginkgo;

int main()
{
#define PI 0
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

	ICollisionMesh* still = createCollisionMesh(1, 1, 1, glm::vec3(1, 0, 0),
		glm::vec3(0, 1, 0),
		glm::vec3(0, 0, 1));
	IPhysicsObject* newEnt = physicsObjectFactory(moving, CTYPE_WORLDDYNAMIC, 5, Material(), nullptr, glm::vec3(0, -4.4f, 0), true, true, glm::vec3(1, 1, 1), glm::vec3(0, 0, 0), glm::vec3(0.1f, 1.f, 0));
	IPhysicsObject* newEnt2 = physicsObjectFactory(still, CTYPE_WORLDSTATIC, 5, Material(), nullptr, glm::vec3(), true, true);
	getWorld()->addEntity(newEnt);
	getWorld()->addEntity(newEnt2);
	while (true)
	{
		glm::vec3 prevPos = getWorld()->getEntity(1)->getPosition();
		tickCore();
		glm::vec3 diff = getWorld()->getEntity(1)->getPosition();// -prevPos;
		printf("%f, %f, %f\n", diff.x, diff.y, diff.z);
		sleepTickTime();
	}
}