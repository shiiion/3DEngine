
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
			Bind const& ctl = s->getControl(key);
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
			Bind const& ctl = s->getControl(button);
			if (ctl.inputType == INPUTTYPE_USER)
			{
				s->onInputCode(ctl, action == GLFW_PRESS);
			}
		}
	}

	void setMouseMoveCallback(GLFWwindow* window, double x, double y)
	{
		for (IAbstractInputSystem* s : getAllInputSystems())
		{
			Bind const& ctl = s->getControl(INCODE_MOUSE);
			if (ctl.inputType == INPUTTYPE_USER)
			{
				Command2f* cmd = (Command2f*)s->onInputCode(ctl, true);
				if (cmd != nullptr)
				{
					cmd->a = x;
					cmd->b = y;
				}
			}
		}
	}

	void registerCallbacks(GLFWwindow* window)
	{
		glfwSetKeyCallback(window, setKeyCallback);
		glfwSetMouseButtonCallback(window, setMouseButtonCallback);
		glfwSetCursorPosCallback(window, setMouseMoveCallback);
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