#pragma once

#include "RenderResource.h"


namespace ginkgo
{
	class IWindow;
	class ICamera
	{
	public:
		virtual void setProjection(const mat4& projection) = 0;
		virtual void setView(const mat4& view) = 0;
		virtual void setFOV(float fov) = 0;
		virtual void setCameraPosition(const vec3& cameraPosition) = 0;
		virtual void setCameraRotation(const quat& rot) = 0;

		virtual const mat4& getProjection() const = 0;
		virtual const mat4& getView() const = 0;
		virtual const vec3& getCameraPosition() const = 0;
		virtual const mat4& getCameraPositionTranslation() = 0;
		virtual const quat& getCameraRotation() const = 0;

		virtual void update() const = 0;

	};

	DECLSPEC_RENDER ICamera* cameraFactory(const IWindow* win);
}