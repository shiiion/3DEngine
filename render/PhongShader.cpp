#include <iostream>

#include "PhongShader.h"

#include "Material.h"
#include "LightStructs.h"


namespace ginkgo {

	PhongShader::PhongShader()
	{
		addVertexShader("shaders/phongVertex.vs");
		addFragmentShader("shaders/phongFragment.fs");
		compileShader();
		lightCounter = 0;

		ambientLight = vec4(0.1f, 0.1f, 0.1f, 1.0f);
		directionalLight = DirectionalLight(BaseLight(vec4(1.0f, 1.0f, 1.0f, 1.0f), 0.0f), vec3(0.0f, 0.0f, 0.0f));
	}

	PhongShader::~PhongShader()
	{

	}

	void PhongShader::updateUniforms(const mat4& model, const mat4& transformProjectionViewModel, const Material& material, const vec3& cameraPosition) const
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
			{
				setUniform("pointLights[" + std::to_string(i) + "]", pointLights[i].second);
			}
		}

		setUniform3f("cameraPosition", cameraPosition);

		setUniform1f("refractiveIndex", material.refractiveIndex);
		setUniform1i("hasTexture", material.texture != nullptr);
		setUniform1f("rIntensity", material.rIntensity);
	}


	int PhongShader::addPointLight(PointLight const& light)
	{
		pointLights.emplace_back(std::pair<int, PointLight>(lightCounter, light));
		return lightCounter++;
	}

	void PhongShader::removePointLight(int index)
	{
		for (unsigned int a = 0; a < pointLights.size(); a++)
		{
			if (pointLights[a].first == index)
			{
				pointLights.erase(pointLights.begin() + a);
				return;
			}
		}
	}

	void PhongShader::setPointLight(int index, const PointLight& light)
	{
		for (unsigned int a = 0; a < pointLights.size(); a++)
		{
			if (pointLights[a].first == index)
			{
				pointLights[a].second = light;
				break;
			}
		}
	}

	void PhongShader::setPointLightPosition(int index, const vec3& pos)
	{
		for (unsigned int a = 0; a < pointLights.size(); a++)
		{
			if (pointLights[a].first == index)
			{
				pointLights[a].second.position = pos;
				break;
			}
		}
	}

	void PhongShader::setDirectionalLight(const DirectionalLight& directionalLight)
	{
		//DirectionalLight a = directionalLight; a.direction = normalize(directionalLight.direction);
		this->directionalLight = directionalLight;
	}

	const PointLight& PhongShader::getPointLight(int index)
	{
		static PointLight invalid;
		

		for (unsigned int a = 0; a < pointLights.size(); a++)
		{
			if (pointLights[a].first == index)
			{
				return pointLights[a].second;
			}
		}
		return invalid;
	}

	void PhongShader::setUniform(const string& name, const BaseLight& baseLight) const
	{
		setUniform4f((name + ".color").c_str(), baseLight.color);
		setUniform1f((name + ".intensity").c_str(), baseLight.intensity);
	}

	void PhongShader::setUniform(const string& name, const DirectionalLight& DirectionalLight) const
	{
		setUniform((name + ".base").c_str(), directionalLight.base);
		setUniform3f((name + ".direction").c_str(), directionalLight.direction);
	}

	void PhongShader::setUniform(const string& name, const PointLight& pointLight) const
	{
		setUniform((name + ".base").c_str(), pointLight.base);
		setUniform1f((name + ".attenuation.constant").c_str(), pointLight.attenuation.constant);
		setUniform1f((name + ".attenuation.linear").c_str(), pointLight.attenuation.linear);
		setUniform1f((name + ".attenuation.quadratic").c_str(), pointLight.attenuation.quadratic);
		setUniform3f((name + ".position").c_str(), pointLight.position);
	}

	IPhongShader* phongShaderFactory()
	{
		return new PhongShader();
	}

}