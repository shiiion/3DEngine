#pragma once

#include <glm/glm.hpp>

namespace ginkgo {

	class Texture;

	struct Material
	{
		const Texture* texture;
		glm::vec4 color;
		float specularIntensity;
		float specularPower;
		float refractiveIndex; //	>= 1.0f -> depends on phongFragment.fs
		float rIntensity; // 0 <= x <= 1

	public:
		static const float REFLECT;// == 0.0f -> depends on phongFragment.fs
		static const float NO_ENVIRONMENTAL_MAPPING;// -1.0f -> depends on phongFragment.fs
	public:
		Material(float refractiveIndex)
			: Material(2, 32, refractiveIndex, 1.0f, glm::vec4(), nullptr)
		{ }

		Material(const Texture* texture)
			: Material(2, 32, NO_ENVIRONMENTAL_MAPPING, 1.0f, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), texture)
		{ }

		Material(const glm::vec4& color, const Texture* texture)
			: Material(2, 32, NO_ENVIRONMENTAL_MAPPING, 1.0f, color, texture)
		{ }

		Material(float refractiveIndex, const Texture* texture)
			: Material(2, 32, refractiveIndex, 1.0f, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), texture)
		{ }

		Material(float refractiveIndex, float rIntensity, const Texture* texture)
			: Material(2, 32, refractiveIndex, rIntensity, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), texture)
		{}

		Material(float refractiveIndex, float rIntensity, const glm::vec4& color, const Texture* texture)
			: Material(2, 32, refractiveIndex, rIntensity, color, texture)
		{ }

		Material(float refractiveIndex, const glm::vec4& color, const Texture* texture)
			: Material(2, 32, refractiveIndex, 1.0f, color, texture)
		{ }

		Material(float specularIntensity, float specularExponent, float refractiveIndex, float rIntensity, const glm::vec4& color, const Texture* texture)
			: specularIntensity(specularIntensity), specularPower(specularExponent), refractiveIndex(refractiveIndex), rIntensity(rIntensity), color(color), texture(texture)
		{ }
	};

}
