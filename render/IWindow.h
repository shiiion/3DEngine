#pragma once

#include "RenderResource.h"
#include <glm\glm.hpp>

struct GLFWwindow;

namespace ginkgo
{

	class IWindow
	{
	public:
		virtual void update() const = 0;
		virtual bool closed() const = 0;
		//void clear() const; //deprecated lol

		virtual unsigned int getWidth() const = 0;
		virtual unsigned int getHeight() const = 0;
		virtual float getAspectRatio() const = 0;
		virtual const vec4& getClearColor() const = 0;

		virtual void setClearColor(const vec4& color) = 0;

		virtual bool isKeyPressed(unsigned int keycode) const = 0;
		virtual bool isMouseButtonPressed(unsigned int button) const = 0;
		virtual void getMousePosition(double& x, double& y) const = 0;
		virtual void getScrollOffset(double& xoffset, double& yoffset) const = 0;

		virtual void disableMouseCursor() const = 0;
		virtual void enableMouseCursor() const = 0;
		virtual void setMousePosition(double x, double y) const = 0;

		virtual GLFWwindow* getInternalWindow() = 0;
	};

	DECLSPEC_RENDER IWindow* windowFactory(const char* name, int width, int height, const vec4& clear_color, bool isFullScreen = false);
}