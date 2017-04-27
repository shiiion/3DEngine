#pragma once

#include <gl/glew.h>
#include <glm\glm.hpp>

#include "Transform.h"

namespace ginkgo {

	class Mesh;
	class Material;

	class Renderable
	{
	private:
		const Mesh* mesh;
		Material* material;
		Transform model;
		static unsigned int index;
		unsigned int r_index;
	public:
		Renderable(const Mesh* mesh, Material* material, const glm::mat4& model = glm::mat4());

		void setMesh(const Mesh* mesh) { this->mesh = mesh; }
		void setMaterial(Material* material) { this->material = material; }
		Material* alterMaterial() const { return material; }
		
		const glm::mat4& getModel() const;
		Transform& alterModel() { return model; }

		const Mesh& getMesh() const { return *mesh; }
		const Material& getMaterial() const { return *material; }

		const unsigned int getIndex() const { return r_index; }

		void draw() const;
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