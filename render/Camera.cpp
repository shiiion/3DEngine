#include <iostream>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/quaternion.hpp>

#include "Camera.h"

#include "Window.h"
#include "FileUtils.h"

namespace ginkgo 
{
	Camera::Camera(const IWindow* win)
		: window(win)
	{
		viewOOD = true;
		fov = 45.0f;

		projection = glm::perspective(fov, window->getAspectRatio(), 0.1f, 1000.0f);

		quat pitch = angleAxis(0.0f, vec3(1, 0, 0));
		quat yaw = angleAxis(0.0f, vec3(0, 1, 0));

		cameraRotation = normalize(pitch * yaw);
		view = toMat4(cameraRotation);
	}

	const mat4& Camera::getCameraPositionTranslation()
	{
		position_translation[3][0] = -getCameraPosition().x;
		position_translation[3][1] = -getCameraPosition().y;
		position_translation[3][2] = -getCameraPosition().z;
		return position_translation;
	}
	
	void Camera::update() const
	{
		view = toMat4(cameraRotation);
	}

	ICamera* cameraFactory(const IWindow* win)
	{
		return new Camera(win);
	}
}