#pragma once

namespace ginkgo
{
	class ICamera
	{
	public:
		virtual void setProjection(const glm::mat4& projection) = 0;
		virtual void setView(const glm::mat4& view) = 0;
		virtual void setFOV(float fov) = 0;
		virtual void setCameraPosition(const glm::vec3& cameraPosition) = 0;

		virtual const glm::mat4& getProjection() const = 0;
		virtual const glm::mat4& getView() const = 0;
		virtual const glm::vec3& getCameraPosition() const = 0;
		virtual const glm::mat4& getCameraPositionTranslation() = 0;

		virtual void update(double dt) = 0;

	};
}