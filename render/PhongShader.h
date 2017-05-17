#pragma once

#include <vector>

#include <glm/glm.hpp>

#include "Shader.h"
#include "IPhongShader.h"

namespace ginkgo {
	
	class Material;


	class PhongShader : public Shader, public IPhongShader
	{
	private:
		//glm::vec4 ambientLight = glm::vec4(0.1f, 0.1f, 0.1f, 0.1f);
		//DirectionalLight directionalLight = { .base = {.color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), .intensity = 0 }, .direction = glm::vec3(1.0f, 1.0f, 1.0f) };
		glm::vec4 ambientLight; //should be static
		DirectionalLight directionalLight; //should be static
		std::vector<PointLight> pointLights;
		const int MAX_POINT_LIGHTS = 4;
		
	public:
		PhongShader();
		~PhongShader();
		void updateUniforms(const glm::mat4& model, const glm::mat4& transformProjectionViewModel, const Material& material, const glm::vec3& cameraPosition) const override;

		const glm::vec4& getAmbientLight() const override { return ambientLight; }
		void setAmbientLight(const glm::vec4& ambientLight) override { this->ambientLight = ambientLight; }

		void setPointLights(const std::vector<PointLight> pointLights) override;
		void setPointLightPosition(unsigned int index, const glm::vec3& position) override;

		const DirectionalLight& getDirectionalLight() const override { return directionalLight; }
		void setDirectionalLight(const DirectionalLight& directionalLight) override;
		
		void setUniform(const std::string& name, const DirectionalLight& directionalLight) const override;
		void setUniform(const std::string& name, const BaseLight& baseLight) const override;
		void setUniform(const std::string& name, const PointLight& pointLight) const override;
	};


}