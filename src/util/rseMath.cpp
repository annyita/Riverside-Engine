#include "rseMath.h"

#include <glm/gtc/matrix_transform.hpp>

namespace RSE {
	glm::mat4 rseMath::trsToTransformMatrix(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale){

		glm::mat4 matrix(1.0f);

		matrix = glm::translate(matrix, position);

		// expects radians, if in degrees please remember to use 'glm::radians()'
		matrix = glm::rotate(matrix, rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
		matrix = glm::rotate(matrix, rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
		matrix = glm::rotate(matrix, rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));

		matrix = glm::scale(matrix, scale);

		return matrix;
	}
}