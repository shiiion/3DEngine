
#include <Windows.h>

#include <IWindow.h>
#include <ICamera.h>
#include <ResourceManagement.h>
#include <ICubeMap.h>
#include <IRenderable.h>
#include <Material.h>
#include <Layer.h>
#include <IPhongShader.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/quaternion.hpp>
#include <Renderer.h>

#include <IAbstractInputSystem.h>
#include <Core.h>
#include <IEntity.h>
#include <IPhysicsObject.h>
#include <ICollisionMesh.h>
#include <IWorld.h>
#include <IRenderComponent.h>
#include <GLFW\glfw3.h>


using namespace ginkgo;

bool pressf, pressb, pressl, pressr, mouselol;

float prevX = 0, prevY = 0, dx = 0, dy = 0;

void move(IAbstractInputSystem* sys, int oc, bool set)
{
	switch (oc)
	{
	case 1:
		pressf = set;
		break;
	case 2:
		pressb = set;
		break;
	case 3:
		pressl = set;
		break;
	case 4:
		pressr = set;
		break;
	}
}

void mouseTest(IAbstractInputSystem* sys, int oc, float a, float b)
{
	dx = a - prevX;
	dy = b - prevY;
	prevX = a;
	prevY = b;
}

void mousePress(IAbstractInputSystem* sys, int oc, bool set)
{
	mouselol = set;
}

void setupInput(IWindow* w)
{
	setupInput(w->getInternalWindow());
	IAbstractInputSystem* sys1 = createUserInputSystem();
	registerInputSystem(sys1, nullptr);

	sys1->addCommand(CommandSetReset(1, move));
	sys1->bindInputCode(GLFW_KEY_W, 1);
	sys1->addCommand(CommandSetReset(2, move));
	sys1->bindInputCode(GLFW_KEY_S, 2);
	sys1->addCommand(CommandSetReset(3, move));
	sys1->bindInputCode(GLFW_KEY_A, 3);
	sys1->addCommand(CommandSetReset(4, move));
	sys1->bindInputCode(GLFW_KEY_D, 4);
	sys1->addCommand(Command2f(5, mouseTest));
	sys1->bindInputCode(INCODE_MOUSE, 5);
	sys1->addCommand(CommandSetReset(6, mousePress));
	sys1->bindInputCode(GLFW_MOUSE_BUTTON_1, 6);
}

IRenderable* one;
IRenderable* two;
IRenderable* three;
IRenderable* four;
IRenderable* five;
IRenderable* six;

void setupRendering(IRenderer* renderer)
{
	createTexture("test_resources/textures/EPIC.png", false, "epic");
	Mesh* plane = loadMesh("test_resources/models/plane.ob", "plane");
	Material* pretty = new Material(retrieveTexture("epic"));
	Material* pretty2 = new Material(retrieveTexture("epic"));
	Material* pretty3 = new Material(retrieveTexture("epic"));
	Material* pretty4 = new Material(retrieveTexture("epic"));
	Material* pretty5 = new Material(retrieveTexture("epic"));
	Material* pretty6 = new Material(retrieveTexture("epic"));

	pretty->refractiveIndex = pretty2->refractiveIndex = pretty3->refractiveIndex = pretty4->refractiveIndex = pretty5->refractiveIndex = pretty6->refractiveIndex = 0;
	pretty2->rIntensity = 0.15f;
	pretty->rIntensity = 0.30f;
	pretty3->rIntensity = 0.45f;
	pretty4->rIntensity = 0.60f;
	pretty5->rIntensity = 0.75f;
	pretty6->rIntensity = 0.90f;


	one = renderableFactory(plane, pretty);
	two = renderableFactory(plane, pretty2);
	three = renderableFactory(plane, pretty3);
	four = renderableFactory(plane, pretty4);
	five = renderableFactory(plane, pretty5);
	six = renderableFactory(plane, pretty6);
	std::map<unsigned int, std::string> skyboxImages;
	skyboxImages[CM_LEFT] = "test_resources/skybox/left.jpg";
	skyboxImages[CM_RIGHT] = "test_resources/skybox/right.jpg";
	skyboxImages[CM_FRONT] = "test_resources/skybox/front.jpg";
	skyboxImages[CM_BACK] = "test_resources/skybox/back.jpg";
	skyboxImages[CM_TOP] = "test_resources/skybox/top.jpg";
	skyboxImages[CM_BOTTOM] = "test_resources/skybox/bottom.jpg";
	renderer->loadSkybox(skyboxImages, 50000);
	renderer->addRenderable(one);
	renderer->addRenderable(two);
	renderer->addRenderable(three);
	//renderer->addRenderable(four);
	//renderer->addRenderable(five);
	//renderer->addRenderable(six);

	renderer->setAmbientLight(vec4(0.2f, 0.2f, 0.2f, 1));

	renderer->setDirectionalLight(DirectionalLight(BaseLight(vec4(1, 1, 1, 1), 0.1f), normalize(vec3(1, 1, 0))));
	//int ind = renderer->addPointLight(PointLight(BaseLight(vec4(1, 1, 1, 1), 1), Attenuation(0, 5, 25), vec3(0, 1.5f, -5)));
	//int ind2 = renderer->addPointLight(PointLight(BaseLight(vec4(1, 1, 1, 1), 1), Attenuation(0, 5, 25), vec3(0, 1.5f, -5)));
	//int ind3 = renderer->addPointLight(PointLight(BaseLight(vec4(1, 1, 1, 1), 1), Attenuation(0, 5, 25), vec3(0, 1.5f, -5)));
	//int ind4 = renderer->addPointLight(PointLight(BaseLight(vec4(1, 1, 1, 1), 1), Attenuation(0, 5, 25), vec3(0, 1.5f, -5)));
	//int ind5 = renderer->addPointLight(PointLight(BaseLight(vec4(1, 1, 1, 1), 1), Attenuation(0, 5, 25), vec3(0, 1.5f, -5)));
	//int ind6 = renderer->addPointLight(PointLight(BaseLight(vec4(1, 1, 1, 1), 1), Attenuation(0, 5, 25), vec3(0, 1.5f, -5)));
	//int ind7 = renderer->addPointLight(PointLight(BaseLight(vec4(1, 1, 1, 1), 1), Attenuation(0, 5, 25), vec3(0, 1.5f, -5)));
	//int ind8 = renderer->addPointLight(PointLight(BaseLight(vec4(1, 1, 1, 1), 1), Attenuation(0, 5, 25), vec3(0, 1.5f, -5)));
	//int ind9 = renderer->addPointLight(PointLight(BaseLight(vec4(1, 1, 1, 1), 1), Attenuation(0, 5, 25), vec3(0, 1.5f, -5)));
	renderer->getCamera()->setCameraPosition(vec3(0.8f, 1, 2));
	renderer->createTextComponent("C:\\Windows\\Fonts\\arial.ttf", 12);
}

void setupEntity()
{
	setTickTime(1.f / 60.f);

	ICollisionMesh* moving = createCollisionMesh(1, 1, 1, glm::vec3(1, 0, 0),
		glm::vec3(0, 1, 0),
		glm::vec3(0, 0, 1));
	ICollisionMesh* moving2 = createCollisionMesh(1, 1, 1, glm::vec3(1, 0, 0),
		glm::vec3(0, 1, 0),
		glm::vec3(0, 0, 1));
	ICollisionMesh* moving3 = createCollisionMesh(1, 1, 1, glm::vec3(1, 0, 0),
		glm::vec3(0, 1, 0),
		glm::vec3(0, 0, 1));
	ICollisionMesh* moving4 = createCollisionMesh(1, 1, 1, glm::vec3(1, 0, 0),
		glm::vec3(0, 1, 0),
		glm::vec3(0, 0, 1));
	ICollisionMesh* moving5 = createCollisionMesh(1, 1, 1, glm::vec3(1, 0, 0),
		glm::vec3(0, 1, 0),
		glm::vec3(0, 0, 1));


	ICollisionMesh* still = createCollisionMesh(1, 1, 1, glm::vec3(1, 0, 0),
		glm::vec3(0, 1, 0),
		glm::vec3(0, 0, 1));

	PhysMaterial super;
	super.reboundFraction = 0.8f;
	super.friction = 0;
	IEntity* newEnt1 = entityFactory(glm::vec3(0, 3, 0), glm::vec3(), glm::vec3(0, 55, 0));
	IEntity* newEnt2 = entityFactory(glm::vec3(0, 0, 0), glm::vec3(), glm::vec3(0, 0, 0));
	IEntity* newEnt3 = entityFactory(glm::vec3(0, 6, 0), glm::vec3(), glm::vec3(0, 0, 0));
	//IEntity* newEnt4 = entityFactory(glm::vec3(0, 9, 0), glm::vec3(), glm::vec3(0, 0, 0));
	//IEntity* newEnt5 = entityFactory(glm::vec3(0, 12, 0), glm::vec3(), glm::vec3(0, 0, 0));
	//IEntity* newEnt6 = entityFactory(glm::vec3(0, 15, 0), glm::vec3(), glm::vec3(0, 0, 0));
	newEnt1->setPhysics(physicsObjectFactory(newEnt1, moving, CTYPE_WORLDDYNAMIC, 1, super, true));
	newEnt2->setPhysics(physicsObjectFactory(newEnt2, still, CTYPE_WORLDSTATIC, 1, super, true));
	newEnt3->setPhysics(physicsObjectFactory(newEnt3, moving2, CTYPE_WORLDSTATIC, 1, super, true));
	//newEnt4->setPhysics(physicsObjectFactory(newEnt4, moving3, CTYPE_WORLDDYNAMIC, 1, super, true));
	//newEnt5->setPhysics(physicsObjectFactory(newEnt5, moving4, CTYPE_WORLDDYNAMIC, 1, super, true));
	//newEnt6->setPhysics(physicsObjectFactory(newEnt6, moving5, CTYPE_WORLDDYNAMIC, 1, super, true));

	newEnt1->setGravityEnabled(true);
	newEnt2->setGravityEnabled(false);
	newEnt3->setGravityEnabled(false);
	//newEnt3->setGravityEnabled(false);
	//newEnt5->setGravityEnabled(false);
	//newEnt4->setGravityEnabled(false);
	//newEnt6->setGravityEnabled(false);
	
	IRenderComponent* rc = renderComponentFactory(newEnt1, one);
	IRenderComponent* rc2 = renderComponentFactory(newEnt2, two);
	IRenderComponent* rc3 = renderComponentFactory(newEnt3, three);
	//IRenderComponent* rc4 = renderComponentFactory(newEnt4, four);
	//IRenderComponent* rc5 = renderComponentFactory(newEnt5, five);
	//IRenderComponent* rc6 = renderComponentFactory(newEnt6, six);

	newEnt1->setRenderable(rc);
	newEnt2->setRenderable(rc2);
	newEnt3->setRenderable(rc3);
	//newEnt4->setRenderable(rc4);
	//newEnt5->setRenderable(rc5);
	//newEnt6->setRenderable(rc6);

	getWorld()->addEntity(newEnt1);
	getWorld()->addEntity(newEnt2);
	getWorld()->addEntity(newEnt3);
	//getWorld()->addEntity(newEnt4);
	//getWorld()->addEntity(newEnt5);
	//getWorld()->addEntity(newEnt6);
}

int main()
{
	IWindow* w = windowFactory("render", 800, 800, glm::vec4(0, 0, 0, 0));

	startCore();
	setupInput(w);
	IRenderer* renderer = initRenderer(w);
	setupRendering(renderer);
	setupEntity();

	//enables the text shader and loads given font
	float angle = 0.0f;
	float pitch = 0, yaw = 0;
	long long pt = 0;

	int textID = renderer->addText("cool", 15, 15, 1, vec3(1, 1, 1));
	while (true)
	{
		//drawme->alterModel().rotateMatrix(angle, (vec3(0, 0, 1)));
		//c->setCameraRotation(glm::angleAxis(angle, glm::vec3(0, 1, 0)));
		angle += 0.05f;

		//renderer->setPointLightPosition(ind, vec3(cos(angle) * 2, 1, -5 + sin(angle) * 2));
		//renderer->setPointLightPosition(ind2, vec3(cos(angle - 1) * 2, 1, -5 + sin(angle - 1) * 2));
		//renderer->setPointLightPosition(ind3, vec3(cos(angle - 2) * 2, 1, -5 + sin(angle - 2) * 2));
		//renderer->setPointLightPosition(ind4, vec3(cos(angle - 3) * 2, 1, -5 + sin(angle - 3) * 2));
		//renderer->setPointLightPosition(ind5, vec3(cos(angle - 4) * 2, 1, -5 + sin(angle - 4) * 2));
		//renderer->setPointLightPosition(ind6, vec3(cos(angle - 5) * 2, 1, -5 + sin(angle - 5) * 2));
		//renderer->setPointLightPosition(ind7, vec3(cos(angle - 6) * 2, 1, -5 + sin(angle - 6) * 2));
		//renderer->setPointLightPosition(ind8, vec3(cos(angle - 7) * 2, 1, -5 + sin(angle - 7) * 2));
		//renderer->setPointLightPosition(ind9, vec3(cos(angle - 8) * 2, 1, -5 + sin(angle - 8) * 2));

		//c->update(0.016);
		//reallyDrawMe->alterModel().rotateMatrix(angle, glm::normalize(glm::vec3(1, 1, 1)));
		mat4 const& view = renderer->getCamera()->getView();
		glm::vec3 right;
		glm::vec3 forward;

		right.x = view[0][0];
		right.y = view[1][0];
		right.z = view[2][0];

		forward.x = -view[0][2];
		forward.y = -view[1][2];
		forward.z = -view[2][2];
		right *= 0.02f;
		forward *= 0.02f;
		if (pressf)
		{
			renderer->getCamera()->setCameraPosition(renderer->getCamera()->getCameraPosition() + forward);
		}
		if (pressb)
		{
			renderer->getCamera()->setCameraPosition(renderer->getCamera()->getCameraPosition() - forward);
		}
		if (pressl)
		{
			renderer->getCamera()->setCameraPosition(renderer->getCamera()->getCameraPosition() - right);
		}
		if (pressr)
		{
			renderer->getCamera()->setCameraPosition(renderer->getCamera()->getCameraPosition() + right);
		}
		if (mouselol)
		{
			pitch += dy / 500.0f;
			yaw += dx / 500.f;
			renderer->getCamera()->setCameraRotation(glm::normalize(glm::angleAxis(pitch, vec3(1, 0, 0)) * glm::angleAxis(yaw, vec3(0, 1, 0))));
		}
		auto millis = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
		printf("Zz");
		tickCore(1);
		renderer->renderAndSwap();
		renderer->editText(textID, std::to_string(millis - pt));
		pt = millis;

		sleepTickTime();
	}
}