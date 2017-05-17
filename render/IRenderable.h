#pragma once

#include <glm\glm.hpp>

namespace ginkgo
{

	struct Material;
	class Transform;
	class Mesh;

	class IRenderable
	{
	public:
		virtual void setMesh(const Mesh* mesh) = 0;
		virtual void setMaterial(Material* material) = 0;
		virtual Material* alterMaterial() const = 0;

		virtual const glm::mat4& getModel() const = 0;
		virtual Transform& alterModel() = 0;

		virtual const Mesh& getMesh() const = 0;
		virtual const Material& getMaterial() const = 0;

		virtual const unsigned int getIndex() const = 0;

		virtual void draw() const = 0;
	};
}