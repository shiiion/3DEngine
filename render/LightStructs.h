#pragma once

#include <glm/glm.hpp>

namespace ginkgo {

	struct BaseLight 
	{
		vec4 color;
		float intensity;
		BaseLight() {}
		BaseLight(vec4& colorBL, float intensityBL) : color(colorBL), intensity(intensityBL) {}
	};

	struct DirectionalLight 
	{
		BaseLight base;
		vec3 direction;
		DirectionalLight() {}
		DirectionalLight(BaseLight& baseDL, vec3& directionDL) : base(baseDL), direction(directionDL) {}
	};

	struct Attenuation 
	{
		float constant;
		float linear;
		float quadratic;
		Attenuation() {}
		Attenuation(float constantA, float linearA, float quadraticA) : constant(constantA), linear(linearA), quadratic(quadraticA) {}
	};

	struct PointLight
	{
		BaseLight base;
		Attenuation attenuation;
		vec3 position;
		PointLight() {}
		PointLight(BaseLight& basePL, Attenuation& attenuationPL, vec3& positionPL) : base(basePL), attenuation(attenuationPL), position(positionPL) {}
	};

}