#ifndef RIVERSIDE_MODEL_CLASS_H
#define RIVERSIDE_MODEL_CLASS_H

#include <render/vkCore/rseDevice.hpp>
#include <render/vkCore/rseBuffer.h>

#include <vulkan/vulkan.h>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>

#include <vector>
#include <memory>

namespace RSE {

	class Mesh {
	public:

		struct vertex {
			glm::vec3 position;
			glm::vec3 normal;
			glm::vec3 colour;
			glm::vec2 uv;
			glm::vec3 tangent;
			glm::vec3 biTangent;

			static std::vector<VkVertexInputBindingDescription> getBindingDescriptions();
			static std::vector<VkVertexInputAttributeDescription> getAttributeDescriptions();
		};

		struct Builder {
			std::vector<vertex> vertices{};
			std::vector<uint32_t> indices{};

		};

		Mesh(rseDevice &device, const Builder &builder);
		~Mesh();

		Mesh(const Mesh&) = delete;
		Mesh&operator=(const Mesh&) = delete;

		void bind(VkCommandBuffer commandBuffer);
		void draw(VkCommandBuffer commandBuffer);

		std::string name = "null";

	private:
		void createVertexBuffers(const std::vector<vertex>& vertices);
		void createIndexBuffers(const std::vector<uint32_t>& indices);

		rseDevice& mainDevice;

		std::unique_ptr<rseBuffer> vertexBuffer;
		uint32_t vertexCount;

		bool hasIndexBuffer = false;
		std::unique_ptr<rseBuffer> indexBuffer;
		uint32_t indexCount;
	};
}

#endif