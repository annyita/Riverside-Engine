#pragma once

#include <glm/glm.hpp>
#include <vector>
#include <memory>
#include <render/object/rseMesh.h>

namespace RSE {

	class entityHandler{
	public:

		struct transformation {
			glm::vec3 position = glm::vec3(0.0f);
			glm::vec3 rotation = glm::vec3(0.0f);
			glm::vec3 scale	   = glm::vec3(1.0f);

			glm::mat4 matrix   = glm::mat4(1.0f);
		};

		struct rendering {
			std::shared_ptr<Mesh> mesh{};
			uint64_t renderid = -1;
		};

		struct entity {
			transformation transform;
			rendering render;
			uint64_t uuid = -1;
		};

		static std::vector<entity> entities;

		static void create(entity& entity);
		static void remove(int index);

		static uint64_t getUUID(int index) {
			return entities[index].uuid;
		};

		static uint64_t getIndex(uint64_t uuid) {
			// i dont wanna do a map so this is fine for now
			for (size_t i = 0; i < entities.size(); i++){
				if (uuid == getUUID(i)) return i;
			}

			return -1;
		}

		static void transform(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale, int index);


	private:

	};

}