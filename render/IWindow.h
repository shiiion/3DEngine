#pragma once

#include <glm\glm.hpp>

namespace ginkgo
{
	class IWindow
	{
		virtual void update() const = 0;
		virtual bool closed() const = 0;
		//void clear() const; //deprecated lol

		virtual unsigned int getWidth() const = 0;
		virtual unsigned int getHeight() const = 0;
		virtual float getAspectRatio() const = 0;
		virtual const glm::vec4& getClearColor() const = 0;

		virtual void setClearColor(const glm::vec4& color) = 0;

		virtual bool isKeyPressed(unsigned int keycode) const = 0;
		virtual bool isMouseButtonPressed(unsigned int button) const = 0;
		virtual void getMousePosition(double& x, double& y) const = 0;
		virtual void getScrollOffset(double& xoffset, double& yoffset) const = 0;

		virtual void disableMouseCursor() const = 0;
		virtual void enableMouseCursor() const = 0;
		virtual void setMousePosition(double x, double y) const = 0;

	};
}