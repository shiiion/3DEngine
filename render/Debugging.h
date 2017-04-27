#pragma once

#include <glm\glm.hpp>

namespace ginkgo {

	class Debugging
	{
	public:
		static void print(const glm::vec3& vector);
		static void print(const glm::mat4& matrix);
	};

}