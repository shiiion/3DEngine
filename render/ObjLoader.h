#pragma once

#include "RenderResource.h"
#include <glm/glm.hpp>

namespace ginkgo {

	class ObjLoader
	{
	private:
		vector<vec3> positions;
		vector<vec2> uvs;
		vector<vec3> normals;
		vector<unsigned int> indices;
	public:
		ObjLoader(const string& path);
		
		const vector<vec3>& getPositionList() const { return positions; }
		const vector<vec2>& getUVList() const { return uvs; }
		const vector<vec3>& getNormalList() const { return normals; }
		const vector<unsigned int>& getIndexList() const { return indices; }

	};


}