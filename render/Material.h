#pragma once

#include <glm/glm.hpp>

namespace ginkgo {

	class Texture;

	class Material
	{
	private:
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
		Material(const Texture* texture);
		Material(const glm::vec4& color, const Texture* texture);
		Material(float refractiveIndex, const Texture* texture);
		Material(float refractiveIndex, float rIntensity, const Texture* texture);
		Material(float specularIntensity, float specularExponent, float refractiveIndex, float rIntensity, const glm::vec4& color, const Texture* texture);
		Material(float refractiveIndex);

		Material(float refractiveIndex, float rIntensity, const glm::vec4& color, const Texture* texture);
		Material(float refractiveIndex, const glm::vec4& color, const Texture* texture);

		void setColor(const glm::vec4& color) { this->color = color; }
		const void setSpecularIntensity(float specular_Intensity) { specularIntensity = specular_Intensity; }
		const void setSpecularPower(float specular_Power) { this->specularPower = specular_Power; }
		const void setRefractiveIndex(float refractiveIndex) { this->refractiveIndex = refractiveIndex; }
		float getRefractiveIndex() const { return this->refractiveIndex; }

		const void setRIntensity(float rCoefficient) { this->rIntensity = rIntensity; }
		float getRIntensity() const { return this->rIntensity; }

		const Texture* const getTexture() const { return texture; }
		const glm::vec4& getColor() const { return color; }
		bool hasTexture() const { return texture != nullptr; }
		float getSpecularIntensity() const { return specularIntensity; }
		float getSpecularPower() const { return specularPower; }


	};

}
