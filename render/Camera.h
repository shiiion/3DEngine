#pragma once

#include "ICamera.h"
#include <GL/glew.h>
#include <glm/gtc/quaternion.hpp>

namespace ginkgo {

	class IWindow;

	class Camera : public ICamera
	{
	private:
		const IWindow* window;
		mat4 projection;
		mutable mat4 view;
		mat4 position_translation;
		vec3 cameraPosition;
		quat cameraRotation;
		GLfloat fov;
		mutable bool viewOOD;
	public:
		Camera(const IWindow* window);
		
		void setProjection(const mat4& projection) override { this->projection = projection; }
		void setView(const mat4& view) override { this->view = view; }
		void setFOV(float fov) override { this->fov= fov; }
		void setCameraPosition(const vec3& cameraPosition) override { this->cameraPosition = cameraPosition; }
		void setCameraRotation(const quat& rot) override { this->cameraRotation = rot; viewOOD = true; }

		const mat4& getProjection() const override { return projection; }
		const mat4& getView() const override 
		{
			if(viewOOD)	
			{
				viewOOD = false; update();
			}
			return view;
		}
		const vec3& getCameraPosition() const override { return cameraPosition; }
		const mat4& getCameraPositionTranslation() override;
		const quat& getCameraRotation() const
		{
			return cameraRotation;
		}
		
		void update() const override;

	};

}