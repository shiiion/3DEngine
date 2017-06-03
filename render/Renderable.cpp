#pragma once

#include "Renderable.h"

#include "Mesh.h"
#include "Material.h"
#include "Transform.h"

namespace ginkgo {
	
	int Renderable::index = 0;

	Renderable::Renderable(const Mesh* mesh, Material* material, const mat4& model)
		: mesh(mesh), material(material), model(model)
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

	IRenderable* renderableFactory(const Mesh* mesh, Material* material, const mat4& model)
	{
		return new Renderable(mesh, material, model);
	}
}
