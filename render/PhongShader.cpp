#include <iostream>
#include <string>

#include "PhongShader.h"

#include "Material.h"
#include "LightStructs.h"


namespace ginkgo {

	PhongShader::PhongShader()
	{
		addVertexShader("Render/res/shaders/phongVertex.vs");
		addFragmentShader("Render/res/shaders/phongFragment.fs");
		compileShader();

		ambientLight = glm::vec4(0.1f, 0.1f, 0.1f, 1.0f);
		directionalLight = DirectionalLight(BaseLight(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	}

	PhongShader::~PhongShader()
	{

	}

	void PhongShader::updateUniforms(const glm::mat4& model, const glm::mat4& transformProjectionViewModel, const Material& material, const glm::vec3& cameraPosition) const
	{
		setUniformMat4("model", model);
		setUniformMat4("transform", transformProjectionViewModel);

		if (material.texture != nullptr)
		{
			setUniform4f("baseColor", material.color);
			setUniform4f("ambientLight", ambientLight);
			setUniform("directionalLight", directionalLight);
			setUniform1f("specularIntensity", material.specularIntensity);
			setUniform1f("specularPower", material.specularPower);
		
			for (unsigned int i = 0; i < pointLights.size(); i++)
				setUniform("pointLights[" + std::to_string(i) + "]", pointLights[i]);
		}

		setUniform3f("cameraPosition", cameraPosition);

		setUniform1f("refractiveIndex", material.refractiveIndex);
		setUniform1i("hasTexture", material.texture != nullptr);
		setUniform1f("rIntensity", material.rIntensity);
	}

	void PhongShader::setPointLights(const std::vector<PointLight> pointLights)
	{
		if (pointLights.size() > MAX_POINT_LIGHTS)
		{
			std::cout << "Error: You passed in too many point lights. Max allowed is " <<
				MAX_POINT_LIGHTS << ", you passed in " << pointLights.size() << std::endl;
			system("pause");
		}
		this->pointLights = pointLights;
	}

	void PhongShader::setPointLightPosition(unsigned int index, const glm::vec3& pos)
	{
		pointLights[index].position = pos;
	}

	void PhongShader::setDirectionalLight(const DirectionalLight& directionalLight)
	{
		//DirectionalLight a = directionalLight; a.direction = glm::normalize(directionalLight.direction);
		this->directionalLight = directionalLight;
	}


	void PhongShader::setUniform(const std::string& name, const BaseLight& baseLight) const
	{
		setUniform4f((name + ".color").c_str(), baseLight.color);
		setUniform1f((name + ".intensity").c_str(), baseLight.intensity);
	}

	void PhongShader::setUniform(const std::string& name, const DirectionalLight& DirectionalLight) const
	{
		setUniform((name + ".base").c_str(), directionalLight.base);
		setUniform3f((name + ".direction").c_str(), directionalLight.direction);
	}

	void PhongShader::setUniform(const std::string& name, const PointLight& pointLight) const
	{
		setUniform((name + ".base").c_str(), pointLight.base);
		setUniform1f((name + ".attenuation.constant").c_str(), pointLight.attenuation.constant);
		setUniform1f((name + ".attenuation.linear").c_str(), pointLight.attenuation.linear);
		setUniform1f((name + ".attenuation.quadratic").c_str(), pointLight.attenuation.quadratic);
		setUniform3f((name + ".position").c_str(), pointLight.position);
	}

}