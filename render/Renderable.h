#pragma once

#include <gl/glew.h>

#include "IRenderable.h"
#include "Transform.h"

namespace ginkgo {

	class Mesh;
	class Material;

	class Renderable : public IRenderable
	{
	private:
		const Mesh* mesh;
		Material* material;
		Transform model;
		static unsigned int index;
		unsigned int r_index;
	public:
		Renderable(const Mesh* mesh, Material* material, const glm::mat4& model = glm::mat4());

		void setMesh(const Mesh* mesh) override { this->mesh = mesh; }
		void setMaterial(Material* material) override { this->material = material; }
		Material* alterMaterial() const override { return material; }
		
		const glm::mat4& getModel() const override;
		Transform& alterModel() override { return model; }

		const Mesh& getMesh() const override { return *mesh; }
		const Material& getMaterial() const override { return *material; }

		const unsigned int getIndex() const override { return r_index; }

		void draw() const override;
	};

}

/*
Renderables all share a 
Mesh (pointer)
but each have a unique Material
but (possible) share the Material's const Texture

change to one Mesh changes all Rendrables
change to one Material changes one Rendrables
cannot change Material's const Texture

cannot change Material of a mesh?
*/