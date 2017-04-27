#include <iostream>

#include "Window.h"

namespace ginkgo {
	Window::Window(const char* name, int width, int height, const glm::vec4& clear_color, bool isFullScreen)
		: title(name), width(width), height(height), clear_color(clear_color), isFullScreen(isFullScreen)
	{
		if (!init())
			glfwTerminate();

		for (int i = 0; i < MAX_KEYS; i++)
			keys[i] = false;

		for (int i = 0; i < MAX_BUTTONS; i++)
			mouseButtons[i] = false;
	}

	Window::~Window()
	{
		//glfwDestroyWindow(window); //doesn't actually matter cuz glfwTerminate destroys any remaining windows
		glfwTerminate();
	}

	bool Window::init()
	{
		if (!glfwInit())
		{
			std::cout << "Failed to initialize GLFW!" << std::endl;
			return false;
		}

		//window resize
		glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
		window = (isFullScreen) ?
			glfwCreateWindow(width, height, title, glfwGetPrimaryMonitor(), NULL) :
			glfwCreateWindow(width, height, title, NULL, NULL);
		if (!window)
		{
			glfwTerminate();
			std::cout << "Failed to return a window!" << std::endl;
			return false;
		}

		glfwMakeContextCurrent(window);
		glfwSetWindowUserPointer(window, this);
		glfwSetFramebufferSizeCallback(window, window_resize_callback);
		glfwSetKeyCallback(window, key_callback);
		glfwSetMouseButtonCallback(window, mouse_button_callback);
		glfwSetCursorPosCallback(window, cursor_position_callback);
		glfwSetScrollCallback(window, scroll_callback);
		//optimize for more than 60 fps
		glfwSwapInterval(0);

		//************************************************
		//glew must be initialized AFTER glfw makes context 
		//************************************************

		if (glewInit() != GLEW_OK)
		{
			std::cout << "Could not initialize GLEW!" << std::endl;
			return false;
		}

		std::cout << "OpenGL " << glGetString(GL_VERSION) << std::endl;

		/**********Other Settings**********/
		glFrontFace(GL_CW);	//Drawing front face in Clockwise //TBB //already
		glCullFace(GL_BACK); //Not drawing back face	//TBB
		//glEnable(GL_CULL_FACE);	//Not drawing unnecessary front and back stuff //TBB
		//glEnable(GL_DEPTH_TEST); //Z component for depth //TBB //already
		//glDepthFunc(GL_LESS); //already
		glEnable(GL_DEPTH_CLAMP);//Depth clamp so camera won't be halfway inside or outside //TBB //already
		//glEnable(GL_FRAMEBUFFER_SRGB); //More gamma correction, all other colors are already exponential, it does it for us
		//Wireframe
		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		
		return true;
	}

	bool Window::isKeyPressed(unsigned int keycode) const
	{
		if (keycode >= MAX_KEYS)
			return false;

		return keys[keycode];
	}

	bool Window::isMouseButtonPressed(unsigned int button) const
	{
		if (button >= MAX_BUTTONS)
			return false;

		return mouseButtons[button];
	}

	void Window::getMousePosition(double& x, double& y) const
	{
		x = mx;
		y = my;
	}

	void Window::getScrollOffset(double& xvalue, double& yvalue) const
	{
		xvalue = s_xoffset;
		yvalue = s_yoffset;
	}


	/*void Window::clear() const //deprecated lol
	{
		//TODO: change so specific for each framebuffer
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
	}*/

	void Window::update() const
	{
		glfwPollEvents();

		//		glfwGetFramebufferSize(Window, &Width, &Height);
		glfwSwapBuffers(window);
	}

	bool Window::closed() const
	{
		return glfwWindowShouldClose(window) == 1;
	}

	void window_resize_callback(GLFWwindow* window, int width, int height)
	{
		glViewport(0, 0, width, height);
		Window* win = (Window *)glfwGetWindowUserPointer(window);
		win->width = width;
		win->height = height;
	}

	void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		Window* win = (Window *)glfwGetWindowUserPointer(window);
		win->keys[key] = (action != GLFW_RELEASE);
	}

	void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
	{
		Window* win = (Window *)glfwGetWindowUserPointer(window);
		win->mouseButtons[button] = (action != GLFW_RELEASE);
	}

	void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
	{
		Window* win = (Window *)glfwGetWindowUserPointer(window);
		win->mx = xpos;
		win->my = ypos;
	}

	void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
	{
		Window* win = (Window *)glfwGetWindowUserPointer(window);
		win->s_xoffset = xoffset;
		win->s_yoffset = yoffset;
	}


}