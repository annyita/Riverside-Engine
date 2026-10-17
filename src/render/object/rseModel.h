#ifndef RIVERSIDE_MODEL_CLASS_H
#define RIVERSIDE_MODEL_CLASS_H

#include <render/vkCore/rseDevice.hpp>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>

#include <vector>

namespace RSE {

	class Model {
	public:

		struct vertex {
			glm::vec2 position;
			glm::vec3 colour;
			static std::vector<VkVertexInputBindingDescription> getBindingDescriptions();
			static std::vector<VkVertexInputAttributeDescription> getAttributeDescriptions();
		};

		Model(rseDevice &device, const std::vector<vertex>& vertices);
		~Model();

		Model(const Model&) = delete;
		Model &operator=(const Model&) = delete;

		void bind(VkCommandBuffer commandBuffer);
		void draw(VkCommandBuffer commandBuffer);

	private:
		void createVertexBuffers(const std::vector<vertex>& vertices);

		rseDevice& mainDevice;
		VkBuffer vertexBuffer;
		VkDeviceMemory vertexBufferMemory;
		uint32_t vertexCount;
	};
}

#endif