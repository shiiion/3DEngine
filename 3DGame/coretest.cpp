
#include <Core.h>
#include <IWorld.h>
#include <IEntity.h>
#include <ICharacter.h>
#include <IPhysicsObject.h>
#include <ICollisionMesh.h>
#include <glm/gtx/rotate_vector.hpp>
#include <windows.h>
#include <string>
#include <sstream>
#include <glfw/glfw3.h>

using namespace ginkgo;

int main()
{
	glfwInit();
	GLFWwindow* window = glfwCreateWindow(500, 500, "cool person", nullptr, nullptr);
	startCore();


	ginkgo::setupInput(window);
	glfwShowWindow(window);

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
	Material super;
	super.reboundFraction = 0;
	super.friction = 1;
	ICharacter* newEnt1 = characterFactory(glm::vec3(0, 5, 0), glm::vec3(), glm::vec3(0, 0, 0));
	newEnt1->setPhysics(physicsObjectFactory(newEnt1, moving, CTYPE_WORLDDYNAMIC, 1, super, true));

	getWorld()->addEntity(newEnt1);

	IEntity* newEnt2 = entityFactory(glm::vec3(0, -4, 0), glm::vec3(), glm::vec3(0, 0, 0));
	newEnt2->setPhysics(physicsObjectFactory(newEnt2, still, CTYPE_WORLDSTATIC, 1, super, true));
	getWorld()->addEntity(newEnt2);
	newEnt2->setGravityEnabled(false);
//	getWorld()->addEntity(newEnt2);
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
		unsigned int movementState = ((ICharacter*)getWorld()->getEntity(1))->isGravityEnabled();
		printf("position: %f, %f, %f, %d\n", b.x, b.y, b.z, movementState);
		tickCore(1);

		iter++;
		sleepTickTime();
		
	}
}