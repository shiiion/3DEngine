#include "Material.h"

#include "Texture.h"

namespace ginkgo {

	const float Material::REFLECT = 0.0f;
	const float Material::NO_ENVIRONMENTAL_MAPPING = -1.0f;

	Material::Material(float refractiveIndex)
		: Material(2, 32, refractiveIndex, 1.0f, glm::vec4(), nullptr)
	{ }

	Material::Material(const Texture* texture)
		: Material(2, 32, Material::NO_ENVIRONMENTAL_MAPPING, 1.0f, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), texture)
	{ }

	Material::Material(const glm::vec4& color, const Texture* texture)
		: Material(2, 32, Material::NO_ENVIRONMENTAL_MAPPING, 1.0f, color, texture)
	{ }

	Material::Material(float refractiveIndex, const Texture* texture)
		: Material(2, 32, refractiveIndex, 1.0f, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), texture)
	{ }

	Material::Material(float refractiveIndex, float rIntensity, const Texture* texture)
		: Material(2, 32, refractiveIndex, rIntensity, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), texture)
	{}

	Material::Material(float refractiveIndex, float rIntensity, const glm::vec4& color, const Texture* texture)
		: Material(2, 32, refractiveIndex, rIntensity, color, texture)
	{ }

	Material::Material(float refractiveIndex, const glm::vec4& color, const Texture* texture)
		: Material(2, 32, refractiveIndex, 1.0f, color, texture)
	{ }
	
	Material::Material(float specularIntensity, float specularExponent, float refractiveIndex, float rIntensity, const glm::vec4& color, const Texture* texture)
		: specularIntensity(specularIntensity), specularPower(specularExponent), refractiveIndex(refractiveIndex), rIntensity(rIntensity), color(color), texture(texture)
	{ }
}
