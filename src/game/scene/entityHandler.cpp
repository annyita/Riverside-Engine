#include "entityHandler.h"

#include <util/rseMath.h>
#include <util/UUID.h>

namespace RSE {

	std::vector<entityHandler::entity> entityHandler::entities;


	void entityHandler::create(entity& entity){
		if (entity.uuid == -1) entity.uuid = UUID::returnHandle();

		entities.push_back(entity);
	}

	void entityHandler::remove(int index){
		entities.erase(entities.begin() + index);
	}

	void entityHandler::transform(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale, int index){
		entities[index].transform.position = position;
		entities[index].transform.rotation = rotation;
		entities[index].transform.scale = scale;

		entities[index].transform.matrix = rseMath::trsToTransformMatrix(position, glm::radians(rotation), scale);
	}

}