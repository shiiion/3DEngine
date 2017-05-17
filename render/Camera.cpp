#include <iostream>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/quaternion.hpp>

#include "Camera.h"

#include "Window.h"
#include "FileUtils.h"

namespace ginkgo {
	Camera::Camera(const Window* win, const glm::vec3& camera_position)
		: window(win), cameraPosition(camera_position)
	{
		fov = 45.0f;

		projection = glm::perspective(fov, window->getAspectRatio(), 0.1f, 1000.0f);

		glm::quat pitch = glm::angleAxis(0.0f, glm::vec3(1, 0, 0));
		glm::quat yaw = glm::angleAxis(0.0f, glm::vec3(0, 1, 0));

		cameraRotation = glm::normalize(pitch * yaw);
		view = glm::toMat4(cameraRotation);

		//cameraSpeedSensitivity = 3.0f; //3.0f
		//mouseRotationSensitivity = 200.f; //5.0f

		window->disableMouseCursor();
		window->setMousePosition(window->getWidth() / 2.0f, window->getHeight() / 2.0f);

		//xSave = window->getWidth() / 2.0;
		//ySave = window->getHeight() / 2.0;
	}

	const glm::mat4& Camera::getCameraPositionTranslation()
	{
		position_translation[3][0] = -getCameraPosition().x;
		position_translation[3][1] = -getCameraPosition().y;
		position_translation[3][2] = -getCameraPosition().z;
		return position_translation;
	}
	//this will be transferred to core
	//void Camera::input(bool& isGameOver, double dt)
	//{
	//	GLfloat cameraSpeed = dt * cameraSpeedSensitivity;
	//	glm::vec3 right;
	//	glm::vec3 forward;

	//	right.x = view[0][0];
	//	right.y = view[1][0];
	//	right.z = view[2][0];

	//	forward.x = -view[0][2];
	//	forward.y = -view[1][2];
	//	forward.z = -view[2][2];

	//	right = glm::normalize(right);
	//	forward = glm::normalize(forward);

	//	if (window->isKeyPressed(GLFW_KEY_W))
	//		cameraPosition += cameraSpeed * forward;
	//	if (window->isKeyPressed(GLFW_KEY_S))
	//		cameraPosition -= cameraSpeed * forward;
	//	if (window->isKeyPressed(GLFW_KEY_A))
	//		cameraPosition -= right * cameraSpeed;
	//	if (window->isKeyPressed(GLFW_KEY_D))
	//		cameraPosition += right * cameraSpeed;

	//	if (window->isKeyPressed(GLFW_KEY_ESCAPE))
	//		isGameOver = true;

	//	static bool pressedPrevious = false;
	//	bool keyPressed = window->isKeyPressed(GLFW_KEY_0);

	//	if (keyPressed && !pressedPrevious)
	//	{
	//		pressedPrevious = true;
	//		FileUtils::screenshot(window->getWidth(), window->getHeight());
	//		std::cout << "Screenshot taken." << std::endl;
	//	}
	//	else if (!keyPressed)
	//	{
	//		pressedPrevious = false;
	//	}

	//	static bool first = true;
	//	double x, y;
	//	window->getMousePosition(x, y);

	//	static float _pitch = 0;
	//	static float _yaw = 0;

	//	double dx = x - xSave;
	//	double dy = y - ySave;

	//	if (x == 0 || y == 0)
	//	{
	//		return;
	//	}

	//	//can comment out?
	//	if (x != 0 && y != 0 && xSave == 0 && ySave == 0 && first)
	//	{
	//		first = false;
	//		xSave = x;
	//		ySave = y;
	//		dx = 0;
	//		dy = 0;
	//	}

	//	_pitch += (dy / mouseRotationSensitivity);
	//	_yaw += (dx / mouseRotationSensitivity);

	//	glm::quat pitch = glm::angleAxis(_pitch, glm::vec3(1, 0, 0));
	//	glm::quat yaw = glm::angleAxis(_yaw, glm::vec3(0, 1, 0));

	//	cameraRotation = glm::normalize(pitch * yaw);

	//	xSave = x;
	//	ySave = y;
	//}

	//void Camera::lensInput(bool& isGameOver, double dt)
	//{
	//	GLfloat cameraSpeed = dt * cameraSpeedSensitivity;
	//	glm::vec3 right;
	//	glm::vec3 forward;

	//	right.x = view[0][0];
	//	right.y = view[1][0];
	//	right.z = view[2][0];

	//	forward.x = -view[0][2];
	//	forward.y = -view[1][2];
	//	forward.z = -view[2][2];

	//	right = glm::normalize(right);
	//	forward = glm::normalize(forward);

	//	if (window->isKeyPressed(GLFW_KEY_ESCAPE))
	//		isGameOver = true;

	//	static bool pressedPrevious = false;
	//	bool keyPressed = window->isKeyPressed(GLFW_KEY_0);

	//	if (keyPressed && !pressedPrevious)
	//	{
	//		pressedPrevious = true;
	//		FileUtils::screenshot(window->getWidth(), window->getHeight());
	//	}
	//	else if (!keyPressed)
	//	{
	//		pressedPrevious = false;
	//	}

	//	static bool first = true;
	//	double x, y;
	//	window->getMousePosition(x, y);

	//	static float _pitch = 0;
	//	static float _yaw = 0;

	//	double dx = x - xSave;
	//	double dy = y - ySave;

	//	if (x == 0 || y == 0)
	//	{
	//		return;
	//	}

	//	//can comment out?
	//	if (x != 0 && y != 0 && xSave == 0 && ySave == 0 && first)
	//	{
	//		first = false;
	//		xSave = x;
	//		ySave = y;
	//		dx = 0;
	//		dy = 0;
	//	}

	//	_pitch += (dy / mouseRotationSensitivity);
	//	_yaw += (dx / mouseRotationSensitivity);

	//	glm::quat pitch = glm::angleAxis(_pitch, glm::vec3(1, 0, 0));
	//	glm::quat yaw = glm::angleAxis(_yaw, glm::vec3(0, 1, 0));

	//	cameraRotation = glm::normalize(pitch * yaw);

	//	xSave = x;
	//	ySave = y;
	//}


	void Camera::update(double dt)
	{
		//window resize
		//projection = glm::perspective(fov, window->getAspectRatio(), 0.1f, 1000.0f);
		view = glm::toMat4(cameraRotation);
	}

}