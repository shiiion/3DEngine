#pragma once

#include <vector>
#include <string>

#include <glm/glm.hpp>

namespace ginkgo {

	class ObjLoader
	{
	private:
		std::vector<glm::vec3> positions;
		std::vector<glm::vec2> uvs;
		std::vector<glm::vec3> normals;
		std::vector<unsigned int> indices;
	public:
		ObjLoader(const std::string& path);
		
		const std::vector<glm::vec3>& getPositionList() const { return positions; }
		const std::vector<glm::vec2>& getUVList() const { return uvs; }
		const std::vector<glm::vec3>& getNormalList() const { return normals; }
		const std::vector<unsigned int>& getIndexList() const { return indices; }

	};


}