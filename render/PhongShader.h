#pragma once

#include "IPhongShader.h"
#include "Shader.h"
#include <glm/glm.hpp>
#include <utility>

namespace ginkgo {
	
	struct Material;


	class PhongShader : public Shader, public IPhongShader
	{
	private:
		//vec4 ambientLight = vec4(0.1f, 0.1f, 0.1f, 0.1f);
		//DirectionalLight directionalLight = { .base = {.color = vec4(1.0f, 1.0f, 1.0f, 1.0f), .intensity = 0 }, .direction = vec3(1.0f, 1.0f, 1.0f) };
		vec4 ambientLight; //should be static
		DirectionalLight directionalLight; //should be static
		vector<std::pair<int, PointLight>> pointLights;
		
		int lightCounter;

	public:
		PhongShader();
		~PhongShader();
		void updateUniforms(const mat4& model, const mat4& transformProjectionViewModel, const Material& material, const vec3& cameraPosition) const override;

		const vec4& getAmbientLight() const override { return ambientLight; }
		void setAmbientLight(const vec4& ambientLight) override { this->ambientLight = ambientLight; }

		int addPointLight(PointLight const& light) override;
		void removePointLight(int index) override;
		void setPointLight(int index, const PointLight& light) override;
		void setPointLightPosition(int index, const vec3& position) override;
		const PointLight& getPointLight(int index) override;

		const DirectionalLight& getDirectionalLight() const override { return directionalLight; }
		void setDirectionalLight(const DirectionalLight& directionalLight) override;
		
		void setUniform(const string& name, const DirectionalLight& directionalLight) const override;
		void setUniform(const string& name, const BaseLight& baseLight) const override;
		void setUniform(const string& name, const PointLight& pointLight) const override;
	};


}