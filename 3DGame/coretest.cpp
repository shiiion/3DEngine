//
//#include <Windows.h>
//
//#include <IWindow.h>
//#include <ICamera.h>
//#include <ResourceManagement.h>
//#include <ICubeMap.h>
//#include <IRenderable.h>
//#include <Material.h>
//#include <Layer.h>
//#include <IPhongShader.h>
//#include <glm/gtc/matrix_transform.hpp>
//#include <glm/gtx/rotate_vector.hpp>
//#include <glm/gtx/quaternion.hpp>
//#include <Renderer.h>
//
//#include <IAbstractInputSystem.h>
//#include <Core.h>
//#include <IEntity.h>
//#include <IPhysicsComponent.h>
//#include <ICollisionMesh.h>
//#include <IWorld.h>
//#include <IRenderComponent.h>
//#include <GLFW\glfw3.h>
//
//using namespace ginkgo;
//
//bool pressf, pressb, pressl, pressr, mouselol;
//
//float prevX = 0, prevY = 0, dx = 0, dy = 0;
//float up = 15;
//
//void move(IAbstractInputSystem* sys, int oc, bool set)
//{
//	switch (oc)
//	{
//	case 1:
//		pressf = set;
//		break;
//	case 2:
//		pressb = set;
//		break;
//	case 3:
//		pressl = set;
//		break;
//	case 4:
//		pressr = set;
//		break;
//	}
//}
//
//void mouseTest(IAbstractInputSystem* sys, int oc, float a, float b)
//{
//	dx = a - prevX;
//	dy = b - prevY;
//	prevX = a;
//	prevY = b;
//}
//
//void mousePress(IAbstractInputSystem* sys, int oc, bool set)
//{
//	mouselol = set;
//}
//
//void setupInput(IWindow* w)
//{
//	setupInput(w->getInternalWindow());
//	IAbstractInputSystem* sys1 = createUserInputSystem();
//	registerInputSystem(sys1, nullptr);
//
//	sys1->addCommand(CommandSetReset(1, move));
//	sys1->bindInputCode(GLFW_KEY_W, 1);
//	sys1->addCommand(CommandSetReset(2, move));
//	sys1->bindInputCode(GLFW_KEY_S, 2);
//	sys1->addCommand(CommandSetReset(3, move));
//	sys1->bindInputCode(GLFW_KEY_A, 3);
//	sys1->addCommand(CommandSetReset(4, move));
//	sys1->bindInputCode(GLFW_KEY_D, 4);
//	sys1->addCommand(Command2f(5, mouseTest));
//	sys1->bindInputCode(INCODE_MOUSE, 5);
//	sys1->addCommand(CommandSetReset(6, mousePress));
//	sys1->bindInputCode(GLFW_MOUSE_BUTTON_1, 6);
//}
//
//IRenderable* one;
//IRenderable* two;
//IRenderable* three;
//IRenderable* suz;
//
//void setupRendering(IRenderer* renderer)
//{
//	Mesh* suzanne = loadMesh("test_resources/models/smallsmooth.obj", "suzanne");
//	Mesh* shower = loadMesh("test_resources/models/cylinder2.obj", "shower");
//	Mesh* showerhead = loadMesh("test_resources/models/plane.ob", "showerHead");
//
//	Texture* gray = createTexture("test_resources/textures/epic.png", false, "gray");
//	Texture* waterDrop = createTexture("test_resources/textures/waterdrop.png", false, "water");
//	Texture* road = createTexture("test_resources/textures/car1.png", false, "road");
//	Texture* suzietex = createTexture("test_resources/textures/suzanne.png", false, "suzetex");
//
//	//pretty->refractiveIndex = pretty2->refractiveIndex = pretty3->refractiveIndex = pretty4->refractiveIndex = pretty5->refractiveIndex = pretty6->refractiveIndex = 0;
//	Material* pretty = new Material(gray);
//	Material* waterpretty = new Material(waterDrop);
//	Material* roade = new Material(road);
//	Material* LOl = new Material(suzietex);
//	//pretty->refractiveIndex = 0;
//	//pretty->rIntensity = 0.2f;
//
//	one = renderableFactory(shower, pretty);
//	two = renderableFactory(showerhead, pretty);
//	three = renderableFactory(shower, roade);
//	suz = renderableFactory(suzanne, LOl);// CHANGE
//
//	std::map<unsigned int, std::string> skyboxImages;
//	skyboxImages[CM_LEFT] = "test_resources/skybox/shower.jpg";
//	skyboxImages[CM_RIGHT] = "test_resources/skybox/shower.jpg";
//	skyboxImages[CM_FRONT] = "test_resources/skybox/shower.jpg";
//	skyboxImages[CM_BACK] = "test_resources/skybox/shower.jpg";
//	skyboxImages[CM_TOP] = "test_resources/skybox/shower.jpg";
//	skyboxImages[CM_BOTTOM] = "test_resources/skybox/shower.jpg";
//	renderer->loadSkybox(skyboxImages, 50000);
//
//	two->getTransform().scaleMatrix(vec3(0.6f, 0.05f, 1.3f));
//	two->getTransform().translateMatrix(vec3(0, 3.94f, 1.3f));
//	three->getTransform().translateMatrix(vec3(0, 3.64f, 1.3f * 1.5f));
//	three->getTransform().scaleMatrix(vec3(3, 0.08f, 3));
//
//	//suz->getTransform().translateMatrix(vec3(0, 0, 2));
//
//	renderer->addRenderable(one);
//	renderer->addRenderable(two);
//	renderer->addRenderable(three);
//	renderer->addRenderable(suz);
//	//renderer->addRenderable(five);
//	//renderer->addRenderable(six);
//
//	renderer->setAmbientLight(vec4(0.5f, 0.5f, 0.5f, 1));
//
//	renderer->setDirectionalLight(DirectionalLight(BaseLight(vec4(1, 1, 1, 1), 0.1f), normalize(vec3(1, 1, 0))));
//	//int ind = renderer->addPointLight(PointLight(BaseLight(vec4(1, 1, 1, 1), 1), Attenuation(0, 0, 0.5f), vec3(-12, 3, -16)));
//	//int ind2 = renderer->addPointLight(PointLight(BaseLight(vec4(1, 1, 1, 1), 1), Attenuation(0, 5, 25), vec3(0, 1.5f, -5)));
//	//int ind3 = renderer->addPointLight(PointLight(BaseLight(vec4(1, 1, 1, 1), 1), Attenuation(0, 5, 25), vec3(0, 1.5f, -5)));
//	//int ind4 = renderer->addPointLight(PointLight(BaseLight(vec4(1, 1, 1, 1), 1), Attenuation(0, 5, 25), vec3(0, 1.5f, -5)));
//	//int ind5 = renderer->addPointLight(PointLight(BaseLight(vec4(1, 1, 1, 1), 1), Attenuation(0, 5, 25), vec3(0, 1.5f, -5)));
//	//int ind6 = renderer->addPointLight(PointLight(BaseLight(vec4(1, 1, 1, 1), 1), Attenuation(0, 5, 25), vec3(0, 1.5f, -5)));
//	//int ind7 = renderer->addPointLight(PointLight(BaseLight(vec4(1, 1, 1, 1), 1), Attenuation(0, 5, 25), vec3(0, 1.5f, -5)));
//	//int ind8 = renderer->addPointLight(PointLight(BaseLight(vec4(1, 1, 1, 1), 1), Attenuation(0, 5, 25), vec3(0, 1.5f, -5)));
//	//int ind9 = renderer->addPointLight(PointLight(BaseLight(vec4(1, 1, 1, 1), 1), Attenuation(0, 5, 25), vec3(0, 1.5f, -5)));
//	renderer->getCamera()->setCameraPosition(vec3(7, 0, 7));
//	renderer->getCamera()->setCameraRotation(glm::angleAxis(-PI / 4.f, vec3(0, 1, 0)));
//	renderer->createTextComponent("C:\\Windows\\Fonts\\arial.ttf", 48);
//}
//
//void setupEntity()
//{
//	setTickTime(1.f / 60.f);
//
//	ICollisionMesh* still = createCollisionMesh(1, 1, 1);
//
//	PhysMaterial super;
//	super.reboundFraction = 0;
//	IEntity* newEnt1 = entityFactory(glm::vec3(0, 0, 0));
//	newEnt1->setPhysics(PhysicsComponentFactory(newEnt1, still, CTYPE_WORLDSTATIC, 1, super, true));
//
//	newEnt1->setGravityEnabled(false);
//
//
//	IRenderComponent* rc = renderComponentFactory(newEnt1, suz);
//
//	rc->setScale(vec3(1, 1, 1));
//
//	newEnt1->setRenderable(rc);
//	newEnt1->setPosition(vec3(0, 0, 2));
//	newEnt1->setRotation(glm::angleAxis(PI / 2.f, vec3(0, 1, 0)));
//
//	getWorld()->addEntity(newEnt1);
//}
//
//void spawnWater()
//{
//	Material* waterpretty = new Material(retrieveTexture("water"));
//	IRenderable* wcube = renderableFactory(retrieveMesh("showerHead"), waterpretty);
//	getRendererInstance()->addRenderable(wcube);
//	ICollisionMesh* cm = createCollisionMesh(0.001f, 0.003f, 0.001f);
//
//	PhysMaterial super;
//	super.reboundFraction = 0;
//	IEntity* water = entityFactory(glm::vec3(0, 0, 0), quat(), vec3(0, -5.f, 0));
//	water->setPhysics(PhysicsComponentFactory(water, cm, CTYPE_WORLDDYNAMIC, 1, super, true));
//
//	water->setGravityEnabled(false);
//
//
//	IRenderComponent* rc = renderComponentFactory(water, wcube);
//
//	rc->setScale(vec3(0.1f, 0.3f, 0.1f));
//
//	water->setRenderable(rc);
//	float rx = (float)(rand() % 1000) / 1000.f - 0.5f;
//	float rz = (float)(rand() % 1000) / 1000.f - 0.5f;
//	water->setPosition(vec3(0 + rx, 3.2f, 2 + rz));
//
//	getWorld()->addEntity(water);
//}
//
//int main()
//{
//	IWindow* w = windowFactory("render", 800, 800, glm::vec4(0, 0, 0, 0));
//
//	startCore();
//	setupInput(w);
//	IRenderer* renderer = initRenderer(w);
//	setupRendering(renderer);
//	setupEntity();
//
//	//enables the text shader and loads given font
//	float pitch = 0, yaw = -PI / 4.f;
//	long long pt = 0;
//
//	//int textID = renderer->addText("cool", 300, 650, 1, vec3(0, 0.2f, 0));
//	srand(time(NULL));
//	float strt = getEngineTime();
//	int ctr = 0;
//	while (true)
//	{
//		mat4 const& view = renderer->getCamera()->getView();
//		glm::vec3 right;
//		glm::vec3 forward;
//
//		right.x = view[0][0];
//		right.y = view[1][0];
//		right.z = view[2][0];
//		forward.x = -view[0][2];
//		forward.y = -view[1][2];
//		forward.z = -view[2][2];
//		right *= 0.02f;
//		forward *= 0.02f;
//
//		if (getEngineTime() - strt > 0.1f)
//		{
//			strt = getEngineTime();
//			ctr++;
//			spawnWater();
//		}
//		if (pressf)
//		{
//			//up += 0.1f;
//			renderer->getCamera()->setCameraPosition(renderer->getCamera()->getCameraPosition() + forward);
//		}
//		if (pressb)
//		{
//			//up -= 0.1f;
//			renderer->getCamera()->setCameraPosition(renderer->getCamera()->getCameraPosition() - forward);
//		}
//		if (pressl)
//		{
//			renderer->getCamera()->setCameraPosition(renderer->getCamera()->getCameraPosition() - right);
//		}
//		if (pressr)
//		{
//			renderer->getCamera()->setCameraPosition(renderer->getCamera()->getCameraPosition() + right);
//			//whenToGo = true;
//		}
//		if (mouselol)
//		{
//			pitch += dy / 500.0f;
//			yaw += dx / 500.f;
//			renderer->getCamera()->setCameraRotation(glm::normalize(glm::angleAxis(pitch, vec3(1, 0, 0)) * glm::angleAxis(yaw, vec3(0, 1, 0))));
//		}
//		
//		for (int a = 1; a < getWorld()->getEntityList().size();a++)
//		{
//			vector<IEntity*> const& el = getWorld()->getEntityList();
//			if (el[a]->getPhysics()->getNumCollisions() > 0)
//			{
//				getWorld()->removeEntity(el[a]->getEntityID());
//				a--;
//				continue;
//			}
//		}
//
//		tickCore(1);
//		renderer->renderAndSwap();
//		//renderer->editText(textID, std::to_string(millis - pt));
//
//		//renderer->getCamera()->setCameraPosition(max);
//		sleepTickTime();
//	}
//}