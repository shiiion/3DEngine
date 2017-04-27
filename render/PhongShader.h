#pragma once

#include <vector>

#include <glm/glm.hpp>

#include "Shader.h"

namespace ginkgo {
	
	class Material;

	struct DirectionalLight;
	struct PointLight;
	struct BaseLight;

	class PhongShader : public Shader
	{
	private:
		//glm::vec4 ambientLight = glm::vec4(0.1f, 0.1f, 0.1f, 0.1f);
		//DirectionalLight directionalLight = { .base = {.color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), .intensity = 0 }, .direction = glm::vec3(1.0f, 1.0f, 1.0f) };
		glm::vec4 ambientLight; //should be static
		const DirectionalLight* directionalLight; //should be static
		std::vector<PointLight*> pointLights;
		const int MAX_POINT_LIGHTS = 4;
		
	public:
		PhongShader();
		~PhongShader();
		void updateUniforms(const glm::mat4& model, const glm::mat4& transformProjectionViewModel, const Material& material, const glm::vec3& cameraPosition) const;

		const glm::vec4& getAmbientLight() const { return ambientLight; }
		void setAmbientLight(const glm::vec4& ambientLight) { this->ambientLight = ambientLight; }

		void setPointLights(const std::vector<PointLight*> pointLights);
		void setPointLightPosition(unsigned int index, const glm::vec3& position);

		const DirectionalLight& getDirectionalLight() const { return *directionalLight; }
		void setDirectionalLight(const DirectionalLight* directionalLight);
		
		void setUniform(const std::string& name, const DirectionalLight& directionalLight) const;
		void setUniform(const std::string& name, const BaseLight& baseLight) const;
		void setUniform(const std::string& name, const PointLight& pointLight) const;
	};


}