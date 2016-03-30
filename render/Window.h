#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

namespace ginkgo {

#define MAX_KEYS 1024
#define MAX_BUTTONS 32


	class Window
	{
	private:
		GLFWwindow* window;
		int width, height;
		bool isClosed;
		const char* title;

		bool keys[MAX_KEYS];
		bool mouseButtons[MAX_BUTTONS];

		double m_x, m_y;
		double s_xoffset, s_yoffset;
	public:
		Window(const char* title, int width, int height);
		~Window();
		void update();
		bool closed() const;
		void clear() const;

		inline int getWidth() const { return width; };
		inline int getHeight() const { return height; };

		bool isKeyPressed(unsigned int keycode) const;
		bool isMouseButtonPressed(unsigned int button) const;
		void getMousePosition(double& x, double& y) const;
		void getScrollOffset(double& xoffset, double& yoffset) const;
	private:
		bool init();

		friend void window_resize(GLFWwindow* window, int width, int height);
		friend void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
		friend void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
		friend void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);
		friend void scroll_callback(GLFWwindow* window, double xpos, double ypos);

	};

}
