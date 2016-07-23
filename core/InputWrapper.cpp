
#include "InputWrapper.h"
#include "Core.h"
#include "IAbstractInputSystem.h"
#ifndef _WIN64
#include <GLFW\glfw3.h>

namespace ginkgo
{
	void setKeyCallback(GLFWwindow* window, int key, int sc, int action, int mod)
	{
		for (IAbstractInputSystem* s : getAllInputSystems())
		{
			Control const& ctl = s->getControl(key);
			if (ctl.inputType == INPUTTYPE_USER && action != GLFW_REPEAT)
			{
				s->onInputCode(ctl, action == GLFW_PRESS);
			}
		}
	}

	void setMouseButtonCallback(GLFWwindow* window, int button, int action, int mod)
	{
		for (IAbstractInputSystem * s : getAllInputSystems())
		{
			Control const& ctl = s->getControl(button);
			if (ctl.inputType == INPUTTYPE_USER)
			{
				s->onInputCode(ctl, action == GLFW_PRESS);
			}
		}
	}

	void registerCallbacks(GLFWwindow* window)
	{

		glfwSetKeyCallback(window, setKeyCallback);
		glfwSetMouseButtonCallback(window, setMouseButtonCallback);
	}
}
#else
namespace ginkgo
{
	void registerCallbacks(GLFWwindow* window)
	{
		//do nothing
	}
}
#endif