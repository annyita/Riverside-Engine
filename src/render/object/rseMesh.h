#ifndef RIVERSIDE_MODEL_CLASS_H
#define RIVERSIDE_MODEL_CLASS_H

#include <render/vkCore/rseDevice.hpp>

#include <vulkan/vulkan.h>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>

#include <vector>

namespace RSE {

	class Mesh {
	public:

		struct vertex {
			glm::vec3 position;
			//glm::vec3 normal;
			glm::vec2 uv;

			static std::vector<VkVertexInputBindingDescription> getBindingDescriptions();
			static std::vector<VkVertexInputAttributeDescription> getAttributeDescriptions();
		};

		Mesh(rseDevice &device, const std::vector<vertex>& vertices);
		~Mesh();

		Mesh(const Mesh&) = delete;
		Mesh&operator=(const Mesh&) = delete;

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