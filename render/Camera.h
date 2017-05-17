#pragma once

#include <GL/glew.h>
#include <glm/gtc/quaternion.hpp>
#include "ICamera.h"

namespace ginkgo {

	class Window;

	class Camera : public ICamera
	{
	private:
		const Window* window;
		glm::mat4 projection;
		glm::mat4 view;
		glm::mat4 position_translation;
		glm::vec3 cameraPosition;
		glm::quat cameraRotation;
		GLfloat fov;
		/*float cameraSpeedSensitivity;
		float mouseRotationSensitivity;
		double xSave;
		double ySave;*/
	public:
		Camera(const Window* window, const glm::vec3& camera_position);
		
		void setProjection(const glm::mat4& projection) override { this->projection = projection; }
		void setView(const glm::mat4& view) override { this->view = view; }
		//void setCameraSpeedSensitivity(float cameraSpeedSensitivity) { this->cameraSpeedSensitivity = cameraSpeedSensitivity; }
		//void setMouseRotationSensitivity(float mouseSensitivity) { this->mouseRotationSensitivity = mouseSensitivity; }
		void setFOV(float fov) override { this->fov= fov; }
		void setCameraPosition(const glm::vec3& cameraPosition) override { this->cameraPosition = cameraPosition; }

		const glm::mat4& getProjection() const override { return projection; }
		const glm::mat4& getView() const override { return view; }
		const glm::vec3& getCameraPosition() const override { return cameraPosition; }
		const glm::mat4& getCameraPositionTranslation() override;
		
		//void input(bool& isGameOver, double dt);
		void update(double dt) override;

		//void lensInput(bool& isGameOver, double dt);
	};

}