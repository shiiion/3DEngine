#pragma once

#include "Renderable.h"

#include "Mesh.h"
#include "Material.h"
#include "Transform.h"

namespace ginkgo {
	
	int Renderable::index = 0;

	Renderable::Renderable(const Mesh* mesh, Material* material)
		: mesh(mesh), material(material)
	{
		r_index = index;
		index++;
	}

	const mat4& Renderable::getModel() const 
	{ 
		return model.getMatrix(); 
	}


	void Renderable::draw() const
	{
		mesh->draw(); 
	}

	IRenderable* renderableFactory(const Mesh* mesh, Material* material)
	{
		return new Renderable(mesh, material);
	}
}
