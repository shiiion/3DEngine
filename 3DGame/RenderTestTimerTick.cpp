
#include <Windows.h>
#include <iostream>

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
#include <IPhysicsComponent.h>
#include <ICollisionMesh.h>
#include <IWorld.h>
#include <IRenderComponent.h>
#include <GLFW\glfw3.h>

#define IDT_NEWFRAME (101)
#define TIMER_WINDOW_CLASS ("TIMER_WINDOW_CLASS")
#define FRAME_DELAY (1000/60)


using namespace ginkgo;

struct rendervars {
	IRenderer* renderer;
	float pitch = 0;
	float yaw = 0;
	long long pt = 0;

	int textID;
	glm::vec3 rotup{ 0, 1, 0 };
	bool whenToGo = false;
	float strt;
};

bool pressf, pressb, pressl, pressr, mouselol;

float prevX = 0, prevY = 0, dx = 0, dy = 0;
float up = 15;

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
	createTexture("test_resources/textures/road.png", false, "road");
	createTexture("test_resources/textures/car1.png", false, "car1");
	createTexture("test_resources/textures/car2.png", false, "car2");
	createTexture("test_resources/textures/car3.png", false, "car3");
	createTexture("test_resources/textures/car4.png", false, "car4");
	createTexture("test_resources/textures/car5.png", false, "car5");
	Mesh* plane = loadMesh("test_resources/models/plane.ob", "plane");
	Mesh* addhash = loadMesh("test_resources/models/monkey.ob", "monkey");
	Material* pretty = new Material(retrieveTexture("car1"));
	Material* pretty2 = new Material(retrieveTexture("road"));
	Material* pretty3 = new Material(retrieveTexture("car2"));
	Material* pretty4 = new Material(retrieveTexture("car3"));
	Material* pretty5 = new Material(retrieveTexture("car4"));
	Material* pretty6 = new Material(retrieveTexture("car5"));

	//pretty->refractiveIndex = pretty2->refractiveIndex = pretty3->refractiveIndex = pretty4->refractiveIndex = pretty5->refractiveIndex = pretty6->refractiveIndex = 0;



	one = renderableFactory(addhash, pretty);
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
	renderer->addRenderable(four);
	renderer->addRenderable(five);
	renderer->addRenderable(six);

	renderer->setAmbientLight(vec4(0.2f, 0.2f, 0.2f, 1));

	renderer->setDirectionalLight(DirectionalLight(BaseLight(vec4(1, 1, 1, 1), 0.1f), normalize(vec3(1, 1, 0))));
	int ind = renderer->addPointLight(PointLight(BaseLight(vec4(1, 1, 1, 1), 1), Attenuation(0, 0, 0.5f), vec3(-12, 3, -16)));
	//int ind2 = renderer->addPointLight(PointLight(BaseLight(vec4(1, 1, 1, 1), 1), Attenuation(0, 5, 25), vec3(0, 1.5f, -5)));
	//int ind3 = renderer->addPointLight(PointLight(BaseLight(vec4(1, 1, 1, 1), 1), Attenuation(0, 5, 25), vec3(0, 1.5f, -5)));
	//int ind4 = renderer->addPointLight(PointLight(BaseLight(vec4(1, 1, 1, 1), 1), Attenuation(0, 5, 25), vec3(0, 1.5f, -5)));
	//int ind5 = renderer->addPointLight(PointLight(BaseLight(vec4(1, 1, 1, 1), 1), Attenuation(0, 5, 25), vec3(0, 1.5f, -5)));
	//int ind6 = renderer->addPointLight(PointLight(BaseLight(vec4(1, 1, 1, 1), 1), Attenuation(0, 5, 25), vec3(0, 1.5f, -5)));
	//int ind7 = renderer->addPointLight(PointLight(BaseLight(vec4(1, 1, 1, 1), 1), Attenuation(0, 5, 25), vec3(0, 1.5f, -5)));
	//int ind8 = renderer->addPointLight(PointLight(BaseLight(vec4(1, 1, 1, 1), 1), Attenuation(0, 5, 25), vec3(0, 1.5f, -5)));
	//int ind9 = renderer->addPointLight(PointLight(BaseLight(vec4(1, 1, 1, 1), 1), Attenuation(0, 5, 25), vec3(0, 1.5f, -5)));
	renderer->getCamera()->setCameraPosition(vec3(0, 15, 18));
	renderer->createTextComponent("C:\\Windows\\Fonts\\arial.ttf", 48);
}

void setupEntity()
{
	setTickTime(1.f / 60.f);

	ICollisionMesh* moving = createCollisionMesh(6, 1.8f, 2);
	ICollisionMesh* moving2 = createCollisionMesh(6, 1.8f, 2);
	ICollisionMesh* moving3 = createCollisionMesh(6, 1.8f, 2);
	ICollisionMesh* moving4 = createCollisionMesh(6, 1.8f, 2);
	ICollisionMesh* moving5 = createCollisionMesh(6, 1.8f, 2);

	ICollisionMesh* still = createCollisionMesh(40000, 1, 100);

	PhysMaterial super;
	PhysMaterial super2;
	PhysMaterial super3;
	PhysMaterial super4;
	PhysMaterial super5;
	super.reboundFraction = 0.7f;
	super2.reboundFraction = 0.7f;
	super3.reboundFraction = 0.7f;
	super4.reboundFraction = 0.7f;
	super5.reboundFraction = 0.7f;
	IEntity* newEnt1 = entityFactory(glm::vec3(0, 5, 0));
	IEntity* newEnt2 = entityFactory(glm::vec3(0, 0, 0));
	IEntity* newEnt3 = entityFactory(glm::vec3(0, 5, -8));
	IEntity* newEnt4 = entityFactory(glm::vec3(0, 5, -16));
	IEntity* newEnt5 = entityFactory(glm::vec3(0, 5, -24));
	IEntity* newEnt6 = entityFactory(glm::vec3(0, 5, -32));
	newEnt1->setPhysics(PhysicsComponentFactory(newEnt1, moving, CTYPE_WORLDDYNAMIC, 1, super, true));
	newEnt2->setPhysics(PhysicsComponentFactory(newEnt2, still, CTYPE_WORLDSTATIC, 1, super, true));
	newEnt3->setPhysics(PhysicsComponentFactory(newEnt3, moving2, CTYPE_WORLDDYNAMIC, 1, super2, true));
	newEnt4->setPhysics(PhysicsComponentFactory(newEnt4, moving3, CTYPE_WORLDDYNAMIC, 1, super3, true));
	newEnt5->setPhysics(PhysicsComponentFactory(newEnt5, moving4, CTYPE_WORLDDYNAMIC, 1, super4, true));
	newEnt6->setPhysics(PhysicsComponentFactory(newEnt6, moving5, CTYPE_WORLDDYNAMIC, 1, super5, true));

	newEnt1->setGravityEnabled(true);
	newEnt2->setGravityEnabled(false);


	IRenderComponent* rc = renderComponentFactory(newEnt1, one);
	IRenderComponent* rc2 = renderComponentFactory(newEnt2, two);
	IRenderComponent* rc3 = renderComponentFactory(newEnt3, three);
	IRenderComponent* rc4 = renderComponentFactory(newEnt4, four);
	IRenderComponent* rc5 = renderComponentFactory(newEnt5, five);
	IRenderComponent* rc6 = renderComponentFactory(newEnt6, six);

	rc2->setScale(vec3(40000, 1, 100));
	rc->setScale(vec3(1, 1, 1));
	rc3->setScale(vec3(6, 1.8f, 2));
	rc4->setScale(vec3(6, 1.8f, 2));
	rc5->setScale(vec3(6, 1.8f, 2));
	rc6->setScale(vec3(6, 1.8f, 2));

	newEnt1->setRenderable(rc);
	newEnt2->setRenderable(rc2);
	newEnt3->setRenderable(rc3);
	newEnt4->setRenderable(rc4);
	newEnt5->setRenderable(rc5);
	newEnt6->setRenderable(rc6);

	//getWorld()->addEntity(newEnt1);
	getWorld()->addEntity(newEnt2);
	getWorld()->addEntity(newEnt3);
	//getWorld()->addEntity(newEnt4);
	//getWorld()->addEntity(newEnt5);
	//getWorld()->addEntity(newEnt6);
}

VOID CALLBACK FrameCallback(PVOID param, BOOLEAN timerorwaitfired)
{
	PostMessage(*(HWND*)param, WM_TIMER, IDT_NEWFRAME, NULL); //just tell the window to do its thing
}

LRESULT CALLBACK TimerWinProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg) {
	case WM_CREATE:
	{
		//don't do anything anymore. timer is handled elsewhere
	}
		break;
	case WM_TIMER:
	{
		rendervars& vars = *(rendervars*)GetWindowLongPtr(hwnd, 0);
		long long ppt = vars.pt;
		mat4 const& view = vars.renderer->getCamera()->getView();
		glm::vec3 right;
		glm::vec3 forward;

		right.x = view[0][0];
		right.y = view[1][0];
		right.z = view[2][0];
		forward.x = -view[0][2];
		forward.y = -view[1][2];
		forward.z = -view[2][2];
		right *= 0.2f;
		forward *= 0.2f;
		if (pressf)
		{
			//up += 0.1f;
			vars.renderer->getCamera()->setCameraPosition(vars.renderer->getCamera()->getCameraPosition() + forward);
		}
		if (pressb)
		{
			//up -= 0.1f;
			vars.renderer->getCamera()->setCameraPosition(vars.renderer->getCamera()->getCameraPosition() - forward);
		}
		if (pressl)
		{
			vars.renderer->getCamera()->setCameraPosition(vars.renderer->getCamera()->getCameraPosition() - right);
		}
		if (pressr)
		{
			vars.renderer->getCamera()->setCameraPosition(vars.renderer->getCamera()->getCameraPosition() + right);
			//whenToGo = true;
		}
		if (mouselol)
		{
			vars.pitch += dy / 500.0f;
			vars.yaw += dx / 500.f;
			vars.renderer->getCamera()->setCameraRotation(glm::normalize(glm::angleAxis(vars.pitch, vec3(1, 0, 0)) * glm::angleAxis(vars.yaw, vec3(0, 1, 0))));
		}
		auto millis = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
		tickCore(1);
		vars.renderer->renderAndSwap();
		//renderer->editText(textID, std::to_string(millis - pt));
		//vec3 v = getWorld()->getEntity(1)->getPosition();
		vars.renderer->editText(vars.textID, !vars.whenToGo ? "Ready" : "GO!!");
		vars.pt = millis;

		vec3 max;
		if (vars.whenToGo)
		{
			for (int a = 1; a <= 6; a++)
			{
				IEntity* e = getWorld()->getEntity(a);
				if (e->getPhysics()->getCollisionType() == CTYPE_WORLDSTATIC) continue;
				e->setVelocity(e->getVelocity() + vec3((float)rand() / 100000.f, 0, 0));
				if (max.x < e->getPosition().x) max = e->getPosition();

			}
		}
		max.z = 18;
		max.x += 8;
		max += up * vars.rotup;

		//renderer->getCamera()->setCameraPosition(max);
		std::cout << millis - ppt << '\n';
	}
		break;
	case WM_CLOSE:
		DestroyWindow(hwnd);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}
	return 0;
}

int main()
{
	IWindow* w = windowFactory("render", 800, 800, glm::vec4(0, 0, 0, 0));
	
	rendervars vars;

	startCore();
	setupInput(w);
	vars.renderer = initRenderer(w);
	setupRendering(vars.renderer);
	setupEntity();

	vars.textID = vars.renderer->addText("cool", 300, 650, 1, vec3(0, 0.2f, 0));
	srand(time(NULL));
	vars.strt = getEngineTime();

	WNDCLASSEX wc;
	ZeroMemory(&wc, sizeof(wc));
	wc.cbSize = sizeof(wc);
	wc.style = CS_VREDRAW | CS_HREDRAW;
	wc.lpfnWndProc = TimerWinProc;
	wc.hInstance = GetModuleHandle(NULL);
	wc.cbClsExtra = 0;
	wc.cbWndExtra = sizeof(void*);
	wc.lpszClassName = TIMER_WINDOW_CLASS;

	ATOM wcTimer = RegisterClassEx(&wc);
	if (!wcTimer) {
		MessageBox(NULL, "Window Registration Failed", "Error", MB_ICONERROR | MB_OK);
		return -1;
	}
	HWND hwnd = CreateWindow(TIMER_WINDOW_CLASS, "timer dummy window", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 400, 300, NULL, NULL, GetModuleHandle(NULL), 0);
	SetWindowLongPtr(hwnd, 0, (LONG_PTR)&vars);
	
	HANDLE frame_timer;
	CreateTimerQueueTimer(&frame_timer, NULL, FrameCallback, &hwnd, 0, FRAME_DELAY, WT_EXECUTEDEFAULT);

	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0) > 0) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}
