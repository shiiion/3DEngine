#pragma once

#include "LightStructs.h"

namespace ginkgo
{
	
	class IPhongShader
	{
		virtual void updateUniforms(const glm::mat4& model, const glm::mat4& transformProjectionViewModel, const Material& material, const glm::vec3& cameraPosition) const = 0;

		virtual const glm::vec4& getAmbientLight() const = 0;
		virtual void setAmbientLight(const glm::vec4& ambientLight) = 0;

		virtual void setPointLights(const std::vector<PointLight> pointLights) = 0;
		virtual void setPointLightPosition(unsigned int index, const glm::vec3& position) = 0;

		virtual const DirectionalLight& getDirectionalLight() const = 0;
		virtual void setDirectionalLight(const DirectionalLight& directionalLight) = 0;

		virtual void setUniform(const std::string& name, const DirectionalLight& directionalLight) const = 0;
		virtual void setUniform(const std::string& name, const BaseLight& baseLight) const = 0;
		virtual void setUniform(const std::string& name, const PointLight& pointLight) const = 0;
	};
}