#pragma once

#include <GL/glew.h>
#include <glm/gtc/quaternion.hpp>

namespace ginkgo {

	class Window;

	class Camera
	{
	private:
		const Window* window;
		glm::mat4 projection;
		glm::mat4 view;
		glm::mat4 position_translation;
		glm::vec3 cameraPosition;
		glm::quat cameraRotation;
		GLfloat fov;
		float cameraSpeedSensitivity;
		float mouseRotationSensitivity;
		double xSave;
		double ySave;
	public:
		Camera(const Window* window, const glm::vec3& camera_position);
		
		void setProjection(const glm::mat4& projection) { this->projection = projection; }
		void setView(const glm::mat4& view) { this->view = view; }
		void setCameraSpeedSensitivity(float cameraSpeedSensitivity) { this->cameraSpeedSensitivity = cameraSpeedSensitivity; }
		void setMouseRotationSensitivity(float mouseSensitivity) { this->mouseRotationSensitivity = mouseSensitivity; }
		void setFOV(float fov) { this->fov= fov; }
		void setCameraPosition(const glm::vec3& cameraPosition) { this->cameraPosition = cameraPosition; }

		const glm::mat4& getProjection() const { return projection; }
		const glm::mat4& getView() const { return view; }
		const glm::vec3& getCameraPosition() const { return cameraPosition; }
		const glm::mat4& getCameraPositionTranslation();
		
		void input(bool& isGameOver, double dt);
		void update(double dt);

		void lensInput(bool& isGameOver, double dt);
	};

}