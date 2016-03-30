#include "Window.h"
#include <iostream>

namespace ginkgo
{
	Window::Window(const char* name, int w, int h)
		:title(name), width(w), height(h)
	{
		if (!init())
			glfwTerminate();

		for (unsigned int i = 0; i < MAX_KEYS; i++)
			keys[i] = false;

		for (unsigned int i = 0; i < MAX_BUTTONS; i++)
			mouseButtons[i] = false;
	}

	Window::~Window()
	{
		glfwTerminate();
	}

	bool Window::init()
	{

		if (!glfwInit())
		{
			std::cout << "Failed to initialize GLFW!" << std::endl;
			return false;
		}

		//TODO: Might want to change so multiple screens, primary screen can change, fullscreen option, etc,
		window = glfwCreateWindow(width, height, title, NULL, NULL);
		if (!window)
		{
			glfwTerminate();
			std::cout << "Failed to return GLFW window!" << std::endl;	
			return false;
		}

		glfwMakeContextCurrent(window);
		glfwSetWindowUserPointer(window, this);
		glfwSetFramebufferSizeCallback(window, window_resize);
		glfwSetKeyCallback(window, key_callback);
		glfwSetMouseButtonCallback(window, mouse_button_callback);
		glfwSetCursorPosCallback(window, cursor_position_callback);
		glfwSetScrollCallback(window, scroll_callback);

		glfwSwapInterval(0); //The VSync thing for more than 60 fps

		//************************************************
		//glew must be initialized AFTER glfw makes context 
		//************************************************

		if (glewInit() != GLEW_OK)
		{
			std::cout << "Could not initialize GLEW!" << std::endl;
			return false;
		}
		std::cout << "OpenGL " << glGetString(GL_VERSION) << std::endl;
		
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
		x = m_x;
		y = m_y;
	}

	void Window::getScrollOffset(double& xvalue, double& yvalue) const
	{
		xvalue = s_xoffset;
		yvalue = s_yoffset;
	}


	void Window::clear() const
	{
		//glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void Window::update()
	{
		GLenum error = glGetError();
		glfwPollEvents();

		if (error != GL_NO_ERROR)
			std::cout << "OpenGL Error: " << error << std::endl;

		//		glfwGetFramebufferSize(window, &width, &height);
		glfwSwapBuffers(window);
	}

	bool Window::closed() const
	{
		return glfwWindowShouldClose(window) == 1;
	}

	void window_resize(GLFWwindow *window, int width, int height)
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
		win->m_x = xpos;
		win->m_y = ypos;
	}

	void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
	{
		Window* win = (Window *)glfwGetWindowUserPointer(window);
		win->s_xoffset = xoffset;
		win->s_yoffset = yoffset;
	}

}