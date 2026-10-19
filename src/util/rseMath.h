#pragma once

#include <glm/glm.hpp>

namespace RSE {
	class rseMath{
	public:

		static glm::mat4 trsToTransformMatrix(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale);

	private:

	};
}