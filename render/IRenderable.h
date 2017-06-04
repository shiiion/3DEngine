#pragma once

#include "RenderResource.h"

namespace ginkgo
{

	struct Material;
	class ITransform;
	class Mesh;

	class IRenderable
	{
	public:
		virtual void setMesh(const Mesh* mesh) = 0;
		virtual void setMaterial(Material* material) = 0;
		virtual Material& getMaterial() = 0;

		virtual const mat4& getModel() const = 0;
		virtual ITransform const& getTransform() const = 0;
		virtual ITransform& getTransform() = 0;

		virtual const Mesh& getMesh() const = 0;
		virtual const Material& getMaterial() const = 0;

		virtual const int getIndex() const = 0;

		virtual void draw() const = 0;
	};

	DECLSPEC_RENDER IRenderable* renderableFactory(const Mesh* mesh, Material* material);
}