#pragma once

#include <iostream>

#include "Debugging.h"

namespace ginkgo {

	void Debugging::print(const glm::vec3& vector)
	{
		std::cout << vector.x << " " << vector.y << " " << vector.z << std::endl << std::endl;
	}

	void Debugging::print(const glm::mat4& matrix)
	{
		for (int r = 0; r < 4; r++)
		{
			for (int c = 0; c < 4; c++)
				std::cout << matrix[r][c] << " ";
			std::cout << std::endl;
		}
		std::cout << std::endl;
	}

}