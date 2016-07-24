//#include <Core.h>
//#include <IAbstractInputSystem.h>
//#include <GLFW\glfw3.h>
//
//void testFunc(ginkgo::IAbstractInputSystem* whee, int code, bool set)
//{
//	printf("%d is now %s\n", code, (set ? "pressed" : "released"));
//}
//
//int main()
//{
//	glfwInit();
//	GLFWwindow* window = glfwCreateWindow(500, 500, "black person", nullptr, nullptr);
//	ginkgo::startCore();
//	ginkgo::setupInput(window);
//	glfwShowWindow(window);
//
//	ginkgo::IAbstractInputSystem* is = ginkgo::createUserInputSystem();
//	is->addControl(GLFW_KEY_Z, 0, testFunc);
//
//	ginkgo::registerInputSystem(is, nullptr);
//
//	while (true)
//	{
//		glfwPollEvents();
//		ginkgo::tickCore();
//		ginkgo::sleepTickTime();
//	}
//}